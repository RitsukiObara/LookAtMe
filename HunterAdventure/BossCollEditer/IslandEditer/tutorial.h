//============================================
//
// チュートリアル画面ヘッダー[tutorial.h]
// Author：小原立暉
//
//============================================
#ifndef _TUTORIAL_H_			//このマクロ定義がされていなかったら
#define _TUTORIAL_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "scene.h"

//--------------------------------------------
// クラス(チュートリアルクラス)
//--------------------------------------------
class CTutorial : public CScene
{
public:				// 誰でもアクセスできる

	CTutorial();		// コンストラクタ
	~CTutorial();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新処理
	void Draw(void) override;			// 描画処理

private:			// 自分だけアクセスできる

	// メンバ変数
	int m_nEndCount;					// 終了までのカウント
};

#endif