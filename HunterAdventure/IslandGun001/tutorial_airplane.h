//===================================
//
// チュートリアル飛行機ヘッダー[tutorial_airplane.h]
// Author 小原立暉
//
//===================================
#ifndef _TUTORIAL_AIRPLANE_H_
#define _TUTORIAL_AIRPLANE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CPropeller;			// プロペラ

//-----------------------------------
// クラス定義(チュートリアル飛行機)
//-----------------------------------
class CTutorialAirplane : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_STOP = 0,		// 停止状態
		STATE_FLY,			// 飛行状態
		STATE_MAX			// この列挙型の総数
	};

	CTutorialAirplane();			// コンストラクタ
	~CTutorialAirplane();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(void);				// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);		// 状態の設定処理
	STATE GetState(void) const;				// 状態の取得処理

	// 静的メンバ関数
	static CTutorialAirplane* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Fly(void);			// 飛行処理

	// メンバ変数
	CPropeller* m_pPropeller;		// プロペラの情報
	STATE m_state;					// 状態
};

#endif