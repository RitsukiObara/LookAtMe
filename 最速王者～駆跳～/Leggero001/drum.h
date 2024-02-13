//===================================
//
// ドラム缶ヘッダー[drum.h]
// Author 小原立暉
//
//===================================
#ifndef _DRUM_H_
#define _DRUM_H_

//***********************************
// インクルードファイル
//***********************************
#include "obstacle.h"

//-----------------------------------
// クラス定義(障害物)
//-----------------------------------
class CDrum : public CObstacle
{
public:			// 誰でもアクセスできる

	// 列挙型定義
	typedef enum
	{
		STATE_NONE = 0,			// 通常状態
		STATE_ROLL,				// 転がり状態
		STATE_MAX				// この列挙型の総数
	}STATE;

	CDrum();					// コンストラクタ
	~CDrum();					// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	bool Hit(void);				// ヒット処理
	void HitLeft(void);			// ヒット(左に飛ばす)処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3 m_move;				// 移動量
	float m_fRotMove;				// 向きの移動量
	STATE m_state;					// 状態
	int m_nCollIdx;					// 当たり判定ポリゴンのインデックス
	int m_nSoundCount;				// サウンドのカウント
};

#endif