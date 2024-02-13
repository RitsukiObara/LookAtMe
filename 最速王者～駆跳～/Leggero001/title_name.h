//============================================
//
// タイトルヘッダー[title_name.h]
// Author：小原立暉
//
//============================================
#ifndef _TITLE_NAME_H_					// このマクロ定義がされていなかったら
#define _TITLE_NAME_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"

//--------------------------------------------
// クラス定義(タイトル)
//--------------------------------------------
class CTitleName : public CObject2D
{
public:				// 誰でもアクセスできる

	// 状態
	typedef enum
	{
		STATE_MOVE = 0,		// 移動状態
		STATE_BRAKE,		// ブレーキ状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	CTitleName();			// コンストラクタ
	~CTitleName();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ関数
	static CTitleName* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void MoveState(void);	// 移動状態処理
	void BrakeState(void);	// ブレーキ状態処理

	// メンバ変数
	STATE m_state;			// 状態
	int m_nStateCount;		// 状態カウント
};

#endif