//============================================
//
// 弾ヘッダー[bullet.h]
// Author：小原立暉
//
//============================================
#ifndef _BULLET_H_			//このマクロ定義がされていなかったら
#define _BULLET_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define BULLET_SIZE		(D3DXVECTOR3(40.0f, 20.0f, 0.0f))			// 弾のサイズ

//--------------------------------------------
// クラス(弾クラス)
//--------------------------------------------
class CBullet : public CBillboard
{
public:			// 誰でもアクセスできる

	// 弾の種類
	typedef enum
	{
		TYPE_PLAYER = 0,	// プレイヤーの弾
		TYPE_ENEMY,			// 敵の弾
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	CBullet();				// コンストラクタ
	~CBullet();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fMoveRot, TYPE type);			// 情報の設定処理

	// セット・ゲット関係
	void SetMoveRot(const float fMoveRot);			// 移動方向の設定処理
	float GetMoveRot(void) const;					// 移動方向の取得処理

	void SetMove(const D3DXVECTOR3& move);			// 移動量の設定処理
	D3DXVECTOR3 GetMove(void) const;				// 移動量の取得処理

	// 静的メンバ関数
	static CBullet* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fMoveRot, TYPE type);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Move(void);		// 移動処理
	void Impact(const D3DXVECTOR3& pos);			// 着弾処理

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	TYPE m_type;			// 種類
	int m_nLife;			// 寿命
	float m_fMoveRot;		// 移動方向
};

#endif