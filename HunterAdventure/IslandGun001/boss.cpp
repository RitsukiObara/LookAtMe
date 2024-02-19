//============================================================
//
// ボス処理 [boss.cpp]
// Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "model.h"
#include "boss.h"
#include "manager.h"
#include "file.h"
#include "useful.h"

#include "motion.h"
#include "boss_appearstate.h"
#include "boss_destroystate.h"
#include "boss_stunstate.h"
#include "boss_lifeUI.h"
#include "objectElevation.h"
#include "boss_collision.h"
#include "fraction.h"

//------------------------------------------------------------
// 無名名前空間
//------------------------------------------------------------
namespace
{
	const float LAND_GRAVITY = -50.0f;			// 着地時の重力
	const int MAX_LIFE = 3000;					// 体力の最大数
	const int BARRIER_FRAC_MOVE_WIDTH = 40;		// バリア攻撃時の破片の横の移動量
	const int BARRIER_FRAC_MOVE_HEIGHT = 20;	// バリア攻撃時の破片の縦の移動量
	const int BARRIER_FRAC_MOVE_MIN = 10;		// バリア攻撃時の破片の移動量の最低値
	const int NUM_BARRIER_FRAC = 5;				// バリア攻撃時の破片の数
	const int BARRIER_FRAC_LIFE = 100;			// バリア攻撃時の破片の寿命
	const int WEAK_LIFE[CBoss::WEAK_MAX] =		// 弱点のライフ
	{
		40,
		10,
		10,
		10,
		10
	};
	const int WEAK_PART[CBoss::WEAK_MAX] =		// 弱点のパーツ番号
	{
		2,
		5,
		6,
		13,
		14
	};
	const int WEAK_MATERIAL_NUM = 0;			// 弱点のマテリアル番号
}

// 静的メンバ変数
CListManager<CBoss*> CBoss::m_list = {};				// リスト

//================================
// コンストラクタ
//================================
CBoss::CBoss() : CCharacter(CObject::TYPE_BOSS, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pMotion = nullptr;			// モーション
	m_pState = nullptr;				// 状態の情報
	m_pLifeUI = nullptr;			// 体力UIの情報
	memset(m_apColl, 0, sizeof(m_apColl));			// 当たり判定の球
	memset(m_apMatCopy, 0, sizeof(m_apMatCopy));	// マテリアルのコピー
	for (int nCnt = 0; nCnt < WEAK_MAX; nCnt++)
	{
		m_aWeakPointLife[nCnt] = WEAK_LIFE[nCnt];	// 弱点のライフ
	}
	m_nLife = MAX_LIFE;				// 体力
	m_bDown = false;				// ダウン状況

	// リストに追加する
	m_list.Regist(this);
}

//================================
// デストラクタ
//================================
CBoss::~CBoss()
{

}

//================================
// 初期化処理
//================================
HRESULT CBoss::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// モデルの総数を設定
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_BOSS));

	// データの設定処理
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // モーションが NULL だった場合

		if (m_pMotion == nullptr)
		{ // モーションが NULL の場合

			// モーションを生成する
			m_pMotion = CMotion::Create();
		}
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pMotion != nullptr)
	{ // ポインタが NULL じゃない場合

		// モーションの情報を取得する
		m_pMotion->SetInfo(CMotion::STYLE_BOSS, GetHierarchy(), GetNumModel());
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	// 値を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CBoss::Uninit(void)
{
	if (m_pMotion != nullptr)
	{ // モーションが NULL じゃない場合

		// モーションの終了処理
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pState != nullptr)
	{ // 状態が NULL じゃない場合

		// 状態の終了処理
		m_pState->Delete();
		m_pState = nullptr;
	}

	if (m_pLifeUI != nullptr)
	{ // 体力UIが NULL じゃない場合

		// 体力UIを NULL にする
		m_pLifeUI = nullptr;
	}

	for (int nCntColl = 0; nCntColl < GetNumModel(); nCntColl++)
	{
		if (m_apColl[nCntColl] != nullptr)
		{ // 当たり判定の情報が NULL じゃない場合

			// 終了処理
			m_apColl[nCntColl]->Uninit();
			m_apColl[nCntColl] = nullptr;
		}
	}

	for (int nCntMat = 0; nCntMat < MAX_PARTS; nCntMat++)
	{
		if (m_apMatCopy[nCntMat] != nullptr)
		{ // マテリアルのコピーが NULL じゃない場合

			// マテリアルの終了処理
			delete[] m_apMatCopy[nCntMat];
			m_apMatCopy[nCntMat] = nullptr;
		}
	}

	// 終了処理
	CCharacter::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//================================
// 更新処理
//================================
void CBoss::Update(void)
{
	if (m_pState != nullptr)
	{ // 状態が NULL じゃない場合

		// 状態処理
		m_pState->Process(this);
	}

	if (m_pMotion != nullptr)
	{ // モーションが NULL じゃない場合

		// モーションの更新処理
		m_pMotion->Update();
	}
}

//================================
// 描画処理
//================================
void CBoss::Draw(void)
{
	// 描画処理
	CCharacter::Draw(m_apMatCopy);
}

//================================
// ヒット処理
//================================
void CBoss::Hit(const int nDamage)
{
	if (m_nLife > 0)
	{ // 体力がまだある場合

		// ダメージを受ける
		m_nLife -= nDamage;

		// 寿命を適用する
		m_pLifeUI->SetLife(m_nLife);

		if (m_nLife <= 0)
		{ // 体力が無くなった場合

			// 状態の消去処理(メモリリークが起きる可能性があるため)
			m_pState->Delete();
			m_pState = nullptr;

			// 死亡状態にする
			ChangeState(new CBossDestroyState);
		}
	}
}

//================================
// バリア破壊処理
//================================
void CBoss::BarrierBreak(const D3DXVECTOR3& pos, const int nPart)
{
	for (int nCntPart = 0; nCntPart < WEAK_MAX; nCntPart++)
	{
		if (m_aWeakPointLife[nCntPart] > 0 &&
			nPart == WEAK_PART[nCntPart])
		{
			// バリアのヒット処理
			BarrierHit(pos, nPart, nCntPart);

			// for文から抜ける
			break;
		}
	}

	if (m_bDown == false &&
		m_aWeakPointLife[WEAK_RFSHIN] <= 0 &&
		m_aWeakPointLife[WEAK_LFSHIN] <= 0 &&
		m_aWeakPointLife[WEAK_RBSHIN] <= 0 &&
		m_aWeakPointLife[WEAK_LBSHIN] <= 0)
	{ // 弱点が全て壊された場合

		// 状態の消去処理(メモリリークが起きる可能性があるため)
		m_pState->Delete();
		m_pState = nullptr;

		// 気絶状態にする
		ChangeState(new CBossStunState);

		// 気絶状態にする
		m_bDown = true;
	}
}

//================================
// バリアのヒット処理
//================================
void CBoss::BarrierHit(const D3DXVECTOR3& pos, const int nPart, const int nCntPart)
{
	// 移動量
	int nMoveWidth;
	int nMoveHeight;

	for (int nCnt = 0; nCnt < NUM_BARRIER_FRAC; nCnt++)
	{
		// 移動量をランダムで算出
		nMoveWidth = (rand() % BARRIER_FRAC_MOVE_WIDTH + BARRIER_FRAC_MOVE_MIN);
		nMoveHeight = (rand() % BARRIER_FRAC_MOVE_HEIGHT + BARRIER_FRAC_MOVE_MIN);

		// 破片の生成
		CFraction::Create(pos, CFraction::TYPE::TYPE_BOSSROCK, BARRIER_FRAC_LIFE, nMoveWidth, nMoveHeight);
	}

	// 寿命を減算する
	m_aWeakPointLife[nCntPart]--;

	if (m_aWeakPointLife[nCntPart] <= 0)
	{ // ライフが無くなった場合

		// 色を設定する
		m_apMatCopy[nPart][WEAK_MATERIAL_NUM].MatD3D.Diffuse = D3DXCOLOR(0.1f, 0.0f, 0.1f, 1.0f);
		m_apMatCopy[nPart][WEAK_MATERIAL_NUM].MatD3D.Ambient = D3DXCOLOR(0.1f, 0.0f, 0.1f, 1.0f);
		m_apMatCopy[nPart][WEAK_MATERIAL_NUM].MatD3D.Emissive = D3DXCOLOR(0.1f, 0.0f, 0.1f, 1.0f);
	}
}

//================================
// 気絶回復処理
//================================
void CBoss::StunRecovery(void)
{
	int nNumMat = 0;

	// 気絶を直す
	m_bDown = false;

	for (int nCnt = WEAK_RFSHIN; nCnt < WEAK_MAX; nCnt++)
	{
		// 体力を回復する
		m_aWeakPointLife[nCnt] = WEAK_LIFE[nCnt];

		// マテリアルの総数を取得
		nNumMat = (int)GetHierarchy(WEAK_PART[nCnt])->GetFileData().dwNumMat;

		// マテリアルのコピーを取得
		m_apMatCopy[WEAK_PART[nCnt]][WEAK_MATERIAL_NUM] = GetHierarchy(WEAK_PART[nCnt])->GetMaterial(WEAK_MATERIAL_NUM);
	}
}

//================================
// 情報の設定処理
//================================
void CBoss::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	int nNumMat;

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

		// マテリアルの数を取得する
		nNumMat = (int)GetHierarchy(nCntData)->GetFileData().dwNumMat;

		// マテリアルのコピーを生成する
		m_apMatCopy[nCntData] = new D3DXMATERIAL[nNumMat];

		for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
		{
			// マテリアルのコピーを取得
			m_apMatCopy[nCntData][nCntMat] = GetHierarchy(nCntData)->GetMaterial(nCntMat);
		}
	}

	// モーションのリセット処理
	m_pMotion->ResetMotion(MOTIONTYPE_FLY);

	// 状態の切り替え処理
	ChangeState(new CBossAppearState);

	// 全ての値を設定する
	m_pLifeUI = CBossLifeUI::Create(MAX_LIFE);		// 体力UI
	m_nLife = MAX_LIFE;		// 体力
	m_bDown = false;		// ダウン状況

	// 当たり判定を設定する
	CManager::Get()->GetFile()->SetBossColl(&m_apColl[0]);
}

//===========================================
// 生成処理
//===========================================
CBoss* CBoss::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	if (m_list.IsEmpty() == false)
	{ // 空白状態の場合

		// 生成しない(応急処置)
		return nullptr;
	}

	// ボスのインスタンス
	CBoss* pBoss = nullptr;

	if (pBoss == nullptr)
	{ // オブジェクトが NULL の場合

		// ボスを生成する
		pBoss = new CBoss;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pBoss;
	}

	if (pBoss != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBoss->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBoss->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// ボスのポインタを返す
	return pBoss;
}

//===========================================
// リストの取得処理
//===========================================
CListManager<CBoss*> CBoss::GetList(void)
{
	// リストの情報を返す
	return m_list;
}

//===========================================
// 起伏地面との当たり判定
//===========================================
bool CBoss::ElevationCollision(void)
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

				// 位置を更新する
				SetPos(pos);

				// true を返す
				return true;
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

	// false を返す
	return false;
}

//===========================================
// 状態の遷移処理
//===========================================
void CBoss::ChangeState(CBossState* pNext)
{
	if (m_pState != nullptr)
	{ // 状態が NULL じゃない場合

		// 状態の終了処理
		m_pState->Uninit();
		m_pState = nullptr;
	}

	// 次の状態を設定する
	m_pState = pNext;

	if (m_pState != nullptr)
	{ // 状態が NULL じゃない場合

		// 情報の設定処理
		m_pState->SetData(this);
	}
}

//===========================================
// モーションの取得処理
//===========================================
CMotion* CBoss::GetMotion(void)
{
	// モーションの情報を返す
	return m_pMotion;
}

//===========================================
// 当たり判定の取得処理
//===========================================
CBossCollision* CBoss::GetColl(const int nIdx)
{
	// 当たり判定の情報を返す
	return m_apColl[nIdx];
}