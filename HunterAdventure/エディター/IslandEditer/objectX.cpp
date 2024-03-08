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
#include "objectX.h"
#include "texture.h"

//------------------------------------------------------------
// 無名名前空間
//------------------------------------------------------------
namespace
{
	const char* LOAD_TXT = "data/TXT/Model.txt";		// モデルの情報テキスト
}

//============================================================
// コンストラクタ
//============================================================
CXFile::CXFile()
{
	for (int nCnt = 0; nCnt < MAX_XFILE; nCnt++)
	{
		// モデルの情報を初期化する
		m_apModel[nCnt].pMesh = nullptr;			// メッシュ (頂点情報) へのポインタ
		m_apModel[nCnt].pBuffMat = nullptr;			// マテリアルへのポインタ
		m_apModel[nCnt].dwNumMat = 0;				// マテリアルの数
		m_apModel[nCnt].vtxMin = INIT_VTX_MIN;		// 最小の頂点座標
		m_apModel[nCnt].vtxMax = INIT_VTX_MAX;		// 最大の頂点座標
		m_apModel[nCnt].collsize = INIT_SIZE;		// 当たり判定のサイズ
		m_apModel[nCnt].vtxPos = nullptr;			// 頂点の位置
		m_apModel[nCnt].fRadius = 0.0f;				// 半径
		m_apModel[nCnt].nNumFace = 0;				// 面の数
		m_apModel[nCnt].bEmpty = true;				// 空白状況

		for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
		{
			m_apModel[nCnt].m_nTexIdx[nCntMat] = NONE_TEXIDX;		// テクスチャのインデックス
		}

		*m_apModelName[nCnt] = {};					// モデルの名前
	}
}

//============================================================
// デストラクタ
//============================================================
CXFile::~CXFile()
{

}

//============================================================
// モデルの初期化処理
//============================================================
HRESULT CXFile::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_XFILE; nCnt++)
	{
		// モデルの情報を初期化する
		m_apModel[nCnt].pMesh = nullptr;			// メッシュ (頂点情報) へのポインタ
		m_apModel[nCnt].pBuffMat = nullptr;			// マテリアルへのポインタ
		m_apModel[nCnt].dwNumMat = 0;				// マテリアルの数
		m_apModel[nCnt].vtxMin = INIT_VTX_MIN;		// 最小の頂点座標
		m_apModel[nCnt].vtxMax = INIT_VTX_MAX;		// 最大の頂点座標
		m_apModel[nCnt].collsize = INIT_SIZE;		// 当たり判定のサイズ
		m_apModel[nCnt].vtxPos = nullptr;			// 頂点の位置
		m_apModel[nCnt].fRadius = 0.0f;				// 半径
		m_apModel[nCnt].nNumFace = 0;				// 面の数
		m_apModel[nCnt].bEmpty = true;				// 空白状況

		for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
		{
			m_apModel[nCnt].m_nTexIdx[nCntMat] = NONE_TEXIDX;		// テクスチャのインデックス
		}

		*m_apModelName[nCnt] = {};					// モデルの名前
	}

	// 成功を返す
	return S_OK;
}

//============================================================
// モデルの終了処理
//============================================================
void CXFile::Unload(void)
{
	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < MAX_XFILE; nCntModel++)
	{ // モデルの最大数分繰り返す

		if (m_apModel[nCntModel].pMesh != nullptr)
		{ // 変数 (m_apModel[nCntModel].pMesh) が NULL じゃない場合

			m_apModel[nCntModel].pMesh->Release();
			m_apModel[nCntModel].pMesh = nullptr;
		}

		if (m_apModel[nCntModel].pBuffMat != nullptr)
		{ // 変数 (m_apModel[nCntModel].pBuffMat) が NULL じゃない場合

			m_apModel[nCntModel].pBuffMat->Release();
			m_apModel[nCntModel].pBuffMat = nullptr;
		}

		if (m_apModel[nCntModel].vtxPos != nullptr)
		{ // 頂点座標 (m_apModel[nCntModel].vtxPos) が NULL じゃない場合

			delete[] m_apModel[nCntModel].vtxPos;
			m_apModel[nCntModel].vtxPos = nullptr;
		}
	}

	// メモリを開放する
	delete this;
}

//============================================================
// Xファイルの登録処理
//============================================================
CXFile::SXFile CXFile::Regist(const char* filename)
{
	// ローカル変数宣言
	SXFile XFile;		// 返り値用変数

	for (int nCntModel = 0; nCntModel < MAX_TEXTURE; nCntModel++)
	{
		if (m_apModel[nCntModel].bEmpty == true)
		{ // 中に情報が入っていない場合

			// テクスチャの読み込み
			if (FAILED(LoadXFile(filename,&m_apModel[nCntModel])))
			{ // テクスチャの生成に失敗した場合

				// 停止
				assert(false);
			}

			// 当たり判定の作成
			SetCollision(&m_apModel[nCntModel]);

			// テクスチャの読み込み
			if (FAILED(LoadTexture(&m_apModel[nCntModel])))
			{ // テクスチャの読み込みに失敗した場合

				// 停止
				assert(false);
			}

			// パスを登録する
			strcpy(&m_apModelName[nCntModel][0], filename);

			// 空白状況を false にする
			m_apModel[nCntModel].bEmpty = false;

			// モデルを代入する
			XFile = m_apModel[nCntModel];

			// 抜け出す
			break;
		}
		else
		{ // ポインタが NULL じゃない場合

			if (strcmp(&m_apModelName[nCntModel][0], filename) == 0)
			{ // 過去に読み込んでいた場合

				// モデルを代入する
				XFile = m_apModel[nCntModel];

				// 抜け出す
				break;
			}
		}
	}

	// 結果のモデルを返す
	return XFile;
}

//============================================================
// ロード処理
//============================================================
HRESULT CXFile::Load(void)
{
	// ローカル変数宣言
	int nEnd;						// テキスト読み込み終了の確認用
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aModelName[MAX_STRING];	// テクスチャの名前(相対パス)

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(LOAD_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{ // 読み込んだ文字列が MODEL_FILENAME の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%s", &aModelName[0]);		// モデルの相対パスを読み込む

				// テクスチャの登録処理
				Regist(&aModelName[0]);
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	xファイルの読み込み
//============================================================
HRESULT CXFile::LoadXFile(const char* filename, SXFile* pXFile)
{
	// 変数を宣言
	HRESULT hr;		// 異常終了の確認用

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// xファイルの読み込み
	hr = D3DXLoadMeshFromX
	( // 引数
		filename,				// モデルの相対パス
		D3DXMESH_SYSTEMMEM,		// メッシュ作成用オプション
		pDevice,				// デバイスへのポインタ
		NULL,					// 隣接性データ
		&pXFile->pBuffMat,		// マテリアルへのポインタ
		NULL,					// エフェクトデータ
		&pXFile->dwNumMat,		// マテリアルの数
		&pXFile->pMesh			// メッシュ (頂点情報) へのポインタ
	);

	if (FAILED(hr))
	{ // xファイルの読み込みに失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	当たり判定の作成
//============================================================
void CXFile::SetCollision(SXFile* pXFile)
{
	// 変数を宣言
	int         nNumVtx;		// モデルの頂点数
	DWORD       dwSizeFVF;		// モデルの頂点フォーマットのサイズ
	BYTE* pVtxBuff;				// モデルの頂点バッファへのポインタ
	D3DXVECTOR3 vtx;			// モデルの頂点座標

	// モデルの頂点数を取得
	nNumVtx = pXFile->pMesh->GetNumVertices();

	// 頂点位置のメモリを確保する
	pXFile->vtxPos = new D3DXVECTOR3[nNumVtx];

	// モデルの面の数を取得する
	pXFile->nNumFace = pXFile->pMesh->GetNumFaces();

	// モデルの頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(pXFile->pMesh->GetFVF());

	// モデルの頂点バッファをロック
	pXFile->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{ // モデルの頂点数分繰り返す

		// モデルの頂点座標を代入
		vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点座標 (x) の設定
		if (vtx.x < pXFile->vtxMin.x)
		{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも小さい場合

			// 今回の頂点情報 (x) を代入
			pXFile->vtxMin.x = vtx.x;
		}
		else if (vtx.x > pXFile->vtxMax.x)
		{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも大きい場合

			// 今回の頂点情報 (x) を代入
			pXFile->vtxMax.x = vtx.x;
		}

		// 頂点座標 (y) の設定
		if (vtx.y < pXFile->vtxMin.y)
		{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも小さい場合

			// 今回の頂点情報 (y) を代入
			pXFile->vtxMin.y = vtx.y;
		}
		else if (vtx.y > pXFile->vtxMax.y)
		{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも大きい場合

			// 今回の頂点情報 (y) を代入
			pXFile->vtxMax.y = vtx.y;
		}

		// 頂点座標 (z) の設定
		if (vtx.z < pXFile->vtxMin.z)
		{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも小さい場合

			// 今回の頂点情報 (z) を代入
			pXFile->vtxMin.z = vtx.z;
		}
		else if (vtx.z > pXFile->vtxMax.z)
		{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも大きい場合

			// 今回の頂点情報 (z) を代入
			pXFile->vtxMax.z = vtx.z;
		}

		// 頂点の位置を設定する
		pXFile->vtxPos[nCntVtx] = vtx;

		// 頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	// モデルの頂点バッファをアンロック
	pXFile->pMesh->UnlockVertexBuffer();

	// モデルサイズを求める
	pXFile->collsize = pXFile->vtxMax - pXFile->vtxMin;

	// モデルの円の当たり判定を作成
	pXFile->fRadius = ((pXFile->collsize.x * 0.5f) + (pXFile->collsize.z * 0.5f)) * 0.5f;
}

//============================================================
//	テクスチャの読み込み
//============================================================
HRESULT CXFile::LoadTexture(SXFile* pXFile)
{
	// デバイスを取得する
	D3DXMATERIAL* pMat;						// マテリアルへのポインタ

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)pXFile->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pXFile->dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{ // テクスチャファイルが存在する場合

			// テクスチャの読み込み処理
			pXFile->m_nTexIdx[nCntMat] = CManager::Get()->GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
// 生成処理
//============================================================
CXFile* CXFile::Create(void)
{
	// ローカルオブジェクトを生成
	CXFile* pXFile = nullptr;		// Xファイルのインスタンスを生成

	if (pXFile == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pXFile = new CXFile;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pXFile != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pXFile->Init()))
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

	// Xファイルのポインタを返す
	return pXFile;
}