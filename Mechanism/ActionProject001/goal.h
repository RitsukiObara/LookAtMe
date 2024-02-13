//============================================
//
// ゴールヘッダー[goal.h]
// Author：小原立暉
//
//============================================
#ifndef _GOAL_H_					// このマクロ定義がされていなかったら
#define _GOAL_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CModel;		// モデル

//--------------------------------------------
// クラス定義(ゴール)
//--------------------------------------------
class CGoal : public CObject
{
public:				// 誰でもアクセスできる

	// 列挙型定義(モデルの種類)
	enum MODEL
	{
		MODEL_BODY = 0,		// 本体
		MODEL_POINT,		// 得点
		MODEL_MAX			// この列挙型の総数
	};

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_PUNCH = 0,		// パンチ
		TYPE_SMALL_R,		// 右の「+1000」
		TYPE_BIG,			// +2000
		TYPE_SMALL_L,		// 左の「+2000」
		TYPE_MAX			// この列挙型の総数
	};

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_CYCLE = 0,	// 回転処理
		STATE_STOP,			// 停止処理
		STATE_MAX			// この列挙型の総数
	};

	CGoal();				// コンストラクタ
	~CGoal();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void Hit(void);			// ヒット処理
	void Break(void);		// 破壊処理
	void SetData(const D3DXVECTOR3& pos);		// 情報の設定処理

	// セット・ゲット関係
	CModel* GetModel(const MODEL model) const;	// モデルの取得処理
	TYPE GetType(void) const;					// 種類の取得処理
	STATE GetState(void) const;					// 状態の取得処理

	// 静的メンバ変数
	static CGoal* Get(void);					// 取得処理
	static CGoal* Create(const D3DXVECTOR3& pos);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void RotDecide(float* m_fRot);	// 向きの決定処理
	void RotCorrect(void);			// 向きの補正処理
	void HitRotDecide(void);		// ヒット時の向きの決定処理
	void PlayerPosDecide(void);		// プレイヤーの位置の設定処理
	void PlayerSetting(void);		// プレイヤーの情報の設定処理

	// メンバ変数
	CModel* m_aGoal[MODEL_MAX];		// ゴールの変数
	TYPE m_type;					// 種類
	STATE m_state;					// 状態
	int m_nStateCount;				// 状態カウント
	float m_fRotDest;				// 目標の向き

	// 静的メンバ変数
	static CGoal* m_pGoal;			// ゴールの情報
};

#endif