//===================================
//
// タイトル影絵ヘッダー[silhouette.h]
// Author 小原立暉
//
//===================================
#ifndef _SILHOUETTE_H_
#define _SILHOUETTE_H_

//***********************************
// インクルードファイル
//***********************************
#include "object2D.h"

//-----------------------------------
// クラス定義(CSilhouette)
//-----------------------------------
class CSilhouette : public CObject2D
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	typedef enum
	{
		TYPE_RUN = 0,		// 走る人型
		TYPE_CLIMBOVER,		// 乗り越える人型
		TYPE_GRIP,			// 掴む人型
		TYPE_JUMP,			// 飛び越える人型
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_APPEAR = 0,	// 出現状態
		STATE_STOP,			// 停止状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	CSilhouette();			// コンストラクタ
	~CSilhouette();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const TYPE type, const bool bRight);					// 情報の設定処理

	// 静的メンバ関数
	static CSilhouette* Create(const TYPE type, const bool bRight);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	STATE m_state;		// 状態
	TYPE m_type;		// 種類
	int m_nStateCount;	// 状態カウント
	float m_fCircumRot;	// 円周上の向き
	float m_fRotMove;	// 向きの移動量
	bool m_bRight;		// 右側かどうか

	// 静的メンバ変数
	static const char* m_apTextureFile[TYPE_MAX];			// テクスチャの名前
};

#endif