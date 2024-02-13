//============================================
//
// 多重背景のメイン処理[bgmulti.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "bgmulti.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char *CBgMulti::m_apTextureName[TYPE_MAX] =			// テクスチャの名前
{
	"data/TEXTURE/bg100.png",
	"data/TEXTURE/bg101.png",
	"data/TEXTURE/bg102.png",
};

const D3DXVECTOR2 CBgMulti::m_aSpeedInfo[TYPE_MAX] =		// スピードの情報
{
	D3DXVECTOR2(0.0f, 0.001f),
	D3DXVECTOR2(0.0f, 0.0005f),
	D3DXVECTOR2(0.0f, 0.005f)
};

//=========================================
// コンストラクタ
//=========================================
CBgMulti::CBgMulti() : CObject(CObject::TYPE_BG, CObject::PRIORITY_BG)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 背景のポインタ
		m_apBgMulti[nCnt] = nullptr;
	}
}

//=========================================
// デストラクタ
//=========================================
CBgMulti::~CBgMulti()
{

}

//===========================================
// オブジェクト2Dの初期化処理
//===========================================
HRESULT CBgMulti::Init(void)
{
	// ローカル変数宣言
	HRESULT hr = S_OK;			// 結果の変数

	for (int nCntBg = 0; nCntBg < TYPE_MAX; nCntBg++)
	{
		if (m_apBgMulti[nCntBg] == nullptr)
		{ // ポインタが NULL の場合

			// メモリを確保する
			m_apBgMulti[nCntBg] = new CScroll;
		}
		else
		{ // ポインタが NULL じゃない場合

			// 警告文
			MessageBox(NULL, "多重背景のメモリの確保に失敗！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}

		if (m_apBgMulti[nCntBg] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 背景の処理
			hr = m_apBgMulti[nCntBg]->Init();
		}
		else
		{ // ポインタが　NULL の場合

			// 警告文
			MessageBox(NULL, "多重背景の初期化に失敗！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}

		// テクスチャの割り当て処理
		m_apBgMulti[nCntBg]->BindTexture(CManager::GetTexture()->Regist(m_apTextureName[nCntBg]));

		// スクロールの設定処理
		m_apBgMulti[nCntBg]->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));	// 位置設定
		m_apBgMulti[nCntBg]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き設定
		m_apBgMulti[nCntBg]->SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));		// サイズ設定
		m_apBgMulti[nCntBg]->SetLength();			// 長さ設定
		m_apBgMulti[nCntBg]->SetAngle();			// 方向設定

		// 頂点情報の設定処理
		m_apBgMulti[nCntBg]->SetVertex();

		{ // テクスチャ設定

			D3DXVECTOR2 tex = m_apBgMulti[nCntBg]->GetTex();

			// テクスチャ設定
			m_apBgMulti[nCntBg]->SetVtxTextureScroll(tex);
		}

		// スピードを設定する
		m_apBgMulti[nCntBg]->SetSpeed(m_aSpeedInfo[nCntBg]);		// 速度の設定処理
	}

	// 成功を返す
	return hr;
}

//===========================================
// オブジェクト2Dの終了処理
//===========================================
void CBgMulti::Uninit(void)
{
	for (int nCntBg = 0; nCntBg < TYPE_MAX; nCntBg++)
	{
		// 背景の終了処理
		m_apBgMulti[nCntBg]->Uninit();
	}

	// 多重背景の塊の破棄処理
	CObject::Release();
}

//===========================================
// オブジェクト2Dの更新処理
//===========================================
void CBgMulti::Update(void)
{
	for (int nCntBg = 0; nCntBg < TYPE_MAX; nCntBg++)
	{
		// 背景の更新処理
		m_apBgMulti[nCntBg]->Update();
	}
}

//===========================================
// オブジェクト2Dの描画処理
//===========================================
void CBgMulti::Draw(void)
{
	for (int nCntBg = 0; nCntBg < TYPE_MAX; nCntBg++)
	{
		// 背景の描画処理
		m_apBgMulti[nCntBg]->Draw();
	}
}

//===========================================
// 多重背景の取得処理
//===========================================
CScroll* CBgMulti::GetBgMulti(int nIdx) const
{
	// 背景の情報を返す
	return m_apBgMulti[nIdx];
}

//===========================================
// 位置の設定処理
//===========================================
void CBgMulti::SetPos(const D3DXVECTOR3& pos)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 情報を設定する
		m_apBgMulti[nCnt]->SetPos(pos);
	}
}

//===========================================
// 位置の取得処理
//===========================================
D3DXVECTOR3 CBgMulti::GetPos(void) const
{
	// 警告文
	MessageBox(NULL, "多重背景の位置取得は無効です！", "警告！", MB_ICONWARNING);

	// 位置を返す
	return m_apBgMulti[0]->GetPos();
}

//===========================================
// 生成処理
//===========================================
CBgMulti* CBgMulti::Create(void)
{
	// ローカルオブジェクトを生成
	CBgMulti* pBgMulti = nullptr;	// プレイヤーのインスタンスを生成

	if (pBgMulti == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pBgMulti = new CBgMulti;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pBgMulti != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBgMulti->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "多重背景の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pBgMulti;
}