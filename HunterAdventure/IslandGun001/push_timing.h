//============================================
//
// 押すタイミング表示ヘッダー[push_timing.h]
// Author：小原立暉
//
//============================================
#ifndef _PUSH_TIMING_H_			//このマクロ定義がされていなかったら
#define _PUSH_TIMING_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// クラス(押すタイミングクラス)
//--------------------------------------------
class CPushTiming : public CBillboardAnim
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_PAD_A = 0,		// Aボタン
		TYPE_KEY_SPACE,		// SPACEキー
		TYPE_MAX			// この列挙型の総数
	};

	CPushTiming();			// コンストラクタ
	~CPushTiming();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const float fSize, const TYPE type, const int nInterval);					// 設定処理

	// セット・ゲット関係
	void SetType(const TYPE type);			// 種類の設定処理
	void SetMagni(const float fSize);		// サイズの倍率の設定処理
	float GetMagni(void) const;				// サイズの倍率の取得処理
	void SetEnableDisp(const bool bDisp);	// 描画状況の設定処理
	bool IsDisp(void) const;				// 描画状況の取得処理

	// 静的メンバ関数
	static CPushTiming* Create(const D3DXVECTOR3& pos, const float fSize, const TYPE type, const int nInterval);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	float m_fSize;				// サイズの倍率
	bool m_bDisp;				// 描画状況
};

#endif