//===================================
//
// 新記録ヘッダー[new_record.h]
// Author 小原立暉
//
//===================================
#ifndef _NEW_RECORD_H_			// このマクロ定義がされていなかったら
#define _NEW_RECORD_H_			// 2重インクルード防止のマクロを設定する

//-----------------------------------
// インクルードファイル
//-----------------------------------
#include "object2D.h"

//-----------------------------------
// クラス定義(新記録)
//-----------------------------------
class CNewRecord : public CObject2D
{
public:			// 誰でもアクセスできる

	CNewRecord();		// コンストラクタ
	~CNewRecord();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3 pos);			// 情報の設定処理

	// 静的メンバ関数
	static CNewRecord* Create(const D3DXVECTOR3 pos);	// 生成処理

private:		// 自分だけアクセスできる

};

#endif