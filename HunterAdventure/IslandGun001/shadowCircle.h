//===================================
//
// 丸影ヘッダー[shadowCircle.h]
// Author 小原立暉
//
//===================================
#ifndef _SHADOWCIRCLE_H_				//このマクロ定義がされていなかったら
#define _SHADOWCIRCLE_H_				//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"
#include "object3D.h"

//-----------------------------------
// クラス定義(影)
//-----------------------------------
class CShadowCircle : public CObject3D
{
public:			// 誰でもアクセスできる

	CShadowCircle();			// コンストラクタ
	~CShadowCircle();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新処理
	void Draw(void) override;			// 描画処理

	void SetData(const D3DXVECTOR3& pos, const float fRadius, D3DXVECTOR3* posParent);		// 情報の設定処理
	void Distance(void);		// 透明度の設定処理

	// 静的メンバ関数
	static CShadowCircle* Create(const D3DXVECTOR3& pos, const float fRadius, D3DXVECTOR3* posParent);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3* m_posParent;	// 親の位置
	D3DXCOLOR m_col;			// 色
	float m_fRadiusInit;		// 初期の半径
};


#endif