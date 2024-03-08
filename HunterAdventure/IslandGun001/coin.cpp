//===========================================
//
// 小判のメイン処理[coin.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "coin.h"
#include "sound.h"
#include "useful.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Coin.x";	// モデルの名前
	const float INIT_CYCLESPEED = 0.08f;		// 初期の回転速度
	const float GET_CYCLESPEED = 0.2f;			// 取得時の回転速度
	const float GET_HEIGHTDEST = 150.0f;		// 取得時の目的の高さ
	const float GET_POS_CORRECT = 10.0f;		// 取得時の位置の補正数
	const int DEATH_COUNT = 70;					// 死亡までのカウント
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CCoin*> CCoin::m_aList[area::NUM_AREA] = {};		// リスト

//==============================
// コンストラクタ
//==============================
CCoin::CCoin() : CModel(CObject::TYPE_COIN, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_state = STATE_NONE;		// 状態
	m_nAreaIdx = 0;				// 区分の番号
	m_nDeathCount = 0;			// 死亡カウント
	m_fCycleSpeed = 0.0f;		// 回転速度
	m_fHeightDest = 0.0f;		// 目標の高さ
}

//==============================
// デストラクタ
//==============================
CCoin::~CCoin()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CCoin::Init(void)
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
//ブロックの終了処理
//========================================
void CCoin::Uninit(void)
{
	// 引き抜き処理
	m_aList[m_nAreaIdx].Pull(this);

	// 終了処理
	CModel::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CCoin::Update(void)
{
	switch (m_state)
	{
	case CCoin::STATE_NONE:		// 通常状態

		// 特になし

		break;

	case CCoin::STATE_GET:		// 取得状態

		// 取得処理
		Get();

		break;

	case STATE_DEATH:			// 死亡状態

		// 死亡カウントを加算する
		m_nDeathCount++;

		if (m_nDeathCount >= DEATH_COUNT)
		{ //死亡カウントが一定数以上になった場合

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 回転処理
	Cycle();
}

//=====================================
//ブロックの描画処理
//=====================================
void CCoin::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CCoin::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);						// 位置
	SetPosOld(pos);						// 前回の位置
	SetRot(NONE_D3DXVECTOR3);			// 向き
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));		// モデルの情報

	// 全ての値を初期化する
	m_state = STATE_NONE;						// 状態
	m_nDeathCount = 0;							// 死亡カウント
	m_fCycleSpeed = INIT_CYCLESPEED;			// 回転速度
	m_fHeightDest = pos.y + GET_HEIGHTDEST;		// 目標の高さ

	// 区分の番号の設定処理
	m_nAreaIdx = area::SetFieldIdx(GetPos());

	// リストに追加する
	m_aList[m_nAreaIdx].Regist(this);
}

//=======================================
// 生成処理
//=======================================
CCoin* CCoin::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CCoin* pCoin = nullptr;	// インスタンスを生成

	if (pCoin == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pCoin = new CCoin;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pCoin != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCoin->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pCoin->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 小判のポインタを返す
	return pCoin;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CCoin*> CCoin::GetList(const int nIdx)
{
	// リストマネージャーを返す
	return m_aList[nIdx];
}

//=======================================
// ヒット処理
//=======================================
void CCoin::Hit(void)
{
	// 取得状態にする
	m_state = STATE_GET;

	// 回転速度を早くする
	m_fCycleSpeed = GET_CYCLESPEED;

	// コインゲット音を鳴らす
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_COINGET);
}

//=======================================
// 状態の取得処理
//=======================================
CCoin::STATE CCoin::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 取得状態処理
//=======================================
void CCoin::Get(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	if (useful::FrameCorrect(m_fHeightDest, &pos.y, 10.0f) == true)
	{ // 目的の値に達した場合

		// 死亡状態にする
		m_state = STATE_DEATH;
	}

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 回転処理
//=======================================
void CCoin::Cycle(void)
{
	// 向きを取得する
	D3DXVECTOR3 rot = GetRot();

	// 向きを加算する
	rot.y += m_fCycleSpeed;

	// 向きの正規化
	useful::RotNormalize(&rot.y);

	// 向きを適用する
	SetRot(rot);
}