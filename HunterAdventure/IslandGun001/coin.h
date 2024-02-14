//===================================
//
// 小判ヘッダー[coin.h]
// Author 小原立暉
//
//===================================
#ifndef _COIN_H_
#define _COIN_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// クラス定義(小判)
//-----------------------------------
class CCoin : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態 
		STATE_GET,			// 取得状態
		STATE_DEATH,		// 死亡状態
		STATE_MAX			// この列挙型の総数
	};

	CCoin();					// コンストラクタ
	~CCoin();					// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);			// 情報の設定処理
	void Hit(void);					// ヒット処理

	// セット・ゲット関数
	STATE GetState(void) const;		// 状態の取得処理

	// 静的メンバ関数
	static CCoin* Create(const D3DXVECTOR3& pos);	// 生成処理

	static CListManager<CCoin*> GetList(void);		// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Get(void);			// 取得状態処理
	void Cycle(void);		// 回転処理

	// メンバ変数
	STATE m_state;			// 状態
	int m_nDeathCount;		// 死亡カウント
	float m_fCycleSpeed;	// 回転速度
	float m_fHeightDest;	// 目標の高さ

	// 静的メンバ変数
	static CListManager<CCoin*> m_list;		// リスト
};

#endif