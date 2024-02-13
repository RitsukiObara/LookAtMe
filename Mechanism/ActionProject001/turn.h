//============================================
//
// 振り向きヘッダー[turn.h]
// Author：小原立暉
//
//============================================
#ifndef _TURN_H_			//このマクロ定義がされていなかったら
#define _TURN_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// クラス(振り向きクラス)
//--------------------------------------------
class CTurn : public CBillboardAnim
{
public:			// 誰でもアクセスできる

	CTurn();				// コンストラクタ
	~CTurn();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const bool bRight);				// 設定処理

	// 静的メンバ関数
	static CTurn* Create(const D3DXVECTOR3& pos, const bool bRight);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nLife;			// 寿命
	bool m_bRight;			// 右向き状況
};

#endif