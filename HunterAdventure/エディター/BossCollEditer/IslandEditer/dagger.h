//===================================
//
// ダガーヘッダー[dagger.h]
// Author 小原立暉
//
//===================================
#ifndef _DAGGER_H_
#define _DAGGER_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class COrbit;		// 軌跡

//-----------------------------------
// クラス定義(ダガー)
//-----------------------------------
class CDagger : public CModel
{
public:			// 誰でもアクセスできる

	CDagger();			// コンストラクタ
	~CDagger();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(D3DXMATRIX* mtx);				// 情報の設定処理

	// セット・ゲット関係
	void SetEnableDisp(const bool bDisp);		// 描画状況の設定処理
	bool IsDisp(void) const;					// 描画状況の取得処理

	// 静的メンバ関数
	static CDagger* Create(D3DXMATRIX* mtx);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	COrbit* m_pOrbit;				// 軌跡の情報
	D3DXMATRIX* m_pMtxParent;		// 親のマトリックス
	bool m_bDisp;					// 描画状況
	bool m_bDispOld;				// 前回の描画状況
};

#endif