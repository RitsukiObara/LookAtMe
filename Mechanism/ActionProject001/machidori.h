//===================================
//
// マシンドリーヘッダー[machidori.h]
// Author 小原立暉
//
//===================================
#ifndef _MACHIDORI_H_
#define _MACHIDORI_H_

//***********************************
// インクルードファイル
//***********************************
#include "enemy.h"

//-----------------------------------
// クラス定義(マシンドリー)
//-----------------------------------
class CMachidori : public CEnemy
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_FLY = 0,		// 浮遊状態
		STATE_STANDBY,		// スタンバイ状態
		STATE_ATTACK,		// 攻撃状態
		STATE_UP,			// 上昇状態
		STATE_DEATH,		// 死亡状態
		STATE_SMASH,		// 吹き飛び状態
		STATE_MAX			// この列挙型の総数
	};

	CMachidori();			// コンストラクタ
	~CMachidori();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void Hit(void);			// ヒット処理
	void SmashHit(void);	// 吹き飛びヒット処理
	void StunHit(void);		// 気絶のヒット処理
	void SetData(const D3DXVECTOR3& pos, const TYPE type);				// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void CheckPlayer(void);	// プレイヤーの判断処理
	void Move(void);		// 移動処理
	void Height(void);		// 高さ設定処理
	void Elevation(void);	// 起伏地面の当たり判定
	void SetRightOld(void);	// 前回の左右状況の設定処理
	void StandBy(void);		// スタンバイ処理
	void AttackRot(void);	// 攻撃状態での向きの設定処理
	void UpAscent(void);	// 上昇状態の上昇処理
	void DeathVib(void);	// 死亡時の振動処理
	void Smoke(void);		// 煙の生成処理
	void Smash(void);		// 吹き飛び状態の処理

	// メンバ変数
	STATE m_state;			// 状態
	int m_nStateCount;		// 状態カウント
	float m_fCurveRot;		// カーブ用向き
	float m_fHeight;		// 追加の高さ
	float m_fElevPos;		// 起伏の位置
	bool m_bRight;			// 右向き状況
};

#endif