//============================================
//
// 当たり判定ポリゴンのメイン処理[collpolygon.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "collpolygon.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------
// 静的メンバ変数
//--------------------------------------------
int CCollPolygon::m_nNumAll = 0;			// ポリゴンの総数

//===========================================
// コンストラクタ
//===========================================
CCollPolygon::CCollPolygon() : CObject3D(TYPE_COLLPOLY, CObject::PRIORITY_BG)
{
	// 全ての値をクリアする
	m_vtxLeftUp = NONE_D3DXVECTOR3;		// 左上の座標
	m_vtxRightUp = NONE_D3DXVECTOR3;	// 右上の座標
	m_vtxLeftDown = NONE_D3DXVECTOR3;	// 左下の座標
	m_vtxRightDown = NONE_D3DXVECTOR3;	// 右下の座標
	m_nNumID = NONE_COLLIDX;			// ポリゴンの番号
}

//===========================================
// デストラクタ
//===========================================
CCollPolygon::~CCollPolygon()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CCollPolygon::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CCollPolygon::Uninit(void)
{
	// 終了
	CObject3D::Uninit();

	// 総数を減算する
	m_nNumAll--;
}

//===========================================
// 更新処理
//===========================================
void CCollPolygon::Update(void)
{
	// ローカル変数を宣言
	D3DXVECTOR3 pos = GetPos();			// 弾の位置

	// 前回の位置の設定処理
	SetPosOld(pos);
}

//===========================================
// 描画処理
//===========================================
void CCollPolygon::Draw(void)
{
// デバッグ用
#ifdef _DEBUG

	// 色を設定する
	CObject3D::SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

#else

	// 色を設定する
	CObject3D::SetVtxColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

#endif

	// 描画処理
	CObject3D::Draw();
}

//===========================================
// 情報の設定処理
//===========================================
void CCollPolygon::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin)
{
	// 情報の設定処理
	SetPos(pos);								// 位置
	SetPosOld(pos);								// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き
	SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// サイズ

	// 全ての値を設定する
	m_vtxLeftUp = D3DXVECTOR3(VtxMin.x, VtxMax.y, VtxMax.z);	// 左上の座標
	m_vtxRightUp = D3DXVECTOR3(VtxMax.x, VtxMax.y, VtxMin.z);	// 右上の座標
	m_vtxLeftDown = D3DXVECTOR3(VtxMin.x, VtxMin.y, VtxMax.z);	// 左下の座標
	m_vtxRightDown = D3DXVECTOR3(VtxMax.x, VtxMin.y, VtxMin.z);	// 右下の座標

	// 頂点情報の設定処理(決め打ち版)
	SetVertexHardCoding(m_vtxLeftUp, m_vtxRightUp, m_vtxLeftDown, m_vtxRightDown);

	// ポリゴンの番号を設定する
	m_nNumID = m_nNumAll;

	// 総数を加算する
	m_nNumAll++;

// デバッグ用
#ifdef _DEBUG

	// 色を設定する
	CObject3D::SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

#else

	// 色を設定する
	CObject3D::SetVtxColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

#endif
}

//===========================================
// 左上座標の取得処理
//===========================================
D3DXVECTOR3 CCollPolygon::GetLeftUp(void) const
{
	// 左上の座標を返す
	return m_vtxLeftUp;
}

//===========================================
// 右上座標の取得処理
//===========================================
D3DXVECTOR3 CCollPolygon::GetRightUp(void) const
{
	// 右上の座標を返す
	return m_vtxRightUp;
}

//===========================================
// 左下座標の取得処理
//===========================================
D3DXVECTOR3 CCollPolygon::GetLeftDown(void) const
{
	// 左下の座標を返す
	return m_vtxLeftDown;
}

//===========================================
// 右下座標の取得処理
//===========================================
D3DXVECTOR3 CCollPolygon::GetRightDown(void) const
{
	// 右下の座標を返す
	return m_vtxRightDown;
}

//===========================================
// ポリゴンの番号の取得処理
//===========================================
int CCollPolygon::GetNumID(void) const
{
	// 番号を返す
	return m_nNumID;
}

//===========================================
// 位置と向きの設定処理
//===========================================
void CCollPolygon::SetPosRot(const int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	if (nIdx > NONE_COLLIDX)
	{ // 番号が -1 超過の場合

		// ローカルポインタを宣言
		CCollPolygon* pPoly = GetCollPolygon(nIdx);

		if (pPoly != nullptr)
		{ // ポインタが NULL じゃない場合

			// 位置の設定処理
			pPoly->SetPos(pos);

			// 向きの設定処理
			pPoly->SetRot(rot);
		}
		else
		{ // ポインタが NULL の場合

			// 停止
			assert(false);
		}
	}
}

//===========================================
// 生成処理
//===========================================
int CCollPolygon::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin)
{
	// ローカルオブジェクトを生成
	CCollPolygon* pCollPolygon = nullptr;	// プレイヤーのインスタンスを生成

	if (pCollPolygon == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pCollPolygon = new CCollPolygon;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// -1 を返す
		return NONE_COLLIDX;
	}

	if (pCollPolygon != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCollPolygon->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// -1 を返す
			return NONE_COLLIDX;
		}

		// 情報の設定処理
		pCollPolygon->SetData(pos, VtxMax, VtxMin);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// -1 を返す
		return NONE_COLLIDX;
	}

	// 当たり判定ポリゴンの番号を返す
	return pCollPolygon->GetNumID();
}

//===========================================
// 破棄処理
//===========================================
int CCollPolygon::Delete(const int nIdx)
{
	// ローカル変数宣言
	CCollPolygon* pColl = GetCollPolygon(nIdx);		// 当たり判定ポリゴンのポインタ

	if (pColl != nullptr)
	{ // 当たり判定ポリゴンが NULL じゃない場合

		// 終了処理
		pColl->Uninit();
	}

	// インデックスを初期化する
	return NONE_COLLIDX;
}

//===========================================
// 当たり判定ポリゴンの取得処理
//===========================================
CCollPolygon* CCollPolygon::GetCollPolygon(const int nIdx)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;			// オブジェクトのポインタ
	CCollPolygon* pCollPoly = nullptr;	// 影のポインタ

	if (nIdx != INIT_SHADOW)
	{ // 影のインデックスがある場合

		// オブジェクトの情報を取得する
		pObj = GetTop(DIM_3D, PRIORITY_BG);

		while (pObj != nullptr)
		{ // オブジェクトのポインタが NULL じゃない限り回す

			if (pObj->GetType() == TYPE_COLLPOLY)
			{ // 種類が当たり判定ポリゴンの場合

				// 影の情報を取得する
				pCollPoly = dynamic_cast<CCollPolygon*>(pObj);

				if (pCollPoly != nullptr)
				{ // 影のポインタが NULL じゃない場合

					if (pCollPoly->GetNumID() == nIdx)
					{ // 影の番号が引数と一致した場合

						// 影のポインタを返す
						return pCollPoly;
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