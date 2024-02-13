//===================================
//
// コンボの制限円ヘッダー[combo_circle.h]
// Author 小原立暉
//
//===================================
#ifndef _COMBO_CIRCLE_H_
#define _COMBO_CIRCLE_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectDonut2D.h"

//-----------------------------------
// クラス定義(コンボの制限円)
//-----------------------------------
class CComboCircle : public CDonut2D
{
public:			// 誰でもアクセスできる

	CComboCircle();			// コンストラクタ
	~CComboCircle();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi);					// 情報の設定処理

	void ResetNumVtx(void);		// 頂点のリセット処理

	// セット・ゲット関係
	void SetEnd(const bool bEnd);		// 終了状況の設定処理
	bool GetEnd(void);					// 終了状況の取得処理

	// 静的メンバ関数
	static CComboCircle* Create(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nVtx;			// 頂点数
	bool m_bEnd;		// 終了状況
};

#endif