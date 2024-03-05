//===================================
//
// 壁ヘッダー[wall.h]
// Author 小原立暉
//
//===================================
#ifndef _WALL_H_
#define _WALL_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// クラス定義(壁)
//-----------------------------------
class CWall : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_NORMAL = 0,	// 通常種類
		TYPE_NARROW,		// 細い種類
		TYPE_MAX			// この列挙型の総数
	};

	// 列挙型定義(種類)
	enum ROTTYPE
	{
		ROTTYPE_FRONT = 0,	// 前向き
		ROTTYPE_RIGHT,		// 右向き
		ROTTYPE_BACK,		// 後ろ向き
		ROTTYPE_LEFT,		// 左向き
		ROTTYPE_MAX			// この列挙型の総数
	};

	CWall();				// コンストラクタ
	~CWall();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const TYPE type, const ROTTYPE rottype);				// 情報の設定処理

	// セット・ゲット関係
	D3DXVECTOR3 GetVtxMax(void) const;		// 頂点の最大値の取得処理
	D3DXVECTOR3 GetVtxMin(void) const;		// 頂点の最小値の取得処理
	TYPE GetType(void) const;				// 種類の取得処理
	ROTTYPE GetRotType(void) const;			// 向きの種類の取得処理

	// 静的メンバ関数
	static CWall* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const TYPE type, const ROTTYPE rottype);		// 生成処理
	static CListManager<CWall*> GetList(void);			// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void SetVertex(void);		// 頂点の設定処理

	// メンバ変数
	D3DXVECTOR3 m_vtxMax;		// 頂点の最大値
	D3DXVECTOR3 m_vtxMin;		// 頂点の最小値
	TYPE m_type;				// 種類
	ROTTYPE m_rottype;			// 向きの種類

	// 静的メンバ変数
	static CListManager<CWall*> m_list;		// リスト
};

#endif