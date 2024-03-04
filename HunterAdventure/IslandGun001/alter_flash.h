//===================================
//
// 祭壇閃光ヘッダー[alter_flash.h]
// Author 小原立暉
//
//===================================
#ifndef _ALTER_FLASH_H_
#define _ALTER_FLASH_H_

//***********************************
// インクルードファイル
//***********************************
#include "object2D.h"

//-----------------------------------
// クラス定義(CAlterFlash)
//-----------------------------------
class CAlterFlash : public CObject2D
{
public:			// 誰でもアクセスできる

	CAlterFlash();				// コンストラクタ
	~CAlterFlash();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(void);				// 情報の設定処理

	// 静的メンバ関数
	static CAlterFlash* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	float m_fAlpha;			// 透明度
};

#endif