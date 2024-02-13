//===================================
//
// 範囲移動ヘッダー[range_move.h]
// Author：小原立暉
//
//===================================
#ifndef _RANGE_MOVE_H_			// このマクロ定義がされていなかったら
#define _RANGE_MOVE_H_			// 2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "object3D.h"

//-----------------------------------
// クラス定義(ゴール)
//-----------------------------------
class CRangeMove : public CObject3D
{
public:		// 誰でもアクセスできる

	// 状態
	typedef enum
	{
		STATE_ADJUST = 0,	// サイズ調整状態
		STATE_MOVE,			// 移動状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	CRangeMove();			// コンストラクタ
	~CRangeMove();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos);		// 情報の設定処理

	// 静的メンバ関数
	static CRangeMove* Create(const D3DXVECTOR3& pos);		// 生成処理

private:	// 自分だけアクセスできる

	// メンバ関数
	void Move(void);			// 移動処理
	void Pos(void);				// 位置移動処理
	void StateChange(void);		// 状態の切り替え処理
	void SizeX(void);			// サイズ(X軸)の調整処理
	void SizeY(void);			// サイズ(Y軸)の調整処理
	void Size(void);			// サイズ(全体)の調整処理
	void RangeMove(void);		// 範囲の移動処理
	D3DXVECTOR3 Hit(D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin);		// 当たり判定処理

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	STATE m_state;			// 状態
};

#endif