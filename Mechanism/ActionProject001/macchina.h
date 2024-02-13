//===================================
//
// マキナ草ヘッダー[macchina.h]
// Author 小原立暉
//
//===================================
#ifndef _MACCHINA_H_				// このマクロ定義がされていなかったら
#define _MACCHINA_H_				// 2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(マキナ草)
//-----------------------------------
class CMacchina : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_REDUCE,		// 縮小状態
		STATE_EXPANSE,		// 伸び状態
		STATE_EJECT,		// 排出状態
		STATE_MAX			// この列挙型の総数
	};

	CMacchina();			// コンストラクタ
	~CMacchina();			// デストラクタ

	// リスト構造関係
	void SetPrev(CMacchina* pPrev);	// 前のポインタの設定処理
	void SetNext(CMacchina* pNext);	// 後のポインタの設定処理
	CMacchina* GetPrev(void) const;	// 前のポインタの設定処理
	CMacchina* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void Hit(void);			// ヒット処理

	void SetData(const D3DXVECTOR3& pos);		// 情報の設定処理

	// セット・ゲット関係
	bool GetCollision(void);					// 当たり判定の取得処理

	// 静的メンバ関数
	static CMacchina* Create(const D3DXVECTOR3& pos);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void NoneRescale(void);		// 通常状態の拡大率の変化処理
	void ReduceScale(void);		// 縮小状態の拡大率の変化処理
	void ExpanseScale(void);	// 伸び状態の拡大率の変化処理
	void EjectScale(void);		// 排出状態の拡大率の変化処理

	// メンバ変数
	D3DXVECTOR3 m_scaleDest;	// 目的の拡大率
	STATE m_state;				// 状態
	int m_nStateCount;			// 状態カウント
	bool m_bCollision;			// 当たり判定の有無

	// リスト構造関係
	CMacchina* m_pPrev;	// 前へのポインタ
	CMacchina* m_pNext;	// 次へのポインタ
};

#endif