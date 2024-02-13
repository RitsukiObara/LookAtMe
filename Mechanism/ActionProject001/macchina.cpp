//===========================================
//
// マキナ草のメイン処理[macchina.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "macchina.h"
#include "macchina_manager.h"
#include "useful.h"

#include"screw.h"

// マクロ定義
#define NONE_SCALE_COUNT	(20)								// 通常状態の動くカウント
#define NONE_SCALE_REDUCE	(D3DXVECTOR3(0.9f, 1.1f, 0.9f))		// 通常状態の縮んだ際の拡大率
#define NONE_ADD_CORRECT	(0.008f)							// 通常状態の補正する時の加算数
#define REDUCE_SCALE_DEST	(D3DXVECTOR3(1.6f, 0.6f, 1.6f))		// 縮小状態の目的のサイズ
#define REDUCE_ADD_CORRECT	(0.2f)								// 縮小状態の補正するときの加算数
#define REDUCE_COUNT		(3)									// 縮小状態のカウント
#define EXPANSE_SCALE_DEST	(D3DXVECTOR3(0.6f, 1.5f, 0.6f))		// 伸び状態の目的のサイズ
#define EXPANSE_ADD_CORRECT	(0.07f)								// 伸び状態の補正するときの加算数
#define EXPANSE_COUNT		(10)								// 伸び状態のカウント
#define EJECT_SCALE_DEST	(D3DXVECTOR3(1.6f, 0.5f, 1.6f))		// 排出状態の目的のサイズ
#define EJECT_ADD_CORRECT	(0.01f)								// 排出状態の補正するときの加算数
#define SCREW_SHIFT_Z		(100.0f)							// ネジのずらす幅(Z軸)
#define SCREW_MOVE_Y		(20.0f)								// ネジの移動量(Y軸)

//==============================
// コンストラクタ
//==============================
CMacchina::CMacchina() : CModel(CObject::TYPE_VIVLIO, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_scaleDest = NONE_SCALE;		// 目的のサイズ
	m_state = STATE_NONE;			// 状態
	m_nStateCount = 0;				// 状態カウント
	m_bCollision = true;			// 当たり判定の有無

	m_pPrev = nullptr;		// 前へのポインタ
	m_pNext = nullptr;		// 次へのポインタ

	if (CMacchinaManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CMacchinaManager::Get()->Regist(this);
	}
}

//==============================
// デストラクタ
//==============================
CMacchina::~CMacchina()
{

}

//============================
// 前のポインタの設定処理
//============================
void CMacchina::SetPrev(CMacchina* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CMacchina::SetNext(CMacchina* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CMacchina* CMacchina::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CMacchina* CMacchina::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CMacchina::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_scaleDest = NONE_SCALE;		// 目的のサイズ
	m_state = STATE_NONE;			// 状態
	m_nStateCount = 0;				// 状態カウント
	m_bCollision = true;			// 当たり判定の有無

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CMacchina::Uninit(void)
{
	// 終了処理
	CModel::Uninit();

	if (CMacchinaManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CMacchinaManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//========================================
//ブロックの更新処理
//========================================
void CMacchina::Update(void)
{
	switch (m_state)
	{
	case STATE_NONE:		// 通常状態

		// 状態カウントを加算する
		m_nStateCount++;

		// 通常状態の拡大率変化処理
		NoneRescale();

		break;

	case STATE_REDUCE:		// 縮小状態

		// 状態カウントを加算する
		m_nStateCount++;

		// 縮小状態の拡大率変化処理
		ReduceScale();

		if (m_nStateCount >= REDUCE_COUNT)
		{ // 状態カウントが一定以上になった場合

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 伸び状態にする
			m_state = STATE_EXPANSE;

			// 目的の拡大率を設定する
			m_scaleDest = EXPANSE_SCALE_DEST;
		}

		break;

	case STATE_EXPANSE:		// 伸び状態

		// 状態カウントを初期化する
		m_nStateCount++;

		// 伸び状態の拡大率変化処理
		ExpanseScale();

		if (m_nStateCount >= EXPANSE_COUNT)
		{ // 状態カウントが一定以上になった場合

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 排出状態にする
			m_state = STATE_EJECT;

			// 目的の拡大率を設定する
			m_scaleDest = EJECT_SCALE_DEST;

			// ネジの生成処理
			CScrew::Create(D3DXVECTOR3(GetPos().x, GetPos().y, GetPos().z - SCREW_SHIFT_Z), D3DXVECTOR3(0.0f, SCREW_MOVE_Y, 0.0f), true);
		}

		break;

	case STATE_EJECT:		// 排出状態

		// 排出状態の拡大率変化処理
		EjectScale();

		break;

	default:				// 上記以外

		// 停止
		assert(false);

		break;
	}
}

//=====================================
//ブロックの描画処理
//=====================================
void CMacchina::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// ヒット処理
//=====================================
void CMacchina::Hit(void)
{
	// 状態を設定する
	m_state = STATE_REDUCE;

	// 状態カウントを初期化する
	m_nStateCount = 0;

	// 目的の拡大率を設定する
	m_scaleDest = REDUCE_SCALE_DEST;

	// 当たり判定を false にする
	m_bCollision = false;
}

//=====================================
// 情報の設定処理
//=====================================
void CMacchina::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);							// 位置
	SetPosOld(pos);							// 前回の位置
	SetRot(NONE_D3DXVECTOR3);				// 向き
	SetScale(NONE_SCALE);					// 拡大率
	SetFileData(CXFile::TYPE_MACCHINA);		// モデルの情報

	// 全ての値を初期化する
	m_scaleDest = NONE_SCALE;		// 目的のサイズ
	m_state = STATE_NONE;			// 状態
	m_nStateCount = 0;				// 状態カウント
	m_bCollision = true;			// 当たり判定の有無
}

//=====================================
// 当たり判定の取得処理
//=====================================
bool CMacchina::GetCollision(void)
{
	// 当たり判定の有無を返す
	return m_bCollision;
}

//=======================================
// 生成処理
//=======================================
CMacchina* CMacchina::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CMacchina* pMacchina = nullptr;	// インスタンスを生成

	if (pMacchina == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pMacchina = new CMacchina;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pMacchina != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pMacchina->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pMacchina->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// マキナ草のポインタを返す
	return pMacchina;
}

//=======================================
// 通常状態の拡大率の変化処理
//=======================================
void CMacchina::NoneRescale(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 scale = GetScale();		// 拡大率を取得する

	if (m_nStateCount % NONE_SCALE_COUNT == 0)
	{ // 状態カウントが一定以上になった場合

		// 目的のサイズを設定する
		m_scaleDest = (m_scaleDest.x <= NONE_SCALE_REDUCE.x) ? NONE_SCALE : NONE_SCALE_REDUCE;
	}

	// サイズの補正処理
	useful::FrameCorrect(m_scaleDest.x, &scale.x, NONE_ADD_CORRECT);
	useful::FrameCorrect(m_scaleDest.y, &scale.y, NONE_ADD_CORRECT);
	useful::FrameCorrect(m_scaleDest.z, &scale.z, NONE_ADD_CORRECT);

	// 拡大率を適用する
	SetScale(scale);
}

//=======================================
// 縮小状態の拡大率の変化処理
//=======================================
void CMacchina::ReduceScale(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 scale = GetScale();		// 拡大率を取得する

	// サイズの補正処理
	useful::FrameCorrect(m_scaleDest.x, &scale.x, REDUCE_ADD_CORRECT);
	useful::FrameCorrect(m_scaleDest.y, &scale.y, REDUCE_ADD_CORRECT);
	useful::FrameCorrect(m_scaleDest.z, &scale.z, REDUCE_ADD_CORRECT);

	// 拡大率を適用する
	SetScale(scale);
}

//=======================================
// 伸び状態の拡大率の変化処理
//=======================================
void CMacchina::ExpanseScale(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 scale = GetScale();		// 拡大率を取得する

	// サイズの補正処理
	useful::FrameCorrect(m_scaleDest.x, &scale.x, EXPANSE_ADD_CORRECT);
	useful::FrameCorrect(m_scaleDest.y, &scale.y, EXPANSE_ADD_CORRECT);
	useful::FrameCorrect(m_scaleDest.z, &scale.z, EXPANSE_ADD_CORRECT);

	// 拡大率を適用する
	SetScale(scale);
}

//=======================================
// 排出状態の拡大率の変化処理
//=======================================
void CMacchina::EjectScale(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 scale = GetScale();		// 拡大率を取得する

	// サイズの補正処理
	useful::FrameCorrect(m_scaleDest.x, &scale.x, EJECT_ADD_CORRECT);
	useful::FrameCorrect(m_scaleDest.y, &scale.y, EJECT_ADD_CORRECT);
	useful::FrameCorrect(m_scaleDest.z, &scale.z, EJECT_ADD_CORRECT);

	// 拡大率を適用する
	SetScale(scale);
}