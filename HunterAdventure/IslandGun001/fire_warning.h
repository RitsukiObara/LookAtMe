//===================================
//
// 炎注意ヘッダー[fire_warning.h]
// Author 小原立暉
//
//===================================
#ifndef _FIRE_WARNING_H_
#define _FIRE_WARNING_H_

//***********************************
// インクルードファイル
//***********************************
#include "object3Dfan.h"

//--------------------------------------------
// クラス(炎注意クラス)
//--------------------------------------------
class CFireWarning : public CObject3DFan
{
public:			// 誰でもアクセスできる

	CFireWarning();		// オーバーロードコンストラクタ
	virtual ~CFireWarning();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);					// 情報の設定処理

	// 静的メンバ関数
	static CFireWarning* Create(const D3DXVECTOR3& pos);	// 生成処理

private:

	// メンバ変数
	D3DXCOLOR m_col;		// 色
	int m_nCount;			// 経過カウント
};

#endif