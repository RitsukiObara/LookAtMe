//============================================
//
// プレイヤーのメイン処理[player.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "area.h"
#include "sound.h"
#include "useful.h"

#include "player.h"
#include "player_action.h"
#include "handgun.h"
#include "dagger.h"
#include "aim.h"
#include "bulletUI.h"
#include "gold_bone_UI.h"
#include "lifeUI.h"
#include "player_controller.h"
#include "airplane.h"
#include "shadowCircle.h"

#include "collision.h"
#include "camera.h"
#include "objectElevation.h"
#include "motion.h"
#include "block.h"
#include "orbit.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
namespace
{
	const float GRAVITY = 1.0f;						// 重力
	const float LAND_GRAVITY = -50.0f;				// 着地時の重力
	const float SHADOW_RADIUS = 70.0f;				// 影の半径
	const D3DXVECTOR3 INIT_CAMERA_ROT = D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f);		// カメラの初期向き
	const float INIT_POSV_CAMERA_Y = 250.0f;		// カメラの視点のY座標
	const float ROT_CORRECT = 0.2f;					// 向きの補正倍率
	const D3DXVECTOR3 AIRPLANE_ARRIVAL_MOVE = D3DXVECTOR3(0.0f, 30.0f, 0.0f);			// 飛行機到着時の移動量
	const D3DXVECTOR3 AIRPLANE_ARRIVAL_POSV = D3DXVECTOR3(-1000.0f, 1500.0f, 0.0f);		// 飛行機到着時のカメラの視点の位置
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(40.0f, 200.0f, 40.0f);				// 当たり判定時のサイズ
	const D3DXVECTOR3 GUN_POS[NUM_HANDGUN] =		// 拳銃の位置
	{
		D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
		D3DXVECTOR3(+10.0f, 0.0f, 0.0f)
	};
	const D3DXVECTOR3 GUN_ROT[NUM_HANDGUN] =		// 拳銃の向き
	{
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, -D3DX_PI * 0.5f),
		D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, D3DX_PI * 0.5f)
	};
	const int INIT_GUN_IDX = 9;						// 銃の初期値のインデックス
	const int INIT_DAGGER_IDX = 9;					// ダガーのインデックス
	const int MAX_LIFE = 100;						// 体力の最大値
	const int DAMAGE_COUNT = 30;					// ダメージ状態のカウント数
	const D3DXCOLOR DAMAGE_COLOR = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		// ダメージ状態の色
	const int INVINCIBLE_COUNT = 90;				// 無敵状態のカウント数
	const int INVINCIBLE_FLUSH_COUNT = 10;			// 無敵状態の点滅のカウント

	const float KNOCKBACK_MOVE = 23.0f;				// 吹き飛ぶ移動量
	const float KNOCKBACK_JUMP = 15.0f;				// 吹き飛ぶ高さ
	const int WIND_SHOT_DAMAGE = 10;				// 風攻撃のダメージ
	const int FIRE_SHOT_DAMAGE = 20;				// 炎攻撃のダメージ
	const int BOSS_RUSH_DAMAGE = 50;				// ボス突進のダメージ
}

//=========================================
// コンストラクタ
//=========================================
CPlayer::CPlayer() : CCharacter(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_pMotion = nullptr;					// モーションの情報
	m_pAction = nullptr;					// プレイヤーの行動の情報
	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		m_apHandGun[nCntGun] = nullptr;		// 拳銃の情報
	}
	m_pAim = nullptr;						// エイムの情報
	m_pDagger = nullptr;					// ダガーの情報
	m_pBulletUI = nullptr;					// 弾丸UIの情報
	m_pGoldBoneUI = nullptr;				// 金の骨のUIの情報
	m_pLifeUI = nullptr;					// 寿命UIの情報
	m_pController = nullptr;				// プレイヤーのコントローラーの情報
	m_pAirplane = nullptr;					// 飛行機の情報
	m_pShadow = nullptr;					// 丸影の情報

	m_stateInfo.state = STATE_NONE;			// 状態
	m_stateInfo.nCount = 0;					// 状態カウント
	m_stateInfo.col = NONE_D3DXCOLOR;		// 体の色
	m_rotDest = NONE_D3DXVECTOR3;			// 目標の向き
	m_move = NONE_D3DXVECTOR3;				// 移動量
	m_nLife = MAX_LIFE;						// 体力
	m_nAreaIdx = 0;							// 区分の番号
	m_bMove = false;						// 移動状況
	m_bJump = false;						// ジャンプ状況
}

//=========================================
// デストラクタ
//=========================================
CPlayer::~CPlayer()
{

}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CPlayer::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// モデルの総数を設定する
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_PLAYER));

	// データの設定処理
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // モーションが NULL だった場合

		// モーションの生成処理
		m_pMotion = CMotion::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pMotion != nullptr)
	{ // ポインタが NULL じゃない場合

		// モーションの情報を取得する
		m_pMotion->SetInfo(CMotion::STYLE_PLAYER, GetHierarchy(), GetNumModel());
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	// モーションの設定処理
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	if (m_pAction == nullptr)
	{ // 行動の情報が NULL の場合

		// プレイヤーアクションの生成処理
		m_pAction = CPlayerAction::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pBulletUI == nullptr)
	{ // 残弾UIが NULL の場合

		// 弾丸UIの生成
		m_pBulletUI = CBulletUI::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pGoldBoneUI == nullptr)
	{ // 金の骨UIが NULL の場合

		// 金の骨のUIの生成
		m_pGoldBoneUI = CGoldBoneUI::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pLifeUI == nullptr)
	{ // 金の骨UIが NULL の場合

		// 金の骨のUIの生成
		m_pLifeUI = CLifeUI::Create(MAX_LIFE);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pController == nullptr)
	{ // プレイヤーのコントローラーの情報が NULL の場合

		// プレイヤーコントローラーの生成
		m_pController = CPlayerController::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	// カメラの向きを設定
	CManager::Get()->GetCamera()->SetRot(INIT_CAMERA_ROT);

	// 全ての値を初期化する
	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		m_apHandGun[nCntGun] = nullptr;	// 拳銃の情報
	}
	m_pAim = nullptr;					// エイムの情報
	m_pDagger = nullptr;				// ダガーの情報
	m_pShadow = nullptr;				// 丸影の情報

	m_stateInfo.state = STATE_NONE;		// 状態
	m_stateInfo.nCount = 0;				// 状態カウント
	m_stateInfo.col = NONE_D3DXCOLOR;	// 体の色
	m_rotDest = NONE_D3DXVECTOR3;		// 目標の向き
	m_move = NONE_D3DXVECTOR3;			// 移動量
	m_nLife = MAX_LIFE;					// 体力
	m_bMove = false;					// 移動状況
	m_bJump = false;					// ジャンプ状況

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CPlayer::Uninit(void)
{
	if (m_pMotion != nullptr)
	{ // モーションが NULL じゃない場合

		// モーションのメモリを開放する
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pAction != nullptr)
	{ // 行動が NULL じゃない場合

		// 行動のメモリを開放する
		m_pAction->Uninit();
		m_pAction = nullptr;
	}

	if (m_pAim != nullptr)
	{ // エイムが NULL じゃない場合

		// エイムのメモリを開放する
		m_pAim->Uninit();
		m_pAim = nullptr;
	}

	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		if (m_apHandGun[nCntGun] != nullptr)
		{ // 拳銃の情報が NULL じゃない場合

			// 拳銃の終了処理
			m_apHandGun[nCntGun]->Uninit();
			m_apHandGun[nCntGun] = nullptr;
		}
	}

	if (m_pDagger != nullptr)
	{ // ダガーの情報が NULL じゃない場合

		// ダガーの終了処理
		m_pDagger->Uninit();
		m_pDagger = nullptr;
	}

	if (m_pBulletUI != nullptr)
	{ // 残弾UIの情報が NULL じゃない場合

		// 残弾UIを NULL にする
		m_pBulletUI = nullptr;
	}

	if (m_pGoldBoneUI != nullptr)
	{ // 金の骨の情報が NULL じゃない場合

		// 金の骨UIを NULL にする
		m_pGoldBoneUI = nullptr;
	}

	if (m_pLifeUI != nullptr)
	{ // 寿命UIの情報が NULL じゃない場合

		// 寿命UIを NULL にする
		m_pLifeUI = nullptr;
	}

	if (m_pController != nullptr)
	{ // コントローラーの情報が NULL じゃない場合

		// 終了処理
		m_pController->Uninit();
		m_pController = nullptr;
	}

	if (m_pAirplane != nullptr)
	{ // 飛行機の情報が NULL じゃない場合

		// 飛行機の終了処理
		m_pAirplane->Uninit();
		m_pAirplane = nullptr;
	}

	// 終了処理
	CCharacter::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CPlayer::Update(void)
{
	switch (CGame::GetState())
	{
	case CGame::STATE_START:	// 開始状態

		// 移動処理
		Move();

		if (m_pAirplane != nullptr &&
			m_pAirplane->GetState() == CAirplane::STATE_MOVE)
		{ // 飛行機が NULL じゃない場合

			// 位置を飛行機に合わせる
			SetPos(m_pAirplane->GetPos());

			// 更新処理
			m_pAirplane->Update(this);
		}

		if (m_pMotion != nullptr)
		{ // モーションが NULL じゃない場合

			// モーションの更新処理
			m_pMotion->Update();
		}

		// 起伏地面との当たり判定処理
		ElevationCollision();

		if (m_bJump == false)
		{ // ジャンプ状況が false の場合

			// プレイ状態にする
			CGame::SetState(CGame::STATE_PLAY);

			// 通常カメラにする
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_NONE);
		}

		break;

	case CGame::STATE_PLAY:			// プレイ状態

		// 前回の位置の設定処理
		SetPosOld(GetPos());

		// 状態管理処理
		StateManager();

		if (m_stateInfo.state == STATE_NONE ||
			m_stateInfo.state == STATE_INVINSIBLE)
		{ // 通常状態・無敵状態の場合

			// 操作処理
			m_pController->Control(this);
		}

		// 移動処理
		Move();

		if (m_pMotion != nullptr)
		{ // モーションが NULL じゃない場合

			// モーションの更新処理
			m_pMotion->Update();
		}

		if (m_pAction != nullptr)
		{ // 行動が NULL じゃない場合

			// 行動の更新処理
			m_pAction->Update(this);
		}

		for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
		{
			if (m_apHandGun[nCntGun] != nullptr)
			{ // 拳銃が NULL じゃない場合

				// 更新処理
				m_apHandGun[nCntGun]->Update();
			}
		}

		// 緊急のリロード処理
		EmergentReload();

		if (m_pAim != nullptr)
		{ // エイムが NULL じゃない場合

			// エイムの更新処理
			m_pAim->Update();
		}

		if (m_pLifeUI != nullptr)
		{ // 寿命が NULL じゃない場合

			// 寿命を設定する
			m_pLifeUI->SetLife(m_nLife);
		}

		// 区分の設定処理
		m_nAreaIdx = area::SetFieldIdx(GetPos());

		// ヤシの実との当たり判定
		collision::PalmFruitHit(this, COLLISION_SIZE.x, COLLISION_SIZE.y);

		// 金の骨との当たり判定
		collision::GoldBoneCollision(*this, COLLISION_SIZE);

		// 祭壇の周囲の当たり判定
		collision::AlterSurrounding(GetPos(), COLLISION_SIZE.x);

		// 起伏地面との当たり判定処理
		ElevationCollision();

		// 当たり判定処理
		Collision();

		// 祭壇との当たり判定
		AlterCollision();

		// ステージの当たり判定
		StageCollision();

		CManager::Get()->GetDebugProc()->Print("位置：%f %f %f", GetPos().x, GetPos().y, GetPos().z);

		break;

	case CGame::STATE_BOSSMOVIE:	// ボス出現状態

		// 起伏地面との当たり判定処理
		ElevationCollision();

		break;

	case CGame::STATE_GAMEOVER:		// ゲームオーバー状態

		if (m_pMotion != nullptr)
		{ // モーションが NULL じゃない場合

			// モーションの更新処理
			m_pMotion->Update();
		}

		// 起伏地面との当たり判定処理
		ElevationCollision();

		break;

	case CGame::STATE_FINISH:		// 終了状態

		if (m_pMotion != nullptr)
		{ // モーションが NULL じゃない場合

			// モーションの更新処理
			m_pMotion->Update();
		}

		// 起伏地面との当たり判定処理
		ElevationCollision();

		break;

	case CGame::STATE_CONTINUE:		// コンティニュー状態

		// 特に無し

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 影の位置設定処理
	ShadowPosSet();
}

//===========================================
// プレイヤーの描画処理
//===========================================
void CPlayer::Draw(void)
{
	if (m_pAirplane != nullptr)
	{ // 飛行機が NULL じゃない場合

		// 飛行機の描画処理
		m_pAirplane->Draw();
	}

	switch (m_stateInfo.state)
	{
	case STATE_NONE:		// 通常状態
	case STATE_DEATH:		// 死亡状態

		// 描画処理
		CCharacter::Draw();

		break;

	case STATE_DAMAGE:		// ダメージ状態

		// 描画処理
		CCharacter::Draw(m_stateInfo.col);

		break;

	case STATE_INVINSIBLE:	// 無敵状態

		// 描画処理
		CCharacter::Draw(m_stateInfo.col.a);

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		if (m_apHandGun[nCntGun] != nullptr)
		{ // 拳銃の情報が NULL じゃない場合

			// 拳銃の描画処理
			m_apHandGun[nCntGun]->Draw();
		}
	}

	if (m_pDagger != nullptr)
	{ // 拳銃の情報が NULL じゃない場合

		// 拳銃の描画処理
		m_pDagger->Draw();
	}

	if (m_pAim != nullptr)
	{ // エイムの情報が NULL じゃない場合

		// エイムの描画処理
		m_pAim->Draw();
	}
}

//===========================================
// ヒット処理
//===========================================
void CPlayer::Hit(const int nDamage, const float fRotSmash)
{
	// 体力を減らす
	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{ // 体力が0以下になった場合

		// 体力を0にする
		m_nLife = 0;

		// 死亡状態にする
		m_stateInfo.state = STATE_DEATH;

		// 移動量を0にする
		m_move = NONE_D3DXVECTOR3;

		if (m_pMotion->GetType() != MOTIONTYPE_DEATH)
		{ // 死亡モーションじゃない場合

			// 死亡モーションを行う
			m_pMotion->Set(MOTIONTYPE_DEATH);
		}

		if (CManager::Get()->GetMode() == CScene::MODE_GAME)
		{ // ゲームモードの場合

			// ゲームオーバーにする
			CGame::SetState(CGame::STATE_GAMEOVER);

			// ゲームオーバーカメラにする
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_GAMEOVER);
		}
	}
	else
	{ // 上記以外

		// ダメージ音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_PLAYERDAMAGE);

		// ダメージ状態にする
		m_stateInfo.state = STATE_DAMAGE;

		if (m_pAction->GetAction() != CPlayerAction::ACTION_RELOAD)
		{ // リロード状態以外

			// ダガーを表示しない
			m_pDagger->SetEnableDisp(false);
			m_pDagger->SetEnableDispOrbit(false);

			// 拳銃を描画する
			m_apHandGun[0]->SetEnableDisp(true);
			m_apHandGun[1]->SetEnableDisp(true);

			// 通常行動にする
			m_pAction->SetAction(CPlayerAction::ACTION_NONE);
		}

		// カウントを0にする
		m_stateInfo.nCount = 0;

		// 移動量を設定する
		m_move.x = sinf(fRotSmash) * KNOCKBACK_MOVE;
		m_move.y = KNOCKBACK_JUMP;
		m_move.z = cosf(fRotSmash) * KNOCKBACK_MOVE;

		if (m_pMotion->GetType() != MOTIONTYPE_DAMAGE)
		{ // ダメージモーションじゃなかった場合

			// ダメージモーションを設定
			m_pMotion->Set(MOTIONTYPE_DAMAGE);
		}

		// ダメージ音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);
	}
}

//===========================================
// 回復処理
//===========================================
void CPlayer::Healing(const int nHealing)
{
	// 体力を回復する
	m_nLife += nHealing;

	if (m_nLife >= MAX_LIFE)
	{ // 体力が最大値以上になった場合

		// 体力を最大値にする
		m_nLife = MAX_LIFE;
	}
}

//===========================================
// モーションの情報の設定処理
//===========================================
CMotion* CPlayer::GetMotion(void) const
{
	// モーションの情報を返す
	return m_pMotion;
}

//===========================================
// 行動の情報の取得処理
//===========================================
CPlayerAction* CPlayer::GetAction(void) const
{
	// 行動の情報を返す
	return m_pAction;
}

//===========================================
// 拳銃の情報の取得処理
//===========================================
CHandgun* CPlayer::GetHandGun(const int nCount) const
{
	// 拳銃の情報を返す
	return m_apHandGun[nCount];
}

//===========================================
// ダガーの情報の取得処理
//===========================================
CDagger* CPlayer::GetDagger(void) const
{
	// ダガーの情報を返す
	return m_pDagger;
}

//===========================================
// エイムの取得処理
//===========================================
CAim* CPlayer::GetAim(void) const
{
	// エイムの情報を返す
	return m_pAim;
}

//===========================================
// 残弾UIの情報の取得処理
//===========================================
CBulletUI* CPlayer::GetBulletUI(void) const
{
	// 残弾UIの情報を返す
	return m_pBulletUI;
}

//===========================================
// 金の骨UIの情報の取得処理
//===========================================
CGoldBoneUI* CPlayer::GetGoldBoneUI(void) const
{
	// 金の骨UIの情報を返す
	return m_pGoldBoneUI;
}

//=======================================
// 寿命UIの情報の取得処理
//=======================================
CLifeUI* CPlayer::GetLifeUI(void) const
{
	// 寿命UIの情報を返す
	return m_pLifeUI;
}

//=======================================
// コントローラーの情報の取得処理
//=======================================
CPlayerController* CPlayer::GetController(void) const
{
	// コントローラー情報を返す
	return m_pController;
}

//=======================================
// 飛行機の情報の取得処理
//=======================================
CAirplane* CPlayer::GetAirplane(void) const
{
	// 飛行機の情報を返す
	return m_pAirplane;
}

//=======================================
// 飛行機のNULL化処理
//=======================================
void CPlayer::DeleteAirplane()
{
	// 飛行機を NULL にする
	m_pAirplane = nullptr;
}

//=======================================
// 飛行機の到着処理
//=======================================
void CPlayer::ArrivalAirplane(void)
{
	if (m_pAirplane != nullptr)
	{ // 飛行機が NULL じゃない場合

		// 飛行機の位置を取得
		D3DXVECTOR3 pos = m_pAirplane->GetPos();

		// 目的の位置に補正する
		pos.x = m_pAirplane->GetPosDest().x;
		pos.z = m_pAirplane->GetPosDest().z;

		// 位置を適用
		m_pAirplane->SetPos(pos);

		// プレイヤー登場状態にする
		CManager::Get()->GetCamera()->SetType(CCamera::TYPE_PLAYERAPPEAR);

		// カメラの視点を設定する
		CManager::Get()->GetCamera()->SetPosV(pos + AIRPLANE_ARRIVAL_POSV);

		// 位置を設定する
		SetPos(pos);

		// 移動量の設定処理
		SetMove(AIRPLANE_ARRIVAL_MOVE);

		// 墜落状態にする
		m_pAirplane->SetState(CAirplane::STATE_FALL);

		// 種類を飛行機に変える
		m_pAirplane->SetType(TYPE_AIRPLANE);

		// 飛行機を NULL にする
		m_pAirplane = nullptr;

		if (m_pMotion->GetType() != MOTIONTYPE_AIRDIVE)
		{ // 飛行機降り状態以外の場合

			// 飛行機降りモーションを行う
			m_pMotion->Set(MOTIONTYPE_AIRDIVE);
		}
	}
}

//=======================================
// 情報の設定処理
//=======================================
void CPlayer::SetData(const D3DXVECTOR3& pos)
{
	// 全ての値を初期化する
	SetPos(pos);					// 位置
	SetPosOld(GetPos());			// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetScale(NONE_SCALE);			// 拡大率

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// 初期化処理
		GetHierarchy(nCntData)->SetPos(pos);					// 位置
		GetHierarchy(nCntData)->SetPosOld(pos);					// 前回の位置
		GetHierarchy(nCntData)->SetRot(NONE_D3DXVECTOR3);		// 向き
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);			// 拡大率
	}

	// モーションの設定処理
	m_pMotion->ResetMotion(MOTIONTYPE_NEUTRAL);

	for (int nCnt = 0; nCnt < NUM_HANDGUN; nCnt++)
	{
		if (m_apHandGun[nCnt] == nullptr)
		{ // 拳銃が NULL の場合

			// 拳銃の情報を生成する
			m_apHandGun[nCnt] = CHandgun::Create(GUN_POS[nCnt], GUN_ROT[nCnt], GetHierarchy(INIT_GUN_IDX + nCnt)->GetMatrixP());
		}
	}

	if (m_pDagger == nullptr)
	{ // ダガーが NULL の場合

		// ダガーを生成する
		m_pDagger = CDagger::Create(GetHierarchy(INIT_DAGGER_IDX)->GetMatrixP());
	}

	if (m_pAim == nullptr)
	{ // エイムが NULL の場合

		// エイムを生成する
		m_pAim = CAim::Create(GetPos());
	}

	if (m_pAirplane == nullptr)
	{ // 飛行機が NULL の場合

		// 飛行機を生成
		m_pAirplane = CAirplane::Create(pos);
	}

	// 位置を飛行機に合わせる
	SetPos(m_pAirplane->GetPos());
	
	// 全ての値を設定する
	m_stateInfo.state = STATE_NONE;		// 状態
	m_stateInfo.nCount = 0;				// 状態カウント
	m_stateInfo.col = NONE_D3DXCOLOR;	// 体の色
	m_rotDest = NONE_D3DXVECTOR3;		// 目標の向き
	m_move = NONE_D3DXVECTOR3;			// 移動量
	m_nLife = MAX_LIFE;					// 体力
	m_bMove = false;					// 移動状況
	m_bJump = false;					// ジャンプ状況

	// 区分の設定処理
	m_nAreaIdx = area::SetFieldIdx(GetPos());

	if (m_pShadow == nullptr)
	{ // 丸影が NULL の場合

		// 丸影を生成
		m_pShadow = CShadowCircle::Create(GetPos(), SHADOW_RADIUS, m_nAreaIdx);
	}
}

//===========================================
// 生成処理
//===========================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos)
{
	// プレイヤーのインスタンスを生成
	CPlayer* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pPlayer = new CPlayer;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pPlayer;
	}

	if (pPlayer != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPlayer->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pPlayer;
}

//=======================================
// 目的の向きの設定処理
//=======================================
void CPlayer::SetRotDest(const D3DXVECTOR3& rot)
{
	// 目的の向きを設定する
	m_rotDest = rot;
}

//=======================================
// 目的の向きの取得処理
//=======================================
D3DXVECTOR3 CPlayer::GetRotDest(void) const
{
	// 目的の向きを返す
	return m_rotDest;
}

//=======================================
// 移動量の設定処理
//=======================================
void CPlayer::SetMove(const D3DXVECTOR3& move)
{
	// 移動量を設定する
	m_move = move;
}

//=======================================
// 移動量の取得処理
//=======================================
D3DXVECTOR3 CPlayer::GetMove(void) const
{
	// 移動量を返す
	return m_move;
}

//=======================================
// 寿命の設定処理
//======================================
void CPlayer::SetLife(const int nLife)
{
	// 時縫い
	m_nLife = nLife;
}

//======================================
// 体力の取得処理
//======================================
int CPlayer::GetLife(void) const
{
	// 体力を返す
	return m_nLife;
}

//=======================================
// 状態の設定処理
//=======================================
void CPlayer::SetState(const SState state)
{
	// 状態を設定する
	m_stateInfo = state;
}

//=======================================
// 状態の取得処理
//=======================================
CPlayer::SState CPlayer::GetState(void) const
{
	// 状態関連の情報を返す
	return m_stateInfo;
}

//=======================================
// ジャンプ状況の設定処理
//=======================================
void CPlayer::SetEnableJump(const bool bJump)
{
	// ジャンプ状況を設定する
	m_bJump = bJump;
}

//=======================================
// ジャンプ状況の取得処理
//=======================================
bool CPlayer::IsJump(void) const
{
	//ジャンプ状況を返す
	return m_bJump;
}

//=======================================
// 区分の番号の設定処理
//=======================================
void CPlayer::SetAreaIdx(const int nIdx)
{
	// 区分の番号を設定する
	m_nAreaIdx = nIdx;
}

//=======================================
// 区分の番号の取得処理
//=======================================
int CPlayer::GetAreaIdx(void) const
{
	// 区分の番号を返す
	return m_nAreaIdx;
}

//=======================================
// 起伏地面の当たり判定処理
//=======================================
void CPlayer::ElevationCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ
	bool bJump = true;				// ジャンプ状況
	bool bRange = false;			// 範囲内状況
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// 先頭の小判
	CElevation* pElevEnd = nullptr;		// 末尾の値
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
			fHeight = pElev->ElevationCollision(pos, &bRange);

			if (pos.y < fHeight)
			{ // 当たり判定の位置が高かった場合

				// 高さを設定する
				pos.y = fHeight;

				// 重力を設定する
				m_move.y = LAND_GRAVITY;

				if (m_bJump == true)
				{ // ジャンプ中の場合

					// 着地音を鳴らす
					CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
				}

				// ジャンプ状況を false にする
				bJump = false;
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

	// ジャンプ状況を代入する
	m_bJump = bJump;

	// 位置を更新する
	SetPos(pos);
}

//=======================================
// 当たり判定処理
//=======================================
void CPlayer::Collision(void)
{
	D3DXVECTOR3 pos = GetPos();									// 位置
	D3DXVECTOR3 posOld = GetPosOld();							// 前回の位置
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(COLLISION_SIZE);	// 頂点の最小値
	int nIdx = 0;

	for (int nCnt = 0; nCnt < area::NUM_COLL; nCnt++)
	{
		nIdx = m_nAreaIdx + area::COLL_ADD_IDX[nCnt];

		if (area::IndexCheck(nIdx) == true)
		{ // 区分内の場合

			// 小判との当たり判定
			collision::CoinCollision(this, COLLISION_SIZE, nIdx);

			// 木との当たり判定
			collision::TreeCollision(&pos, COLLISION_SIZE.x, nIdx);

			// ブロックとの当たり判定
			BlockCollision(&pos, posOld, COLLISION_SIZE, vtxMin, nIdx);

			// 岩との当たり判定
			collision::RockCollision(&pos, posOld, COLLISION_SIZE.x, COLLISION_SIZE.y, nIdx, &m_move.y, &m_bJump);

			// 岩との当たり判定
			collision::WallCollision(&pos, posOld, COLLISION_SIZE, vtxMin, nIdx);
		}
	}

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 状態管理処理
//=======================================
void CPlayer::StateManager(void)
{
	switch (m_stateInfo.state)
	{
	case CPlayer::STATE_NONE:

		if (m_pAction->GetAction() != CPlayerAction::ACTION_DODGE)
		{ // 回避状態以外

			// 当たり判定
			if (collision::EnemyHitToPlayer(this, COLLISION_SIZE.x, COLLISION_SIZE.y) == true ||
				collision::ExplosionHitToPlayer(this, COLLISION_SIZE.x, COLLISION_SIZE.y) == true)
			{ // 何かに当たった場合

				// この先の処理を行わない
				return;
			}

			// 吹き飛ぶ向き
			float fRotSmash = 0.0f;
			D3DXVECTOR3 pos = GetPos();

			if (collision::WindShotHit(pos, COLLISION_SIZE.x, COLLISION_SIZE.y, &fRotSmash) == true)
			{ // 風攻撃に当たった場合

				// ヒット処理
				Hit(WIND_SHOT_DAMAGE, fRotSmash);
			}
			else if (collision::FireShotHit(pos, COLLISION_SIZE.x, COLLISION_SIZE.y, &fRotSmash) == true)
			{ // 炎攻撃に当たった場合

				// ヒット処理
				Hit(FIRE_SHOT_DAMAGE, fRotSmash);
			}
			else if (collision::BossAttack(D3DXVECTOR3(pos.x, pos.y + (COLLISION_SIZE.y * 0.5f), pos.z), COLLISION_SIZE.x, &fRotSmash) == true)
			{ // ボスの突進に当たった場合

				// ヒット処理
				Hit(BOSS_RUSH_DAMAGE, fRotSmash);
			}
		}

		break;

	case CPlayer::STATE_DAMAGE:

		// ダメージ状態処理
		DamageState();


		break;

	case CPlayer::STATE_INVINSIBLE:

		// 無敵状態処理
		InvisibleState();

		break;

	case CPlayer::STATE_DEATH:

		if (m_pMotion->GetType() != MOTIONTYPE_DEATH)
		{ // 死亡モーションじゃない場合

			// 死亡モーションを行う
			m_pMotion->Set(MOTIONTYPE_DEATH);
		}

		if (CManager::Get()->GetMode() == CScene::MODE_GAME)
		{ // ゲームモードの場合

			// ゲームオーバーにする
			CGame::SetState(CGame::STATE_GAMEOVER);

			// ゲームオーバーカメラにする
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_GAMEOVER);
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=======================================
// ダメージ状態処理
//=======================================
void CPlayer::DamageState()
{
	// 状態カウントを加算する
	m_stateInfo.nCount++;

	// 体の色を変える
	m_stateInfo.col = DAMAGE_COLOR;

	if (m_stateInfo.nCount >= DAMAGE_COUNT)
	{ // 状態カウントが一定数以上になった場合

		// 状態カウントを0にする
		m_stateInfo.nCount = 0;

		// 体の色を元に戻す
		m_stateInfo.col = NONE_D3DXCOLOR;

		// 無敵状態にする
		m_stateInfo.state = STATE_INVINSIBLE;
	}
}

//=======================================
// 無敵状態処理
//=======================================
void CPlayer::InvisibleState()
{
	// 状態カウントを加算する
	m_stateInfo.nCount++;

	if (m_stateInfo.nCount % INVINCIBLE_FLUSH_COUNT == 0)
	{ // カウントが一定数以上になった場合

		// 色を変える
		m_stateInfo.col.a = (m_stateInfo.col.a >= 1.0f) ? 0.0f : 1.0f;
	}

	if (m_stateInfo.nCount >= INVINCIBLE_COUNT)
	{ // 状態カウントが一定数以上になった場合

		// 状態カウントを0にする
		m_stateInfo.nCount = 0;

		// 通常状態にする
		m_stateInfo.state = STATE_NONE;
	}
}

//=======================================
// 移動処理
//=======================================
void CPlayer::Move(void)
{
	// 位置と向きを取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 向きの補正処理
	useful::RotCorrect(m_rotDest.y, &rot.y, ROT_CORRECT);

	if (m_pAction->IsRecoil() == false)
	{ // 反動状況が false の場合

		// 位置を移動させる
		pos.x += m_move.x;
		pos.z += m_move.z;
	}

	// 重力をかける
	useful::Gravity(&m_move.y, &pos.y, GRAVITY);

	// 位置と向きを適用する
	SetPos(pos);
	SetRot(rot);
}

//=======================================
// 緊急のリロード処理
//=======================================
void CPlayer::EmergentReload(void)
{
	if (m_pBulletUI != nullptr &&
		m_pBulletUI->GetNumBullet() <= 0 &&
		m_stateInfo.state == STATE_NONE)
	{ // 通常状態で弾がリロードできてなかった場合

		// 緊急で弾丸をリロードする
		m_pBulletUI->SetNumBullet(MAX_REMAINING_BULLET);
	}
}

//=======================================
// 影の位置設定処理
//=======================================
void CPlayer::ShadowPosSet(void)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	// 影の位置と区分の番号を設定する
	m_pShadow->SetPos(pos);
	m_pShadow->SetAreaIdx(m_nAreaIdx);

	if (m_bJump == true)
	{ // ジャンプしている場合

		// 高さを最下層にする
		pos.y = FLT_MIN;

		// 影の当たり判定
		m_pShadow->Collision();
	}
}

//=======================================
// ブロックとの当たり判定
//=======================================
void CPlayer::BlockCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx)
{
	// ローカル変数宣言
	collision::SCollision coll = { false,false,false,false,false,false };				// 当たり判定の変数
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

			if (coll.bTop == true)
			{ // 上に乗った場合

				// 移動量を設定する
				m_move.y = 0.0f;

				// ジャンプしていない
				m_bJump = false;
			}

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

//=======================================
// 祭壇との当たり判定
//=======================================
void CPlayer::AlterCollision(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 祭壇との当たり判定
	collision::AlterCollision
	(
		&pos,
		GetPosOld(),
		COLLISION_SIZE,
		D3DXVECTOR3(-COLLISION_SIZE.x, 0.0f, -COLLISION_SIZE.z)
	);

	// 位置の設定処理
	SetPos(pos);
}

//=======================================
// ステージとの当たり判定
//=======================================
void CPlayer::StageCollision(void)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	// ステージの当たり判定
	collision::StageCollision(&pos, COLLISION_SIZE.x);

	// 位置を適用
	SetPos(pos);
}