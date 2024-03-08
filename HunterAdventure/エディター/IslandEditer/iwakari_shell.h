//===================================
//
// イワカリの殻ヘッダー[iwakari_shell.h]
// Author 小原立暉
//
//===================================
#ifndef _IWAKARI_SHELL_H_
#define _IWAKARI_SHELL_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(イワカリの殻)
//-----------------------------------
class CIwakariShell : public CModel
{
public:			// 誰でもアクセスできる

	CIwakariShell();				// コンストラクタ
	~CIwakariShell();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);					// 情報の設定処理

	// セット・ゲット関係
	void SetLife(const int nLife);	// 体力の設定処理
	int GetLife(void) const;		// 体力の取得処理

	// 静的メンバ関数
	static CIwakariShell* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nLife;		// 体力
};

#endif