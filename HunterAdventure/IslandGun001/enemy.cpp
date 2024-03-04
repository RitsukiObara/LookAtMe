//============================================================
//
// 敵処理 [enemy.cpp]
// Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "enemy.h"
#include "manager.h"
#include "area.h"
#include "sound.h"
#include "useful.h"

#include "motion.h"
#include "game.h"
#include "game_score.h"
#include "addscoreUI.h"
#include "rock.h"
#include "block.h"
#include "collision.h"
#include "anim_reaction.h"
#include "objectElevation.h"

#include "tordle.h"
#include "iwakari.h"

//------------------------------------------------------------
// 無名名前空間
//------------------------------------------------------------
namespace
{
	const D3DXVECTOR3 COLLSIZE[CEnemy::TYPE_MAX] =		// 当たり判定のサイズ
	{
		D3DXVECTOR3(85.0f,85.0f,85.0f),
		D3DXVECTOR3(120.0f,230.0f,120.0f),
	};
	const int LIFE[CEnemy::TYPE_MAX] =					// 体力
	{
		50,
		5,
	};
	const int SCORE[CEnemy::TYPE_MAX] =					// スコア
	{
		500,
		1000,
	};
	const CAddScoreUI::TYPE ADDSCORE_TYPE[CEnemy::TYPE_MAX] =		// 追加スコアの種類
	{
		CAddScoreUI::TYPE_TORDLE,
		CAddScoreUI::TYPE_IWAKARI,
	};
	const D3DXVECTOR3 DEATH_EXPLOSION = D3DXVECTOR3(200.0f, 200.0f, 0.0f);		// 死亡時の爆発
	const D3DXCOLOR DAMAGE_COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);				// ダメージ状態の色
	const int DAMAGE_COUNT = 12;						// ダメージ状態のカウント
	const float LAND_GRAVITY = -50.0f;					// 起伏地面に着地している時の重力
	const int SLASHRIPPLE_DAMAGE = 2;					// 斬撃波紋に当たった時のダメージ
	const float SLASHRIPPLE_KNOCKBACK = 20.0f;			// 斬撃波紋に当たった時の吹き飛び
	const int WIND_SHOT_DAMAGE = 10;					// 風攻撃に当たった時のダメージ
	const float WIND_SHOT_KNOCKBACK = 50.0f;			// 風攻撃に当たった時の吹き飛び
}

// 静的メンバ変数
CListManager<CEnemy*> CEnemy::m_list = {};				// リスト

//================================
// コンストラクタ
//================================
CEnemy::CEnemy() : CCharacter(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pMotion = nullptr;			// モーション

	m_type = TYPE_TORDLE;			// 種類
	m_state = STATE_NONE;			// 状態
	m_collSize = NONE_D3DXVECTOR3;	// 当たり判定のサイズ
	m_nLife = 0;					// 体力
	m_nStateCount = 0;				// 状態カウント
	m_fGravity = 0.0f;				// 重力
	m_nFieldIdx = 0;				// 区分の番号

	// リストに追加する
	m_list.Regist(this);
}

//================================
// デストラクタ
//================================
CEnemy::~CEnemy()
{

}

//================================
// 終了処理
//================================
void CEnemy::Uninit(void)
{
	// モーションの終了処理
	delete m_pMotion;
	m_pMotion = nullptr;

	// 終了処理
	CCharacter::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//================================
// 更新処理
//================================
void CEnemy::Update(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 重力処理
	useful::Gravity(&m_fGravity, &pos.y, 0.5f);

	// 位置を適用する
	SetPos(pos);

	// 区分の番号設定処理
	m_nFieldIdx = area::SetFieldIdx(GetPos());

	// モーションの更新処理
	m_pMotion->Update();

	// 斬撃波紋との当たり判定
	SlashRippleHit();

	// 起伏地面との当たり判定
	ElevationCollision();

	// 当たり判定処理
	Collision();

	// 祭壇との当たり判定
	AlterCollision();

	// 敵同士の当たり判定
	collision::EnemyHitToEnemy(this);

	switch (m_state)
	{
	case CEnemy::STATE_NONE:

		if (collision::WindShotHit(GetPos(), m_collSize.x, m_collSize.y) == true ||
			collision::FireShotHit(GetPos(), m_collSize.x, m_collSize.y) == true)
		{ // 当たった場合

			// ヒット処理
			Hit(WIND_SHOT_DAMAGE, WIND_SHOT_KNOCKBACK);
		}

		break;

	case CEnemy::STATE_DAMAGE:

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % DAMAGE_COUNT == 0)
		{ // カウントが一定数に達した場合

			// 状態カウントを0にする
			m_nStateCount = 0;

			// 通常カウントにする
			m_state = STATE_NONE;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// ステージの当たり判定
	StageCollision();
}

//================================
// 描画処理
//================================
void CEnemy::Draw(void)
{
	switch (m_state)
	{
	case CEnemy::STATE_NONE:

		// 描画処理
		CCharacter::Draw();

		break;

	case CEnemy::STATE_DAMAGE:

		// 描画処理
		CCharacter::Draw(DAMAGE_COL);
		
		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//================================
// 情報の設定処理
//================================
void CEnemy::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// 全ての値を初期化する
	SetPos(pos);				// 位置
	SetPosOld(GetPos());		// 前回の位置
	SetRot(rot);				// 向き
	SetScale(NONE_SCALE);		// 拡大率

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// 初期化処理
		GetHierarchy(nCntData)->SetPos(pos);										// 位置
		GetHierarchy(nCntData)->SetPosOld(pos);										// 前回の位置
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);								// 拡大率
	}

	// モーションのリセット処理
	m_pMotion->ResetMotion(MOTIONTYPE_NEUTRAL);

	// 全ての値を設定する
	m_type = type;					// 種類
	m_state = STATE_NONE;			// 状態
	m_collSize = COLLSIZE[m_type];	// 当たり判定のサイズ
	m_nLife = LIFE[m_type];			// 体力
	m_nStateCount = 0;				// 状態カウント
	m_fGravity = 0.0f;				// 重力

	// 区分の番号設定処理
	m_nFieldIdx = area::SetFieldIdx(GetPos());
}

//===========================================
// 死亡時の処理
//===========================================
void CEnemy::Death(void)
{
	if (m_nLife <= 0)
	{ // 体力が0以下になった場合

		// 位置を取得する
		D3DXVECTOR3 pos = GetPos();

		// 位置を真ん中にする
		pos.y += (m_collSize.y * 0.5f);

		// アニメーションリアクションを生成
		CAnimReaction::Create(pos, DEATH_EXPLOSION, NONE_D3DXCOLOR, CAnimReaction::TYPE::TYPE_GUNEXPLOSION, 4, 1);

		// ダメージ音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);

		if (CGame::GetGameScore() != nullptr)
		{ // ゲームスコアが NULL じゃない場合

			// スコアを加算する
			CGame::GetGameScore()->AddScore(SCORE[m_type]);

			// 追加スコアUIを生成
			CAddScoreUI::Create(pos + m_collSize, ADDSCORE_TYPE[m_type]);
		}

		// 終了処理
		Uninit();
	}
}

//===========================================
// 生成処理
//===========================================
CEnemy* CEnemy::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// 敵のインスタンスを生成
	CEnemy* pEnemy = nullptr;

	if (pEnemy == nullptr)
	{ // オブジェクトが NULL の場合

		switch (type)
		{
		case CEnemy::TYPE_TORDLE:		// タードル

			// タードルを生成する
			pEnemy = new CTordle;

			break;

		case CEnemy::TYPE_IWAKARI:		// イワカリ

			// イワカリを生成する
			pEnemy = new CIwakari;

			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pEnemy;
	}

	if (pEnemy != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pEnemy->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEnemy->SetData(pos, rot, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 敵のポインタを返す
	return pEnemy;
}

//===========================================
// モーションの取得処理
//===========================================
CMotion* CEnemy::GetMotion(void) const
{
	// モーションの情報を返す
	return m_pMotion;
}

//===========================================
// モーションの生成処理
//===========================================
void CEnemy::CreateMotion(void)
{
	if (m_pMotion == nullptr)
	{ // モーションが NULL の場合

		// モーションを生成する
		m_pMotion = CMotion::Create();
	}
}

//===========================================
// 状態の設定処理
//===========================================
void CEnemy::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//===========================================
// 状態の取得処理
//===========================================
CEnemy::STATE CEnemy::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//===========================================
// 当たり判定のサイズの設定処理
//===========================================
void CEnemy::SetCollSize(const D3DXVECTOR3& size)
{
	// 当たり判定のサイズを設定する
	m_collSize = size;
}

//===========================================
// 当たり判定のサイズの取得処理
//===========================================
D3DXVECTOR3 CEnemy::GetCollSize(void) const
{
	// 当たり判定のサイズを取得する
	return m_collSize;
}

//===========================================
// 体力の設定処理
//===========================================
void CEnemy::SetLife(const int nLife)
{
	// 体力を設定する
	m_nLife = nLife;
}

//===========================================
// 体力の取得処理
//===========================================
int CEnemy::GetLife(void) const
{
	// 体力を返す
	return m_nLife;
}

//===========================================
// 重力の設定処理
//===========================================
void CEnemy::SetGravity(const float fGravity)
{
	// 重力を設定する
	m_fGravity = fGravity;
}

//===========================================
// 重力の取得処理
//===========================================
float CEnemy::GetGravity(void) const
{
	// 重力を返す
	return m_fGravity;
}

//===========================================
// 種類の取得処理
//===========================================
CEnemy::TYPE CEnemy::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//===========================================
// リストの取得処理
//===========================================
CListManager<CEnemy*> CEnemy::GetList(void)
{
	// リストの情報を返す
	return m_list;
}

//===========================================
// 起伏地面との当たり判定
//===========================================
void CEnemy::ElevationCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;	// 先頭の小判
	CElevation* pElevEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pElev = list.GetTop();

		// 末尾の値を取得する
		pElevEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 当たり判定を取る
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // 当たり判定の位置が高かった場合

				// 高さを設定する
				pos.y = fHeight;

				// 重力を設定する
				m_fGravity = LAND_GRAVITY;
			}

			if (pElev == pElevEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pElev = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// 位置を更新する
	SetPos(pos);
}

//===========================================
// 当たり判定処理
//===========================================
void CEnemy::Collision(void)
{
	D3DXVECTOR3 pos = GetPos();								// 位置
	D3DXVECTOR3 posOld = GetPosOld();						// 前回の位置
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(m_collSize);	// 頂点の最小値
	int nIdx = 0;

	for (int nCnt = 0; nCnt < area::NUM_COLL; nCnt++)
	{
		nIdx = m_nFieldIdx + area::COLL_ADD_IDX[nCnt];

		if (area::IndexCheck(nIdx) == true)
		{ // 区分内の場合

			// 木との当たり判定
			collision::TreeCollision(&pos, m_collSize.x, nIdx);

			// ブロックとの当たり判定
			BlockCollision(&pos, posOld, m_collSize, vtxMin, nIdx);

			// 岩との当たり判定
			collision::RockCollision(&pos, posOld, m_collSize.x, m_collSize.y, nIdx, &m_fGravity);

			// 壁との当たり判定
			collision::WallCollision(&pos, posOld, m_collSize, vtxMin, nIdx);
		}
	}

	// 位置を適用する
	SetPos(pos);
}

//===========================================
// ブロックとの当たり判定
//===========================================
void CEnemy::BlockCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx)
{
	// ローカル変数宣言
	collision::SCollision coll = { false,false,false,false,false,false };		// 当たり判定の変数
	CListManager<CBlock*> list = CBlock::GetList(nAreaIdx);
	CBlock* pBlock = nullptr;		// 先頭の値
	CBlock* pBlockEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBlock = list.GetTop();

		// 末尾の値を取得する
		pBlockEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 六面体の当たり判定
			coll = collision::HexahedronClush
			(
				pos,
				pBlock->GetPos(),
				posOld,
				pBlock->GetPosOld(),
				vtxMin,
				pBlock->GetVtxMin(),
				vtxMax,
				pBlock->GetVtxMax()
			);

			if (pBlock == pBlockEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBlock = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

//===========================================
// 祭壇との当たり判定
//===========================================
void CEnemy::AlterCollision(void)
{
	D3DXVECTOR3 pos = GetPos();			// 位置
	D3DXVECTOR3 posOld = GetPosOld();	// 前回の位置
	D3DXVECTOR3 vtxMax = m_collSize;	// 最大値
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(m_collSize);	// 最小値

	// 壁との当たり判定
	collision::AlterCollision(&pos, posOld, vtxMax, vtxMin);

	// 位置を適用する
	SetPos(pos);
}

//===========================================
// 斬撃の波紋との当たり判定
//===========================================
void CEnemy::SlashRippleHit(void)
{
	// 変数を設定
	D3DXVECTOR3 pos = GetPos();
	float fRadius = m_collSize.x;
	float fHeight = m_collSize.y;

	if (m_state == STATE_NONE &&
		collision::RippleHit(pos, fRadius, fHeight) == true)
	{ // 斬撃波紋に当たった場合

		// ヒット処理
		Hit(SLASHRIPPLE_DAMAGE, SLASHRIPPLE_KNOCKBACK);
	}
}

//===========================================
// ステージの当たり判定
//===========================================
void CEnemy::StageCollision(void)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	// ステージの当たり判定
	collision::StageCollision(&pos, m_collSize.x);

	// 位置を適用
	SetPos(pos);
}