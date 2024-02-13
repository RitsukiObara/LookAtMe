//============================================
//
// 残り距離告知ヘッダー[announce.h]
// Author：小原立暉
//
//============================================
#ifndef _ANNOUNCE_H_					// このマクロ定義がされていなかったら
#define _ANNOUNCE_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;		// 2Dポリゴン
class CAnim;			// アニメーション

//--------------------------------------------
// クラス定義(告知)
//--------------------------------------------
class CAnnounce : public CObject
{
public:				// 誰でもアクセスできる

	// 列挙型定義(ポリゴンの種類)
	typedef enum
	{
		TEX_ROAD = 0,			// 道のり
		TEX_GOAL,				// ゴール
		TEX_PLAYER,				// プレイヤー
		TEX_MAX					// この列挙型の総数
	}TEX;

	// 列挙型定義(ポリゴンの種類)
	typedef enum
	{
		POLY_ROAD = 0,			// 道のり
		POLY_GOAL,				// ゴール
		POLY_MAX				// この列挙型の総数
	}POLY;

	CAnnounce();				// コンストラクタ
	~CAnnounce();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);		// 情報の設定処理

	// 静的メンバ変数
	static CAnnounce* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void DistanceMeasure(void);			// 距離測定処理
	void PlayerMove(void);				// プレイヤーの移動処理

	// メンバ変数
	CObject2D* m_apObject2D[POLY_MAX];	// 番号の情報
	CAnim* m_pPlayer;					// プレイヤー
	float m_fTotalDist;		// 総合の距離
	int m_nDist;			// 距離

	// 静的メンバ変数
	static const char* c_apTextureName[TEX_MAX];	// テクスチャの名前
};

#endif