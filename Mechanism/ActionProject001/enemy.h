//===================================
//
// 敵ヘッダー[enemy.h]
// Author 小原立暉
//
//===================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CStun;			// 気絶演出

//-----------------------------------
// クラス定義(敵)
//-----------------------------------
class CEnemy : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_ITOCAN = 0,		// イトキャン
		TYPE_MACHIDORI,			// マシンドリー
		TYPE_GARREJANTE,		// ガレジャンテ
		TYPE_MAX				// この列挙型の総数
	};

	CEnemy();							// コンストラクタ
	CEnemy(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);	// オーバーロードコンストラクタ
	virtual ~CEnemy();					// デストラクタ
	void Box(void);						// コンストラクタの箱

	// リスト構造関係
	void SetPrev(CEnemy* pPrev);	// 前のポインタの設定処理
	void SetNext(CEnemy* pNext);	// 後のポインタの設定処理
	CEnemy* GetPrev(void) const;	// 前のポインタの設定処理
	CEnemy* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void) = 0;	// 更新処理
	virtual void Draw(void);		// 描画処理

	virtual void Hit(void);			// ヒット処理
	virtual void SmashHit(void);	// 吹き飛びヒット処理
	virtual void StunHit(void);		// 気絶のヒット処理
	virtual void SetData(const D3DXVECTOR3& pos, const TYPE type);				// 情報の設定処理

	// セット・ゲット関係
	CStun* GetStun(void) const;					// 気絶演出の取得処理
	void DeleteStun(void);						// 気絶状態の消去処理

	D3DXVECTOR3 GetInitPos(void) const;			// 初期位置の取得処理

	void SetMove(const D3DXVECTOR3& move);		// 移動量の設定処理
	D3DXVECTOR3 GetMove(void) const;			// 移動量の取得処理

	void SetCollSize(const D3DXVECTOR3& size);	// 当たり判定のサイズの設定処理
	D3DXVECTOR3 GetCollSize(void) const;		// 当たり判定のサイズの取得処理

	TYPE GetType(void) const;					// 種類の取得処理

	void SetStunCount(const int nCount);		// 気絶カウントの設定処理
	int GetStunCount(void) const;				// 気絶カウントの取得処理

	void SetEnableStep(const bool bStep);		// 踏みつけ状況の設定処理
	bool IsStep(void) const;					// 踏みつけ状況の取得処理

	void SetEnableCollision(const bool bColl);	// 当たり判定状況の設定処理
	bool IsCollision(void) const;				// 当たり判定状況の取得処理

	void SetEnableStun(const bool bStun);		// 気絶状況の設定処理
	bool IsStun(void) const;					// 気絶状況の取得処理

	// その他の関数
	void Gravity(void);				// 重力処理
	bool ElevationCollision(void);	// 起伏地面の当たり判定処理
	bool BlockCollision(void);		// ブロックとの当たり判定
	bool TableCollision(void);		// 台との当たり判定

	// 静的メンバ関数
	static CEnemy* Create(const D3DXVECTOR3& pos, const TYPE type);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CStun* m_pStun;			// 気絶演出のポインタ

	D3DXVECTOR3 m_posInit;	// 初期位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_sizeColl;	// 当たり判定のサイズ
	TYPE m_type;			// 種類
	int m_nStunCount;		// 気絶カウント
	bool m_bStep;			// 踏みつけ状況
	bool m_bCollision;		// 当たり判定を通るかどうか
	bool m_bStun;			// 気絶状況

	// リスト構造関係
	CEnemy* m_pPrev;	// 前へのポインタ
	CEnemy* m_pNext;	// 次へのポインタ
};

#endif