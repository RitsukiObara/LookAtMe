//===================================
//
// ドアヘッダー[door.h]
// Author 小原立暉
//
//===================================
#ifndef _DOOR_H_
#define _DOOR_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CPushTiming;		// ボタン表示

//-----------------------------------
// クラス定義(ドア)
//-----------------------------------
class CDoor : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_OPEN,			// 開き状態
		STATE_CLOSE,		// 閉じ状態
		STATE_MAX			// この列挙型の総数
	};

	CDoor();		// コンストラクタ
	~CDoor();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);				// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);		// 状態の設定処理
	STATE GetState(void) const;				// 状態の取得処理
	void SetEnableDisp(const bool bDisp);	// 描画状況の設定処理
	bool IsDisp(void) const;				// 描画状況の取得処理

	// 静的メンバ関数
	static CDoor* Create(const D3DXVECTOR3& pos);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Open(void);		// 開き処理
	void Close(void);		// 閉じ処理

	// メンバ変数
	CModel* m_pDoor;		// ドア本体の情報
	CPushTiming* m_pButton;	// ボタンの情報
	STATE m_state;			// 状態
	bool m_bDisp;			// ボタンの描画状況
};

#endif