//============================================
//
// 寿命UIヘッダー[lifeUI.h]
// Author：小原立暉
//
//============================================
#ifndef _LIFE_UI_H_					// このマクロ定義がされていなかったら
#define _LIFE_UI_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;		// 2Dポリゴン

//--------------------------------------------
// クラス定義(寿命UI)
//--------------------------------------------
class CLifeUI : public CObject
{
public:				// 誰でもアクセスできる

	// 列挙型定義(ポリゴン)
	enum POLY
	{
		POLY_GROUND = 0,	// 下地
		POLY_METER,			// メーター
		POLY_WIPE,			// ワイプ
		POLY_MAX			// この列挙型の総数
	};

	// 構造体定義(アイコンの点滅)
	struct SIconFlash
	{
		D3DXCOLOR col;		// アイコンの色
		bool bAdd;			// 加算状況
	};

	CLifeUI();				// コンストラクタ
	~CLifeUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const int nLife);	// 情報の設定処理

	// セット・ゲット関係
	void SetLife(const int nLife);	// 寿命の設定処理
	int GetLife(void) const;		// 寿命の取得処理

	// 静的メンバ関数
	static CLifeUI* Create(const int nLife);	// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void IconFlash(void);		// アイコンの点滅処理

	// メンバ変数
	CObject2D* m_apObject2D[POLY_MAX];		// ポリゴンの情報
	SIconFlash m_iconflash;		// アイコンの点滅関係
	int m_nLife;				// 寿命
	float m_fMeterDest;			// 目的のメーター
	float m_fMeter;				// メーター
};

#endif