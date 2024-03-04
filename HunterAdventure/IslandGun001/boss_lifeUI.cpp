//============================================
//
// ボスの寿命UI処理[boss_lifeUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "boss_lifeUI.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

//--------------------------------------------
// 無名名前空間
//--------------------------------------------
namespace
{
	const float METER_WIDTH = 480.0f;		// メーターの横幅
	const D3DXVECTOR3 POLY_SIZE = D3DXVECTOR3(METER_WIDTH, 30.0f, 0.0f);		// ポリゴンのサイズ
	const D3DXVECTOR3 POLY_POS[CBossLifeUI::POLY_MAX] =			// ポリゴンの位置
	{
		D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - METER_WIDTH, 650.0f, 0.0f),
		D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - METER_WIDTH, 650.0f, 0.0f),
	};
	const D3DXVECTOR3 POLY_ROT = NONE_D3DXVECTOR3;			// ポリゴンの向き
	const char* TEXTURE[CBossLifeUI::POLY_MAX] =			// テクスチャ
	{
		"data\\TEXTURE\\BossLifeGage.png",
		"data\\TEXTURE\\BossLifeMeter.png",
	};
	const float METER_ADD = 1.0f;							// メーターの加算数
}

//========================
// コンストラクタ
//========================
CBossLifeUI::CBossLifeUI() : CObject(TYPE_LIFEUI, DIM_2D, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;		// ポリゴン
	}
	m_nLife = 0;							// 寿命
	m_fMeterDest = 0.0f;					// 目的のメーター
	m_fMeter = 0.0f;						// メーター
	m_fMemory = 0.0f;						// 1メモリの値
}

//========================
// デストラクタ
//========================
CBossLifeUI::~CBossLifeUI()
{

}

//========================
// 初期化処理
//========================
HRESULT CBossLifeUI::Init(void)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] == nullptr)
		{ // ポリゴンが NULL じゃない場合

			// ポリゴンを生成
			m_apObject2D[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		}
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CBossLifeUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // 情報が NULL じゃない場合

			// ポリゴンの終了処理
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = nullptr;
		}
	}

	// 本体の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CBossLifeUI::Update(void)
{
	// サイズを設定する
	m_fMeterDest = m_fMemory * m_nLife;

	// 均等な数値の補正処理
	useful::FrameCorrect(m_fMeterDest, &m_fMeter, METER_ADD);

	{ // サイズの適用

		// サイズを取得する
		D3DXVECTOR3 size = m_apObject2D[POLY_METER]->GetSize();

		// サイズを設定する
		size.x = m_fMeter;

		// サイズを適用する
		m_apObject2D[POLY_METER]->SetSize(size);
	}

	// 頂点座標の設定処理
	m_apObject2D[POLY_METER]->SetVtxWidthGage();

	// テクスチャ座標の設定処理
	m_apObject2D[POLY_METER]->SetVtxTextureWidth(m_fMeter / METER_WIDTH);
}

//========================
// 描画処理
//========================
void CBossLifeUI::Draw(void)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // ポリゴンが NULL じゃない場合

			// 描画処理
			m_apObject2D[nCnt]->Draw();
		}
	}
}

//========================
// 情報の設定処理
//========================
void CBossLifeUI::SetData(const int nLife)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // ポリゴンが NULL じゃない場合

			// 情報の設定処理
			m_apObject2D[nCnt]->SetPos(POLY_POS[nCnt]);
			m_apObject2D[nCnt]->SetPosOld(POLY_POS[nCnt]);
			m_apObject2D[nCnt]->SetRot(POLY_ROT);
			m_apObject2D[nCnt]->SetSize(POLY_SIZE);
			m_apObject2D[nCnt]->SetAngle();
			m_apObject2D[nCnt]->SetLength();

			// 頂点座標の設定処理
			m_apObject2D[nCnt]->SetVertex();

			// テクスチャの割り当て処理
			m_apObject2D[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[nCnt]));

			switch (nCnt)
			{
			case POLY::POLY_GROUND:

				// 頂点座標の設定処理
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			case POLY::POLY_METER:

				// 頂点座標の設定処理
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			default:

				// 停止
				assert(false);

				break;
			}
		}
	}
	m_nLife = nLife;						// 寿命
	m_fMemory = METER_WIDTH / nLife;		// 1メモリの値
	m_fMeterDest = nLife * m_fMemory;		// 目的のメーター
	m_fMeter = m_fMeterDest;				// メーター
}

//========================
// 生成処理
//========================
CBossLifeUI* CBossLifeUI::Create(const int nLife)
{
	// ローカルオブジェクトを生成
	CBossLifeUI* pUI = nullptr;	// UIのインスタンスを生成

	if (pUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pUI = new CBossLifeUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pUI != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pUI->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pUI->SetData(nLife);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// UIのポインタを返す
	return pUI;
}

//========================
// 寿命の設定処理
//========================
void CBossLifeUI::SetLife(const int nLife)
{
	// 寿命を設定する
	m_nLife = nLife;
}

//========================
// 寿命の取得処理
//========================
int CBossLifeUI::GetLife(void) const
{
	// 寿命を返す
	return m_nLife;
}