//===================================
//
// 消火栓ヘッダー[hydrant.h]
// Author 小原立暉
//
//===================================
#ifndef _HYDRANT_H_
#define _HYDRANT_H_

//***********************************
// インクルードファイル
//***********************************
#include "obstacle.h"

//-----------------------------------
// クラス定義(障害物)
//-----------------------------------
class CHydrant : public CObstacle
{
public:			// 誰でもアクセスできる

	// 列挙型定義
	typedef enum
	{
		STATE_NONE = 0,			// 通常状態
		STATE_SMASH,			// 吹き飛ばし状態
		STATE_MAX				// この列挙型の総数
	}STATE;

	CHydrant();					// コンストラクタ
	~CHydrant();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	bool Hit(void);				// ヒット処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ変数
	STATE m_state;				// 状態
	float m_fGravity;			// 重力処理
	float m_fRotMove;			// 向きの移動量
};

#endif