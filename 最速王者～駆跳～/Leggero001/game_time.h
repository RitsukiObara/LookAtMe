//============================================
//
// ゲームの時間ヘッダー[game_time.h]
// Author：小原立暉
//
//============================================
#ifndef _GAME_TIME_H_					// このマクロ定義がされていなかったら
#define _GAME_TIME_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "time.h"

//--------------------------------------------
// クラス定義(ゲームタイム)
//--------------------------------------------
class CGameTime : public CTime
{
public:				// 誰でもアクセスできる

	CGameTime();			// コンストラクタ
	~CGameTime();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void TimeStart(void);	// タイムの開始処理

	// 静的メンバ変数
	static CGameTime* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void Basic(void);			// 通常の更新処理
	void PauseProcess(void);	// ポーズ中の処理
	void Calculate(void);		// 計算処理

	// メンバ変数
	DWORD m_StartTime;		// 開始時間
	DWORD m_PauseTime;		// ポーズ中の時間
	bool m_bGoal;			// ゴール状況
};

#endif