//============================================
//
// 崖捕まりヘッダー[cliffchecker.h]
// Author：小原立暉
//
//============================================
#ifndef _CLIFFCHECKER_H_			// このマクロ定義がされていなかったら
#define _CLIFFCHECKER_H_			// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object3D.h"

//--------------------------------------------
// クラス(背景クラス)
//--------------------------------------------
class CCliffChecker : public CObject3D
{
public:			// 誰でもアクセスできる

	CCliffChecker();		// コンストラクタ
	~CCliffChecker();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos);			// 情報の設定処理

	// 静的メンバ関数
	static CCliffChecker* Create(const D3DXVECTOR3& pos);	// 生成処理

private:		// 自分だけアクセスできる

};

#endif