//===================================
//
// エイムヘッダー[aim.h]
// Author 小原立暉
//
//===================================
#ifndef _AIM_H_
#define _AIM_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(CAim)
//-----------------------------------
class CAim : public CBillboard
{
public:

	CAim();			// コンストラクタ
	~CAim();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);			// 情報の設定処理

	// セット・ゲット関係
	void SetPosPlayer(const D3DXVECTOR3& pos);		// プレイヤーの位置の設定処理

	// 静的メンバ関数
	static CAim* Create(const D3DXVECTOR3& pos);	// 生成処理

private:

	// メンバ関数
	void Collision(void);			// 当たり判定処理
	bool EnemyCollision(void);		// 敵との当たり判定
	bool BalloonCollision(void);	// 風船との当たり判定

	// メンバ変数
	D3DXVECTOR3 m_posPlayer;		// プレイヤーの位置
	float m_fLength;				// 長さ
	bool m_bColl;					// 当たり判定状況
};

#endif