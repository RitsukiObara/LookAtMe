//===========================================
//
// チュートリアルマップのメイン処理[tutorial_map.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "tutorial_map.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "mesh_wall.h"
#include "mesh_field.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const D3DXVECTOR3 WALL_POS[CTutorialMap::NUM_WALL] =		// 壁の位置
	{
		D3DXVECTOR3(2500.0f,0.0f,0.0f),
		D3DXVECTOR3(-2500.0f,0.0f,0.0f),
		D3DXVECTOR3(0.0f,0.0f,-2500.0f),
	};
	const D3DXVECTOR3 WALL_ROT[CTutorialMap::NUM_WALL] =		// 壁の向き
	{
		D3DXVECTOR3(0.0f,D3DX_PI * 0.5f,0.0f),
		D3DXVECTOR3(0.0f,D3DX_PI * -0.5f,0.0f),
		D3DXVECTOR3(0.0f,D3DX_PI,0.0f),
	};
	const float WALL_WIDTH = 5000.0f;			// 壁の幅
	const float WALL_HEIGHT = 2000.0f;			// 壁の高さ
	const CMesh::SGrid WALL_DIVI = { 1,1 };		// 壁の分割数
	const char* WALL_TEXTURE = "data\\TEXTURE\\TutorialWall.png";	// 壁のテクスチャ

	const D3DXVECTOR3 CEIL_POS = D3DXVECTOR3(0.0f, 2000.0f, 0.0f);	// 天井の位置
}

//==============================
// コンストラクタ
//==============================
CTutorialMap::CTutorialMap() : CObject(CObject::TYPE_TUTORIALMAP, DIM_3D, CObject::PRIORITY_BG)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < NUM_WALL; nCnt++)
	{
		m_apWall[nCnt] = nullptr;	// 壁の情報
	}
}

//==============================
// デストラクタ
//==============================
CTutorialMap::~CTutorialMap()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CTutorialMap::Init(void)
{
	// 値を返す
	return S_OK;
}

//========================================
// 破片の終了処理
//========================================
void CTutorialMap::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_WALL; nCnt++)
	{
		if (m_apWall[nCnt] != nullptr)
		{ // 壁が NULL じゃない場合

			// 壁の終了処理
			m_apWall[nCnt]->Uninit();
			m_apWall[nCnt] = nullptr;
		}
	}

	// 本体の終了処理
	Release();
}

//=====================================
// 破片の更新処理
//=====================================
void CTutorialMap::Update(void)
{

}

//=====================================
// 破片の描画処理
//=====================================
void CTutorialMap::Draw(void)
{
	for (int nCnt = 0; nCnt < NUM_WALL; nCnt++)
	{
		if (m_apWall[nCnt] != nullptr)
		{ // 壁が NULL じゃない場合

			// 壁の描画処理
			m_apWall[nCnt]->Draw();
		}
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CTutorialMap::SetData(void)
{
	for (int nCnt = 0; nCnt < NUM_WALL; nCnt++)
	{
		if (m_apWall[nCnt] == nullptr)
		{ // 壁が NULL の場合

			// 壁を生成
			m_apWall[nCnt] = CMeshWall::Create
			(
				WALL_POS[nCnt], 
				WALL_ROT[nCnt], 
				WALL_WIDTH, 
				WALL_HEIGHT, 
				WALL_DIVI, 
				PRIORITY_BG
			);
			m_apWall[nCnt]->CObject::SetType(TYPE_NONE);		// 無状態にする
			m_apWall[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(WALL_TEXTURE));
			m_apWall[nCnt]->SetLighting(true);					// ライティング状況
			m_apWall[nCnt]->SetCulling(false);					// カリング状況
		}
		else
		{ // 上記以外

			// 停止
			assert(false);
		}
	}
}

//=======================================
// 生成処理
//=======================================
CTutorialMap* CTutorialMap::Create(void)
{
	// ローカルオブジェクトを生成
	CTutorialMap* pMap = nullptr;	// インスタンスを生成

	if (pMap == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pMap = new CTutorialMap;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pMap != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pMap->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pMap->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// マップのポインタを返す
	return pMap;
}