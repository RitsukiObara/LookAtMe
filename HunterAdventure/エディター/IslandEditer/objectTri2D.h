//============================================
//
// オブジェクト三角形2Dヘッダー[objectTri2D.h]
// Author：小原立暉
//
//============================================
#ifndef _OBJECTTRI2D_H_		//このマクロ定義がされていなかったら
#define _OBJECTTRI2D_H_		//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// クラス(オブジェクト三角形2Dクラス)
//--------------------------------------------
class CObjectTri2D : public CObject
{
public:			// 誰でもアクセスできる

	CObjectTri2D(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CObjectTri2D();		// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	void BindTexture(int nIdx);		// テクスチャの割り当て処理

	// 頂点操作系
	void SetVertex(void);						// 頂点情報の設定
	void SetVertexRot(void);					// 頂点情報の設定(向き有り)
	void SetVtxHardCoding(const D3DXVECTOR3& Left, const D3DXVECTOR3& Up, const D3DXVECTOR3& Right);	// 頂点情報の設定(ベタ打ち)
	void SetVtxColor(const D3DXCOLOR& col);		// 頂点の色設定処理
	void SetVtxTexture(void);					// テクスチャの設定
	void SetVertexAll(void);					// 全ての頂点情報の設定

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);		// 位置設定処理
	D3DXVECTOR3 GetPos(void) const;				// 位置取得処理

	void SetPosOld(const D3DXVECTOR3& posOld);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;			// 前回の位置取得処理

	void SetRot(const D3DXVECTOR3& rot);		// 向き設定処理
	D3DXVECTOR3 GetRot(void) const;				// 向き取得処理

	void SetSize(const D3DXVECTOR3& size);		// サイズ設定処理
	D3DXVECTOR3 GetSize(void) const;			// サイズ取得処理

	void SetLength(void);						// 長さ設定処理
	float GetLength(void) const;				// 長さ取得処理

	int GetTexIdx(void) const;					// テクスチャの番号の取得処理

	// 静的メンバ関数
	static CObjectTri2D* Create(const CObject::TYPE type, const PRIORITY priority);		// 生成処理

protected:		// 自分と派生クラスだけアクセスできる

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR3 m_size;							// サイズ
	float m_fLength;							// 長さ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	int m_nTexIdx;								// テクスチャのインデックス
};

#endif