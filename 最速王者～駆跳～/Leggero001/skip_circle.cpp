//=======================================
//
// スキップ円のメイン処理[skip_circle.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "scene.h"
#include "tutorial.h"
#include "skip_circle.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"
#include "input.h"

//=======================================
// マクロ定義
//=======================================
#define ADD_SIZE_DEST		(10.0f)							// 目標のサイズの追加量
#define ADD_SIZE_CORRECT	(ADD_SIZE_DEST / 10)			// サイズの補正数

//=========================
// コンストラクタ
//=========================
CSkipCircle::CSkipCircle() : CDonut2D(CObject::TYPE_SKIP, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_nVtx = 0;				// 頂点数
	m_fSizeDest = 0.0f;		// 目的のサイズ
	m_fSize = 0.0f;			// サイズ(保存用)
	m_fDist = 0.0f;			// 距離(保存用)
	m_fDistDest = 0.0f;		// 目的の距離
	m_bEnd = false;			// 終了状況
}

//=========================
// デストラクタ
//=========================
CSkipCircle::~CSkipCircle()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CSkipCircle::Init(void)
{
	if (FAILED(CDonut2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 色を設定する
	SetVtxColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CSkipCircle::Uninit(void)
{
	// 終了
	CDonut2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CSkipCircle::Update(void)
{
	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{ // チュートリアルモードの場合

		if ((CManager::GetInputKeyboard()->GetPress(DIK_RETURN) == true ||
			CManager::GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_START,0) == true) &&
			m_nVtx < GetNumVtx())
		{ // ENTERキーを押した場合

			// 押している時の処理
			PushProcess();
		}
		else if (m_nVtx > 0)
		{ // 押していない場合

			// 押していないときの処理
			LeaveProcess();
		}

		// 終了時の処理
		EndProcess();
	}

	if (m_bEnd == true)
	{ // 終了状況が true の場合

		// 頂点数を設定する
		m_nVtx = GetNumVtx();

		// 拡大処理
		BigScale();
	}

	// サイズ補正処理
	SizeCorrect();

	if (m_nVtx <= 0)
	{ // 頂点数が 0 以下の場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点座標の設定処理
	SetVertex();

	// 頂点カラーの設定処理(範囲設定)
	SetVtxColor(m_nVtx, D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));

	// テクスチャ座標の設定処理
	SetVtxTexture();
}

//=========================
// 描画処理
//=========================
void CSkipCircle::Draw(void)
{
	// 描画処理
	CDonut2D::Draw();
}

//=========================
// 情報の設定処理
//=========================
void CSkipCircle::SetData(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi)
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
	m_fSize = fSize;		// サイズ(保存用)
	m_fSizeDest = fSize;	// 目的のサイズ
	m_fDist = fDist;		// 距離(保存用)
	m_fDistDest = fDist;	// 目的の距離
	m_bEnd = false;			// 終了状況
}

//=========================
// 生成処理
//=========================
CSkipCircle* CSkipCircle::Create(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi)
{
	// ローカルオブジェクトを生成
	CSkipCircle* pSkipCircle = nullptr;	// プレイヤーのインスタンスを生成

	if (pSkipCircle == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pSkipCircle = new CSkipCircle;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pSkipCircle != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 情報の設定処理
		pSkipCircle->SetData(pos, fSize, fDist, nDivi);

		// 初期化処理
		if (FAILED(pSkipCircle->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "エフェクト2Dの初期化に失敗！", "警告！", MB_ICONWARNING);

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
	return pSkipCircle;
}

//=========================
// 押している時の処理
//=========================
void CSkipCircle::PushProcess(void)
{
	// 頂点数を加算する
	m_nVtx++;

	// 拡大処理
	BigScale();
}

//=========================
// 押していないときの処理
//=========================
void CSkipCircle::LeaveProcess(void)
{
	// 頂点数を減算する
	m_nVtx--;

	// 目標のサイズを設定する
	m_fSizeDest = m_fSize;

	// 目標の距離を設定する
	m_fDistDest = m_fDist;
}

//=========================
// 終了時の処理
//=========================
void CSkipCircle::EndProcess(void)
{
	if (m_nVtx >= GetNumVtx())
	{ // 総頂点数以上になった場合

		// 頂点数を設定する
		m_nVtx = GetNumVtx();

		// 状態を設定する
		CTutorial::SetState(CTutorial::STATE_SKIP);

		// 終了状況を true にする
		m_bEnd = true;
	}
}

//=========================
// 拡大処理
//=========================
void CSkipCircle::BigScale(void)
{
	// 目標のサイズを設定する
	m_fSizeDest = m_fSize + ADD_SIZE_DEST;

	// 目標の距離を設定する
	m_fDistDest = m_fDist + ADD_SIZE_DEST;
}

//=========================
// サイズの補正処理
//=========================
void CSkipCircle::SizeCorrect(void)
{
	// ローカル変数宣言
	float fSize = GetSize();		// サイズ
	float fDist = GetDistance();	// 距離

	// 情報の補正処理
	FrameCorrect(m_fSizeDest, &fSize, ADD_SIZE_CORRECT);		// サイズ
	FrameCorrect(m_fDistDest, &fDist, ADD_SIZE_CORRECT);	// 距離

	// 情報の設定処理
	SetSize(fSize);			// サイズ設定処理
	SetDistance(fDist);		// 距離の設定処理
}