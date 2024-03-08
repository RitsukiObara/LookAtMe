//============================================================
//
// Xファイルヘッダー [xfile.h]
// Author：小原立暉
//
//============================================================
#ifndef _XFILE_H_	// このマクロ定義がされていない場合
#define _XFILE_H_	// 二重インクルード防止のマクロを定義する

//------------------------------------------------------------
// マクロ定義
//------------------------------------------------------------
#define INIT_VTX_MIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))		// モデルの最小の頂点座標の初期値
#define INIT_VTX_MAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))		// モデルの最大の頂点座標の初期値
#define INIT_SIZE		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))					// モデルサイズの初期値
#define MAX_XFILE		(1024)											// 読み込めるモデルの最大数

//------------------------------------------------------------
// クラス定義(Xファイル)
//------------------------------------------------------------
class CXFile
{
public:			// 誰でもアクセスできる

	//************************************************************
	//	構造体定義 (SXFile)
	//************************************************************
	struct SXFile
	{
		LPD3DXMESH	 pMesh;						// メッシュ (頂点情報) へのポインタ
		LPD3DXBUFFER pBuffMat;					// マテリアルへのポインタ
		DWORD		 dwNumMat;					// マテリアルの数
		D3DXVECTOR3	 vtxMin;					// 最小の頂点座標
		D3DXVECTOR3	 vtxMax;					// 最大の頂点座標
		D3DXVECTOR3	 collsize;					// 当たり判定のサイズ
		D3DXVECTOR3* vtxPos;					// 頂点の位置
		float		 fRadius;					// 半径
		int			 nNumFace;					// 面の数
		int			 m_nTexIdx[MAX_MATERIAL];	// テクスチャのインデックス
		bool		 bEmpty;					// 空白状況
	};

	CXFile();			// コンストラクタ
	~CXFile();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// Xファイルの初期化処理
	void Unload(void);		// Xファイルの終了処理

	SXFile Regist(const char* filename);		// Xファイルの登録処理
	HRESULT Load(void);		// ロード処理

	// 静的メンバ関数
	static CXFile* Create(void);		// 生成処理

private:

	// メンバ関数
	HRESULT LoadXFile(const char* filename, SXFile* pXFile);	// xファイルの読み込み
	void SetCollision(SXFile* pXFile);			// 当たり判定の作成
	HRESULT LoadTexture(SXFile* pXFile);		// テクスチャの読み込み

	// メンバ変数
	SXFile m_apModel[MAX_XFILE];						// モデルの情報
	char m_apModelName[MAX_XFILE][MAX_STRING];	// モデルの名前
};

#endif