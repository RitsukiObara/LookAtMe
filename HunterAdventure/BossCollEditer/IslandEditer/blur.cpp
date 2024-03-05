//=======================================
//
// モーションブラーのメイン処理[blur.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "blur.h"
#include "useful.h"

//=========================
// オーバーロードコンストラクタ
//=========================
CBlur::CBlur(PRIORITY priority) : CModel(CObject::TYPE_LOCUSMODEL, priority)
{
	// 全ての値をクリアする
	m_nLife = 0;		// 寿命
	m_fSub = 0.0f;		// 減算量
}

//=========================
// デストラクタ
//=========================
CBlur::~CBlur()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CBlur::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nLife = 0;		// 寿命
	m_fSub = 0.0f;		// 減算量

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CBlur::Uninit(void)
{
	// 終了
	CModel::Uninit();
}

//=========================
// 更新処理
//=========================
void CBlur::Update(void)
{
	// 寿命を減らしていく
	m_nLife--;

	if (m_nLife <= 0)
	{ // エフェクトの寿命が尽きた時

		// 破棄処理
		Uninit();

		// この先の処理を行わない
		return;
	}
}

//=========================
// 描画処理
//=========================
void CBlur::Draw(void)
{

}

//=========================
// 情報の設定処理
//=========================
void CBlur::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile& filedata, const int nLife, const float fAlpha)
{
	// 情報の設定処理
	SetPos(pos);				// 位置
	SetPosOld(pos);				// 前回の位置
	SetRot(rot);				// 向き
	SetScale(scale);			// 拡大率
	SetFileData(filedata);		// モデルの情報

	// 全ての値を設定する
	m_nLife = nLife;			// 寿命
	m_fSub = fAlpha / nLife;	// 減算量
}

//=========================
// 減算量の取得処理
//=========================
float CBlur::GetSub(void) const
{
	// 減算量を取得する
	return m_fSub;
}