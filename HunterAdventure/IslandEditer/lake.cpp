//===========================================
//
// 水場のメイン処理[lake.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "lake.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* TEXTURE[CLake::NUM_MAX] =				// テクスチャ
	{
		"data\\TEXTURE\\Lake001.png",				// 波紋
		"data\\TEXTURE\\Lake002.png",				// 海
	};
	const float TEXTURE_VTX_MAX = 1.0f;					// テクスチャ座標の最大値
	const D3DXVECTOR2 TEXTURE_MOVE[CLake::NUM_MAX] =	// テクスチャ座標の移動量
	{
		D3DXVECTOR2(0.004f, -0.002f),		// 波紋
		D3DXVECTOR2(0.003f, 0.001f)			// 海
	};		
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CLake*> CLake::m_list = {};		// リスト

//==============================
// コンストラクタ
//==============================
CLake::CLake() : CObject3DMultiTex(TYPE_LAKE, PRIORITY_BG)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < NUM_MAX; nCnt++)
	{
		m_aTexU[nCnt] = NONE_D3DXVECTOR2;			// テクスチャ座標の開始位置(U値)
	}

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CLake::~CLake()
{

}

//==============================
// 水場の初期化処理
//==============================
HRESULT CLake::Init(void)
{
	if (FAILED(CObject3DMultiTex::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
// 水場の終了処理
//========================================
void CLake::Uninit(void)
{
	// 終了処理
	CObject3DMultiTex::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
// 水場の更新処理
//========================================
void CLake::Update(void)
{
	for (int nCntBG = 0; nCntBG < NUM_MAX; nCntBG++)
	{
		//移動量を加算する
		m_aTexU[nCntBG].x += TEXTURE_MOVE[nCntBG].x;
		m_aTexU[nCntBG].y += TEXTURE_MOVE[nCntBG].y;

		if (m_aTexU[nCntBG].x >= TEXTURE_VTX_MAX ||
			m_aTexU[nCntBG].x <= -TEXTURE_VTX_MAX)
		{//背景が一番奥まで行った場合

			//背景を0に戻す
			m_aTexU[nCntBG].x = 0.0f;
		}

		if (m_aTexU[nCntBG].y >= TEXTURE_VTX_MAX ||
			m_aTexU[nCntBG].y <= -TEXTURE_VTX_MAX)
		{//背景が一番奥まで行った場合

			//背景を0に戻す
			m_aTexU[nCntBG].y = 0.0f;
		}

		// テクスチャ座標の設定処理
		SetVtxTextureAnim(m_aTexU[nCntBG], nCntBG);
	}
}

//=====================================
// 水場の描画処理
//=====================================
void CLake::Draw(void)
{
	// 描画処理
	CObject3DMultiTex::DrawLightOff();
}

//=====================================
// 情報の設定処理
//=====================================
void CLake::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// 情報の設定処理
	SetPos(pos);
	SetPosOld(pos);
	SetRot(NONE_D3DXVECTOR3);
	SetSize(size);
	SetType(CObject3DMultiTex::TYPE::TYPE_FUSION);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// テクスチャの割り当て処理
		BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[nCnt]), nCnt);
	}

	// 頂点座標の設定処理
	SetVertex();

	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < NUM_MAX; nCnt++)
	{
		m_aTexU[nCnt] = NONE_D3DXVECTOR2;			// テクスチャ座標の開始位置(U値)
	}
}

//=======================================
// 生成処理
//=======================================
CLake* CLake::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// ローカルオブジェクトを生成
	CLake* pRock = nullptr;		// インスタンスを生成

	if (pRock == nullptr)
	{ // オブジェクトが NULL の場合

		// 岩を生成する
		pRock = new CLake;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pRock != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRock->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRock->SetData(pos, size);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 岩のポインタを返す
	return pRock;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CLake*> CLake::GetList(void)
{
	// リストマネージャーを返す
	return m_list;
}