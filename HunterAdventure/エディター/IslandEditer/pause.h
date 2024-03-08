//===========================================
//
// ポーズヘッダー[pause.h]
// Author 小原立暉
//
//===========================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//-------------------------------------------
// 前方宣言
//-------------------------------------------
class CObject2D;

//-------------------------------------------
// クラス定義(ポーズ)
//-------------------------------------------
class CPause
{
public:		// 誰でもアクセスできる

	// ポーズ画面の構成物の定義
	enum MENU
	{
		MENU_CONTINUE = 0,	// コンテニュー
		MENU_RESET,			// リトライ
		MENU_TITLE,			// タイトルに戻る
		MENU_MAX
	};

	// ポーズ画面のバッファの定義
	enum POLYGON
	{
		POLYGON_BACK = 0,	// 真っ黒ポリゴン
		POLYGON_MENU,		// メニュー
		POLYGON_CONTINUE,	// コンテニュー
		POLYGON_RESET,		// リセット
		POLYGON_TITLE,		// タイトル
		POLYGON_MAX
	};

	CPause();				// コンストラクタ
	~CPause();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// セット・ゲット関係
	void ChangeDisp(void);			// 描画状況の切り替え処理

	// 静的メンバ関数
	static CPause* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void SizeCorrect(int nIdx);		// サイズ補正処理
	void PauseSelect(void);			// ポーズの選択処理
	bool PauseDecide(void);			// ポーズの決定処理
	void PauseAlphaCorrect(void);	// ポーズの透明度補正処理
	void PauseVertex(void);			// ポーズの頂点設定処理

	// メンバ変数
	D3DXCOLOR m_PauseColor;					// 選択中の選択肢の色
	D3DXVECTOR3 sizeDest;					// 目標のサイズ
	CObject2D* m_apObject[POLYGON_MAX];		// ポリゴンの情報
	MENU m_PauseMenu;						// ポーズメニュー
	int m_nPauseCounter;					// カウンター
	float m_fPauseAlpha;					// 透明度を変化させる変数
	bool m_bDisp;							// 描画状況
};

#endif