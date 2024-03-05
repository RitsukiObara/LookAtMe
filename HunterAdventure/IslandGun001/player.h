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

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define NUM_HANDGUN		(2)		// 拳銃の数

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CMotion;				// モーション
class CPlayerAction;		// プレイヤーの行動
class CHandgun;				// 拳銃
class CDagger;				// ダガー
class CAim;					// エイム
class CBulletUI;			// 弾丸UI
class CGoldBoneUI;			// 金の骨UI
class CLifeUI;				// 寿命UI
class CPlayerController;	// プレイヤーコントローラー
class CAirplane;			// 飛行機
class CShadowCircle;		// 丸影

//--------------------------------------------
// クラス(プレイヤークラス)
//--------------------------------------------
class CPlayer : public CCharacter
{
public:			// 誰でもアクセスできる

	// 列挙型定義(モーションの種類)
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		// 待機
		MOTIONTYPE_MOVE,			// 移動
		MOTIONTYPE_DODGE,			// 回避
		MOTIONTYPE_DAGGER,			// ダガー
		MOTIONTYPE_SWOOP,			// 急降下
		MOTIONTYPE_DAMAGE,			// ダメージ
		MOTIONTYPE_AIRDIVE,			// 飛行機降り
		MOTIONTYPE_DEATH,			// 死亡
		MOTIONTYPE_MAX				// この列挙型の総数
	};

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,				// 通常状態
		STATE_DAMAGE,				// ダメージ状態
		STATE_INVINSIBLE,			// 無敵状態
		STATE_DEATH,				// 死亡状態
		STATE_MAX					// この列挙型の総数
	};

	// 構造体定義(状態関連)
	struct SState
	{
		D3DXCOLOR col;		// 体の色(状態によって変わる)
		STATE state;		// 状態
		int nCount;			// 状態カウント
	};

	CPlayer();				// コンストラクタ
	~CPlayer();				// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	void Hit(const int nDamage, const float fRotSmash);		// ヒット処理
	void Healing(const int nHealing);			// 回復処理
	void SetData(const D3DXVECTOR3& pos);		// 情報の設定処理
	void ArrivalAirplane(void);					// 飛行機の到着処理

	// セット・ゲット関係
	CMotion* GetMotion(void) const;					// モーションの情報の取得処理
	CPlayerAction* GetAction(void) const;			// 行動の情報の取得処理
	CHandgun* GetHandGun(const int nCount) const;	// 拳銃の情報の取得処理
	CDagger* GetDagger(void) const;					// ダガーの情報の取得処理
	CAim* GetAim(void) const;						// エイムの取得処理
	CBulletUI* GetBulletUI(void) const;				// 残弾UIの情報の取得処理
	CGoldBoneUI* GetGoldBoneUI(void) const;			// 金の骨UIの情報の取得処理
	CLifeUI* GetLifeUI(void) const;					// 寿命UIの情報の取得処理
	CPlayerController* GetController(void) const;	// コントローラーの情報の取得処理
	CAirplane* GetAirplane(void) const;				// 飛行機の情報の取得処理
	void DeleteAirplane();							// 飛行機のNULL化処理

	void SetRotDest(const D3DXVECTOR3& rot);	// 目的の向きの設定処理
	D3DXVECTOR3 GetRotDest(void) const;			// 目的の向きの取得処理
	void SetMove(const D3DXVECTOR3& move);		// 移動量の設定処理
	D3DXVECTOR3 GetMove(void) const;			// 移動量の取得処理
	void SetLife(const int nLife);				// 体力の設定処理
	int GetLife(void) const;					// 体力の取得処理
	void SetState(const SState state);			// 状態の設定処理
	SState GetState(void) const;				// 状態の取得処理
	void SetEnableJump(const bool bJump);		// ジャンプ状況の設定処理
	bool IsJump(void) const;					// ジャンプ状況の取得処理
	void SetAreaIdx(const int nIdx);			// 区分の番号の設定処理
	int GetAreaIdx(void) const;					// 区分の番号の取得処理

	// 静的メンバ関数
	static CPlayer* Create(const D3DXVECTOR3& pos);	// 生成処理

protected:		// 自分と派生クラスだけアクセスできる

	// メンバ関数
	void StateManager(void);		// 状態管理処理
	void Move(void);				// 移動処理
	void EmergentReload(void);		// 緊急のリロード処理
	void ShadowPosSet(void);		// 影の位置設定処理

	void ElevationCollision(void);	// 起伏地面の当たり判定処理
	void Collision(void);			// 当たり判定処理
	void BlockCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx);		// ブロックとの当たり判定
	void AlterCollision(void);		// 祭壇との当たり判定
	void StageCollision(void);		// ステージとの当たり判定

private:		// 自分だけアクセスできる

	// 状態ごとの処理
	void DamageState();				// ダメージ状態処理
	void InvisibleState();			// 無敵状態処理

	// メンバ変数
	CMotion* m_pMotion;					// モーションの情報
	CPlayerAction* m_pAction;			// プレイヤーの行動の情報
	CHandgun* m_apHandGun[NUM_HANDGUN];	// 拳銃の情報
	CDagger* m_pDagger;					// ダガーの情報
	CAim* m_pAim;						// エイムの情報
	CBulletUI* m_pBulletUI;				// 弾丸の情報
	CGoldBoneUI* m_pGoldBoneUI;			// 金の骨UIの情報
	CLifeUI* m_pLifeUI;					// 寿命UIの情報
	CPlayerController* m_pController;	// プレイヤーのコントローラーの情報
	CAirplane* m_pAirplane;				// 飛行機の情報
	CShadowCircle* m_pShadow;			// 丸影の情報

	D3DXVECTOR3 m_rotDest;		// 目標の向き
	D3DXVECTOR3 m_move;			// 移動量
	SState m_stateInfo;			// 状態関連の構造体
	int m_nLife;				// 体力
	int m_nAreaIdx;				// 区分の番号
	bool m_bMove;				// 移動状況
	bool m_bJump;				// ジャンプ状況

};

#endif