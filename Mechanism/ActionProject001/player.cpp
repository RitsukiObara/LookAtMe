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
#include "player_act.h"
#include "player_ability.h"
#include "screwUI.h"
#include "lifeUI.h"
#include "combo.h"
#include "combo_magni.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "model.h"
#include "sound.h"
#include "useful.h"
#include "texture.h"

#include "camera.h"
#include "collision.h"
#include "elevation_manager.h"
#include "objectElevation.h"
#include "Particle.h"
#include "motion.h"
#include "shadowCircle.h"
#include "game_score.h"
#include "goal.h"
#include "bonus.h"
#include "destruction.h"
#include "ripple.h"
#include "push_timing.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_LIFE				(3)			// 体力の最大数
#define STEPHIT_JUMP			(20.0f)		// 踏みつけたときのジャンプ力
#define COLLISION_WIDTH			(20.0f)		// 当たり判定時に使う時の幅
#define COLLISION_HEIGHT		(150.0f)	// 当たり判定時に使う時の高さ
#define COLLISION_DEPTH			(20.0f)		// 当たり判定時に使う時の奥行
#define START_CAMERA_POSR_Y		(100.0f)	// スタート時のカメラの注視点(Y軸)
#define START_CAMERA_POSV_Y		(130.0f)	// スタート時のカメラの視点(Y軸)
#define START_CAMERA_DISTANCE	(200.0f)	// スタート時のカメラの距離
#define START_COUNT				(40)		// スタートのカウント
#define PUNCH_COUNT				(150)		// パンチ状態のカウント数
#define GOAL_COUNT				(80)		// ゴール状態のカウント数
#define LEAVE_GRAVITY			(0.4f)		// 退場状態の重力
#define FINISH_GRAVITY			(-0.6f)		// 終了状態の重力
#define NEAR_POS				(0.0f)		// 手前の位置
#define FAR_POS					(1000.0f)	// 奥行の位置
#define ADD_GRAVITY				(-50.0f)	// 追加の重力
#define BOUND_SPEED				(-5.0f)		// バウンド時のスピード
#define FALL_HEIGHT				(-600.0f)	// 落ちた判定が通る所
#define PUNCH_DSTR_SHIFT		(D3DXVECTOR3(40.0f, 100.0f, 0.0f))		// パンチ時の撃破のずらす幅
#define PUNCH_DSTR_SIZE			(D3DXVECTOR3(100.0f, 100.0f, 0.0f))		// パンチ時の撃破のサイズ
#define PUNCH_DSTR_COL			(D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f))		// パンチ時の撃破の色
#define PUNCH_DSTR_LIFE			(6)			// パンチ時の撃破の寿命
#define PUNCH_RIPPLE_SHIFT		(D3DXVECTOR3(45.0f, 100.0f, 0.0f))		// パンチ時の波紋のずらす幅
#define OUT_RANGE_GRAVITY		(-5.0f)		// 範囲外に出たときの重力
#define ADD_START_POS			(D3DXVECTOR3(20.0f, -20.0f, 0.0f))		// スタート状態の加算する位置

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CPlayer* CPlayer::m_pPlayer = nullptr;		// プレイヤーのポインタ

//=========================================
// コンストラクタ
//=========================================
CPlayer::CPlayer() : CCharacter(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_pMotion = nullptr;			// モーションの情報
	m_pAction = nullptr;			// プレイヤーの行動の情報
	m_pAbility = nullptr;			// 能力の情報
	m_pScrewUI = nullptr;			// ネジUIの情報
	m_pLifeUI = nullptr;			// 体力UIの情報
	m_pCombo = nullptr;				// コンボの情報
	m_pPushTiming = nullptr;		// 押しボタンのタイミング
	m_pBlock = nullptr;				// ブロックの情報
	m_posDest = NONE_D3DXVECTOR3;	// 目的の位置
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_rotDest = NONE_D3DXVECTOR3;	// 目的の向き
	m_nShadowIdx = INIT_SHADOW;		// 影のインデックス
	m_nStartCount = 0;				// スタートカウント
	m_nGoalCount = 0;				// ゴール時のカウント
	m_nLife = MAX_LIFE;				// 体力
	m_fSpeed = 0.0f;				// 速度
	m_fAlpha = 1.0f;				// 透明度
	m_bMove = false;				// 移動状況
	m_bRight = true;				// 右向き状況
	m_bJump = false;				// ジャンプ状況
	m_bPunch = false;				// パンチ状況
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

	// ベタ打ち
	SetNumModel(16);

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
		m_pMotion->SetModel(GetHierarchy(), GetNumModel());

		// ロード処理
		m_pMotion->Load(CMotion::TYPE_PLAYER);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	// モーションの設定処理
	m_pMotion->Set(MOTIONTYPE_APPEAR);

	if (m_pAction == nullptr)
	{ // 行動状態が NULL の場合

		// プレイヤーの行動状態を生成
		m_pAction = CPlayerAct::Create();
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}

	if (m_pAbility == nullptr)
	{ // 能力状態が NULL の場合

		// 能力の情報を生成
		m_pAbility = CAbility::Create();
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}

	if (m_pScrewUI == nullptr)
	{ // ネジUIが NULL の場合

		// ネジUIの情報を生成
		m_pScrewUI = CScrewUI::Create();
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}

	if (m_pCombo == nullptr)
	{ // コンボが NULL の場合

		// コンボの情報を生成
		m_pCombo = CCombo::Create();
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}

	if (m_pLifeUI == nullptr)
	{ // 体力UIが NULL の場合

		// 体力UIの情報を生成
		m_pLifeUI = CLifeUI::Create();
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}

	// 全ての値を初期化する
	m_pPushTiming = nullptr;		// 押しボタンのタイミング
	m_pBlock = nullptr;				// ブロックの情報
	m_posDest = NONE_D3DXVECTOR3;	// 目的の位置
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_rotDest = NONE_D3DXVECTOR3;	// 目的の向き
	m_nShadowIdx = INIT_SHADOW;		// 影のインデックス
	m_nStartCount = 0;				// スタートカウント
	m_nGoalCount = 0;				// ゴール時のカウント
	m_nLife = MAX_LIFE;				// 体力
	m_fSpeed = 0.0f;				// 速度
	m_fAlpha = 1.0f;				// 透明度
	m_bMove = false;				// 移動状況
	m_bRight = true;				// 右向き状況
	m_bJump = false;				// ジャンプ状況
	m_bPunch = false;				// パンチ状況

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

	// 行動の終了処理
	m_pAction->Uninit();
	m_pAction = nullptr;

	// 能力の終了処理
	m_pAbility->Uninit();
	m_pAbility = nullptr;

	// ネジUIの終了処理
	m_pScrewUI->Uninit();
	m_pScrewUI = nullptr;

	// ブロックのポインタを NULL にする
	m_pBlock = nullptr;

	// 終了処理
	CCharacter::Uninit();

	// プレイヤーを NULL にする
	m_pPlayer = nullptr;
}

//===========================================
// 更新処理
//===========================================
void CPlayer::Update(void)
{
	// ジャンプ状況を取得する
	bool bJump = m_bJump;

	// 前回の位置の設定処理
	SetPosOld(GetPos());

	switch (CGame::GetState())
	{
	case CGame::STATE_START:

		// スタート状態の処理
		StartProcess();

		break;

	case CGame::STATE_PLAY:

		// 行動の更新処理
		m_pAction->Update(*this);

		// 飛行機との当たり判定
		collision::AirplaneHit(*this);

		// ネジとの当たり判定
		collision::ScrewHit(*this);

		// 敵との当たり判定
		collision::EnemyHit(*this);

		// 敵とのめり込み判定処理
		collision::EnemyPenetrate(*this);

		// ゴールとの当たり判定
		collision::GoalHit(*this);

		// 棘との当たり判定
		collision::NeedleHit(*this);

		break;

	case CGame::STATE_GOAL:

		// ゴール状態の処理
		GoalProcess();

		break;

	case CGame::STATE_LEAVE:

		// 退場状態の処理
		LeaveProcess();

		break;

	case CGame::STATE_FINISH:

		// 終了状態の処理
		FinishProcess();

		break;

	case CGame::STATE_DEATH:

		// 行動の更新処理
		m_pAction->Update(*this);

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// モーションの更新処理
	m_pMotion->Update();

	// 体力UIの更新処理
	m_pLifeUI->Update();

	if (m_pAction->GetState() != CPlayerAct::STATE_FALL)
	{ // 落下状態以外の場合

		// 起伏地面との当たり判定処理
		ElevationCollision();

		// ブロックとの当たり判定
		BlockCollision();

		// 台との当たり判定
		TableCollision();

		// 行動制限判定
		CollisionMagicWall();
	}

	if (m_bJump == false &&
		bJump == true)
	{ // 着地した瞬間

		if (m_pMotion->GetType() != MOTIONTYPE_JETDASH &&
			m_pMotion->GetType() != MOTIONTYPE_APPEAR)
		{ // ジェットダッシュモーション以外の場合

			// 着地モーションを設定する
			m_pMotion->Set(MOTIONTYPE_LANDING);
		}

		// 着地音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
	}

	// 落下判定処理
	FallCheck();

	if (m_bJump == true)
	{ // ジャンプしている場合

		// 影の位置向きの設定処理
		CShadowCircle::SetPosRotXZ(m_nShadowIdx, GetPos(), GetRot());
	}
	else
	{ // ジャンプしていない場合

		// 影の位置向きの設定処理
		CShadowCircle::SetPosRot(m_nShadowIdx, GetPos(), GetRot());
	}

	// 範囲外の当たり判定
	OutRangeCollision();

	// プレイヤーの情報を表示
	CManager::Get()->GetDebugProc()->Print("位置：%f %f %f\n移動量：%f %f %f\nプレイヤーの状態：%d\nジャンプ状況：%d\n", GetPos().x, GetPos().y, GetPos().z, m_move.x, m_move.y, m_move.z, m_pAction->GetState(), m_bJump);
}

//===========================================
// プレイヤーの描画処理
//===========================================
void CPlayer::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	switch (m_pAction->GetState())
	{
	case CPlayerAct::STATE_INVINCIBLE:		// 無敵状態

		// 描画処理
		CCharacter::Draw(m_fAlpha);

		break;

	case CPlayerAct::STATE_DEATH:			// 死亡状態
	case CPlayerAct::STATE_DAMAGE:			// ダメージ状態

		// Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

		// 描画処理
		CCharacter::Draw(m_fAlpha);

		// Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Zテストの設定
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zテストの有効/無効設定

		break;

	default:

		// 描画処理
		CCharacter::Draw();

		break;
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
// アクションの情報の設定処理
//===========================================
CPlayerAct* CPlayer::GetAction(void) const
{
	// 行動状態の情報を返す
	return m_pAction;
}

//===========================================
// 能力の情報の設定処理
//===========================================
CAbility* CPlayer::GetAbility(void) const
{
	// 能力の情報を返す
	return m_pAbility;
}

//===========================================
// ネジUIの情報の取得処理
//===========================================
CScrewUI* CPlayer::GetScrewUI(void) const
{
	// ネジUIの情報を返す
	return m_pScrewUI;
}

//===========================================
// 体力UIの情報の取得処理
//===========================================
CLifeUI* CPlayer::GetLifeUI(void) const
{
	// 体力UIの情報を返す
	return m_pLifeUI;
}

//===========================================
// コンボの情報の取得処理
//===========================================
CCombo* CPlayer::GetCombo(void) const
{
	// コンボの情報を返す
	return m_pCombo;
}

//===========================================
// 押しボタンの情報の設定処理
//===========================================
void CPlayer::SetPushTiming(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 押しボタンを生成
	m_pPushTiming = CPushTiming::Create(D3DXVECTOR3(pos.x + sinf(-rot.y) * 100.0f, pos.y + 160.0f, pos.z));
}

//===========================================
// 押しボタンの情報の取得処理
//===========================================
CPushTiming* CPlayer::GetPushTiming(void) const
{
	// 押しボタンを返す
	return m_pPushTiming;
}

//===========================================
// ブロックの情報の取得処理
//===========================================
CBlock* CPlayer::GetBlock(void) const
{
	// ブロックの情報を返す
	return m_pBlock;
}

//===========================================
// ブロックのNULL化処理
//===========================================
void CPlayer::DeleteBlock(void)
{
	// ブロックのポインタを NULL にする
	m_pBlock = nullptr;
}

//===========================================
// 取得処理
//===========================================
CPlayer* CPlayer::Get(void)
{
	if (m_pPlayer != nullptr)
	{ // プレイヤーの情報がある場合

		// プレイヤーのポインタを返す
		return m_pPlayer;
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// プレイヤーのポインタを返す
		return m_pPlayer;
	}
}

//===========================================
// 生成処理
//===========================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos)
{
	if (m_pPlayer == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		m_pPlayer = new CPlayer;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return m_pPlayer;
	}

	if (m_pPlayer != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(m_pPlayer->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "プレイヤーの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		m_pPlayer->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return m_pPlayer;
}

//=======================================
// ヒット処理
//=======================================
void CPlayer::Hit(void)
{
	// 向きを取得する
	D3DXVECTOR3 rot = GetRot();

	if (m_pAction->GetState() != CPlayerAct::STATE_DAMAGE &&
		m_pAction->GetState() != CPlayerAct::STATE_INVINCIBLE &&
		m_pAction->GetState() != CPlayerAct::STATE_CANNON &&
		m_pAction->GetState() != CPlayerAct::STATE_DEATH &&
		m_pAction->GetState() != CPlayerAct::STATE_FALL)
	{ // 一定の状態以外の場合

		// ダメージ音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);
		
		// 体力を減算する
		m_nLife--;

		if (m_nLife <= 0)
		{ // 寿命がもうない場合

			// 体力を0にする
			m_nLife = 0;

			// 死亡状態にする
			m_pAction->SetState(CPlayerAct::STATE_DEATH);

			// ゲームの状態を死亡状態にする
			CGame::SetState(CGame::STATE_DEATH);

			// 移動量を初期化する
			m_move = NONE_D3DXVECTOR3;

			// 速度を初期化する
			m_fSpeed = 0.0f;
		}
		else
		{ // 上記以外

			// ダメージ状態にする
			m_pAction->SetState(CPlayerAct::STATE_DAMAGE);
		}

		// ダメージモーションを設定する
		m_pMotion->Set(MOTIONTYPE_DAMAGE);

		// 向きを設定する
		rot.y = D3DX_PI;

		// 爆発パーティクルを生成
		CParticle::Create(GetPos(), CParticle::TYPE_FIRE);

		// 無能力状態にする
		m_pAbility->SetAbility(CAbility::ABILITY_NONE, *this);

		// 撃破の生成処理
		CDestruction::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 100.0f, GetPos().z), D3DXVECTOR3(100.0f, 100.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CDestruction::TYPE_EXPLOSION, 20);
	}

	// 向きを適用する
	SetRot(rot);
}

//=======================================
// 踏みつけ時の処理
//=======================================
void CPlayer::StepHit(void)
{
	// 移動量を上げる
	m_move.y = STEPHIT_JUMP;

	// 踏みつけ音を鳴らす
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_STEPPING);

	// コンボの加算処理
	m_pCombo->AddCombo(100);
}

//=======================================
// バウンド処理
//=======================================
void CPlayer::BoundHit(void)
{
	if (m_pAction->GetState() != CPlayerAct::STATE_BOUND &&
		m_pAction->GetState() != CPlayerAct::STATE_DAMAGE &&
		m_pAction->GetState() != CPlayerAct::STATE_INVINCIBLE)
	{ // 一定の状態以外の場合

		// バウンド状態にする
		m_pAction->SetState(CPlayerAct::STATE_BOUND);

		// 無能力状態にする
		m_pAbility->SetAbility(CAbility::ABILITY_NONE, *this);

		// 速度を設定する
		m_fSpeed = BOUND_SPEED;
	}
}

//=======================================
// 情報の設定処理
//=======================================
void CPlayer::SetData(const D3DXVECTOR3& pos)
{
	// 全ての値を初期化する
	m_posDest = pos;	// 目的の位置
	SetPos(D3DXVECTOR3(pos.x - 500.0f, pos.y + 400.0f, pos.z));		// 位置
	SetPosOld(GetPos());			// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));				// 向き
	SetScale(NONE_SCALE);			// 拡大率

	m_rotDest = GetRot();	// 目的の向きを設定する

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

	// モーションの設定処理
	m_pMotion->Set(MOTIONTYPE_APPEAR);

	// カメラを設定する
	CManager::Get()->GetCamera()->SetPosR(D3DXVECTOR3(pos.x, pos.y + START_CAMERA_POSR_Y, pos.z));
	CManager::Get()->GetCamera()->SetPosV(D3DXVECTOR3(pos.x, pos.y + START_CAMERA_POSV_Y, pos.z));
	CManager::Get()->GetCamera()->SetDistance(START_CAMERA_DISTANCE);
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
// 目標の向きの設定処理
//=======================================
void CPlayer::SetRotDest(const D3DXVECTOR3& rot)
{
	// 目標の向きを設定する
	m_rotDest = rot;
}

//=======================================
// 目標の向きの取得処理
//=======================================
D3DXVECTOR3 CPlayer::GetRotDest(void) const
{
	// 目標の向きを返す
	return m_rotDest;
}

//=======================================
// 体力の設定処理
//=======================================
void CPlayer::SetLife(const int nLife)
{
	if (nLife > MAX_LIFE)
	{ // 体力が一定以上の場合

		// 体力を最大に設定する
		m_nLife = MAX_LIFE;
	}
	else
	{ // 上記以外

		// 体力を設定する
		m_nLife = nLife;
	}
}

//=======================================
// 体力の取得処理
//=======================================
int CPlayer::GetLife(void) const
{
	// 体力を返す
	return m_nLife;
}

//=======================================
// 速度の設定処理
//=======================================
void CPlayer::SetSpeed(float fSpeed)
{
	// 速度を設定する
	m_fSpeed = fSpeed;
}

//=======================================
// 速度の取得処理
//=======================================
float CPlayer::GetSpeed(void) const
{
	// 速度を返す
	return m_fSpeed;
}

//=======================================
// 透明度の設定処理
//=======================================
void CPlayer::SetAlpha(const float fAlpha)
{
	// 透明度を設定する
	m_fAlpha = fAlpha;
}

//=======================================
// 透明度の入れ替え処理
//=======================================
void CPlayer::SwapAlpha(void)
{
	// 透明度を入れ替える
	m_fAlpha = (m_fAlpha >= 1.0f) ? 0.0f : 1.0f;
}

//=======================================
// 透明度の取得処理
//=======================================
float CPlayer::GetAlpha(void) const
{
	// 透明度を返す
	return m_fAlpha;
}

//=======================================
// 移動状況の設定処理
//=======================================
void CPlayer::SetEnableMove(bool bMove)
{
	// 移動状況を設定する
	m_bMove = bMove;
}

//=======================================
// 移動状況の取得処理
//=======================================
bool CPlayer::IsMove(void) const
{
	// 移動状況を返す
	return m_bMove;
}

//=======================================
// 右向き状況の設定処理
//=======================================
void CPlayer::SetEnableRight(bool bRight)
{
	// 右向き状況を設定する
	m_bRight = bRight;
}

//=======================================
// 右向き状況の取得処理
//=======================================
bool CPlayer::IsRight(void) const
{
	// 右向き状況を取得する
	return m_bRight;
}

//=======================================
// ジャンプ状況の設定処理
//=======================================
void CPlayer::SetEnableJump(bool bJump)
{
	// ジャンプ状況を設定する
	m_bJump = bJump;
}

//=======================================
// ジャンプ状況の取得処理
//=======================================
bool CPlayer::IsJump(void) const
{
	// ジャンプ状況を返す
	return m_bJump;
}

//=======================================
// パンチ状況の設定処理
//=======================================
void CPlayer::SetEnablePunch(bool bPunch)
{
	// パンチ状況を設定する
	m_bPunch = bPunch;
}

//=======================================
// パンチ状況の取得処理
//=======================================
bool CPlayer::IsPunch(void) const
{
	// パンチ状況を返す
	return m_bPunch;
}

//=======================================
// 起伏地面の当たり判定処理
//=======================================
void CPlayer::ElevationCollision(void)
{
	// ローカル変数宣言
	CElevation* pMesh = CElevationManager::Get()->GetTop();		// 起伏の先頭のオブジェクトを取得する
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ
	bool bJump = true;				// ジャンプ状況
	bool bRange = false;			// 範囲内状況

	while (pMesh != nullptr)
	{ // 地面の情報がある限り回す

		// 当たり判定を取る
		fHeight = pMesh->ElevationCollision(pos, bRange);
		
		if (pos.y < fHeight)
		{ // 当たり判定の位置が高かった場合

			// 高さを設定する
			pos.y = fHeight;

			// 重力を設定する
			m_move.y = ADD_GRAVITY;

			// ジャンプ状況を false にする
			bJump = false;

			if (m_pAction->GetState() == CPlayerAct::STATE_CANNON ||
				m_pAction->GetState() == CPlayerAct::STATE_FLY)
			{ // 通常状態以外の場合

				// 通常状態の設定処理
				SetNone();

				if (m_pAction->GetFront() == true)
				{ // 前後状況を設定する

					// 後ろにする
					pos.z = FAR_POS;

					// 前後状況を設定する
					m_pAction->SetFront(false);
				}
				else
				{ // 上記以外

					// 手前にする
					pos.z = NEAR_POS;

					// 前後状況を設定する
					m_pAction->SetFront(true);
				}

				// リセット処理
				m_pAbility->ResetAbility();
			}

			if (CGame::GetState() == CGame::STATE_LEAVE)
			{ // 退場状態の場合

				// 移動量を設定する
				m_move.x = 0.0f;
				m_move.y = 0.0f;

				// 終了状態にする
				CGame::SetState(CGame::STATE_FINISH);
			}
		}

		// 次のポインタを取得する
		pMesh = pMesh->GetNext();
	}

	if (bRange == false &&
		m_bJump != bJump)
	{ // 範囲外に出た瞬間

		// 重力を無くす
		m_move.y = OUT_RANGE_GRAVITY;
	}

	// ジャンプ状況を代入する
	m_bJump = bJump;

	// 位置を更新する
	SetPos(pos);
}

//=======================================
// 着地時の通常状態の設定処理
//=======================================
void CPlayer::SetNone(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 通常状態にする
	m_pAction->SetState(CPlayerAct::STATE_NONE);

	// 移動量を設定する
	m_move.z = 0.0f;

	// 向きを設定する
	rot.x = 0.0f;
	rot.z = 0.0f;

	if (m_bRight == true)
	{ // 右向き状況が true の場合

		// 右を向かせる
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // 上記以外

		// 左を向かせる
		rot.y = -D3DX_PI * 0.5f;
	}

	// 情報の設定処理
	SetRot(rot);			// 向きを設定する
}

//=======================================
// 移動制限判定
//=======================================
void CPlayer::CollisionMagicWall(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	if (pos.z >= FAR_POS)
	{ // 位置が一定以上になった場合

		// 位置を補正する
		pos.z = FAR_POS;
	}

	if (pos.z <= NEAR_POS)
	{ // 位置を一定以下になった場合

		// 位置を補正する
		pos.z = NEAR_POS;
	}

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// ブロックの当たり判定
//=======================================
void CPlayer::BlockCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置
	D3DXVECTOR3 posOld = GetPosOld();	// 前回の位置

	// ブロックとの当たり判定
	if (collision::BlockCollision(&pos, posOld, COLLISION_WIDTH, COLLISION_HEIGHT, m_bJump, &m_pBlock))
	{ // 着地した場合

		// ジャンプしていない
		m_bJump = false;

		// 移動量を0にする
		m_move.y = 0.0f;
	}
	else
	{ // 上記以外

		// ブロックのポインタを NULL にする
		m_pBlock = nullptr;
	}

	// 情報を設定する
	SetPos(pos);	// 位置
}

//=======================================
// 台との当たり判定
//=======================================
void CPlayer::TableCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置
	D3DXVECTOR3 posOld = GetPosOld();	// 前回の位置

	if (collision::TableCollision(&pos, posOld, COLLISION_WIDTH, COLLISION_DEPTH) == true)
	{ // 台との当たり判定が true だった場合

		// 縦の移動量を無くす
		m_move.y = 0.0f;

		// ジャンプ状況を false にする
		m_bJump = false;

		if (m_pAction->GetState() == CPlayerAct::STATE_CANNON ||
			m_pAction->GetState() == CPlayerAct::STATE_FLY)
		{ // 通常状態以外の場合

			// 通常状態の設定処理
			SetNone();

			if (m_pAction->GetFront() == true)
			{ // 前後状況を設定する

				// 後ろにする
				pos.z = FAR_POS;

				// 前後状況を設定する
				m_pAction->SetFront(false);
			}
			else
			{ // 上記以外

				// 後ろにする
				pos.z = NEAR_POS;

				// 前後状況を設定する
				m_pAction->SetFront(true);
			}

			// リセット処理
			m_pAbility->ResetAbility();
		}
	}

	// 位置を適用させる
	SetPos(pos);
}

//=======================================
// 落下の判定処理
//=======================================
void CPlayer::FallCheck(void)
{
	if (GetPos().y <= FALL_HEIGHT &&
		m_pAction->GetState() != CPlayerAct::STATE_FALL)
	{ // 完全に落ち切った場合

		// 移動量を初期化する
		m_move = NONE_D3DXVECTOR3;

		// 速度を0にする
		m_fSpeed = 0.0f;

		// 落下状態にする
		m_pAction->SetState(CPlayerAct::STATE_FALL);

		// ゲームの状態を死亡状態にする
		CGame::SetState(CGame::STATE_DEATH);

		// ライフを全て消す
		m_nLife = 0;
	}
}

//=======================================
// 範囲外の当たり判定
//=======================================
void CPlayer::OutRangeCollision(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 起伏範囲外の当たり判定処理
	if (collision::ElevOutRangeCollision(&pos, GetPosOld(), COLLISION_WIDTH) == true)
	{ // 当たり判定が通った場合

		// 移動量を0.0fにする
		m_move.x = 0.0f;

		// 速度を0.0fにする
		m_fSpeed = 0.0f;

		// 無能力状態にする
		m_pAbility->SetAbility(CAbility::ABILITY_NONE, *this);
	}

	// 位置の設定処理
	SetPos(pos);
}

//=======================================
// スタート状態の処理
//=======================================
void CPlayer::StartProcess(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 位置を加算する
	pos += ADD_START_POS;

	if (pos.x >= m_posDest.x)
	{ // 位置が目的を超えた場合

		// 位置を補正する
		pos = m_posDest;

		if (m_nStartCount == 0)
		{ // 最初のみ

			// 波紋の生成
			CRipple::Create(GetPos(), NONE_D3DXVECTOR3);
		}

		// スタートカウントを加算する
		m_nStartCount++;
	}

	if (m_nStartCount >= START_COUNT)
	{ // スタートカウントを設定する

		// 通常モーションを設定する
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);

		// 遊び状態にする
		CGame::SetState(CGame::STATE_PLAY);
	}

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// ゴール状態の処理
//=======================================
void CPlayer::GoalProcess(void)
{
	// 通常状態にする
	m_pAction->SetState(CPlayerAct::STATE_NONE);

	// ゴール時のカウントを加算する
	m_nGoalCount++;

	if (m_bPunch == true)
	{ // パンチ状態の場合

		if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
		{ // SPACEキーを押した場合

			// モーションの種類を取得する
			int nMotionType = m_pMotion->GetType();

			// モーションの種類を設定する
			nMotionType = (nMotionType + 1) % 2;

			// モーションを設定する
			m_pMotion->Set(MOTIONTYPE_SCRATCH001 + nMotionType);

			// ボーナスの加算処理
			CBonus::Get()->AddBonus();

			if (CGoal::Get() != nullptr)
			{ // ゴールの情報があった場合

				// ゴールの向き
				D3DXVECTOR3 rot = NONE_D3DXVECTOR3;

				// 向きを設定する
				rot.x = (rand() % 80 - 40) * 0.01f;
				rot.z = (rand() % 80 - 40) * 0.01f;

				// ゴールの向きを設定する
				CGoal::Get()->GetModel(CGoal::MODEL_BODY)->SetRot(rot);
				CGoal::Get()->GetModel(CGoal::MODEL_POINT)->SetRot(rot);
			}

			// 撃破の設定処理
			CDestruction::Create
			(
				D3DXVECTOR3(GetPos().x + sinf(GetRot().y) * (PUNCH_DSTR_SHIFT.x + (rand() % 20)), GetPos().y + (PUNCH_DSTR_SHIFT.y + (rand() % 50 - 25)), GetPos().z),
				PUNCH_DSTR_SIZE,
				PUNCH_DSTR_COL,
				CDestruction::TYPE_THORN,
				PUNCH_DSTR_LIFE
			);

			// 波紋を生成する
			CRipple::Create(D3DXVECTOR3(GetPos().x + sinf(GetRot().y) * PUNCH_RIPPLE_SHIFT.x, GetPos().y + PUNCH_RIPPLE_SHIFT.y, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, GetRot().y));
		}

		if (m_nGoalCount >= PUNCH_COUNT)
		{ // 一定カウント以上になった場合

			// 退場状態にする
			CGame::SetState(CGame::STATE_LEAVE);

			// ゴールカウントを初期化する
			m_nGoalCount = 0;

			// ゲームスコアの加算処理
			CGameScore::Get()->AddScore(CBonus::Get()->GetBonus() * 100);

			if (CGoal::Get() != nullptr)
			{ // ゴールがあった場合

				// 破壊処理
				CGoal::Get()->Break();

				// ゴールの終了処理
				CGoal::Get()->Uninit();
			}

			if (m_pPushTiming != nullptr)
			{ // 押しボタンが NULL じゃない場合

				// 終了処理
				m_pPushTiming->Uninit();
				m_pPushTiming = nullptr;
			}
		}
	}
	else
	{ // 上記以外

		if (m_nGoalCount >= GOAL_COUNT)
		{ // 一定カウント以上になった場合

			// 退場状態にする
			CGame::SetState(CGame::STATE_LEAVE);

			// ゴールカウントを初期化する
			m_nGoalCount = 0;
		}
	}
}

//=======================================
// 退場状態の処理
//=======================================
void CPlayer::LeaveProcess(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	// 重力処理
	useful::Gravity(&m_move.y, pos, LEAVE_GRAVITY);

	// 位置を更新する
	pos.x += m_move.x;

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 終了状態の処理
//=======================================
void CPlayer::FinishProcess(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を設定する

	// 重力処理
	useful::Gravity(&m_move.y, pos, FINISH_GRAVITY);

	// 位置を適用する
	SetPos(pos);
}