//=====================================================
//
// メッシュスフィアヘッダー [mesh_sphere.h]
// Author：小原立暉
//
//=====================================================
#ifndef _MESH_SPHERE_H_			// このマクロ定義がされていない場合
#define _MESH_SPHERE_H_			// 二重インクルード防止のマクロを定義する

//*****************************************************
// インクルードファイル
//*****************************************************
#include "Objectmesh.h"

//-----------------------------------------------------
// クラス定義(メッシュスフィア)
//-----------------------------------------------------
class CMeshSphere : public CMesh
{
public:		// 誰でもアクセスできる

	CMeshSphere(CObject::PRIORITY priority = PRIORITY_BG);			// オーバーロードコンストラクタ
	virtual ~CMeshSphere();		// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircum, const float fHeight, const SGrid& Divi);			// 情報の設定処理
	void SetVertexAll(void);	// 頂点の設定処理
	void SetVertex(void);		// 頂点の設定処理

	// セット・ゲット関係
	void SetCircum(const float fCircum);		// 幅のサイズの設定処理
	float GetCircum(void) const;				// 幅のサイズの取得処理
	void SetHeight(const float fHeight);		// 縦のサイズの設定処理
	float GetHeight(void) const;				// 縦のサイズの取得処理

	// 静的メンバ関数
	static CMeshSphere* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircum, const float fHeight, const SGrid& Divi, const PRIORITY priority);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	float m_fCircumSize;		// 幅のサイズ
	float m_fHeightSize;		// 縦のサイズ
};


#endif