//===================================
//
// 街灯(光源)ヘッダー[streetlamp.h]
// Author 小原立暉
//
//===================================
#ifndef _STREETLAMP_H_
#define _STREETLAMP_H_

//***********************************
// インクルードファイル
//***********************************
#include "obstacle.h"

//-----------------------------------
// クラス定義(障害物)
//-----------------------------------
class CStreetLamp : public CObstacle
{
public:			// 誰でもアクセスできる

	CStreetLamp();				// コンストラクタ
	~CStreetLamp();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	void PlayerCatch(D3DXVECTOR3* pos);		// プレイヤーのキャッチ処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// 情報の設定処理

private:		// 自分だけアクセスできる


};

#endif