//============================================
//
// 擦り煙のメイン処理[rubsmoke.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "rubsmoke.h"
#include "player.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define RUBSMOKE_TEXTURE		"data/TEXTURE/explosion000.png"		// 爆発のテクスチャ
#define RUBSMOKE_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))	// 擦り煙のサイズ

//===========================================
// コンストラクタ
//===========================================
CRubSmoke::CRubSmoke() : CBillboardAnim(CObject::TYPE_RUBSMOKE,CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_nLife = 0;				// 寿命
}

//===========================================
// デストラクタ
//===========================================
CRubSmoke::~CRubSmoke()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CRubSmoke::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nLife = 0;				// 寿命

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CRubSmoke::Uninit(void)
{
	// 終了
	CBillboardAnim::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CRubSmoke::Update(void)
{
	if (CScene::GetPlayer() != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// ローカル変数宣言
		D3DXVECTOR3 pos = GetPos();

		// 位置をずらす
		pos.y += CScene::GetPlayer()->GetMove().y;

		// 位置を設定する
		SetPos(pos);
	}

	// 寿命を減らす
	m_nLife--;

	// 更新処理
	CBillboardAnim::Update();

	if (m_nLife <= 0)
	{ // パターン数が最大数を超えた場合

		// 終了処理
		Uninit();
	}
}

//===========================================
// 描画処理
//===========================================
void CRubSmoke::Draw(void)
{
	// 描画処理
	CBillboardAnim::DrawLightOff();
}

//===========================================
// 設定処理
//===========================================
void CRubSmoke::SetData(const D3DXVECTOR3& pos)
{
	// スクロールの設定処理
	SetPos(pos);				// 位置設定
	SetSize(RUBSMOKE_SIZE);		// サイズ設定

	// アニメーションの設定処理
	SetAnim(3, 8);

	// 全ての値を初期化する
	m_nLife = 3 * 8;			// 寿命

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxAnim(GetTexPattern(), GetPattern());

	// テクスチャの読み込み処理
	BindTexture(CManager::GetTexture()->Regist(RUBSMOKE_TEXTURE));
}

//===========================================
// 生成処理
//===========================================
CRubSmoke* CRubSmoke::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CRubSmoke* pRubSmoke = nullptr;	// プレイヤーのインスタンスを生成

	if (pRubSmoke == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pRubSmoke = new CRubSmoke;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pRubSmoke != nullptr)
	{ // オブジェクトが NULL じゃない場合

	  // 初期化処理
		if (FAILED(pRubSmoke->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "擦り煙の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRubSmoke->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pRubSmoke;
}