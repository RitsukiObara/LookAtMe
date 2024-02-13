//============================================
//
// キャラクターブラーヘッダー[chara_blur.h]
// Author：小原立暉
//
//============================================
#ifndef _CHARA_BLUR_H_					// このマクロ定義がされていなかったら
#define _CHARA_BLUR_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "character.h"

//--------------------------------------------
// クラス(キャラクターブラークラス)
//--------------------------------------------
class CCharaBlur : public CCharacter
{
public:			// 誰でもアクセスできる

	CCharaBlur(PRIORITY priority = PRIORITY_EFFECT);		// オーバーロードコンストラクタ
	~CCharaBlur();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const int nNumModel, const D3DXCOLOR& col, const int nLife);		// 情報の設定処理
	void SetModelData(const int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile& filedata, const int nParentIdx);		// モデルの情報設定処理

	// 静的メンバ関数
	static CCharaBlur* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const int nNumModel, const D3DXCOLOR& col, const int nLife, const PRIORITY priority);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXCOLOR m_col;		// 色
	int m_nLife;			// 寿命
	float m_fSub;			// 減算量
};

#endif