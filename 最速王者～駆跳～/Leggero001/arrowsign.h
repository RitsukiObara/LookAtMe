//===================================
//
// 矢印看板ヘッダー[arrowsign.h]
// Author 小原立暉
//
//===================================
#ifndef _ARROWSIGN_H_
#define _ARROWSIGN_H_

//***********************************
// インクルードファイル
//***********************************
#include "object.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CModel;			// モデル

//-----------------------------------
// クラス定義(矢印看板)
//-----------------------------------
class CArrowSign : public CObject
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	typedef enum
	{
		TYPE_POLE = 0,		// ポール
		TYPE_ARROW,			// 矢印
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	CArrowSign();															// コンストラクタ
	CArrowSign(CObject::TYPE type, PRIORITY priority = PRIORITY_BLOCK);		// オーバーロードコンストラクタ
	virtual ~CArrowSign();													// デストラクタ
	void Box(void);															// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rotPole, const D3DXVECTOR3& rotSign);		// 情報の設定処理

	// 静的メンバ関数
	static CArrowSign* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rotPole, const D3DXVECTOR3& rotSign);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void SetDataPole(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// 柱の情報の設定処理
	void SetDataArrow(const D3DXVECTOR3& rot);										// 矢印の情報の設定処理

	// メンバ変数
	CModel* m_apArrow[TYPE_MAX];		// モデルの情報
};

#endif