//===========================================
//
// ドアのメイン処理[door.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "door.h"
#include "useful.h"

#include "tutorial.h"
#include "push_timing.h"
#include "tutorial_airplane.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	const char* DOOR_MODEL = "data\\MODEL\\Door.x";						// ドアモデルの名前
	const char* FRAME_MODEL = "data\\MODEL\\DoorFrame.x";				// ドアフレームモデルの名前
	const D3DXVECTOR3 DOOR_OFFSET = D3DXVECTOR3(60.7f, 0.0f, 0.0f);		// ドアのオフセット
	const float BUTTON_SHIFT = 300.0f;			// ボタンのずらす高さ
	const float BUTTON_SIZE = 30.0f;			// ボタンのサイズ
	const int BUTTON_INTERVAL = 10;				// ボタンの間隔

	const float OPEN_ROT = -D3DX_PI * 0.5f;		// 開き状態の時の向き
	const float ROT_MOVE = 0.1f;				// 向きの移動量
}

//==============================
// コンストラクタ
//==============================
CDoor::CDoor() : CModel(CObject::TYPE_DOOR, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_pDoor = nullptr;			// ドアのモデル
	m_pButton = nullptr;		// ボタンの情報
	m_state = STATE_NONE;		// 状態
	m_bDisp = false;			// ボタンの表示状況
}

//==============================
// デストラクタ
//==============================
CDoor::~CDoor()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CDoor::Init(void)
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
void CDoor::Uninit(void)
{
	if (m_pDoor != nullptr)
	{ // ドアが NULL じゃない場合

		// ドアの終了処理
		m_pDoor->Uninit();
		m_pDoor = nullptr;
	}

	if (m_pButton != nullptr)
	{ // ボタンが NULL じゃない場合

		// ボタンの終了処理
		m_pButton->Uninit();
		m_pButton = nullptr;
	}

	// 終了処理
	CModel::Uninit();

	// ドアのNULL化処理
	CTutorial::DeleteDoor();
}

//=====================================
// 破片の更新処理
//=====================================
void CDoor::Update(void)
{
	// 前回の位置を設定する
	SetPosOld(GetPos());

	switch (m_state)
	{
	case CDoor::STATE_NONE:


		break;

	case CDoor::STATE_OPEN:

		// 開き処理
		Open();

		// 描画状況を false にする
		m_bDisp = false;

		break;

	case CDoor::STATE_CLOSE:

		// 閉じ処理
		Close();

		if (CTutorial::GetPlayer() != nullptr &&
			m_pDoor->GetRot().y >= -0.0001f)
		{ // もうほぼ閉まったとき
			
			// 飛行機を飛行状態にする
			CTutorial::GetAirplane()->SetState(CTutorialAirplane::STATE_FLY);
		}

		// 描画状況を false にする
		m_bDisp = false;

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	if (m_pButton != nullptr)
	{ // ボタンが NULL じゃない場合

		// ボタンの更新処理
		m_pButton->Update();
	}
}

//=====================================
// 破片の描画処理
//=====================================
void CDoor::Draw(void)
{
	// 描画処理
	CModel::Draw();

	if (m_pDoor != nullptr)
	{ // ドアが NULL じゃない場合

		// ドアの描画処理
		m_pDoor->DrawMatrix(GetMatrix());
	}

	if (m_bDisp == true &&
		m_pButton != nullptr)
	{ // ボタンが表示可能な場合

		// ボタンの描画処理
		m_pButton->Draw();
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CDoor::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetScale(NONE_SCALE);			// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(FRAME_MODEL));	// モデル情報

	// 全ての値を設定する
	if (m_pDoor == nullptr)
	{ // ドアが NULL じゃない場合

		// ドアの生成
		m_pDoor = CModel::Create(TYPE_NONE, PRIORITY_ENTITY);

		// 情報の設定処理
		m_pDoor->SetPos(DOOR_OFFSET);		// 位置
		m_pDoor->SetPosOld(DOOR_OFFSET);	// 前回の位置
		m_pDoor->SetRot(NONE_D3DXVECTOR3);	// 向き
		m_pDoor->SetScale(NONE_SCALE);		// 拡大率
		m_pDoor->SetFileData(CManager::Get()->GetXFile()->Regist(DOOR_MODEL));	// モデル情報
	}

	if (m_pButton == nullptr)
	{ // ボタンが NULL じゃない場合

		// ボタンの生成
		m_pButton = CPushTiming::Create(D3DXVECTOR3(pos.x, pos.y + BUTTON_SHIFT, pos.z), BUTTON_SIZE, CPushTiming::TYPE_PAD_A, BUTTON_INTERVAL);
		m_pButton->CObject::SetType(TYPE_NONE);
	}
	m_state = STATE_NONE;		// 状態
	m_bDisp = false;			// 描画状況
}

//=======================================
// 生成処理
//=======================================
CDoor* CDoor::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CDoor* pDoor = nullptr;		// インスタンスを生成

	if (pDoor == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pDoor = new CDoor;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pDoor != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pDoor->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pDoor->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ドアのポインタを返す
	return pDoor;
}

//=======================================
// 状態の設定処理
//=======================================
void CDoor::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CDoor::STATE CDoor::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 描画状況の設定処理
//=======================================
void CDoor::SetEnableDisp(const bool bDisp)
{
	// 描画状況を設定する
	m_bDisp = bDisp;
}

//=======================================
// 描画状況の取得処理
//=======================================
bool CDoor::IsDisp(void) const
{
	// 描画状況を返す
	return m_bDisp;
}

//=======================================
// 開き処理
//=======================================
void CDoor::Open(void)
{
	// 向きを取得
	D3DXVECTOR3 rot = m_pDoor->GetRot();

	// 向きの補正処理
	useful::RotCorrect(OPEN_ROT, &rot.y, ROT_MOVE);

	// 向きを適用
	m_pDoor->SetRot(rot);
}

//=======================================
// 閉じ処理
//=======================================
void CDoor::Close(void)
{
	// 向きを取得
	D3DXVECTOR3 rot = m_pDoor->GetRot();

	// 向きの補正処理
	useful::RotCorrect(0.0f, &rot.y, ROT_MOVE);

	// 向きを適用
	m_pDoor->SetRot(rot);
}