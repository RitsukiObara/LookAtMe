//============================================
//
// 爆弾の爆発ヘッダー[bomb_explosion.h]
// Author：小原立暉
//
//============================================
#ifndef _BOMB_EXPLOSION_H_					// このマクロ定義がされていなかったら
#define _BOMB_EXPLOSION_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "mesh_sphere.h"
#include "list_manager.h"

//--------------------------------------------
// クラス定義(爆発)
//--------------------------------------------
class CBombExplosion : public CMeshSphere
{
public:				// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_EXTEND = 0,	// 拡大状態
		STATE_CLEAR,		// 透明化状態
		STATE_MAX			// この列挙型の総数
	};

	CBombExplosion();				// コンストラクタ
	~CBombExplosion();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新処理
	void Draw(void) override;			// 描画処理

	void SetData(const D3DXVECTOR3& pos);	// 情報の設定処理

	// 静的メンバ関数
	static CBombExplosion* Create(const D3DXVECTOR3& pos);	// 生成処理
	static CListManager<CBombExplosion*> GetList(void);		// リストの取得処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void AddScale(const float fAdd);		// 拡大処理

	// メンバ変数
	STATE m_state;				// 状態
	int m_nStateCount;			// 状態カウント
	float m_fAlpha;				// 透明度

	// 静的メンバ変数
	static CListManager<CBombExplosion*> m_list;	// リスト
};

#endif