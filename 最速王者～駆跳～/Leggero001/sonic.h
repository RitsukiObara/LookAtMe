//===================================
//
// 高速演出ヘッダー[sonic.h]
// Author 小原立暉
//
//===================================
#ifndef _SONIC_H_
#define _SONIC_H_

//***********************************
// インクルードファイル
//***********************************
#include "scrollobject.h"

//-----------------------------------
// クラス定義(CSonic)
//-----------------------------------
class CSonic : public CScroll
{
public:			// 誰でもアクセスできる

	CSonic();				// コンストラクタ
	~CSonic();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ関数
	static CSonic* Create(void);		// 生成処理

private:		// 自分だけアクセスできる

};

#endif