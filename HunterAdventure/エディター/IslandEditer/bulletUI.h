//============================================
//
// 残弾UIヘッダー[bulletUI.h]
// Author：小原立暉
//
//============================================
#ifndef _BULLET_UI_H_					// このマクロ定義がされていなかったら
#define _BULLET_UI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_REMAINING_BULLET		(15)		// 残弾の最大値

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;		// 2Dポリゴン

//--------------------------------------------
// クラス定義(残弾UI)
//--------------------------------------------
class CBulletUI : public CObject
{
public:				// 誰でもアクセスできる

	// 構造体定義(残弾)
	struct SBullet
	{
		CObject2D* pBullet;		// 残弾
		bool bDisp;				// 描画状況
	};

	CBulletUI();				// コンストラクタ
	~CBulletUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新処理
	void Draw(void) override;			// 描画処理

	void SetData(void);					// 情報の設定処理

	// セット・ゲット関係
	void SetNumBullet(const int nNum);	// 残弾数の設定処理
	int GetNumBullet(void) const;		// 残弾数の取得処理

	// 静的メンバ関数
	static CBulletUI* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ変数
	SBullet m_aBullet[MAX_REMAINING_BULLET];	// 弾丸の情報
	CObject2D* m_pReload;		// リロードの文字
	int m_nNumBullet;		// 残弾数
};

#endif