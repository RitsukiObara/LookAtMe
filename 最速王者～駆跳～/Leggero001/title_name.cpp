//============================================
//
// タイトル処理[title_name.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "locus2D.h"
#include "renderer.h"
#include "title_name.h"
#include "title_select.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define TITLE_NAME_TEXTURE			"data/TEXTURE/KariTitle.png"			// タイトルのテクスチャ
#define TITLE_NAME_SIZE				(D3DXVECTOR3(287.0f, 175.0f, 0.0f))		// タイトルのサイズ
#define TITLE_NAME_POS				(D3DXVECTOR3(-700.0f, 250.0f, 0.0f))	// タイトルの位置
#define TITLE_NAME_LOCUS_COUNT		(5)										// タイトルの残像を出すカウント
#define TITLE_NAME_MOVE				(40.0f)									// タイトルの移動量
#define TITLE_NAME_LOCUS_ALPHA		(0.75f)									// 残像の透明度
#define TITLE_NAME_LOCUS_LIFE		(15)									// 残像の寿命

//============================
// コンストラクタ
//============================
CTitleName::CTitleName() : CObject2D(CObject2D::TYPE_LANDUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_state = STATE_MOVE;		// 状態
	m_nStateCount = 0;			// 状態カウント
}

//============================
// デストラクタ
//============================
CTitleName::~CTitleName()
{

}

//============================
// 初期化処理
//============================
HRESULT CTitleName::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_state = STATE_MOVE;			// 状態
	m_nStateCount = 0;			// 状態カウント

	// 全ての値を設定する
	SetPos(TITLE_NAME_POS);					// 位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き
	SetSize(TITLE_NAME_SIZE);				// サイズ設定
	SetLength();							// 長さ設定
	SetAngle();								// 方向設定

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(TITLE_NAME_TEXTURE));

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CTitleName::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//============================
// 更新処理
//============================
void CTitleName::Update(void)
{
	switch (m_state)
	{
	case STATE_MOVE:		// 移動状態

		// 移動状態処理
		MoveState();

		break;

	case STATE_BRAKE:

		// ブレーキ状態処理
		BrakeState();

		break;
	}
}

//============================
// 描画処理
//============================
void CTitleName::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//============================
//生成処理
//============================
CTitleName* CTitleName::Create(void)
{
	// ローカルオブジェクトを生成
	CTitleName* pTitleName = nullptr;	// プレイヤーのインスタンスを生成

	if (pTitleName == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pTitleName = new CTitleName;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pTitleName != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pTitleName->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "タイトルの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pTitleName;
}

//============================
// 移動状態処理
//============================
void CTitleName::MoveState(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 位置を加算する
	pos.x += TITLE_NAME_MOVE;

	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount % TITLE_NAME_LOCUS_COUNT == 0)
	{ // 状態カウントが一定数に達した場合

		// 残像の生成処理
		CLocus2D::Create(pos, GetSize(), TITLE_NAME_LOCUS_ALPHA, TITLE_NAME_LOCUS_LIFE, GetTexIdx());
	}

	if (pos.x >= SCREEN_WIDTH * 0.5f)
	{ // 位置が中心を超えた場合

		// X座標を補正する
		pos.x = SCREEN_WIDTH * 0.5f;

		// 状態を設定する
		m_state = STATE_BRAKE;

		// 指示を生成する
		CTitleSelect::Create();
	}

	// 位置を更新する
	SetPos(pos);

	// 頂点情報の初期化
	SetVertex();
}

//============================
// ブレーキ状態処理
//============================
void CTitleName::BrakeState(void)
{

}