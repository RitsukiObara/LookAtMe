//============================================
//
// 弾撃ち敵ヘッダー[shotenemy.h]
// Author：小原立暉
//
//============================================
#ifndef _SHOTENEMY_H_			//このマクロ定義がされていなかったら
#define _SHOTENEMY_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "enemy.h"

//--------------------------------------------
// クラス(弾撃ち敵クラス)
//--------------------------------------------
class CShotEnemy : public CEnemy
{
public:			// 誰でもアクセスできる

	// モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,		// 通常
		MOTIONTYPE_MAX				// この列挙型の総数
	}MOTIONTYPE;

	// 状態
	typedef enum
	{
		ACT_SHOT = 0,	// 射撃状態
		ACT_TURN,		// 振り向き状態
		ACT_MAX			// この列挙型の総数
	}ACT;

	CShotEnemy();		// コンストラクタ
	~CShotEnemy();		// デストラクタ

	// メンバ関数
	HRESULT Init();		// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	void SetData(const D3DXVECTOR3& pos);		// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void InitRot(void);		// 初期の向きの設定処理
	void Shot(void);		// 射撃処理
	void Turn(void);		// 振り向き処理

	void ActionShot(void);	// 弾撃ち状態での処理
	void ActionTurn(void);	// 振り向き状態での処理

	// メンバ変数
	ACT m_action;			// 行動状態
	int m_nShotCount;		// 弾撃ちカウント
	float m_fGravity;		// 重力
	float m_fRotDest;		// 目的の向き
	float m_fRotDiff;		// 向きの差分
};

#endif