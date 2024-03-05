//===================================
//
// ヤシの木ヘッダー[palm_tree.h]
// Author 小原立暉
//
//===================================
#ifndef _PALM_TREE_H_
#define _PALM_TREE_H_

//***********************************
// インクルードファイル
//***********************************
#include "tree.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CPalmFruit;	// ヤシの実

//-----------------------------------
// クラス定義(ヤシの木)
//-----------------------------------
class CPalmTree : public CTree
{
public:			// 誰でもアクセスできる

	CPalmTree();				// コンストラクタ
	~CPalmTree();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type) override;		// 情報の設定処理
	void Hit(void) override;		// ヒット処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CPalmFruit* m_pPalmFruit;		// ヤシの実の情報
};

#endif