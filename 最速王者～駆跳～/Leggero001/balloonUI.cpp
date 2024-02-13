//============================================
//
// 吹き出しUI処理[balloonUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "balloonUI.h"
#include "file.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define BALLOON_POS				(D3DXVECTOR3(1100.0f, 610.0f, 0.0f))	// 位置
#define SIZE_MAGNI				(0.7f)									// サイズの倍率

//============================
// コンストラクタ
//============================
CBalloonUI::CBalloonUI() : CObject2D(CObject::TYPE_BALLOONUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_type = CBalloon::TYPE_NORMAL;		// 種類
}

//============================
// デストラクタ
//============================
CBalloonUI::~CBalloonUI()
{

}

//============================
// 初期化処理
//============================
HRESULT CBalloonUI::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_type = CBalloon::TYPE_NORMAL;		// 種類

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CBalloonUI::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//============================
// 更新処理
//============================
void CBalloonUI::Update(void)
{

}

//============================
// 描画処理
//============================
void CBalloonUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//============================
// 情報の設定処理
//============================
void CBalloonUI::SetData(void)
{
	// ローカル変数宣言
	CFile::SBalloonData data;		// データ

	// 全ての値を設定する
	m_type = CBalloon::TYPE_NORMAL;		// 種類

	if (CManager::GetFile() != nullptr)
	{ // ファイルのポインタがあった場合

		// データを設定する
		data = CManager::GetFile()->GetBalloon((CBalloon::TYPE)(m_type));
	}

	// 情報の初期化
	SetPos(BALLOON_POS);						// 位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き
	SetSize(data.size * SIZE_MAGNI);			// サイズ
	SetAngle();									// 方向
	SetLength();								// 長さ

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(data.aTextureName));
}

//============================
// 種類の設定処理
//============================
void CBalloonUI::SetType(const CBalloon::TYPE nType)
{
	if (m_type != nType)
	{ // 種類が違った場合

		// 種類を設定する
		m_type = nType;

		if (CManager::GetFile() != nullptr)
		{ // ファイルのポインタがあった場合

			// ローカル変数宣言
			CFile::SBalloonData data = CManager::GetFile()->GetBalloon((CBalloon::TYPE)(nType));

			// サイズを設定する
			SetSize(data.size * SIZE_MAGNI);

			// テクスチャの割り当て処理
			BindTexture(CManager::GetTexture()->Regist(data.aTextureName));
		}
	}
}

//============================
// 種類の取得処理
//============================
CBalloon::TYPE CBalloonUI::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//============================
//生成処理
//============================
CBalloonUI* CBalloonUI::Create(void)
{
	// ローカルオブジェクトを生成
	CBalloonUI* pBalloon = nullptr;	// プレイヤーのインスタンスを生成

	if (pBalloon == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pBalloon = new CBalloonUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pBalloon != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBalloon->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "吹き出しUIの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBalloon->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// タイトルの選択肢のポインタを返す
	return pBalloon;
}