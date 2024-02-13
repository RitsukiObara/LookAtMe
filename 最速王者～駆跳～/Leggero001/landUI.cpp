//============================================
//
// 着地UI処理[landUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "renderer.h"
#include "landUI.h"
#include "texture.h"

#include "locus2D.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define LANDUI_DEST_POS			(D3DXVECTOR3(1000.0f, 500.0f, 0.0f))		// 着地UIの位置
#define LANDUI_INIT_POS			(D3DXVECTOR3(1600.0f, 500.0f, 0.0f))		// 着地UIの位置
#define LANDUI_SIZE				(D3DXVECTOR3(150.0f, 60.0f, 0.0f))			// 着地UIのBADのサイズ
#define LANDUI_DISP_CNT			(10)										// 表示状態に移行するカウント数
#define LANDUI_DELETE_CNT		(40)										// 消去状態に移行するカウント数
#define LANDUI_MOVE				((LANDUI_DEST_POS.x - LANDUI_INIT_POS.x) / LANDUI_DISP_CNT)		// 移動量
#define LANDUI_LOCUS_ALPHA		(0.5f)										// 残像の透明度
#define LANDUI_LOCUS_LIFE		(7)											// 残像の寿命
#define LANDUI_LOCUS_INTERVAL	(2)											// 残像の寿命

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CLandUI::m_apTexturename[CLandUI::TYPE_MAX] =				// テクスチャのパス名
{
	"data/TEXTURE/BOO.png",												// BOO
	"data/TEXTURE/COOL.png",											// COOL
};

//============================
// コンストラクタ
//============================
CLandUI::CLandUI() : CObject2D(CObject2D::TYPE_LANDUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_type = TYPE_BAD;				// 種類
	m_state = STATE_APPEAR;			// 状態
	m_nStateCount = 0;				// 状態カウント
}

//============================
// デストラクタ
//============================
CLandUI::~CLandUI()
{

}

//============================
// 初期化処理
//============================
HRESULT CLandUI::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_type = TYPE_BAD;				// 種類
	m_state = STATE_APPEAR;			// 状態
	m_nStateCount = 0;				// 状態カウント

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CLandUI::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//============================
// 更新処理
//============================
void CLandUI::Update(void)
{
	// 状態カウントを加算する
	m_nStateCount++;

	switch (m_state)
	{
	case STATE_APPEAR:		// 出現状態

		// 移動処理
		Move();

		// 表示状態判定処理
		DispDecision();

		if (m_type == TYPE_GOOD && 
			m_nStateCount % LANDUI_LOCUS_INTERVAL == 0)
		{ // GOODかつ、一定時間ごとに

			// 残像の生成処理
			CLocus2D::Create(GetPos(), GetSize(), LANDUI_LOCUS_ALPHA, LANDUI_LOCUS_LIFE, GetTexIdx());
		}

		break;

	case STATE_DISP:		// 表示処理

		if (m_nStateCount >= LANDUI_DELETE_CNT)
		{ // 状態カウントが一定数以上になった場合

			// 消去状態にする
			m_state = STATE_DELETE;

			// 状態カウントを初期化する
			m_nStateCount = 0;
		}

		break;

	case STATE_DELETE:		// 消去状態

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;

		break;

	default:				// 上記以外

		// 警告文
		MessageBox(NULL, "着地UIで予期しない状態を検知！", "警告！", MB_ICONWARNING);

		break;
	}

	SetLength();							// 長さ設定
	SetAngle();								// 方向設定

	// 頂点情報の初期化
	SetVertexRot();
}

//============================
// 描画処理
//============================
void CLandUI::Draw(void)
{
	// アルファテストを有効にする
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// アルファテストの有効/無効設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		// アルファテストの設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);	// アルファテストの参照値設定

	// 描画処理
	CObject2D::Draw();

	// アルファテストを無効にする
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// アルファテストの有効/無効設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// アルファテストの設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0);					// アルファテストの参照値設定
}

//============================
// 情報の設定処理
//============================
void CLandUI::SetData(TYPE type)
{
	// ローカル変数宣言
	float fRot = (rand() % 101 - 50) * 0.01f;		// 向き

	// スクロールの設定処理
	SetPos(LANDUI_INIT_POS);						// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, fRot));	// 向き設定
	SetSize(LANDUI_SIZE);			// サイズ設定

	SetLength();							// 長さ設定
	SetAngle();								// 方向設定

	// 頂点情報の初期化
	SetVertexRot();

	// 全ての値をクリアする
	m_type = type;				// 種類
	m_state = STATE_APPEAR;			// 状態
	m_nStateCount = 0;				// 状態カウント

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(m_apTexturename[m_type]));
}

//============================
//生成処理
//============================
CLandUI* CLandUI::Create(TYPE type)
{
	// ローカルオブジェクトを生成
	CLandUI* pLandUI = nullptr;	// プレイヤーのインスタンスを生成

	if (pLandUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLandUI = new CLandUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pLandUI != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLandUI->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "着地UIの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pLandUI->SetData(type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pLandUI;
}

//============================
// 移動処理
//============================
void CLandUI::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	// 移動量を加算する
	pos.x += LANDUI_MOVE;

	// 位置の設定処理
	SetPos(pos);
}

//============================
// 表示状態判定処理
//============================
void CLandUI::DispDecision(void)
{
	if (m_nStateCount >= LANDUI_DISP_CNT)
	{ // 状態カウントが一定数以上になった場合

		// 位置を設定する
		SetPos(LANDUI_DEST_POS);

		// 表示状態にする
		m_state = STATE_DISP;

		// 状態カウントを初期化する
		m_nStateCount = 0;
	}
}