//============================================
//
// ENTERスキップ処理[enter_skip.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "enter_skip.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define ENTER_SKIP_TEXTURE	"data/TEXTURE/EnterSkip.png"			// テクスチャ
#define ENTER_SKIP_POS		(D3DXVECTOR3(200.0f, 690.0f, 0.0f))		// 位置
#define ENTER_SKIP_SIZE		(D3DXVECTOR3(200.0f, 20.0f, 0.0f))		// サイズ

//============================
// コンストラクタ
//============================
CEnterSkip::CEnterSkip() : CObject2D(CObject::TYPE_ENTERSKIP, PRIORITY_UI)
{

}

//============================
// デストラクタ
//============================
CEnterSkip::~CEnterSkip()
{

}

//============================
// 初期化処理
//============================
HRESULT CEnterSkip::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CEnterSkip::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//============================
// 更新処理
//============================
void CEnterSkip::Update(void)
{

}

//============================
// 描画処理
//============================
void CEnterSkip::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//============================
// 情報の設定処理
//============================
void CEnterSkip::SetData(void)
{
	// 情報の初期化
	SetPos(ENTER_SKIP_POS);						// 位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き
	SetSize(ENTER_SKIP_SIZE);					// サイズ
	SetAngle();									// 方向
	SetLength();								// 長さ

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(ENTER_SKIP_TEXTURE));
}

//============================
//生成処理
//============================
CEnterSkip* CEnterSkip::Create(void)
{
	// ローカルオブジェクトを生成
	CEnterSkip* pEnterSkip = nullptr;	// プレイヤーのインスタンスを生成

	if (pEnterSkip == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pEnterSkip = new CEnterSkip;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pEnterSkip != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pEnterSkip->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "スキップ表示の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEnterSkip->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// タイトルの選択肢のポインタを返す
	return pEnterSkip;
}