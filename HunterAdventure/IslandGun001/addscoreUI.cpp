//=======================================
//
// 追加スコアUIのメイン処理[addscoreUI.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "addscoreUI.h"
#include "renderer.h"
#include "texture.h"

//---------------------------------------
// 定数定義
//---------------------------------------
namespace
{
	const char* TEXTURE[CAddScoreUI::TYPE_MAX] =
	{
		"data\\TEXTURE\\ScoreCoin.png",		// コイン
		"data\\TEXTURE\\ScoreEnm001.png",	// タードル
		"data\\TEXTURE\\ScoreEnm002.png",	// イワカリ
	};
	const D3DXVECTOR3 UI_SIZE = D3DXVECTOR3(90.0f, 30.0f, 0.0f);		// サイズ
	const float ADD_HEIGHT = 2.0f;			// 高さの追加量
	const int LIFE = 60;					// 寿命
	const float SUB_ALPHA = 0.1f;			// 透明度の減算量
	const float SUB_ALPHA_LIFE = 10;		// 透明度を減らす寿命
}

//=========================
// コンストラクタ
//=========================
CAddScoreUI::CAddScoreUI() : CBillboard(TYPE_ADDSCOREUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_type = TYPE_COIN;			// 種類
	m_nLife = LIFE;				// 寿命
	m_fAlpha = 1.0f;			// 透明度
}

//=========================
// デストラクタ
//=========================
CAddScoreUI::~CAddScoreUI()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CAddScoreUI::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CAddScoreUI::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CAddScoreUI::Update(void)
{
	// 寿命を減算する
	m_nLife--;

	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	// 上に移動する
	pos.y += ADD_HEIGHT;

	// 位置を適用
	SetPos(pos);

	if (m_nLife <= SUB_ALPHA_LIFE)
	{ // 透明度を減らす寿命数になった場合

		// 透明度を減算する
		m_fAlpha -= SUB_ALPHA;
	}

	if (m_nLife <= 0)
	{ // 寿命が0になった場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点情報の初期化
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// 描画処理
//=========================
void CAddScoreUI::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff(false);
}

//=========================
// 情報の設定処理
//=========================
void CAddScoreUI::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// スクロールの設定処理
	SetPos(pos);			// 位置設定
	SetRot(0.0f);			// 向き設定
	SetSize(UI_SIZE);		// サイズ設定
	SetLength();			// 長さ設定
	SetAngle();				// 方向設定
	SetEnableLookY(true);	// Y軸設定

	// 全ての値を初期化する
	m_type = type;			// 種類
	m_nLife = LIFE;			// 寿命
	m_fAlpha = 1.0f;			// 透明度

	// 頂点情報の初期化
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[m_type]));
}

//=========================
// 生成処理
//=========================
CAddScoreUI* CAddScoreUI::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ローカルオブジェクトを生成
	CAddScoreUI* pLocus3D = nullptr;	// 残像のインスタンスを生成

	if (pLocus3D == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLocus3D = new CAddScoreUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pLocus3D != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLocus3D->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pLocus3D->SetData(pos, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 残像のポインタを返す
	return pLocus3D;
}