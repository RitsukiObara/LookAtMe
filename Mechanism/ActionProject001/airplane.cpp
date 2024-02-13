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
#include "renderer.h"
#include "airplane.h"
#include "airplane_manager.h"
#include "useful.h"

// マクロ定義
#define APPEAR_SCALE		(D3DXVECTOR3(0.5f, 0.5f, 0.5f))		// 出現状態の拡大率
#define APPEAR_ADD_SCALE	(0.01f)								// 出現状態の加算する拡大率
#define APPEAR_ADD_ROT		(D3DX_PI / 25.0f)					// 出現状態の加算する向き
#define APPEAR_ADD_POS_Y	(2.0f)								// 出現状態の加算する位置(Y軸)

//==============================
// コンストラクタ
//==============================
CAirplane::CAirplane() : CModel(CObject::TYPE_AIRPLANE, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_state = STATE_NONE;	// 状態
	m_bFront = false;		// 前後状況
	m_bCollision = true;	// 当たり判定状況
	m_pPrev = nullptr;		// 前へのポインタ
	m_pNext = nullptr;		// 次へのポインタ

	if (CAirplaneManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CAirplaneManager::Get()->Regist(this);
	}
}

//==============================
// デストラクタ
//==============================
CAirplane::~CAirplane()
{

}

//============================
// 前のポインタの設定処理
//============================
void CAirplane::SetPrev(CAirplane* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CAirplane::SetNext(CAirplane* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CAirplane* CAirplane::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CAirplane* CAirplane::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}

//==============================
// 飛行機の初期化処理
//==============================
HRESULT CAirplane::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_state = STATE_NONE;	// 状態
	m_bFront = false;		// 前後状況
	m_bCollision = true;	// 当たり判定状況

	// 値を返す
	return S_OK;
}

//========================================
// 飛行機の終了処理
//========================================
void CAirplane::Uninit(void)
{
	// 終了処理
	CModel::Uninit();

	if (CAirplaneManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CAirplaneManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//========================================
// 飛行機の更新処理
//========================================
void CAirplane::Update(void)
{
	switch (m_state)
	{
	case CAirplane::STATE_APPEAR:		// 出現状態

		// 出現状態の処理
		Appear();

		break;

	case CAirplane::STATE_NONE:			// 通常状態

		// 特に処理無し

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=====================================
// 飛行機の描画処理
//=====================================
void CAirplane::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CAirplane::SetData(const D3DXVECTOR3& pos, const bool bFront, const STATE state)
{
	// 情報の設定処理
	SetPos(pos);								// 位置
	SetPosOld(pos);								// 前回の位置
	SetFileData(CXFile::TYPE_AIRPLANE);			// モデルの情報

	// 全ての値を設定する
	m_state = state;		// 状態
	m_bFront = bFront;		// 前後状況

	switch (m_state)
	{
	case CAirplane::STATE_APPEAR:

		// 当たり判定がない
		m_bCollision = false;

		// 拡大率を設定する
		SetScale(APPEAR_SCALE);

		break;

	case CAirplane::STATE_NONE:

		// 当たり判定が存在する
		m_bCollision = true;

		// 拡大率を設定する
		SetScale(NONE_SCALE);

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	if (m_bFront == true)
	{ // 前後状況が true の場合

		SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));	// 向きを設定する
	}
	else
	{ // 上記以外

		SetRot(NONE_D3DXVECTOR3);					// 向きを設定する
	}
}

//=======================================
// 前後状況の取得処理
//=======================================
bool CAirplane::GetFront(void) const
{
	// 前後状況を返す
	return m_bFront;
}

//=======================================
// 当たり判定状況の取得処理
//=======================================
bool CAirplane::GetCollision(void) const
{
	// 当たり判定状況を返す
	return m_bCollision;
}

//=======================================
// 生成処理
//=======================================
CAirplane* CAirplane::Create(const D3DXVECTOR3& pos, const bool bFront, const STATE state)
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
		pAirplane->SetData(pos, bFront, state);
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
// 出現状態の処理
//=======================================
void CAirplane::Appear(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置を取得する
	D3DXVECTOR3 rot = GetRot();			// 向きを取得する
	D3DXVECTOR3 scale = GetScale();		// 拡大率を取得する

	// 拡大率を加算する
	scale.x += APPEAR_ADD_SCALE;
	scale.y += APPEAR_ADD_SCALE;
	scale.z += APPEAR_ADD_SCALE;

	// 向きを加算する
	rot.x += APPEAR_ADD_ROT;

	// 位置を加算する
	pos.y += APPEAR_ADD_POS_Y;

	if (scale.x >= 1.0f ||
		scale.y >= 1.0f ||
		scale.z >= 1.0f)
	{ // 拡大率が 1,0f 以上の場合

		// 拡大率を補正する
		scale = NONE_SCALE;

		// 向きを補正する
		rot.x = 0.0f;

		// 当たり判定状況を true にする
		m_bCollision = true;

		// 通常状態にする
		m_state = STATE_NONE;
	}

	// 情報を適用する
	SetPos(pos);			// 位置
	SetRot(rot);			// 向き
	SetScale(scale);		// 拡大率
}