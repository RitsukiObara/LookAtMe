//============================================
//
// 吹き出しUIヘッダー[balloonUI.h]
// Author：小原立暉
//
//============================================
#ifndef _BALLOONUI_H_			// このマクロ定義がされていなかったら
#define _BALLOONUI_H_			// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"
#include "balloon.h"

//--------------------------------------------
// クラス定義(2D吹き出し)
//--------------------------------------------
class CBalloonUI : public CObject2D
{
public:				// 誰でもアクセスできる

	CBalloonUI();			// コンストラクタ
	~CBalloonUI();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);		// 情報の設定処理

	// セット・ゲット関係
	void SetType(const CBalloon::TYPE nType);		// 種類の設定処理
	CBalloon::TYPE GetType(void) const;				// 種類の取得処理

	// 静的メンバ関数
	static CBalloonUI* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CBalloon::TYPE m_type;	// 種類
};

#endif