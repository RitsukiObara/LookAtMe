//===================================
//
// モデル影ヘッダー[shadowModel.h]
// Author 小原立暉
//
//===================================
#ifndef _SHADOWMODEL_H_				//このマクロ定義がされていなかったら
#define _SHADOWMODEL_H_				//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"
#include "model.h"

//-----------------------------------
// クラス定義(影)
//-----------------------------------
class CShadowModel : public CModel
{
public:			// 誰でもアクセスできる

	CShadowModel();			// コンストラクタ
	~CShadowModel();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新処理
	void Draw(void) override;			// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile file, const D3DXMATRIX& mtx);		// 情報の設定処理
	void SetPosShadow(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);		// 影の位置設定処理

	// 静的メンバ関数
	static CShadowModel* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile file, const D3DXMATRIX& mtx);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXMATRIX m_mtxParent;			// 親のワールドマトリックス
};


#endif