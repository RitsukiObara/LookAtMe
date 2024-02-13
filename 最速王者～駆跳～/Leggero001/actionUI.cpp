//============================================
//
// アクションUI処理[actionUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"
#include "manager.h"
#include "renderer.h"
#include "actionUI.h"
#include "texture.h"
#include "useful.h"

#include "praiseUI.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define ACTIONUI_GROUND_TEXTURE		"data/TEXTURE/ActUI_Back.png"					// 背景のテクスチャ
#define ACTIONUI_POS				(D3DXVECTOR3(1080.0f, 150.0f, 0.0f))			// アクションUIの位置

#define ACTIONUI_WK_SIZE			(D3DXVECTOR3(175.0f, 50.0f, 0.0f))				// 壁キックUIのサイズ
#define ACTIONUI_LT_SIZE			(D3DXVECTOR3(200.0f, 50.0f, 0.0f))				// 着地ターボUIのサイズ
#define ACTIONUI_SJ_SIZE			(D3DXVECTOR3(200.0f, 50.0f, 0.0f))				// 超ジャンプUIのサイズ
#define ACTIONUI_NJ_SIZE			(D3DXVECTOR3(240.0f, 40.0f, 0.0f))				// 街灯ジャンプのサイズ
#define ACTIONUI_NA_SIZE			(D3DXVECTOR3(200.0f, 40.0f, 0.0f))				// 攻撃のサイズ

#define ACTIONUI_SIZE_SUB			(0.5f)											// アクションUIのサイズの縮小率
#define ACTIONUI_ADD_ROT			(0.1f)											// アクションUIの向きの加算数
#define ACTIONUI_SIZE_DELETE		(0.01f)											// アクションUIの消去対象のサイズ
#define ACTIONUI_GROUND_SIZE_MOVE	(D3DXVECTOR3(-10.0f, -10.0f, 0.0f))				// アクションUIの背景のサイズの移動量

#define ACTIONUI_APPEAR_COUNT		(30)											// アクションUIの出現状態のカウント数
#define ACTIONUI_DISP_COUNT			(10)											// アクションUIの表示状態のカウント数

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CActionUI::m_apTexturename[CActionUI::TYPE_MAX] =				// テクスチャのパス名
{
	"data/TEXTURE/ActUI_WK.png",		// 壁キック
	"data/TEXTURE/ActUI_LT.png",		// 着地ターボ
	"data/TEXTURE/ActUI_SJ.png",		// 超ジャンプ
	"data/TEXTURE/ActUI_NJ.png",		// 街灯ジャンプ
	"data/TEXTURE/ActUI_NA.png",		// 攻撃
};

const D3DXVECTOR3 CActionUI::m_aSize[CActionUI::TYPE_MAX] =		// それぞれの大きさ
{
	ACTIONUI_WK_SIZE,		// 壁キック
	ACTIONUI_LT_SIZE,		// 着地ターボ
	ACTIONUI_SJ_SIZE,		// 超ジャンプ
	ACTIONUI_NJ_SIZE,		// 街灯ジャンプ
	ACTIONUI_NA_SIZE,		// 攻撃
};

//============================
// コンストラクタ
//============================
CActionUI::CActionUI() : CObject(TYPE_ACTIONUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		m_apActUI[nCntAct] = nullptr;				// アクションUIの情報
	}

	m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標のサイズ
	m_GroundMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 背景のサイズの移動量
	m_nStateCount = 0;								// 状態カウント
	m_state = STATE_APPEAR;							// 状態
	m_nScore = 0;									// 得点
}

//============================
// デストラクタ
//============================
CActionUI::~CActionUI()
{

}

//============================
// 初期化処理
//============================
HRESULT CActionUI::Init(void)
{
	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		if (m_apActUI[nCntAct] == nullptr)
		{ // 情報が NULL の場合

			// アクションUIの生成処理
			m_apActUI[nCntAct] = m_apActUI[nCntAct]->Create(CObject2D::TYPE_NONE, TYPE_ACTIONUI, PRIORITY_UI);
		}

		if (m_apActUI[nCntAct] != nullptr)
		{ // 情報が NULL じゃない場合

			if (FAILED(m_apActUI[nCntAct]->Init()))
			{ // 初期化に失敗した場合

				// 失敗を返す
				return E_FAIL;
			}
		}
	}

	// 全ての値をクリアする
	m_nStateCount = 0;				// 状態カウント
	m_state = STATE_APPEAR;			// 状態
	m_nScore = 0;					// 得点

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CActionUI::Uninit(void)
{
	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		// 情報の終了
		m_apActUI[nCntAct]->Uninit();
		m_apActUI[nCntAct] = nullptr;
	}

	// 本体の終了
	Release();
}

//============================
// 更新処理
//============================
void CActionUI::Update(void)
{
	// 状態カウントを加算する
	m_nStateCount++;

	switch (m_state)
	{
	case STATE_APPEAR:		// 出現状態

		// サイズの変化処理
		SizeChange(POLY_ACT);

		// 背景のバウンド処理
		GroundBound();

		if (m_nStateCount % ACTIONUI_APPEAR_COUNT == 0)
		{ // 状態カウントが一定数に達した場合

			// 出現状態変化処理
			AppearChange(POLY_ACT);

			// 背景の変化処理
			GroundChange();
		}

		break;

	case STATE_DISP:		// 表示状態

		// 背景の表示状態時の処理
		GroundDispProcess();

		if (m_nStateCount % ACTIONUI_DISP_COUNT == 0)
		{ // 状態カウントが一定数に達した場合

			// 表示処理
			DispChange();
		}

		break;

	case STATE_SMALLER:		// 縮小状態

		for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
		{
			// サイズ変化処理
			SizeChange(nCnt);

			// 向き変化処理
			Rotation(nCnt);
		}

		if (m_apActUI[POLY_ACT]->GetSize().x <= ACTIONUI_SIZE_DELETE)
		{ // サイズが一定数いかになった場合

			// 終了処理
			Uninit();

			// 褒め言葉の生成処理
			CPraiseUI::Create(m_nScore);

			// この先の処理を行わない
			return;
		}

		break;
	}

	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		// 長さ設定
		m_apActUI[nCntAct]->SetLength();

		// 方向設定
		m_apActUI[nCntAct]->SetAngle();

		// 頂点情報の初期化
		m_apActUI[nCntAct]->SetVertexRot();
	}
}

//============================
// 描画処理
//============================
void CActionUI::Draw(void)
{
	// アルファテストを有効にする
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// アルファテストの有効/無効設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		// アルファテストの設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);	// アルファテストの参照値設定

	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		// 描画処理
		m_apActUI[nCntAct]->Draw();
	}

	// アルファテストを無効にする
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// アルファテストの有効/無効設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// アルファテストの設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0);					// アルファテストの参照値設定
}

//============================
// 情報の設定処理
//============================
void CActionUI::SetData(const TYPE type, const int nScore)
{
	// ローカル変数宣言
	float fRot = (rand() % 157 - 78) * 0.01f;		// 向き

	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		if (m_apActUI[nCntAct] != nullptr)
		{ // 情報が NULL じゃない場合

			// スクロールの設定処理
			m_apActUI[nCntAct]->SetPos(ACTIONUI_POS);					// 位置設定
			m_apActUI[nCntAct]->SetRot(D3DXVECTOR3(0.0f, 0.0f, fRot));	// 向き設定
			m_apActUI[nCntAct]->SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// サイズ設定

			m_apActUI[nCntAct]->SetLength();			// 長さ設定
			m_apActUI[nCntAct]->SetAngle();				// 方向設定

			// 頂点情報の初期化
			m_apActUI[nCntAct]->SetVertexRot();

			if (nCntAct == POLY_GROUND)
			{ // 背景の場合

				// テクスチャの読み込み処理
				m_apActUI[nCntAct]->BindTexture(CManager::GetTexture()->Regist(ACTIONUI_GROUND_TEXTURE));
			}
			else if (nCntAct == POLY_ACT)
			{ // アクション名の場合

				// テクスチャの読み込み処理
				m_apActUI[nCntAct]->BindTexture(CManager::GetTexture()->Regist(m_apTexturename[type]));
			}
		}
	}

	// 全ての値を設定する
	m_nStateCount = 0;			// 状態カウント
	m_state = STATE_APPEAR;		// 状態
	m_nScore = nScore;			// 得点

	// 目標のサイズを設定する
	m_sizeDest = m_aSize[type];

	// 背景のサイズの移動量
	m_GroundMove = ACTIONUI_GROUND_SIZE_MOVE;
}

//============================
// サイズ変化処理
//============================
void CActionUI::SizeChange(const int nCnt)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = m_apActUI[nCnt]->GetSize();	// サイズを取得する
	
	if (m_nStateCount <= (ACTIONUI_APPEAR_COUNT / 2))
	{ // 状態カウントが一定数以下だった場合

		// 補正処理
		Correct(m_sizeDest.x * 1.2f, &size.x, ACTIONUI_SIZE_SUB);
		Correct(m_sizeDest.y * 1.2f, &size.y, ACTIONUI_SIZE_SUB);
	}
	else
	{ // 上記以外

		// 補正処理
		Correct(m_sizeDest.x, &size.x, ACTIONUI_SIZE_SUB);
		Correct(m_sizeDest.y, &size.y, ACTIONUI_SIZE_SUB);
	}

	// サイズの設定処理
	m_apActUI[nCnt]->SetSize(size);
}

//============================
// 出現状態変化処理
//============================
void CActionUI::AppearChange(const int nCnt)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = m_apActUI[nCnt]->GetSize();	// サイズを取得する

	// 状態カウントを初期化する
	m_nStateCount = 0;

	// 表示状態にする
	m_state = STATE_DISP;

	// サイズを設定(補正)する
	size = m_sizeDest;

	// サイズの設定処理
	m_apActUI[nCnt]->SetSize(size);
}

//============================
// 表示状態変化処理
//============================
void CActionUI::DispChange(void)
{
	// 目標のサイズを設定する
	m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 状態カウントを初期化する
	m_nStateCount = 0;

	// 縮小状態にする
	m_state = STATE_SMALLER;
}

//============================
// 向き変化処理
//============================
void CActionUI::Rotation(const int nCnt)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = m_apActUI[nCnt]->GetRot();		// 向きを取得する

	// 向きに加算する
	rot.z += ACTIONUI_ADD_ROT;

	// 向きの正規化
	RotNormalize(&rot.z);

	// 向きを設定する
	m_apActUI[nCnt]->SetRot(rot);
}

//============================
// 背景のバウンド処理
//============================
void CActionUI::GroundBound(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = m_apActUI[POLY_GROUND]->GetSize();	// サイズを取得する

	if (size.x >= m_sizeDest.x)
	{ // サイズが目標のサイズ以上になった場合

		// サイズを補正する
		size.x = m_sizeDest.x;

		// X軸の移動量を変更する
		m_GroundMove.x *= -1;
	}

	if (size.x <= 0.0f)
	{ // サイズが0.0f以下になった場合

		// サイズを補正する
		size.x = 0.0f;

		// X軸の移動量を変更する
		m_GroundMove.x *= -1;
	}

	if (size.y >= m_sizeDest.y)
	{ // サイズが目標のサイズ以上になった場合

		// サイズを補正する
		size.y = m_sizeDest.y;

		// X軸の移動量を変更する
		m_GroundMove.y *= -1;
	}

	if (size.y <= 0.0f)
	{ // サイズが0.0f以下になった場合

		// サイズを補正する
		size.y = 0.0f;

		// X軸の移動量を変更する
		m_GroundMove.y *= -1;
	}

	// サイズを加算する
	size += m_GroundMove;

	// サイズの設定処理
	m_apActUI[POLY_GROUND]->SetSize(size);
}

//============================
// 背景の状態変化処理
//============================
void CActionUI::GroundChange(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = m_apActUI[POLY_GROUND]->GetSize();	// サイズを取得する

	if (size.x <= m_sizeDest.x)
	{ // サイズが目標のサイズ以下だった場合

		// サイズの移動量を設定する
		m_GroundMove.x = -ACTIONUI_GROUND_SIZE_MOVE.x;
	}
	else
	{ // 上記以外

		// サイズを補正する
		size.x = m_sizeDest.x;

		// X軸の移動量を変更する
		m_GroundMove.x = 0.0f;
	}

	if (size.y <= m_sizeDest.y)
	{ // サイズが目標のサイズ以下だった場合

		// サイズの移動量を設定する
		m_GroundMove.y = -ACTIONUI_GROUND_SIZE_MOVE.y;
	}
	else
	{ // 上記以外

		// サイズを補正する
		size.y = m_sizeDest.y;

		// X軸の移動量を変更する
		m_GroundMove.y = 0.0f;
	}
}

//============================
// 背景の表示状態時処理
//============================
void CActionUI::GroundDispProcess(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = m_apActUI[POLY_GROUND]->GetSize();	// サイズを取得する

	if (size.x >= m_sizeDest.x)
	{ // サイズが目標のサイズ以上になった場合

		// サイズを補正する
		size.x = m_sizeDest.x;

		// X軸の移動量を変更する
		m_GroundMove.x = 0.0f;
	}

	if (size.y >= m_sizeDest.y)
	{ // サイズが目標のサイズ以上になった場合

		// サイズを補正する
		size.y = m_sizeDest.y;

		// X軸の移動量を変更する
		m_GroundMove.y = 0.0f;
	}

	// サイズを加算する
	size += m_GroundMove;

	// サイズの設定処理
	m_apActUI[POLY_GROUND]->SetSize(size);
}

//============================
//生成処理
//============================
CActionUI* CActionUI::Create(const TYPE type, const int nScore)
{
	// ローカルオブジェクトを生成
	CActionUI* pActionUI = nullptr;	// プレイヤーのインスタンスを生成

	if (pActionUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pActionUI = new CActionUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pActionUI != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pActionUI->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "アクションUIの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pActionUI->SetData(type, nScore);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// アクションUIのポインタを返す
	return pActionUI;
}