//============================================
//
// 障害物蹴り敵のメイン処理[kick_enemy.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "model.h"
#include "kick_enemy.h"
#include "manager.h"
#include "game.h"
#include "collision.h"
#include "motion.h"
#include "renderer.h"
#include "useful.h"
#include "debugproc.h"

#include "drum.h"
#include "player.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define WAITING_COUNT		(120)		// 待機状態のカウント

//===========================================
// コンストラクタ
//===========================================
CKickEnemy::CKickEnemy() : CEnemy(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pDrum = nullptr;								// 障害物のポインタ
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 障害物の目的の位置
	m_drumMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 障害物の移動量
	m_action = ACT_WAITING;							// 行動状態
	m_nStateCount = 0;								// 状態カウント
	m_fGravity = 0.0f;								// 重力
	m_bRight = false;								// 右向き状況
}

//===========================================
// デストラクタ
//===========================================
CKickEnemy::~CKickEnemy()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CKickEnemy::Init(void)
{
	// 全ての値をクリアする
	m_pDrum = nullptr;								// 障害物のポインタ
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 障害物の目的の位置
	m_drumMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 障害物の移動量
	m_action = ACT_WAITING;							// 行動状態
	m_nStateCount = 0;								// 状態カウント
	m_fGravity = 0.0f;								// 重力
	m_bRight = false;								// 右向き状況

	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// ベタ打ち
	SetNumModel(14);

	// データの設定処理
	CCharacter::SetData();

	if (GetMotion() == nullptr)
	{ // モーションが NULL だった場合

		// モーションの設定処理
		SetMotion();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 警告文
		MessageBox(NULL, "障害物キック敵のモーションのポインタが使われています！", "警告！", MB_ICONWARNING);
	}

	if (GetMotion() != nullptr)
	{ // ポインタが NULL じゃない場合

		// モーションの情報を取得する
		GetMotion()->SetModel(GetHierarchy(), GetNumModel());

		// ロード処理
		GetMotion()->Load(CMotion::TYPE_PLAYER);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 警告文
		MessageBox(NULL, "障害物キック敵のモーションのメモリが確保できませんでした！", "警告！", MB_ICONWARNING);
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CKickEnemy::Uninit(void)
{
	// 終了
	CEnemy::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CKickEnemy::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 前回の位置を設定する
	SetPosOld(pos);

	// 障害物の捜索処理
	if (SearchObstacle() == false)
	{ // ポインタが一致しなかった場合

		// ポインタを NULL にする
		m_pDrum = nullptr;
	}

	switch (m_action)
	{
	case ACT_WAITING:		// 待機状態

		// 振り向き処理
		Turn();

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % WAITING_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 構え状態にする
			m_action = ACT_BRING;

			// 障害物の目的の向きを設定する
			m_posDest = D3DXVECTOR3
			(
				pos.x + sinf(GetRot().y) * 50.0f,
				pos.y,
				0.0f
			);

			// 移動量を設定する
			m_drumMove = (m_posDest - m_pDrum->GetPos()) * 0.05f;
		}

		break;

	case ACT_BRING:			// 構え状態

		if (m_pDrum != nullptr)
		{ // 障害物が存在した場合

			// ローカル変数宣言
			D3DXVECTOR3 ObstaPos;		// 障害物の位置

			// 障害物の位置を取得する
			ObstaPos = m_pDrum->GetPos();

			// 障害物の位置を移動する
			ObstaPos.x += m_drumMove.x;
			ObstaPos.z += m_drumMove.z;

			if (ObstaPos.z <= 0.0f)
			{ // Z軸が 0.0f 以上になった場合

				// 障害物の位置を設定する
				ObstaPos.x = m_posDest.x;
				ObstaPos.z = m_posDest.z;

				// 状態カウントを加算する
				m_nStateCount++;
			}

			// 障害物の位置を設定する
			m_pDrum->SetPos(ObstaPos);

			if (m_nStateCount >= 90)
			{ // 状態カウントが一定数以上になった場合

				// シュート状態にする
				m_action = ACT_SHOOT;

				if (m_bRight == true)
				{ // 右を向いていた場合

					// ヒット処理
					m_pDrum->Hit();
				}
				else
				{ // 左を向いていた場合

					// ヒット処理
					m_pDrum->HitLeft();
				}

				// 障害物のポインタを NULL にする
				m_pDrum = nullptr;
			}
		}

		break;

	case ACT_SHOOT:			// シュート状態

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % 50 == 0)
		{ // 状態カウントが一定数になった場合

			// 障害物の生成処理
			m_pDrum = dynamic_cast<CDrum*>(CObstacle::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 1.0f, 100.0f), CObstacle::TYPE::TYPE_DRUM));

			// アクション判定の設定処理
			m_pDrum->SetCollision(false);

			// アクション判定の設定処理
			m_pDrum->SetAttack(false);

			// 待機状態にする
			m_action = ACT_WAITING;
		}

		break;
	}

	// 当たり判定処理
	Collision();

	// モーションの更新処理
	GetMotion()->Update();

	CManager::GetDebugProc()->Print("障害物のポインタ：%s\n", m_pDrum);
}

//===========================================
// 描画処理
//===========================================
void CKickEnemy::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//===========================================
// 情報の設定処理
//===========================================
void CKickEnemy::SetData(const D3DXVECTOR3& pos)
{
	// 全ての値を初期化する
	SetPos(pos);			// 位置
	SetPosOld(pos);			// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));			// 向き
	SetScale(NONE_SCALE);	// 拡大率

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// 初期化処理
		GetHierarchy(nCntData)->SetPos(pos);								// 位置
		GetHierarchy(nCntData)->SetPosOld(pos);								// 前回の位置
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);						// 拡大率
		GetHierarchy(nCntData)->SetFileData(CXFile::TYPE(INIT_PLAYER + nCntData));		// データの設定処理
	}

	// モーションの設定処理
	GetMotion()->Set(MOTIONTYPE_NEUTRAL);

	// 障害物の生成処理
	m_pDrum = dynamic_cast<CDrum*>(CObstacle::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 1.0f, 100.0f), CObstacle::TYPE::TYPE_DRUM));

	// アクション判定の設定処理
	m_pDrum->SetCollision(false);

	// アクション判定の設定処理
	m_pDrum->SetAttack(false);
}

//===========================================
// 障害物の捜索処理
//===========================================
bool CKickEnemy::SearchObstacle(void)
{
	if (m_pDrum != nullptr)
	{ // 障害物のポインタが NULL じゃない場合

		// ローカル変数宣言
		CObject* pObj = nullptr;		// オブジェクトへのポインタ
		CObject* pObjNext = nullptr;	// 次のオブジェクトへのポインタ

		// オブジェクトの情報を取得する
		pObj = CObject::GetTop(PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // オブジェクトへのポインタが NULL じゃない限り回す

			// 次のオブジェクトを代入する
			pObjNext = pObj->GetNext();

			if (pObj->GetType() == TYPE_OBSTACLE &&
				pObj == m_pDrum)
			{ // オブジェクトの種類がアイテムかつ、アドレスが一致した場合

				// true を返す
				return true;
			}

			// 次のオブジェクトを代入する
			pObj = pObjNext;
		}

		// false を返す
		return false;
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//===========================================
// 振り向き処理
//===========================================
void CKickEnemy::Turn(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// ローカルポインタ宣言
	CPlayer* pPlayer = CScene::GetPlayer();		// プレイヤーの情報を取得する

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		if (pPlayer->GetPos().x > pos.x)
		{ // プレイヤーが敵より右にいた場合

			// 右向きにする
			m_bRight = true;

			// 向きを設定する
			rot.y = D3DX_PI * 0.5f;
		}
		else if (pPlayer->GetPos().x < pos.x)
		{ // プレイヤーが敵より左にいた場合

			// 左向きにする
			m_bRight = false;

			// 向きを設定する
			rot.y = -D3DX_PI * 0.5f;
		}
	}

	// 向きを設定する
	SetRot(rot);
}

//===========================================
// 当たり判定処理
//===========================================
void CKickEnemy::Collision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 重力処理
	Gravity(&m_fGravity, pos, ENEMY_GRAVITY);

	// 位置を設定する
	SetPos(pos);

	if (collision::EnemyBlockCollision(this) == true)
	{ // ブロックとの当たり判定が true の場合

		// 重力を 0.0f にする
		m_fGravity = 0.0f;
	}
}