//===================================
//
// 障害物ヘッダー[obstacle.h]
// Author 小原立暉
//
//===================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(障害物)
//-----------------------------------
class CObstacle : public CModel
{
public:			// 誰でもアクセスできる

	//-----------------------------------
	// 種類
	//-----------------------------------
	typedef enum
	{
		TYPE_HYDRANT = 0,			// 消火栓
		TYPE_SIGNPOLE,				// 標識
		TYPE_DRUM,					// ドラム缶
		TYPE_WOODBOX,				// 木箱
		TYPE_LAMP,					// 街灯(光源)
		TYPE_POLE,					// 街灯(支柱)
		TYPE_MAX					// この列挙型の総数
	}TYPE;

	CObstacle();															// コンストラクタ
	CObstacle(CObject::TYPE type, PRIORITY priority = PRIORITY_BLOCK);		// オーバーロードコンストラクタ
	virtual ~CObstacle();													// デストラクタ
	void Box(void);															// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);			// 初期化処理
	virtual void Uninit(void);			// 終了処理
	virtual void Update(void);			// 更新処理
	virtual void Draw(void);			// 描画処理

	virtual bool Hit(void);				// ヒット処理

	virtual void SetData(const D3DXVECTOR3& pos, const TYPE type);		// 情報の設定処理

	// セット・ゲット関係
	void SetType(const TYPE type);		// 種類の設定処理
	TYPE GetType(void) const;			// 種類の取得処理

	void SetCollision(const bool bCollision);		// 当たり判定の設定処理
	bool GetCollision(void) const;					// 当たり判定の取得処理

	void SetAttack(const bool bAttack);				// アクションの設定処理
	bool GetAttack(void) const;						// アクションの取得処理

	void SetRange(const bool bRange);	// 攻撃範囲内かどうかの設定処理
	bool GetRange(void) const;			// 攻撃範囲内かどうかの取得処理

// デバッグ処理
#ifdef _DEBUG

	bool GetDelete(void) const;			// 消去状況の取得処理
	void SetDelete(const bool bDelete);	// 消去状況の設定処理

#endif 

	// 静的メンバ関数
	static CObstacle* Create(const D3DXVECTOR3& pos, const TYPE type);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void ShadowPosSet(void);			// 影の位置設定処理

	// メンバ変数
	TYPE m_type;						// 種類
	int m_nShadowIdx;					// 影のインデックス
	bool m_bCollision;					// 当たり判定が機能するか
	bool m_bAttack;						// 攻撃出来るかどうか
	bool m_bRangeWithin;				// 攻撃範囲内かどうか

// デバッグ処理
#ifdef _DEBUG

	bool m_bDelete;			// 消去状況
#endif 

};

#endif