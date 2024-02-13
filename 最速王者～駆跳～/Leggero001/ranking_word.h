//===================================
//
// ランキングの文字ヘッダー[ranking_word.h]
// Author 小原立暉
//
//===================================
#ifndef _RANKING_WORD_H_			//このマクロ定義がされていなかったら
#define _RANKING_WORD_H_			//2重インクルード防止のマクロを設定する

//-----------------------------------
// インクルードファイル
//-----------------------------------
#include "object2D.h"

//-----------------------------------
// クラス定義(ランキングの文字)
//-----------------------------------
class CRankingWord : public CObject2D
{
public:			// 誰でもアクセスできる

	CRankingWord();		// コンストラクタ
	~CRankingWord();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// ランキングの文字の初期化処理
	void Uninit(void);	// ランキングの文字の終了処理
	void Update(void);	// ランキングの文字の更新
	void Draw(void);	// ランキングの文字の描画処理

	// 静的メンバ関数
	static CRankingWord* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

};

#endif