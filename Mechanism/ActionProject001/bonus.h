//============================================
//
// ボーナスヘッダー[bonus.h]
// Author：小原立暉
//
//============================================
#ifndef _BONUS_H_					// このマクロ定義がされていなかったら
#define _BONUS_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define NUM_BONUS_DIGIT			(3)			// ボーナスの桁数

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CBillboardAnim;		// ビルボード

//--------------------------------------------
// クラス定義(ボーナス)
//--------------------------------------------
class CBonus : public CObject
{
public:				// 誰でもアクセスできる

	CBonus();		// コンストラクタ
	~CBonus();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos);		// 情報の設定処理
	
	// セット・ゲット関係
	int GetBonus(void) const;	// ボーナスの取得処理
	void AddBonus(void);		// ボーナスの加算処理

	// 静的メンバ関数
	static CBonus* Get(void);		// 取得処理
	static CBonus* Create(const D3DXVECTOR3& pos);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void SetShift(void);		// ずらす幅の設定処理

	// メンバ変数
	CBillboardAnim* m_aNumber[NUM_BONUS_DIGIT];		// 数字の情報
	D3DXVECTOR3 m_posShift;		// ずらす幅
	int m_nBonusCombo;			// ボーナスのコンボ数

	// 静的メンバ変数
	static CBonus* m_pBonus;	// ボーナスの情報
};

#endif