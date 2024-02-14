//=======================================
//
// 草処理[grass.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "game.h"
#include "grass.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "player.h"

//---------------------------------------
// 無名名前空間
//---------------------------------------
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Diabolo.jpg";		// テクスチャ
	const float SUB_LENGTH = 500.0f;			// 傾き始める長さ
}

//---------------------------------------
// 静的メンバ変数宣言
//---------------------------------------
CListManager<CGrass*> CGrass::m_list = {};		// リスト

//=========================
// コンストラクタ
//=========================
CGrass::CGrass() : CObject3D(CObject::TYPE_GRASS, CObject::PRIORITY_BG)
{
	// 全ての値をクリアする
	m_LeftUpDest = NONE_D3DXVECTOR3;		// 目的の左上の座標
	m_RightUpDest = NONE_D3DXVECTOR3;		// 目的の右上の座標

	// リストに追加する
	m_list.Regist(this);
}

//=========================
// デストラクタ
//=========================
CGrass::~CGrass()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CGrass::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CGrass::Uninit(void)
{
	// 終了
	CObject3D::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//=========================
// 更新処理
//=========================
void CGrass::Update(void)
{
	// 頂点座標の設定処理
	SetVertexUnder();
}

//=========================
// 描画処理
//=========================
void CGrass::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// カリングの設定をOFFにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 描画処理
	CObject3D::DrawLightOff();

	// カリングの設定をONにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=========================
// 情報の設定処理
//=========================
void CGrass::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// スクロールの設定処理
	SetPos(pos);					// 位置設定
	SetPosOld(pos);					// 位置設定
	SetRot(rot);					// 向き設定
	SetSize(size);					// サイズ設定

	// テクスチャの割り当て処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

	// 頂点座標の設定処理
	SetVertexUnder();
}

//=========================
// 生成処理
//=========================
CGrass* CGrass::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// ローカルオブジェクトを生成
	CGrass* pGrass = nullptr;	// 草のインスタンスを生成

	if (pGrass == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pGrass = new CGrass;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pGrass != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pGrass->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pGrass->SetData(pos, rot, size);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 草のポインタを返す
	return pGrass;
}

//=========================
// リストの取得処理
//=========================
CListManager<CGrass*> CGrass::GetList(void)
{
	// リストマネージャーを返す
	return m_list;
}
//
////=========================
//// 頂点座標の設定処理
////=========================
//void CGrass::SetVertexPos(void)
//{
//	if (CGame::GetPlayer() != nullptr)
//	{ // プレイヤーが NULL じゃない場合
//
//		D3DXVECTOR3 posPlayer = CGame::GetPlayer()->GetPos();	// プレイヤーの位置
//		D3DXVECTOR3 pos = GetPos();		// 位置
//		float fRotY = GetRot().y;		// Y軸の向き
//		D3DXVECTOR3 size = GetSize();	// サイズ
//		float fRotTop = 0.0f;			// 上の向き
//		float fLength = 0.0f;			// 長さ
//
//		// 上頂点の向きを設定する
//		fRotTop = atan2f(pos.x - posPlayer.x, pos.z - posPlayer.z);
//
//		// 長さを設定する
//		fLength = sqrtf((posPlayer.x - pos.x) * (posPlayer.x - pos.x) + (posPlayer.z - pos.z) * (posPlayer.z - pos.z));
//
//		D3DXVECTOR3 posLeftUp;			// 左上の座標
//		D3DXVECTOR3 posRightUp;			// 右上の座標
//		D3DXVECTOR3 posLeftDown;		// 左下の座標
//		D3DXVECTOR3 posRightDown;		// 右下の座標
//
//		// 左下の座標を設定する
//		posLeftDown.x = -size.x;
//		posLeftDown.y = 0.0f;
//		posLeftDown.z = 0.0f;
//
//		// 右下の座標を設定する
//		posRightDown.x = size.x;
//		posRightDown.y = 0.0f;
//		posRightDown.z = 0.0f;
//
//		if (fLength <= 140.0f)
//		{ // 一定の距離まで近づいた場合
//
//			// 左上の座標を設定する
//			m_LeftUpDest.x = posLeftDown.x + sinf(fRotTop + fRotY + (GetMatrix()._11 - GetMatrix()._13)) * size.y;
//			m_LeftUpDest.y = 40.0f;
//			m_LeftUpDest.z = posLeftDown.z + cosf(fRotTop + fRotY + (GetMatrix()._11 - GetMatrix()._13)) * size.y;
//
//			// 右上の座標を設定する
//			m_RightUpDest.x = posRightDown.x + sinf(fRotTop + fRotY + (GetMatrix()._11 - GetMatrix()._13)) * size.y;
//			m_RightUpDest.y = 40.0f;
//			m_RightUpDest.z = posRightDown.z + cosf(fRotTop + fRotY + (GetMatrix()._11 - GetMatrix()._13)) * size.y;
//		}
//		else
//		{ // 上記以外
//
//			// 左上の座標を設定する
//			m_LeftUpDest.x = -size.x;
//			m_LeftUpDest.y = size.y;
//			m_LeftUpDest.z = 0.0f;
//
//			// 右上の座標を設定する
//			m_RightUpDest.x = size.x;
//			m_RightUpDest.y = size.y;
//			m_RightUpDest.z = 0.0f;
//		}
//
//		// 均等な補正処理
//		useful::Correct(m_LeftUpDest.x, &m_LeftUp.x, 0.15f);
//		useful::Correct(m_LeftUpDest.y, &m_LeftUp.y, 0.15f);
//		useful::Correct(m_LeftUpDest.z, &m_LeftUp.z, 0.15f);
//
//		// 均等な補正処理
//		useful::Correct(m_RightUpDest.x, &m_RightUp.x, 0.15f);
//		useful::Correct(m_RightUpDest.y, &m_RightUp.y, 0.15f);
//		useful::Correct(m_RightUpDest.z, &m_RightUp.z, 0.15f);
//
//		// 頂点座標の設定処理
//		SetVertexHardCoding(m_LeftUp, m_RightUp, posLeftDown, posRightDown);
//	}
//}