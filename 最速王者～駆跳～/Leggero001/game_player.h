//============================================
//
// ゲームのプレイヤーヘッダー[game_player.h]
// Author：小原立暉
//
//============================================
#ifndef _GAME_PLAYER_H_					// このマクロ定義がされていなかったら
#define _GAME_PLAYER_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "player.h"

//--------------------------------------------
// クラス(ゲームのプレイヤークラス)
//--------------------------------------------
class CGamePlayer : public CPlayer
{
public:			// 誰でもアクセスできる

	CGamePlayer();				// コンストラクタ
	~CGamePlayer();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void CollisionStageLeft(void);		// ステージの左端の当たり判定

	// メンバ変数
	D3DXVECTOR3 m_stageLeft;	// ステージの左端
};

#endif