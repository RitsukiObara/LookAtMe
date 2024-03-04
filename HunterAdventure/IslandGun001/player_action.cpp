//===========================================
//
// プレイヤーの行動のメイン処理[player_action.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "player_action.h"
#include "renderer.h"

#include "player.h"
#include "handgun.h"
#include "dagger.h"
#include "player_controller.h"
#include "slash_ripple.h"
#include "chara_blur.h"
#include "collision.h"
#include "bulletUI.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const float DODGE_SPEED = 20.0f;			// 回避状態の速度
	const int DODGE_COUNT = 27;					// 回避状態のカウント数
	const int DAGGER_COUNT = 35;				// ダガー状態のカウント数
	const int SWOOP_COUNT = 50;					// 急降下状態のカウント数

	const float SWOOP_RIPPLE_HEIGHT = 50.0f;	// 急降下状態の波紋の出る高さ

	const int DODGE_BLUR_LIFE = 10;				// 回避状態のブラーの寿命
	const D3DXCOLOR DODGE_BLUR_COL = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);		// 回避状態のブラーの色

	const float ATTACK_DAGGER_HEIGHT = 150.0f;	// ダガー攻撃時の高さ
	const float ATTACK_DAGGER_RADIUS = 240.0f;	// ダガー攻撃時の半径
	const int DAGGER_BOSS_ATTACK = 100;			// ボスへのダガーの攻撃力
	const float DAGGER_MOVE = 10.0f;			// ダガー状態の移動量
	const int DAGGER_MOVE_COUNT = 25;			// ダガー状態で動くカウント数
	const int DAGGER_ATTACK_START = 8;			// ダガーの攻撃判定が始まるカウント数
	const int DAGGER_ATTACK_END = 28;			// ダガーの攻撃判定が終わるカウント数
}

//=========================
// コンストラクタ
//=========================
CPlayerAction::CPlayerAction()
{
	// 全ての値をクリアする
	m_moveRecoil = NONE_D3DXVECTOR3;	// 反動の移動量
	m_action = ACTION_NONE;				// 行動
	m_nActionCount = 0;					// 行動カウント
	m_nDodgeInterval = 0;				// 回避のインターバルカウント
	m_fDodgeRot = 0.0f;					// 回避する向き
	m_bDodgeUse = true;					// 回避使用可能状況
	m_bRipple = false;					// 波紋状況
	m_bRecoil = false;					// 反動状況
	m_bBossAttack = false;				// ボスへの当たり判定状況
}

//=========================
// デストラクタ
//=========================
CPlayerAction::~CPlayerAction()
{

}

//=========================
// 行動の初期化処理
//=========================
HRESULT CPlayerAction::Init(void)
{
	// 全ての値を初期化する
	m_moveRecoil = NONE_D3DXVECTOR3;	// 反動の移動量
	m_action = ACTION_NONE;				// 行動
	m_nActionCount = 0;					// 行動カウント
	m_nDodgeInterval = 0;				// 回避のインターバルカウント
	m_fDodgeRot = 0.0f;					// 回避する向き
	m_bDodgeUse = true;					// 回避使用可能状況
	m_bRipple = false;					// 波紋状況
	m_bRecoil = false;					// 反動状況
	m_bBossAttack = false;				// ボスへの当たり判定状況

	// 成功を返す
	return S_OK;
}

//=========================
// 行動の終了処理
//=========================
void CPlayerAction::Uninit(void)
{
	// 自身を消去する
	delete this;
}

//=========================
// 行動の更新処理
//=========================
void CPlayerAction::Update(CPlayer* pPlayer)
{
	switch (m_action)
	{
	case CPlayerAction::ACTION_NONE:	// 通常状態

		// 通常状態処理
		NoneProcess(pPlayer);

		break;

	case CPlayerAction::ACTION_SHOT:	// 射撃状態

		// 射撃状態
		ShotProcess(pPlayer);

		break;

	case CPlayerAction::ACTION_DAGGER:	// ダガー状態

		// ダガー状態処理
		DaggerPrecess(pPlayer);

		break;

	case CPlayerAction::ACTION_DODGE:	// 回避状態

		// 回避状態処理
		DodgeProcess(pPlayer);

		break;

	case CPlayerAction::ACTION_SHOTGUN:	// 散弾(J+銃)状態

		// 散弾状態処理
		ShotgunProcess(pPlayer);

		break;

	case CPlayerAction::ACTION_SWOOP:	// 急降下(J+ダガー)状態

		// 急降下状態処理
		SwoopProcess(pPlayer);

		break;

	case CPlayerAction::ACTION_RELOAD:	// リロード状態

		// リロード処理
		ReloadProcess(pPlayer);

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	if (m_nDodgeInterval > 0)
	{ // 回避インターバルカウントが0超過の場合

		// 回避インターバルを減算する
		m_nDodgeInterval--;

		if (m_nDodgeInterval <= 0)
		{ // 回避インターバルが0以上になった場合

			// 回避のインターバルカウントを0にする
			m_nDodgeInterval = 0;

			// 回避を使用できる
			m_bDodgeUse = true;
		}
	}
}

//=========================
// 生成処理
//=========================
CPlayerAction* CPlayerAction::Create(void)
{
	// ローカルオブジェクトを生成
	CPlayerAction* pAction = nullptr;		// 行動のインスタンスを生成

	if (pAction == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pAction = new CPlayerAction;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pAction != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pAction->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 行動のポインタを返す
	return pAction;
}

//=========================
// 移動量の設定処理
//=========================
void CPlayerAction::SetMoveRecoil(const D3DXVECTOR3& move)
{
	// 反動の移動量を設定する
	m_moveRecoil = move;
}

//=========================
// 移動量の取得処理
//=========================
D3DXVECTOR3 CPlayerAction::GetMoveRecoil(void) const
{
	// 反動の移動量を返す
	return m_moveRecoil;
}

//=========================
// 行動の設定処理
//=========================
void CPlayerAction::SetAction(const ACTION action)
{
	// 行動を設定する
	m_action = action;

	// 行動カウントをリセットする
	m_nActionCount = 0;

	// 波紋状況を false にする
	m_bRipple = false;

	// ボスの攻撃状況を false にする
	m_bBossAttack = false;
}

//=========================
// 行動の取得処理
//=========================
CPlayerAction::ACTION CPlayerAction::GetAction(void) const
{
	// 行動を返す
	return m_action;
}

//=========================
// 回避する向きの設定処理
//=========================
void CPlayerAction::SetDodgeRot(const float fRot)
{
	// 回避する向きを設定する
	m_fDodgeRot = fRot;
}

//=========================
// 回避インターバルの設定処理
//=========================
void CPlayerAction::SetDodgeInterval(const int nInterval)
{
	// 回避使用可能状況を設定する
	m_nDodgeInterval = nInterval;
}

//=========================
// 回避使用可能状態の設定処理
//=========================
void CPlayerAction::SetEnableDodgeUse(const bool bUse)
{
	// 回避使用可能状況を返す
	m_bDodgeUse = bUse;
}

//=========================
// 回避使用可能状況の取得処理
//=========================
bool CPlayerAction::IsDodgeUse(void) const
{
	// 回避使用可能状況を返す
	return m_bDodgeUse;
}

//=========================
// 反動状況の設定処理
//=========================
void CPlayerAction::SetEnableRecoil(const bool bRecoil)
{
	// 反動状況を設定する
	m_bRecoil = bRecoil;
}

//=========================
// 反動状況の取得処理
//=========================
bool CPlayerAction::IsRecoil(void) const
{
	// 反動状況を取得する
	return m_bRecoil;
}

//=========================
// 通常状態処理
//=========================
void CPlayerAction::NoneProcess(CPlayer* pPlayer)
{
	// 緊急時に移動できるようにしておく
	m_bRecoil = false;

	// 緊急時にボスに攻撃が通るようにする
	m_bBossAttack = false;

	// 緊急時にスピードを設定する
	pPlayer->GetController()->SetSpeed(pPlayer->GetController()->GetSpeedInit());
}

//=========================
// 射撃状態処理
//=========================
void CPlayerAction::ShotProcess(CPlayer* pPlayer)
{
	// 向きを取得する
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();
	D3DXVECTOR3 rotCamera = CManager::Get()->GetCamera()->GetRot();

	// カメラの向きを同じ向きを揃える
	rotDest.y = rotCamera.y;

	// 向きを適用する
	pPlayer->SetRotDest(rotDest);

	// 腕を真っ直ぐ伸ばす
	pPlayer->GetHierarchy(5)->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	pPlayer->GetHierarchy(6)->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pPlayer->GetHierarchy(7)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(8)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(9)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f,  0.0f, 0.0f));
	pPlayer->GetHierarchy(10)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
}

//=========================
// ダガー状態処理
//=========================
void CPlayerAction::DaggerPrecess(CPlayer* pPlayer)
{
	// 行動カウントを加算する
	m_nActionCount++;

	// 向きと移動量を取得する
	D3DXVECTOR3 rot = pPlayer->GetRot();
	D3DXVECTOR3 move = pPlayer->GetMove();

	if (m_nActionCount <= DAGGER_MOVE_COUNT)
	{ // 動く時間の場合 

		// 移動量を設定する
		pPlayer->GetController()->SetSpeed(DAGGER_MOVE);
	}
	else
	{ // 上記以外

		// 移動量を0にする
		pPlayer->GetController()->SetSpeed(0.0f);
	}

	// 移動量を適用する
	pPlayer->SetMove(move);

	if (m_nActionCount >= DAGGER_ATTACK_START &&
		m_nActionCount <= DAGGER_ATTACK_END)
	{ // 行動カウントが一定以上の場合

		// プレイヤーの位置を取得
		D3DXVECTOR3 pos = pPlayer->GetPos();

		// 軌跡の描画状況を true にする
		pPlayer->GetDagger()->SetEnableDispOrbit(true);

		// 爆弾花とダガーとの当たり判定
		collision::BangFlowerHit(pos, ATTACK_DAGGER_RADIUS, ATTACK_DAGGER_HEIGHT);

		// 木への攻撃判定処理
		collision::TreeAttack(*pPlayer, ATTACK_DAGGER_RADIUS, ATTACK_DAGGER_HEIGHT);

		// 敵とダガーの当たり判定
		collision::EnemyHitToDagger(pos, ATTACK_DAGGER_HEIGHT, ATTACK_DAGGER_RADIUS);

		// 爆弾とダガーの当たり判定
		collision::BombHitToDagger(pos, ATTACK_DAGGER_RADIUS, ATTACK_DAGGER_HEIGHT);

		// 的(風船)の当たり判定
		collision::TargetHit(pos, ATTACK_DAGGER_RADIUS);

		if (m_bBossAttack == false)
		{ // ボスに攻撃がまだ通っていなかった場合

			// ボスとの当たり判定
			collision::BossHit(D3DXVECTOR3(pos.x, pos.y + (ATTACK_DAGGER_HEIGHT * 0.5f), pos.z), ATTACK_DAGGER_RADIUS * 0.5f, DAGGER_BOSS_ATTACK);

			// ボスに攻撃が通った
			m_bBossAttack = true;
		}
	}

	if (m_nActionCount % DAGGER_COUNT == 0)
	{ // 行動カウントが一定数に達した場合

		// 行動を設定する
		SetAction(ACTION_NONE);

		// ダガーを表示しない
		pPlayer->GetDagger()->SetEnableDisp(false);
		pPlayer->GetDagger()->SetEnableDispOrbit(false);

		// 拳銃を描画する
		pPlayer->GetHandGun(0)->SetEnableDisp(true);
		pPlayer->GetHandGun(1)->SetEnableDisp(true);

		// 緊急時にスピードを設定する
		pPlayer->GetController()->SetSpeed(pPlayer->GetController()->GetSpeedInit());
	}
}

//=========================
// 回避状態処理
//=========================
void CPlayerAction::DodgeProcess(CPlayer* pPlayer)
{
	// 位置を取得する
	D3DXVECTOR3 pos = pPlayer->GetPos();

	pos.x += sinf(m_fDodgeRot) * DODGE_SPEED;
	pos.z += cosf(m_fDodgeRot) * DODGE_SPEED;

	// 位置を適用する
	pPlayer->SetPos(pos);

	CCharaBlur* pBlur = CCharaBlur::Create
	(
		pPlayer->GetPos(),
		pPlayer->GetRot(),
		pPlayer->GetScale(),
		pPlayer->GetNumModel(),
		DODGE_BLUR_COL,
		DODGE_BLUR_LIFE,
		CObject::PRIORITY_PAUSE
	);

	if (pBlur != nullptr)
	{ // ブラーが NULL じゃない場合

		for (int nCnt = 0; nCnt < pPlayer->GetNumModel(); nCnt++)
		{
			// モデルの情報設定処理
			pBlur->SetModelData
			(
				nCnt,
				pPlayer->GetHierarchy(nCnt)->GetPos(),
				pPlayer->GetHierarchy(nCnt)->GetRot(),
				pPlayer->GetHierarchy(nCnt)->GetScale(),
				pPlayer->GetHierarchy(nCnt)->GetFileData(),
				pPlayer->GetHierarchy(nCnt)->GetParentIdx()
			);
		}
	}

	// 行動カウントを加算する
	m_nActionCount++;

	if (m_nActionCount >= DODGE_COUNT)
	{ // 行動カウントが一定数以上になった場合

		// 行動を設定する
		SetAction(ACTION_NONE);
	}
}

//=========================
// 散弾(J+銃)状態
//=========================
void CPlayerAction::ShotgunProcess(CPlayer* pPlayer)
{
	// 位置と向きを取得する
	D3DXVECTOR3 pos = pPlayer->GetPos();
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();
	D3DXVECTOR3 rotCamera = CManager::Get()->GetCamera()->GetRot();

	if (pPlayer->IsJump() == true)
	{ // 空中にいる場合

		// 位置を移動する
		pos += m_moveRecoil;
	}
	else
	{ // 上記以外

		// 行動を設定する
		SetAction(ACTION_NONE);

		// 反動状況を false にする
		m_bRecoil = false;
	}

	// カメラの向きを同じ向きを揃える
	rotDest.y = rotCamera.y;

	// 位置と向きを適用する
	pPlayer->SetPos(pos);
	pPlayer->SetRotDest(rotDest);

	// 腕を真っ直ぐ伸ばす
	pPlayer->GetHierarchy(5)->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	pPlayer->GetHierarchy(6)->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pPlayer->GetHierarchy(7)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(8)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(9)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(10)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
}

//=========================
// 急降下状態処理
//=========================
void CPlayerAction::SwoopProcess(CPlayer* pPlayer)
{
	// 移動量を取得する
	D3DXVECTOR3 move = pPlayer->GetMove();

	if (pPlayer->IsJump() == false &&
		m_bRipple == false)
	{ // 地上に立った瞬間

		D3DXVECTOR3 pos = pPlayer->GetPos();

		// 位置を設定する
		pos.y += SWOOP_RIPPLE_HEIGHT;

		// 斬撃の波紋を生成する
		CSlashRipple::Create(pos, pPlayer->GetRot());

		// 波紋を出した
		m_bRipple = true;
	}

	// 移動量を設定する
	move.x = 0.0f;
	move.y = -20.0f - (m_nActionCount * 0.5f);
	move.z = 0.0f;

	// 移動量を適用する
	pPlayer->SetMove(move);

	// 行動カウントを加算する
	m_nActionCount++;

	if (m_nActionCount >= DAGGER_ATTACK_START &&
		m_nActionCount <= DAGGER_ATTACK_END)
	{ // 行動カウントが一定以上の場合

		// プレイヤーの位置を取得する
		D3DXVECTOR3 pos = pPlayer->GetPos();

		// 軌跡の描画状況を true にする
		pPlayer->GetDagger()->SetEnableDispOrbit(true);

		// 木への攻撃判定処理
		collision::TreeAttack(*pPlayer, ATTACK_DAGGER_RADIUS, ATTACK_DAGGER_HEIGHT);

		// 敵とダガーの当たり判定
		collision::EnemyHitToDagger(pos, ATTACK_DAGGER_HEIGHT, ATTACK_DAGGER_RADIUS);

		if (m_bBossAttack == false)
		{ // ボスに攻撃がまだ通っていなかった場合

			// ボスとの当たり判定
			collision::BossHit(D3DXVECTOR3(pos.x, pos.y + (ATTACK_DAGGER_HEIGHT * 0.5f), pos.z), ATTACK_DAGGER_RADIUS * 0.5f, DAGGER_BOSS_ATTACK);

			// ボスに攻撃が通った
			m_bBossAttack = true;
		}
	}

	if (m_nActionCount % SWOOP_COUNT == 0)
	{ // 行動カウントが一定数に達した場合

		// 行動を設定する
		SetAction(ACTION_NONE);

		// ダガーを表示しない
		pPlayer->GetDagger()->SetEnableDisp(false);
		pPlayer->GetDagger()->SetEnableDispOrbit(false);

		// 拳銃を描画する
		pPlayer->GetHandGun(0)->SetEnableDisp(true);
		pPlayer->GetHandGun(1)->SetEnableDisp(true);
	}
}

//=========================
// リロード処理
//=========================
void CPlayerAction::ReloadProcess(CPlayer* pPlayer)
{
	// 位置を取得する
	D3DXVECTOR3 pos = pPlayer->GetPos();

	if (pPlayer->IsJump() == true)
	{ // 空中にいる場合

		// 位置を移動する
		pos += m_moveRecoil;
	}
	else
	{ // 上記以外

		// 反動状況を false にする
		m_bRecoil = false;
	}

	// 位置を適用する
	pPlayer->SetPos(pos);

	// 腕を少し上にあげる
	pPlayer->GetHierarchy(5)->SetRot(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.25f));
	pPlayer->GetHierarchy(6)->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.25f));
	pPlayer->GetHierarchy(7)->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, -D3DX_PI * 0.25f));
	pPlayer->GetHierarchy(8)->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, D3DX_PI * 0.25f));
	pPlayer->GetHierarchy(9)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(10)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	if (pPlayer->GetHandGun(0)->GetState() == CHandgun::STATE_NONE &&
		pPlayer->GetHandGun(1)->GetState() == CHandgun::STATE_NONE)
	{ // どっちのハンドガンも通常に戻った場合

		// リロード完了にする
		pPlayer->GetBulletUI()->SetNumBullet(MAX_REMAINING_BULLET);

		// 通常状態にする
		SetAction(ACTION_NONE);
	}
}