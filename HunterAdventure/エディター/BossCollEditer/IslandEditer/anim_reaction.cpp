//============================================
//
// アニメリアクションのメイン処理[explosion.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "anim_reaction.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// 無名名前空間
//--------------------------------------------
namespace
{
	const char* TEXTURE[CAnimReaction::TYPE_MAX] =		// テクスチャ
	{
		"data\\TEXTURE\\Platform\\Explosion.png",		// 爆発
		"data\\TEXTURE\\Platform\\GunExplosion.png",	// 爆薬爆発
		"data\\TEXTURE\\Platform\\Splash.png",			// 飛沫
		"data\\TEXTURE\\Platform\\WaterExpl.png",		// 水爆発
	};
	const int ANIM_PATTERN[CAnimReaction::TYPE_MAX] =	// パターン
	{
		8,			// 爆発
		8,			// 爆薬爆発
		6,			// 飛沫
		8,			// 水爆発
	};
}

//===========================================
// コンストラクタ
//===========================================
CAnimReaction::CAnimReaction() : CBillboardAnim(CObject::TYPE_ANIMREACTION, CObject::PRIORITY_EFFECT)
{
	// 全ての値をクリアする
	m_type = TYPE_EXPLOSION;	// 種類
	m_nLife = 0;				// 寿命
}

//===========================================
// デストラクタ
//===========================================
CAnimReaction::~CAnimReaction()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CAnimReaction::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_type = TYPE_EXPLOSION;	// 種類
	m_nLife = 0;				// 寿命

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CAnimReaction::Uninit(void)
{
	// 終了
	CBillboardAnim::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CAnimReaction::Update(void)
{
	// 寿命を減算する
	m_nLife--;

	if (m_nLife <= 0)
	{ // 寿命が0になった場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 更新処理
	CBillboardAnim::Update();
}

//===========================================
// 描画処理
//===========================================
void CAnimReaction::Draw(void)
{
	// 描画処理
	CBillboardAnim::DrawLightOff(false);
}

//===========================================
// 設定処理
//===========================================
void CAnimReaction::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nCount, const int nLife)
{
	// スクロールの設定処理
	SetPos(pos);				// 位置設定
	SetSize(size);				// サイズ設定
	SetEnableLookY(true);		// Y軸状況
	SetAnim(nCount, ANIM_PATTERN[type]);		// アニメーション情報

	// 全ての値を設定する
	m_type = type;				// 種類
	m_nLife = nCount * ANIM_PATTERN[m_type] * nLife;		// 寿命

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxAnim(GetTexPattern(), GetPattern());

	// 頂点カラーの設定処理
	SetVtxColor(col);

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[m_type]));
}

//===========================================
// 生成処理
//===========================================
CAnimReaction* CAnimReaction::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nCount, const int nLife)
{
	// ローカルオブジェクトを生成
	CAnimReaction* pReaction = nullptr;		// リアクションのインスタンスを生成

	if (pReaction == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pReaction = new CAnimReaction;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pReaction != nullptr)
	{ // オブジェクトが NULL じゃない場合

	  // 初期化処理
		if (FAILED(pReaction->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pReaction->SetData(pos, size, col, type, nCount, nLife);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// リアクションのポインタを返す
	return pReaction;
}