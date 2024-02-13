//=======================================
//
// コンボの制限円のメイン処理[combo_circle.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "combo_circle.h"
#include "objectDonut2D.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// マクロ定義
//=======================================

//=========================
// コンストラクタ
//=========================
CComboCircle::CComboCircle() : CDonut2D(CObject::TYPE_NONE, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_nVtx = 0;				// 頂点数
	m_bEnd = false;			// 制限時間状況
}

//=========================
// デストラクタ
//=========================
CComboCircle::~CComboCircle()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CComboCircle::Init(void)
{
	if (FAILED(CDonut2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nVtx = 0;				// 頂点数
	m_bEnd = false;			// 制限時間状況

	// 色を設定する
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CComboCircle::Uninit(void)
{
	// 終了
	CDonut2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CComboCircle::Update(void)
{
	// 頂点数を減算する
	m_nVtx--;

	if (m_nVtx <= 0)
	{ // 頂点数が0以下になった場合

		// 頂点数を設定する
		m_nVtx = 0;

		// 終了状況にする
		m_bEnd = true;
	}

	// 頂点カラーの設定処理(範囲設定)
	SetVtxColor(m_nVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f));
}

//=========================
// 描画処理
//=========================
void CComboCircle::Draw(void)
{
	// 描画処理
	CDonut2D::Draw();
}

//=========================
// 情報の設定処理
//=========================
void CComboCircle::SetData(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi)
{
	// 全ての値を設定する
	SetPos(pos);			// 位置
	SetPosOld(pos);			// 前回の位置
	SetSize(fSize);			// サイズ
	SetDistance(fDist);		// 距離
	SetDivi(nDivi);			// 分割数
	SetNumVtx();			// 総頂点数

	// 全ての値を設定する
	m_nVtx = 0;				// 頂点数
	m_bEnd = false;			// 制限時間状況
}

//=========================
// 生成処理
//=========================
CComboCircle* CComboCircle::Create(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi)
{
	// ローカルオブジェクトを生成
	CComboCircle* pComboCircle = nullptr;	// プレイヤーのインスタンスを生成

	if (pComboCircle == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pComboCircle = new CComboCircle;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pComboCircle != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 情報の設定処理
		pComboCircle->SetData(pos, fSize, fDist, nDivi);

		// 初期化処理
		if (FAILED(pComboCircle->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// コンボの円のポインタを返す
	return pComboCircle;
}

//=========================
// 頂点のリセット処理
//=========================
void CComboCircle::ResetNumVtx(void)
{
	// 頂点を最大に設定する
	m_nVtx = GetNumVtx();

	// 終了状況を false にする
	m_bEnd = false;
}

//=========================
// 終了状況の設定処理
//=========================
void CComboCircle::SetEnd(const bool bEnd)
{
	// 終了状況を設定する
	m_bEnd = bEnd;
}

//=========================
// 終了状況の取得処理
//=========================
bool CComboCircle::GetEnd(void)
{
	// 終了状況を返す
	return m_bEnd;
}