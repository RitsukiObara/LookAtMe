//===================================
//
// 石柱ヘッダー[alter_pole.h]
// Author 小原立暉
//
//===================================
#ifndef _ALTER_POLE_H_
#define _ALTER_POLE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CGoldBone;		// 金の骨

//-----------------------------------
// クラス定義(石柱)
//-----------------------------------
class CAlterPole : public CModel
{
public:			// 誰でもアクセスできる

	CAlterPole();			// コンストラクタ
	~CAlterPole();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);				// 情報の設定処理

	// メンバ関数
	void Charge(void);		// チャージ処理
	void GoldBonePosSet(const D3DXVECTOR3& pos);		// 金の骨の位置設定処理

	// セット・ゲット関係
	void SetGoldBone(CGoldBone* pBone);			// 金の骨の設定処理
	CGoldBone* GetGoldBone(void) const;			// 金の骨の取得処理
	void SetBoneRot(const float fRot);			// 金の骨への向きの設定処理
	float GetBoneRot(void) const;				// 金の骨への向きの取得処理
	void SetBoneDist(const float fDist);		// 金の骨への距離の設定処理
	float GetBoneDist(void) const;				// 金の骨への距離の取得処理
	void SetEnableEmpty(const bool bEmpty);		// 空白状況の設定処理
	bool IsEmpty(void) const;					// 空白状況の取得処理
	void SetEnableArrival(const bool bArv);		// 到着状況の設定処理
	bool IsArrival(void) const;					// 到着状況の取得処理

	// 静的メンバ関数
	static CAlterPole* Create(const D3DXVECTOR3& pos);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CGoldBone* m_pGoldBone;	// 金の骨の情報
	float m_fBoneRot;		// 骨への向き
	float m_fBoneDist;		// 骨への距離
	bool m_bEmpty;			// 空白状況
	bool m_bArrival;		// 到着状況
};

#endif