//===========================================
//
// チュートリアルふきだしのメイン処理[tutorial.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "tutorial.h"
#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// 名前空間(テクスチャ)
//-------------------------------------------
namespace
{
	static const char* c_apTextureFileNameKey[CPork::TYPE_MAX] =		// キーボードのテクスチャの名前
	{
		"data\\TEXTURE\\KeyTutorial001.png",
		"data\\TEXTURE\\KeyTutorial002.png",
		"data\\TEXTURE\\KeyTutorial003.png",
	};
	static const char* c_apTextureFileNameButton[CPork::TYPE_MAX] =		// ゲームパッドのテクスチャの名前
	{
		"data\\TEXTURE\\ButTutorial001.png",
		"data\\TEXTURE\\ButTutorial002.png",
		"data\\TEXTURE\\ButTutorial003.png",
	};
}

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define TUTORIAL_SIZE		(D3DXVECTOR3(100.0f, 100.0f, 0.0f))		// チュートリアルのサイズ
#define SHIFT				(D3DXVECTOR3(-90.0f, 220.0f, 0.0f))		// ずらす幅

//==============================
// コンストラクタ
//==============================
CTutorial::CTutorial() : CBillboard(CObject::TYPE_TUTORIAL, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_type = CPork::TYPE_HOVER;		// 種類
}

//==============================
// デストラクタ
//==============================
CTutorial::~CTutorial()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CTutorial::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_type = CPork::TYPE_HOVER;		// 種類

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CTutorial::Uninit(void)
{
	// 終了処理
	CBillboard::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CTutorial::Update(void)
{
	if (CManager::Get()->GetInputGamePad()->GetConnect() == true)
	{ // ゲームパッドを繋いでいた場合

		// テクスチャの割り当て処理
		BindTexture(CManager::Get()->GetTexture()->Regist(c_apTextureFileNameButton[m_type]));
	}
	else
	{ // 上記以外

		// テクスチャの割り当て処理
		BindTexture(CManager::Get()->GetTexture()->Regist(c_apTextureFileNameKey[m_type]));
	}
}

//=====================================
//ブロックの描画処理
//=====================================
void CTutorial::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff();
}

//=====================================
// 情報の設定処理
//=====================================
void CTutorial::SetData(const D3DXVECTOR3& pos, const CPork::TYPE type)
{
	// 情報の設定処理
	SetPos(pos + SHIFT);			// 位置
	SetPosOld(pos + SHIFT);			// 前回の位置
	SetSize(TUTORIAL_SIZE);			// サイズ

	// 全ての値を設定する
	m_type = type;		// 種類

	if (CManager::Get()->GetInputGamePad()->GetConnect() == true)
	{ // ゲームパッドを繋いでいた場合

		// テクスチャの割り当て処理
		BindTexture(CManager::Get()->GetTexture()->Regist(c_apTextureFileNameButton[m_type]));
	}
	else
	{ // 上記以外

		// テクスチャの割り当て処理
		BindTexture(CManager::Get()->GetTexture()->Regist(c_apTextureFileNameKey[m_type]));
	}

	// 下中心の頂点情報設定処理
	CBillboard::SetVtxPosUnder();
}

//=======================================
// 生成処理
//=======================================
CTutorial* CTutorial::Create(const D3DXVECTOR3& pos, const CPork::TYPE type)
{
	// ローカルオブジェクトを生成
	CTutorial* pSign = nullptr;	// インスタンスを生成

	if (pSign == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pSign = new CTutorial;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pSign != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pSign->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pSign->SetData(pos, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 看板のポインタを返す
	return pSign;
}