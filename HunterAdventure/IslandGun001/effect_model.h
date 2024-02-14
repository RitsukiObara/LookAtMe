//===================================
//
// エフェクトモデルヘッダー[effect_model.h]
// Author 小原立暉
//
//===================================
#ifndef _EFFECT_MODEL_H_
#define _EFFECT_MODEL_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(CEffectModel)
//-----------------------------------
class CEffectModel : public CModel
{
public:					// 誰でもアクセスできる

	CEffectModel();			// コンストラクタ
	~CEffectModel();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXVECTOR3& scale, const int nLife, const D3DXVECTOR3& rotMove);					// 情報の設定処理

	// 静的メンバ関数
	static CEffectModel* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXVECTOR3& scale, const int nLife, const D3DXVECTOR3& rotMove);	// 生成処理

private:				// 自分だけアクセスできる

	// メンバ関数
	void Move(void);		// 移動処理
	void RotMove(void);		// 向きの移動処理
	void SubAlpha(void);	// 透明度の減算処理
	void Reduce(void);		// 拡大率の減算処理

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rotMove;	// 向きの移動量
	D3DXVECTOR3 m_reduce;	// 拡大率の縮む間隔
	int m_nLife;			// 寿命
	float m_fAlpha;			// 透明度
	float m_fSub;			// 透明になる間隔
};

#endif