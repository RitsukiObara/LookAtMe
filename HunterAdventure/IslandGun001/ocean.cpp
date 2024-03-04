//====================================================
//
// 海の処理 [ocean.cpp]
// Author：小原立暉
//
//====================================================
//****************************************************
//	インクルードファイル
//****************************************************
#include "manager.h"
#include "ocean.h"
#include "texture.h"

//----------------------------------------------------
// マクロ定義
//----------------------------------------------------
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Lake002.png";			// テクスチャのパス
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, -50.0f, 0.0f);	// 位置
	const int OCEAN_DIVI_WIDTH = 80;		// 海の縦の分割数
	const int OCEAN_DIVI_DEPTH = 80;		// 海の奥行の分割数
	const float OCEAN_WIDTH = 80000.0f;		// 海の幅
	const float OCEAN_DEPTH = 80000.0f;		// 海の奥行
	const float OCEAN_WAVE_HEIGHT = 30.0f;	// 海の高さ
	const float OCEAN_CYCLE = 1.0f;			// 海の回転率
	const float OCEAN_SPEED = 0.01f;		// 海の流れの速度
}

//================================
// コンストラクタ
//================================
COcean::COcean() : CMeshWave(CObject::PRIORITY_BG)
{
	// タイプを変更する
	CObject::SetType(TYPE_OCEAN);
}

//================================
// デストラクタ
//================================
COcean::~COcean()
{

}

//================================
// 初期化処理
//================================
HRESULT COcean::Init(void)
{
	if (FAILED(CMeshWave::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void COcean::Uninit(void)
{
	// 終了処理
	CMeshWave::Uninit();
}

//================================
// 更新処理
//================================
void COcean::Update(void)
{
	// 更新処理
	CMeshWave::Update();
}

//================================
// 描画処理
//================================
void COcean::Draw(void)
{
	// 描画処理
	CMeshWave::Draw();
}

//================================
// データの設定処理
//================================
void COcean::SetData(void)
{
	SGrid grid;

	// 方眼情報を設定する
	grid.nWidth = OCEAN_DIVI_WIDTH;
	grid.nHeight = OCEAN_DIVI_DEPTH;

	// 情報の設定処理
	CMeshWave::SetData(POS, NONE_D3DXVECTOR3, OCEAN_WIDTH, OCEAN_DEPTH, grid, OCEAN_WAVE_HEIGHT, OCEAN_CYCLE, OCEAN_SPEED);

	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
	SetLighting(true);
	SetCulling(false);
}

//================================
// 生成処理
//================================
COcean* COcean::Create(void)
{
	// ローカルオブジェクトを生成
	COcean* pOcean = nullptr;			// 海のインスタンスを生成

	if (pOcean == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pOcean = new COcean;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pOcean != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 情報の設定処理
		pOcean->SetData();

		// 初期化処理
		if (FAILED(pOcean->Init()))
		{ // 初期化処理に失敗した場合

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 海のポインタを返す
	return pOcean;
}