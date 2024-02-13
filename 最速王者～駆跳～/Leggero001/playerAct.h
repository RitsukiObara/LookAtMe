//============================================
//
// プレイヤーの行動管理ヘッダー[playerAct.h]
// Author：小原立暉
//
//============================================
#ifndef _PLAYERACT_H_					// このマクロ定義がされていなかったら
#define _PLAYERACT_H_					// 2重インクルード防止のマクロを定義する

//--------------------------------------------
// マクロ定義
//--------------------------------------------
// カウント関係
#define PLAYER_CLIFFCLIMB_SUCCESS_COUNT		(60)		// 崖掴まり状態での成功のカウント数
#define PLAYER_CLIFFCLIMB_FALL_COUNT		(120)		// 崖掴まり状態での自動落下のカウント数
#define PLAYER_INVINCIBLE_COUNT				(100)		// 無敵状態のカウント数
#define PLAYER_DAMAGE_COUNT					(10)		// ダメージ状態のカウント数
#define PLAYER_WALLKICK_COUNT				(45)		// 壁キック状態のカウント数
#define PLAYER_WALLAFTERGLOW_COUNT			(20)		// 壁キックの余韻状態のカウント数
#define PLAYER_WALLLEAVE_COUNT				(6)			// 壁離し状態のカウント数
#define PLAYER_CLIFFLAND_COUNT				(12)		// 崖離し着地のカウント数
#define PLAYER_CLIFFAFTERGLOW_COUNT			(4)			// 余韻状態のカウント数
#define PLAYER_WALLFRIC_COUNT				(80)		// 壁擦り状態が強制解除されるカウント数
#define PLAYER_CLIFFFALL_COUNT				(30)		// 崖降り状態のカウント数
#define PLAYER_POLEJUMP_COUNT				(3)			// ポールからジャンプしている状態のカウント数
#define PLAYER_WALLFRIC_DUST_COUNT			(4)			// 壁擦り時の埃が出るカウント数
#define PLAYER_CLIFFCLIMB_AURA_COUNT		(25)		// 崖上がり時のオーラが出てくるカウント
#define PLAYER_CLIFFLAND_AURA_COUNT			(8)			// 崖着地時のオーラが出てくるカウント
#define PLAYER_FAST_LAMPJUMP_COUNT			(13)		// 最速街灯ジャンプが出るカウント数

// スコア関係
#define PLAYER_SUPERJUMP_SCORE			(300)		// プレイヤーの超ジャンプのスコア
#define PLAYER_CLIFFLAND_SCORE			(500)		// プレイヤーの崖離し着地のスコア
#define PLAYER_WALLKICK_SCORE			(100)		// プレイヤーの壁キックのスコア
#define PLAYER_LAMPJUMP_SCORE			(200)		// プレイヤーの街灯ジャンプのスコア
#define PLAYER_OBSTACLE_SCORE			(50)		// プレイヤーの障害物へのアクションのスコア

#define POLE_PLAYER_DIST				(35.0f)		// ぶら下がっている時のポールとプレイヤーとの距離
#define AURA_RAND_SIZE					(75.0f)		// オーラを出す時のランダムのサイズ
#define AURA_NORMAL_LIFE				(20)		// オーラの通常の寿命
#define AURA_CLIFF_LIFE					(60)		// オーラの崖掴まり時の寿命

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CPlayer;			// プレイヤー

//--------------------------------------------
// クラス(プレイヤーの行動クラス)
//--------------------------------------------
class CPlayerAct
{
public:			// 誰でもアクセスできる

	//--------------------------------------------
	// 行動
	//--------------------------------------------
	typedef enum
	{
		ACT_NONE = 0,			// 無し
		ACT_BIGJUMP,			// 超ジャンプ
		ACT_WALLFRIC,			// 壁擦り状態
		ACT_WALLLEAVE,			// 壁離れ状態
		ACT_WALLSTEPIN,			// 壁踏み込み処理
		ACT_WALLKICK,			// 壁キック
		ACT_WALLAFTERGLOW,		// 壁キックの余韻
		ACT_CLIFFCATCH,			// 崖つかまり
		ACT_CLIFFCLIMB,			// 崖登り
		ACT_CLIFFFALL,			// 崖降り
		ACT_CLIFFLAND,			// 崖離し着地
		ACT_CLIFFAFTERGLOW,		// 崖着地の余韻状態
		ACT_HANGING,			// ぶら下がり状態
		ACT_LAMPJUMP,			// ランプから飛んでいる状態
		ACT_POLEDOWN,			// ポール下り状態
		ACT_MAX					// この列挙型の総数
	}ACT;

	CPlayerAct();				// コンストラクタ
	~CPlayerAct();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理

	void Action(CPlayer& player);			// 行動状態処理

	// 静的メンバ関数
	static CPlayerAct* Create(void);		// 生成処理

	// セットゲット関係
	void SetAct(const ACT act);				// 行動状態の設定処理
	ACT GetAct(void) const;					// 行動状態の取得処理

	void SetWallOld(const bool bWall);		// 前回の壁擦り状況の設定処理
	bool GetWallOld(void) const;			// 前回の壁擦り状況の取得処理

	void SetRightLeave(const bool bRightLeave);		// 壁離れ時の向き設定処理
	bool GetRightLeave(void) const;					// 壁離れ時の向き取得処理

	void SetRightKick(const bool bRight);			// 右壁を蹴ったかの設定処理

	void SetPosObject(const D3DXVECTOR3& pos);		// 関与しているオブジェクトの位置の設定処理
	D3DXVECTOR3 GetPosObject(void) const;			// 関与しているオブジェクトの位置の取得処理

	float GetCliffGravity(void) const;		// 崖捕まり時の重力の取得処理

	void SetRotDest(const float fRot);		// 向きの移動量の設定処理

	void ActionChange(const ACT action);	// アクション変化処理

	bool GetCliffCatch(void) const;			// 崖登り成功判定の取得処理

	int GetActCount(void) const;			// アクションカウントの取得処理

private:

	// メンバ関数
	void ActNone(CPlayer& player);				// 通常行動処理
	void ActSuperJump(CPlayer& player);			// 超ジャンプ行動処理
	void ActWallFriction(CPlayer& player);		// 壁擦り行動処理
	void ActWallLeave(CPlayer& player);			// 壁離れ行動処理
	void ActWallStepIn(CPlayer& player);		// 壁踏み込み処理
	void ActWallKick(CPlayer& player);			// 壁キック行動処理
	void ActWallAfterGlaw(CPlayer& player);		// 壁キックの余韻行動処理
	void ActCliffCatch(CPlayer& player);		// 崖捕まり行動処理
	void ActCliffClimb(CPlayer& player);		// 崖降り行動処理
	void ActCliffFall(CPlayer& player);			// 崖登り行動処理
	void ActCliffLand(CPlayer& player);			// 崖登り着地行動処理
	void ActCliffAfterGlaw(CPlayer& player);	// 崖着地の余韻行動処理
	void ActHanging(CPlayer& player);			// ぶら下がり行動処理
	void ActLampJump(CPlayer& player);			// ランプから飛んでいる状態処理
	void ActPoleDown(CPlayer& player);			// ポール下り状態処理

	void SuperJumpEffect(CPlayer& player);	// 超ジャンプ中のエフェクト生成処理
	void PlayerSwing(CPlayer& player);		// プレイヤーのスイング処理
	void WallKickRight(CPlayer& player);	// 壁キック(右版)
	void WallKickLeft(CPlayer& player);		// 壁キック(左版)
	void WallKickProcess(CPlayer& player);	// 壁キック共通の処理

	// メンバ変数
	D3DXVECTOR3 m_posObject;	// 関与しているオブジェクトの位置
	ACT m_Action;				// 行動
	int m_nActCount;			// 行動カウント
	float m_fCliffUpSpeed;		// 崖登りの速度
	float m_fCliffGravity;		// 崖上がり時の重力
	float m_fRotDest;			// 目標の向き
	bool m_bRightLeave;			// 壁離れしたときに右を向いていたかどうか
	bool m_bRightKick;			// 右壁を蹴ったかどうか
	bool m_bWallOld;			// 前回壁擦りしていたかどうか
	bool m_bCliffSuccess;		// 崖登りに失敗したかどうか
};

#endif