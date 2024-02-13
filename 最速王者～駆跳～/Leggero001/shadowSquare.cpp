//===========================================
//
// 四角影のメイン処理[shadowCircle.cpp]
// Author 小原立暉
//
//===========================================
#include "main.h"
#include "shadowSquare.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
int CShadowSquare::m_nNumAll = 0;				// 影の総数

//====================
// コンストラクタ
//====================
CShadowSquare::CShadowSquare() : CObject3D(TYPE_SHADOW, PRIORITY_SHADOW)
{
	// 全ての値をクリアする
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_nID = INIT_SHADOW;						// 影のインデックス
}

//====================
// デストラクタ
//====================
CShadowSquare::~CShadowSquare()
{

}

//====================
// 初期化処理
//====================
HRESULT CShadowSquare::Init(void)
{
	// 初期化処理
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_nID = INIT_SHADOW;						// 影のインデックス

	// 成功を返す
	return S_OK;
}

//====================
// 終了処理
//====================
void CShadowSquare::Uninit(void)
{
	// オブジェクトの終了処理
	CObject3D::Uninit();

	// 影の総数を減算する
	m_nNumAll--;
}

//====================
// 更新処理
//====================
void CShadowSquare::Update(void)
{

}

//====================
// 描画処理
//====================
void CShadowSquare::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	// 描画処理
	CObject3D::Draw();

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定
}

//====================
// 情報の設定処理
//====================
void CShadowSquare::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// 情報の初期化
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_nID = m_nNumAll;								// 番号

	// 影の総数を加算する
	m_nNumAll++;

	// 情報の設定
	SetPos(D3DXVECTOR3(pos.x, 0.0f, pos.z));			// 位置
	SetPosOld(pos);			// 前回の位置
	SetRot(rot);			// 向き
	SetSize(size);			// サイズ

	// 設定処理
	AlphaSet();

	// 頂点情報の設定処理
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(m_col);
}

//====================
// 透明度の設定処理
//====================
void CShadowSquare::AlphaSet(void)
{

}

//====================
// IDの取得処理
//====================
int CShadowSquare::GetNumID(void) const
{
	// 影の番号を返す
	return m_nID;
}

//====================
// 四角影の取得処理
//====================
CShadowSquare* CShadowSquare::GetShadow(int nIdx)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;			// オブジェクトのポインタ
	CShadowSquare* pShadow = nullptr;	// 影のポインタ

	if (nIdx != INIT_SHADOW)
	{ // 影のインデックスがある場合

		// オブジェクトの情報を取得する
		pObj = GetTop(PRIORITY_SHADOW);

		while (pObj != nullptr)
		{ // オブジェクトのポインタが NULL じゃない限り回す

			if (pObj->GetType() == TYPE_SHADOW)
			{ // 種類が影の場合

				// 影の情報を取得する
				pShadow = dynamic_cast<CShadowSquare*>(pObj);

				if (pShadow != nullptr)
				{ // 影のポインタが NULL じゃない場合

					if (pShadow->GetNumID() == nIdx)
					{ // 影の番号が引数と一致した場合

						// 影のポインタを返す
						return pShadow;
					}
					else
					{ // 上記以外

						// 次のオブジェクトを設定する
						pObj = pObj->GetNext();
					}
				}
				else
				{ // 影のポインタが NULL の場合

					// 次のオブジェクトを設定する
					pObj = pObj->GetNext();
				}
			}
			else
			{ // 上記以外

				// 次のオブジェクトを設定する
				pObj = pObj->GetNext();
			}
		}

		// NULL を返す
		return nullptr;
	}
	else
	{ // 上記以外

		// NULL を返す
		return nullptr;
	}
}

//====================
// 生成処理
//====================
CShadowSquare* CShadowSquare::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// ローカルオブジェクトを生成
	CShadowSquare* pShadow = nullptr;	// 影のインスタンスを生成

	if (pShadow == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pShadow = new CShadowSquare;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pShadow != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pShadow->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "四角影の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pShadow->SetData(pos, rot, size);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 影のポインタを返す
	return pShadow;
}