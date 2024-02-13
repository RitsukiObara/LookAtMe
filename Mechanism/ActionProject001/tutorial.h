//===================================
//
// チュートリアルふきだしヘッダー[tutorial.h]
// Author 小原立暉
//
//===================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"
#include "pork.h"

//-----------------------------------
// クラス定義(チュートリアル)
//-----------------------------------
class CTutorial : public CBillboard
{
public:			// 誰でもアクセスできる

	CTutorial();			// コンストラクタ
	~CTutorial();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const CPork::TYPE type);				// 情報の設定処理

	// 静的メンバ関数
	static CTutorial* Create(const D3DXVECTOR3& pos, const CPork::TYPE type);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CPork::TYPE m_type;		// 種類
};

#endif