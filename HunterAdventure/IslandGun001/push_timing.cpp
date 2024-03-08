//============================================
//
// 押しタイミングの表示処理[push_timing.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "push_timing.h"
#include "input.h"
#include "texture.h"

//--------------------------------------------
// 無名名前空間
//--------------------------------------------
namespace
{
	const char* TEXTURE[CPushTiming::TYPE_MAX] =		// テクスチャ
	{
		"data\\TEXTURE\\Platform\\PushPadA.png",		// Aボタンのテクスチャ
		"data\\TEXTURE\\Platform\\PushKeySpace.png",	// SPACEキーのテクスチャ
	};
	const D3DXVECTOR3 TYPE_SIZE[CPushTiming::TYPE_MAX] =	// サイズ
	{
		D3DXVECTOR3(1.0f, 1.0f, 0.0f),		// Aボタンのサイズ
		D3DXVECTOR3(2.0f, 1.0f, 0.0f),		// SPACEキーのサイズ
	};
	const int ANIM_PATTERN = 2;			// アニメーションパターン
}

//===========================================
// コンストラクタ
//===========================================
CPushTiming::CPushTiming() : CBillboardAnim(CObject::TYPE_PUSHTIMING,CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_fSize = 0.0f;		// サイズの倍率
	m_bDisp = true;		// 描画状況
}

//===========================================
// デストラクタ
//===========================================
CPushTiming::~CPushTiming()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CPushTiming::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CPushTiming::Uninit(void)
{
	// 終了
	CBillboardAnim::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CPushTiming::Update(void)
{
	// 更新処理
	CBillboardAnim::Update();
}

//===========================================
// 描画処理
//===========================================
void CPushTiming::Draw(void)
{
	if (m_bDisp == true)
	{ // 描画状況が true の場合

		// 描画処理
		CBillboardAnim::DrawLightOff(false);
	}
}

//===========================================
// 設定処理
//===========================================
void CPushTiming::SetData(const D3DXVECTOR3& pos, const float fSize, const TYPE type, const int nInterval)
{
	// 情報の設定処理
	SetPos(pos);						// 位置
	SetPosOld(pos);						// 前回の位置
	SetSize(fSize * TYPE_SIZE[type]);	// サイズ
	SetEnableLookY(true);				// Y軸状況

	// アニメーションの設定処理
	SetAnim(nInterval, ANIM_PATTERN);

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxAnim(GetTexPattern(), GetPattern());

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[type]));

	// 全ての値をクリアする
	m_fSize = fSize;	// サイズの倍率
	m_bDisp = true;		// 描画状況
}

//===========================================
// 生成処理
//===========================================
CPushTiming* CPushTiming::Create(const D3DXVECTOR3& pos, const float fSize, const TYPE type, const int nInterval)
{
	// ローカルオブジェクトを生成
	CPushTiming* pPushTiming = nullptr;			// 押しタイミングのインスタンスを生成

	if (pPushTiming == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pPushTiming = new CPushTiming;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pPushTiming != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPushTiming->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPushTiming->SetData(pos, fSize, type, nInterval);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 押しタイミングのポインタを返す
	return pPushTiming;
}

//===========================================
// 種類の設定処理
//===========================================
void CPushTiming::SetType(const TYPE type)
{
	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[type]));

	// サイズの設定処理
	SetSize(m_fSize * TYPE_SIZE[type]);

	// 頂点座標の設定処理
	SetVertex();
}

//===========================================
// サイズの倍率の設定処理
//===========================================
void CPushTiming::SetMagni(const float fSize)
{
	// サイズの倍率を設定する
	m_fSize = fSize;
}

//===========================================
// サイズの倍率の取得処理
//===========================================
float CPushTiming::GetMagni(void) const
{
	// サイズの倍率を返す
	return m_fSize;
}

//===========================================
// 描画状況の設定処理
//===========================================
void CPushTiming::SetEnableDisp(const bool bDisp)
{
	// 描画状況を設定する
	m_bDisp = bDisp;
}

//===========================================
// 描画状況の取得処理
//===========================================
bool CPushTiming::IsDisp(void) const
{
	// 描画状況を返す
	return m_bDisp;
}