//===================================
//
// フィールドヘッダー[field.h]
// Author 小原立暉
//
//===================================
#ifndef _FIELD_H_
#define _FIELD_H_

//***********************************
// インクルードファイル
//***********************************
#include "object3D.h"

//--------------------------------------------
// クラス(フィールドクラス)
//--------------------------------------------
class CField : public CObject3D
{
public:			// 誰でもアクセスできる

	CField();				// コンストラクタ
	~CField();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);			// 情報の設定処理

	// 静的メンバ関数
	static CField* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	// 生成処理

private:

};

#endif