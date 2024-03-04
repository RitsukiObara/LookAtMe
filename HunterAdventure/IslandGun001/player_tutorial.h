//============================================
//
// チュートリアルプレイヤーヘッダー[player_tutorial.h]
// Author：小原立暉
//
//============================================
#ifndef _PLAYER_TUTORIAL_H_					// このマクロ定義がされていなかったら
#define _PLAYER_TUTORIAL_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "player.h"

//--------------------------------------------
// クラス(チュートリアルプレイヤークラス)
//--------------------------------------------
class CTutorialPlayer : public CPlayer
{
public:			// 誰でもアクセスできる

	CTutorialPlayer();				// コンストラクタ
	~CTutorialPlayer();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);		// 情報の設定処理

	// 静的メンバ関数
	static CTutorialPlayer* Create(const D3DXVECTOR3& pos);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void DoorCollision(void);		// ドアとの当たり判定
	void Trans(void);				// 遷移状態処理
	void StageCollision(void);		// ステージとの当たり判定

	// メンバ変数
	int m_nTransCount;		// 遷移カウント
};

#endif