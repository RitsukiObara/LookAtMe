//============================================
//
// スタミナUIヘッダー[staminaUI.h]
// Author：小原立暉
//
//============================================
#ifndef _STAMINAUI_H_					// このマクロ定義がされていなかったら
#define _STAMINAUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"
#include "scene.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define STAMINAUI_MARK_SIZE			(D3DXVECTOR3(50.0f, 50.0f, 0.0f))		// スタミナUIのマークのサイズ

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;			// オブジェクト2D
class CAnim;				// アニメーション

//--------------------------------------------
// クラス定義(ゲームのスタミナUI)
//--------------------------------------------
class CStaminaUI : public CObject
{
public:				// 誰でもアクセスできる

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_NONE = 0,		// 通常状態
		STATE_HEAL,			// 回復状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	// 列挙型定義(スタミナUIのポリゴン)
	typedef enum
	{
		TYPE_GAGE = 0,		// ゲージ
		TYPE_METER,			// メーター
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	// 列挙型定義(テクスチャ)
	typedef enum
	{
		TEX_GAGE = 0,		// ゲージ
		TEX_METER,			// メーター
		TEX_MARK,			// マーク
		TEX_MAX				// この列挙型の総数
	}TEX;

	CStaminaUI();				// コンストラクタ
	CStaminaUI(CObject::TYPE type, PRIORITY priority = PRIORITY_UI);		// オーバーロードコンストラクタ
	virtual ~CStaminaUI();		// デストラクタ
	void Box(void);				// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	virtual void SetData(void) = 0;			// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);		// 状態の設定処理
	STATE GetState(void) const;				// 状態の取得処理

	CObject2D* GetObject2D(const TYPE type) const;		// オブジェクト2Dの取得処理
	CAnim* GetAnim(void) const;							// アニメーションの取得処理
	
	// 静的メンバ関数
	static CStaminaUI* Create(const CScene::MODE mode);		// 生成処理

private:			// 自分だけアクセスできる

	// スタミナ反映処理
	void StaminaInfluence(void);		// スタミナ反映処理
	void SizeSub(void);					// サイズの減算処理
	void CalcStaminaSub(void);			// スタミナの減算量計算処理
	void ColorCalc(void);				// スタミナのカラー計算処理
	void NoneStateProcess(void);		// 通常状態の処理
	void HealStateProcess(void);		// 回復状態の処理

	// メンバ変数
	CObject2D* m_apObject2D[TYPE_MAX];		// オブジェクトの情報
	CAnim* m_pAnim;							// アニメーションの情報
	float m_fStaminaDest;					// 目標のスタミナ
	float m_fSubStamina;					// スタミナの減算量
	int m_nFlashCount;						// 点滅カウント
	int m_nStateCount;						// 状態カウント
	STATE m_state;							// 状態
	bool m_bFlash;							// 点滅状況

	// 静的メンバ変数
	static const char* m_apTexturename[TEX_MAX];	// テクスチャのパス名
};

#endif