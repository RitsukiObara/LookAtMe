//===================================
//
// パーティクルヘッダー[Particle.h]
// Author 小原立暉
//
//===================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Effect.h"

//-----------------------------------
// クラス定義(CParticle)
//-----------------------------------
class CParticle
{
public:			// 誰でもアクセス出来る

	CParticle();							// コンストラクタ
	~CParticle();							// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ関数
	static void Create(const D3DXVECTOR3& pos, const CEffect::TYPE type);		// 生成処理

private:		// 自分だけアクセスできる

	// パーティクル発生処理
	void InitFire(const D3DXVECTOR3& pos);				// 炎系の生成
	void InitItemGet(const D3DXVECTOR3& pos);			// アイテム獲得の生成
	void InitCatch(const D3DXVECTOR3& pos);				// 掴み時(埃舞い)の生成
	void InitBigJumpCharge(const D3DXVECTOR3& pos);		// 超ジャンプのチャージの生成
	void InitLandDust(const D3DXVECTOR3& pos);			// 着地煙の生成
};

#endif