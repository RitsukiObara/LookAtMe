//===========================================
//
// ライトのメイン処理[light.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"

#include "input.h"
#include "debugproc.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const D3DXVECTOR3 LIGHT_VEC[3] =		// ライトの方向
	{
		D3DXVECTOR3(0.22f, -0.87f, 0.22f),
		D3DXVECTOR3(-0.9f, -0.2f, 0.2f),
		D3DXVECTOR3(0.55f, 0.3f, 0.88f)
	};
	const D3DXCOLOR LIGHT_DIFFUSE[3] =		// ライトの色
	{
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f),
		D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f)
	};

	const D3DXCOLOR CAMERALIGHT_DIFFUSE = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);	// カメラ追従ライトの色
}

//=========================
// コンストラクタ
//=========================
CLight::CLight()
{
	// ライトの情報
	ZeroMemory(m_aLight, sizeof(m_aLight));
	ZeroMemory(&m_lightCamera, sizeof(m_lightCamera));
}

//=========================
// デストラクタ
//=========================
CLight::~CLight()
{

}

//=========================
// ライトの初期化処理
//=========================
HRESULT CLight::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;				// 設定用方向ベクトル

	for (int nCntSet = 0; nCntSet < NUM_LIGHT; nCntSet++)
	{
		// ライトの種類を設定
		m_aLight[nCntSet].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		m_aLight[nCntSet].Diffuse = LIGHT_DIFFUSE[nCntSet];

		// ライトの方向を設定
		vecDir = LIGHT_VEC[nCntSet];

		D3DXVec3Normalize(&vecDir, &vecDir);				// ベクトルを正規化する

		m_aLight[nCntSet].Direction = vecDir;

		// ライトを設定する
		pDevice->SetLight(nCntSet, &m_aLight[nCntSet]);

		// ライトを有効にする
		pDevice->LightEnable(nCntSet, TRUE);
	}

	// ライトの種類を設定
	m_lightCamera.Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	m_lightCamera.Diffuse = CAMERALIGHT_DIFFUSE;

	// ライトの方向を設定
	vecDir = NONE_D3DXVECTOR3;

	D3DXVec3Normalize(&vecDir, &vecDir);				// ベクトルを正規化する

	m_lightCamera.Direction = vecDir;

	// ライトを設定する
	pDevice->SetLight(3, &m_lightCamera);

	// ライトを有効にする
	pDevice->LightEnable(3, TRUE);

	// 成功を返す
	return S_OK;
}

//=========================
// ライトの終了処理
//=========================
void CLight::Uninit(void)
{
	// メモリを開放する
	delete this;
}

//=========================
// ライトの更新処理
//=========================
void CLight::Update(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;				// 設定用方向ベクトル

	for (int nCntSet = 0; nCntSet < NUM_LIGHT; nCntSet++)
	{
		// ライトを設定する
		pDevice->SetLight(nCntSet, &m_aLight[nCntSet]);
	}

	// カメラ追従ライトを設定する
	pDevice->SetLight(3, &m_lightCamera);
}

//=========================
// ライトの取得処理
//=========================
D3DLIGHT9 CLight::GetLightInfo(const int nIdx) const
{
	if (nIdx >= NUM_LIGHT)
	{ // ライトの上限を超えていた場合

		// 停止
		assert(false);

		// 代わりに0番目のライトを渡す
		return m_aLight[0];
	}

	// ライトの情報を返す
	return m_aLight[nIdx];
}

//=========================
// カメラ追従ライトの設定処理
//=========================
void CLight::SetLightCamera(const D3DLIGHT9& light)
{
	// カメラ追従ライトを設定する
	m_lightCamera = light;
}

//=========================
// カメラ追従ライトの取得処理
//=========================
D3DLIGHT9 CLight::GetLightCamera(void) const
{
	// カメラ追従ライトを返す
	return m_lightCamera;
}

//=========================
// 生成処理
//=========================
CLight* CLight::Create(void)
{
	// ローカルオブジェクトを生成
	CLight* pLight = nullptr;		// ライトのインスタンスを生成

	if (pLight == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLight = new CLight;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pLight != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLight->Init()))
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

	// ライトのポインタを返す
	return pLight;
}