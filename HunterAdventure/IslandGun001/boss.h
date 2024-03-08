//===================================
//
// ボスヘッダー[boss.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_H_
#define _BOSS_H_

//***********************************
// インクルードファイル
//***********************************
#include "character.h"
#include "list_manager.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CMotion;			// モーション
class CBossState;		// ボスの状態
class CBossLifeUI;		// ボスの体力UI
class CBossCollision;	// ボスの当たり判定

//-----------------------------------
// クラス定義(CBoss)
//-----------------------------------
class CBoss : public CCharacter
{
public:					// 誰でもアクセスできる

	// 列挙型定義(モーションの種類)
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		// 通常モーション
		MOTIONTYPE_FLY,				// 飛行モーション
		MOTIONTYPE_HOVERING,		// ホバリングモーション
		MOTIONTYPE_LANDING,			// 着地モーション
		MOTIONTYPE_HOWLING,			// 雄たけびモーション
		MOTIONTYPE_FLYING,			// 飛行モーション
		MOTIONTYPE_CHARGE,			// チャージモーション
		MOTIONTYPE_DOWN,			// ダウンモーション
		MOTIONTYPE_STOMP,			// 足踏みモーション
		MOTIONTYPE_DEATH,			// 死亡モーション
		MOTIONTYPE_MAX				// この列挙型の総数
	};

	// 弱点のパーツ
	enum WEAKPART
	{
		WEAK_BODY = 0,				// 体
		WEAK_RFSHIN,				// 右前脛
		WEAK_LFSHIN,				// 左前脛
		WEAK_RBSHIN,				// 右後ろ脛
		WEAK_LBSHIN,				// 左後ろ脛
		WEAK_MAX					// この列挙型の総数
	};

	// ダメージ状況関係
	struct SDamage
	{
		int nCount;		// カウント
		bool bDamage;	// ダメージ状況
	};

	CBoss();		// コンストラクタ
	~CBoss();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void Hit(const int nDamage);	// ヒット処理
	void BarrierBreak(const D3DXVECTOR3& pos, const int nPart, const int nDamage);		// バリア破壊処理
	void BarrierHit(const D3DXVECTOR3& pos, const int nPart, const int nCntPart);		// バリアのヒット処理
	void StunRecovery(void);		// 気絶回復処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// 情報の設定処理

	bool ElevationCollision(void);					// 起伏地面との当たり判定
	void ChangeState(CBossState* pNext);			// 状態の遷移処理

	// セット・ゲット関係
	CMotion* GetMotion(void);		// モーションの取得処理
	CBossCollision* GetColl(const int nIdx);		// 当たり判定の取得処理

	void SetEnableHit(const bool bHit);		// ヒット状況の設定処理
	bool IsHit(void) const;					// ヒット状況の取得処理

	// 静的メンバ関数
	static CBoss* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// 生成処理

	static CListManager<CBoss*> GetList(void);			// リストの取得処理

private:				// 自分だけアクセスできる

	// メンバ関数
	void EnemyAllClear(void);	// 敵の全消去処理
	void Damage(void);			// ダメージ処理

	// メンバ変数
	CMotion* m_pMotion;		// モーションの情報
	CBossState* m_pState;	// 状態の情報
	CBossLifeUI* m_pLifeUI;	// 体力UIの情報
	CBossCollision* m_apColl[MAX_PARTS];	// 当たり判定の球
	D3DXMATERIAL* m_apMatCopy[MAX_PARTS];	// マテリアルのコピー

	SDamage m_damage;		// ダメージ情報
	int m_aWeakPointLife[WEAK_MAX];			// 弱点のライフ
	int m_nLife;			// 体力
	bool m_bDown;			// ダウン状況
	bool m_bHit;			// ヒット状況

	// 静的メンバ変数
	static CListManager<CBoss*> m_list;		// リスト
};

#endif