//===========================================
//
// 拳銃のメイン処理[handgun.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "dagger.h"
#include "useful.h"

#include "orbit.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Dagger.x";		// モデルの名前
	const char* ORBIT_TEXTURE = "data\\TEXTURE\\DaggerOrbit.png";		// 軌跡のテクスチャ
	const D3DXVECTOR3 DAGGER_POS = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);		// ダガーの位置
	const D3DXVECTOR3 DAGGER_ROT = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// ダガーの向き
	const D3DXVECTOR3 ORBIT_OFFSET[2] =					// 軌跡のオフセット
	{
		D3DXVECTOR3(0.0f, 0.0f, -10.0f),
		D3DXVECTOR3(0.0f, 0.0f, -50.0f)
	};
}

//==============================
// コンストラクタ
//==============================
CDagger::CDagger() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_pOrbit = nullptr;				// 軌跡の情報
	m_pMtxParent = nullptr;			// 親のマトリックス
	m_bDisp = false;				// 描画状況
	m_bDispOrbit = false;			// 軌跡の描画状況
	m_bDispOrbitOld = m_bDispOrbit;	// 軌跡の前回の描画状況
}

//==============================
// デストラクタ
//==============================
CDagger::~CDagger()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CDagger::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
// 破片の終了処理
//========================================
void CDagger::Uninit(void)
{
	if (m_pOrbit != nullptr)
	{ // 軌跡が NULL じゃない場合

		// 軌跡の終了処理
		m_pOrbit->Uninit();
		m_pOrbit = nullptr;
	}

	// 終了処理
	CModel::Uninit();
}

//=====================================
// 破片の更新処理
//=====================================
void CDagger::Update(void)
{

}

//=====================================
// 破片の描画処理
//=====================================
void CDagger::Draw(void)
{
	if (m_bDisp == true)
	{ // 描画状況が true の場合

		// 描画処理
		CModel::DrawMatrix(*m_pMtxParent);
	}

	if (m_bDispOrbit == true &&
		m_pOrbit != nullptr)
	{ // 軌跡が NULL じゃない場合

		if (m_bDispOrbit == true &&
			m_bDispOrbitOld == false)
		{ // 描画状況が true になった瞬間

			// 位置のリセット処理
			m_pOrbit->PosReset();
		}

		// 描画処理
		m_pOrbit->Draw();
	}

	// 前回の描画状況を代入する
	m_bDispOrbitOld = m_bDispOrbit;
}

//=====================================
// 情報の設定処理
//=====================================
void CDagger::SetData(D3DXMATRIX* mtx)
{
	// 情報の設定処理
	SetPos(DAGGER_POS);					// 位置
	SetPosOld(DAGGER_POS);				// 前回の位置
	SetRot(DAGGER_ROT);					// 向き
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデル情報

	// 全ての値をクリアする
	m_pMtxParent = mtx;				// 親のマトリックス
	m_bDisp = false;				// 描画状況
	m_bDispOrbit = false;			// 軌跡の描画状況
	m_bDispOrbitOld = m_bDispOrbit;	// 軌跡の前回の描画状況

	// 軌跡を生成する
	m_pOrbit = COrbit::Create(GetMatrixPoint(), GetPos(), ORBIT_OFFSET[0], ORBIT_OFFSET[1], ORBIT_TEXTURE, TYPE_NONE);
}

//=======================================
// 生成処理
//=======================================
CDagger* CDagger::Create(D3DXMATRIX* mtx)
{
	// ローカルオブジェクトを生成
	CDagger* pDagger = nullptr;	// インスタンスを生成

	if (pDagger == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pDagger = new CDagger;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pDagger != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pDagger->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pDagger->SetData(mtx);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ダガーのポインタを返す
	return pDagger;
}

//=======================================
// 描画状況の設定処理
//=======================================
void CDagger::SetEnableDisp(const bool bDisp)
{
	// 描画状況を設定する
	m_bDisp = bDisp;
}

//=======================================
// 描画状況の取得処理
//=======================================
bool CDagger::IsDisp(void) const
{
	// 描画状況を返す
	return m_bDisp;
}

//=======================================
// 軌跡の描画状況の設定処理
//=======================================
void CDagger::SetEnableDispOrbit(const bool bDisp)
{
	// 軌跡の描画状況を設定する
	m_bDispOrbit = bDisp;
}

//=======================================
// 軌跡の描画状況の取得処理
//=======================================
bool CDagger::IsDispOrbit(void) const
{
	// 軌跡の描画状況を返す
	return m_bDispOrbit;
}