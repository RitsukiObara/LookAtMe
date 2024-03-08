//===========================================
//
// ボスの爆発処理[boss_explosion.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "boss_explosion.h"
#include "texture.h"
#include "useful.h"

#include "mesh_sphere.h"
#include "mesh_donut.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	const float EXPLOSION_CIRCUM = 300.0f;			// 爆発の円周
	const float EXPLOSION_HEIGHT = 300.0f;			// 爆発の高さ
	const CMesh::SGrid EXPLOSION_DIVI = { 16,16 };	// 爆発の分割数
	const char* EXPLOSION_TEXTURE = "data\\TEXTURE\\BombExplosion.png";		// 爆発のテクスチャ

	const D3DXVECTOR3 SHOCK_ROT[CBossExplosion::NUM_SHOCK] =	// 衝撃の向き
	{
		D3DXVECTOR3(D3DX_PI * 0.25f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(D3DX_PI * -0.25f, D3DX_PI * 0.5f, 0.0f),
	};
	const float SHOCK_CIRCUM = 400.0f;				// 衝撃の円周
	const float SHOCK_WIDTH = 100.0f;				// 衝撃の幅
	const CMesh::SGrid SHOCK_DIVI = { 1,16 };		// 衝撃の分割数
	const char* SHOCK_TEXTURE = "data\\TEXTURE\\BossRipple.png";			// 衝撃のテクスチャ

	const float INIT_ADD_EXPLOSION = 30.0f;			// 爆発の初期拡大率
	const float INIT_ADD_SHOCK = 30.0f;				// 衝撃の初期拡大率
	const float EXPLOSION_CORRECT = 0.2f;			// 爆発の補正率
	const float SHOCK_CORRECT = 0.1f;				// 衝撃の補正率
	const float INIT_ALPHA = 1.0f;					// 透明度の初期値
	const float SUB_ALPHA = 0.01f;					// 透明度の減算量
}

//==============================
// コンストラクタ
//==============================
CBossExplosion::CBossExplosion() : CObject(CObject::TYPE_EXPLOSION, DIM_3D, CObject::PRIORITY_EFFECT)
{
	// 全ての値をクリアする
	m_pExplosion = nullptr;						// 爆発の情報
	memset(m_apShock, 0, sizeof(m_apShock));	// 衝撃の情報
	m_fAddExpl = INIT_ADD_EXPLOSION;			// 爆発の追加量
	m_fAddShock = INIT_ADD_SHOCK;				// 衝撃の追加量
	m_fAlpha = INIT_ALPHA;						// 透明度
}

//==============================
// デストラクタ
//==============================
CBossExplosion::~CBossExplosion()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CBossExplosion::Init(void)
{
	// 値を返す
	return S_OK;
}

//========================================
// 破片の終了処理
//========================================
void CBossExplosion::Uninit(void)
{
	if (m_pExplosion != nullptr)
	{ // 爆発が NULL じゃない場合

		// 爆発の終了処理
		m_pExplosion->Uninit();
		m_pExplosion = nullptr;
	}

	for (int nCnt = 0; nCnt < NUM_SHOCK; nCnt++)
	{
		if (m_apShock[nCnt] != nullptr)
		{ // 衝撃が NULL じゃない場合

			// 衝撃の終了処理
			m_apShock[nCnt]->Uninit();
			m_apShock[nCnt] = nullptr;
		}
	}

	// 本体の消去処理
	Release();
}

//=====================================
// 破片の更新処理
//=====================================
void CBossExplosion::Update(void)
{
	if (m_pExplosion !=  nullptr)
	{ // 爆発が NULL じゃない場合

		// 円周と高さを取得
		float fCircum = m_pExplosion->GetCircum();
		float fHeight = m_pExplosion->GetHeight();

		// 円を広げる
		fCircum += m_fAddExpl;
		fHeight += m_fAddExpl;

		// 円周と高さを適用
		m_pExplosion->SetCircum(fCircum);
		m_pExplosion->SetHeight(fHeight);

		// 頂点情報の設定処理
		m_pExplosion->SetVertex();
		m_pExplosion->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
	}

	for (int nCnt = 0; nCnt < NUM_SHOCK; nCnt++)
	{
		if (m_apShock[nCnt] != nullptr)
		{ // 衝撃が NULL じゃない場合

			// 円周と幅を取得
			float fCircum = m_apShock[nCnt]->GetCircum();

			// 円を広げる
			fCircum += m_fAddShock;

			// 円周と高さを適用
			m_apShock[nCnt]->SetCircum(fCircum);

			// 頂点情報の設定処理
			m_apShock[nCnt]->SetVertex();
		}
	}

	// 均等な補正処理
	useful::FrameCorrect(0.0f, &m_fAddExpl, EXPLOSION_CORRECT);
	useful::FrameCorrect(0.0f, &m_fAddShock, SHOCK_CORRECT);

	if (m_fAlpha > 0.0f)
	{ // 透明度が0超過の場合

		// 透明度を減算する
		m_fAlpha -= SUB_ALPHA;
	}
	else
	{ // 上記以外

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}
}

//=====================================
// 破片の描画処理
//=====================================
void CBossExplosion::Draw(void)
{
	if (m_pExplosion != nullptr)
	{ // 爆発が NULL じゃない場合

		// 爆発の描画処理
		m_pExplosion->Draw();
	}

	for (int nCnt = 0; nCnt < NUM_SHOCK; nCnt++)
	{
		if (m_apShock[nCnt] != nullptr)
		{ // 衝撃が NULL じゃない場合

			// 衝撃の描画処理
			m_apShock[nCnt]->Draw();
		}
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CBossExplosion::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 全ての値を設定する
	if (m_pExplosion == nullptr)
	{ // 爆発が NULL の場合

		// 爆発を生成
		m_pExplosion = CMeshSphere::Create(pos, NONE_D3DXVECTOR3, EXPLOSION_CIRCUM, EXPLOSION_HEIGHT, EXPLOSION_DIVI, PRIORITY_EFFECT);

		// 情報の設定処理
		m_pExplosion->CObject::SetType(TYPE_NONE);	// 種類
		m_pExplosion->BindTexture(CManager::Get()->GetTexture()->Regist(EXPLOSION_TEXTURE));		// テクスチャの割り当て
		m_pExplosion->SetCulling(true);				// カリング状況
		m_pExplosion->SetLighting(true);			// ライティング状況
	}

	for (int nCnt = 0; nCnt < NUM_SHOCK; nCnt++)
	{
		if (m_apShock[nCnt] == nullptr)
		{ // 衝撃が NULL の場合

			// 衝撃を生成
			m_apShock[nCnt] = CMeshDonut::Create(pos, rot + SHOCK_ROT[nCnt], SHOCK_CIRCUM, SHOCK_WIDTH, SHOCK_DIVI, PRIORITY_EFFECT);

			// 情報の設定処理
			m_apShock[nCnt]->CObject::SetType(TYPE_NONE);	// 種類
			m_apShock[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(SHOCK_TEXTURE));		// テクスチャの割り当て
			m_apShock[nCnt]->SetCulling(true);				// カリング状況
			m_apShock[nCnt]->SetLighting(true);				// ライティング状況
		}
	}

	m_fAddExpl = INIT_ADD_EXPLOSION;		// 爆発の追加量
	m_fAddShock = INIT_ADD_SHOCK;			// 衝撃の追加量
	m_fAlpha = INIT_ALPHA;					// 透明度
}

//=======================================
// 生成処理
//=======================================
CBossExplosion* CBossExplosion::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカルオブジェクトを生成
	CBossExplosion* pExpl = nullptr;	// インスタンスを生成

	if (pExpl == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pExpl = new CBossExplosion;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pExpl != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pExpl->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pExpl->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 爆発のポインタを返す
	return pExpl;
}