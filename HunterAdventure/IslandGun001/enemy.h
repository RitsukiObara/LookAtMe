//===================================
//
// 敵ヘッダー[enemy.h]
// Author 小原立暉
//
//===================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//***********************************
// インクルードファイル
//***********************************
#include "character.h"
#include "list_manager.h"

// 前方宣言
class CMotion;		// モーション

//-----------------------------------
// クラス定義(CEnemy)
//-----------------------------------
class CEnemy : public CCharacter
{
public:					// 誰でもアクセスできる

	// 列挙型定義(モーションの種類)
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		// 通常モーション
		MOTIONTYPE_MOVE,			// 移動モーション
		MOTIONTYPE_MAX				// この列挙型の総数
	};

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_TORDLE = 0,	// 地上を歩く敵(タードル)
		TYPE_IWAKARI,		// 防御力が高い敵(イワカリ)
		TYPE_MAX			// この列挙型の総数
	};

	// 列挙型定義(共通の状態)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_MAX			// この列挙型の総数
	};

	CEnemy();				// コンストラクタ
	virtual ~CEnemy();		// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) = 0;			// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	virtual void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);		// 情報の設定処理
	virtual void Hit(const int nDamage, const float fKnockback) = 0;	// ヒット処理
	void Death(void);						// 死亡時の処理

	// セット・ゲット関係
	CMotion* GetMotion(void) const;			// モーションの取得処理
	void CreateMotion(void);				// モーションの生成処理

	void SetState(const STATE state);		// 状態の設定処理
	STATE GetState(void) const;				// 状態の取得処理

	void SetCollSize(const D3DXVECTOR3& size);		// 当たり判定のサイズの設定処理
	D3DXVECTOR3 GetCollSize(void) const;			// 当たり判定のサイズの取得処理

	void SetLife(const int nLife);			// 体力の設定処理
	int GetLife(void) const;				// 体力の取得処理

	void SetGravity(const float fGravity);	// 重力の設定処理
	float GetGravity(void) const;			// 重力の取得処理

	// 静的メンバ関数
	static CEnemy* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);		// 生成処理

	static CListManager<CEnemy*> GetList(void);			// リストの取得処理

private:				// 自分だけアクセスできる

	// メンバ関数
	void ElevationCollision(void);	// 起伏地面との当たり判定
	void TreeCollision(void);		// 木との当たり判定
	void RockCollision(void);		// 岩との当たり判定
	void BlockCollision(void);		// ブロックとの当たり判定
	void WallCollision(void);		// 壁との当たり判定
	void SlashRippleHit(void);		// 斬撃の波紋との当たり判定

	// メンバ変数
	CMotion* m_pMotion;		// モーションの情報

	TYPE m_type;			// 種類
	STATE m_state;			// 状態
	D3DXVECTOR3 m_collSize;	// 当たり判定のサイズ
	int m_nLife;			// 体力
	int m_nStateCount;		// 状態カウント
	float m_fGravity;		// 重力

	// 静的メンバ変数
	static CListManager<CEnemy*> m_list;		// リスト
};

#endif