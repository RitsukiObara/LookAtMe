//============================================
//
// チュートリアルのプレイヤーヘッダー[tutorial_player.h]
// Author：小原立暉
//
//============================================
#ifndef _TUTORIAL_PLAYER_H_					// このマクロ定義がされていなかったら
#define _TUTORIAL_PLAYER_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "player.h"

//--------------------------------------------
// 押しタイミング
//--------------------------------------------
class CPushTiming;			// 押しタイミング

//--------------------------------------------
// クラス(チュートリアルプレイヤークラス)
//--------------------------------------------
class CTutorialPlayer : public CPlayer
{
public:			// 誰でもアクセスできる

	CTutorialPlayer();		// コンストラクタ
	~CTutorialPlayer();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const CScene::MODE mode);		// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void CollisionStageLeft(void);		// ステージの左端との当たり判定

	// メンバ変数
	CPushTiming* m_pPushTiming;		// 押しタイミングのポインタ
};

#endif