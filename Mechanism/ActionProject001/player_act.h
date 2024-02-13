//============================================
//
// プレイヤーの行動ヘッダー[player_act.h]
// Author：小原立暉
//
//============================================
#ifndef _PLAYER_ACT_H_					// このマクロ定義がされていなかったら
#define _PLAYER_ACT_H_					// 2重インクルード防止のマクロを定義する

//--------------------------------------------
// クラス(プレイヤーの行動クラス)
//--------------------------------------------
class CPlayerAct
{
public:			// 誰でもアクセスできる

	//************************************************************
	//	列挙型定義(行動状態)
	//************************************************************
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_INVINCIBLE,	// 無敵状態
		STATE_BOUND,		// バウンド状態
		STATE_CANNON,		// 大砲状態
		STATE_FLY,			// 飛んでいる状態
		STATE_DEATH,		// 死亡状態
		STATE_FALL,			// 落下状態
		STATE_MAX			// この列挙の総数
	};

	CPlayerAct();			// コンストラクタ
	~CPlayerAct();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(CPlayer& player);		// 更新処理

	void SetData(void);		// 情報の設定処理

	// セット・ゲット関数
	void SetState(STATE act);			// 状態の設定処理
	STATE GetState(void) const;			// 状態の取得処理
	void SetFront(bool bFront);		// 前後状況の設定処理
	bool GetFront(void) const;		// 前後状況の取得処理

	// 静的メンバ関数
	static CPlayerAct* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void NoneControl(CPlayer& player);	// 通常時の操作処理
	void GroundQuake(CPlayer& player);	// グラウンドクエイク
	void RotDecide(CPlayer& player);	// 向きの決定処理
	void Control(CPlayer& player);		// 操作処理
	void MoveProcess(CPlayer& player);	// 移動の基本処理
	void ModeSpeed(CPlayer& player);	// モードごとのスピード処理
	void Move(CPlayer& player);			// 移動処理
	void Gravity(CPlayer& player);		// 重力処理
	void RotMove(CPlayer& player);		// 向きの移動処理
	void CannonRot(CPlayer& player);	// 大砲で向きを変える処理
	void GravityReset(CPlayer& player);	// 重力のリセット処理
	void FlyMove(CPlayer& player);		// 大砲で飛ぶ処理

	// メンバ変数
	STATE m_state;			// 行動状態
	int m_nStateCount;		// 状態カウント
	bool m_bFront;			// 前後状況
};

#endif