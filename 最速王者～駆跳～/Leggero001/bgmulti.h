//============================================
//
// 多重背景ヘッダー[bgmulti.h]
// Author：小原立暉
//
//============================================
#ifndef _BGMULTI_H_			//このマクロ定義がされていなかったら
#define _BGMULTI_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"
#include "scrollobject.h"

//--------------------------------------------
// クラス(多重背景クラス)
//--------------------------------------------
class CBgMulti : public CObject
{
public:			// 誰でもアクセスできる

	// 背景の種類
	typedef enum
	{
		TYPE_BACK = 0,		// 背景
		TYPE_LIGHT,			// 光
		TYPE_STAR,			// 星空
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	CBgMulti();				// コンストラクタ
	~CBgMulti();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	//　セット・ゲット関係
	CScroll* GetBgMulti(int nIdx) const;	// 多重背景の取得処理

	void SetPos(const D3DXVECTOR3& pos);	// 位置の設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置の取得処理

	// 静的メンバ関数
	static CBgMulti* Create(void);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CScroll *m_apBgMulti[TYPE_MAX];							// 多重背景の情報

	// 静的メンバ変数
	static const char *m_apTextureName[TYPE_MAX];			// テクスチャの名前
	static const D3DXVECTOR2 m_aSpeedInfo[TYPE_MAX];		// スピードの情報
};

#endif