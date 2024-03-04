//===================================
//
// 風船スポナーヘッダー[balloon_spawner.h]
// Author 小原立暉
//
//===================================
#ifndef _BALLOON_SPAWNER_H_
#define _BALLOON_SPAWNER_H_

//***********************************
// インクルードファイル
//***********************************
#include "object.h"
#include "list_manager.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CBalloon;			// 風船

//-----------------------------------
// クラス定義(風船スポナー)
//-----------------------------------
class CBalloonSpawner : public CObject
{
public:			// 誰でもアクセスできる

	CBalloonSpawner();		// コンストラクタ
	~CBalloonSpawner();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);				// 情報の設定処理
	void Hit(void);					// ヒット処理

	// セット・ゲット関係
	CBalloon* GetBalloon(void) const;	// 風船の取得処理

	// 静的メンバ関数
	static CBalloonSpawner* Create(const D3DXVECTOR3& pos);		// 生成処理

	static CListManager<CBalloonSpawner*> GetList(void);		// リストの取得処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CBalloon* m_pBalloon;	// 風船の情報

	D3DXVECTOR3 m_pos;		// 位置
	int m_nIntervalCount;	// 風船の間隔カウント

	// 静的メンバ変数
	static CListManager<CBalloonSpawner*> m_list;		// リスト情報
};

#endif