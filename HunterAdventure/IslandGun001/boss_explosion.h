//===================================
//
// ボスの爆発ヘッダー[boss_explosion.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_EXPLOSION_H_
#define _BOSS_EXPLOSION_H_

//***********************************
// インクルードファイル
//***********************************
#include "object.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CMeshSphere;			// メッシュスフィア
class CMeshDonut;			// メッシュドーナツ

//-----------------------------------
// クラス定義(ボスの爆発)
//-----------------------------------
class CBossExplosion : public CObject
{
public:			// 誰でもアクセスできる

	CBossExplosion();		// コンストラクタ
	~CBossExplosion();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// 情報の設定処理

	// 静的メンバ関数
	static CBossExplosion* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// 生成処理

	// 定数定義
	static const int NUM_SHOCK = 2;		// 衝撃の総数

private:		// 自分だけアクセスできる

	// メンバ変数
	CMeshSphere* m_pExplosion;			// 爆発
	CMeshDonut* m_apShock[NUM_SHOCK];	// 衝撃

	float m_fAddExpl;		// 爆発の追加量
	float m_fAddShock;		// 衝撃の追加量
	float m_fAlpha;			// 透明度
};

#endif