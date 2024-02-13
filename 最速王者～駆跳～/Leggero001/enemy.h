//============================================
//
// 敵ヘッダー[enemy.h]
// Author：小原立暉
//
//============================================
#ifndef _ENEMY_H_			//このマクロ定義がされていなかったら
#define _ENEMY_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "character.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CMotion;		// モーション

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define ENEMY_GRAVITY		(0.5f)		// 敵の重力
#define ENEMY_WIDTH			(50.0f)		// 敵の幅
#define ENEMY_HEIGHT		(120.0f)	// 敵の高さ

//--------------------------------------------
// クラス(敵クラス)
//--------------------------------------------
class CEnemy : public CCharacter
{
public:			// 誰でもアクセスできる

	// 種類
	typedef enum
	{
		TYPE_SHOT = 0,		// 弾打ち型
		TYPE_KICK,			// キック型
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	// 状態
	typedef enum
	{
		STATE_NONE = 0,		// 通常状態
		STATE_DEATH,		// 死亡状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	CEnemy();				// コンストラクタ
	CEnemy(CObject::TYPE type, PRIORITY priority = PRIORITY_BLOCK);		// コンストラクタ(オーバーロード)
	virtual ~CEnemy();		// デストラクタ
	void Box(void);			// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void) = 0;	// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	void Hit(void);										// ヒット処理
	virtual void SetData(const D3DXVECTOR3& pos) = 0;	// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);	// 状態の設定処理
	STATE GetState(void) const;			// 状態の取得処理

	void SetMotion(void);				// モーションの設定(生成)処理
	CMotion* GetMotion(void);			// モーションの取得処理

	// 静的メンバ関数
	static CEnemy* Create(const D3DXVECTOR3& pos, const TYPE type);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CMotion* m_pMotion;		// モーションの情報
	TYPE m_type;			// 種類
	STATE m_state;			// 状態
};

#endif