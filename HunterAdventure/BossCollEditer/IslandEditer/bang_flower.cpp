//===========================================
//
// 爆弾花のメイン処理[bang_flower.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "bang_flower.h"
#include "texture.h"
#include "useful.h"

#include "bomb.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\BombFlower.x";		// モデルの名前
	const float BOMB_POS_SHIFT = 185.0f;					// 爆弾の位置のずらす幅
	const float BOMB_ADD_HEIGHT = 199.0f;					// 爆弾の高さの追加量
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CBangFlower*> CBangFlower::m_list = {};			// リスト

//==============================
// コンストラクタ
//==============================
CBangFlower::CBangFlower() : CModel(TYPE_BANGFLOWER, PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pBomb = nullptr;		// 爆弾の情報

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CBangFlower::~CBangFlower()
{

}

//==============================
//爆弾花の初期化処理
//==============================
HRESULT CBangFlower::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_pBomb = nullptr;		// 爆弾の情報

	// 値を返す
	return S_OK;
}

//========================================
//爆弾花の終了処理
//========================================
void CBangFlower::Uninit(void)
{
	if (m_pBomb != nullptr)
	{ // 爆弾が NULL じゃない場合

		// 終了処理
		m_pBomb->Uninit();
		m_pBomb = nullptr;
	}

	// 終了処理
	CModel::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
//爆弾花の更新処理
//========================================
void CBangFlower::Update(void)
{
	if (m_pBomb != nullptr)
	{ // 爆弾が NULL じゃない場合

		// 更新処理
		m_pBomb->Update();
	}
}

//=====================================
//爆弾花の描画処理
//=====================================
void CBangFlower::Draw(void)
{
	// 描画処理
	CModel::Draw();

	if (m_pBomb != nullptr)
	{ // 爆弾が NULL じゃない場合

		// 描画処理
		m_pBomb->Draw();
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CBangFlower::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(rot);					// 向き
	SetScale(NONE_SCALE);			// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデルの情報

	D3DXVECTOR3 posBomb;			// 爆弾の位置

	// 爆弾の位置を設定する
	posBomb.x = pos.x + sinf(rot.y + D3DX_PI) * BOMB_POS_SHIFT;
	posBomb.y = pos.y + BOMB_ADD_HEIGHT;
	posBomb.z = pos.z + cosf(rot.y + D3DX_PI) * BOMB_POS_SHIFT;

	// 全ての値を設定する
	m_pBomb = CBomb::Create(posBomb, D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));		// 爆弾の生成
}

//=======================================
// ヒット処理
//=======================================
void CBangFlower::Hit(void)
{
	// バウンド状態にする
	m_pBomb->SetState(CBomb::STATE_BOUND);

	// 種類を 爆弾 に変える
	m_pBomb->SetType(CObject::TYPE_BOMB);

	// 爆弾を管轄から外す
	m_pBomb = nullptr;
}

//=======================================
// 生成処理
//=======================================
CBangFlower* CBangFlower::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカルオブジェクトを生成
	CBangFlower* pBangFlower = nullptr;		// インスタンスを生成

	if (pBangFlower == nullptr)
	{ // オブジェクトが NULL の場合

		// 爆弾花を生成する
		pBangFlower = new CBangFlower;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBangFlower != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBangFlower->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBangFlower->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 爆弾花のポインタを返す
	return pBangFlower;
}

//=======================================
// 爆弾の取得処理
//=======================================
CBomb* CBangFlower::GetBomb(void) const
{
	// 爆弾の情報を返す
	return m_pBomb;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CBangFlower*> CBangFlower::GetList(void)
{
	// リスト構造を返す
	return m_list;
}