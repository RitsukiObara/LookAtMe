//============================================
//
// カメラの範囲ヘッダー[camera_range.h]
// Author：小原立暉
//
//============================================
#ifndef _CAMERAEDIT_H_		//このマクロ定義がされていなかったら
#define _CAMERAEDIT_H_		//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "camera_range.h"
#include "camera.h"

//--------------------------------------------
// クラス(カメラ範囲クラス)
//--------------------------------------------
class CCameraEdit : public CCameraRange
{
public:			// 誰でもアクセスできる

	CCameraEdit();				// コンストラクタ
	CCameraEdit(CObject::TYPE type, PRIORITY priority = PRIORITY_BG);		// オーバーロードコンストラクタ
	~CCameraEdit();	// デストラクタ
	void Box(void);				// コンストラクタの箱

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ関数
	static CCameraEdit* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void TypeChange(void);		// 種類変更処理
	void Move(void);			// 移動処理
	void ScaleX(void);			// 拡大縮小処理(X軸)
	void ScaleY(void);			// 拡大縮小処理(Y軸)
	void Scale(void);			// 拡大縮小処理
	void Set(void);				// 設定処理
	void Delete(void);			// 消去処理
	void PlayerPosSet(void);	// プレイヤー位置移動処理

	// 静的メンバ変数
	static const char* c_apTypeName[CCamera::TYPE_STARTSTAGE];		// 種類の名前
};

#endif