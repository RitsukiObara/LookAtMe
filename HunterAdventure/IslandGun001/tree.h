//===================================
//
// 木ヘッダー[tree.h]
// Author 小原立暉
//
//===================================
#ifndef _TREE_H_
#define _TREE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"
#include "area.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CPalmFruit;		// ヤシの実

//-----------------------------------
// クラス定義(木)
//-----------------------------------
class CTree : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_PALM = 0,		// ヤシの木
		TYPE_MAX			// この列挙型の総数
	};

	CTree(CObject::TYPE type = TYPE_TREE, PRIORITY priority = PRIORITY_BLOCK);				// オーバーロードコンストラクタ
	virtual ~CTree();		// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	virtual void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);	// 情報の設定処理
	virtual void Hit(void);					// ヒット処理

	// セット・ゲット関係
	TYPE GetType(void) const;				// 種類の取得処理
	virtual CPalmFruit* GetFruit(void);		// ヤシの実の取得処理(ヤシの木で必要)

	// 静的メンバ関数
	static CTree* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);	// 生成処理

	static CListManager<CTree*> GetList(const int nIdx);		// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ変数
	TYPE m_type;		// 種類
	int m_nAreaIdx;		// 区分の番号
	
	// 静的メンバ変数
	static CListManager<CTree*> m_aList[area::NUM_AREA];		// リスト
};

#endif