//============================================
//
// 爆発ヘッダー[bullet.h]
// Author：小原立暉
//
//============================================
#ifndef _EXPLOSION_H_			//このマクロ定義がされていなかったら
#define _EXPLOSION_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define EXPLOSION_SIZE		(D3DXVECTOR3(60.0f, 60.0f, 0.0f))			// 爆発のサイズ

//--------------------------------------------
// クラス(爆発クラス)
//--------------------------------------------
class CExplosion : public CBillboardAnim
{
public:			// 誰でもアクセスできる

	CExplosion();			// コンストラクタ
	~CExplosion();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);					// 設定処理

	// 静的メンバ関数
	static CExplosion* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nLife;			// 寿命
};

#endif