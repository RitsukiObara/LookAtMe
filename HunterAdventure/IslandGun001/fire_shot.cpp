//===========================================
//
// 炎攻撃のメイン処理[fire_shot.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "fire_shot.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* FIRE_TEXTURE = "data\\TEXTURE\\BombExplosion.png";		// テクスチャ
	const float FIRE_CIRCUM = 200.0f;				// 半径
	const float FIRE_HEIGHT = 100.0f;				// 高さ
	const CMesh::SGrid FIRE_DIVI = { 8,1 };			// 分割数

	const float ADD_HEIGHT = 100.0f;				// 高さの加算数
	const float MAX_HEIGHT = 6000.0f;				// 高さの最大値
	const float ADD_EXTEND_ROT = 0.01f;				// 拡大状態中の向きの加算数
	const float ADD_SHRINK_ROT = 0.001f;			// 縮小状態中の向きの加算数
	const float SUB_CIRCUM = 20.0f;					// 円周の減算量
	const float INIT_POS_Y = -500.0f;				// 初期位置(Y軸)
	const float TEX_V_MEMORY = 1.0f / MAX_HEIGHT;	// テクスチャ(V値)の1メモリ分の値
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CFireShot*> CFireShot::m_list = {};		// リスト

//==============================
// コンストラクタ
//==============================
CFireShot::CFireShot() : CMeshCylinder(CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_state = STATE_EXTEND;			// 状態

	// 種類を設定する
	CObject::SetType(TYPE_FIRESHOT);

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CFireShot::~CFireShot()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CFireShot::Init(void)
{
	if (FAILED(CMeshCylinder::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CFireShot::Uninit(void)
{
	// 終了処理
	CMeshCylinder::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
//ブロックの更新処理
//========================================
void CFireShot::Update(void)
{
	switch (m_state)
	{
	case CFireShot::STATE_EXTEND:

		// 伸び状態処理
		Extend();

		break;

	case CFireShot::STATE_SHRINK:

		// 縮み状態処理
		if (Shrink() == true)
		{ // 縮小しきった場合

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

	// テクスチャ座標の設定処理
	SetTex();
}

//=====================================
//ブロックの描画処理
//=====================================
void CFireShot::Draw(void)
{
	// 描画処理
	CMeshCylinder::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CFireShot::SetData(const D3DXVECTOR3& pos)
{
	// 炎の位置
	D3DXVECTOR3 posFire = D3DXVECTOR3(pos.x, INIT_POS_Y, pos.z);

	// 情報の設定処理
	CMeshCylinder::SetData
	(
		posFire,
		NONE_D3DXVECTOR3,
		FIRE_CIRCUM,
		FIRE_HEIGHT,
		FIRE_DIVI
	);

	BindTexture(CManager::Get()->GetTexture()->Regist(FIRE_TEXTURE));		// テクスチャの割り当て処理
	SetLighting(true);		// ライティングの設定処理
	SetCulling(true);		// カリングの設定処理

	// 頂点座標の設定処理
	SetVertex();

	// 全ての値を設定する
	m_state = STATE_EXTEND;			// 状態
}

//=======================================
// 生成処理
//=======================================
CFireShot* CFireShot::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CFireShot* pFire = nullptr;		// インスタンスを生成

	if (pFire == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pFire = new CFireShot;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pFire != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 情報の設定処理
		pFire->SetData(pos);

		// 初期化処理
		if (FAILED(pFire->Init()))
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

	// 炎のポインタを返す
	return pFire;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CFireShot*> CFireShot::GetList(void)
{
	// リストの情報を返す
	return m_list;
}

//=======================================
// 状態の設定処理
//=======================================
void CFireShot::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CFireShot::STATE CFireShot::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 伸び状態処理
//=======================================
void CFireShot::Extend(void)
{
	// 向きと高さを取得
	D3DXVECTOR3 rot = GetRot();
	float fHeight = GetHeight();

	// 高さを加算する
	fHeight += ADD_HEIGHT;

	if (fHeight >= MAX_HEIGHT)
	{ // 高さが最大値以上になった場合

		// 高さを補正する
		fHeight = MAX_HEIGHT;

		// 縮小状態にする
		m_state = STATE_SHRINK;
	}

	// 向きを加算する
	rot.y += ADD_EXTEND_ROT;

	// 向きの正規化
	useful::RotNormalize(&rot.y);

	// 向きと高さを適用する
	SetRot(rot);
	SetHeight(fHeight);
}

//=======================================
// 縮み状態処理
//=======================================
bool CFireShot::Shrink(void)
{
	// 向きと円周を取得する
	D3DXVECTOR3 rot = GetRot();
	float fCircum = GetCircum();
	bool bFinish = false;

	// 円周を減算する
	fCircum -= SUB_CIRCUM;

	if (fCircum <= 0.0f)
	{ // 円周が0以下になった場合

		// 円周を0にする
		fCircum = 0.0f;

		// 終了の合図を出す
		bFinish = true;
	}

	// 向きを加算する
	rot.y += fCircum * ADD_SHRINK_ROT;

	// 向きの正規化
	useful::RotNormalize(&rot.y);

	// 向きと円周を適用する
	SetRot(rot);
	SetCircum(fCircum);

	// 終了状況を返す
	return bFinish;
}

//=======================================
// テクスチャ座標の設定処理
//=======================================
void CFireShot::SetTex(void)
{
	// テクスチャの高さ設定処理
	float fHeight = GetHeight();

	// テクスチャ座標の設定処理
	SetVtxTexture(TEX_V_MEMORY * fHeight);
}