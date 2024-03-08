//===================================
//
// マルチテクスチャのオブジェクト3Dヘッダー[object3D_multitex.h]
// Author 小原立暉
//
//===================================
#ifndef _OBJECT3D_MULTITEX_H_
#define _OBJECT3D_MULTITEX_H_

#include "object.h"
#include "manager.h"
#include "renderer.h"

//=================================================================================================================================================
// それぞれの描画方法
//=================================================================================================================================================
class CTextureStyle
{
public:
	virtual ~CTextureStyle() = default;		// デストラクタ
	virtual void DispStart(void) = 0;		// 描画初めの処理

	// 描画終わりの処理
	void DispEnd(void)
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定 (テクスチャのアルファブレンドの設定)
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// クラス(マルチテクスチャ3Dポリゴンクラス)
//--------------------------------------------
class CObject3DMultiTex : public CObject
{
public:			// 誰でもアクセスできる

	// 列挙型定義(描画の種類)
	enum TYPE
	{
		TYPE_NONE = 0,					// 無状態
		TYPE_PULL_OUT_WHITE,			// 型抜き(背景白)
		TYPE_BACKGROUND_WHITE,			// 背景切り抜き(背景白)
		TYPE_PULL_OUT_BLACK,			// 型抜き(背景黒)
		TYPE_BACKGROUND_BLACK,			// 背景切り抜き(背景黒)
		TYPE_LIGHT_BLACK,				// 輝くテクスチャ(黒背景)
		TYPE_NEGA_BLACK,				// ネガテクスチャ(黒背景)
		TYPE_MONO_TRANS,				// 白黒背景(半透過) 
		TYPE_PULL_OUT_SP,				// 型抜き(完全透過)
		TYPE_LIGHT_PULL_OUT,			// 型抜きしつつ輝くテクスチャ
		TYPE_LIGHT_SP,					// 輝くテクスチャ(完全透過)
		TYPE_BACKGROUND_SP,				// 背景切り抜き(完全透過)
		TYPE_NEGA_SP,					// ネガテクスチャ(完全透過)
		TYPE_BACKGROUND_NEGA,			// ネガテクスチャ(背景切り抜き)
		TYPE_FUSION,					// 合体処理
		TYPE_MAX						// この列挙型の総数
	};

	CObject3DMultiTex(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CObject3DMultiTex();			// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	void DrawLightOff(void);				// 描画処理(ライティング無し)

	void BindTexture(const int nIdx, const int nCnt);	// テクスチャの割り当て処理

	// 頂点操作系
	void SetVertex(void);									// 頂点情報の設定
	void SetVtxColor(const D3DXCOLOR& col);					// 頂点カラーの設定処理
	void SetVtxTextureWidth(const D3DXVECTOR2& size, int nCnt = 0);		// 頂点テクスチャの設定処理(縦の一定間隔のテクスチャ配分)
	void SetVtxTextureHeight(const D3DXVECTOR2& size, int nCnt = 0);	// 頂点テクスチャの設定処理(横の一定間隔のテクスチャ配分)
	void SetVtxTextureAnim(const D3DXVECTOR2& anim, int nCnt = 0);		// 頂点テクスチャの設定処理(アニメーション版)

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);	// 位置設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置取得処理

	void SetPosOld(const D3DXVECTOR3& pos);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;		// 前回の位置取得処理

	void SetRot(const D3DXVECTOR3& rot);	// 向き設定処理
	D3DXVECTOR3 GetRot(void) const;			// 向き取得処理

	void SetSize(const D3DXVECTOR3& size);	// サイズ設定処理
	D3DXVECTOR3 GetSize(void) const;		// サイズ取得処理

	D3DXMATRIX GetMatrix(void) const;		// マトリックスの取得処理
	D3DXMATRIX* GetMatrixPoint(void);		// マトリックスポインタの取得処理

	void SetType(const TYPE type);			// 種類の設定処理

	int GetTexIdx(const int nCnt) const;	// テクスチャのインデックスの取得処理

	// 静的メンバ関数
	static CObject3DMultiTex* Create(const CObject::TYPE type, const PRIORITY priority, const TYPE typeTex = TYPE_NONE);		// 生成処理

private:

	// メンバ変数
	CTextureStyle* m_pTexStyle;					// テクスチャ描画の情報
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR3 m_size;							// サイズ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_aTexIdx[2];							// テクスチャのインデックス
};

//--------------------------------------------
// クラス定義(型抜き背景白)
//--------------------------------------------
class CPulloutWhite : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_LERP);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// クラス定義(背景切り抜き(背景白))
//--------------------------------------------
class CBackGroundWhite : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// クラス定義(型抜き背景黒)
//--------------------------------------------
class CPulloutBlack : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// クラス定義(背景切り抜き(背景黒))
//--------------------------------------------
class CBackGroundBlack : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// クラス定義(輝くテクスチャ(黒背景))
//--------------------------------------------
class CLightBlack : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// クラス定義(ネガテクスチャ(黒背景))
//--------------------------------------------
class CNegaBlack : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

//--------------------------------------------
// クラス定義(白黒背景(半透過))
//--------------------------------------------
class CMonoTrans : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

//--------------------------------------------
// クラス定義(型抜き(完全透過))
//--------------------------------------------
class CPulloutSpecial : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// クラス定義(型抜きしつつ輝くテクスチャ)
//--------------------------------------------
class CLightPullout : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_PREMODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// クラス定義(輝くテクスチャ(完全透過))
//--------------------------------------------
class CLightSpecial : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// クラス定義(背景切り抜き(完全透過))
//--------------------------------------------
class CBackGroundSpecial : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEMP);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_LERP);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// クラス定義(ネガテクスチャ(完全透過))
//--------------------------------------------
class CNegaSpecial : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

//--------------------------------------------
// クラス定義(ネガテクスチャ(背景切り抜き))
//--------------------------------------------
class CBackGroundNega : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEMP);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

//--------------------------------------------
// クラス定義(合体)
//--------------------------------------------
class CFusion : public CTextureStyle
{
public:
	// 描画初めの処理
	void DispStart(void) override
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// テクスチャステージステートの設定
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

#endif