//============================================
//
// 新記録処理[new_record.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "new_record.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define NEW_RECORD_POS_X			(300.0f)								// 新記録の位置(X軸)
#define NEW_RECORD_SIZE				(D3DXVECTOR3(160.0f, 60.0f, 0.0f))		// 新記録のサイズ
#define NEW_RECORD_TEXTURE			"data/TEXTURE/NewRecord.png"			// 新記録のテクスチャ

//========================
// コンストラクタ
//========================
CNewRecord::CNewRecord() : CObject2D(CObject::TYPE_NEWRECORD, CObject::PRIORITY_UI)
{

}

//========================
// デストラクタ
//========================
CNewRecord::~CNewRecord()
{

}

//========================
// 初期化処理
//========================
HRESULT CNewRecord::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CNewRecord::Uninit(void)
{
	// 破棄処理
	CObject2D::Uninit();
}

//========================
// 更新処理
//========================
void CNewRecord::Update(void)
{

}

//========================
// 描画処理
//========================
void CNewRecord::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//========================
// 情報の設定処理
//========================
void CNewRecord::SetData(const D3DXVECTOR3 pos)
{
	// 情報の設定
	SetPos(D3DXVECTOR3(NEW_RECORD_POS_X,pos.y,0.0f));					// 位置
	SetPosOld(GetPos());					// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き
	SetSize(NEW_RECORD_SIZE);				// サイズ
	SetAngle();								// 方向
	SetLength();							// 長さ

	SetVertex();							// 頂点座標の設定処理

	// テクスチャの割り当て処理
	BindTexture(CManager::Get()->GetTexture()->Regist(NEW_RECORD_TEXTURE));
}

//========================
// 生成処理
//========================
CNewRecord* CNewRecord::Create(const D3DXVECTOR3 pos)
{
	// ローカルオブジェクトを生成
	CNewRecord* pNewRecord = nullptr;	// プレイヤーのインスタンスを生成

	if (pNewRecord == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pNewRecord = new CNewRecord;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pNewRecord != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pNewRecord->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pNewRecord->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 新記録のポインタを返す
	return pNewRecord;
}