//===========================================
//
// 飛行機のメイン処理[airplane.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "airplane.h"
#include "useful.h"

#include "game.h"
#include "player.h"
#include "propeller.h"
#include "camera.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Airplane.x";		// モデルのパス名
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, 2000.0f, 30000.0f);		// 位置
	const float MOVE_MAGNI = 0.005f;					// 移動量の倍率
}

//==============================
// コンストラクタ
//==============================
CAirplane::CAirplane() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pPropeller = nullptr;			// プロペラの情報
	m_posDest = NONE_D3DXVECTOR3;	// 目的の位置
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_state = STATE_MOVE;			// 状態
}

//==============================
// デストラクタ
//==============================
CAirplane::~CAirplane()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CAirplane::Init(void)
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
void CAirplane::Uninit(void)
{
	if (m_pPropeller != nullptr)
	{ // プロペラが NULL じゃない場合

		// 終了処理
		m_pPropeller->Uninit();
		m_pPropeller = nullptr;
	}

	// 終了処理
	CModel::Uninit();
}

//=====================================
// 破片の更新処理
//=====================================
void CAirplane::Update(void)
{
	// 墜落処理
	Fall();

	if (m_pPropeller != nullptr)
	{ // プロペラが NULL じゃない場合

		// 更新処理
		m_pPropeller->Update();
	}
}

//=====================================
// 更新処理(実際使う処理)
//=====================================
void CAirplane::Update(CPlayer* pPlayer)
{
	// 移動処理
	Move(pPlayer);

	if (m_pPropeller != nullptr)
	{ // プロペラが NULL じゃない場合

		// 更新処理
		m_pPropeller->Update();
	}
}

//=====================================
// 破片の描画処理
//=====================================
void CAirplane::Draw(void)
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
void CAirplane::SetData(const D3DXVECTOR3& posDest)
{
	// 情報の設定処理
	SetPos(POS);						// 位置
	SetPosOld(POS);						// 前回の位置
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデル情報

	// 全ての値を設定する
	m_pPropeller = CPropeller::Create(GetMatrixPoint());		// プロペラの情報
	m_posDest = posDest;				// 目的の位置
	m_state = STATE_MOVE;			// 状態

	// 移動量を設定
	m_move.x = fabsf((posDest.x - POS.x) * MOVE_MAGNI);
	m_move.y = 0.0f;
	m_move.z = fabsf((posDest.z - POS.z) * MOVE_MAGNI);
}

//=======================================
// 生成処理
//=======================================
CAirplane* CAirplane::Create(const D3DXVECTOR3& posDest)
{
	// ローカルオブジェクトを生成
	CAirplane* pAirplane = nullptr;	// インスタンスを生成

	if (pAirplane == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pAirplane = new CAirplane;
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
		pAirplane->SetData(posDest);
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
void CAirplane::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CAirplane::STATE CAirplane::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 移動処理
//=======================================
void CAirplane::Move(CPlayer* pPlayer)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	if (useful::FrameCorrect(m_posDest.x, &pos.x, m_move.x) == true ||
		useful::FrameCorrect(m_posDest.z, &pos.z, m_move.z) == true)
	{ // 目的の位置に着いた場合

		// 目的の位置に補正する
		pos.x = m_posDest.x;
		pos.z = m_posDest.z;

		// プレイヤー登場状態にする
		CManager::Get()->GetCamera()->SetType(CCamera::TYPE_PLAYERAPPEAR);

		// カメラの視点を設定する
		CManager::Get()->GetCamera()->SetPosV(D3DXVECTOR3(pos.x - 1000.0f, pos.y + 1500.0f, pos.z));

		// 移動量の設定処理
		pPlayer->SetMove(D3DXVECTOR3(0.0f, 30.0f, 10.0f));

		// 飛行機の管轄外し処理
		pPlayer->RemoveAirplane();

		// 墜落状態にする
		m_state = STATE_FALL;

		// 種類を飛行機に変える
		SetType(TYPE_AIRPLANE);
	}

	// 位置を適用
	SetPos(pos);
}

//=======================================
// 墜落処理
//=======================================
void CAirplane::Fall(void)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	// 移動する
	pos.x -= m_move.x;
	pos.y -= 5.0f;
	pos.z -= m_move.z;

	// 位置を適用
	SetPos(pos);
}