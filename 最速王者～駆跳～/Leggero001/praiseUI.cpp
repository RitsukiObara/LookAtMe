//============================================
//
// 褒め言葉UI処理[praiseUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "praiseUI.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "texture.h"

#include "score.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define AMAZING_SIZE			(D3DXVECTOR3(90.0f,15.0f,0.0f))		// アメイジングのサイズ
#define GREAT_SIZE				(D3DXVECTOR3(60.0f,15.0f,0.0f))		// グレートのサイズ
#define EXCITING_SIZE			(D3DXVECTOR3(90.0f,15.0f,0.0f))		// エキサイティングのサイズ
#define POS_DEST				(D3DXVECTOR3(750.0f, 40.0f, 0.0f))	// 褒め言葉の目的の位置
#define SCALING_MAGNI			(0.05f)								// 褒め言葉の拡大率
#define MOVE_COUNT				(15)								// 移動状態のカウント数
#define APPEAR_CHANGE_COUNT		(40)								// 出現状態から変わるカウント数
#define ROT_MOVE				(0.2f)								// 回転速度

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CPraiseUI::m_apTexturename[CPraiseUI::TYPE_MAX] =				// テクスチャのパス名
{
	"data/TEXTURE/Amazing.png",			// アメイジング
	"data/TEXTURE/Great.png",			// グレート
	"data/TEXTURE/Exciting.png",		// エキサイティング
};

//============================
// コンストラクタ
//============================
CPraiseUI::CPraiseUI() : CObject2D(CObject2D::TYPE_PRAISEUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的のサイズ
	m_scaling = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの拡大率
	m_state = STATE_APPEAR;							// 状態
	m_nScore = 0;									// 得点
}

//============================
// デストラクタ
//============================
CPraiseUI::~CPraiseUI()
{

}

//============================
// 初期化処理
//============================
HRESULT CPraiseUI::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_posDest = POS_DEST;							// 目的の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的のサイズ
	m_scaling = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの拡大率
	m_state = STATE_APPEAR;							// 状態
	m_nScore = 0;									// 得点

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CPraiseUI::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//============================
// 更新処理
//============================
void CPraiseUI::Update(void)
{
	switch (m_state)
	{
	case STATE_APPEAR:

		// 出現処理
		Appear();

		break;

	case STATE_MOVE:

		// 状態カウントを加算する
		m_nStateCount++;

		// 移動処理
		Move();

		// 回転処理
		Cycle();

		// 縮小処理
		ScaleDown();	

		if (m_nStateCount >= MOVE_COUNT)
		{ // 状態カウントが一定数以上になった場合

			if (CManager::GetMode() == CScene::MODE_GAME)
			{ // ゲームモードの場合

				if (CGame::GetScore() != nullptr)
				{ // スコアの情報があった場合

					// アクションスコアを加算する
					CGame::GetScore()->AddScore(m_nScore);
				}
			}

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

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
void CPraiseUI::Draw(void)
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
void CPraiseUI::SetData(const int nScore)
{
	// ローカル変数宣言
	TYPE type;				// 種類

	// 種類をランダムで決める
	type = (TYPE)(rand() % TYPE_MAX);

	// スクロールの設定処理
	SetPos(D3DXVECTOR3(1080.0f, 150.0f, 0.0f));		// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// 向き設定
	SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// サイズ設定

	switch (type)
	{
	case TYPE_AMAZING:			// アメイジング

		m_sizeDest = AMAZING_SIZE;		// 目的のサイズ

		break;

	case TYPE_GREAT:			// グレート

		m_sizeDest = GREAT_SIZE;		// 目的のサイズ

		break;

	case TYPE_EXCITING:			// エキサイティング

		m_sizeDest = EXCITING_SIZE;		// 目的のサイズ

		break;

	default:

		// 警告文
		MessageBox(NULL, "褒め演出UIで予期しない種類を検知！", "警告！", MB_ICONWARNING);

		break;
	}

	// 頂点情報の初期化
	SetVertexRot();

	// 全ての値を設定する
	m_posDest = POS_DEST;						// 目的の位置
	m_move = (m_posDest - GetPos()) / (float)MOVE_COUNT;		// 移動量
	m_state = STATE_APPEAR;						// 状態
	m_nScore = nScore;							// 得点
	m_scaling = m_sizeDest * SCALING_MAGNI;		// サイズの拡大率

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(m_apTexturename[type]));
}

//============================
// 出現処理
//============================
void CPraiseUI::Appear(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();		// サイズ

	if (size.x >= m_sizeDest.x ||
		size.y >= m_sizeDest.y)
	{ // 現在のサイズが目的のサイズ以上だった場合

		// 目的のサイズに補正する
		size = m_sizeDest;

		// 状態カウントを加算する
		m_nStateCount++;
	}
	else
	{ // 上記以外

		// サイズを拡大する
		size += m_scaling;
	}

	if (m_nStateCount >= APPEAR_CHANGE_COUNT)
	{ // 状態カウントが一定以上になった場合

		// 移動状態にする
		m_state = STATE_MOVE;

		// 状態カウントを 0 にする
		m_nStateCount = 0;

		// 目標のサイズを設定する
		m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// サイズの拡大率を設定する
		m_scaling = (m_sizeDest - size) / (float)MOVE_COUNT;
	}

	// サイズの設定処理
	SetSize(size);
}

//============================
// 移動処理
//============================
void CPraiseUI::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	// 移動量を位置に加算する
	pos += m_move;

	// 情報を更新する
	SetPos(pos);		// 位置
}

//============================
// 回転処理
//============================
void CPraiseUI::Cycle(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向き

	// 向きを加算する
	rot.z += ROT_MOVE;

	// 向きを更新する
	SetRot(rot);		
}

//============================
// 縮小処理
//============================
void CPraiseUI::ScaleDown(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();		// サイズ

	if (size.x <= m_sizeDest.x ||
		size.y <= m_sizeDest.y)
	{ // 現在のサイズが目的のサイズ以上だった場合

		// 目的のサイズに補正する
		size = m_sizeDest;
	}
	else
	{ // 上記以外

		// サイズを拡大する
		size += m_scaling;
	}

	// サイズの設定処理
	SetSize(size);
}

//============================
//生成処理
//============================
CPraiseUI* CPraiseUI::Create(const int nScore)
{
	// ローカルオブジェクトを生成
	CPraiseUI* pLandUI = nullptr;	// プレイヤーのインスタンスを生成

	if (pLandUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLandUI = new CPraiseUI;
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
			MessageBox(NULL, "褒め言葉UIの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pLandUI->SetData(nScore);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pLandUI;
}