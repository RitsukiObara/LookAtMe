//=====================================================
//
// ボスの死亡波紋ヘッダー [boss_ripple.h]
// Author：小原立暉
//
//=====================================================
#ifndef _BOSS_RIPPLE_H_			// このマクロ定義がされていない場合
#define _BOSS_RIPPLE_H_			// 二重インクルード防止のマクロを定義する

//*****************************************************
// インクルードファイル
//*****************************************************
#include "mesh_donut.h"

//-----------------------------------------------------
// クラス定義(ボスの死亡波紋)
//-----------------------------------------------------
class CBossRipple : public CMeshDonut
{
public:		// 誰でもアクセスできる

	CBossRipple();		// オーバーロードコンストラクタ
	~CBossRipple();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);				// 情報の設定処理

	// 静的メンバ関数
	static CBossRipple* Create(const D3DXVECTOR3& pos);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Spread(void);		// 広げ処理

	// メンバ変数
	float m_fAddCircum;		// 円周の追加量
	float m_fAddWidth;		// 幅の追加量
	float m_fAlpha;			// 透明度
};


#endif