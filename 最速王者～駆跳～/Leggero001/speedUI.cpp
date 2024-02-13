//============================================
//
// 速度UI処理[speedUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "speedUI.h"
#include "object2D.h"
#include "texture.h"

#include "tutorial_speedUI.h"
#include "game_speedUI.h"
#include "player.h"
#include "particle2D.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SPEEDUI_GAGE_SIZE			(D3DXVECTOR3(300.0f, 40.0f, 0.0f))								// 速度UIのゲージのサイズ
#define SPEEDUI_METER_SIZE			(D3DXVECTOR3(0.0f, SPEEDUI_GAGE_SIZE.y, 0.0f))					// 速度UIのメーターのサイズ
#define SPEEDUI_METER_MAX			(SPEEDUI_GAGE_SIZE.x)											// 速度UIのメーターの最高値
#define SPEEDUI_METER_CONVERSION	(SPEEDUI_METER_MAX / (PLAYER_MAX_SPEED - PLAYER_MIN_SPEED))		// 速度UIのメーターの変換率
#define SPEEDUI_METER_TEXTURE		((float)(1.0f / (PLAYER_MAX_SPEED - PLAYER_MIN_SPEED)))			// 速度UIの1メモリのテクスチャの値
#define SPEEDUI_OVERTOP_TEXTURE		"data/TEXTURE/SpeedUIOver.png"									// 速度UIの限界突破のテクスチャ

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CSpeedUI::m_apTexturename[CSpeedUI::TYPE_MAX] =			// テクスチャのパス名
{
	"data/TEXTURE/SpeedUIGage.png",				// ゲージ
	"data/TEXTURE/SpeedUIMeter.png",			// メーター
	"data/TEXTURE/SpeedUIMark.png",				// マーク
};

//============================
// コンストラクタ
//============================
CSpeedUI::CSpeedUI() : CObject(CObject::TYPE_SPEEDUI, PRIORITY_UI)
{
	// コンストラクタの箱
	Box();
}

//============================
// オーバーロードコンストラクタ
//============================
CSpeedUI::CSpeedUI(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//============================
// デストラクタ
//============================
CSpeedUI::~CSpeedUI()
{

}

//============================
// コンストラクタの箱
//============================
void CSpeedUI::Box(void)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;			// 2Dオブジェクトの情報
	}

	m_fLevel = 0.0f;				// レベル
	m_fLevelDest = 0.0f;			// 目標のレベル
	m_nOverTopTex = NONE_TEXIDX;	// スーパーブースト時のテクスチャのインデックス
	m_nMeterTex = NONE_TEXIDX;		// メーターのテクスチャのインデックスの保存変数
}

//============================
// 初期化処理
//============================
HRESULT CSpeedUI::Init(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// 生成処理
		m_apObject2D[nCntUI] = CObject2D::Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, CObject::PRIORITY_UI);

		switch (nCntUI)
		{
		case TYPE_GAGE:		// ゲージ

			m_apObject2D[nCntUI]->SetSize(SPEEDUI_GAGE_SIZE);			// サイズ設定

			break;

		case TYPE_METER:	// メーター

			m_apObject2D[nCntUI]->SetSize(SPEEDUI_METER_SIZE);			// サイズ設定

			break;
				
		case TYPE_MARK:		// マーク

			m_apObject2D[nCntUI]->SetSize(SPEEDUI_MARK_SIZE);			// サイズ設定

			break;
		}

		// スクロールの設定処理
		m_apObject2D[nCntUI]->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 位置設定
		m_apObject2D[nCntUI]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き設定
		m_apObject2D[nCntUI]->SetLength();								// 長さ設定
		m_apObject2D[nCntUI]->SetAngle();								// 方向設定

		if (nCntUI == TYPE_MARK)
		{ // マークの場合

			// 頂点情報の初期化
			m_apObject2D[nCntUI]->SetVertex();
		}
		else
		{ // 上記以外

			// 頂点情報の初期化
			m_apObject2D[nCntUI]->SetVtxWidthGage();
		}

		// テクスチャの割り当て処理
		m_apObject2D[nCntUI]->BindTexture(CManager::GetTexture()->Regist(m_apTexturename[nCntUI]));
	}

	m_fLevel = 0.0f;			// レベル
	m_fLevelDest = 0.0f;		// 目標のレベル
	m_nOverTopTex = CManager::GetTexture()->Regist(SPEEDUI_OVERTOP_TEXTURE);	// スーパーブースト時のテクスチャのインデックスの保存変数
	m_nMeterTex = m_apObject2D[TYPE_METER]->GetTexIdx();						// メーターのテクスチャのインデックスの保存変数

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CSpeedUI::Uninit(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// 終了処理
		m_apObject2D[nCntUI]->Uninit();
		m_apObject2D[nCntUI] = nullptr;
	}

	// 破棄処理
	Release();
}

//============================
// 更新処理
//============================
void CSpeedUI::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size;		// サイズ

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // オブジェクトが NULL じゃない場合

			switch (nCnt)
			{
			case TYPE_GAGE:			// ゲージ

				// 頂点情報の設定処理
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			case TYPE_METER:		// メーター

				// メーター設定処理
				SetSpeedMeter();

				// サイズの補正処理
				SizeCorrect();

				if (CScene::GetPlayer()->GetState() == CPlayer::STATE_BOOST)
				{ // ブースト状態の場合

					if (m_apObject2D[TYPE_METER]->GetTexIdx() != m_nOverTopTex)
					{ // テクスチャが違った場合

						// スーパーブーストのテクスチャを割り当てる
						m_apObject2D[TYPE_METER]->BindTexture(m_nOverTopTex);
					}

					// テクスチャ座標の設定処理
					m_apObject2D[nCnt]->SetVtxTexture();
				}
				else
				{ // 上記以外

					if (m_apObject2D[TYPE_METER]->GetTexIdx() != m_nMeterTex)
					{ // テクスチャが違った場合

						// 通常のテクスチャを割り当てる
						m_apObject2D[TYPE_METER]->BindTexture(m_nMeterTex);
					}

					// テクスチャ座標の設定処理
					m_apObject2D[nCnt]->SetVtxTextureWidth(m_fLevel * SPEEDUI_METER_TEXTURE);
				}

				// 頂点情報の設定処理
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			case TYPE_MARK:			// マーク

				// 頂点情報の設定処理
				m_apObject2D[nCnt]->SetVertex();

				break;
			}
		}
	}
}

//============================
// 描画処理
//============================
void CSpeedUI::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // 2Dオブジェクトの情報が NULL じゃない場合

			// 描画処理
			m_apObject2D[nCnt]->Draw();
		}
	}
}

//============================
// オブジェクトの取得処理
//============================
CObject2D* CSpeedUI::GetObject2D(const TYPE type)
{
	// オブジェクトの情報を返す
	return m_apObject2D[type];
}

//============================
//生成処理
//============================
CSpeedUI* CSpeedUI::Create(const CScene::MODE mode)
{
	// ローカルオブジェクトを生成
	CSpeedUI* pSpeedUI = nullptr;	// プレイヤーのインスタンスを生成

	if (pSpeedUI == nullptr)
	{ // オブジェクトが NULL の場合

		switch (mode)
		{
		case CScene::MODE_TUTORIAL:		// チュートリアル

			// オブジェクトを生成
			pSpeedUI = new CTutorialSpeedUI;

			break;

		case CScene::MODE_GAME:			// ゲーム

			// オブジェクトを生成
			pSpeedUI = new CGameSpeedUI;

			break;

		default:		// 上記以外

			// NULL を返す
			return nullptr;

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pSpeedUI != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pSpeedUI->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "速度UIの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pSpeedUI->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 速度UIのポインタを返す
	return pSpeedUI;
}

//============================
// メーター設定処理
//============================
void CSpeedUI::SetSpeedMeter(void)
{
	// ローカルポインタを宣言
	CPlayer* pPlayer = CScene::GetPlayer();						// プレイヤーのポインタを取得

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// レベルを設定する
		m_fLevelDest = (pPlayer->GetSpeed() - (int)(PLAYER_MIN_SPEED));
	}
}

//============================
// サイズの補正処理
//============================
void CSpeedUI::SizeCorrect(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = m_apObject2D[TYPE_METER]->GetSize();		// サイズを取得する

	// 補正処理
	Correct(m_fLevelDest, &m_fLevel, 0.5f);

	// 目的の速度を設定する
	size.x = m_fLevel * SPEEDUI_METER_CONVERSION;

	// サイズを更新する
	m_apObject2D[TYPE_METER]->SetSize(size);
}