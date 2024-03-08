//===========================================
//
// 爆弾の導火線のメイン処理[bomb_fuse.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "bomb_fuse.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\BombFuse.x";		// モデルの名前
	const D3DXVECTOR3 FUSE_POS = D3DXVECTOR3(0.0f, 94.0f, 0.0f);	// 位置の設定処理
	const float SUB_POS_Y = 0.03f;						// 位置の減算量
}

//==============================
// コンストラクタ
//==============================
CBombFuse::CBombFuse() : CModel(TYPE_NONE, PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pMtxParent = nullptr;		// 親のマトリックス
}

//==============================
// デストラクタ
//==============================
CBombFuse::~CBombFuse()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CBombFuse::Init(void)
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
void CBombFuse::Uninit(void)
{
	// 親のマトリックスを NULL にする
	m_pMtxParent = nullptr;

	// 終了処理
	CModel::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CBombFuse::Update(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 位置を下げる
	pos.y -= SUB_POS_Y;

	// 位置を適用する
	SetPos(pos);
}

//=====================================
//ブロックの描画処理
//=====================================
void CBombFuse::Draw(void)
{
	if (m_pMtxParent != nullptr)
	{ // 親のマトリックスが NULL じゃない場合

		// 他のマトリックスとの掛け合わせ描画処理
		CModel::DrawMatrix(*m_pMtxParent);
	}
	else
	{ // 上記以外

		// 描画処理
		CModel::Draw();
	}

}

//=====================================
// 情報の設定処理
//=====================================
void CBombFuse::SetData(D3DXMATRIX* pMtx)
{
	// 情報の設定処理
	SetPos(FUSE_POS);				// 位置
	SetPosOld(FUSE_POS);			// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetScale(NONE_SCALE);			// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデルの情報

	// 全ての値を設定する
	m_pMtxParent = pMtx;		// 親のマトリックス
}

//=======================================
// 生成処理
//=======================================
CBombFuse* CBombFuse::Create(D3DXMATRIX* pMtx)
{
	// ローカルオブジェクトを生成
	CBombFuse* pFuse = nullptr;			// 導火線を生成

	if (pFuse == nullptr)
	{ // オブジェクトが NULL の場合

		// ブロックを生成する
		pFuse = new CBombFuse;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pFuse != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pFuse->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pFuse->SetData(pMtx);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 導火線のポインタを返す
	return pFuse;
}