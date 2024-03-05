//=====================================================
//
// メッシュの波ヘッダー [mesh_wave.h]
// Author：小原立暉
//
//=====================================================
#ifndef _MESH_WAVE_H_			// このマクロ定義がされていない場合
#define _MESH_WAVE_H_			// 二重インクルード防止のマクロを定義する

//*****************************************************
// インクルードファイル
//*****************************************************
#include "mesh_field.h"

//-----------------------------------------------------
// クラス定義(メッシュの波)
//-----------------------------------------------------
class CMeshWave : public CMeshField
{
public:		// 誰でもアクセスできる

	CMeshWave(CObject::PRIORITY priority = PRIORITY_BG);			// オーバーロードコンストラクタ
	virtual ~CMeshWave();			// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const float fWaveHeight, const float fCycle, const float fSpeed);		// データの設定処理

	// 静的メンバ関数
	static CMeshWave* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const float fWaveHeight, const float fCycle, const float fSpeed, const PRIORITY priority);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void SetWave(void);		// 波の設定処理

	// メンバ変数
	float m_fWaveRot;		// 波の向き
	float m_fWaveHeight;	// 波の高さ
	float m_fCycle;			// 波の周期
	float m_fSpeed;			// 波の速度
};


#endif