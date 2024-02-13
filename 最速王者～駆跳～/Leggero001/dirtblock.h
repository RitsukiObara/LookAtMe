//===================================
//
// 地面ブロックヘッダー[dirtblock.h]
// Author 小原立暉
//
//===================================
#ifndef _DIRTBLOCK_H_
#define _DIRTBLOCK_H_

//***********************************
// インクルードファイル
//***********************************
#include "block.h"

//-----------------------------------
// クラス定義(地面ブロック)
//-----------------------------------
class CDirtBlock : public CBlock
{
public:			// 誰でもアクセスできる

	CDirtBlock();				// コンストラクタ
	~CDirtBlock();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

private:		// 自分だけアクセスできる

};

#endif