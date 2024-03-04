//===================================
//
//ランキング画面ヘッダー[ranking.h]
//Author 小原立暉
//
//===================================
#ifndef _RANKING_H_			//このマクロ定義がされていなかったら
#define _RANKING_H_			//2重インクルード防止のマクロを設定する

//***********************************
// インクルードファイル
//***********************************
#include "scene.h"

//-----------------------------------
// クラス定義(ランキングシーン)
//-----------------------------------
class CRanking : public CScene
{
public:			// 誰でもアクセスできる

	CRanking();					// コンストラクタ
	~CRanking();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新
	void Draw(void) override;			// 描画処理

	// 定数
	static const int MAX_RANKING = 5;	// ランキングの数

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nTransCount;				// 自動遷移カウント
};

#endif