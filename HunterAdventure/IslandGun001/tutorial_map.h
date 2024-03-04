//===================================
//
// チュートリアルマップヘッダー[tutorial_map.h]
// Author 小原立暉
//
//===================================
#ifndef _TUTORIAL_MAP_H_
#define _TUTORIAL_MAP_H_

//***********************************
// インクルードファイル
//***********************************
#include "object.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CMeshWall;			// メッシュウォール
class CMeshField;			// メッシュフィールド

//-----------------------------------
// クラス定義(チュートリアルマップ)
//-----------------------------------
class CTutorialMap : public CObject
{
public:			// 誰でもアクセスできる

	CTutorialMap();			// コンストラクタ
	~CTutorialMap();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(void);				// 情報の設定処理

	// 静的メンバ関数
	static CTutorialMap* Create(void);		// 生成処理

	// 定数定義
	static const int NUM_WALL = 3;		// 壁の総数

private:		// 自分だけアクセスできる

	// メンバ変数
	CMeshWall* m_apWall[NUM_WALL];	// 壁の情報
};

#endif