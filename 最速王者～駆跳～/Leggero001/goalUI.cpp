//============================================
//
// ゴールUI処理[goalUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "goalUI.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define GOALUI_SHIFT_POS_X			(-1200.0f)							// ゴールUIのずらす位置(X軸)
#define GOALUI_SHIFT_POS_Y			(50.0f)								// ゴールUIのずらす位置(Y軸)
#define GOALUI_SHIFT_POS_Z			(-150.0f)							// ゴールUIのずらす位置(Z軸)
#define GOALUI_INIT_ROT				(D3DXVECTOR3(0.0f, 0.8f, 0.0f))		// ゴールUIの向き
#define GOALUI_INIT_MOVE			(20.0f)								// ゴールUIの初期移動量
#define GOALUI_ROT_MOVE				(0.02f)								// ゴールUIの向きの移動量の加算数
#define GOALUI_BRAKE_ROT_MAGNI		(0.15f)								// ゴールUIの向きのブレーキ倍率
#define GOALUI_STOP_CHANGE_MOVE		(0.005f)							// ゴールUIの停止状態から変化する移動量
#define GOALUI_CLEAR_COUNT			(30)								// ゴールUIの消去状態になるカウント数
#define GOALUI_CLEAR_POS			(1000.0f)							// ゴールUIの消去判定が通る位置
#define GOALUI_CLEAR_MOVE			(30.0f)								// ゴールUIの消去時の速度

//============================
// コンストラクタ
//============================
CGoalUI::CGoalUI() : CModel(CObject::TYPE_GOALUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの移動量
	m_state = STATE_APPEAR;						// 状態
	m_nStateCount = 0;							// 状態カウント
}

//============================
// デストラクタ
//============================
CGoalUI::~CGoalUI()
{

}

//============================
// 初期化処理
//============================
HRESULT CGoalUI::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの移動量
	m_state = STATE_APPEAR;						// 状態
	m_nStateCount = 0;							// 状態カウント

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CGoalUI::Uninit(void)
{
	// 終了
	CModel::Uninit();
}

//============================
// 更新処理
//============================
void CGoalUI::Update(void)
{
	switch (m_state)
	{
	case STATE_APPEAR:			// 出現状態

		// 移動処理
		Move();

		// 回転処理
		Cycle();

		// 出現状態変化処理
		AppearChange();

		break;

	case STATE_STOP:			// 停止状態

		// ブレーキ処理
		Brake();

		// 停止状態変化処理
		StopChange();

		if (m_nStateCount >= GOALUI_CLEAR_COUNT)
		{ // 状態カウントが一定数以上になった場合

			// 状態カウントを 0 にする
			m_nStateCount = 0;

			// 目的の位置を設定する
			m_posDest.x = GetPos().x + GOALUI_CLEAR_POS;

			// 移動量を設定する
			m_move.x = GOALUI_CLEAR_MOVE;

			// 消去状態にする
			m_state = STATE_CLEAR;
		}

		break;

	case STATE_CLEAR:			// 消去状態

		// 移動処理
		Move();

		if (Clear() == true)
		{ // 消去処理が true だった場合

			// 終了処理
			Uninit();

			// 終了状態にする
			CGame::SetState(CGame::STATE_FINISH);

			// この先の処理を行わない
			return;
		}

		break;
	}
}

//============================
// 描画処理
//============================
void CGoalUI::Draw(void)
{
	// 画面クリア(Zバッファのクリア)
	CManager::GetRenderer()->GetDevice()->Clear(0,
		NULL,
		D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画処理
	CModel::Draw();
}

//============================
// 情報の設定処理
//============================
void CGoalUI::SetData(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置

	// 初期位置設定処理
	InitPos = InitPosCalc(pos);

	// 全ての値を初期化する
	m_posDest = D3DXVECTOR3(pos.x, pos.y, pos.z);		// 目標の位置
	m_move = D3DXVECTOR3(GOALUI_INIT_MOVE, 0.0f, 0.0f);		// 移動量
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向きの移動量
	m_state = STATE_APPEAR;									// 状態
	m_nStateCount = 0;										// 状態カウント

	// 情報の設定処理
	SetPos(InitPos);						// 位置の設定
	SetPosOld(InitPos);						// 前回の位置の設定
	SetRot(GOALUI_INIT_ROT);				// 向きの設定
	SetScale(NONE_SCALE);					// 拡大率の設定
	SetFileData(CXFile::TYPE_GOALUI);		// Xファイルのデータの設定処理
}

//============================
//生成処理
//============================
CGoalUI* CGoalUI::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CGoalUI* pGoalUI = nullptr;	// プレイヤーのインスタンスを生成

	if (pGoalUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pGoalUI = new CGoalUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pGoalUI != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pGoalUI->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "ゴールUIの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pGoalUI->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pGoalUI;
}

//============================
// 初期位置設定処理
//============================
D3DXVECTOR3 CGoalUI::InitPosCalc(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 InitPos = pos;		// 位置を取得する

	// 位置をずらす
	InitPos.x = pos.x + GOALUI_SHIFT_POS_X;			// X座標
	InitPos.y = pos.y + GOALUI_SHIFT_POS_Y;			// Y座標
	InitPos.z = pos.z + GOALUI_SHIFT_POS_Z;			// Z座標

	// 初期位置を返す
	return InitPos;
}

//============================
// 移動処理
//============================
void CGoalUI::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置の取得

	// 位置を移動する
	pos += m_move;

	// 位置を更新する
	SetPos(pos);
}

//============================
// 出現状態変化処理
//============================
void CGoalUI::AppearChange(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	if (pos.x >= m_posDest.x)
	{ // 位置が中心より左に行った場合

		// 位置を設定する
		pos.x = m_posDest.x;

		// 回転状態にする
		m_state = STATE_STOP;
	}

	// 位置を更新する
	SetPos(pos);
}

//============================
// 回転処理
//============================
void CGoalUI::Cycle(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();			// 向きを取得する

	// 向きの移動量を加算する
	m_rotmove.y += GOALUI_ROT_MOVE;

	// 向きを加算する
	rot.y += m_rotmove.y;

	// 向きの正規化
	RotNormalize(&rot.y);

	// 向きを更新する
	SetRot(rot);
}

//============================
// ブレーキ処理
//============================
void CGoalUI::Brake(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();			// 向きを取得する

	// 向きの移動量を加算する
	m_rotmove.y += (0.0f - m_rotmove.y) * GOALUI_BRAKE_ROT_MAGNI;

	// 向きを加算する
	rot.y += m_rotmove.y;

	// 向きの正規化
	RotNormalize(&rot.y);

	// 向きを更新する
	SetRot(rot);
}

//============================
// 停止状態変化処理
//============================
void CGoalUI::StopChange(void)
{
	if (m_rotmove.y <= GOALUI_STOP_CHANGE_MOVE)
	{ // 移動量が一定以下の場合

		// 状態カウントを加算する
		m_nStateCount++;
	}
}

//============================
// 消去処理
//============================
bool CGoalUI::Clear(void)
{
	if (GetPos().x >= m_posDest.x)
	{ // 位置が一定位置を超えた場合

		// true を返す
		return true;
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}