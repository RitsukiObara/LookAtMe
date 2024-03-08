//======================================================================================================================
//
// メッシュヘッダー [mesh.h]
// Author：小原立暉
//
//======================================================================================================================
#ifndef _MESH_H_			// このマクロ定義がされていない場合
#define _MESH_H_			// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
// インクルードファイル
//**********************************************************************************************************************
#include "object.h"
#include "list_manager.h"

//----------------------------------------------------------------------------------------------------------------------
// クラス定義(メッシュ)
//----------------------------------------------------------------------------------------------------------------------
class CMesh : public CObject
{
public:			// 誰でもアクセスできる

	// 構造体定義(方眼情報)
	struct SGrid
	{
		int nWidth;			// 横の方眼情報
		int nHeight;		// 縦の方眼情報
	};

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_FIELD = 0,		// フィールド
		TYPE_WALL,			// 壁
		TYPE_CYLINDER,		// 筒
		TYPE_DOME,			// ドーム
		TYPE_DONUT,			// ドーナツ
		TYPE_TORNADO,		// トルネード
		TYPE_MAX			// この列挙型の総数
	};

	CMesh(CObject::TYPE type, CObject::PRIORITY priority = PRIORITY_BG);			// オーバーロードコンストラクタ
	virtual ~CMesh();	// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	void DrawProcess(void);			// 描画処理の中身

	// 頂点・インデックスの設定関係
	void SetIndex(const int nNumVtx1, const int nNumVtx2);		// インデックスの設定処理
	void SetColor(const D3DXCOLOR& col);						// 頂点カラーの設定処理

	void BindTexture(int nIdx);		// テクスチャの割り当て処理

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);	// 位置の設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置の取得処理

	void SetRot(const D3DXVECTOR3& rot);	// 向きの設定処理
	D3DXVECTOR3 GetRot(void) const;			// 向きの取得処理

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const;		// 頂点バッファのポインタの取得処理

	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void) const;		// インデックスバッファのポインタの取得処理

	void SetType(const TYPE type);			// 種類の設定処理
	TYPE GetType(void) const;				// 種類の取得処理

	void SetVtx(const SGrid vtx);			// 頂点数の設定処理
	SGrid GetVtx(void) const;				// 頂点数の取得処理

	void SetDivi(const SGrid divi);			// 分割数の設定処理
	SGrid GetDivi(void) const;				// 分割数の取得処理

	void SetNumVtx(void);					// 総頂点数の設定処理
	int GetNumVtx(void);					// 総頂点数の取得処理

	void SetNumIdx(void);					// 総インデックス数の設定処理
	int GetNumIdx(void);					// 総インデックス数の取得処理

	void SetLighting(const bool bLighting);	// ライティングの設定処理

	void SetCulling(const bool bCulling);	// カリングの設定処理

	// 静的メンバ変数
	static void TxtSet(void);				// テキスト読み込み処理
	static int TxtField(FILE* pFile);		// フィールドの読み込み処理
	static int TxtWall(FILE* pFile);		// ウォールの読み込み処理
	static int TxtCylinder(FILE* pFile);	// シリンダーの読み込み処理
	static int TxtDome(FILE* pFile);		// ドームの読み込み処理
	static int TxtDonut(FILE* pFile);		// ドーナツの読み込み処理
	static int TxtTornado(FILE* pFile);		// トルネードの読み込み処理
	static int TxtSphere(FILE* pFile);		// スフィアの読み込み処理

	static CListManager<CMesh*> GetList(void);			// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3	m_pos;			// 位置
	D3DXVECTOR3	m_rot;			// 向き
	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;		// インデックスバッファへのポインタ
	TYPE	m_type;				// 種類
	SGrid	m_vtx;				// 頂点数
	SGrid	m_divi;				// 分割数
	int		m_nNumVtx;			// 総頂点数
	int		m_nNumIdx;			// 総インデックス数
	int		m_nTexIdx;			// テクスチャのインデックス
	bool	m_bLightOff;		// ライティング状況
	bool	m_bCullOff;			// カリング状況

	// 静的メンバ変数
	static CListManager<CMesh*> m_list;		// リスト
};


#endif