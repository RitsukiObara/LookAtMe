//===================================
//
// アイテム生成器ヘッダー[item_generator.h]
// Author 小原立暉
//
//===================================
#ifndef _ITEM_GENERATOR_H_			//このマクロ定義がされていなかったら
#define _ITEM_GENERATOR_H_			//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CItem;			// アイテム

//-----------------------------------
// クラス定義(アイテム生成器)
//-----------------------------------
class CItemGenerator : public CModel
{
public:		// 誰でもアクセスできる

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_SET = 0,			// 設定状態
		STATE_VIBRATE,			// 振動処理
		STATE_NONE,				// 通常処理
		STATE_MAX				// この列挙型の総数
	}STATE;

	CItemGenerator();			// コンストラクタ
	~CItemGenerator();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	void SetData(const D3DXVECTOR3& pos);	// 情報の設定処理

	void Discriminate(void);				// 判別処理

	// 静的メンバ関数
	static CItemGenerator* Create(const D3DXVECTOR3& pos);		// 生成処理

private:	// 自分だけアクセスできる

	// メンバ変数
	CItem* m_pItemAddress;	// アイテムのアドレス
	STATE m_state;			// 状態
	int m_nStateCount;		// 状態カウント
	float m_fRotDest;		// 目標の向き
};

#endif