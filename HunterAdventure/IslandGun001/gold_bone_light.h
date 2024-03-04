//===================================
//
// 金の骨の光ヘッダー[gold_bone_light.h]
// Author 小原立暉
//
//===================================
#ifndef _GOLD_BONE_LIGHT_H_
#define _GOLD_BONE_LIGHT_H_

//***********************************
// インクルードファイル
//***********************************
#include "mesh_cylinder.h"

//-----------------------------------
// クラス定義(金の骨の光)
//-----------------------------------
class CGoldBoneLight : public CMeshCylinder
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_EXTEND = 0,		// 拡大状態
		STATE_SHRINK,			// 縮小状態
		STATE_MAX				// この列挙型の総数
	};

	CGoldBoneLight();			// コンストラクタ
	~CGoldBoneLight();			// デストラクタ

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
	static CGoldBoneLight* Create(const D3DXVECTOR3& pos);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Extend(void);		// 伸び状態処理
	void Shrink(void);		// 縮小状態処理
	void SetTex(void);		// テクスチャ座標の設定処理

	// メンバ変数
	STATE m_state;			// 状態
};

#endif