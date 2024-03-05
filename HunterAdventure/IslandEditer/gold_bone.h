//===================================
//
// 金の骨ヘッダー[gold_bone.h]
// Author 小原立暉
//
//===================================
#ifndef _GOLD_BONE_H_
#define _GOLD_BONE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// クラス定義(金の骨)
//-----------------------------------
class CGoldBone : public CModel
{
public:			// 誰でもアクセスできる

	CGoldBone();					// コンストラクタ
	~CGoldBone();					// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);				// 情報の設定処理
	void Hit(void);					// ヒット処理

	// 静的メンバ関数
	static CGoldBone* Create(const D3DXVECTOR3& pos);	// 生成処理

	static CListManager<CGoldBone*> GetList(void);		// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Get(void);			// 取得状態処理

	// 静的メンバ変数
	static CListManager<CGoldBone*> m_list;		// リスト
};

#endif