//============================================
//
// 吹き出しヘッダー[balloon.h]
// Author：小原立暉
//
//============================================
#ifndef _BALLOON_H_			// このマクロ定義がされていなかったら
#define _BALLOON_H_			// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// クラス定義(ビルボード)
//--------------------------------------------
class CBalloon : public CBillboard
{
public:				// 誰でもアクセスできる

	// 列挙型定義(種類)
	typedef enum
	{
		TYPE_NORMAL = 0,	// 通常操作
		TYPE_SUPERJUMP,		// 超ジャンプ
		TYPE_WALLKICK,		// 壁キック
		TYPE_CLIFFCLIMB,	// 崖上がり
		TYPE_CLIFFLAND,		// 崖着地
		TYPE_ACTION,		// アクション
		TYPE_POLECATCH,		// ポールキャッチ
		TYPE_POLEJUMP,		// ポールジャンプ
		TYPE_BOOST,			// ブースト
		TYPE_STAMINA,		// スタミナ
		TYPE_SPEED,			// 速度関係
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_SMALL = 0,	// 収縮状態
		STATE_DISP,			// 表示状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	CBalloon();				// コンストラクタ
	~CBalloon();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);							// 状態の設定処理

	TYPE GetType(void) const;									// 種類の取得処理

	// 静的メンバ関数
	static CBalloon* Create(const D3DXVECTOR3& pos, const TYPE type);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void Disp(void);		// 表示状態の処理
	void Small(void);		// 収縮状態の処理
	void Expansion(void);	// 拡大処理
	void Reduce(void);		// 縮小処理
	void Bound(void);		// バウンド処理

	// メンバ変数
	D3DXVECTOR3 m_sizeDisp;		// 描画時のサイズ
	D3DXVECTOR3 m_sizeMove;		// サイズの移動量
	STATE m_state;				// 状態
	TYPE m_type;				// 種類
	bool m_bChange;				// 変化状況
};

#endif