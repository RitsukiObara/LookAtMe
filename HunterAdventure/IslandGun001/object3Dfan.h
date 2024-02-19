//===================================
//
// オブジェクト3Dファンヘッダー[object3Dfan.h]
// Author 小原立暉
//
//===================================
#ifndef _OBJECT3DFAN_H_
#define _OBJECT3DFAN_H_

#include "object.h"

//--------------------------------------------
// クラス(オブジェクト3Dクラス)
//--------------------------------------------
class CObject3DFan : public CObject
{
public:			// 誰でもアクセスできる

	CObject3DFan(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CObject3DFan();		// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	//void BindTexture(int nIdx);				// テクスチャの割り当て処理

	// 頂点操作系
	void SetVertex(void);						// 頂点情報の設定
	void SetVtxColor(const D3DXCOLOR& col);		// 頂点カラーの設定処理

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);		// 位置の設定処理
	D3DXVECTOR3 GetPos(void) const;				// 位置の取得処理
	void SetRot(const D3DXVECTOR3& rot);		// 向きの設定処理
	D3DXVECTOR3 GetRot(void) const;				// 向きの取得処理
	void SetNumAngle(const int nNum);			// 角度の数の設定処理
	int GetNumAngle(void) const;				// 角度の数の取得処理
	void SetRadius(const float fRadius);		// 半径の設定処理
	float GetRadius(void) const;				// 半径の取得処理

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 向き
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_nNumAngle;							// 角度の数
	float m_fRadius;							// 半径
};

#endif