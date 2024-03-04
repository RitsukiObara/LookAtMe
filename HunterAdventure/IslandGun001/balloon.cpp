//===========================================
//
// 風船のメイン処理[balloon.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "balloon.h"
#include "useful.h"

#include "balloon_rope.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Balloon.x";		// 風船モデルの名前
	const float HEIGHT = 40.0f;			// 高さの変化量
	const float ADD_ROT = 0.02f;		// 向きの加算量
}

//==============================
// コンストラクタ
//==============================
CBalloon::CBalloon() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pRope = nullptr;			// 紐のモデル
	m_fPosYInit = 0.0f;			// 初期の高さ
	m_fHeightRot = 0.0f;		// 高さを決める向き
}

//==============================
// デストラクタ
//==============================
CBalloon::~CBalloon()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CBalloon::Init(void)
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
void CBalloon::Uninit(void)
{
	if (m_pRope != nullptr)
	{ // 紐が NULL じゃない場合

		// 紐の終了処理
		m_pRope->Uninit();
		m_pRope = nullptr;
	}

	// 終了処理
	CModel::Uninit();
}

//=====================================
// 破片の更新処理
//=====================================
void CBalloon::Update(void)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	// 位置を設定する
	pos.y = m_fPosYInit + sinf(m_fHeightRot) * HEIGHT;

	// 向きを加算する
	m_fHeightRot += ADD_ROT;

	// 向きの正規化
	useful::RotNormalize(&m_fHeightRot);

	// 位置を適用
	SetPos(pos);
}

//=====================================
// 破片の描画処理
//=====================================
void CBalloon::Draw(void)
{
	// 描画処理
	CModel::Draw();

	if (m_pRope != nullptr)
	{ // 紐が NULL じゃない場合

		// 紐の描画処理
		m_pRope->Draw();
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CBalloon::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetScale(NONE_SCALE);			// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデル情報

	// 全ての値を設定する
	m_fPosYInit = pos.y;			// 初期の高さ
	m_fHeightRot = 0.0f;			// 高さを決める向き

	if (m_pRope == nullptr)
	{ // 紐が NULL じゃない場合

		// 紐の生成
		m_pRope = CBalloonRope::Create(GetMatrixPoint());
	}
}

//=======================================
// 生成処理
//=======================================
CBalloon* CBalloon::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CBalloon* pBalloon = nullptr;		// インスタンスを生成

	if (pBalloon == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pBalloon = new CBalloon;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBalloon != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBalloon->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBalloon->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 風船のポインタを返す
	return pBalloon;
}