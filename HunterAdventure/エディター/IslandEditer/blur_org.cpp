//=======================================
//
// モーションブラー(元の色)のメイン処理[blur_org.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "blur_org.h"
#include "useful.h"

//=========================
// オーバーロードコンストラクタ
//=========================
CBlurOrg::CBlurOrg(PRIORITY priority) : CBlur(priority)
{
	// 全ての値をクリアする
	m_fAlpha = 0.0f;		// 透明度
}

//=========================
// デストラクタ
//=========================
CBlurOrg::~CBlurOrg()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CBlurOrg::Init(void)
{
	if (FAILED(CBlur::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_fAlpha = 0.0f;		// 透明度

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CBlurOrg::Uninit(void)
{
	// 終了
	CBlur::Uninit();
}

//=========================
// 更新処理
//=========================
void CBlurOrg::Update(void)
{
	// 透明度を減算する
	m_fAlpha -= GetSub();

	if (m_fAlpha <= 0.0f)
	{ // 透明度が 0.0f 以下になった場合

		// 透明度を補正する
		m_fAlpha = 0.0f;
	}

	// 更新処理
	CBlur::Update();
}

//=========================
// 描画処理
//=========================
void CBlurOrg::Draw(void)
{
	// 描画処理
	CModel::Draw(m_fAlpha);
}

//=========================
// 情報の設定処理
//=========================
void CBlurOrg::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const float fAlpha, const CXFile::SXFile& filedata, const int nLife)
{
	// 情報の設定処理
	CBlur::SetData(pos, rot, scale, filedata, nLife, fAlpha);

	// 全ての値を設定する
	m_fAlpha = fAlpha;		// 透明度
}

//=========================
// 生成処理
//=========================
CBlurOrg* CBlurOrg::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const float fAlpha, const CXFile::SXFile& filedata, const int nLife, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CBlurOrg* pLocus = nullptr;	// 残像のインスタンスを生成

	if (pLocus == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLocus = new CBlurOrg(priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pLocus != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLocus->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pLocus->SetData(pos, rot, scale, fAlpha, filedata, nLife);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 残像のポインタを返す
	return pLocus;
}