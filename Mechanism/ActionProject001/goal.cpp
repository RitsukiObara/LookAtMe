//===========================================
//
// ゴールのメイン処理[goal.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "goal.h"
#include "useful.h"

#include "bonus.h"
#include "game_score.h"
#include "player.h"
#include "fraction.h"
#include "push_timing.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define CYCLE_COUNT			(40)		// 向きが変わるカウント
#define GOAL_PLAYER_SPEED	(7.0f)		// ゴール時のプレイヤーの速度
#define SMALL_ADD_SCORE		(1000)		// 小スコア
#define BIG_ADD_SCORE		(2000)		// 大スコア
#define GOAL_FRACTION_COUNT	(15)		// ゴールが壊れたときの破片の数

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CGoal* CGoal::m_pGoal = nullptr;		// ゴールの情報

//==============================
// コンストラクタ
//==============================
CGoal::CGoal() : CObject(CObject::TYPE_GOAL, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		m_aGoal[nCnt] = nullptr;	// ゴールの変数
	}
	m_type = TYPE_PUNCH;			// 種類
	m_state = STATE_CYCLE;			// 状態
	m_nStateCount = 0;				// 状態カウント
	m_fRotDest = 0.0f;				// 目標の向き
}

//==============================
// デストラクタ
//==============================
CGoal::~CGoal()
{

}

//==============================
// ゴールの初期化処理
//==============================
HRESULT CGoal::Init(void)
{
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		if (m_aGoal[nCnt] == nullptr)
		{ // オブジェクトが NULL の場合

			// インスタンスを生成
			m_aGoal[nCnt] = new CModel(TYPE_NONE, PRIORITY_PLAYER);
		}
		else
		{ // オブジェクトが NULL じゃない場合

			// 停止
			assert(false);

			// 失敗 を返す
			return E_FAIL;
		}

		if (m_aGoal[nCnt] != nullptr)
		{ // オブジェクトが NULL じゃない場合

			// 初期化処理
			if (FAILED(m_aGoal[nCnt]->Init()))
			{ // 初期化に失敗した場合

				// 停止
				assert(false);

				// 失敗 を返す
				return E_FAIL;
			}
		}
		else
		{ // オブジェクトが NULL の場合

			// 停止
			assert(false);

			// 失敗 を返す
			return E_FAIL;
		}
	}

	// 全ての値を初期化する
	m_type = TYPE_PUNCH;			// 種類
	m_state = STATE_CYCLE;			// 状態
	m_nStateCount = 0;				// 状態カウント
	m_fRotDest = 0.0f;				// 目標の向き

	// 成功を返す
	return S_OK;
}

//========================================
// ゴールの終了処理
//========================================
void CGoal::Uninit(void)
{
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		if (m_aGoal[nCnt] != nullptr)
		{ // モデルが NULL じゃない場合

			// 終了処理
			m_aGoal[nCnt]->Uninit();
			m_aGoal[nCnt] = nullptr;
		}
	}

	// 本体の終了処理
	Release();

	// ゴールの情報を NULL にする
	m_pGoal = nullptr;
}

//========================================
// ゴールの更新処理
//========================================
void CGoal::Update(void)
{
	switch (m_state)
	{
	case CGoal::STATE_CYCLE:		// 回転状態

		// 向きの決定処理
		RotDecide(&m_fRotDest);

		// 向きの補正処理
		RotCorrect();

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % CYCLE_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// 種類を変える
			m_type = (TYPE)((m_type + 1) % TYPE::TYPE_MAX);
		}

		break;

	case CGoal::STATE_STOP:			// 停止状態


		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=====================================
// ゴールの描画処理
//=====================================
void CGoal::Draw(void)
{
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		// 描画処理
		m_aGoal[nCnt]->Draw();
	}
}

//=====================================
// ヒット処理
//=====================================
void CGoal::Hit(void)
{
	// ヒット時の向きの決定処理
	HitRotDecide();

	// 停止状態にする
	m_state = STATE_STOP;

	// 状態カウントを初期化する
	m_nStateCount = 0;

	// ゴール状態に設定する
	CGame::SetState(CGame::STATE_GOAL);

	// 敵を全て消す
	CObject::AnyAllClear(TYPE_ENEMY);

	// プレイヤーの位置の設定処理
	PlayerPosDecide();

	// プレイヤーの情報の設定処理
	PlayerSetting();

	if (m_type != TYPE_PUNCH)
	{ // パンチ以外の場合

		// 破壊処理
		Break();

		// 終了処理
		Uninit();
	}
	else
	{ // 上記以外

		if (CPlayer::Get() != nullptr)
		{ // プレイヤーが NULL じゃない場合

			// 押しボタンを生成
			CPlayer::Get()->SetPushTiming();
		}
	}
}

//=====================================
// 破壊処理
//=====================================
void CGoal::Break(void)
{
	for (int nCnt = 0; nCnt < GOAL_FRACTION_COUNT; nCnt++)
	{
		// 破片を出す
		CFraction::Create(CGoal::Get()->GetModel(CGoal::MODEL_POINT)->GetPos(), CFraction::TYPE_REDRUST);
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CGoal::SetData(const D3DXVECTOR3& pos)
{
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		// 情報の設定処理
		m_aGoal[nCnt]->SetPos(pos);						// 位置
		m_aGoal[nCnt]->SetPosOld(pos);					// 前回の位置
		m_aGoal[nCnt]->SetRot(NONE_D3DXVECTOR3);		// 向き
		m_aGoal[nCnt]->SetScale(NONE_SCALE);			// 拡大率

		// モデルの情報
		switch (nCnt)
		{
		case MODEL_BODY:			// 本体

			m_aGoal[nCnt]->SetFileData(CXFile::TYPE_GOAL);

			break;

		case MODEL_POINT:			// 得点

			m_aGoal[nCnt]->SetFileData(CXFile::TYPE_GOALPOINT);

			break;
		}
	}

	// 全ての値を設定する
	m_type = TYPE_PUNCH;			// 種類
	m_nStateCount = 0;				// 状態カウント
	m_fRotDest = 0.0f;				// 目標の向き
}

//=======================================
// モデルの設定処理
//=======================================
CModel* CGoal::GetModel(const MODEL model) const
{
	// モデルの情報を返す
	return m_aGoal[model];
}

//=======================================
// 種類の取得処理
//=======================================
CGoal::TYPE CGoal::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//=======================================
// 状態の取得処理
//=======================================
CGoal::STATE CGoal::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 取得処理
//=======================================
CGoal* CGoal::Get(void)
{
	if (m_pGoal != nullptr)
	{ // ゴールの情報がある場合

		// ゴールのポインタを返す
		return m_pGoal;
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// ゴールのポインタを返す
		return m_pGoal;
	}
}

//=======================================
// 生成処理
//=======================================
CGoal* CGoal::Create(const D3DXVECTOR3& pos)
{
	if (m_pGoal == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		m_pGoal = new CGoal;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (m_pGoal != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(m_pGoal->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		m_pGoal->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ゴールのポインタを返す
	return m_pGoal;
}

//=======================================
// 向きの決定処理
//=======================================
void CGoal::RotDecide(float* m_fRot)
{
	switch (m_type)
	{
	case CGoal::TYPE_PUNCH:		// パンチ

		*m_fRot = 0.0f;

		break;

	case CGoal::TYPE_SMALL_R:	// +1000

		*m_fRot = -D3DX_PI * 0.5f;

		break;

	case CGoal::TYPE_BIG:		// +2000

		*m_fRot = D3DX_PI;

		break;

	case CGoal::TYPE_SMALL_L:	// +1000

		*m_fRot = D3DX_PI* 0.5f;

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=======================================
// 向きの補正処理
//=======================================
void CGoal::RotCorrect(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = m_aGoal[MODEL_POINT]->GetRot();		// 向きを取得する

	// 向きの正規化
	useful::RotCorrect(m_fRotDest, &rot.y, 0.5f);

	// 向きを適用する
	m_aGoal[MODEL_POINT]->SetRot(rot);
}

//=======================================
// ヒット時の向きの決定処理
//=======================================
void CGoal::HitRotDecide(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = m_aGoal[MODEL_POINT]->GetRot();		// 向きを取得する

	// 向きの決定処理
	RotDecide(&rot.y);

	// 向きを適用する
	m_aGoal[MODEL_POINT]->SetRot(rot);
}

//=======================================
// プレイヤーの位置の設定処理
//=======================================
void CGoal::PlayerPosDecide(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = CPlayer::Get();		// プレイヤーの情報

	if (pPlayer->GetPos().x >= m_aGoal[MODEL_POINT]->GetPos().x)
	{ // ゴールの右側にいた場合

		// 位置を設定する
		pPlayer->SetPos(D3DXVECTOR3(m_aGoal[MODEL_POINT]->GetPos().x + 200.0f, m_aGoal[MODEL_POINT]->GetPos().y - 50.0f, m_aGoal[MODEL_POINT]->GetPos().z));

		// 左向きにする
		pPlayer->SetEnableRight(false);

		// 向きを設定する
		pPlayer->SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));
	}
	else
	{ // 上記以外

		// 位置を設定する
		pPlayer->SetPos(D3DXVECTOR3(m_aGoal[MODEL_POINT]->GetPos().x - 200.0f, m_aGoal[MODEL_POINT]->GetPos().y - 50.0f, m_aGoal[MODEL_POINT]->GetPos().z));

		// 右向きにする
		pPlayer->SetEnableRight(true);

		// 向きを設定する
		pPlayer->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	}
}

//=======================================
// プレイヤーの情報の設定処理
//=======================================
void CGoal::PlayerSetting(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 move;

	switch (m_type)
	{
	case CGoal::TYPE_PUNCH:

		// パンチ状況を true にする
		CPlayer::Get()->SetEnablePunch(true);

		// ボーナスの生成
		CBonus::Create(m_aGoal[MODEL_POINT]->GetPos());

		break;

	case CGoal::TYPE_SMALL_R:

		// パンチ状況を false にする
		CPlayer::Get()->SetEnablePunch(false);

		// 小スコアを加算する
		CGameScore::Get()->AddScore(SMALL_ADD_SCORE);

		break;

	case CGoal::TYPE_BIG:

		// パンチ状況を false にする
		CPlayer::Get()->SetEnablePunch(false);

		// 大スコアを加算する
		CGameScore::Get()->AddScore(BIG_ADD_SCORE);

		break;

	case CGoal::TYPE_SMALL_L:

		// パンチ状況を false にする
		CPlayer::Get()->SetEnablePunch(false);

		// 小スコアを加算する
		CGameScore::Get()->AddScore(SMALL_ADD_SCORE);

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 移動量を設定する
	move.x = sinf(CPlayer::Get()->GetRot().y) * GOAL_PLAYER_SPEED;
	move.y = 0.0f;
	move.z = 0.0f;

	// 移動量の設定処理
	CPlayer::Get()->SetMove(move);
}