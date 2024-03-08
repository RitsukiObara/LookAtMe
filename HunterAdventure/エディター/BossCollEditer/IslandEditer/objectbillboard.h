//===================================
//
// ビルボードヘッダー[billboard.h]
// Author 小原立暉
//
//===================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "object3D.h"

//--------------------------------------------
// クラス(ビルボードクラス)
//--------------------------------------------
class CBillboard : public CObject
{
public:			// 誰でもアクセスできる

	CBillboard(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CBillboard();				// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;		// 初期化処理
	virtual void Uninit(void) override;			// 終了処理
	virtual void Update(void) override;			// 更新処理
	virtual void Draw(void) override;			// 描画処理

	void DrawLightOff(const bool bZTest);			// 描画処理(ライティング無し)
	void DrawShift(const D3DXVECTOR3 shift);		// 描画処理(位置ずらし)

	void BindTexture(int nIdx);			// テクスチャの割り当て処理

	// 頂点情報関係
	void SetVertex(void);										// 頂点情報の設定
	void SetVertexRot(void);									// 頂点情報の設定(向き有り)
	void SetVtxPosUnder(void);									// 頂点情報の設定(下頂点)
	void SetVtxAnim(const float fTex, const int nPattern);		// アニメーションの設定
	void SetVtxAnimRev(const float fTex, const int nPattern);	// アニメーションの設定(反転)
	void SetVtxColor(D3DXCOLOR col);							// 頂点カラーの設定

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);		// 位置の設定処理
	D3DXVECTOR3 GetPos(void) const;				// 位置の取得処理

	void SetPosOld(const D3DXVECTOR3& posOld);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;			// 前回の位置取得処理

	void SetRot(const float fRot);				// 向きの設定処理
	float GetRot(void) const;					// 向きの取得処理

	void SetSize(const D3DXVECTOR3& size);		// サイズの設定処理
	D3DXVECTOR3 GetSize(void) const;			// サイズの取得処理

	void SetAngle(void);						// 方向の設定処理

	void SetLength(void);						// 長さの設定処理

	int GetTexIdx(void) const;					// テクスチャのインデックスの取得処理

	void SetEnableLookY(const bool bLookY);		// Y軸状況の設定処理
	bool IsLookY(void) const;					// Y軸状況の取得処理

	// 静的メンバ関数
	static CBillboard* Create(const TYPE type, const PRIORITY priority);	// 生成処理

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	D3DXVECTOR3 m_size;							// サイズ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_nTexIdx;								// テクスチャのインデックス
	float m_fRot;								// 向き
	float m_fAngle;								// 方向
	float m_fLength;							// 長さ
	bool m_bLookY;								// Y軸状況
};

//--------------------------------------------
// クラス(ビルボードクラス)
//--------------------------------------------
class CBillboardAnim : public CBillboard
{
public:			// 誰でもアクセスできる

	CBillboardAnim(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CBillboardAnim();				// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;		// 初期化処理
	virtual void Uninit(void) override;			// 終了処理
	virtual void Update(void) override;			// 更新処理
	virtual void Draw(void) override;			// 描画処理

	void DrawLightOff(const bool bZTest);	// 描画処理(ライティング無し)

	// セット・ゲット関係
	int GetPattern(void) const;								// パターンの取得処理
	float GetTexPattern(void) const;						// テクスチャのパターンの取得処理
	void SetAnim(const int nCount, const int nPattern);		// アニメーションの設定処理

	// 静的メンバ関数
	static CBillboardAnim* Create(const TYPE type, const PRIORITY priority);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nCounter;					// アニメーションカウンター
	int m_nReplayCount;				// 再生カウント
	int m_nPattern;					// アニメーションパターン
	int m_nReplayPattern;			// 再生パターン
	float m_fTexPattern;			// テクスチャのパターン
};

#endif