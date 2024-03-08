//===================================
//
// 祭壇ヘッダー[alter.h]
// Author 小原立暉
//
//===================================
#ifndef _ALTER_H_
#define _ALTER_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "gold_bone.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CAlterPole;		// 石柱
class CAlterMessage;	// 祭壇メッセージ

//-----------------------------------
// クラス定義(祭壇)
//-----------------------------------
class CAlter : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_CHARGE,		// チャージ状態
		STATE_WAIT,			// 待機状態
		STATE_BREAK,		// 破壊状態
		STATE_MAX			// この列挙型の総数
	};

	CAlter();			// コンストラクタ
	~CAlter();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(void);				// 情報の設定処理
	void Break(void);				// 破壊処理

	// セット・ゲット関係
	CAlterPole* GetPole(const int nIdx) const;		// 石柱の取得処理
	void SetAlterMessage(CAlterMessage* pMessage);	// 祭壇のメッセージの設定処理
	void SetState(const STATE state);				// 状態の設定処理
	STATE GetState(void) const;						// 状態の取得処理
	void SetEnableLightUp(const bool bLight);		// ライト点灯状況の設定処理
	bool IsLightUp(void) const;						// ライト点灯状況の取得処理

	// 静的メンバ関数
	static CAlter* Create(void);	// 生成処理

	// 定数定義
	static const int NUM_POLE = 3;	// 石柱の数

private:		// 自分だけアクセスできる

	// メンバ関数
	void ChargeTrans(void);			// チャージ状態への遷移処理
	void GoldBoneLight(void);		// 金の骨の光処理

	// メンバ変数
	CAlterPole* m_apPole[NUM_POLE];		// 石柱の情報
	CAlterMessage* m_pMessage;			// メッセージの情報
	STATE m_state;						// 状態
	bool m_bLightUp;					// ライト点灯状況
};

#endif