//===================================
//
// マルチカメラヘッダー[multi_camera.h]
// Author 小原立暉
//
//===================================
#ifndef _MULTI_CAMERA_H_			//このマクロ定義がされていなかったら
#define _MULTI_CAMERA_H_			//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "camera.h"

// マクロ定義
#define MAX_CAMERA		(4)		// カメラの最大数

//-----------------------------------
// クラス定義(マルチカメラ)
//-----------------------------------
class CMultiCamera
{
public:			// 誰でもアクセス出来る

	CMultiCamera();		// コンストラクタ
	~CMultiCamera();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Set(const int nIdx);	// 設定処理

	// セット・ゲット関係
	CCamera* GetCamera(const int nIdx);	// カメラの取得処理

private:		// 自分だけアクセス出来る

	// メンバ変数
	CCamera* m_aCamera[MAX_CAMERA];		// カメラの情報
};

#endif