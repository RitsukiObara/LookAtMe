//============================================
//
// プレイヤーのメイン処理[player.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "player.h"
#include "game_player.h"
#include "tutorial_player.h"
#include "playerAct.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "sound.h"
#include "useful.h"
#include "texture.h"

#include "actionUI.h"
#include "bgmulti.h"
#include "bullet.h"
#include "block.h"
#include "camera.h"
#include "collision.h"
#include "collpolygon.h"
#include "enemy.h"
#include "Effect.h"
#include "effect2D.h"
#include "Particle.h"
#include "particle2D.h"
#include "file.h"
#include "fraction.h"
#include "motion.h"
#include "orbit.h"
#include "ripple.h"
#include "score.h"
#include "shadowCircle.h"
#include "signpole.h"
#include "staminaUI.h"
#include "streetlamp.h"
#include "speedUI.h"
#include "sonic.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define PLAYER_SIGNPOLE_WALK			(10.0f)		// 標識渡りの微調整用マクロ定義
#define PLAYER_MOVE_DAMP_MAGNI			(0.25f)		// 移動量の減衰係数
#define PLAYER_WALLKICK_MOVE_DAMP_MAGNI	(0.00f)		// 壁キック時の移動量の減衰係数
#define PLAYER_JUMP_MOVE_CORRECT		(0.3f)		// プレイヤーのジャンプ中の移動量の補正倍率
#define DAMAGE_HITSTOP					(4)			// ダメージ時のヒットストップ
#define PLAYER_JUMP_MOVE_MAGNI			(0.8f)		// ジャンプ時の移動量の倍率
#define PLAYER_CRUSH_FLY				(20.0f)		// プレイヤーの圧殺復活時の飛び跳ねる高さ
#define PLAYER_CRUSH_WIDTH				(0.2f)		// プレイヤーが圧死したときの幅
#define PLAYER_CRUSH_WIDTH_SUB			(0.05f)		// プレイヤーが圧死したときの幅の減算量
#define PLAYER_BOOST_COUNT				(120)		// ブースト状態が続くカウント数
#define PLAYER_JUMP_ATTEN_COUNT			(40)		// ジャンプ中の移動量が減衰し始めるカウント数
#define PLAYER_BOOST_ADD_SPEED			(10.0f)		// ブースト中の追加スピード
#define PLAYER_DASH_SOUND_COUNT			(15)		// 移動音を鳴らすカウント数
#define PLAYER_CRUSH_COUNT				(180)		// プレイヤーの圧死状態のカウント
#define PLAYER_CRUSHVOICE_COUNT			(20)		// 潰れた時の声を出すカウント
#define PLAYER_BOOSTSOUND_COUNT			(44)		// スーパーブーストの音の出すカウント
#define PLAYER_FALLSOUND_COUNT			(28)		// 落下音が流れるカウント
#define PLAYER_FALLSOUND_GRAVITY		(-5.0f)		// 落下音が流れる重力
#define PLAYER_JUMPCHARGE_SOUND_COUNT	(50)		// 超ジャンプチャージ音を出すカウント数
#define PLAYER_SONIC_JUMP_COUNT			(3)			// 最速ジャンプのカウント数

//--------------------------------------------
// マクロ定義(テクスチャ系)
//--------------------------------------------
#define ORBIT_TEXTURE			"data/TEXTURE/Orbit.png"					// 通常の軌跡のテクスチャ
#define ORBIT_POLE_TEXTURE		"data/TEXTURE/PoleOrbit.png"				// ぶら下がり状態の軌跡のテクスチャ

//=========================================
// コンストラクタ
//=========================================
CPlayer::CPlayer() : CCharacter(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// オーバーロードコンストラクタ
//=========================================
CPlayer::CPlayer(CObject::TYPE type, PRIORITY priority) : CCharacter(type, priority)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// デストラクタ
//=========================================
CPlayer::~CPlayer()
{

}

//=========================================
// コンストラクタの箱
//=========================================
void CPlayer::Box(void)
{
	// 全ての値をクリアする
	m_pMotion = nullptr;							// モーションの情報
	m_pAction = nullptr;							// 行動関係
	m_pOrbit = nullptr;								// 軌跡情報
	m_pStreetLamp = nullptr;						// 街灯の情報
	m_pSonic = nullptr;								// 高速演出の情報
	m_pSpeedUI = nullptr;							// 速度UIの情報
	m_crush.scaleDest = NONE_SCALE;					// 目標の拡大率
	m_crush.bTop = false;							// 上からの判定
	m_crush.bBottom = false;						// 下からの判定
	m_crush.bRight = false;							// 右からの判定
	m_crush.bLeft = false;							// 左からの判定
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_state = STATE_NONE;							// 状態
	m_nStateCount = 0;								// 状態カウント
	m_nShadowIdx = INIT_SHADOW;						// 影のインデックス
	m_nWallFricCount = 0;							// 壁擦りカウント
	m_nJumpCount = 0;								// ジャンプしているカウント
	m_nOrbitIdx[ORBIT_TEX_NORMAL] = NONE_TEXIDX;	// 軌跡のテクスチャのインデックス
	m_nOrbitIdx[ORBIT_TEX_POLE] = NONE_TEXIDX;		// 軌跡のテクスチャのインデックス
	m_nMoveCount = 0;								// ダッシュカウント
	m_fSpeed = 0.0f;								// 速度
	m_fStamina = PLAYER_MAX_STAMINA;				// スタミナ
	m_fMoveDest = 0.0f;								// 目的の移動量
	m_button.nButtonCount = 0;						// ボタンを押している時間
	m_button.bSpacePush = false;					// ジャンプキーの押し状況
	m_bJump = false;								// ジャンプ状況
	m_bRight = false;								// 右を向いているか
	m_bMove = false;								// 移動状況
}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CPlayer::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// ベタ打ち
	SetNumModel(14);

	// データの設定処理
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // モーションが NULL だった場合

		// モーションの生成処理
		m_pMotion = CMotion::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 警告文
		MessageBox(NULL, "プレイヤーのモーションのポインタが使われています！", "警告！", MB_ICONWARNING);
	}

	if (m_pMotion != nullptr)
	{ // ポインタが NULL じゃない場合

		// モーションの情報を取得する
		m_pMotion->SetModel(GetHierarchy(), GetNumModel());

		// ロード処理
		m_pMotion->Load(CMotion::TYPE_PLAYER);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 警告文
		MessageBox(NULL, "プレイヤーのモーションのメモリが確保できませんでした！", "警告！", MB_ICONWARNING);
	}

	m_pStreetLamp = nullptr;					// 街灯の情報
	m_pSonic = nullptr;							// 高速演出の情報
	m_pSpeedUI = nullptr;						// 速度UIの情報
	m_pStaminaUI = nullptr;						// スタミナUIの情報
	m_crush.scaleDest = NONE_SCALE;				// 目標の拡大率
	m_crush.bTop = false;						// 上からの判定
	m_crush.bBottom = false;					// 下からの判定
	m_crush.bRight = false;						// 右からの判定
	m_crush.bLeft = false;						// 左からの判定
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_state = STATE_NONE;						// 状態
	m_nStateCount = 0;							// 状態カウント
	m_nWallFricCount = 0;						// 壁擦りカウント
	m_nJumpCount = 0;							// ジャンプしているカウント
	m_nMoveCount = 0;							// ダッシュカウント
	m_fSpeed = PLAYER_INIT_SPEED;				// 速度
	m_fStamina = PLAYER_MAX_STAMINA;			// スタミナ
	m_fMoveDest = 0.0f;							// 目的の移動量
	m_button.nButtonCount = 0;					// ボタンを押している時間
	m_button.bSpacePush = false;				// ジャンプキーの押し状況
	m_bJump = false;							// ジャンプ状況
	m_bRight = true;							// 右を向いているか
	m_bMove = false;							// 移動状況

	// モーションの設定処理
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	// 行動の情報を生成する
	m_pAction = CPlayerAct::Create();

	// テクスチャの読み込み
	m_nOrbitIdx[ORBIT_TEX_NORMAL] = CManager::GetTexture()->Regist(ORBIT_TEXTURE);
	m_nOrbitIdx[ORBIT_TEX_POLE] = CManager::GetTexture()->Regist(ORBIT_POLE_TEXTURE);

	// 軌跡を生成する
	m_pOrbit = COrbit::Create(GetPos(), GetRot(), m_nOrbitIdx[ORBIT_TEX_NORMAL]);

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CPlayer::Uninit(void)
{
	// モーションのメモリを開放する
	delete m_pMotion;
	m_pMotion = nullptr;

	// 行動の情報のメモリを開放する
	delete m_pAction;
	m_pAction = nullptr;

	// ポインタを NULL にする
	m_pStreetLamp = nullptr;		// 街灯
	m_pSonic = nullptr;				// ダッシュ演出

	if (m_pSpeedUI != nullptr)
	{ // 速度UIが NULL じゃなかった場合

		// 終了処理
		m_pSpeedUI->Uninit();
		m_pSpeedUI = nullptr;
	}

	if (m_pStaminaUI != nullptr)
	{ // スタミナUIが NULL じゃなかった場合

		// 終了処理
		m_pStaminaUI->Uninit();
		m_pStaminaUI = nullptr;
	}

	// 終了処理
	CCharacter::Uninit();

	// プレイヤーポインタのNULL化処理
	CGame::DeletePlayer();
}

//===========================================
// スタート更新処理
//===========================================
void CPlayer::StartUpdate(void)
{
	if (CManager::GetCamera() != nullptr)
	{ // カメラの情報がある場合

		switch (CManager::GetCamera()->GetType())
		{
		case CCamera::TYPE_STARTSTAGE:		// ステージ映し

			// 特に無し

			break;

		case CCamera::TYPE_STARTRUN:		// 走り姿

			if (m_nMoveCount % PLAYER_DASH_SOUND_COUNT == 0 &&
				m_bJump == false)
			{ // 移動カウントが一定数に達したかつ、陸に要る場合

				// 移動音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DASH);
			}

			// 移動カウントを加算する
			m_nMoveCount++;

			{ // 位置の更新

				// ローカル変数宣言
				D3DXVECTOR3 pos = GetPos();

				// 走らせる
				pos.x += PLAYER_INIT_SPEED;

				// 位置の設定処理
				SetPos(pos);
			}

			if (m_pMotion->GetType() != MOTIONTYPE_MOVE)
			{ // 移動モーション以外の場合

				// 移動モーションを設定する
				m_pMotion->Set(MOTIONTYPE_MOVE);
			}

			break;

		case CCamera::TYPE_STARTBACK:		// 戻り

			if (m_pMotion->GetType() != MOTIONTYPE_START)
			{ // スタート前モーション以外の場合

				// スタート前モーションを設定する
				m_pMotion->Set(MOTIONTYPE_START);
			}

			break;
		}

		// モーションの更新処理
		m_pMotion->Update();
	}

	if (m_bJump == true)
	{ // ジャンプ状況が true の場合

		// 位置の設定処理(Y軸無し)
		CShadowCircle::SetPosRotXZ(m_nShadowIdx, GetPos(), GetRot());
	}
	else
	{ // 上記以外

		// 影の位置向き設定処理
		CShadowCircle::SetPosRot(m_nShadowIdx, GetPos(), GetRot());
	}
}

//===========================================
// カウントダウン更新処理
//===========================================
void CPlayer::CountDownUpdate(void)
{
	if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL)
	{ // 立ちモーション以外の場合

		// 立ちモーションを設定する
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);
	}

	// モーションの更新処理
	m_pMotion->Update();
}

//===========================================
// 通常更新処理
//===========================================
void CPlayer::MainUpdate(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得
	bool bRight = false;			// 右
	bool bLeft = false;				// 左

	// 前回の位置を保存する
	SetPosOld(pos);

	// 状態処理
	State();

	if (m_state != STATE_CRUSH)
	{ // 圧殺状態以外の場合

		if (m_bMove == true)
		{ // 移動している場合

			// 移動カウントを加算する
			m_nMoveCount++;

			// スタミナを微量自動回復させる
			CalcStamina(PLAYER_AUTO_HEAL_STAMINA_MOVE);

			if (m_nMoveCount % PLAYER_DASH_SOUND_COUNT == 0 &&
				m_bJump == false)
			{ // 移動カウントが一定数に達したかつ、陸に要る場合

				// 移動音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DASH);
			}
		}
		else
		{ // 移動していない場合

			// スタミナを自動回復させる
			CalcStamina(PLAYER_AUTO_HEAL_STAMINA_STOP);

			// カウントを 0 にする
			m_nMoveCount = 0;
		}

		// 操作処理
		Control();

		// 位置を取得する
		pos = GetPos();

		if (m_pAction->GetAct() != CPlayerAct::ACT_POLEDOWN)
		{ // ポール下り状態以外の場合

			// 位置を設定する
			pos.z = 0.0f;
		}

		// 移動量を加算する
		pos.x += m_move.x;

		// 走った後の埃の設定処理
		RunDustEffect();

		// 位置の設定処理
		SetPos(pos);

		// 重力処理
		PlayerGravity();
	}

	// ブロックの当たり判定処理
	StageCollision(&bRight, &bLeft);

	if (m_state != STATE_CRUSH)
	{ // 圧殺状態以外の場合

		// 壁擦り処理
		WallFriction(bRight, bLeft);

		// 壁キック処理
		WallKick(bRight, bLeft);

		// 行動状態処理
		m_pAction->Action(*this);

		// アイテムとの当たり判定
		collision::PlayerItemCollision(*this);
	}

	// 速度の星生成処理
	SpeedStarEffect();

	// 地面着地処理
	GroundLanding();

	// 位置の取得処理
	pos = GetPos();

	// 位置の設定処理
	SetPos(pos);

	if (m_state != STATE_CRUSH)
	{ // 圧殺状態以外の場合

		// モーションの更新処理
		m_pMotion->Update();

		// プレイヤーでの軌跡処理
		PlayerOrbit();
	}

	if (m_pAction->GetAct() == CPlayerAct::ACT_NONE &&
		m_pMotion->GetType() != MOTIONTYPE_FALL &&
		m_move.y < -10.0f)
	{ // 落ちている場合

		// 落下モーションを設定する
		m_pMotion->Set(MOTIONTYPE_FALL);
	}

	//// プレイヤーでの高速演出処理
	//PlayerSonic();

	// プレイヤーとカメラの範囲の当たり判定処理
	collision::PlayerCameraRangeCollision(*this, PLAYER_WIDTH, PLAYER_HEIGHT);

	if (CManager::GetDebugProc() != nullptr)
	{ // デバッグ表示が NULL じゃない場合

		// デバッグ表示
		CManager::GetDebugProc()->Print("移動量：%f\n", m_move.x);
	}
}

//===========================================
// ゴール更新処理
//===========================================
void CPlayer::GoalUpdate(void)
{
	if (m_pMotion->GetType() != MOTIONTYPE_MOVE)
	{ // 移動モーション以外の場合

		// 走りモーションを追加する
		m_pMotion->Set(MOTIONTYPE_MOVE);
	}

	// 行動状況を通常状態にする
	m_pAction->ActionChange(CPlayerAct::ACT_NONE);

	// 右を向かせる
	m_bRight = true;

	// モーションの更新処理
	m_pMotion->Update();
}

//===========================================
// 終了更新処理
//===========================================
void CPlayer::FinishUpdate(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// プレイヤーの位置
	bool bRight = false;			// 右
	bool bLeft = false;				// 左

	if (m_nMoveCount % PLAYER_DASH_SOUND_COUNT == 0 &&
		m_bJump == false)
	{ // 移動カウントが一定数に達したかつ、陸に要る場合

		// 移動音を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DASH);
	}

	// 移動カウントを加算する
	m_nMoveCount++;

	// 左に歩かせる
	pos.x += PLAYER_MIN_SPEED;

	// 位置の設定処理
	SetPos(pos);

	// 走った後の埃の設定処理
	RunDustEffect();

	// 重力処理
	PlayerGravity();

	// ブロックの当たり判定処理
	StageCollision(&bRight, &bLeft);

	// 地面着地処理
	GroundLanding();

	// 右を向かせる
	m_bRight = true;

	// 移動する
	m_bMove = true;

	// モーションの更新処理
	m_pMotion->Update();
}

//===========================================
// プレイヤーの描画処理
//===========================================
void CPlayer::Draw(void)
{
	switch (m_state)
	{
	case STATE_NONE:		// 通常状態

		// 描画処理
		CCharacter::Draw();

		break;

	case STATE_BOOST:		// ブースト状態

		// 描画処理
		CCharacter::Draw();

		break;

	case STATE_DAMAGE:		// ダメージ状態

		// 描画処理
		CCharacter::Draw(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		break;

	case STATE_INVINCIBLE:	// 無敵状態

		// 描画処理
		CCharacter::Draw(m_col.a);

		break;

	case STATE_CRUSH:

		// 描画処理
		CCharacter::Draw();

		break;
	}
}

//===========================================
// 色の設定処理
//===========================================
void CPlayer::SetColor(const D3DXCOLOR& col)
{
	// 色を設定する
	m_col = col;
}

//===========================================
// 色の取得処理
//===========================================
D3DXCOLOR CPlayer::GetColor(void) const
{
	// 色を返す
	return m_col;
}

//===========================================
// 移動量の設定処理
//===========================================
void CPlayer::SetMove(const D3DXVECTOR3& move)
{
	// 移動量を設定する
	m_move = move;
}

//===========================================
// 移動量の取得処理
//===========================================
D3DXVECTOR3 CPlayer::GetMove(void) const
{
	// 移動量を返す
	return m_move;
}

//===========================================
// 状態の設定処理
//===========================================
void CPlayer::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;

	// 状態カウントを設定する
	m_nStateCount = 0;
}

//===========================================
// 状態の取得処理
//===========================================
CPlayer::STATE CPlayer::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//===========================================
// ジャンプ状況の設定処理
//===========================================
void CPlayer::SetJump(const bool bJump)
{
	// ジャンプ状況を設定する
	m_bJump = bJump;
}

//===========================================
// ジャンプ状況の取得処理
//===========================================
bool CPlayer::GetJump(void) const
{
	// ジャンプ状況を返す
	return m_bJump;
}

//===========================================
// 速度の設定処理
//===========================================
void CPlayer::SetSpeed(float fSpeed)
{
	if (fSpeed >= PLAYER_MAX_SPEED)
	{ // 速度が最大値以上の場合

		// 速度を設定する
		fSpeed = PLAYER_MAX_SPEED;
	}

	// 速度を設定する
	m_fSpeed = fSpeed;
}

//===========================================
// 速度の取得処理
//===========================================
float CPlayer::GetSpeed(void) const
{
	// 速度を返す
	return m_fSpeed;
}

//===========================================
// ボタン押し状況の設定処理
//===========================================
void CPlayer::SetButtonPush(const bool bPush)
{
	// ボタンの押し状況を設定する
	m_button.bSpacePush = bPush;
}

//===========================================
// ボタンカウントの設定処理
//===========================================
void CPlayer::SetButtonCount(const int nCount)
{
	// ボタンのカウントを設定する
	m_button.nButtonCount = nCount;
}

//===========================================
// ボタンカウントの取得処理
//===========================================
CPlayer::SButton CPlayer::GetButton(void)
{
	// ボタンの情報を返す
	return m_button;
}

//===========================================
// 右向き状況の設定処理
//===========================================
void CPlayer::SetRight(const bool bRight)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 右向き状況を設定する
	m_bRight = bRight;

	if (m_bRight == true)
	{ // 右を向いていた場合

		// 向きを右向きにする
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // 左を向いていた場合

		// 向きを左向きにする
		rot.y = D3DX_PI * -0.5f;
	}

	// 向きを更新する
	SetRot(rot);
}

//===========================================
// 右向き状況の取得処理
//===========================================
bool CPlayer::GetRight(void) const
{
	// 右向き状況を返す
	return m_bRight;
}

//===========================================
// 壁擦りカウントの設定処理
//===========================================
void CPlayer::SetWallFricCount(const int nCount)
{
	// 壁擦りカウントを設定する
	m_nWallFricCount = nCount;
}

//===========================================
// 圧殺判定(上)の設定処理
//===========================================
void CPlayer::SetCrushTop(const bool bTop)
{
	// 上からの判定を設定する
	m_crush.bTop = bTop;
}

//===========================================
// 圧殺判定(下)の設定処理
//===========================================
void CPlayer::SetCrushBottom(const bool bBottom)
{
	// 下からの判定を設定する
	m_crush.bBottom = bBottom;
}

//===========================================
// 圧殺判定(右)の設定処理
//===========================================
void CPlayer::SetCrushRight(const bool bRight)
{
	// 右からの判定を設定する
	m_crush.bRight = bRight;
}

//===========================================
// 圧殺判定(左)の設定処理
//===========================================
void CPlayer::SetCrushLeft(const bool bLeft)
{
	// 左からの判定を設定する
	m_crush.bLeft = bLeft;
}

//===========================================
// モーションの情報の設定処理
//===========================================
CMotion* CPlayer::GetMotion(void)
{
	// モーションの情報を返す
	return m_pMotion;
}

//===========================================
// 行動の情報の取得処理
//===========================================
CPlayerAct* CPlayer::GetAction(void)
{
	// 行動情報を返す
	return m_pAction;
}

//===========================================
// 軌跡の情報の取得処理
//===========================================
COrbit* CPlayer::GetOrbit(void)
{
	// 軌跡の情報を返す
	return m_pOrbit;
}

//===========================================
// 生成処理
//===========================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos, const CScene::MODE mode)
{
	// ローカルオブジェクトを生成
	CPlayer* pPlayer = nullptr;	// プレイヤーのインスタンスを生成

	if (pPlayer == nullptr)
	{ // オブジェクトが NULL の場合

		switch (mode)
		{
		case CScene::MODE_TUTORIAL:		// チュートリアル

			// メモリを確保する
			pPlayer = new CTutorialPlayer;

			break;

		case CScene::MODE_GAME:			// ゲーム

			// メモリを確保する
			pPlayer = new CGamePlayer;

			break;

		default:						// 上記以外

			// NULL を返す
			return nullptr;

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pPlayer != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "プレイヤーの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPlayer->SetData(pos, mode);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pPlayer;
}

//===========================================
// 操作処理
//===========================================
void CPlayer::Control(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置の取得

	// アクション操作処理
	ActionControl();

	// 障害物の当たり判定
	collision::PlayerObstcleAttack(this);

	if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCATCH)
	{ // 崖つかまり状態以外

		if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFLAND && 
			m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCLIMB && 
			m_pAction->GetAct() != CPlayerAct::ACT_HANGING)
		{ // 崖離し着地状態かつ、崖離し状態以外

			// プレイヤーの移動処理
			Move();

			// ジャンプ処理
			Jump();
		}
	}

	// 位置の設定処理
	SetPos(pos);

	if (m_bJump == true)
	{ // ジャンプ中の場合

		if (m_nJumpCount % PLAYER_FALLSOUND_COUNT == 0 &&
			m_move.y <= PLAYER_FALLSOUND_GRAVITY)
		{ // 一定カウント数経過したかつ、重力が一定以下の場合

			// 落下音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FALLING);
		}

		// ジャンプカウントを加算する
		m_nJumpCount++;
	}
	else
	{ // 上記以外

		// ジャンプカウントをリセットする
		m_nJumpCount = 0;
	}
}

//===========================================
// 重力処理
//===========================================
void CPlayer::PlayerGravity(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置を取得する

	if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCATCH)
	{ // 崖つかまり状態以外

		if (m_pAction->GetAct() == CPlayerAct::ACT_WALLKICK)
		{ // 壁キック状態の場合

			// 重力処理
			Gravity(&m_move.y, pos, PLAYER_WALLKICK_GRAVITY);
		}
		else if (m_pAction->GetAct() == CPlayerAct::ACT_WALLSTEPIN)
		{ // 壁踏み込み状態の場合

			// 移動量を 0 にする
			m_move.y = 0.0f;
		}
		else if (m_pAction->GetAct() == CPlayerAct::ACT::ACT_CLIFFCLIMB)
		{ // 崖離し状態の場合

			// 重力処理
			Gravity(&m_move.y, pos, m_pAction->GetCliffGravity());
		}
		else if(m_pAction->GetAct() != CPlayerAct::ACT::ACT_HANGING &&
			m_pAction->GetAct() != CPlayerAct::ACT_POLEDOWN)
		{ // ぶら下がり状態以外かつ、ポールダウン状態以外の場合

			// 重力処理
			Gravity(&m_move.y, pos, PLAYER_GRAVITY);
		}
	}

	// 位置の設定処理
	SetPos(pos);
}

//===========================================
// アクションボタンの操作処理
//===========================================
void CPlayer::ActionControl(void)
{
	if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW &&
		(CManager::GetInputKeyboard()->GetPress(DIK_SPACE) == true || CManager::GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_A, 0) == true))
	{ // 余韻状態以外かつ、SPACEキーを押していた場合

		// ボタンカウントを加算する
		m_button.nButtonCount++;

		if (m_fStamina >= PLAYER_BIGJUMP_STAMINA &&
			m_button.nButtonCount >= PLAYER_BIGJUMP_BTNCOUNT &&
			m_bJump == false)
		{ // 超ジャンプ分スタミナがあるかつ、ボタンカウントが一定以上になったかつ、ジャンプしていない場合

			if (m_button.nButtonCount % PLAYER_JUMPCHARGE_SOUND_COUNT == 0)
			{ // 超ジャンプ判定ボタンカウントが一定数に達した場合

				// 超ジャンプチャージ音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMPCHARGE);
			}

			// 超ジャンプのチャージのパーティクルを設定
			CParticle::Create(D3DXVECTOR3(GetPos().x, GetPos().y + (PLAYER_HEIGHT * 0.5f), GetPos().z), CEffect::TYPE_BIGJUMPCHARGE);
		}
	}

	if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW &&
		(CManager::GetInputKeyboard()->GetRelease(DIK_SPACE) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::JOYKEY_A, 0) == true))
	{ // SPACEキーを離した場合

		// 押し状況を true にする
		m_button.bSpacePush = true;
	}
	else
	{ // 上記以外

		if (m_button.bSpacePush == true)
		{ // 前回の押し状況が true の場合

			// ボタンカウントを0にする
			m_button.nButtonCount = 0;
		}

		// 押し状況を false にする
		m_button.bSpacePush = false;
	}
}

//===========================================
// プレイヤーの移動処理
//===========================================
void CPlayer::Move(void)
{
	if (m_pAction->GetAct() != CPlayerAct::ACT_WALLKICK &&
		m_pAction->GetAct() != CPlayerAct::ACT_LAMPJUMP && 
		m_pAction->GetAct() != CPlayerAct::ACT_POLEDOWN &&
		m_pAction->GetAct() != CPlayerAct::ACT_WALLSTEPIN)
	{ // アクション中以外の場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true ||
			CManager::GetInputGamePad()->GetGameStickLXPress(0) < 0)
		{ // Aキーを押していた場合

			// 移動の共通の処理
			MoveProcess();

			if (m_bRight == true)
			{ // 右向き状況が true の場合

				// 左を向く
				m_bRight = false;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true ||
			CManager::GetInputGamePad()->GetGameStickLXPress(0) > 0)
		{ // Dキーを押している場合

			// 移動の共通の処理
			MoveProcess();

			if (m_bRight == false)
			{ // 右向き状況が false の場合

				// 右を向く
				m_bRight = true;
			}
		}
		else
		{ // 何も押していない場合

			if(m_pAction->GetAct() != CPlayerAct::ACT_WALLKICK)
			{ // 壁キック中じゃない場合

				if (m_bJump == true)
				{ // ジャンプ中の場合

					// 目標の移動量を設定する
					m_fMoveDest = 0.0f;
				}
				else
				{ // 地面に立っている場合

					// 移動量を更新(減衰する)
					m_move.x += (0.0f - m_move.x) * PLAYER_MOVE_DAMP_MAGNI;
				}
			}

			if (m_bMove == true)
			{ // 移動状況が true の場合

				// 移動状況を false にする
				m_bMove = false;

				if (m_pMotion->GetType() != CPlayer::MOTIONTYPE_BIGJUMP &&
					m_pMotion->GetType() != MOTIONTYPE_LANDING)
				{ // 超ジャンプモーション・着地モーション以外の場合

					// 通常モーションに設定する
					m_pMotion->Set(MOTIONTYPE_NEUTRAL);
				}
			}
		}
	}

	if (m_pAction->GetAct() == CPlayerAct::ACT_WALLKICK)
	{ // 壁キック時の場合

		// 移動量を更新(減衰する)
		m_move.x += (0.0f - m_move.x) * PLAYER_WALLKICK_MOVE_DAMP_MAGNI;
	}
	else if (m_pAction->GetAct() == CPlayerAct::ACT_NONE)
	{ // 通常状態の場合

		if (m_bJump == true &&
			m_nJumpCount >= PLAYER_JUMP_ATTEN_COUNT)
		{ // 空中にいる場合

			// 移動量を設定する
			m_move.x *= PLAYER_JUMP_MOVE_MAGNI;
		}
	}

	if (m_bJump == true &&
		(m_pAction->GetAct() == CPlayerAct::ACT_NONE ||
			m_pAction->GetAct() == CPlayerAct::ACT_BIGJUMP ||
			m_pAction->GetAct() == CPlayerAct::ACT_WALLFRIC ||
			m_pAction->GetAct() == CPlayerAct::ACT_WALLAFTERGLOW))
	{ // ジャンプ中の場合

		// 移動量の補正処理
		Correct(m_fMoveDest, &m_move.x, PLAYER_JUMP_MOVE_CORRECT);
	}
}

//=======================================
// 移動の共通の処理
//=======================================
void CPlayer::MoveProcess(void)
{
	if (m_bJump == true)
	{ // 空中にいる場合

		// 移動量を設定する
		m_fMoveDest = sinf(GetRot().y) * m_fSpeed * PLAYER_JUMP_MOVE_MAGNI;
	}
	else
	{ // 地上にいる場合

		// 移動量を設定する
		m_move.x = sinf(GetRot().y) * m_fSpeed;
	}

	if (m_bMove == false)
	{ // 移動状況が false　の場合

		// 移動状況を true にする
		m_bMove = true;

		if (m_bJump == false)
		{ // 陸にいた場合

			// 走り始め音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RUNSTART);
		}
		
		if (m_pMotion->GetType() != CPlayer::MOTIONTYPE_BIGJUMP &&
			m_pMotion->GetType() != MOTIONTYPE_LANDING)
		{ // 超ジャンプ・着地モーション以外の場合

			// 移動モーションに設定する
			m_pMotion->Set(MOTIONTYPE_MOVE);
		}
	}
}

//=======================================
// 速度の星の発生処理
//=======================================
void CPlayer::SpeedStarEffect(void)
{
	if (m_bMove == true &&
		m_fSpeed >= PLAYER_MAX_SPEED)
	{ // 通常状態かつ、移動している場合

		// [引数用変数]
		D3DXVECTOR3 posEffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// エフェクトの位置
		int nLife = 0;											// 体力
		float fRadius = 0.0f;									// 半径

		if (m_bRight == true)
		{ // 右向きの場合

			// エフェクトの位置を設定する
			posEffect.x = GetPos().x + sinf(-GetRot().x) * (rand() % (int)(PLAYER_HEIGHT));
			posEffect.y = GetPos().y + cosf(-GetRot().x) * (rand() % (int)(PLAYER_HEIGHT));
			posEffect.z = GetPos().z;
		}
		else
		{ // 左向きの場合

			// エフェクトの位置を設定する
			posEffect.x = GetPos().x + sinf(GetRot().x) * (rand() % (int)(PLAYER_HEIGHT));
			posEffect.y = GetPos().y + cosf(GetRot().x) * (rand() % (int)(PLAYER_HEIGHT));
			posEffect.z = GetPos().z;
		}

		// 半径を設定する
		fRadius = (float)(rand() % 1000) / 100.0f + 15.0f;

		// 寿命の設定
		nLife = rand() % 15 + 45;

		// エフェクトの設定処理
		CEffect::Create
		(
			posEffect,		// 位置
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 移動量
			nLife,			// 寿命
			fRadius,		// 半径
			CEffect::TYPE_SPEEDSTAR			// 種類
		);
	}
}

//=======================================
// 走った後の埃の発生処理
//=======================================
void CPlayer::RunDustEffect(void)
{
	if (m_bMove == true &&
		m_bJump == false)
	{ // 地面を移動している場合

		// [引数用変数]
		D3DXVECTOR3 posEffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// エフェクトの位置
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		float fRadius = 0.0f;									// 半径

		// エフェクトの位置を設定する
		posEffect.x = GetPos().x + sinf(-GetRot().y) * (rand() % 10);
		posEffect.y = GetPos().y;
		posEffect.z = GetPos().z;

		// 移動量を設定する
		move.x = sinf(-GetRot().y) * (float)(rand() % 5 + (m_fSpeed * 0.5f));
		move.y = (float)(rand() % 3 + 2);
		move.z = 0.0f;

		// 半径を設定する
		fRadius = (float)((rand() % 8) + (m_fSpeed * 0.5f));

		// エフェクトの設定処理
		CEffect::Create
		(
			posEffect,		// 位置
			move,			// 移動量
			30,				// 寿命
			fRadius,		// 半径
			CEffect::TYPE_RUNDUST			// 種類
		);
	}
}

//=======================================
// ジャンプ処理
//=======================================
void CPlayer::Jump(void)
{
	if (m_bJump == false &&
		m_pAction->GetAct() != CPlayerAct::ACT_CLIFFLAND &&
		m_pAction->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW &&
		m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
		m_button.bSpacePush == true)
	{ // ジャンプ状況が false かつ、一定状態以外の場合

		// ジャンプする
		m_bJump = true;

		if (m_button.nButtonCount <= PLAYER_SONIC_JUMP_COUNT)
		{ // 最速ジャンプ

			// ジャンプさせる
			m_move.y = PLAYER_JUMP;

			// ジャンプ音を流す
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMP);

			// ジャンプモーションに設定する
			m_pMotion->Set(MOTIONTYPE_JUMP);
		}
		else
		{ // 上記以外

			if (m_fStamina >= PLAYER_BIGJUMP_STAMINA &&
				m_button.nButtonCount >= PLAYER_BIGJUMP_BTNCOUNT)
			{ // スタミナが余っているかつ、ボタンカウントが一定数の場合

				// ジャンプさせる
				m_move.y = PLAYER_BIGJUMP;

				// 行動状態変化処理
				m_pAction->ActionChange(CPlayerAct::ACT_BIGJUMP);

				// 軌跡の全頂点の位置の設定
				m_pOrbit->PosSet(GetPos());

				// アクションUI(超ジャンプ)を生成
				CActionUI::Create(CActionUI::TYPE_SUPERJUMP, PLAYER_SUPERJUMP_SCORE);

				// ジャンプモーションに設定する
				m_pMotion->Set(MOTIONTYPE_BIGJUMP);

				// スタミナの計算処理
				CalcStamina(-PLAYER_BIGJUMP_STAMINA);

				// ジャンプ音を流す
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BIGJUMP);

				// 速度を上げる
				SetSpeed(m_fSpeed + PLAYER_BIGJUMP_ADD_SPEED);
			}
			else
			{ // 上記以外

				// ジャンプさせる
				m_move.y = PLAYER_JUMP;

				// ジャンプモーションに設定する
				m_pMotion->Set(MOTIONTYPE_JUMP);

				// ジャンプ音を流す
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMP);
			}
		}
	}
}

//=======================================
// ヒット処理
//=======================================
void CPlayer::Hit(void)
{
	if (m_state == STATE_NONE)
	{ // 通常状態の場合

		// ダメージ状態にする
		SetState(STATE_DAMAGE);

		// ヒットストップの設定処理
		CGame::SetHitStop(DAMAGE_HITSTOP);
	}

	// 爆発パーティクルを生成
	CParticle::Create(GetPos(), CEffect::TYPE::TYPE_FIRE);
}

//=======================================
// 情報の設定処理
//=======================================
void CPlayer::SetData(const D3DXVECTOR3& pos, const CScene::MODE mode)
{
	// 全ての値を初期化する
	SetPos(pos);			// 位置
	SetPosOld(pos);			// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));			// 向き
	SetScale(NONE_SCALE);	// 拡大率

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// 初期化処理
		GetHierarchy(nCntData)->SetPos(pos);										// 位置
		GetHierarchy(nCntData)->SetPosOld(pos);										// 前回の位置
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);								// 拡大率
		GetHierarchy(nCntData)->SetFileData(CXFile::TYPE(INIT_PLAYER + nCntData));	// データの設定処理
	}

	// ローカル変数宣言
	CShadowCircle* pShadow = nullptr;		// 影へのポインタ

	// 影の設定処理
	pShadow = CShadowCircle::Create(pos, GetRot(), 40.0f);

	if (pShadow != nullptr)
	{ // 影のポインタが NULL以外の場合

		// 影のインデックス設定
		m_nShadowIdx = pShadow->GetNumID();
	}

	// 速度UIの生成処理
	m_pSpeedUI = CSpeedUI::Create(mode);

	// スタミナのメモリを確保する
	m_pStaminaUI = CStaminaUI::Create(mode);
}

//=======================================
// 状態処理
//=======================================
void CPlayer::State(void)
{
	switch (m_state)
	{
	case STATE_NONE:		// 通常状態

		// 通常状態処理
		StateNone();

		break;

	case STATE_BOOST:		// ブースト状態

		// ブースト状態処理
		StateBoost();

		break;

	case STATE_DAMAGE:		// ダメージ状態

		// ダメージ状態処理
		StateDamage();

		break;

	case STATE_INVINCIBLE:	// 無敵状態

		// 無敵状態処理
		StateInvincible();

		break;

	case STATE_CRUSH:		// 圧殺状態

		// 圧殺状態処理
		StateCrush();

		break;
	}
}

//=======================================
// 通常状態処理
//=======================================
void CPlayer::StateNone(void)
{
	// 色を設定する
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=======================================
// ブースト状態
//=======================================
void CPlayer::StateBoost(void)
{
	if (CManager::GetCamera() != nullptr &&
		CManager::GetCamera()->GetType() == CCamera::TYPE_NONE)
	{ // カメラが通常状態の場合

		// ブーストカメラを設定する
		CManager::GetCamera()->SetType(CCamera::TYPE_BOOST);
	}

	// 状態カウントを加算する
	m_nStateCount++;

	// 速度を速くする
	m_fSpeed = PLAYER_MAX_SPEED + PLAYER_BOOST_ADD_SPEED;

	if (m_nStateCount >= PLAYER_BOOST_COUNT)
	{ // 状態カウントが一定数に達した場合

		// 通常状態にする
		SetState(STATE_NONE);

		// スピードを戻す
		m_fSpeed = PLAYER_MAX_SPEED;
	}

	if (m_nStateCount % PLAYER_BOOSTSOUND_COUNT == 0)
	{ // 状態カウントが音を鳴らすカウントに達した場合

		// スーパーブーストの音を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SUPERDASH);
	}

	if (m_pSpeedUI != nullptr)
	{ // スタミナUIの情報が NULL じゃない場合

		// ローカル変数宣言
		D3DXVECTOR3 Effectpos;		// エフェクトの位置
		D3DXVECTOR3 size;			// スピードUIのメーターの位置

		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			// 位置を初期化する
			Effectpos = m_pSpeedUI->GetObject2D(CSpeedUI::TYPE_METER)->GetPos();
			size = m_pSpeedUI->GetObject2D(CSpeedUI::TYPE_METER)->GetSize();

			// 位置を設定する
			Effectpos.x = Effectpos.x + rand() % (int)(size.x);
			Effectpos.y = Effectpos.y + size.y;
			Effectpos.z = 0.0f;

			// 限界突破炎を出す
			CParticle2D::Create(Effectpos, CEffect2D::TYPE_OVERTOP);
		}
	}
}

//=======================================
// ダメージ状態処理
//=======================================
void CPlayer::StateDamage(void)
{
	// 状態カウントを加算する
	m_nStateCount++;

	// 色を赤に設定する
	m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	if (m_nStateCount >= PLAYER_DAMAGE_COUNT)
	{ // 状態カウントが一定数に達した場合

		// 無敵状態にする
		SetState(STATE_INVINCIBLE);

		// 色を通常に設定する
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

//=======================================
// 無敵状態処理
//=======================================
void CPlayer::StateInvincible(void)
{
	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount % 15 == 0)
	{ // 状態カウントが一定数になった場合

		if (m_col.a >= 1.0f)
		{ // データが 0.0f 以下の場合

			// データを 1.0f にする
			m_col.a = 0.0f;
		}
		else if (m_col.a <= 0.0f)
		{ // データが 1.0f 以上の場合

			// データを 0.0f にする
			m_col.a = 1.0f;
		}
	}

	if (m_nStateCount >= PLAYER_INVINCIBLE_COUNT)
	{ // 状態カウントが一定数に達した場合

		// 状態を設定する
		SetState(STATE_NONE);
	}
}

//=======================================
// 圧殺状態処理
//=======================================
void CPlayer::StateCrush(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置を取得する
	D3DXVECTOR3 scale = GetScale();		// 拡大率を取得する

	// 拡大率を下げる
	scale.y -= PLAYER_CRUSH_WIDTH_SUB;
	scale.z -= PLAYER_CRUSH_WIDTH_SUB;

	if (scale.y <= m_crush.scaleDest.y)
	{ // 拡大率が目標以下になった場合

		// 拡大率を設定する
		scale.y = m_crush.scaleDest.y;
	}

	if (scale.z <= m_crush.scaleDest.z)
	{ // 拡大率が目標以下になった場合

		// 拡大率を設定する
		scale.z = m_crush.scaleDest.z;
	}

	// 各モデルの拡大率の設定
	for (int nCnt = 0; nCnt < GetNumModel(); nCnt++)
	{
		if (GetHierarchy(nCnt) != nullptr)
		{ // モデルが NULL じゃない場合

			// 拡大率を設定する
			GetHierarchy(nCnt)->SetScale(scale);
		}
	}

	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount % PLAYER_CRUSH_COUNT == 0)
	{ // 状態カウントが一定数に達した場合

		// 全モデルの拡大率を直す
		for (int nCnt = 0; nCnt < GetNumModel(); nCnt++)
		{
			if (GetHierarchy(nCnt) != nullptr)
			{
				GetHierarchy(nCnt)->SetScale(NONE_SCALE);
			}
		}

		// 拡大率を設定する
		scale = NONE_SCALE;

		// 無敵状態にする
		m_state = STATE_INVINCIBLE;

		// 状態カウントを初期化する
		m_nStateCount = 0;

		// 上に少し飛び跳ねさせる
		pos.y = pos.y + PLAYER_CRUSH_FLY;
	}

	if (m_nStateCount == PLAYER_CRUSHVOICE_COUNT)
	{ // 状態カウントが一定数になった場合

		// 潰れた時の声を出す
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_PRESSVOICE);
	}

	// 情報を設定する
	SetPos(pos);		// 位置を設定する
	SetScale(scale);	// 拡大率を設定する
}

//=======================================
// 壁擦り処理
//=======================================
void CPlayer::WallFriction(bool &bRight, bool &bLeft)
{
	if (bRight == true || bLeft == true)
	{ // 右の壁または、左の壁に擦っている場合

		// 壁擦りカウントを加算する
		m_nWallFricCount++;
	}
	else if(m_bJump == false)
	{ // 壁に擦れていない場合

		// 壁擦りカウントを初期化する
		m_nWallFricCount = 0;
	}

	if (m_nWallFricCount >= PLAYER_WALLFRIC_COUNT)
	{ // 壁擦りカウントが一定数を超えた場合

		// 壁擦り判定を false にする
		bRight = false;
		bLeft = false;

		// アクションを設定する
		m_pAction->ActionChange(CPlayerAct::ACT_NONE);
	}

	if (bRight == true || bLeft == true)
	{ // 右の壁または、左の壁に擦っている場合

		if (m_pAction->GetAct() == CPlayerAct::ACT_WALLKICK)
		{ // 行動状況が 壁キック の場合

			// 行動の切り替え処理
			m_pAction->ActionChange(CPlayerAct::ACT_WALLAFTERGLOW);

			if (m_pMotion->GetType() != CPlayer::MOTIONTYPE_WALLFRIC)
			{ // 壁擦りモーション以外の場合

				// 壁擦りモーションの設定処理
				m_pMotion->Set(CPlayer::MOTIONTYPE_WALLFRIC);
			}
		}
		else if (m_pAction->GetAct() != CPlayerAct::ACT_WALLFRIC && 
			m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCATCH && 
			m_pAction->GetAct() != CPlayerAct::ACT_CLIFFFALL &&
			m_pAction->GetAct() != CPlayerAct::ACT_WALLAFTERGLOW)
		{ // 一定の行動状況じゃない場合

			// 行動の切り替え処理
			m_pAction->ActionChange(CPlayerAct::ACT_WALLFRIC);

			if (m_pMotion->GetType() != CPlayer::MOTIONTYPE_WALLFRIC)
			{ // 壁擦りモーション以外の場合

				// 壁擦りモーションの設定処理
				m_pMotion->Set(CPlayer::MOTIONTYPE_WALLFRIC);
			}
		}

		if (bRight == true)
		{ // 右壁に擦れていた場合

			// 右擦れを設定する
			m_pAction->SetRightLeave(true);
		}

		if (bLeft == true)
		{ // 左壁に擦れていた場合

			// 左擦れを設定する
			m_pAction->SetRightLeave(false);
		}
	}

	if (m_pAction->GetAct() == CPlayerAct::ACT_WALLFRIC &&
		bRight == false && 
		bLeft == false)
	{ // 壁を離した場合

		// 行動の切り替え処理
		m_pAction->ActionChange(CPlayerAct::ACT_WALLLEAVE);
	}
}

//=======================================
// 軌跡の破棄処理
//=======================================
void CPlayer::PlayerOrbit(void)
{
	if (m_pAction->GetAct() == CPlayerAct::ACT_BIGJUMP ||
		m_pAction->GetAct() == CPlayerAct::ACT_WALLKICK ||
		m_pAction->GetAct() == CPlayerAct::ACT_LAMPJUMP ||
		m_pAction->GetAct() == CPlayerAct::ACT_POLEDOWN)
	{ // 特殊アクション中の場合

		// 軌跡の親の位置の設定処理
		m_pOrbit->SetParentData(GetPos(), GetRot());

		// テクスチャの割り当て処理
		m_pOrbit->BindTexture(m_nOrbitIdx[ORBIT_TEX_NORMAL]);
	}
	else if (m_pAction->GetAct() == CPlayerAct::ACT_CLIFFCLIMB &&
		m_pAction->GetCliffCatch() == true)
	{ // 崖捕まり中かつ、崖登りに成功していた場合

		// 軌跡の親の位置の設定処理
		m_pOrbit->SetParentData(GetPos(), GetRot());

		// テクスチャの割り当て処理
		m_pOrbit->BindTexture(m_nOrbitIdx[ORBIT_TEX_NORMAL]);
	}
	else if (m_pAction->GetAct() == CPlayerAct::ACT_HANGING)
	{ // ぶら下がり状態の場合

		if (m_bRight == true)
		{ // 右を向いている場合

			// 軌跡の親の位置の設定処理
			m_pOrbit->SetParentData(GetPos(), -GetRot());
		}
		else
		{ // 左を向いている場合

			// 軌跡の親の位置の設定処理
			m_pOrbit->SetParentData(GetPos(), GetRot());
		}

		// テクスチャの割り当て処理
		m_pOrbit->BindTexture(m_nOrbitIdx[ORBIT_TEX_POLE]);
	}
	else
	{ // 上記以外

		//// 軌跡の位置を全設定する
		//m_pOrbit->PosSet(GetPos());
	}
}

//=======================================
// 高速演出の処理
//=======================================
void CPlayer::PlayerSonic(void)
{
		if (m_fSpeed >= PLAYER_MAX_SPEED)
	{ // 最高速の場合

		if (m_pSonic == nullptr)
		{ // 軌跡がない場合

			// 軌跡を生成する
			m_pSonic = CSonic::Create();
		}
	}
	else
	{ // 上記以外

		if (m_pSonic != nullptr)
		{ // 軌跡が存在する場合

			// 軌跡の終了処理
			m_pSonic->Uninit();

			// 軌跡を NULL にする
			m_pSonic = nullptr;
		}
	}
}

//=======================================
// 壁キック処理
//=======================================
void CPlayer::WallKick(bool bRight, bool bLeft)
{
	// ジャンプのみバージョン
	if (WallKickJump(bRight, bLeft) == false)
	{ // 壁キック判定が通らなかった場合

		// 方向キーバージョン
		WallKickDirect();
	}
}

//=======================================
// 壁キック(ジャンプのみ判定)
//=======================================
bool CPlayer::WallKickJump(bool bRight, bool bLeft)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向き

	if (m_fStamina >= PLAYER_WALLKICK_STAMINA && 
		(CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true))
	{ // SPACEキーを押したかつ、スタミナが余っていた場合

		if (bRight == true)
		{ // 右壁から離れた場合

			// 右壁を蹴った
			m_pAction->SetRightKick(true);

			// 壁踏み込み状態にする
			m_pAction->ActionChange(CPlayerAct::ACT_WALLSTEPIN);

			// 壁踏み込みモーションを設定する
			m_pMotion->Set(MOTIONTYPE_WALLSTEPIN);

			// 壁キックした
			return true;
		}

		if (bLeft == true)
		{ // 左壁から離れた場合

			// 左壁を蹴った
			m_pAction->SetRightKick(false);

			// 壁踏み込み状態にする
			m_pAction->ActionChange(CPlayerAct::ACT_WALLSTEPIN);

			// 壁踏み込みモーションを設定する
			m_pMotion->Set(MOTIONTYPE_WALLSTEPIN);

			// 壁キックした
			return true;
		}
	}

	// 向きの設定処理
	SetRot(rot);

	// 壁キック失敗
	return false;
}

//=======================================
// 壁キック(方向キー付き)
//=======================================
void CPlayer::WallKickDirect(void)
{
	if (m_fStamina >= PLAYER_WALLKICK_STAMINA && 
		(CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true) &&
		m_pAction->GetAct() == CPlayerAct::ACT_WALLLEAVE)
	{ // SPACEキーを押したかつ、スタミナが余っていた場合

		if (m_bRight == true && 
			m_pAction->GetRightLeave() == true &&
			CManager::GetInputKeyboard()->GetPress(DIK_D) == true || 
			CManager::GetInputGamePad()->GetGameStickLXPress(0) > 0)
		{ // 右壁から離れた場合

			// 右壁を蹴った
			m_pAction->SetRightKick(true);

			// 壁踏み込み状態にする
			m_pAction->ActionChange(CPlayerAct::ACT_WALLSTEPIN);

			// 壁踏み込みモーションを設定する
			m_pMotion->Set(MOTIONTYPE_WALLSTEPIN);
		}
		else if(m_bRight == false && 
			m_pAction->GetRightLeave() == false &&
			CManager::GetInputKeyboard()->GetPress(DIK_A) == true ||
			CManager::GetInputGamePad()->GetGameStickLXPress(0) < 0)
		{ // 左壁から離れた場合

			// 左壁を蹴った
			m_pAction->SetRightKick(false);

			// 壁踏み込み状態にする
			m_pAction->ActionChange(CPlayerAct::ACT_WALLSTEPIN);

			// 壁踏み込みモーションを設定する
			m_pMotion->Set(MOTIONTYPE_WALLSTEPIN);
		}
	}
}

//=======================================
// ステージの当たり判定
//=======================================
void CPlayer::StageCollision(bool* pRight, bool* pLeft)
{
	// ローカル変数宣言
	bool bJump = true;			// 一時的なジャンプ状況

	// 標識との当たり判定
	if (SignPoleCollision() == true)
	{ // 標識との当たり判定が true だった場合

		if (bJump == true)
		{ // ジャンプ状況が true の場合

			// ジャンプ状況を false にする
			bJump = false;

			// ジャンプ状況を false にする
			m_bJump = false;
		}
	}

	// 障害物の当たり判定
	if (ObstacleCollision() == true)
	{ // 障害物との当たり判定が true だった場合

		if (bJump == true)
		{ // ジャンプ状況が true の場合

			// ジャンプ状況を false にする
			bJump = false;

			// ジャンプ状況を false にする
			m_bJump = false;
		}
	}

	// 当たり判定ポリゴンの当たり判定
	if (CollPolygonCollision() == true)
	{ // 当たり判定ポリゴンとの当たり判定が true だった場合

		if (bJump == true)
		{ // ジャンプ状況が true の場合

			// ジャンプ状況を false にする
			bJump = false;

			// ジャンプ状況を false にする
			m_bJump = false;
		}
	}

	// ブロックの当たり判定
	if (BlockCollision(pRight, pLeft) == true)
	{ // ブロックの当たり判定が true だった場合

		if (bJump == true)
		{ // ジャンプ状況が true の場合

			// ジャンプ状況を false にする
			bJump = false;

			// ジャンプ状況を false にする
			m_bJump = false;
		}
	}

	// ステージのめり込み処理
	StagePenetration();

	// ジャンプ状況を設定する
	m_bJump = bJump;

	if (m_bJump == true)
	{ // ジャンプ状況が true の場合

		// 位置の設定処理(Y軸無し)
		CShadowCircle::SetPosRotXZ(m_nShadowIdx, GetPos(), GetRot());
	}
	else
	{ // 上記以外

		// 影の位置向き設定処理
		CShadowCircle::SetPosRot(m_nShadowIdx, GetPos(), GetRot());
	}

	if (m_state != STATE_INVINCIBLE &&
		m_state != STATE_CRUSH)
	{ // 無敵状態以外かつ、圧死状態以外の場合

		if (m_crush.bTop == true &&
			m_crush.bBottom == true)
		{ // 上下から挟まれた場合

			// 圧殺状態に設定する
			SetState(STATE_CRUSH);

			// 目標の拡大率を設定する
			m_crush.scaleDest = D3DXVECTOR3(1.0f, PLAYER_CRUSH_WIDTH, 1.0f);

			// 速度を半分にする
			SetSpeed(PLAYER_MIN_SPEED + ((GetSpeed() - PLAYER_MIN_SPEED) * 0.5f));
		}
		else if (m_crush.bLeft == true && 
			m_crush.bRight == true)
		{ // 左右から挟まれた場合

			// 圧殺状態に設定する
			SetState(STATE_CRUSH);

			// 目標の拡大率を設定する
			m_crush.scaleDest = D3DXVECTOR3(1.0f, 1.0f, PLAYER_CRUSH_WIDTH);

			// 速度を半分にする
			SetSpeed(PLAYER_MIN_SPEED + ((GetSpeed() - PLAYER_MIN_SPEED) * 0.5f));
		}
	}

	// 圧殺判定を全て false にする
	m_crush.bTop = false;			// 上
	m_crush.bBottom = false;		// 下
	m_crush.bRight = false;			// 右
	m_crush.bLeft = false;			// 左
}

//=======================================
// 障害物の当たり判定処理
//=======================================
bool CPlayer::SignPoleCollision(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;			// オブジェクトのポインタ
	CObstacle* pObstacle = nullptr;		// 障害物のポインタ
	CSignPole* pSignPole = nullptr;		// 標識のポインタ
	float fHeight = 0.0f;				// 高さ
	D3DXVECTOR3 pos = GetPos();			// 位置の取得処理
	bool bLand = false;					// 着地状況

	// オブジェクトの情報を取得する
	pObj = CObject::GetTop(PRIORITY_BLOCK);

	while(pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == TYPE_OBSTACLE)
		{ // 障害物だった場合

			// 障害物の情報を取得する
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // 障害物の情報が NULL じゃない場合

				if (pObstacle->GetType() == CObstacle::TYPE_SIGNPOLE)
				{ // 種類が標識だった場合

					// 標識の情報を取得する
					pSignPole = dynamic_cast<CSignPole*>(pObstacle);

					if (pSignPole != nullptr)
					{ // 標識の情報を NULL じゃなかった場合

						// 当たり判定
						fHeight = pSignPole->ElevationCollision(pos);

						if (fHeight + PLAYER_SIGNPOLE_WALK >= pos.y &&
							pSignPole->GetPos().y - PLAYER_SIGNPOLE_WALK <= pos.y &&
							pSignPole->GetVtx(0).x >= pos.x &&
							pSignPole->GetVtx(2).x <= pos.x &&
							pSignPole->GetState() != CSignPole::STATE_NONE)
						{ // 現在の高さよりも低いかつ、標識の範囲内かつ、通常状態以外の場合

							// 位置を設定する
							pos.y = fHeight + PLAYER_SIGNPOLE_WALK;

							// 移動量を初期化する
							m_move.y = 0.0f;

							// 行動状態変化処理
							m_pAction->ActionChange(CPlayerAct::ACT_NONE);

							if(m_bMove == false &&
								m_pMotion->GetType() != MOTIONTYPE_NEUTRAL)
							{ // 通常モーションが設定されていなかった場合

								// 通常モーションを設定する
								m_pMotion->Set(MOTIONTYPE_NEUTRAL);
							}
							else if (m_bMove == true &&
								m_pMotion->GetType() != MOTIONTYPE_MOVE)
							{ // 移動モーションが設定されていなかった場合

								// 移動モーションを設定する
								m_pMotion->Set(MOTIONTYPE_MOVE);
							}

							// 着地状況を true にする
							bLand = true;
						}
					}
				}
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}

	// 位置の設定処理
	SetPos(pos);

	// 着地状況を返す
	return bLand;
}

//=======================================
// 地面着地処理
//=======================================
void CPlayer::GroundLanding(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置を取得する

	if (pos.y <= 0.0f)
	{ // 地面に達した場合

		if (m_bJump == true)
		{ // ジャンプが true の場合

			// ジャンプ状況を false にする
			m_bJump = false;

			// 着地音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
		}

		// 重力をリセットする
		m_move.y = 0.0f;

		// 位置を設定する
		pos.y = 0.0f;

		if (m_pAction->GetAct() == CPlayerAct::ACT_POLEDOWN)
		{ // ポール下り中の場合

			// 位置を設定する
			pos.x = m_pAction->GetPosObject().x;
			pos.z = 0.0f;
		}

		// 行動状態変化処理
		m_pAction->ActionChange(CPlayerAct::ACT_NONE);

		if (m_bMove == true)
		{ // 移動している場合

			if (m_pMotion->GetType() != MOTIONTYPE_MOVE)
			{ // 移動モーション以外の場合

				// 移動モーションに設定する
				m_pMotion->Set(MOTIONTYPE_MOVE);
			}
		}
		else
		{ // 移動していない場合

			if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL)
			{ // 通常モーション以外の場合

				// 通常モーションに設定する
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}
	}

	// 位置の設定処理
	SetPos(pos);
}

//=======================================
// スタミナの加算処理
//=======================================
void CPlayer::CalcStamina(float fAdd)
{
	// スタミナを加算する
	m_fStamina += fAdd;

	if (m_fStamina <= 0.0f)
	{ // スタミナの最小数を超えた場合

		// スタミナを 0.0f にする
		m_fStamina = 0.0f;
	}

	if (m_fStamina >= PLAYER_MAX_STAMINA)
	{ // スタミナの最大数を超えた場合

		// スタミナを MAX にする
		m_fStamina = PLAYER_MAX_STAMINA;
	}
}

//=======================================
// 街灯の設定処理
//=======================================
void CPlayer::SetStreetLamp(CStreetLamp* pTarget)
{
	// 街灯の情報を設定する
	m_pStreetLamp = pTarget;
}

//=======================================
// 街灯の取得処理
//=======================================
CStreetLamp* CPlayer::GetStreetLamp(void) const
{
	// 街灯の情報を返す
	return m_pStreetLamp;
}

//=======================================
// 速度UIの取得処理
//=======================================
CSpeedUI* CPlayer::GetSpeedUI(void)
{
	// 速度UIを返す
	return m_pSpeedUI;
}

//======================================
// スタミナUIの取得処理
//======================================
CStaminaUI* CPlayer::GetStaminaUI(void)
{
	// スタミナUIの情報を返す
	return m_pStaminaUI;
}

//=======================================
// スタミナの取得処理
//=======================================
float CPlayer::GetStamina(void)
{
	// スタミナを返す
	return m_fStamina;
}

//=======================================
// ブロックの当たり判定
//=======================================
bool CPlayer::BlockCollision(bool* pRight, bool* pLeft)
{
	if (collision::PlayerBlockCollision(pRight, pLeft, this, PLAYER_WIDTH, PLAYER_HEIGHT) == true)
	{ // 当たり判定が true の場合

		// 重力を0.0fにする
		m_move.y = 0.0f;

		if (m_bMove == true)
		{ // 移動している場合

			if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFLAND)
			{ // 崖登り状態以外の場合

				if (m_pMotion->GetType() != MOTIONTYPE_MOVE &&
					m_pMotion->GetType() != MOTIONTYPE_LANDING &&
					m_pMotion->GetType() != MOTIONTYPE_ATTACK)
				{ // 移動モーション、着地モーション、攻撃モーション以外の場合

					// 移動モーションに設定する
					m_pMotion->Set(MOTIONTYPE_MOVE);
				}
			}
		}
		else
		{ // 移動していない場合

			if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // 通常モーション、着地モーション、攻撃モーション以外の場合

				// 通常モーションに設定する
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}

		// true を返す
		return true;
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//=======================================
// 障害物の当たり判定
//=======================================
bool CPlayer::ObstacleCollision(void)
{
	if (collision::PlayerObstacleCollision(this, PLAYER_WIDTH, PLAYER_HEIGHT) == true)
	{ // 当たり判定が true の場合

		// 重力を0.0fにする
		m_move.y = 0.0f;

		if (m_bMove == true)
		{ // 移動している場合

			if (m_pMotion->GetType() != MOTIONTYPE_MOVE &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // 移動モーション、着地モーション、攻撃モーション以外の場合

				// 移動モーションに設定する
				m_pMotion->Set(MOTIONTYPE_MOVE);
			}
		}
		else
		{ // 移動していない場合

			if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // 通常モーション、着地モーション、攻撃モーション以外の場合

				// 通常モーションに設定する
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}

		// true を返す
		return true;
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//=======================================
// 当たり判定ポリゴンの当たり判定
//=======================================
bool CPlayer::CollPolygonCollision(void)
{
	if (collision::PlayerCollPolygonCollision(this, PLAYER_WIDTH, PLAYER_HEIGHT) == true)
	{ // 当たり判定が true の場合

		// 重力を0.0fにする
		m_move.y = 0.0f;

		if (m_bMove == true)
		{ // 移動している場合

			if (m_pMotion->GetType() != MOTIONTYPE_MOVE &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // 移動モーション、着地モーション、攻撃モーション以外の場合

				// 移動モーションに設定する
				m_pMotion->Set(MOTIONTYPE_MOVE);
			}
		}
		else
		{ // 移動していない場合

			if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // 通常モーション、着地モーション、攻撃モーション以外の場合

				// 通常モーションに設定する
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}

		// true を返す
		return true;
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//=======================================
// めり込み判定処理
//=======================================
void CPlayer::StagePenetration(void)
{
	// ブロックとのめり込み処理
	collision::PlayerBlockPenetration(this, PLAYER_WIDTH, PLAYER_HEIGHT);

	// 障害物とのめり込み処理
	collision::PlayerObstaclePenetration(this, PLAYER_WIDTH, PLAYER_HEIGHT);

	// 当たり判定ポリゴンとのめり込み処理
	collision::PlayerCollPolygonPenetration(this, PLAYER_WIDTH, PLAYER_HEIGHT);
}