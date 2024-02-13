//============================================
//
// 背景モデルヘッダー[bgmodel.h]
// Author：小原立暉
//
//============================================
#ifndef _BGMODEL_H_			//このマクロ定義がされていなかったら
#define _BGMODEL_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "model.h"
#include "objectX.h"

//--------------------------------------------
// クラス(敵クラス)
//--------------------------------------------
class CBgModel : public CModel
{
public:			// 誰でもアクセスできる

	CBgModel();				// コンストラクタ
	~CBgModel();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const CXFile::TYPE type);				// 情報の設定処理

	// 静的メンバ関数
	static CBgModel* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const CXFile::TYPE type);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CXFile::TYPE m_type;				// 種類
	D3DXMATERIAL m_aMat[MAX_MATERIAL];	// マトリックスへのポインタ
};

#endif