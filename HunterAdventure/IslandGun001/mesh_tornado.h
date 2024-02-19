//=====================================================
//
// メッシュトルネードヘッダー [mesh_tornado.h]
// Author：小原立暉
//
//=====================================================
#ifndef _MESH_TORNADO_H_			// このマクロ定義がされていない場合
#define _MESH_TORNADO_H_			// 二重インクルード防止のマクロを定義する

//*****************************************************
// インクルードファイル
//*****************************************************
#include "Objectmesh.h"

//-----------------------------------------------------
// クラス定義(メッシュトルネード)
//-----------------------------------------------------
class CMeshTornado : public CMesh
{
public:		// 誰でもアクセスできる

	CMeshTornado(CObject::PRIORITY priority = PRIORITY_BG);			// オーバーロードコンストラクタ
	virtual ~CMeshTornado();		// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const int nVortex, const float fShift, const float fCircum, const SGrid& Divi);			// 情報の設定処理
	void SetVertexAll(void);	// 全ての頂点の設定処理
	void SetVertex(void);		// 頂点の設定処理

	// セット・ゲット関係
	void SetVortex(const int nVortex);		// 渦の設定処理
	int GetVortex(void) const;				// 渦の取得処理
	void SetWidth(const float fWidth);		// 幅の設定処理
	float GetWidth(void) const;				// 幅の取得処理
	void SetHeight(const float fHeight);	// 高さの設定処理
	float GetHeight(void) const;			// 高さの取得処理
	void SetShift(const float fShift);		// ずらす幅の取得処理
	float GetShift(void) const;				// ずらす幅の取得処理
	void SetCircum(const float fCircum);	// 円周の設定処理
	float GetCircum(void) const;			// 円周の取得処理

	// 静的メンバ関数
	static CMeshTornado* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const int nVortex, const float fShift, const float fCircum, const SGrid& Divi, const PRIORITY priority);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nVortex;		// 渦の数
	float m_fWidth;		// 幅
	float m_fHeight;	// 高さ
	float m_fShift;		// ずらす幅
	float m_fCircum;	// 円周
};


#endif