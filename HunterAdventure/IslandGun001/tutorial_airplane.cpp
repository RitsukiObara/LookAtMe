//===========================================
//
// チュートリアル飛行機のメイン処理[tutorial_airplane.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "tutorial_airplane.h"
#include "useful.h"

#include "propeller.h"
#include "tutorial.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Airplane.x";		// モデルのパス名
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 2500.0f);			// 飛行機の初期位置
	const D3DXVECTOR3 INIT_ROT = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);	// 飛行機の初期向き
	const float MOVE_WIDTH = 50.0f;			// 移動量の幅
	const float MOVE_HEIGHT = 8.0f;			// 移動量の高さ
	const float MOVE_ROT_MOVE = 0.004f;		// 移動中の向きの移動量
}

//==============================
// コンストラクタ
//==============================
CTutorialAirplane::CTutorialAirplane() : CModel(CObject::TYPE_AIRPLANE, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_pPropeller = nullptr;			// プロペラの情報
	m_state = STATE_STOP;			// 状態
}

//==============================
// デストラクタ
//==============================
CTutorialAirplane::~CTutorialAirplane()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CTutorialAirplane::Init(void)
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
void CTutorialAirplane::Uninit(void)
{
	if (m_pPropeller != nullptr)
	{ // プロペラが NULL じゃない場合

		// 終了処理
		m_pPropeller->Uninit();
		m_pPropeller = nullptr;
	}

	// 終了処理
	CModel::Uninit();

	// 飛行機をNULLにする
	CTutorial::DeleteAirplane();
}

//=====================================
// 破片の更新処理
//=====================================
void CTutorialAirplane::Update(void)
{
	switch (m_state)
	{
	case CTutorialAirplane::STATE_STOP:


		break;

	case CTutorialAirplane::STATE_FLY:

		// 飛行処理
		Fly();

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	if (m_pPropeller != nullptr)
	{ // プロペラが NULL じゃない場合

		// 更新処理
		m_pPropeller->Update();
	}
}

//=====================================
// 破片の描画処理
//=====================================
void CTutorialAirplane::Draw(void)
{
	// 描画処理
	CModel::Draw();

	if (m_pPropeller != nullptr)
	{ // プロペラの情報が NULL じゃない場合

		// プロペラの描画処理
		m_pPropeller->Draw();
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CTutorialAirplane::SetData(void)
{
	// 情報の設定処理
	SetPos(INIT_POS);					// 位置
	SetPosOld(INIT_POS);				// 前回の位置
	SetRot(INIT_ROT);					// 向き
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデル情報

	// 全ての値を設定する
	m_pPropeller = CPropeller::Create(GetMatrixPoint());		// プロペラの情報
	m_state = STATE_STOP;				// 状態
}

//=======================================
// 生成処理
//=======================================
CTutorialAirplane* CTutorialAirplane::Create(void)
{
	// ローカルオブジェクトを生成
	CTutorialAirplane* pAirplane = nullptr;	// インスタンスを生成

	if (pAirplane == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pAirplane = new CTutorialAirplane;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pAirplane != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pAirplane->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pAirplane->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 飛行機のポインタを返す
	return pAirplane;
}

//=======================================
// 状態の設定処理
//=======================================
void CTutorialAirplane::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CTutorialAirplane::STATE CTutorialAirplane::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 飛行処理
//=======================================
void CTutorialAirplane::Fly(void)
{
	// 位置と向きを取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 移動する
	pos.x += MOVE_WIDTH;
	pos.y += MOVE_HEIGHT;

	// どんどん傾けていく
	rot.x += MOVE_ROT_MOVE;

	// 向きの正規化
	useful::RotNormalize(&rot.x);

	// 位置と向きを適用
	SetPos(pos);
	SetRot(rot);
}