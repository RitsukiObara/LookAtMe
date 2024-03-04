//============================================
//
// ボスの名前表示ヘッダー[boss_name.h]
// Author：小原立暉
//
//============================================
#ifndef _BOSS_NAME_H_					// このマクロ定義がされていなかったら
#define _BOSS_NAME_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;		// 2Dポリゴン

//--------------------------------------------
// クラス定義(ボスの名前表示)
//--------------------------------------------
class CBossName : public CObject
{
public:				// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_NAMESHADOW = 0,	// 名前の影
		TYPE_NAME,				// 名前
		TYPE_MAX				// この列挙型の総数
	};

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_APPEAR = 0,		// 出現状態
		STATE_STOP,				// 停止状態
		STATE_SHADOW,			// 影状態
		STATE_LEAVE,			// 退場状態
		STATE_MAX				// この列挙型の総数
	};

	CBossName();				// コンストラクタ
	~CBossName();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新処理
	void Draw(void) override;			// 描画処理

	void SetData(void);					// 情報の設定処理

	// 静的メンバ関数
	static CBossName* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	bool Appear(CObject2D* pObject);		// 出現状態処理
	void AppearArrival(void);				// 出現状態遷移処理
	void Shadow(void);						// 影状態処理
	bool Leave(CObject2D* pObject);			// 退場状態処理

	// メンバ変数
	CObject2D* m_apObject[TYPE_MAX];	// 名前表示の情報
	STATE m_state;			// 状態
	int m_nStateCount;		// 状態カウント
};

#endif