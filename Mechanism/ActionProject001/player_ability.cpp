//============================================
//
// プレイヤーの能力のメイン処理[player_ability.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "player.h"
#include "motion.h"
#include "player_ability.h"
#include "input.h"
#include "collision.h"
#include "sound.h"
#include "useful.h"

#include "ripple.h"
#include "objectElevation.h"
#include "elevation_manager.h"
#include "turn.h"
#include "fraction.h"
#include "block.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define HOVER_COUNT					(100)				// ホバー状態のカウント
#define HOVER_RIPPLE_COUNT			(10)				// ホバー状態時の波紋が出るカウント
#define HOVER_SOUND_COUNT			(7)					// ホバー音を鳴らすカウント数
#define JETDASH_SOUND_LABEL			(14)				// ジェットダッシュ音を鳴らすカウント
#define JETDASH_POSBL_COUNT			(10)				// ジェットダッシュ状態が使用可能になるカウント
#define GROUNDQUAKE_COUNT			(500)				// グラウンドクエイク状態の強制解除カウント
#define GROUNDQUAKE_RIPPLE_COUNT	(10)				// グラウンドクエイク時の波紋が出る間隔
#define GROUNDQUAKE_POSBL_COUNT		(15)				// グラウンドクエイク状態が使用可能になるカウント
#define JETDASH_SPEED				(25.0f)				// ジェットダッシュ時のスピード
#define DASHJUMP_DEST_SPEED			(8.0f)				// ダッシュジャンプ時の目的のスピード
#define DASHJUMP_ADD_SPEED			(0.5f)				// ダッシュジャンプ時の追加のスピード
#define DASH_SMOKE_SHIFT			(70.0f)				// 煙のずれる座標
#define DASH_SMOKE_COUNT			(4)					// 煙のカウント
#define BLOCK_BREAK_COUNT			(40)				// ブロックの壊れるカウント
#define BLOCK_FRACTION_COUNT		(20)				// ブロックの破片のカウント

//============================================
// コンストラクタ
//============================================
CAbility::CAbility()
{
	// 全ての値をクリアする
	m_pElev = nullptr;				// 起伏地面のポインタ
	m_ability = ABILITY_NONE;		// 能力

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aAblCount[nCnt] = 0;				// カウント
		m_aPossible[nCnt] = true;			// 能力の使用状況
	}
}

//============================================
// デストラクタ
//============================================
CAbility::~CAbility()
{

}

//============================================
// 初期化処理
//============================================
HRESULT CAbility::Init(void)
{
	// 全ての値を初期化する
	m_pElev = nullptr;				// 起伏地面のポインタ
	m_ability = ABILITY_NONE;		// 能力

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aPossible[nCnt] = true;			// 能力の使用状況
		m_aAblCount[nCnt] = 0;				// カウント
	}

	// 成功を返す
	return S_OK;
}

//============================================
// 終了処理
//============================================
void CAbility::Uninit(void)
{
	// 起伏地面のポインタを NULL にする
	m_pElev = nullptr;

	// 自分のメモリを解放する
	delete this;
}

//============================================
// 更新処理
//============================================
void CAbility::Update(CPlayer& player)
{
	switch (m_ability)
	{
	case ABILITY_NONE:			// 無状態

		// 能力操作処理
		Ability(player);

		break;

	case ABILITY_HOVER:			// ホバージェット

		// ホバージェット処理
		HoverJet(player);

		break;

	case ABILITY_JETDASH:		// ジェットダッシュ

		// ジェットダッシュ処理
		SkyDash(player);

		break;

	case ABILITY_DASHJUMP:		// ダッシュジャンプ

		// ダッシュジャンプ処理
		DashJump(player);

		// 能力操作処理
		Ability(player);

		break;

	case ABILITY_GROUNDQUAKE:	// グラウンドクエイク

		// グラウンドクエイク処理
		GroundQuake(player);

		// マキナ草との当たり判定
		collision::MacchinaHit(player);

		// 敵の気絶処理
		collision::EnemyStun(player);

		// ブロックの破壊処理
		BlockBreak(player);

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 使用可能状況の設定処理
	PossibleProcess(player);
}

//============================================
// 情報の設定処理
//============================================
void CAbility::SetData(void)
{
	// 全ての値を設定する
	m_ability = ABILITY_NONE;		// 能力

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aAblCount[nCnt] = 0;				// カウント
	}
}

//============================================
// 能力の設定処理
//============================================
void CAbility::SetAbility(const ABILITY ability, CPlayer& player)
{
	// 能力を設定する
	m_ability = ability;

	switch (m_ability)
	{
	case ABILITY_NONE:			// 通常状態

		break;

	case ABILITY_HOVER:			// ホバージェット状態

		// 浮遊モーションにする
		player.GetMotion()->Set(CPlayer::MOTIONTYPE_FLY);

		break;

	case ABILITY_JETDASH:		// ジェットダッシュ状態

	{
		// ローカル変数宣言
		D3DXVECTOR3 move = player.GetMove();		// 移動量を取得する

		// 移動量を設定する
		move.y = 0.0f;

		// 移動量を適用する
		player.SetMove(move);
	}

		break;

	case ABILITY_DASHJUMP:		// ダッシュジャンプ

		// 速度を設定する
		player.SetSpeed(30.0f);

		break;

	case ABILITY_GROUNDQUAKE:	// グラウンドクエイク状態

		// グラウンドクエイクモーションにする
		player.GetMotion()->Set(CPlayer::MOTIONTYPE_QUAKE);

		// 頂点の探索処理
		SearchVertex(player.GetPos());

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//============================================
// 能力の取得処理
//============================================
CAbility::ABILITY CAbility::GetAbility(void) const
{
	// 能力を返す
	return m_ability;
}

//============================================
// 能力のリセット処理
//============================================
void CAbility::ResetAbility(void)
{
	// 能力を設定する
	m_ability = ABILITY_NONE;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aPossible[nCnt] = true;		// 可能状況
		m_aAblCount[nCnt] = 0;			// 能力カウント
	}
}

//========================
// 可能状況の設定処理
//========================
void CAbility::SetPossible(const TYPE type, const bool bPossible)
{
	// 可能状況を設定する
	m_aPossible[type] = bPossible;
}

//========================
// 可能状況の取得処理
//========================
bool CAbility::GetPossible(const TYPE type)
{
	// 可能状況を返す
	return m_aPossible[type];
}

//============================================
// 生成処理
//============================================
CAbility* CAbility::Create(void)
{
	// ローカル変数宣言
	CAbility* pAbility = nullptr;		// 行動状態

	if (pAbility == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pAbility = new CAbility;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pAbility;
	}

	if (pAbility != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pAbility->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "プレイヤーの能力の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pAbility->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 能力のポインタを返す
	return pAbility;
}

//=======================================
// 能力操作処理
//=======================================
void CAbility::Ability(CPlayer& player)
{
	if ((CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_Y) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_Y, 0) == true) &&
		m_aPossible[TYPE_HOVER] == true &&
		player.IsJump() == true)
	{ // 空中でYキーを押した場合

		// ホバージェット状態にする
		player.GetAbility()->SetAbility(CAbility::ABILITY_HOVER, player);

		// 使用状況を false にする
		m_aPossible[TYPE_HOVER] = false;
	}
	else if ((CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_U) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_X, 0) == true) &&
		m_aPossible[TYPE_JETDASH] == true)
	{ // Uキーを押した場合

		// ジェットダッシュ状態にする
		player.GetAbility()->SetAbility(CAbility::ABILITY_JETDASH, player);

		// 使用状況を false にする
		m_aPossible[TYPE_JETDASH] = false;

		// ジェットダッシュモーションを設定する
		player.GetMotion()->Set(CPlayer::MOTIONTYPE_JETDASH);
	}
	else if ((CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_I) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_B, 0) == true) &&
		m_aPossible[TYPE_GROUNDQUAKE] == true &&
		player.IsJump() == false)
	{ // 地上でIキーを押した場合

		// グラウンドクエイク状態にする
		player.GetAbility()->SetAbility(CAbility::ABILITY_GROUNDQUAKE, player);

		// カメラを振動カメラにする
		CManager::Get()->GetCamera()->SetType(CCamera::TYPE_VIBRATE);
	}
}

//============================================
// ホバージェット処理
//============================================
void CAbility::HoverJet(CPlayer& player)
{
	// ジャンプ状態にする
	player.SetEnableJump(true);

	// カウントを加算する
	m_aAblCount[TYPE_HOVER]++;

	if (m_aAblCount[TYPE_HOVER] % 10 == 0)
	{ // 能力カウントが一定数ごとに

		// 波紋を生成
		CRipple::Create(player.GetPos(), player.GetRot());
	}

	if (m_aAblCount[TYPE_HOVER] % HOVER_SOUND_COUNT == 0)
	{ // 能力カウントが一定数ごとに

		// ホバー音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_HOVER);
	}

	if (m_aAblCount[TYPE_HOVER] >= HOVER_COUNT)
	{ // カウントが一定数以上の場合

		// カウントを初期化する
		m_aAblCount[TYPE_HOVER] = 0;

		// 無能力状態にする
		m_ability = ABILITY_NONE;
	}
}

//============================================
// ジェットダッシュ処理
//============================================
void CAbility::SkyDash(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player.GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得する

	if (m_aAblCount[TYPE_JETDASH] % JETDASH_SOUND_LABEL == 0)
	{
		// ジェットダッシュ音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_JETDASH);
	}

	// 能力カウントを加算する
	m_aAblCount[TYPE_JETDASH]++;

	// 位置を設定する
	pos.x += sinf(rot.y) * JETDASH_SPEED;

	if (m_aAblCount[TYPE_JETDASH] % DASH_SMOKE_COUNT == 0)
	{ // 一定のカウント数になった場合

		// 振り向き演出の生成
		CTurn::Create(D3DXVECTOR3(player.GetPos().x, player.GetPos().y + DASH_SMOKE_SHIFT, player.GetPos().z), player.IsRight());
	}

	if (CManager::Get()->GetInputKeyboard()->GetRelease(DIK_U) == true)
	{ // Iキーを離した場合

		if (player.IsJump() == true &&
			player.GetMove().y > 0.0f)
		{ // ジャンプ状況が true かつ、ジャンプ量が0.0f超過の場合

			// 能力カウントを初期化する
			m_aAblCount[TYPE_JETDASH] = 0;

			// ダッシュジャンプ状態にする
			SetAbility(ABILITY_DASHJUMP, player);
		}
		else
		{ // 上記以外

			// 無能力状態にする
			SetAbility(ABILITY_NONE, player);
		}
	}

	// 位置を適用する
	player.SetPos(pos);
}

//============================================
// ダッシュジャンプ処理
//============================================
void CAbility::DashJump(CPlayer& player)
{
	// ローカル変数宣言
	float fSpeed = player.GetSpeed();		// 速度を取得する

	// 均等な補正処理
	useful::FrameCorrect(DASHJUMP_DEST_SPEED, &fSpeed, DASHJUMP_ADD_SPEED);

	// 速度を適用する
	player.SetSpeed(fSpeed);

	if (fSpeed <= DASHJUMP_DEST_SPEED)
	{ // 速度が一定数以下になった場合

		// 無能力状態にする
		m_ability = ABILITY_NONE;
	}
}

//============================================
// グラウンドクエイク処理
//============================================
void CAbility::GroundQuake(CPlayer& player)
{
	if (m_aAblCount[TYPE_GROUNDQUAKE] % GROUNDQUAKE_RIPPLE_COUNT == 0)
	{ // 一定間隔ごとに

		// 波紋の生成処理
		CRipple::Create(D3DXVECTOR3(player.GetPos().x, player.GetPos().y - player.GetMove().y, player.GetPos().z), NONE_D3DXVECTOR3);
	}

	if (m_aAblCount[TYPE_GROUNDQUAKE] % 35 == 0)
	{ // 能力カウントが一定以上の場合

		// グラウンドクエイク音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_GROUNDQUAKE);
	}

	// 能力カウントを加算する
	m_aAblCount[TYPE_GROUNDQUAKE]++;

	if (CManager::Get()->GetInputKeyboard()->GetRelease(DIK_I) == true ||
		player.IsJump() == true ||
		m_aAblCount[TYPE_GROUNDQUAKE] >= GROUNDQUAKE_COUNT)
	{ // Iキーを離したまたは、一定カウント数を超えた場合

		// 無能力状態にする
		SetAbility(CAbility::ABILITY_NONE, player);

		// 使用状況を false にする
		m_aPossible[TYPE_GROUNDQUAKE] = false;

		// 能力カウントを設定する
		m_aAblCount[TYPE_GROUNDQUAKE] = GROUNDQUAKE_POSBL_COUNT;
	}
}

//============================================
// 可能状況判定処理
//============================================
void CAbility::PossibleProcess(CPlayer& player)
{
	// ホバーの使用状況
	if (player.IsJump() == false)
	{ // 地上に立っている時

		// 能力カウントを初期化する
		m_aAblCount[TYPE_HOVER] = 0;

		// 使用状況を true にする
		m_aPossible[TYPE_HOVER] = true;
	}

	// ジェットダッシュの使用状況
	if (m_aPossible[TYPE_JETDASH] == false &&
		m_ability != ABILITY_JETDASH)
	{ // ジェットダッシュが使えない場合

		// 能力カウントを加算する
		m_aAblCount[TYPE_JETDASH]++;

		if (m_aAblCount[TYPE_JETDASH] >= JETDASH_POSBL_COUNT)
		{ // 能力カウントが一定数以上の場合

			// 能力カウントを初期化する
			m_aAblCount[TYPE_JETDASH] = 0;

			// 使用可能にする
			m_aPossible[TYPE_JETDASH] = true;
		}
	}

	// グラウンドクエイクの使用状況
	if (m_aPossible[TYPE_GROUNDQUAKE] == false)
	{ // グラウンドクエイクが使えない場合

		// 能力カウントを減算する
		m_aAblCount[TYPE_GROUNDQUAKE]--;

		if (m_aAblCount[TYPE_GROUNDQUAKE] <= 0)
		{ // 能力カウントが一定数以下になった場合

			// 能力カウントを設定する
			m_aAblCount[TYPE_GROUNDQUAKE] = 0;

			// 使用可能にする
			m_aPossible[TYPE_GROUNDQUAKE] = true;
		}
	}
}

//============================================
// ブロックの破壊処理
//============================================
void CAbility::BlockBreak(CPlayer& player)
{
	if (player.GetBlock() != nullptr &&
		player.GetBlock()->IsBreak() == true)
	{ // ブロックが破壊できる場合

		// 破壊カウントを取得する
		int nCount = player.GetBlock()->GetBreakCount();

		// カウントを加算する
		nCount++;

		if (nCount % BLOCK_FRACTION_COUNT == 0)
		{ // 一定カウントごとに

			// 位置を取得する
			D3DXVECTOR3 pos = player.GetPos();

			// 位置をずらす
			pos.x += rand() % 40 - 20;
			pos.y -= 10.0f;
			pos.z += rand() % 40 - 20;

			// 破片の生成
			CFraction::Create(pos, CFraction::TYPE_WOODBOX);
		}

		if (nCount % BLOCK_BREAK_COUNT == 0)
		{ // カウントが一定数以上になった場合

			// ドロップ処理
			player.GetBlock()->Drop();

			// 終了処理
			player.GetBlock()->Uninit();

			// ブロックのNULL化処理
			player.DeleteBlock();

			D3DXVECTOR3 pos = NONE_D3DXVECTOR3;

			for (int nCnt = 0; nCnt < BLOCK_FRACTION_COUNT; nCnt++)
			{
				// 位置を取得する
				pos = player.GetPos();

				// 位置をずらす
				pos.x += rand() % 40 - 20;
				pos.y -= 10.0f;
				pos.z += rand() % 40 - 20;

				// 破片の生成
				CFraction::Create(pos, CFraction::TYPE_WOODBOX);
			}

			// この先の処理を行わない
			return;
		}

		// 破壊カウントを適用する
		player.GetBlock()->SetBreakCount(nCount);
	}
}

//============================================
// 起伏地面の頂点の探索処理
//============================================
void CAbility::SearchVertex(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	CElevation* pElev = CElevationManager::Get()->GetTop();		// 起伏地面を取得する
	int nVtxIdx;			// 頂点の番号

	while (pElev != nullptr)
	{ // 起伏地面がある限り続く

		if (pos.x <= pElev->GetPos().x + pElev->GetSize().x &&
			pos.x >= pElev->GetPos().x - pElev->GetSize().x &&
			pos.z <= pElev->GetPos().z + pElev->GetSize().z &&
			pos.z >= pElev->GetPos().z - pElev->GetSize().z)
		{ // ポリゴンの中にいた場合

			// 近くの番号を取得する
			nVtxIdx = pElev->NearVertexSearch(pos);

			// 起伏の地面を設定する
			m_pElev = pElev;
		}

		// 次のポインタを取得する
		pElev = pElev->GetNext();
	}
}