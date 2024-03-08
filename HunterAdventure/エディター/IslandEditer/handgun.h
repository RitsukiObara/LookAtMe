//===================================
//
// 拳銃ヘッダー[handgun.h]
// Author 小原立暉
//
//===================================
#ifndef _HANDGUN_H_
#define _HANDGUN_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(拳銃)
//-----------------------------------
class CHandgun : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,	// 通常状態
		STATE_RELOAD,	// リロード状態
		STATE_RETURN,	// 戻り状態
		STATE_MAX		// この列挙型の総数
	};

	CHandgun();			// コンストラクタ
	~CHandgun();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot, D3DXMATRIX* mtx);				// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);			// 状態の設定処理
	STATE GetState(void) const;					// 状態の取得処理
	void SetEnableDisp(const bool bDisp);		// 描画状況の設定処理
	bool IsDisp(void) const;					// 描画状況の取得処理

	// 静的メンバ関数
	static CHandgun* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot, D3DXMATRIX* mtx);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void PosMove(void);				// 位置の移動処理
	void RotMove(void);				// 向きの移動処理
	void ReturnMove(void);			// 戻り状態の移動処理

	// メンバ変数
	D3DXVECTOR3 m_posDest;			// 目的の位置
	D3DXVECTOR3 m_posInit;			// 初期位置
	D3DXVECTOR3 m_rotInit;			// 初期向き
	D3DXMATRIX* m_pMtxParent;		// 親のマトリックス
	STATE m_state;					// 状態
	int m_nReloadCount;				// リロードカウント
	float m_fRotMove;				// 向きの移動量
	bool m_bDisp;					// 描画状況
};

#endif