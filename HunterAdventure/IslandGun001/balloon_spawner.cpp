//===========================================
//
// 風船のメイン処理[balloon.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "balloon_spawner.h"
#include "renderer.h"
#include "useful.h"

#include "balloon.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const int INTERVAL_COUNT = 80;						// 風船が再出現するカウント
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CBalloonSpawner*> CBalloonSpawner::m_list = {};			// リスト情報

//==============================
// コンストラクタ
//==============================
CBalloonSpawner::CBalloonSpawner() : CObject(CObject::TYPE_TARGET, DIM_3D, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pBalloon = nullptr;		// 風船のモデル
	m_pos = NONE_D3DXVECTOR3;	// 位置
	m_nIntervalCount = 0;		// 風船の間隔カウント

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CBalloonSpawner::~CBalloonSpawner()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CBalloonSpawner::Init(void)
{
	// 値を返す
	return S_OK;
}

//========================================
// 破片の終了処理
//========================================
void CBalloonSpawner::Uninit(void)
{
	if (m_pBalloon != nullptr)
	{ // 風船が NULL じゃない場合

		// 風船の終了処理
		m_pBalloon->Uninit();
		m_pBalloon = nullptr;
	}

	// 本体の終了処理
	Release();

	// 引き抜き処理
	m_list.Pull(this);
}

//=====================================
// 破片の更新処理
//=====================================
void CBalloonSpawner::Update(void)
{
	if (m_pBalloon != nullptr)
	{ // 風船が NULL じゃない場合

		// 風船の更新処理
		m_pBalloon->Update();
	}
	else
	{ // 上記以外

		// 間隔カウントを加算する
		m_nIntervalCount++;

		if (m_nIntervalCount >= INTERVAL_COUNT)
		{ // 間隔カウントが一定数を超えた場合

			// 風船を生成
			m_pBalloon = CBalloon::Create(m_pos);

			// 間隔カウントを0にする
			m_nIntervalCount = 0;
		}
	}
}

//=====================================
// 破片の描画処理
//=====================================
void CBalloonSpawner::Draw(void)
{
	if (m_pBalloon != nullptr)
	{ // 風船が NULL じゃない場合

		// 風船の描画処理
		m_pBalloon->Draw();
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CBalloonSpawner::SetData(const D3DXVECTOR3& pos)
{
	// 全ての値を設定する
	m_pos = pos;			// 位置
	m_nIntervalCount = 0;	// 間隔カウント

	// 風船を生成
	m_pBalloon = CBalloon::Create(m_pos);
}

//=======================================
// ヒット処理
//=======================================
void CBalloonSpawner::Hit(void)
{
	if (m_pBalloon != nullptr)
	{ // 風船が NULL じゃない場合

		// 終了処理
		m_pBalloon->Uninit();
		m_pBalloon = nullptr;
	}
}

//=======================================
// 生成処理
//=======================================
CBalloonSpawner* CBalloonSpawner::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CBalloonSpawner* pTarget = nullptr;	// インスタンスを生成

	if (pTarget == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pTarget = new CBalloonSpawner;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pTarget != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pTarget->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pTarget->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 的のポインタを返す
	return pTarget;
}

//=======================================
// 風船の取得処理
//=======================================
CBalloon* CBalloonSpawner::GetBalloon(void) const
{
	// 風船の情報を返す
	return m_pBalloon;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CBalloonSpawner*> CBalloonSpawner::GetList(void)
{
	// リスト構造の情報を返す
	return m_list;
}