//============================================
//
// ゴールUIヘッダー[GoalUI.h]
// Author：小原立暉
//
//============================================
#ifndef _GOALUI_H_					// このマクロ定義がされていなかったら
#define _GOALUI_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "model.h"

//--------------------------------------------
// クラス定義(数値)
//--------------------------------------------
class CGoalUI : public CModel
{
public:				// 誰でもアクセスできる

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_APPEAR = 0,		// 出現状態
		STATE_STOP,				// 停止状態
		STATE_CLEAR,			// 消去処理
		STATE_MAX				// この列挙型の総数
	}STATE;

	CGoalUI();		// コンストラクタ
	~CGoalUI();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos);			// 情報の設定処理

	// 静的メンバ関数
	static CGoalUI* Create(const D3DXVECTOR3& pos);	// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	D3DXVECTOR3 InitPosCalc(const D3DXVECTOR3& pos);		// 初期位置設定処理

	void Move(void);			// 移動処理
	void Cycle(void);			// 回転処理
	void AppearChange(void);	// 出現状態変化処理
	void Brake(void);			// ブレーキ処理
	void StopChange(void);		// 停止状態変化処理
	bool Clear(void);			// 消去処理

	// メンバ変数
	D3DXVECTOR3 m_posDest;		// 目標の位置
	D3DXVECTOR3 m_move;			// 移動量
	D3DXVECTOR3 m_rotmove;		// 向きの移動量
	STATE m_state;				// 状態
	int m_nStateCount;			// 状態カウント
};

#endif