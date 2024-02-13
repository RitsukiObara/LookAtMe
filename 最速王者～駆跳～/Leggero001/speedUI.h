//============================================
//
// 速度UIヘッダー[speedUI.h]
// Author：小原立暉
//
//============================================
#ifndef _SPEEDUI_H_					// このマクロ定義がされていなかったら
#define _SPEEDUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"
#include "scene.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SPEEDUI_MARK_SIZE			(D3DXVECTOR3(50.0f, 50.0f, 0.0f))													// 速度UIのマークのサイズ

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;			// オブジェクト2D

//--------------------------------------------
// クラス定義(速度UI)
//--------------------------------------------
class CSpeedUI : public CObject
{
public:				// 誰でもアクセスできる

	//--------------------------------------------
	// 列挙型定義(ポリゴンの種類)
	//--------------------------------------------
	typedef enum
	{
		TYPE_GAGE = 0,		// ゲージ
		TYPE_METER,			// メーター
		TYPE_MARK,			// マーク
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	CSpeedUI();				// コンストラクタ
	CSpeedUI(CObject::TYPE type, PRIORITY priority = PRIORITY_UI);				// オーバーロードコンストラクタ
	virtual ~CSpeedUI();	// デストラクタ
	void Box(void);			// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理
	
	virtual void SetData(void) = 0;				// 情報の設定処理

	// セット・ゲット関係
	CObject2D* GetObject2D(const TYPE type);	// オブジェクトの取得処理

	// 静的メンバ関数
	static CSpeedUI* Create(const CScene::MODE mode);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void SetSpeedMeter(void);			// メーター設定処理
	void SizeCorrect(void);				// サイズの補正処理

	// メンバ変数
	CObject2D* m_apObject2D[TYPE_MAX];		// オブジェクトの情報
	float m_fLevel;							// レベル
	float m_fLevelDest;						// 目標のレベル
	int m_nOverTopTex;						// スーパーブースト時のテクスチャのインデックス
	int m_nMeterTex;						// メーターのテクスチャのインデックスの保存変数

	// 静的メンバ変数
	static const char* m_apTexturename[TYPE_MAX];	// テクスチャのパス名
};

#endif