//============================================================
//
// Xファイル処理 [xfile.cpp]
// Author：小原立暉
//
//============================================================
//************************************************************
// インクルードファイル
//************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "texture.h"
#include "useful.h"

//=====================================================================ここからCModelの処理=====================================================================

//========================
// オーバーロードコンストラクタ
//========================
CModel::CModel(CObject::TYPE type, PRIORITY priority) : CObject(type, DIM_3D, priority)
{
	// 全ての値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_scale = NONE_SCALE;								// 拡大率
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));		// ワールドマトリックス
	ZeroMemory(&m_XFileData, sizeof(CXFile::SXFile));	// Xファイルのデータ
}

//========================
// デストラクタ
//========================
CModel::~CModel()
{

}

//========================
// 初期化処理
//========================
HRESULT CModel::Init(void)
{
	// 全ての値を初期化する
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = m_pos;			// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_scale = NONE_SCALE;		// 拡大率

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CModel::Uninit(void)
{
	// 破棄処理
	Release();
}

//========================
// 更新処理
//========================
void CModel::Update(void)
{

}

//========================
// 描画処理
//========================
void CModel::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// 描画処理(色処理)
//========================
void CModel::Draw(D3DXCOLOR col)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL     colMat;					// 描画用マテリアル

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// 構造体の要素をクリア
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// マテリアルデータのコピーに代入する
		colMat = pMat[nCntMat];

		// 透明度を代入する
		colMat.MatD3D.Diffuse = col;
		colMat.MatD3D.Ambient = col;
		colMat.MatD3D.Emissive = col;

		// マテリアルの設定
		pDevice->SetMaterial(&colMat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// モデル影の描画処理
//========================
void CModel::DrawShadow(void)
{
	// 変数を宣言
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     colMat;					// 描画用マテリアル

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// 構造体の要素をクリア
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// 透明度を代入する
		colMat.MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		colMat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアルの設定
		pDevice->SetMaterial(&colMat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// エディットの描画処理
//========================
void CModel::DrawBlock(const D3DXMATERIAL* pMat)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// 他のマトリックスとの掛け合わせ描画処理
//========================
void CModel::DrawMatrix(D3DXMATRIX mtxParent)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL* pMat;						// マテリアルデータへのポインタ

	D3DXMATRIX mtxRotModel, mtxTransModel;		// 計算用マトリックス

	// パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
	D3DXMatrixMultiply
	(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// 描画処理(複数色)
//========================
void CModel::Draw(D3DXCOLOR* col)
{
	// 変数を宣言
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     colMat;					// 描画用マテリアル

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// 構造体の要素をクリア
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// 色を代入する
		colMat.MatD3D.Diffuse = col[nCntMat];

		// マテリアルの設定
		pDevice->SetMaterial(&colMat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// 描画処理(透明度調整)
//========================
void CModel::Draw(const float fAlpha)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL     colMat;					// 描画用マテリアル

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// 構造体の要素をクリア
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// マテリアルデータのコピーに代入する
		colMat = pMat[nCntMat];

		// 透明度を代入する
		colMat.MatD3D.Diffuse.a = fAlpha;
		colMat.MatD3D.Ambient.a = fAlpha;
		colMat.MatD3D.Emissive.a = fAlpha;

		// マテリアルの設定
		pDevice->SetMaterial(&colMat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// モデルの当たり判定
//========================
bool CModel::ModelCollision(D3DXVECTOR3* pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 nor, vec1, vec2;	// 法線、位置
	int nNumVtx;					// モデルの頂点数
	DWORD dwSizeFVF;				// モデルの頂点フォーマットのサイズ
	BYTE* pVtxBuff;					// モデルの頂点バッファへのポインタ
	bool bLand = false;				// 着地判定
	D3DXVECTOR3* aVtxNo;			// 頂点座標

	if (pos->x <= m_pos.x + m_XFileData.vtxMax.x &&
		pos->x >= m_pos.x + m_XFileData.vtxMin.x &&
		pos->z <= m_pos.z + m_XFileData.vtxMax.z &&
		pos->z >= m_pos.z + m_XFileData.vtxMin.z)
	{ // モデルの範囲にいる場合

		// モデルの頂点数を取得
		nNumVtx = m_XFileData.pMesh->GetNumVertices();

		// 頂点座標のメモリを確保する
		aVtxNo = new D3DXVECTOR3[nNumVtx];

		// モデルの頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(m_XFileData.pMesh->GetFVF());

		// モデルの頂点バッファをロック
		m_XFileData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // モデルの頂点数分繰り返す

			// モデルの頂点座標を代入
			aVtxNo[nCntVtx] = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += dwSizeFVF;
		}

		// モデルの頂点バッファをアンロック
		m_XFileData.pMesh->UnlockVertexBuffer();

		D3DXVECTOR3 vtxPos[3];		// 頂点座標
		WORD dwIdx[3];				// 頂点のインデックス
		WORD* pIndex;				// インデックスへのポインタ

		// インデックスバッファをロックする
		m_XFileData.pMesh->LockIndexBuffer(0, (void**)&pIndex);

		for (int nCnt = 0; nCnt < m_XFileData.nNumFace; nCnt++)
		{
			for (int nCntIndex = 0; nCntIndex < 3; nCntIndex++)
			{
				// インデックスを取得する
				dwIdx[nCntIndex] = pIndex[(nCnt * 3) + nCntIndex];

				// 頂点座標を設定する
				vtxPos[nCntIndex] = m_XFileData.vtxPos[dwIdx[nCntIndex]];

				if (useful::LineOuterProductXZ(m_pos + vtxPos[0], m_pos + vtxPos[1], *pos) >= 0 &&
					useful::LineOuterProductXZ(m_pos + vtxPos[1], m_pos + vtxPos[2], *pos) >= 0 &&
					useful::LineOuterProductXZ(m_pos + vtxPos[2], m_pos + vtxPos[0], *pos) >= 0)
				{ // 真ん中の境界線より左側に居た場合

					// 法線の計算(正規化)
					//NormalizeVector(nor, pVtx[nVtxRightDown].pos, pVtx[nVtxLeftUp].pos, pVtx[nVtxLeftDown].pos);

					vec1 = (m_pos + vtxPos[1]) - (m_pos + vtxPos[0]);
					vec2 = (m_pos + vtxPos[2]) - (m_pos + vtxPos[0]);

					D3DXVec3Cross(&nor, &vec1, &vec2);

					D3DXVec3Normalize(&nor, &nor);

					if (nor.y != 0.0f)
					{ // 法線のYが0.0f以外の場合

						// 高さを設定する
						pos->y = (((pos->x - (m_pos.x + vtxPos[0].x)) * nor.x + (-(m_pos.y + vtxPos[0].y)) * nor.y + (pos->z - (m_pos.z + vtxPos[0].z)) * nor.z) * -1.0f) / nor.y;

						// 着地判定を true にする
						bLand = true;
					}
				}
			}
		}

		// 頂点バッファをアンロックする
		m_XFileData.pMesh->UnlockVertexBuffer();

		// 頂点座標のメモリを解放する
		delete[] aVtxNo;
	}

	// 着地判定を返す
	return bLand;
}

//========================
// Xファイルのデータの取得処理
//========================
CXFile::SXFile CModel::GetFileData(void)
{
	// Xファイルのデータを返す
	return m_XFileData;
}

//========================
// マテリアル情報の取得処理
//========================
D3DXMATERIAL CModel::GetMaterial(const int nCnt)
{
	// ポインタを宣言
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	if (nCnt < (int)(m_XFileData.dwNumMat))
	{ // 番号が最大数未満の場合

		// マテリアルを返す
		return pMat[nCnt];
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// 予備のマテリアルを返す
		return pMat[0];
	}
}

//========================
// テクスチャの割り当て処理
//========================
void CModel::BindTexture(const int nIdx, const int nCnt)
{
	if (nCnt < (int)(m_XFileData.dwNumMat))
	{ // 番号が最大数未満の場合

		// テクスチャの番号を設定する
		m_XFileData.m_nTexIdx[nCnt] = nIdx;
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//========================
// 位置設定処理
//========================
void CModel::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//========================
// 位置取得処理
//========================
D3DXVECTOR3 CModel::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//========================
// 前回の位置設定処理
//========================
void CModel::SetPosOld(const D3DXVECTOR3& posOld)
{
	// 前回の位置を設定する
	m_posOld = posOld;
}

//========================
// 前回の位置取得処理
//========================
D3DXVECTOR3 CModel::GetPosOld(void) const
{
	// 前回の位置を返す
	return m_posOld;
}

//========================
// 向き設定処理
//========================
void CModel::SetRot(const D3DXVECTOR3& rot)
{
	// 向きを設定する
	m_rot = rot;
}

//========================
// 向き取得処理
//========================
D3DXVECTOR3 CModel::GetRot(void) const
{
	// 向きを設定する
	return m_rot;
}

//========================
// サイズ設定処理
//========================
void CModel::SetScale(const D3DXVECTOR3& scale)
{
	// 拡大率を設定する
	m_scale = scale;
}

//========================
// サイズ取得処理
//========================
D3DXVECTOR3 CModel::GetScale(void) const
{
	// 拡大率を返す
	return m_scale;
}

//========================
// マトリックスの取得処理
//========================
D3DXMATRIX CModel::GetMatrix(void) const
{
	// マトリックスの情報を返す
	return m_mtxWorld;
}

//========================
// マトリックスポインタの取得処理
//========================
D3DXMATRIX* CModel::GetMatrixPoint(void)
{
	// マトリックスのポインタを返す
	return &m_mtxWorld;
}

//========================
// データの設定処理(ファイルデータの設定版)
//========================
void CModel::SetFileData(const CXFile::SXFile filaData)
{
	// ファイルのデータを代入する
	m_XFileData = filaData;
}

//========================
// 生成処理
//========================
CModel* CModel::Create(const TYPE type, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CModel* pModel = nullptr;	// プレイヤーのインスタンスを生成

	if (pModel == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pModel = new CModel(type, priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pModel != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pModel->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// モデルのポインタを返す
	return pModel;
}