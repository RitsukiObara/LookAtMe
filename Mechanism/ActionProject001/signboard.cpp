//===========================================
//
// 看板のメイン処理[signboard.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "signboard.h"
#include "useful.h"

// マクロ定義
#define SHIFT_POS		(D3DXVECTOR3(80.0f, 190.0f, 0.0f))			// ずらす位置

//==============================
// コンストラクタ
//==============================
CSignBoard::CSignBoard() : CModel(CObject::TYPE_SIGNBOARD, CObject::PRIORITY_ENTITY)
{

}

//==============================
// デストラクタ
//==============================
CSignBoard::~CSignBoard()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CSignBoard::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CSignBoard::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CSignBoard::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CSignBoard::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CSignBoard::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos + SHIFT_POS);				// 位置
	SetRot(NONE_D3DXVECTOR3);				// 向き
	SetPosOld(pos);							// 前回の位置
	SetScale(NONE_SCALE);					// 拡大率
	SetFileData(CXFile::TYPE_SIGNBOARD);	// モデルの情報
}

//=======================================
// 生成処理
//=======================================
CSignBoard* CSignBoard::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CSignBoard* pSign = nullptr;	// インスタンスを生成

	if (pSign == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pSign = new CSignBoard;
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
		pSign->SetData(pos);
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