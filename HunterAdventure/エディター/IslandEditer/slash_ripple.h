//===================================
//
// 斬撃の波紋ヘッダー[slash_ripple.h]
// Author：小原立暉
//
//===================================
#ifndef _SLASH_RIPPLE_H_			// このマクロ定義がされていなかったら
#define _SLASH_RIPPLE_H_			// 2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "ripple.h"

//-----------------------------------
// クラス定義(斬撃の波紋)
//-----------------------------------
class CSlashRipple : public CRipple
{
public:		// 誰でもアクセスできる

	CSlashRipple();			// コンストラクタ
	~CSlashRipple();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot) override;				// 情報の設定処理

	// 静的メンバ関数
	static CSlashRipple* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// 生成処理

private:	// 自分だけアクセスできる

};

#endif