//============================================
//
// 障害物蹴り敵ヘッダー[kick_enemy.h]
// Author：小原立暉
//
//============================================
#ifndef _KICK_ENEMY_H_			//このマクロ定義がされていなかったら
#define _KICK_ENEMY_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "enemy.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CDrum;			// 障害物

//--------------------------------------------
// クラス(障害物蹴り敵クラス)
//--------------------------------------------
class CKickEnemy : public CEnemy
{
public:			// 誰でもアクセスできる

	//--------------------------------------------
	// モーションの種類
	//--------------------------------------------
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,		// 通常
		MOTIONTYPE_MAX				// この列挙型の総数
	}MOTIONTYPE;

	//--------------------------------------------
	// 行動状態
	//--------------------------------------------
	typedef enum
	{
		ACT_WAITING = 0,			// 待機状態
		ACT_BRING,					// 構え状態
		ACT_SHOOT,					// シュート処理
		ACT_MAX						// この列挙型の総数
	}ACT;

	CKickEnemy();		// コンストラクタ
	~CKickEnemy();		// デストラクタ

	// メンバ関数
	HRESULT Init();		// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	void SetData(const D3DXVECTOR3& pos);	// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ関数
	bool SearchObstacle(void);	// 障害物の捜索処理
	void Turn(void);			// 振り向き処理
	void Collision(void);		// 当たり判定処理

	// メンバ変数
	ACT m_action;				// 行動状態
	CDrum* m_pDrum;				// ドラム缶のポインタ
	D3DXVECTOR3 m_posDest;		// ドラム缶の目的の位置
	D3DXVECTOR3 m_drumMove;		// ドラム缶の移動量
	int m_nStateCount;			// 状態カウント
	float m_fGravity;			// 重力
	bool m_bRight;				// 右向き状況
};

#endif