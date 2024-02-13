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
#include "handgun.h"
#include "useful.h"

// 無名名前空間
namespace
{
	const char* MODEL = "data\\MODEL\\Pistol.x";		// モデルの名前
	const float ADD_ROT_MOVE = 0.04f;					// 向きの移動量の加算数
	const float MAX_ROT_MOVE = 0.4f;					// 向きの移動量の最大数
	const float RELOAD_COUNT = 50;						// リロードカウント
	const float DEST_LENGTH = 200.0f;					// 目的の長さ
	const float RELOAD_CORRECT = 5.0f;					// リロード中の補正倍率
	const float RETURN_CORRECT = 16.0f;					// 戻り状態の補正倍率
	const float RETURN_ROT_MOVE = 0.9f;					// 戻り状態の向きの移動量
}

//==============================
// コンストラクタ
//==============================
CHandgun::CHandgun() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_posDest = NONE_D3DXVECTOR3;	// 目的の位置
	m_posInit = NONE_D3DXVECTOR3;	// 初期位置
	m_rotInit = NONE_D3DXVECTOR3;	// 初期向き
	m_pMtxParent = nullptr;			// 親のマトリックス
	m_state = STATE_NONE;			// 状態
	m_nReloadCount = 0;				// リロードカウント
	m_fRotMove = 0.0f;				// 向きの移動量
	m_bDisp = true;					// 描画状況
}

//==============================
// デストラクタ
//==============================
CHandgun::~CHandgun()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CHandgun::Init(void)
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
void CHandgun::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//=====================================
// 破片の更新処理
//=====================================
void CHandgun::Update(void)
{
	switch (m_state)
	{
	case CHandgun::STATE_NONE:

		// 特になし

		break;

	case CHandgun::STATE_RELOAD:

		// 向きの移動処理
		RotMove();

		// 位置の移動処理
		PosMove();

		// リロードカウントを加算する
		m_nReloadCount++;

		if (m_nReloadCount >= RELOAD_COUNT)
		{ // リロードカウントが一定以上の場合

			// リロードカウントを初期化する
			m_nReloadCount = 0;

			// 戻り状態にする
			m_state = STATE_RETURN;

			// 向きの移動量を初期化する
			m_fRotMove = 0.0f;
		}

		break;

	case CHandgun::STATE_RETURN:

		// 戻り状態
		ReturnMove();

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=====================================
// 破片の描画処理
//=====================================
void CHandgun::Draw(void)
{
	if (m_bDisp == true)
	{ // 描画状況が true の場合

		// 描画処理
		CModel::DrawMatrix(*m_pMtxParent);
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CHandgun::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot, D3DXMATRIX* mtx)
{
	// 情報の設定処理
	SetPos(pos);						// 位置
	SetPosOld(pos);						// 前回の位置
	SetRot(rot);						// 向き
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデル情報

	// 全ての値をクリアする
	m_posInit = pos;		// 初期位置
	m_rotInit = rot;		// 初期向き
	m_pMtxParent = mtx;		// 親のマトリックス
	m_state = STATE_NONE;	// 状態
	m_fRotMove = 0.0f;		// 向きの移動量
	m_bDisp = true;			// 描画状況

	// 目的の位置を設定する
	m_posDest = D3DXVECTOR3(m_posInit.x + sinf(m_rotInit.y) * -DEST_LENGTH, m_posInit.y, m_posInit.z + DEST_LENGTH);
}

//=======================================
// 生成処理
//=======================================
CHandgun* CHandgun::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot, D3DXMATRIX* mtx)
{
	// ローカルオブジェクトを生成
	CHandgun* pGun = nullptr;	// インスタンスを生成

	if (pGun == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pGun = new CHandgun;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pGun != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pGun->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pGun->SetData(pos, rot, mtx);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 拳銃のポインタを返す
	return pGun;
}

//=======================================
// 状態の設定処理
//=======================================
void CHandgun::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CHandgun::STATE CHandgun::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 描画状況の設定処理
//=======================================
void CHandgun::SetEnableDisp(const bool bDisp)
{
	// 描画状況を設定する
	m_bDisp = bDisp;
}

//=======================================
// 描画状況の取得処理
//=======================================
bool CHandgun::IsDisp(void) const
{
	// 描画状況を設定する
	return m_bDisp;
}

//=======================================
// 位置の移動処理
//=======================================
void CHandgun::PosMove(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 位置の均等な補正処理
	useful::FrameCorrect(m_posDest.x, &pos.x, RELOAD_CORRECT);
	useful::FrameCorrect(m_posDest.y, &pos.y, RELOAD_CORRECT);
	useful::FrameCorrect(m_posDest.z, &pos.z, RELOAD_CORRECT);
	
	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 向きの移動処理
//=======================================
void CHandgun::RotMove(void)
{
	// 向きを取得する
	D3DXVECTOR3 rot = GetRot();

	// 向きを加算する
	rot.y += m_fRotMove;

	// 向きの正規化処理
	useful::RotNormalize(&rot.y);

	// 向きを適用する
	SetRot(rot);

	if (m_fRotMove < MAX_ROT_MOVE)
	{ // 向きの移動量が最大値未満の場合

		// 向きの移動量を加算する
		m_fRotMove += ADD_ROT_MOVE;
	}
}

//=======================================
// 戻り状態の移動処理
//=======================================
void CHandgun::ReturnMove(void)
{
	// 位置と向きを取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 向きを加算する
	rot.y += RETURN_ROT_MOVE;

	// 向きの正規化処理
	useful::RotNormalize(&rot.y);

	// 位置の均等な補正処理
	if (useful::FrameCorrect(m_posInit.x, &pos.x, RETURN_CORRECT) == true ||
		useful::FrameCorrect(m_posInit.y, &pos.y, RETURN_CORRECT) == true ||
		useful::FrameCorrect(m_posInit.z, &pos.z, RETURN_CORRECT) == true)
	{ // 補正し終わった場合

		// 初期位置に直す
		pos = m_posInit;

		// 初期向きに直す
		rot = m_rotInit;

		// 通常状態にする
		m_state = STATE_NONE;
	}

	// 位置と向きを適用する
	SetPos(pos);
	SetRot(rot);
}