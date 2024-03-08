//============================================
//
// キャラクターブラーのメイン処理[chara_blur.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "chara_blur.h"
#include "model.h"
#include "renderer.h"
#include "useful.h"

//=========================================
// オーバーロードコンストラクタ
//=========================================
CCharaBlur::CCharaBlur(PRIORITY priority) : CCharacter(CObject::TYPE_LOCUSMODEL, priority)
{
	// 全ての値をクリアする
	m_col = NONE_D3DXCOLOR;		// 色
	m_nLife = 0;				// 寿命
	m_fSub = 0.0f;				// 減算量
}

//=========================================
// デストラクタ
//=========================================
CCharaBlur::~CCharaBlur()
{

}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CCharaBlur::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_col = NONE_D3DXCOLOR;		// 色
	m_nLife = 0;				// 寿命
	m_fSub = 0.0f;				// 減算量

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CCharaBlur::Uninit(void)
{
	// 終了処理
	CCharacter::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CCharaBlur::Update(void)
{
	// 透明度を減算する
	m_col.a -= m_fSub;

	if (m_col.a <= 0.0f)
	{ // 透明度が 0.0f 以下になった場合

		// 透明度を補正する
		m_col.a = 0.0f;
	}

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

//===========================================
// プレイヤーの描画処理
//===========================================
void CCharaBlur::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//αブレンディングを加算処理に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CCharacter::Draw(m_col);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================================
// 生成処理
//===========================================
CCharaBlur* CCharaBlur::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const int nNumModel, const D3DXCOLOR& col, const int nLife, const PRIORITY priority)
{
	CCharaBlur* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pPlayer = new CCharaBlur(priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pPlayer;
	}

	if (pPlayer != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPlayer->SetData(pos, rot, scale, nNumModel, col, nLife);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pPlayer;
}

//=======================================
// 情報の設定処理
//=======================================
void CCharaBlur::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const int nNumModel, const D3DXCOLOR& col, const int nLife)
{
	// 全ての値を初期化する
	SetPos(pos);				// 位置
	SetPosOld(GetPos());		// 前回の位置
	SetRot(rot);				// 向き
	SetScale(scale);			// 拡大率

	// ベタ打ち
	SetNumModel(nNumModel);

	// 全ての値を設定する
	m_col = col;				// 色
	m_nLife = nLife;			// 寿命
	m_fSub = m_col.a / nLife;	// 減算量

	// データの設定処理
	CCharacter::SetData();
}

//=======================================
// モデルの情報設定処理
//=======================================
void CCharaBlur::SetModelData(const int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile& filedata, const int nParentIdx)
{
	// 初期化処理
	GetHierarchy(nIdx)->SetPos(pos);				// 位置
	GetHierarchy(nIdx)->SetPosOld(pos);				// 前回の位置
	GetHierarchy(nIdx)->SetRot(rot);				// 向き
	GetHierarchy(nIdx)->SetScale(scale);			// 拡大率
	GetHierarchy(nIdx)->SetFileData(filedata);		// データの設定処理

	if (nParentIdx != NONE_PARENT)
	{ // 親がいる場合

		// 親のポインタ
		GetHierarchy(nIdx)->SetParent(GetHierarchy(nParentIdx));
	}
}