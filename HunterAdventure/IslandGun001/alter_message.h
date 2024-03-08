//===================================
//
// 祭壇のメッセージヘッダー[alter_message.h]
// Author 小原立暉
//
//===================================
#ifndef _ALTER_MESSAGE_H_
#define _ALTER_MESSAGE_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(祭壇メッセージ)
//-----------------------------------
class CAlterMessage : public CBillboard
{
public:

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_EXTEND = 0,	// 伸び状態
		STATE_SHRINK,		// 縮み状態
		STATE_MAX			// この先の処理を行わない
	};

	CAlterMessage();		// コンストラクタ
	~CAlterMessage();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);					// 情報の設定処理

	// 静的メンバ関数
	static CAlterMessage* Create(const D3DXVECTOR3& pos);	// 生成処理

private:

	// メンバ変数
	D3DXVECTOR3 m_sizeDest;	// 目的のサイズ
	STATE m_state;			// 状態
	int m_nStateCount;		// 状態カウント
};

#endif