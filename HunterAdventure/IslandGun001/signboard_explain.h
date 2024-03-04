//===================================
//
// 看板の説明ヘッダー[signboard_explain.h]
// Author 小原立暉
//
//===================================
#ifndef _SIGNBOARD_EXPLAIN_H_
#define _SIGNBOARD_EXPLAIN_H_

//***********************************
// インクルードファイル
//***********************************
#include "object.h"
#include "signboard.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CObject2D;			// オブジェクト2D

//-----------------------------------
// クラス定義(CSignboardExpl)
//-----------------------------------
class CSignboardExpl : public CObject
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_APPEAR = 0,		// 出現状態
		STATE_STOP,				// 停止状態
		STATE_LEAVE,			// 退場状態
		STATE_MAX				// この列挙型の総数
	};

	// 構造体定義(背景)
	struct SScreen
	{
		CObject2D* pScreen;		// 背景の情報
		float fAlpha;			// 透明度
	};

	// 構造体定義(説明)
	struct SExplain
	{
		CObject2D* pExplain;	// 説明の情報
		float fMoveY;			// Y軸の移動量
	};

	CSignboardExpl();			// コンストラクタ
	~CSignboardExpl();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const CSignboard::TYPE type);						// 情報の設定処理

	// 静的メンバ関数
	static CSignboardExpl* Create(const CSignboard::TYPE type);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Appear(void);		// 出現状態処理
	bool Leave(void);		// 退場状態処理

	// メンバ変数
	SScreen m_screen;		// 画面の情報
	SExplain m_explain;		// 説明の情報
	STATE m_state;			// 状態
};

#endif