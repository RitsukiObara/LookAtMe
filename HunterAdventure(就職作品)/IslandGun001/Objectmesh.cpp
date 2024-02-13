//======================================================================================================================
//
//	メッシュ処理 [mesh.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Objectmesh.h"
#include "useful.h"
#include "texture.h"

#include "mesh_field.h"
#include "mesh_wall.h"
#include "mesh_cylinder.h"
#include "mesh_dome.h"
#include "mesh_donut.h"
#include "mesh_tornado.h"
#include "mesh_sphere.h"

//----------------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------------
namespace
{
	const char* MESH_TXT = "data/TXT/Mesh.txt";			// メッシュのテキスト
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CMesh*> CMesh::m_list = {};		// リスト

//================================
// オーバーロードコンストラクタ
//================================
CMesh::CMesh(CObject::TYPE type, CObject::PRIORITY priority) : CObject(type, DIM_3D, priority)
{
	// 全ての値をクリアする
	m_pos = NONE_D3DXVECTOR3;	// 位置
	m_rot = NONE_D3DXVECTOR3;	// 向き
	m_pVtxBuff = nullptr;		// 頂点バッファのポインタ
	m_pIdxBuff = nullptr;		// インデックスバッファへのポインタ
	m_type = TYPE_FIELD;		// 種類
	m_vtx.nWidth = 0;			// 横幅の頂点数
	m_vtx.nHeight = 0;			// 縦幅の頂点数
	m_divi.nWidth = 0;			// 横幅の分割数
	m_divi.nHeight = 0;			// 縦幅の分割数
	m_nNumVtx = 0;				// 総頂点数
	m_nNumIdx = 0;				// 総インデックス数
	m_nTexIdx = NONE_TEXIDX;	// テクスチャのインデックス
	m_bLightOff = false;		// ライティング状況
	m_bCullOff = false;			// カリング状況
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ワールドマトリックス

	// リストに追加する
	m_list.Regist(this);
}

//================================
// デストラクタ
//================================
CMesh::~CMesh()
{

}

//================================
// 初期化処理
//================================
HRESULT CMesh::Init(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * m_nNumVtx,	// 必要頂点数
		D3DUSAGE_WRITEONLY,				// 使用方法
		FVF_VERTEX_3D,					// 頂点フォーマット
		D3DPOOL_MANAGED,				// メモリの指定
		&m_pVtxBuff,					// 頂点バッファへのポインタ
		NULL
	)))
	{ // 頂点の生成に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer
	( // 引数
		sizeof(WORD) * m_nNumIdx,		// 必要インデックス数
		D3DUSAGE_WRITEONLY,				// 使用方法
		D3DFMT_INDEX16,					// インデックスバッファのフォーマット
		D3DPOOL_MANAGED,				// メモリの指定
		&m_pIdxBuff,					// インデックスバッファへのポインタ
		NULL
	)))
	{ // インデックスの生成に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CMesh::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{ // 変数 (g_pVtxBuffMeshField) がNULLではない場合

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{ // 変数 (g_pIdxBuffMeshField) がNULLではない場合

		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	// 破棄処理
	Release();

	// 引き抜き処理
	m_list.Pull(this);
}

//================================
// 更新処理
//================================
void CMesh::Update(void)
{

}

//================================
// 描画処理
//================================
void CMesh::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	if (m_bLightOff == true)
	{ // ライティングOFFの場合

		// ライティングをOFFにする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	if (m_bCullOff == true)
	{ // カリングOFFの場合

		// カリングの設定をOFFにする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	// 描画処理の中身
	DrawProcess();

	if (m_bCullOff == true)
	{ // カリングOFFの場合

		// カリングの設定をONにする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	if (m_bLightOff == true)
	{ // ライティングOFFの場合

		// ライティングをONにする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//================================
// 描画処理の中身
//================================
void CMesh::DrawProcess(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,					// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));			// 頂点情報構造体のサイズ

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,													// 描画する最初の頂点インデックス
		0,
		m_nNumVtx,											// 用意した頂点の数
		0,
		m_nNumIdx - 2);										// 描画するプリミティブ数
}

//================================
// テクスチャの割り当て処理
//================================
void CMesh::BindTexture(int nIdx)
{
	// テクスチャを割り当てる
	m_nTexIdx = nIdx;
}

//================================
// 位置の設定処理
//================================
void CMesh::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//================================
// 位置の取得処理
//================================
D3DXVECTOR3 CMesh::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//================================
// 向きの設定処理
//================================
void CMesh::SetRot(const D3DXVECTOR3& rot)
{
	// 向きを設定する
	m_rot = rot;
}

//================================
// 向きの取得処理
//================================
D3DXVECTOR3 CMesh::GetRot(void) const
{
	// 向きを返す
	return m_rot;
}

//================================
// 頂点バッファのポインタの取得処理
//================================
LPDIRECT3DVERTEXBUFFER9 CMesh::GetVtxBuff(void) const
{
	// 頂点バッファのポインタを返す
	return m_pVtxBuff;
}

//================================
// インデックスバッファのポインタの取得処理
//================================
LPDIRECT3DINDEXBUFFER9 CMesh::GetIdxBuff(void) const
{
	// インデックスバッファのポインタを返す
	return m_pIdxBuff;
}

//================================
// 種類の設定処理
//================================
void CMesh::SetType(const TYPE type)
{
	// 種類を設定する
	m_type = type;
}

//================================
// 種類の取得処理
//================================
CMesh::TYPE CMesh::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//================================
// 頂点数の設定処理
//================================
void CMesh::SetVtx(const SGrid vtx)
{
	// 頂点数を設定する
	m_vtx = vtx;
}

//================================
// 頂点数の取得処理
//================================
CMesh::SGrid CMesh::GetVtx(void) const
{
	// 頂点数を返す
	return m_vtx;
}

//================================
// 分割数の設定処理
//================================
void CMesh::SetDivi(const SGrid divi)
{
	// 分割数を設定する
	m_divi = divi;
}

//================================
// 分割数の取得処理
//================================
CMesh::SGrid CMesh::GetDivi(void) const
{
	// 分割数を返す
	return m_divi;
}

//================================
// 総頂点数の設定処理
//================================
void CMesh::SetNumVtx(void)
{
	// 総頂点数を設定する
	m_nNumVtx = m_vtx.nWidth * m_vtx.nHeight;
}

//================================
// 総頂点数の取得処理
//================================
int CMesh::GetNumVtx(void)
{
	// 総頂点数を返す
	return m_nNumVtx;
}

//================================
// 総インデックス数の設定処理
//================================
void CMesh::SetNumIdx(void)
{	
	// 総インデックス数を設定する
	m_nNumIdx = (2 * (m_vtx.nWidth * m_divi.nHeight)) + ((m_vtx.nHeight - 2) * 2);			
}

//================================
// 総インデックス数の取得処理
//================================
int CMesh::GetNumIdx(void)
{
	// 総インデックス数を返す
	return m_nNumIdx;
}

//================================
// ライティングの設定処理
//================================
void CMesh::SetLighting(const bool bLighting)
{
	// ライティング状況を設定する
	m_bLightOff = bLighting;
}

//================================
// カリングの設定処理
//================================
void CMesh::SetCulling(const bool bCulling)
{
	// カリング状況を設定する
	m_bCullOff = bCulling;
}

/*
* @brief インデックスの設定処理
* @param nNumVtx1 [in] 周回の頂点数
* @param nNumVtx2 [in] 1層ごとの頂点数
*/
void CMesh::SetIndex(const int nNumVtx1, const int nNumVtx2)
{
	// ローカル変数宣言
	WORD *pIdx;			// インデックス情報へのポインタ
	int nIdx = 0;		// インデックス数

	//------------------------------------------------------------------------------------------------------------------
	//	インデックス情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntDep = 1; nCntDep < nNumVtx1; nCntDep++)
	{
		if (nCntDep != 1)
		{ // 最初以外

			// インデックス数を設定する
			pIdx[0] = (WORD)(nNumVtx2 * nCntDep);

			// インデックスデータを加算する
			pIdx++;
			nIdx++;
		}

		for (int nCntWid = 0; nCntWid < nNumVtx2; nCntWid++)
		{ // 1層ごとに設定する

			// インデックス数を設定する
			pIdx[0] = (WORD)((nNumVtx2 * nCntDep) + nCntWid);
			pIdx[1] = (WORD)((nNumVtx2 * nCntDep) - (nNumVtx2 - nCntWid));

			// インデックスデータを2つ進める
			pIdx += 2;
			nIdx += 2;

			if (nCntWid == (nNumVtx2 - 1) && nCntDep != (nNumVtx1 - 1))
			{ // 折り返しに入った場合

				// インデックス数を設定する
				pIdx[0] = (WORD)((nNumVtx2 * nCntDep) - (nNumVtx2 - nCntWid));

				// インデックスデータを加算する
				pIdx++;
				nIdx++;
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//================================
// 頂点カラーの設定処理
//================================
void CMesh::SetColor(const D3DXCOLOR& col)
{
	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCol = 0; nCntCol < m_nNumVtx; nCntCol++)
	{
		//頂点座標の設定
		pVtx[0].col = col;

		pVtx++;			// 頂点データを進める
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================
// テキスト読み込み処理
//================================
void CMesh::TxtSet(void)
{
	// 変数を宣言
	int nEnd;						// テキスト読み込み終了の確認用
	char aString[MAX_STRING];		// テキストの文字列の代入用

	// ポインタを宣言
	FILE  *pFile;							// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(MESH_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (nEnd == EOF)
			{ // 終了条件が一致した場合

				// while文を抜け出す
				break;
			}

			if (strcmp(&aString[0], "SET_MESHFIELD") == 0)
			{ // 読み込んだ文字列が SET_MESHFIELD の場合

				// フィールドの読み込み処理
				nEnd = TxtField(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHWALL") == 0)
			{ // 読み込んだ文字列が SET_MESHWALL の場合

				// ウォールの読み込み処理
				nEnd = TxtWall(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHCYLINDER") == 0)
			{ // 読み込んだ文字列が SET_MESHCYLINDER の場合

				// シリンダーの読み込み処理
				nEnd = TxtCylinder(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHDOME") == 0)
			{ // 読み込んだ文字列が SET_MESHDOME の場合

				// ドームの読み込み処理
				nEnd = TxtDome(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHDONUT") == 0)
			{ // 読み込んだ文字列が SET_MESH_DONUT の場合

				// ドーナツの読み込み処理
				nEnd = TxtDonut(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHTORNADO") == 0)
			{ // 読み込んだ文字列が SET_MESHTORNADO の場合

				// トルネードの読み込み処理
				nEnd = TxtTornado(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHSPHERE") == 0)
			{ // 読み込んだ文字列が SET_MESHSPHERE の場合

				// スフィアの読み込み処理
				nEnd = TxtSphere(pFile);
			}

		} while (nEnd != EOF);														// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);
	}
}

//================================
// フィールドの読み込み処理
//================================
int CMesh::TxtField(FILE* pFile)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// 位置
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// 向き
	float fWidth = 0.0f;				// 幅
	float fDepth = 0.0f;				// 奥行
	SGrid divi = { 0,0 };				// 分割数
	bool bTex = false;					// テクスチャ状況
	bool bLighting = false;				// ライティング状況
	bool bCulling = false;				// カリング状況
	CMeshField* pField = nullptr;		// フィールドのポインタ

	// 変数配列を宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャのパス名

	do
	{ // 読み込んだ文字列が END_SET_MESHFIELD ではない場合ループ

		// ファイルから文字列を読み込む
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // 読み込んだ文字列が ROT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // 読み込んだ文字列が SIZE の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f", &fWidth, &fDepth);			// 大きさを読み込む
		}
		else if (strcmp(&aString[0], "WIDTH") == 0)
		{ // 読み込んだ文字列が WIDTH の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nWidth);					// 横幅の分割数を読み込む
		}
		else if (strcmp(&aString[0], "DEPTH") == 0)
		{ // 読み込んだ文字列が DEPTH の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nHeight);					// 奥幅の分割数を読み込む
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // 読み込んだ文字列が TEXTURE の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む

			// テクスチャ状況を true にする
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // 読み込んだ文字列が LIGHTING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// ライティング状況を読み込む

			// ライティング状況を設定する
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // 読み込んだ文字列が CULLING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// カリング状況を読み込む

			// カリング状況を設定する
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHFIELD") != 0);	// 読み込んだ文字列が END_SET_MESHFIELD ではない場合ループ

	// フィールドの生成処理
	pField = CMeshField::Create(pos, rot, fWidth, fDepth, divi, PRIORITY_BG);

	if (bTex == true)
	{ // テクスチャを使う場合

		// テクスチャの割り当て処理
		pField->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pField->SetLighting(bLighting);		// ライティング状況
	pField->SetCulling(bCulling);		// カリング状況

	// 終了状況を返す
	return nEnd;
}

//================================
// ウォールの読み込み処理
//================================
int CMesh::TxtWall(FILE* pFile)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// 位置
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// 向き
	float fWidth = 0.0f;				// 幅
	float fHeight = 0.0f;				// 高さ
	SGrid divi = { 0,0 };				// 分割数
	bool bTex = false;					// テクスチャ状況
	bool bLighting = false;				// ライティング状況
	bool bCulling = false;				// カリング状況
	CMeshWall* pWall = nullptr;			// ウォールのポインタ

	// 変数配列を宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャのパス名

	do
	{ // 読み込んだ文字列が END_SET_MESHWALL ではない場合ループ

		// ファイルから文字列を読み込む
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // 読み込んだ文字列が ROT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // 読み込んだ文字列が SIZE の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f", &fWidth, &fHeight);			// 大きさを読み込む
		}
		else if (strcmp(&aString[0], "WIDTH") == 0)
		{ // 読み込んだ文字列が WIDTH の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nWidth);					// 横幅の分割数を読み込む
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // 読み込んだ文字列が HEIGHT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nHeight);					// 縦幅の分割数を読み込む
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // 読み込んだ文字列が TEXTURE の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む

			// テクスチャ状況を true にする
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // 読み込んだ文字列が LIGHTING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

			// ライティング状況を設定する
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // 読み込んだ文字列が CULLING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// カリング状況を読み込む

			// カリング状況を設定する
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHWALL") != 0);	// 読み込んだ文字列が END_SET_MESHWALL ではない場合ループ

	// メッシュウォールの生成処理
	pWall = CMeshWall::Create(pos, rot, fWidth, fHeight, divi, PRIORITY_BG);

	if (bTex == true)
	{ // テクスチャを使う場合

		// テクスチャの割り当て処理
		pWall->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pWall->SetLighting(bLighting);		// ライティング状況
	pWall->SetCulling(bCulling);		// カリング状況

	// 終了状況を返す
	return nEnd;
}

//================================
// シリンダーの読み込み処理
//================================
int CMesh::TxtCylinder(FILE* pFile)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// 位置
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// 向き
	float fCircum = 0.0f;				// 円周
	float fHeight = 0.0f;				// 高さ
	SGrid divi = { 0,0 };				// 分割数
	bool bTex = false;					// テクスチャ状況
	bool bLighting = false;				// ライティング状況
	bool bCulling = false;				// カリング状況
	CMeshCylinder* pCylinder = nullptr;	// シリンダーのポインタ

	// 変数配列を宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャのパス名

	do
	{ // 読み込んだ文字列が END_SET_MESHCYLINDER ではない場合ループ

		// ファイルから文字列を読み込む
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // 読み込んだ文字列が ROT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // 読み込んだ文字列が SIZE の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f", &fCircum, &fHeight);			// サイズを読み込む
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // 読み込んだ文字列が CIRCUM の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nWidth);					// 円周の分割数を読み込む
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // 読み込んだ文字列が HEIGHT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nHeight);					// 高さの分割数を読み込む
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // 読み込んだ文字列が TEXTURE の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む

			// テクスチャ状況を true にする
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // 読み込んだ文字列が LIGHTING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

			// ライティング状況を設定する
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // 読み込んだ文字列が CULLING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// カリング状況を読み込む

			// カリング状況を設定する
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHCYLINDER") != 0);	// 読み込んだ文字列が END_SET_MESHCYLINDER ではない場合ループ

	// 筒の生成処理
	pCylinder = CMeshCylinder::Create(pos, rot, fCircum, fHeight, divi, PRIORITY_BG);

	if (bTex == true)
	{ // テクスチャを使う場合

		// テクスチャの割り当て処理
		pCylinder->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pCylinder->SetLighting(bLighting);		// ライティング状況
	pCylinder->SetCulling(bCulling);		// カリング状況

	// 終了状況を返す
	return nEnd;
}

//================================
// ドームの読み込み処理
//================================
int CMesh::TxtDome(FILE* pFile)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// 位置
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// 向き
	float fCircum = 0.0f;				// 円周
	float fHeight = 0.0f;				// 高さ
	SGrid divi = { 0,0 };				// 分割数
	bool bTex = false;					// テクスチャ状況
	bool bLighting = false;				// ライティング状況
	bool bCulling = false;				// カリング状況
	CMeshDome* pDome = nullptr;			// ドームのポインタ

	// 変数配列を宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャのパス名

	do
	{ // 読み込んだ文字列が END_SET_MESHDOME ではない場合ループ

		// ファイルから文字列を読み込む
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // 読み込んだ文字列が ROT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // 読み込んだ文字列が SIZE の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f", &fCircum, &fHeight);			// サイズを読み込む
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // 読み込んだ文字列が CIRCUM の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nWidth);					// 円周の分割数を読み込む
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // 読み込んだ文字列が HEIGHT の場合0
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nHeight);					// 高さの分割数を読み込む
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // 読み込んだ文字列が TEXTURE の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む

			// テクスチャ状況を true にする
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // 読み込んだ文字列が LIGHTING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

			// ライティング状況を設定する
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // 読み込んだ文字列が CULLING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// カリング状況を読み込む

			// カリング状況を設定する
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHDOME") != 0);	// 読み込んだ文字列が END_SET_MESHDOME ではない場合ループ

	// ドームの生成処理
	pDome = CMeshDome::Create(pos, rot, fCircum, fHeight, divi, PRIORITY_BG);

	if (bTex == true)
	{ // テクスチャを使う場合

		// テクスチャの割り当て処理
		pDome->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pDome->SetLighting(bLighting);		// ライティング状況
	pDome->SetCulling(bCulling);		// カリング状況

	// 終了状況を返す
	return nEnd;
}

//================================
// ドーナツの読み込み処理
//================================
int CMesh::TxtDonut(FILE* pFile)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// 位置
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// 向き
	float fWidth = 0.0f;				// 幅
	float fCircum = 0.0f;				// 円周
	SGrid divi = { 0,0 };				// 分割数
	bool bTex = false;					// テクスチャ状況
	bool bLighting = false;				// ライティング状況
	bool bCulling = false;				// カリング状況
	CMeshDonut* pDonut = nullptr;		// ドーナツのポインタ

	// 変数配列を宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャのパス名

	do
	{ // 読み込んだ文字列が END_SET_MESHDONUT ではない場合ループ

		// ファイルから文字列を読み込む
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // 読み込んだ文字列が ROT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // 読み込んだ文字列が SIZE の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f", &fWidth, &fCircum);			// サイズを読み込む
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // 読み込んだ文字列が CIRCUM の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nHeight);					// 円周の分割数を読み込む
		}
		else if (strcmp(&aString[0], "WIDTH") == 0)
		{ // 読み込んだ文字列が HEIGHT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nWidth);					// 高さの分割数を読み込む
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // 読み込んだ文字列が TEXTURE の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む

			// テクスチャ状況を true にする
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // 読み込んだ文字列が LIGHTING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

			// ライティング状況を設定する
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // 読み込んだ文字列が CULLING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// カリング状況を読み込む

			// カリング状況を設定する
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHDONUT") != 0);	// 読み込んだ文字列が END_SET_MESHDONUT ではない場合ループ

	// ドーナツの生成処理
	pDonut = CMeshDonut::Create(pos, rot, fWidth, fCircum, divi, PRIORITY_BG);

	if (bTex == true)
	{ // テクスチャを使う場合

		// テクスチャの割り当て処理
		pDonut->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pDonut->SetLighting(bLighting);		// ライティング状況
	pDonut->SetCulling(bCulling);		// カリング状況

	// 終了状況を返す
	return nEnd;
}

//================================
// トルネードの読み込み処理
//================================
int CMesh::TxtTornado(FILE* pFile)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// 位置
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// 向き
	float fWidth = 0.0f;				// 幅
	float fHeight = 0.0f;				// 高さ
	float fShift = 0.0f;				// ずらす幅
	float fCircum = 0.0f;				// 円周
	int nVortex = 0;					// 渦
	SGrid divi = { 0,0 };				// 分割数
	bool bTex = false;					// テクスチャ状況
	bool bLighting = false;				// ライティング状況
	bool bCulling = false;				// カリング状況
	CMeshTornado* pTornado = nullptr;	// トルネードのポインタ

	// 変数配列を宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャのパス名

	do
	{ // 読み込んだ文字列が END_SET_MESHTORNADO ではない場合ループ

		// ファイルから文字列を読み込む
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // 読み込んだ文字列が ROT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // 読み込んだ文字列が SIZE の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f", &fWidth, &fHeight);			// サイズを読み込む
		}
		else if (strcmp(&aString[0], "SHIFT") == 0)
		{ // 読み込んだ文字列が SHIFT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f", &fShift);						// ずらす幅を読み込む
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // 読み込んだ文字列が CIRCUM の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f", &fCircum);						// 円周を読み込む
		}
		else if (strcmp(&aString[0], "WIDTH") == 0)
		{ // 読み込んだ文字列が WIDTH の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nWidth);					// 幅の分割数を読み込む
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // 読み込んだ文字列が HEIGHT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nHeight);					// 高さの分割数を読み込む
		}
		else if (strcmp(&aString[0], "VORTEX") == 0)
		{ // 読み込んだ文字列が VORTEX の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &nVortex);						// 渦の数を読み込む
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // 読み込んだ文字列が TEXTURE の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む

			// テクスチャ状況を true にする
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // 読み込んだ文字列が LIGHTING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

			// ライティング状況を設定する
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // 読み込んだ文字列が CULLING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// カリング状況を読み込む

			// カリング状況を設定する
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHTORNADO") != 0);	// 読み込んだ文字列が END_SET_MESHTORNADO ではない場合ループ

	// トルネードの生成処理
	pTornado = CMeshTornado::Create(pos, rot, fWidth, fHeight, nVortex, fShift, fCircum, divi, PRIORITY_BG);

	if (bTex == true)
	{ // テクスチャを使う場合

		// テクスチャの割り当て処理
		pTornado->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pTornado->SetLighting(bLighting);		// ライティング状況
	pTornado->SetCulling(bCulling);		// カリング状況

	// 終了状況を返す
	return nEnd;
}

//================================
// スフィアの読み込み処理
//================================
int CMesh::TxtSphere(FILE* pFile)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// 位置
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// 向き
	float fCircum = 0.0f;				// 円周
	float fHeight = 0.0f;				// 高さ
	SGrid divi = { 0,0 };				// 分割数
	bool bTex = false;					// テクスチャ状況
	bool bLighting = false;				// ライティング状況
	bool bCulling = false;				// カリング状況
	CMeshSphere* pSphere = nullptr;		// スフィアのポインタ

	// 変数配列を宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャのパス名

	do
	{ // 読み込んだ文字列が END_SET_MESHSPHERE ではない場合ループ

		// ファイルから文字列を読み込む
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // 読み込んだ文字列が ROT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // 読み込んだ文字列が SIZE の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%f%f", &fCircum, &fHeight);			// サイズを読み込む
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // 読み込んだ文字列が CIRCUM の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nWidth);					// 円周の分割数を読み込む
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // 読み込んだ文字列が HEIGHT の場合
			fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
			fscanf(pFile, "%d", &divi.nHeight);					// 高さの分割数を読み込む
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // 読み込んだ文字列が TEXTURE の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む

			// テクスチャ状況を true にする
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // 読み込んだ文字列が LIGHTING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

			// ライティング状況を設定する
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // 読み込んだ文字列が CULLING の場合
			fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);				// カリング状況を読み込む

			// カリング状況を設定する
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHSPHERE") != 0);	// 読み込んだ文字列が END_SET_MESHSPHERE ではない場合ループ

	// スフィアの生成処理
	pSphere = CMeshSphere::Create(pos, rot, fCircum, fHeight, divi, PRIORITY_BG);

	if (bTex == true)
	{ // テクスチャを使う場合

		// テクスチャの割り当て処理
		pSphere->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pSphere->SetLighting(bLighting);		// ライティング状況
	pSphere->SetCulling(bCulling);		// カリング状況

	// 終了状況を返す
	return nEnd;
}

//================================
// リストの取得処理
//================================
CListManager<CMesh*> CMesh::GetList(void)
{
	// リストマネージャーを返す
	return m_list;
}