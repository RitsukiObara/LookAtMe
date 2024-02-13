//===================================
//
// ゴールヘッダー[Goal.h]
// Author：小原立暉
//
//===================================
#ifndef _GOAL_H_			// このマクロ定義がされていなかったら
#define _GOAL_H_			// 2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "object3D.h"
#include "scene.h"

//-----------------------------------
// クラス定義(ゴール)
//-----------------------------------
class CGoal : public CObject3D
{
public:		// 誰でもアクセスできる

	CGoal();					// コンストラクタ
	CGoal(CObject::TYPE type, CObject::PRIORITY priority = PRIORITY_PLAYER);		// オーバーロードコンストラクタ
	virtual ~CGoal();			// デストラクタ
	void Box(void);				// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void) = 0;	// 更新処理
	virtual void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos);		// 情報の設定処理

	void AlphaChange(void);			// 透明度の変化

	// 静的メンバ関数
	static CGoal* Create(const D3DXVECTOR3& pos, const CScene::MODE mode);		// 生成処理

private:	// 自分だけアクセスできる

	// メンバ変数
	D3DXCOLOR m_col;		// 色
	float m_fSubCol;		// 透明度の減算量
};

#endif