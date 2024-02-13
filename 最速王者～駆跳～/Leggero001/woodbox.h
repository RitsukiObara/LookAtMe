//===================================
//
// 木箱ヘッダー[woodbox.h]
// Author 小原立暉
//
//===================================
#ifndef _WOODBOX_H_
#define _WOODBOX_H_

//***********************************
// インクルードファイル
//***********************************
#include "obstacle.h"

//-----------------------------------
// クラス定義(障害物)
//-----------------------------------
class CWoodBox : public CObstacle
{
public:			// 誰でもアクセスできる

	CWoodBox();					// コンストラクタ
	~CWoodBox();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ変数
	float m_fGravity;			// 重力
};

#endif