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
#define INIT_BLOCK		(CXFile::TYPE_BLOCK_DIRT)						// ブロックの最初の数字
#define INIT_OBSTACLE	(CXFile::TYPE_HYDRANT)							// 障害物の最初の数字
#define INIT_PLAYER		(CXFile::TYPE_PLAYERPANTS)						// プレイヤーの最初の数字

//------------------------------------------------------------
// クラス定義(Xファイル)
//------------------------------------------------------------
class CXFile
{
public:			// 誰でもアクセスできる

	//************************************************************
	//	列挙型定義 (TYPE)
	//************************************************************
	typedef enum
	{
		// 普遍的モデル
		TYPE_ENERGYITEM = 0,			// アイテム
		TYPE_FRACTION,					// 破片
		TYPE_GOALUI,					// ゴールUI
		TYPE_RIPPLE,					// 着地の波紋
		TYPE_SHOTENEMY,					// 弾撃ち敵
		TYPE_RANK_SS,					// SSランク表示
		TYPE_RANK_S,					// Sランク表示
		TYPE_RANK_A,					// Aランク表示
		TYPE_RANK_B,					// Bランク表示
		TYPE_RANK_C,					// Cランク表示
		TYPE_ITEMGENERATOR,				// アイテム生成器
		TYPE_ARROWPOLE,					// 矢印看板の柱
		TYPE_ARROWSIGN,					// 矢印看板
	
		// ブロックモデル
		TYPE_BLOCK_DIRT,				// 地面ブロック
		TYPE_BLOCK_ROCK,				// 岩ブロック
		TYPE_BLOCK_ICE,					// 氷ブロック
		TYPE_BLOCK_MAGMA,				// マグマブロック

		// 障害物モデル
		TYPE_HYDRANT,					// 消火栓
		TYPE_SIGNPOLE,					// 標識
		TYPE_DRUM,						// ドラム缶
		TYPE_WOODBOX,					// 木箱
		TYPE_STREETLAMP,				// 街灯(ランプ)
		TYPE_STREETPOLE,				// 街灯(支柱)

		// プレイヤーモデル
		TYPE_PLAYERPANTS,				// 腰
		TYPE_PLAYERBODY,				// 体
		TYPE_PLAYERNECK,				// 首
		TYPE_PLAYERHEAD,				// 頭
		TYPE_PLAYERLEFTUPPER,			// 左上腕
		TYPE_PLAYERRIGHTUPPER,			// 右上腕
		TYPE_PLAYERLEFTARM,				// 左腕
		TYPE_PLAYERRIGHTARM,			// 右腕
		TYPE_PLAYERLEFTLEG,				// 左脚
		TYPE_PLAYERRIGHTLEG,			// 右脚
		TYPE_PLAYERLEFTSHIN,			// 左脛
		TYPE_PLAYERRIGHTSHIN,			// 右脛
		TYPE_PLAYERLEFTFOOT,			// 左足
		TYPE_PLAYERRIGHTFOOT,			// 右足

		// エディットモデル
		TYPE_EDIT_COLLISION,			// 当たり判定
		TYPE_MAX,						// 全モデルの総数
	} TYPE;

	//************************************************************
	//	構造体定義 (SXFile)
	//************************************************************
	typedef struct
	{
		LPD3DXMESH	 pMesh;						// メッシュ (頂点情報) へのポインタ
		LPD3DXBUFFER pBuffMat;					// マテリアルへのポインタ
		DWORD		 dwNumMat;					// マテリアルの数
		D3DXVECTOR3	 vtxMin;					// 最小の頂点座標
		D3DXVECTOR3	 vtxMax;					// 最大の頂点座標
		D3DXVECTOR3	 collsize;					// 当たり判定のサイズ
		float		 fRadius;					// 半径
		int			 m_nTexIdx[MAX_MATERIAL];	// テクスチャのインデックス
	}SXFile;

	CXFile();			// コンストラクタ
	~CXFile();			// デストラクタ

	// メンバ関数
	static HRESULT Init(void);		// Xファイルの初期化処理
	static void Uninit(void);		// Xファイルの終了処理

	// セット・ゲット関数
	static SXFile GetXFile(TYPE type);	// Xファイルの取得処理

private:

	// メンバ関数
	static HRESULT LoadXFile(void);		// xファイルの読み込み
	static void SetCollision(void);		// 当たり判定の作成
	static HRESULT LoadTexture(void);	// テクスチャの読み込み

	// 静的メンバ変数
	static const char *c_apModelData[CXFile::TYPE_MAX];			// モデルの名前
	static SXFile m_apModel[CXFile::TYPE_MAX];					// モデルの情報
};

#endif