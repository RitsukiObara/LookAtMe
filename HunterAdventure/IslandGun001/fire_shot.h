//===================================
//
// 炎攻撃ヘッダー[fire_shot.h]
// Author 小原立暉
//
//===================================
#ifndef _FIRE_SHOT_H_
#define _FIRE_SHOT_H_

//***********************************
// インクルードファイル
//***********************************
#include "mesh_cylinder.h"
#include "list_manager.h"

//-----------------------------------
// クラス定義(炎攻撃)
//-----------------------------------
class CFireShot : public CMeshCylinder
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_EXTEND = 0,		// 伸び状態
		STATE_SHRINK,			// 縮み処理
		STATE_MAX				// この列挙型の総数
	};

	CFireShot();				// コンストラクタ
	~CFireShot();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);					// 情報の設定処理

	// セット・ゲット関係
	void SetState(const STATE state);		// 状態の設定処理
	STATE GetState(void) const;				// 状態の取得処理

	// 静的メンバ関数
	static CFireShot* Create(const D3DXVECTOR3& pos);		// 生成処理

	static CListManager<CFireShot*> GetList(void);			// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Extend(void);		// 伸び状態処理
	bool Shrink(void);		// 縮み状態処理
	void SetTex(void);		// テクスチャ座標の設定処理

	// メンバ変数
	STATE m_state;			// 状態

	// 静的メンバ変数
	static CListManager<CFireShot*> m_list;		// リスト
};

#endif