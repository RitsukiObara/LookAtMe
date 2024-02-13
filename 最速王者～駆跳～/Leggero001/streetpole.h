//===================================
//
// 街灯(支柱)ヘッダー[streetpole.h]
// Author 小原立暉
//
//===================================
#ifndef _STREETPOLE_H_
#define _STREETPOLE_H_

//***********************************
// インクルードファイル
//***********************************
#include "obstacle.h"

//-----------------------------------
// クラス定義(障害物)
//-----------------------------------
class CStreetPole : public CObstacle
{
public:			// 誰でもアクセスできる

	CStreetPole();				// コンストラクタ
	~CStreetPole();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	bool Hit(void);				// ヒット処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// 情報の設定処理

private:		// 自分だけアクセスできる

};

#endif