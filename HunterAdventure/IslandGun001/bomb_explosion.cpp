//===========================================
//
// 爆弾の爆発のメイン処理[bomb_explosion.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "bomb_explosion.h"
#include "texture.h"
#include "area.h"
#include "useful.h"

#include "collision.h"
#include "rock.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	const char* BOMB_TEXTURE = "data\\TEXTURE\\BombExplosion.png";		// 爆発のテクスチャ
	const int DIVI_CIRCUM = 16;				// 爆発の円周の分割数
	const int DIVI_HEIGHT = 16;				// 爆発の縦の分割数
	const float CIRCUM = 100.0f;			// 円周の大きさ
	const float HEIGHT = 100.0f;			// 縦の大きさ

	const float EXTEND_SCALE = 40.0f;		// 拡大状態のスケール加算数
	const int SUB_ALPHA_COUNT = 15;			// 透明度を減算し始めるカウント数

	const float CLEAR_SCALE = 20.0f;		// 透明化状態のスケール加算数
	const float SUB_ALPHA = 0.15f;			// 透明度の減算量
	const int DEATH_COUNT = 5;				// 消すまでのカウント数
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CBombExplosion*> CBombExplosion::m_list = {};			// リスト

//==============================
// コンストラクタ
//==============================
CBombExplosion::CBombExplosion() : CMeshSphere(PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_state = STATE_EXTEND;	// 状態
	m_nStateCount = 0;		// 状態カウント
	m_fAlpha = 0.0f;		// 透明度
	m_nAreaIdx = 0;			// 区分の番号

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CBombExplosion::~CBombExplosion()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CBombExplosion::Init(void)
{
	if (FAILED(CMeshSphere::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 種類を爆発に設定
	CObject::SetType(TYPE_BOMBEXPLOSION);

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CBombExplosion::Uninit(void)
{
	// 終了処理
	CMeshSphere::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
//ブロックの更新処理
//========================================
void CBombExplosion::Update(void)
{
	// カウントを加算する
	m_nStateCount++;

	switch (m_state)
	{
	case CBombExplosion::STATE_EXTEND:

		// 拡大処理
		AddScale(EXTEND_SCALE);

		if (m_nStateCount >= SUB_ALPHA_COUNT)
		{ // 状態カウントが一定以上になった場合

			// 透明化状態にする
			m_state = STATE_CLEAR;

			// 状態カウントをリセット
			m_nStateCount = 0;
		}

		break;

	case CBombExplosion::STATE_CLEAR:

		// 拡大処理
		AddScale(CLEAR_SCALE);

		// 透明度を下げる
		m_fAlpha -= SUB_ALPHA;

		// 色の設定処理
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

		if (m_nStateCount >= DEATH_COUNT)
		{ // 状態カウントが一定以上になった場合

			int nIdx = 0;

			for (int nCnt = 0; nCnt < area::NUM_COLL; nCnt++)
			{
				nIdx = m_nAreaIdx + area::COLL_ADD_IDX[nCnt];

				if (area::IndexCheck(nIdx) == true)
				{ // 区分内の場合

					// 岩の当たり判定
					collision::ExplosionHitToRock(GetPos(), GetCircum(), GetHeight(), nIdx);
				}
			}

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 頂点座標の設定処理
	SetVertex();

	// 敵の当たり判定
	collision::ExplosionHitToEnemy(GetPos(), GetCircum(), GetHeight());
}

//=====================================
//ブロックの描画処理
//=====================================
void CBombExplosion::Draw(void)
{
	// 描画処理
	CMeshSphere::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CBombExplosion::SetData(const D3DXVECTOR3& pos)
{
	SGrid gridBomb;
	//SGrid gridSmoke;

	// 方眼情報を設定する
	gridBomb.nWidth = DIVI_CIRCUM;
	gridBomb.nHeight = DIVI_HEIGHT;

	// 情報の設定処理
	CMeshSphere::SetData(pos, NONE_D3DXVECTOR3, CIRCUM, HEIGHT, gridBomb);

	// テクスチャの割り当て処理
	BindTexture(CManager::Get()->GetTexture()->Regist(BOMB_TEXTURE));

	SetLighting(true);		// ライティング状況
	SetCulling(true);		// カリング状況

	// 全ての値を設定する
	m_state = STATE_EXTEND;					// 状態
	m_nStateCount = 0;						// 状態カウント
	m_fAlpha = 1.0f;						// 透明度

	// 区分の番号を設定する
	m_nAreaIdx = area::SetFieldIdx(GetPos());
	int nIdx = 0;

	for (int nCnt = 0; nCnt < area::NUM_COLL; nCnt++)
	{
		nIdx = m_nAreaIdx + area::COLL_ADD_IDX[nCnt];

		if (area::IndexCheck(nIdx) == true)
		{ // 区分内の場合

			// 岩の当たり判定
			collision::ExplosionHitToRock(GetPos(), GetCircum(), GetHeight(), nIdx);
		}
	}
}

//=======================================
// 生成処理
//=======================================
CBombExplosion* CBombExplosion::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CBombExplosion* pBomb = nullptr;			// 爆弾花を生成

	if (pBomb == nullptr)
	{ // オブジェクトが NULL の場合

		// ブロックを生成する
		pBomb = new CBombExplosion;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBomb != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 情報の設定処理
		pBomb->SetData(pos);

		// 初期化処理
		if (FAILED(pBomb->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 爆弾のポインタを返す
	return pBomb;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CBombExplosion*> CBombExplosion::GetList(void)
{
	// リストマネージャーを返す
	return m_list;
}

//=======================================
// 拡大処理
//=======================================
void CBombExplosion::AddScale(const float fAdd)
{
	// 幅・高さを取得
	float fCircum = GetCircum();
	float fHeight = GetHeight();

	// 球を広げる
	fCircum += fAdd;
	fHeight += fAdd;

	// 結果を適用
	SetCircum(fCircum);
	SetHeight(fHeight);
}