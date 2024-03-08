//============================================
//
// 金の骨UIヘッダー[gold_bone_UI.h]
// Author：小原立暉
//
//============================================
#ifndef _GOLD_BONE_UI_H_					// このマクロ定義がされていなかったら
#define _GOLD_BONE_UI_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_BONE_MARK		(3)		// 骨マークの最大値

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;		// 2Dポリゴン

//--------------------------------------------
// クラス定義(金の骨UI)
//--------------------------------------------
class CGoldBoneUI : public CObject
{
public:				// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,			// 無状態
		STATE_EXTEND,			// 拡大状態
		STATE_SHRINK,			// 縮小状態
		STATE_STOP,				// 停止状態
		STATE_MAX				// この列挙型の総数
	};

	struct SBoneMark
	{
		CObject2D* pShadow;		// 影
		CObject2D* pMark;		// マーク
		STATE state;			// 状態
	};

	CGoldBoneUI();				// コンストラクタ
	~CGoldBoneUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(void);				// 情報の設定処理

	void GetGoldBone(void);			// 金の骨のゲット処理

	// 静的メンバ関数
	static CGoldBoneUI* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ変数
	SBoneMark m_aBoneMark[MAX_BONE_MARK];		// 金の骨の情報
};

#endif