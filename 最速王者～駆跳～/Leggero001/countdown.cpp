//============================================
//
// カウントダウン処理[countdown.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "game.h"
#include "countdown.h"
#include "locus2D.h"
#include "game_time.h"
#include "renderer.h"
#include "sound.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define COUNTDOWN_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))		// カウントダウンの位置
#define NUMBER_SIZE			(D3DXVECTOR3(100.0f, 200.0f, 0.0f))									// 数字のサイズ
#define START_SIZE			(D3DXVECTOR3(400.0f, 100.0f, 0.0f))									// スタートのサイズ
#define NUMBER_TEX_PATTERN	(0.1f)																// 数字のテクスチャのパターン
#define START_TIME			(3)																	// スタートまでのカウント
#define COUNTDOWN_INTERVAL	(30)																// カウントダウンの間隔
#define LOCUS_ADD_SIZE		(7.0f)																// 残像のサイズの加算量
#define ALPHA_SUB			(1.0f / COUNTDOWN_INTERVAL)											// 透明度の減算量

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CCountdown::m_apTexturename[CCountdown::TEX_MAX] = // テクスチャのパス名
{
	"data/TEXTURE/Number.png",	// カウントダウンの数字
	"data/TEXTURE/Start.png",	// スタート
};

//========================
// コンストラクタ
//========================
CCountdown::CCountdown() : CObject2D(TYPE_COUNTDOWN, PRIORITY_UI)
{
	// 全ての値を初期化する
	m_pLocus = nullptr;			// 残像のポインタ
	m_state = STATE_COUNT;		// 状態
	m_nCountDown = 0;			// スタートまでのタイム
	m_nIntervalCount = 0;		// 間隔のカウント
	m_fAlpha = 0.0f;			// 透明度
}

//========================
// デストラクタ
//========================
CCountdown::~CCountdown()
{

}

//========================
// 初期化処理
//========================
HRESULT CCountdown::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

	  // 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_pLocus = nullptr;			// 残像のポインタ
	m_state = STATE_COUNT;		// 状態
	m_nCountDown = 0;			// スタートまでのタイム
	m_nIntervalCount = 0;		// 間隔のカウント
	m_fAlpha = 1.0f;			// 透明度

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CCountdown::Uninit(void)
{
	if (SearchLocus() == true)
	{ // 残像のポインタが NULL じゃない場合

		// 残像を終了する
		m_pLocus->Uninit();
		m_pLocus = nullptr;
	}

	// 終了処理
	CObject2D::Uninit();
}

//========================
// 更新処理
//========================
void CCountdown::Update(void)
{
	switch (m_state)
	{
	case STATE_COUNT:		// カウント状態

		// カウントダウン処理
		Count();

		if (SearchLocus() == true)
		{ // 残像の情報があった場合

			// 残像のサイズの設定処理
			LocusSize();
		}

		if (m_nCountDown <= 0)
		{ // カウントダウンが0以下になった場合

			// テクスチャの割り当て処理
			BindTexture(CManager::GetTexture()->Regist(m_apTexturename[TEX_START]));

			// サイズを設定する
			SetSize(START_SIZE);

			if (SearchLocus() == true)
			{ // 残像捜索処理が true の場合

				// 終了処理
				m_pLocus->Uninit();
			}

			// 残像のポインタを NULL にする
			m_pLocus = nullptr;

			// 残像の生成処理
			m_pLocus = m_pLocus->Create(GetPos(), GetSize(), 1.0f, COUNTDOWN_INTERVAL, GetTexIdx());

			if (m_pLocus != nullptr)
			{ // 残像のポインタが NULL じゃない場合

				// 残像のテクスチャ座標の設定処理
				m_pLocus->SetVtxTexture();
			}

			// スタート状態に設定する
			m_state = STATE_START;

			// テクスチャ座標の設定処理
			SetVtxTexture();

			// スタート音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_START);
		}
		else
		{ // 上記以外

			// テクスチャ座標の設定処理
			SetVtxTextureAnim(NUMBER_TEX_PATTERN, m_nCountDown);
		}

		break;

	case STATE_START:		// スタート状態

		// 間隔カウントを加算する
		m_nIntervalCount++;

		if (SearchLocus() == true)
		{ // 残像の情報があった場合

			// 残像のサイズの設定処理
			LocusSize();
		}

		// 透明度を減算する
		m_fAlpha -= ALPHA_SUB;

		if (m_nIntervalCount >= COUNTDOWN_INTERVAL)
		{ // 間隔カウントが一定数以上になった場合

			if (SearchLocus() == true)
			{ // 残像の情報があった場合

				// 残像の終了処理
				m_pLocus->Uninit();
				m_pLocus = nullptr;
			}

			// 終了処理
			Uninit();

			// プレイモードに変える
			CGame::SetState(CGame::STATE_PLAY);

			// 時間をスタートさせる
			CGame::GetTime()->TimeStart();

			// この先の処理を行わない
			return;
		}

		// 色を設定する
		SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

		break;
	}

	// 頂点情報の設定処理
	SetVertex();
}

//========================
// 描画処理
//========================
void CCountdown::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//========================
// 情報の設定処理
//========================
void CCountdown::SetData()
{
	// 設定処理
	SetPos(COUNTDOWN_POS);						// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き設定
	SetSize(NUMBER_SIZE);						// サイズ設定
	SetLength();								// 長さ設定
	SetAngle();									// 方向設定

	// 全ての情報を設定する
	m_pLocus = nullptr;			// 残像のポインタ
	m_nCountDown = START_TIME;	// スタートまでのタイム
	m_nIntervalCount = 0;		// 間隔カウント

	// 頂点情報の設定処理
	SetVertex();

	// テクスチャ座標の設定処理
	SetVtxTextureAnim(NUMBER_TEX_PATTERN, m_nCountDown);

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(m_apTexturename[TEX_NUMBER]));

	// 残像の生成処理
	m_pLocus = m_pLocus->Create(GetPos(), GetSize(), 1.0f, COUNTDOWN_INTERVAL, GetTexIdx());

	if (m_pLocus != nullptr)
	{ // 残像のポインタが NULL じゃない場合

		// 残像のテクスチャ座標の設定処理
		m_pLocus->SetVtxTextureAnim(NUMBER_TEX_PATTERN, m_nCountDown);
	}
}

//========================
// カウントダウン処理
//========================
void CCountdown::Count(void)
{
	// 間隔カウントを加算する
	m_nIntervalCount++;

	if (m_nIntervalCount >= COUNTDOWN_INTERVAL)
	{ // 間隔カウントが一定数以上になった場合

		// 間隔カウントを初期化する
		m_nIntervalCount = 0;

		// カウントダウンを減算する
		m_nCountDown--;

		if (m_nCountDown > 0)
		{ // カウントが 0超過の場合

			if (SearchLocus() == true)
			{ // 残像捜索処理が true の場合

				// 終了処理
				m_pLocus->Uninit();
			}

			// 残像のポインタを NULL にする
			m_pLocus = nullptr;

			// 残像の生成処理
			m_pLocus = m_pLocus->Create(GetPos(), GetSize(), 1.0f, COUNTDOWN_INTERVAL, GetTexIdx());

			if (m_pLocus != nullptr)
			{ // 残像のポインタが NULL じゃない場合

				// 残像のテクスチャ座標の設定処理
				m_pLocus->SetVtxTextureAnim(NUMBER_TEX_PATTERN, m_nCountDown);
			}

			// カウントダウン音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
		}
	}
}

//========================
// 残像捜索処理
//========================
bool CCountdown::SearchLocus(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトのポインタ

	// 先頭のオブジェクトの情報を取得する
	pObj = CObject::GetTop(PRIORITY_PLAYER);

	while (pObj != nullptr)
	{ // オブジェクトの情報がある限り回す

		if (pObj->GetType() == TYPE_LOCUS &&
			pObj == m_pLocus)
		{ // 種類が残像だった場合

			// true を返す
			return true;
		}

		// 次のオブジェクトの情報を入れる
		pObj = pObj->GetNext();
	}

	// 見る場所を NULL にする
	m_pLocus = nullptr;

	// false を返す
	return false;
}

//========================
// 残像のサイズ設定処理
//========================
void CCountdown::LocusSize(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = m_pLocus->GetSize();		// 残像のサイズを取得する

	// サイズを拡大する
	size.x += LOCUS_ADD_SIZE;
	size.y += LOCUS_ADD_SIZE;

	// 残像のサイズを設定する
	m_pLocus->SetSize(size);
}

//========================
// 生成処理
//========================
CCountdown* CCountdown::Create(void)
{
	// ローカルオブジェクトを生成
	CCountdown* pCountDown = nullptr;	// プレイヤーのインスタンスを生成

	if (pCountDown == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pCountDown = new CCountdown;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pCountDown != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCountDown->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "告知の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pCountDown->SetData();

		// カウントダウン音を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pCountDown;
}