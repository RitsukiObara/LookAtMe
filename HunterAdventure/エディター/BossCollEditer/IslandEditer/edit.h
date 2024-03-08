//===================================
//
// エディットヘッダー[edit.h]
// Author 小原立暉
//
//===================================
#ifndef _EDIT_H_
#define _EDIT_H_

//***********************************
// インクルードファイル
//***********************************
#include "object.h"
#include "motion.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CBoss;			// ボス
class CBossCollision;	// ボスの当たり判定

//-----------------------------------
// クラス定義(エディット)
//-----------------------------------
class CEdit : public CObject
{
public:			// 誰でもアクセスできる

	CEdit();					// コンストラクタ
	~CEdit();					// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(void);				// 情報の設定処理
	void CollMatrix(void);			// 当たり判定のマトリックス

	// セット・ゲット関係
	CBoss* GetBoss(void);			// ボスの取得処理
	CBossCollision* GetColl(const int nIdx);		// 当たり判定の情報の取得処理

	// 静的メンバ関数
	static CEdit* Create(void);		// 生成処理

private:		// 自分だけアクセスできる
	
	// メンバ関数
	void Positioning(void);			// 位置設定処理
	void SetIdx(void);				// 番号の設定処理
	void CorrectIdx(void);			// 番号の補正処理
	void AddCollision(void);		// 当たり判定の追加処理
	void NumCollDisp(void);			// 当たり判定の総数表示処理
	void SetRadius(void);			// 当たり判定の半径の設定処理
	void SetWeakness(void);			// 当たり判定の弱点状況の設定処理

	// メンバ変数
	CBoss* m_pBoss;		// ボス
	CBossCollision* m_apColl[MAX_PARTS];	// 当たり判定の球
	int m_nSetPartNumber;		// 現在設定中のパーツの番号
	int m_nSetCollNumber;		// 現在設定中の当たり判定の番号
};

#endif