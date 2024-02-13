//===================================
//
// 岩ブロックヘッダー[rockblock.h]
// Author 小原立暉
//
//===================================
#ifndef _ROCKBLOCK_H_
#define _ROCKBLOCK_H_

//***********************************
// インクルードファイル
//***********************************
#include "block.h"

//-----------------------------------
// クラス定義(地面ブロック)
//-----------------------------------
class CRockBlock : public CBlock
{
public:			// 誰でもアクセスできる

	CRockBlock();				// コンストラクタ
	~CRockBlock();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

private:		// 自分だけアクセスできる

};

#endif