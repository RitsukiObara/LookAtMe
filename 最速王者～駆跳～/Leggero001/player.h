//============================================
//
// プレイヤーヘッダー[player.h]
// Author：小原立暉
//
//============================================
#ifndef _PLAYER_H_					// このマクロ定義がされていなかったら
#define _PLAYER_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "character.h"
#include "scene.h"
#include "playerAct.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CPlayerAct;			// プレイヤーの情報
class CMotion;				// モーション
class COrbit;				// 軌跡
class CStreetLamp;			// 街灯(光源)
class CSonic;				// 高速演出
class CSpeedUI;				// 速度UI
class CStaminaUI;			// スタミナUI

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define PLAYER_CLIFF_ADD_SPEED		(1.0f)								// 崖登り時の速度の加算量
#define PLAYER_CLIFF_SUB_SPEED		(0.4f)								// 崖登り時の速度の減算量
#define PLAYER_WALLKICK_ADD_SPEED	(0.05f)								// 壁キック時の速度の加算数
#define PLAYER_BIGJUMP_ADD_SPEED	(0.1f)								// 大ジャンプ時の速度の加算数
#define PLAYER_ACTION_ADD_SPEED		(0.01f)								// アクション時の速度の加算数
#define PLAYER_LAMPJUMP_ADD_SPEED	(0.05f)								// 街灯ジャンプ時の速度の加算数
#define PLAYER_WIDTH				(20.0f)								// プレイヤーの横幅
#define PLAYER_HEIGHT				(85.0f)								// プレイヤーの縦幅
#define PLAYER_COLLISION_WIDTH		(PLAYER_WIDTH + 0.01f)				// プレイヤーの横幅
#define PLAYER_COLLISION_HEIGHT		(PLAYER_HEIGHT + 0.01f)				// プレイヤーの縦幅
#define PLAYER_RADIUS				(30.0f)								// プレイヤーの半径
#define PLAYER_MAX_STAMINA			(100.0f)							// プレイヤーのスタミナの初期値
#define PLAYER_INIT_SPEED			(7.0f)								// プレイヤーの初期移動量
#define PLAYER_MAX_SPEED			(13.0f)								// プレイヤーの移動量の最大値
#define PLAYER_MIN_SPEED			(PLAYER_INIT_SPEED)					// プレイヤーの移動量の最小値
#define PLAYER_GRAVITY				(0.55f)								// プレイヤーの重力
#define PLAYER_WALLKICK_GRAVITY		(0.6f)								// プレイヤーの壁キック時の重力
#define PLAYER_LAND_STAGING_GRAVITY	(-19.0f)							// プレイヤーの着地演出の出る重力

#define PLAYER_JUMP					(13.5f)								// プレイヤーのジャンプ力
#define PLAYER_BIGJUMP				(20.0f)								// プレイヤーの超ジャンプ力

// ボタンカウント関係
#define PLAYER_BIGJUMP_BTNCOUNT		(15)		// 超ジャンプ行動になるまでのカウント数
#define PLAYER_WALLKICK_BTNCOUNT	(1)			// 壁キック行動になるまでのカウント数
#define PLAYER_ATTACK_BTNCOUNT		(1)			// 攻撃行動になるまでのカウント数
#define PLAYER_CLIFFLAND_BTNCOUNT	(1)			// 崖着地行動になるまでのカウント数
#define PLAYER_LAMPJUMP_BTNCOUNT	(1)			// 街灯ジャンプになるまでのカウント数

// デバッグ用
#ifdef _DEBUG

// スタミナ関係
#define PLAYER_BIGJUMP_STAMINA			(20.0f)		// 2段ジャンプ時に減らすスタミナ
#define PLAYER_WALLKICK_STAMINA			(3.0f)		// 壁キック時に減らすスタミナ
#define PLAYER_CLIFFLAND_STAMINA		(5.0f)		// 崖着地時に減らすスタミナ
#define PLAYER_AUTO_HEAL_STAMINA_STOP	(0.10f)		// 止まっている時の自動回復スタミナ数
#define PLAYER_AUTO_HEAL_STAMINA_MOVE	(0.03f)		// 動いている時の自動回復スタミナ数

#else

// スタミナ関係
#define PLAYER_BIGJUMP_STAMINA			(20.0f)		// 2段ジャンプ時に減らすスタミナ
#define PLAYER_WALLKICK_STAMINA			(2.0f)		// 壁キック時に減らすスタミナ
#define PLAYER_CLIFFLAND_STAMINA		(5.0f)		// 崖着地時に減らすスタミナ
#define PLAYER_AUTO_HEAL_STAMINA_STOP	(0.15f)		// 止まっている時の自動回復スタミナ数
#define PLAYER_AUTO_HEAL_STAMINA_MOVE	(0.05f)		// 動いている時の自動回復スタミナ数

// コピー
//#define PLAYER_BIGJUMP_STAMINA		(20.0f)		// 2段ジャンプ時に減らすスタミナ
//#define PLAYER_WALLKICK_STAMINA		(2.0f)		// 壁キック時に減らすスタミナ
//#define PLAYER_CLIFFLAND_STAMINA		(5.0f)		// 崖着地時に減らすスタミナ
//#define PLAYER_AUTO_HEAL_STAMINA_STOP	(0.15f)		// 止まっている時の自動回復スタミナ数
//#define PLAYER_AUTO_HEAL_STAMINA_MOVE	(0.05f)		// 動いている時の自動回復スタミナ数

#endif

//--------------------------------------------
// クラス(プレイヤークラス)
//--------------------------------------------
class CPlayer : public CCharacter
{
public:			// 誰でもアクセスできる

	//************************************************************
	//	列挙型定義(モーションの種類)
	//************************************************************
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,		// 待機
		MOTIONTYPE_MOVE,			// 移動
		MOTIONTYPE_JUMP,			// ジャンプ
		MOTIONTYPE_WALLFRIC,		// 壁擦り状態
		MOTIONTYPE_WALLSTEPIN,		// 踏み込み
		MOTIONTYPE_WALLKICK,		// 壁キック
		MOTIONTYPE_BIGJUMP,			// 超ジャンプ
		MOTIONTYPE_CLIFFCATCH,		// 崖捕まり
		MOTIONTYPE_CLIFFUP,			// 崖上がり
		MOTIONTYPE_HANGING,			// ぶら下がり
		MOTIONTYPE_LANDING,			// 着地モーション
		MOTIONTYPE_LAMPJUMP,		// 街灯ジャンプ
		MOTIONTYPE_POLEDOWN,		// ポール下り
		MOTIONTYPE_FALL,			// 落下
		MOTIONTYPE_START,			// スタート前
		MOTIONTYPE_ASCENT,			// よじ登り
		MOTIONTYPE_ATTACK,			// 攻撃
		MOTIONTYPE_MAX				// この列挙の総数
	}MOTIONTYPE;

	//--------------------------------------------
	// 状態
	//--------------------------------------------
	typedef enum
	{
		STATE_NONE = 0,		// 通常状態
		STATE_BOOST,		// ブースト状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_INVINCIBLE,	// 無敵状態
		STATE_CRUSH,		// 圧殺状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	//--------------------------------------------
	// 軌跡のテクスチャ
	//--------------------------------------------
	typedef enum
	{
		ORBIT_TEX_NORMAL = 0,		// 通常の軌跡
		ORBIT_TEX_POLE,				// ポールの軌跡
		ORBIT_TEX_MAX				// この列挙型の総数
	}ORBIT_TEX;

	//--------------------------------------------
	// 構造体定義(圧死の状況)
	//--------------------------------------------
	typedef struct
	{
		D3DXVECTOR3 scaleDest;		// 目標の拡大率
		bool bTop;					// 上からの判定
		bool bBottom;				// 下からの判定
		bool bRight;				// 右からの判定
		bool bLeft;					// 左からの判定
	}SCrush;

	//--------------------------------------------
	// 構造体定義(アクションボタン関係)
	//--------------------------------------------
	typedef struct
	{
		int nButtonCount;			// ボタンを押している時間
		bool bSpacePush;			// ジャンプキーの押し状況
	}SButton;

	CPlayer();				// コンストラクタ
	CPlayer(CObject::TYPE type, PRIORITY priority = PRIORITY_PLAYER);		// オーバーロードコンストラクタ
	virtual ~CPlayer();		// デストラクタ
	void Box(void);			// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void) = 0;	// 更新処理
	virtual void Draw(void);		// 描画処理
	void Hit(void);					// ヒット処理

	virtual void SetData(const D3DXVECTOR3& pos, const CScene::MODE mode);		// 情報の設定処理

	void StartUpdate(void);		// スタート更新処理
	void CountDownUpdate(void);	// カウントダウン更新処理
	void MainUpdate(void);		// 通常更新処理
	void GoalUpdate(void);		// ゴール更新処理
	void FinishUpdate(void);	// 終了更新処理

	// セット・ゲット関係
	void SetColor(const D3DXCOLOR& col);		// 色の設定処理
	D3DXCOLOR GetColor(void) const;				// 色の取得処理

	void SetMove(const D3DXVECTOR3& move);		// 移動量の設定処理
	D3DXVECTOR3 GetMove(void) const;			// 移動量の取得処理

	void SetState(const STATE state);			// 状態の設定処理
	STATE GetState(void) const;					// 状態の取得処理

	void SetJump(const bool bJump);				// ジャンプ状況の設定処理
	bool GetJump(void) const;					// ジャンプ状況の取得処理

	void SetSpeed(float fSpeed);				// 速度の設定処理
	float GetSpeed(void) const;					// 速度の取得処理

	void SetButtonPush(const bool bPush);		// ボタン押し状況の設定処理
	void SetButtonCount(const int nCount);		// ボタンカウントの設定処理
	SButton GetButton(void);					// ボタンの取得処理

	void SetRight(const bool bRight);			// 右向き状況の設定処理
	bool GetRight(void) const;					// 右向き状況の取得処理

	void SetWallFricCount(const int nCount);	// 壁擦りカウントの設定処理

	void SetCrushTop(const bool bTop);			// 圧殺判定(上)の設定処理
	void SetCrushBottom(const bool bBottom);	// 圧殺判定(下)の設定処理
	void SetCrushRight(const bool bRight);		// 圧殺判定(右)の設定処理
	void SetCrushLeft(const bool bLeft);		// 圧殺判定(左)の設定処理

	CMotion* GetMotion(void);					// モーションの情報の設定処理

	CPlayerAct* GetAction(void);				// 行動の情報の取得処理

	COrbit* GetOrbit(void);						// 軌跡の情報の取得処理

	float GetStamina(void);						// スタミナの取得処理
	void CalcStamina(float fAdd);				// スタミナの計算処理

	void SetStreetLamp(CStreetLamp* pTarget);	// 街灯の設定処理
	CStreetLamp* GetStreetLamp(void) const;		// 街灯の取得処理

	CSpeedUI* GetSpeedUI(void);					// 速度UIの取得処理

	CStaminaUI* GetStaminaUI(void);				// スタミナUIの取得処理

	// 静的メンバ関数
	static CPlayer* Create(const D3DXVECTOR3& pos, const CScene::MODE mode);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Control(void);			// 操作処理
	void PlayerGravity(void);	// 重力処理
	void ActionControl(void);	// アクションボタンの操作処理
	void Move(void);			// 移動処理
	void MoveProcess(void);		// 移動の共通の処理
	void SpeedStarEffect(void);	// 速度の星の発生処理
	void RunDustEffect(void);	// 走った後の埃の発生処理
	void Jump(void);			// ジャンプ処理
	void State(void);			// 状態処理
	void WallKick(bool bRight, bool bLeft);	// 壁キック処理
	void StageCollision(bool* pRight, bool* pLeft);		// ステージの当たり判定
	void StagePenetration(void);			// めり込み判定処理
	bool SignPoleCollision(void);			// 標識の当たり判定処理
	void GroundLanding(void);				// 地面着地処理
	void WallFriction(bool &bRight, bool &bLeft);		// 壁擦り処理
	void PlayerOrbit(void);					// 軌跡の処理
	void PlayerSonic(void);					// 高速演出の処理

	bool BlockCollision(bool* pRight, bool* pLeft);		// ブロックの当たり判定
	bool ObstacleCollision(void);						// 障害物の当たり判定
	bool CollPolygonCollision(void);					// 当たり判定ポリゴンの当たり判定

	bool WallKickJump(bool bRight, bool bLeft);	// 壁キック(ジャンプのみ判定)
	void WallKickDirect(void);					// 壁キック(方向キー付き)

	// 状態処理関係
	void StateNone(void);					// 通常状態処理
	void StateBoost(void);					// ブースト状態
	void StateDamage(void);					// ダメージ状態処理
	void StateInvincible(void);				// 無敵状態処理
	void StateCrush(void);					// 圧殺状態処理

	// メンバ変数
	CMotion* m_pMotion;				// モーションの情報
	CPlayerAct* m_pAction;			// 行動の情報
	COrbit* m_pOrbit;				// 軌跡の情報
	CStreetLamp* m_pStreetLamp;		// 街灯(光源)の情報
	CSonic* m_pSonic;				// 高速演出の情報
	CSpeedUI* m_pSpeedUI;			// 速度UIの情報
	CStaminaUI* m_pStaminaUI;		// スタミナの情報

	SCrush m_crush;					// 圧殺判定
	D3DXCOLOR m_col;				// 色
	D3DXVECTOR3 m_move;				// 移動量
	STATE m_state;					// 状態
	int m_nStateCount;				// 状態カウント
	int m_nShadowIdx;				// 影のインデックス
	int m_nWallFricCount;			// 壁擦りカウント
	int m_nJumpCount;				// ジャンプしているカウント
	int m_nOrbitIdx[ORBIT_TEX_MAX];	// 軌跡のテクスチャのインデックス
	int m_nMoveCount;				// ダッシュカウント
	float m_fSpeed;					// 走る速度
	float m_fStamina;				// スタミナ
	float m_fMoveDest;				// 目的の移動量
	SButton m_button;				// ボタンの情報
	bool m_bJump;					// ジャンプ状況
	bool m_bRight;					// 右向きかどうか
	bool m_bMove;					// 移動状況
};

#endif