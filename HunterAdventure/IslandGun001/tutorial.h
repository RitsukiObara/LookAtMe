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
// 前方宣言
//--------------------------------------------
class CTutorialPlayer;			// チュートリアルプレイヤー
class CSignboard;				// 看板
class CDoor;					// ドア
class CTutorialAirplane;		// チュートリアルの飛行機

//--------------------------------------------
// クラス(チュートリアルクラス)
//--------------------------------------------
class CTutorial : public CScene
{
public:				// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_EXPL,			// 説明状態
		STATE_TRANS,		// 遷移状態
		STATE_MAX			// この列挙型の総数
	};

	CTutorial();		// コンストラクタ
	~CTutorial();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新処理
	void Draw(void) override;			// 描画処理

	// 静的メンバ関数
	static CTutorialPlayer* GetPlayer(void);			// プレイヤーの取得処理
	static CDoor* GetDoor(void);						// ドアの取得処理
	static void SetState(const STATE state);			// 状態の設定処理
	static STATE GetState(void);						// 状態の取得処理
	static void SetLookSign(CSignboard* pSign);			// 看板の設定処理
	static CSignboard* GetLookSign(void);				// 看板の取得処理
	static CTutorialAirplane* GetAirplane(void);		// 飛行機の取得処理

	static void DeletePlayer(void);				// プレイヤーのNULL化処理
	static void DeleteDoor(void);				// ドアのNULL化処理
	static void DeleteAirplane(void);			// 飛行機のNULL化処理

private:			// 自分だけアクセスできる

	// メンバ変数
	int m_nTransCount;					// 遷移カウント

	// 静的メンバ変数
	static CTutorialPlayer* m_pPlayer;	// プレイヤーの情報
	static CSignboard* m_pLook;			// 現在見てる看板
	static CDoor* m_pDoor;				// ドアの情報
	static CTutorialAirplane* m_pAirplane;	// 飛行機の情報
	static STATE m_state;				// 状態
};

#endif