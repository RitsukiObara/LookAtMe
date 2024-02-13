#ifdef _DEBUG	// デバッグ処理
//===================================
//
// 全エディットヘッダー[editAll.h]
// Author 小原立暉
//
//===================================
#ifndef _EDITALL_H_
#define _EDITALL_H_

//******************************************
// インクルードファイル
//******************************************
#include "main.h"

//------------------------------------------
// 前方宣言
//------------------------------------------
class CEdit;			// ブロックエディット
class CBgEdit;			// 背景エディット
class CObstacleEdit;	// 障害物エディット
class CCameraEdit;		// カメラエディット
class CRangeMove;		// 範囲移動

//------------------------------------------
// クラス定義(エディット)
//------------------------------------------
class CEditAll
{
public:			// 誰でもアクセスできる

	// スタイル
	typedef enum
	{
		STYLE_BLOCK = 0,	// ブロック(ステージ)
		STYLE_BGOBJECT,		// 背景オブジェクト
		STYLE_OBSTACLE,		// 障害物
		STYLE_CAMERA,		// カメラ
		STYLE_RANGEMOVE,	// 範囲移動
		STYLE_MAX			// この列挙型の総数
	}STYLE;

	CEditAll();				// コンストラクタ
	~CEditAll();			// デストラクタ

	//プロトタイプ宣言
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// セット・ゲット関係
	STYLE GetStyle(void) const;		// スタイルの取得処理

	// 静的メンバ関数
	static CEditAll* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void StyleChange(void);				// スタイルの設定処理
	void RangeMoveChange(void);			// 範囲移動の設定処理
	D3DXVECTOR3 GetEditPos(void);		// 位置の取得処理
	void AllDelete(void);				// ポインタ全消去処理
	void Set(const D3DXVECTOR3& pos);	// 設定処理

	void SetItem(void);				// アイテムの設定処理
	void DeleteItem(void);			// アイテムの消去処理
	void SetItemGenerator(void);	// アイテム生成器の設定処理
	void DeleteItemGenerator(void);	// アイテム生成器の消去処理

	// メンバ変数
	STYLE m_style;			// スタイル
	STYLE m_saveStyle;		// 保存用スタイル

	// 静的メンバ変数
	static CEdit* m_pEdit;						// エディットのポインタ
	static CBgEdit* m_pBgEdit;					// 背景エディットのポインタ
	static CObstacleEdit* m_pObstacleEdit;		// 障害物エディットのポインタ
	static CCameraEdit* m_pCameraEdit;			// カメラエディットのポインタ
	static CRangeMove* m_pRangeMove;			// 範囲移動のポインタ
};

#endif

#endif