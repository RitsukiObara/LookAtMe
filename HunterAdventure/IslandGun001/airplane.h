//===================================
//
// 飛行機ヘッダー[airplane.h]
// Author 小原立暉
//
//===================================
#ifndef _AIRPLANE_H_
#define _AIRPLANE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CPropeller;			// プロペラ
class CPlayer;				// プレイヤー

//-----------------------------------
// クラス定義(飛行機)
//-----------------------------------
class CAirplane : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_MOVE = 0,		// 移動状態
		STATE_FALL,			// 墜落状態
		STATE_MAX			// この列挙型の総数
	};

	CAirplane();			// コンストラクタ
	~CAirplane();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Update(CPlayer* pPlayer);	// 更新処理(プレイヤー管轄で使う処理)
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& posDest);				// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);		// 状態の設定処理
	STATE GetState(void) const;				// 状態の取得処理
	D3DXVECTOR3 GetPosDest(void) const;		// 目的の位置の取得処理

	// 静的メンバ関数
	static CAirplane* Create(const D3DXVECTOR3& posDest);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Move(CPlayer* pPlayer);	// 移動処理
	void Fall(void);				// 墜落処理

	// メンバ変数
	CPropeller* m_pPropeller;		// プロペラの情報
	D3DXVECTOR3 m_posDest;			// 目的の位置
	D3DXVECTOR3 m_move;				// 移動量
	STATE m_state;					// 状態
	int m_nFallCount;				// 墜落カウント
};

#endif