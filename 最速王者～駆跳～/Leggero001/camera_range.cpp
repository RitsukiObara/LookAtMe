//============================================
//
// カメラの範囲のメイン処理[camera_range.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "renderer.h"
#include "camera_range.h"

//===========================================
// コンストラクタ
//===========================================
CCameraRange::CCameraRange() : CObject3D(TYPE_CAMERARANGE, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// オーバーロードコンストラクタ
//===========================================
CCameraRange::CCameraRange(CObject::TYPE type, PRIORITY priority) : CObject3D(type, priority)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// デストラクタ
//===========================================
CCameraRange::~CCameraRange()
{

}

//===========================================
// コンストラクタの箱
//===========================================
void CCameraRange::Box(void)
{
	// 全ての値をクリアする
	m_type = CCamera::TYPE_NONE;					// カメラの種類
}

//===========================================
// 初期化処理
//===========================================
HRESULT CCameraRange::Init(void)
{
	if (FAILED(CObject3D::Init()))
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
void CCameraRange::Uninit(void)
{
	// 終了
	CObject3D::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CCameraRange::Update(void)
{
	// ローカル変数を宣言
	D3DXVECTOR3 pos = GetPos();			// 弾の位置

	// 前回の位置の設定処理
	SetPosOld(pos);
}

//===========================================
// 描画処理
//===========================================
void CCameraRange::Draw(void)
{
// デバッグ用
#ifdef _DEBUG

	// 描画処理
	CObject3D::Draw();

#endif

}

//===========================================
// 情報の設定処理
//===========================================
void CCameraRange::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type)
{
	// 情報の設定処理
	SetPos(pos);								// 位置
	SetPosOld(pos);								// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き
	SetSize(size);								// サイズ

	// 全ての値を初期化する
	m_type = type;

	// 頂点座標の設定処理
	SetVertex();

	SetVtxColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
}

//===========================================
// カメラの種類の設定処理
//===========================================
void CCameraRange::SetType(const CCamera::TYPE type)
{
	// 種類を設定する
	m_type = type;
}

//===========================================
// カメラの種類の取得処理
//===========================================
CCamera::TYPE CCameraRange::GetType(void) const
{
	// カメラの種類を返す
	return m_type;
}

//===========================================
// 生成処理
//===========================================
CCameraRange* CCameraRange::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type)
{
	// ローカルオブジェクトを生成
	CCameraRange* pCameraRange = nullptr;	// プレイヤーのインスタンスを生成

	if (pCameraRange == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pCameraRange = new CCameraRange;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pCameraRange != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCameraRange->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "カメラの範囲の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pCameraRange->SetData(pos, size, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// カメラの範囲のポインタを返す
	return pCameraRange;
}