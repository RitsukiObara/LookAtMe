//===================================
//
// 金の骨ヘッダー[gold_bone.h]
// Author 小原立暉
//
//===================================
#ifndef _GOLD_BONE_H_
#define _GOLD_BONE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CGoldBoneLight;		// 金の骨の光

//-----------------------------------
// クラス定義(金の骨)
//-----------------------------------
class CGoldBone : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_GET,			// 取得状態
		STATE_UP,			// 上昇状態
		STATE_ALTERSET,		// 祭壇到着状態
		STATE_MAX			// この列挙型の総数
	};

	CGoldBone();			// コンストラクタ
	~CGoldBone();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);				// 情報の設定処理
	void Hit(void);					// ヒット処理

	// セット・ゲット関係
	void SetState(const STATE state);		// 状態の設定処理
	STATE GetState(void) const;				// 状態の取得処理
	void SetLight(void);					// 光の設定処理
	CGoldBoneLight* GetLight(void);			// 光の取得処理

	// 静的メンバ関数
	static CGoldBone* Create(const D3DXVECTOR3& pos);	// 生成処理

	static CListManager<CGoldBone*> GetList(void);		// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Cycle(void);		// 回転処理
	void Get(void);			// 取得処理
	void Up(void);			// 上昇処理
	void AlterSet(void);	// 祭壇設定処理

	// メンバ関数
	CGoldBoneLight* m_pLight;	// 光の情報

	STATE m_state;			// 状態
	int m_nPoleIdx;			// 石柱のインデックス
	float m_fDestHeight;	// 目的の高さ
	float m_fRotMove;		// 向きの移動量

	// 静的メンバ変数
	static CListManager<CGoldBone*> m_list;		// リスト
};

#endif