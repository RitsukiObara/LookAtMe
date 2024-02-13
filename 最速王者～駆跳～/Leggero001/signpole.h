//===================================
//
// 標識ヘッダー[signpole.h]
// Author 小原立暉
//
//===================================
#ifndef _SIGNPOLE_H_
#define _SIGNPOLE_H_

//***********************************
// インクルードファイル
//***********************************
#include "obstacle.h"

//-----------------------------------
// クラス定義(障害物)
//-----------------------------------
class CSignPole : public CObstacle
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_NONE = 0,			// 通常状態
		STATE_TOPPLE,			// 倒れ状態
		STATE_STOP,				// 停止状態
		STATE_MAX				// この列挙型の総数
	}STATE;

	CSignPole();				// コンストラクタ
	~CSignPole();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	bool Hit(void);				// ヒット処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// 情報の設定処理

	// セット・ゲット関係
	STATE GetState(void);		// 状態の設定処理

	D3DXVECTOR3 GetVtx(const int nIdx);					// 頂点の取得処理

	float ElevationCollision(const D3DXVECTOR3 pos);	// 斜めの障害物の当たり判定
	float ObliqueCollision(const D3DXVECTOR3* Vtx, const D3DXVECTOR3& pos);									// 斜めの当たり判定

	bool ToppleCollision(const D3DXVECTOR3 pos);		// 倒れ中の障害物の当たり判定
	bool ToppleSystem(void);							// 倒れ中の当たり判定の大本

private:		// 自分だけアクセスできる

	// メンバ変数
	STATE m_state;				// 状態
	float m_fGravity;			// 重力
	D3DXVECTOR3 m_vtx[4];		// 頂点座標
};

#endif