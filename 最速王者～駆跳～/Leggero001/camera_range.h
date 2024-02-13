//============================================
//
// カメラの範囲ヘッダー[camera_range.h]
// Author：小原立暉
//
//============================================
#ifndef _CAMERA_RANGE_H_		//このマクロ定義がされていなかったら
#define _CAMERA_RANGE_H_		//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object3D.h"
#include "camera.h"

//--------------------------------------------
// クラス(カメラ範囲クラス)
//--------------------------------------------
class CCameraRange : public CObject3D
{
public:			// 誰でもアクセスできる

	CCameraRange();				// コンストラクタ
	CCameraRange(CObject::TYPE type, PRIORITY priority = PRIORITY_BG);		// オーバーロードコンストラクタ
	virtual ~CCameraRange();	// デストラクタ
	void Box(void);				// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type);		// 情報の設定処理

	// セット・ゲット関数
	void SetType(const CCamera::TYPE type);	// カメラの種類の設定処理
	CCamera::TYPE GetType(void) const;		// カメラの種類の取得処理

	// 静的メンバ関数
	static CCameraRange* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CCamera::TYPE m_type;		// 種類
};

#endif