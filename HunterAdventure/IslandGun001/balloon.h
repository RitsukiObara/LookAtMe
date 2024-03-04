//===================================
//
// 風船ヘッダー[balloon.h]
// Author 小原立暉
//
//===================================
#ifndef _BALLOON_H_
#define _BALLOON_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CBalloonRope;		// 風船の紐

//-----------------------------------
// クラス定義(風船)
//-----------------------------------
class CBalloon : public CModel
{
public:			// 誰でもアクセスできる

	CBalloon();			// コンストラクタ
	~CBalloon();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);				// 情報の設定処理

	// 静的メンバ関数
	static CBalloon* Create(const D3DXVECTOR3& pos);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CBalloonRope* m_pRope;	// 紐の情報
	float m_fPosYInit;		// 初期の高さ
	float m_fHeightRot;		// 高さを決める向き
};

#endif