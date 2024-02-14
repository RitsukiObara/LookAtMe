//===================================
//
// 風攻撃ヘッダー[wind_shot.h]
// Author 小原立暉
//
//===================================
#ifndef _WIND_SHOT_H_
#define _WIND_SHOT_H_

//***********************************
// インクルードファイル
//***********************************
#include "mesh_tornado.h"
#include "list_manager.h"

//-----------------------------------
// クラス定義(風攻撃)
//-----------------------------------
class CWindShot : public CMeshTornado
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_ATTACK = 0,		// 攻撃状態
		STATE_DELETE,			// 消去処理
		STATE_MAX				// この列挙型の総数
	};

	CWindShot();				// コンストラクタ
	~CWindShot();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);					// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);		// 状態の設定処理
	STATE GetState(void) const;				// 状態の取得処理

	// 静的メンバ関数
	static CWindShot* Create(const D3DXVECTOR3& pos);		// 生成処理

	static CListManager<CWindShot*> GetList(void);			// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void ElevationCollision(void);		// 起伏地面との当たり判定
	void RotMove(void);					// 向き移動処理
	void Attack(void);					// 攻撃状態処理

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	STATE m_state;			// 状態
	float m_fRotMove;		// 移動方向
	float m_fAlpha;			// 透明度

	// 静的メンバ変数
	static CListManager<CWindShot*> m_list;		// リスト
};

#endif