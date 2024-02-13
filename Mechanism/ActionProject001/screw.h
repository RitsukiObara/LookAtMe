//===================================
//
// ネジヘッダー[screw.h]
// Author 小原立暉
//
//===================================
#ifndef _SCREW_H_
#define _SCREW_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(飛行船)
//-----------------------------------
class CScrew : public CModel
{
public:			// 誰でもアクセスできる

	CScrew();					// コンストラクタ
	~CScrew();					// デストラクタ

	// リスト構造関係
	void SetPrev(CScrew* pPrev);	// 前のポインタの設定処理
	void SetNext(CScrew* pNext);	// 後のポインタの設定処理
	CScrew* GetPrev(void) const;	// 前のポインタの設定処理
	CScrew* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void Hit(const D3DXVECTOR3& pos);					// ヒット処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const bool bGravity);				// 情報の設定処理

	// セット・ゲット関係
	bool IsHit(void) const;			// 当たり判定の取得処理

	// 静的メンバ関数
	static CScrew* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const bool bGravity);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Gravity(void);		// 重力処理
	void Cycle(void);		// 回転処理
	void Elevation(void);	// 起伏の当たり判定処理
	void Table(void);		// 台との当たり判定処理
	void Block(void);		// ブロックとの当たり判定処理

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	bool m_bGravity;		// 重力状況
	bool m_bHit;			// 当たり判定状況

	// リスト構造関係
	CScrew* m_pPrev;	// 前へのポインタ
	CScrew* m_pNext;	// 次へのポインタ
};

#endif