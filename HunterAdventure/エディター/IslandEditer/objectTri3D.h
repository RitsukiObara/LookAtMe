//===================================
//
// オブジェクト三角3Dヘッダー[objectTri3D.h]
// Author 小原立暉
//
//===================================
#ifndef _OBJECTTRI3D_H_
#define _OBJECTTRI3D_H_

#include "object.h"

//--------------------------------------------
// クラス(オブジェクト三角3Dクラス)
//--------------------------------------------
class CObjectTri3D : public CObject
{
public:			// 誰でもアクセスできる

	CObjectTri3D(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CObjectTri3D();		// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	void DrawLightOff(void);		// 描画処理(ライティング無し)

	void BindTexture(int nIdx);		// テクスチャの割り当て処理

	// 頂点操作系
	void SetVertex(void);									// 頂点情報の設定
	void SetVertexHardCoding(const D3DXVECTOR3& Left, const D3DXVECTOR3& Up, const D3DXVECTOR3& Right);			// 頂点情報の設定(決め打ち)
	void SetVtxColor(const D3DXCOLOR& col);					// 頂点カラーの設定処理
	void SetVtxTexture(void);								// テクスチャ座標の設定処理

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);	// 位置設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置取得処理

	void SetPosOld(const D3DXVECTOR3& posOld);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;			// 前回の位置取得処理

	void SetRot(const D3DXVECTOR3& rot);	// 向き設定処理
	D3DXVECTOR3 GetRot(void) const;			// 向き取得処理

	void SetSize(const D3DXVECTOR3& size);	// サイズ設定処理
	D3DXVECTOR3 GetSize(void) const;		// サイズ取得処理

	D3DXMATRIX GetMatrix(void) const;		// マトリックスの取得処理

	int GetTexIdx(void) const;				// テクスチャのインデックスの取得処理

	// 静的メンバ関数
	static CObjectTri3D* Create(const TYPE type, const PRIORITY priority);		// 生成処理

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR3 m_size;							// サイズ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_nTexIdx;								// テクスチャのインデックス
};

#endif