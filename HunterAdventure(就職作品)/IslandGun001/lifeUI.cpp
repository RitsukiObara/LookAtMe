//============================================
//
// 寿命UI処理[lifeUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "lifeUI.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

//--------------------------------------------
// 無名名前空間
//--------------------------------------------
namespace
{
	const float METER_WIDTH = 160.0f;		// メーターの横幅
	const D3DXVECTOR3 POLY_POS[CLifeUI::POLY_MAX] =			// ポリゴンの位置
	{
		D3DXVECTOR3(47.0f, 40.0f, 0.0f),
		D3DXVECTOR3(47.0f, 40.0f, 0.0f),
		D3DXVECTOR3(50.0f, 30.0f, 0.0f),
	};
	const D3DXVECTOR3 POLY_ROT[CLifeUI::POLY_MAX] =			// ポリゴンの向き
	{
		NONE_D3DXVECTOR3,
		NONE_D3DXVECTOR3,
		D3DXVECTOR3(0.0f,0.0f,D3DX_PI * -0.25f),
	};
	const D3DXVECTOR3 POLY_SIZE[CLifeUI::POLY_MAX] =		// ポリゴンのサイズ
	{
		D3DXVECTOR3(METER_WIDTH, 10.0f, 0.0f),
		D3DXVECTOR3(METER_WIDTH, 10.0f, 0.0f),
		D3DXVECTOR3(20.0f, 20.0f, 0.0f),
	};
	const char* TEXTURE[CLifeUI::POLY_MAX] =		// テクスチャ
	{
		"data\\TEXTURE\\LifeUIGage.png",
		"data\\TEXTURE\\LifeUIMeter.png",
		"data\\TEXTURE\\LifeUIIcon.png"
	};
	const float METER_MEMORY = METER_WIDTH * 0.01f;		// メーターの1メモリのサイズ
	const float METER_ADD = 0.2f;						// メーターの加算数
	const int ICON_FLASH_LIFE = 30;						// アイコンが明滅しだす体力
	const D3DXCOLOR ICON_FLASH_COL = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);		// アイコンの点滅時の色
	const float ICON_CALC_COL = 0.05f;					// アイコンの色の移動量
}

//========================
// コンストラクタ
//========================
CLifeUI::CLifeUI() : CObject(TYPE_LIFEUI, DIM_2D, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;		// 影の情報
	}
	m_iconflash.col = NONE_D3DXCOLOR;		// アイコンの色
	m_iconflash.bAdd = false;				// アイコンの加算状況
	m_nLife = 0;							// 寿命
	m_fMeterDest = 0.0f;					// 目的のメーター
	m_fMeter = 0.0f;						// メーター
}

//========================
// デストラクタ
//========================
CLifeUI::~CLifeUI()
{

}

//========================
// 初期化処理
//========================
HRESULT CLifeUI::Init(void)
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
void CLifeUI::Uninit(void)
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
void CLifeUI::Update(void)
{
	// サイズを設定する
	m_fMeterDest = METER_MEMORY * m_nLife;

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

	// アイコンの点滅処理
	IconFlash();

	// 頂点座標の設定処理
	m_apObject2D[POLY_METER]->SetVtxWidthGage();

	// テクスチャ座標の設定処理
	m_apObject2D[POLY_METER]->SetVtxTextureWidth(m_fMeter / METER_WIDTH);
}

//========================
// 描画処理
//========================
void CLifeUI::Draw(void)
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
void CLifeUI::SetData(const int nLife)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // ポリゴンが NULL じゃない場合

			// 情報の設定処理
			m_apObject2D[nCnt]->SetPos(POLY_POS[nCnt]);
			m_apObject2D[nCnt]->SetPosOld(POLY_POS[nCnt]);
			m_apObject2D[nCnt]->SetRot(POLY_ROT[nCnt]);
			m_apObject2D[nCnt]->SetSize(POLY_SIZE[nCnt]);
			m_apObject2D[nCnt]->SetAngle();
			m_apObject2D[nCnt]->SetLength();

			// 頂点座標の設定処理
			m_apObject2D[nCnt]->SetVertex();

			// テクスチャの割り当て処理
			m_apObject2D[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[nCnt]));

			switch (nCnt)
			{
			case POLY::POLY_GROUND:

				m_apObject2D[nCnt]->SetVtxColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

				// 頂点座標の設定処理
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			case POLY::POLY_METER:

				m_apObject2D[nCnt]->SetVtxColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

				// 頂点座標の設定処理
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			case POLY::POLY_WIPE:

				// 頂点座標の設定処理
				m_apObject2D[nCnt]->SetVertexRot();

				break;

			default:

				// 停止
				assert(false);

				break;
			}
		}
	}
	m_iconflash.col = NONE_D3DXCOLOR;		// アイコンの色
	m_iconflash.bAdd = false;				// アイコンの加算状況
	m_nLife = nLife;						// 寿命
	m_fMeterDest = nLife * METER_MEMORY;	// 目的のメーター
	m_fMeter = m_fMeterDest;				// メーター
}

//========================
// 生成処理
//========================
CLifeUI* CLifeUI::Create(const int nLife)
{
	// ローカルオブジェクトを生成
	CLifeUI* pUI = nullptr;	// UIのインスタンスを生成

	if (pUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pUI = new CLifeUI;
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
void CLifeUI::SetLife(const int nLife)
{
	// 寿命を設定する
	m_nLife = nLife;
}

//========================
// 寿命の取得処理
//========================
int CLifeUI::GetLife(void) const
{
	// 寿命を返す
	return m_nLife;
}

//========================
// アイコンの点滅処理
//========================
void CLifeUI::IconFlash(void)
{
	if (m_nLife <= ICON_FLASH_LIFE)
	{ // 体力が一定以下の場合

		if (m_iconflash.bAdd == true)
		{ // 加算状況が true の場合

			// 色を白に近づける
			m_iconflash.col.g += ICON_CALC_COL;
			m_iconflash.col.b += ICON_CALC_COL;

			if (m_iconflash.col.g >= NONE_D3DXCOLOR.g &&
				m_iconflash.col.b >= NONE_D3DXCOLOR.b)
			{ // 白になった場合

				// 色を設定する
				m_iconflash.col = NONE_D3DXCOLOR;

				// 加算状況を false にする
				m_iconflash.bAdd = false;
			}
		}
		else
		{ // 上記以外

			// 色を赤に近づける
			m_iconflash.col.g -= ICON_CALC_COL;
			m_iconflash.col.b -= ICON_CALC_COL;

			if (m_iconflash.col.g <= ICON_FLASH_COL.g &&
				m_iconflash.col.b <= ICON_FLASH_COL.b)
			{ // 赤になった場合

				// 色を設定する
				m_iconflash.col = ICON_FLASH_COL;

				// 加算状況を true にする
				m_iconflash.bAdd = true;
			}
		}
	}
	else
	{ // 上記以外

		// アイコンの色を設定する
		m_iconflash.col = NONE_D3DXCOLOR;
	}

	// 頂点カラーの設定処理
	m_apObject2D[POLY_WIPE]->SetVtxColor(m_iconflash.col);
}