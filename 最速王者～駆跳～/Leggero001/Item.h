//===================================
//
//アイテムヘッダー[Item.h]
//Author 小原立暉
//
//===================================
#ifndef _ITEM_H_			//このマクロ定義がされていなかったら
#define _ITEM_H_			//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// マクロ定義
//-----------------------------------
#define ITEM_ROT		(D3DXVECTOR3(0.0f,0.0f,0.5f))			// アイテムの傾き

//-----------------------------------
// クラス定義(アイテム)
//-----------------------------------
class CItem : public CModel
{
public:		// 誰でもアクセスできる

	// 状態
	typedef enum
	{
		STATE_APPEAR = 0,		// 出現処理
		STATE_NONE,				// 通常状態
		STATE_MAX				// この列挙型の総数
	}STATE;

	CItem();			// コンストラクタ
	~CItem();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理
	void Hit(void);					// ヒット処理

	void SetData(const D3DXVECTOR3& pos, const STATE state);		// 情報の設定処理

	// 静的メンバ関数
	static CItem* Create(const D3DXVECTOR3& pos, STATE state);		// 生成処理

private:	// 自分だけアクセスできる

	// メンバ関数
	void HealingStamina(void);					// スタミナの回復処理

	// メンバ変数
	STATE m_state;								// 状態
	D3DXVECTOR3 m_rotmove;						// 移動量
};

#endif