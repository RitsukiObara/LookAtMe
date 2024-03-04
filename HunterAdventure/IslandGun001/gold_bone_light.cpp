//===========================================
//
// 金の骨の光のメイン処理[gold_bone_light.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "gold_bone_light.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* FIRE_TEXTURE = "data\\TEXTURE\\BoneLight.png";		// テクスチャ
	const float FIRE_CIRCUM = 50.0f;				// 半径
	const float FIRE_HEIGHT = 10.0f;				// 高さ
	const CMesh::SGrid FIRE_DIVI = { 8,1 };			// 分割数

	const float ADD_HEIGHT = 1000.0f;				// 高さの加算数
	const float MAX_HEIGHT = 20000.0f;				// 高さの最大値
	const float ADD_EXTEND_ROT = 0.01f;				// 拡大状態中の向きの加算数
	const float SUB_CIRCUM = 2.0f;					// 円周の減算量
	const float INIT_POS_Y = -500.0f;				// 初期位置(Y軸)
	const float TEX_V_MEMORY = 1.0f / MAX_HEIGHT;	// テクスチャ(V値)の1メモリ分の値
}

//==============================
// コンストラクタ
//==============================
CGoldBoneLight::CGoldBoneLight() : CMeshCylinder(CObject::PRIORITY_ENTITY)
{
	// 種類を設定する
	CObject::SetType(TYPE_NONE);

	// 全ての値をクリアする
	m_state = STATE_EXTEND;		// 状態
}

//==============================
// デストラクタ
//==============================
CGoldBoneLight::~CGoldBoneLight()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CGoldBoneLight::Init(void)
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
void CGoldBoneLight::Uninit(void)
{
	// 終了処理
	CMeshCylinder::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CGoldBoneLight::Update(void)
{
	switch (m_state)
	{
	case CGoldBoneLight::STATE_EXTEND:

		// 伸び状態処理
		Extend();

		// テクスチャ座標の設定処理
		SetTex();

		break;

	case CGoldBoneLight::STATE_SHRINK:

		// 縮小処理
		Shrink();

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 頂点座標の設定処理
	SetVertex();
}

//=====================================
//ブロックの描画処理
//=====================================
void CGoldBoneLight::Draw(void)
{
	// 描画処理
	CMeshCylinder::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CGoldBoneLight::SetData(const D3DXVECTOR3& pos)
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
	m_state = STATE_EXTEND;		// 状態
}

//=======================================
// 生成処理
//=======================================
CGoldBoneLight* CGoldBoneLight::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CGoldBoneLight* pFire = nullptr;		// インスタンスを生成

	if (pFire == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pFire = new CGoldBoneLight;
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
// 状態の設定処理
//=======================================
void CGoldBoneLight::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CGoldBoneLight::STATE CGoldBoneLight::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 伸び状態処理
//=======================================
void CGoldBoneLight::Extend(void)
{
	// 向きと高さを取得
	D3DXVECTOR3 rot = GetRot();
	float fHeight = GetHeight();

	// 高さを加算
	fHeight += ADD_HEIGHT;

	// 向きを加算する
	rot.y += ADD_EXTEND_ROT;

	if (fHeight >= MAX_HEIGHT)
	{ // 高さが最大値以上になった場合

		// 高さを補正する
		fHeight = MAX_HEIGHT;
	}

	// 向きの正規化
	useful::RotNormalize(&rot.y);

	// 高さを適用
	SetRot(rot);
	SetHeight(fHeight);
}

//=======================================
// 縮み状態処理
//=======================================
void CGoldBoneLight::Shrink(void)
{
	// 円周を取得する
	float fCircum = GetCircum();

	// 円周を減算する
	fCircum -= SUB_CIRCUM;

	if (fCircum <= 0.0f)
	{ // 円周が0以下になった場合

		// 円周を0にする
		fCircum = 0.0f;
	}

	// 円周を適用する
	SetCircum(fCircum);
}

//=======================================
// テクスチャ座標の設定処理
//=======================================
void CGoldBoneLight::SetTex(void)
{
	// テクスチャの高さ設定処理
	float fHeight = GetHeight();

	// テクスチャ座標の設定処理
	SetVtxTexture(TEX_V_MEMORY * fHeight);
}