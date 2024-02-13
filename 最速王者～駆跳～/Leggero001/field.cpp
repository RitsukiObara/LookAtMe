//============================================
//
// フィールドのメイン処理[object3D.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "field.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

#include "input.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define FIELD_TEXTURE			"data/TEXTURE/FatherFace.png"		// フィールドのテクスチャ

//=========================================
// コンストラクタ
//=========================================
CField::CField() : CObject3D(CObject::TYPE_FIELD, CObject::PRIORITY_BG)
{

}

//=========================================
// デストラクタ
//=========================================
CField::~CField()
{

}

//===========================================
// フィールドの初期化処理
//===========================================
HRESULT CField::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//===========================================
// フィールドの終了処理
//===========================================
void CField::Uninit(void)
{
	// オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//===========================================
// フィールドの更新処理
//===========================================
void CField::Update(void)
{

}

//===========================================
// フィールドの描画処理
//===========================================
void CField::Draw(void)
{
	// 描画処理
	CObject3D::Draw();
}

//===========================================
// 情報の設定処理
//===========================================
void CField::SetData(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// 情報の設定
	SetPos(pos);								// 位置
	SetPosOld(pos);								// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き
	SetSize(size);								// サイズ

	// 頂点情報の設定処理
	SetVertexElev(300.0f);

	// テクスチャの読み込み処理
	BindTexture(CManager::GetTexture()->Regist(FIELD_TEXTURE));
}

//===========================================
// 生成処理
//===========================================
CField* CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ローカルオブジェクトを生成
	CField* pField = nullptr;	// フィールドのインスタンスを生成

	if (pField == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pField = new CField;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pField != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pField->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "フィールドの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pField->SetData(pos, size);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// フィールドのポインタを返す
	return pField;
}