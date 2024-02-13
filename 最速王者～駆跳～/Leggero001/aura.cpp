//=======================================
//
// オーラのメイン処理[aura.cpp]
// Author 小原立暉
//
//=======================================
#include "aura.h"
#include "player.h"
#include "manager.h"
#include "texture.h"

//=======================================
// マクロ定義
//=======================================
#define AURA_TEXTURE		"data/TEXTURE/PopSmoke.tga"			// オーラのテクスチャ
#define AURA_COL			(D3DXCOLOR(1.0f,1.0f,0.2f,1.0f))	// オーラの色

//=========================
// コンストラクタ
//=========================
CAura::CAura() : CBillboard(CObject::TYPE_AURA, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_nLife = 0;									// 寿命
	m_fContra = 0.0f;								// 半径の縮む間隔
}

//=========================
// デストラクタ
//=========================
CAura::~CAura()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CAura::Init(void)
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
void CAura::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CAura::Update(void)
{
	if (CScene::GetPlayer() != nullptr)
	{ // プレイヤーの情報が NULL じゃない場合

		// 移動量を加算する
		m_move.y = CScene::GetPlayer()->GetMove().y;
	}

	// ローカル変数を宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	D3DXVECTOR3 size = GetSize();	// サイズを取得する

	// 寿命を減らしていく
	m_nLife--;

	// 移動する
	pos += m_move;

	// サイズを縮める
	size.x -= m_fContra;
	size.y -= m_fContra;

	// 位置を更新する
	SetPos(pos);

	// サイズを更新する
	SetSize(size);

	if (m_nLife <= 0)
	{ // エフェクトの寿命が尽きた時

		// 破棄処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点座標の設定処理
	SetVertex();
}

//=========================
// 描画処理
//=========================
void CAura::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff();
}

//=========================
// 情報の設定処理
//=========================
void CAura::SetData(const D3DXVECTOR3& pos, const int nLife)
{
	// スクロールの設定処理
	SetPos(pos);			// 位置設定

	// 寿命を設定する
	m_nLife = nLife;

	{ // 半径の設定

		// ローカル変数宣言
		float fRadius;		// 半径

		// 半径をランダムで出す
		fRadius = (float)(rand() % 10 + 30.0f);

		// サイズ設定
		SetSize(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// 半径の縮む間隔を設定する
		m_fContra = fRadius * (1.0f / m_nLife);			
	}

	// 全ての値を初期化する
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量

	// 頂点情報の初期化
	SetVertex();

	// 色の設定処理
	SetVtxColor(AURA_COL);

	// テクスチャの読み込み処理
	BindTexture(CManager::GetTexture()->Regist(AURA_TEXTURE));
}

//=========================
// 生成処理
//=========================
CAura* CAura::Create(const D3DXVECTOR3& pos, const int nLife)
{
	// ローカルオブジェクトを生成
	CAura* pAura = nullptr;		// オーラのインスタンスを生成

	if (pAura == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pAura = new CAura;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pAura != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pAura->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "オーラの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pAura->SetData(pos, nLife);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オーラのポインタを返す
	return pAura;
}