//===================================
//
// 爆弾ヘッダー[bomb.h]
// Author 小原立暉
//
//===================================
#ifndef _BOMB_H_
#define _BOMB_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CBombFuse;		// 導火線

//-----------------------------------
// クラス定義(爆弾)
//-----------------------------------
class CBomb : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_GROWTH = 0,		// 成長状態
		STATE_RIPEN,			// 実り状態
		STATE_BOUND,			// バウンド状態
		STATE_DETONATION,		// 起爆状態
		STATE_EXPLOSION,		// 爆発状態
		STATE_MAX				// この列挙型の総数
	};

	CBomb();			// コンストラクタ
	~CBomb();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void Hit(const float fRot, const float fSpeed);		// ヒット処理
	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);		// 状態の設定処理
	STATE GetState(void) const;				// 状態の取得処理

	// 静的メンバ関数
	static CBomb* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// 生成処理

	static CListManager<CBomb*> GetList(void);			// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void RotMove(void);				// 向きの移動処理
	void Move(void);				// 移動処理
	void Gravity(void);				// 重力処理
	bool ElevationCollision(void);	// 起伏地面の当たり判定処理
	void Collision(void);			// 当たり判定処理

	// 各状態処理
	void Growth(void);				// 成長状態処理
	void Bound(void);				// バウンド状態処理
	void Detonation(void);			// 起爆状態処理
	bool Explosion(void);			// 爆発状態処理

	// メンバ変数
	CBombFuse* m_pFuse;			// 導火線の情報
	D3DXCOLOR m_col;			// 色
	D3DXVECTOR3 m_move;			// 移動量
	STATE m_state;				// 状態
	int m_nBoundCount;			// バウンドカウント
	int m_nExplosionCount;		// 爆発カウント
	bool m_bAdd;				// 加算状況

	// 静的メンバ変数
	static CListManager<CBomb*> m_list;		// リスト
};

#endif