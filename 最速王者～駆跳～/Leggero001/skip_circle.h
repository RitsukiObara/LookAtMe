//===================================
//
// スキップ円ヘッダー[skip_circle.h]
// Author 小原立暉
//
//===================================
#ifndef _SKIP_CIRCLE_H_
#define _SKIP_CIRCLE_H_

//***********************************
// インクルードファイル
//***********************************
#include "donut2D.h"

//-----------------------------------
// クラス定義(スキップ円)
//-----------------------------------
class CSkipCircle : public CDonut2D
{
public:			// 誰でもアクセスできる

	CSkipCircle();			// コンストラクタ
	~CSkipCircle();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi);					// 情報の設定処理

	// 静的メンバ関数
	static CSkipCircle* Create(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void PushProcess(void);			// 押している時の処理
	void LeaveProcess(void);		// 押していないときの処理
	void EndProcess(void);			// 終了時の処理
	void BigScale(void);			// 拡大処理
	void SizeCorrect(void);			// サイズの補正処理

	// メンバ変数
	int m_nVtx;			// 頂点数
	float m_fSize;		// サイズ(保存用)
	float m_fSizeDest;	// 目的のサイズ
	float m_fDist;		// 距離(保存用)
	float m_fDistDest;	// 目的の距離
	bool m_bEnd;		// 終了状況
};

#endif