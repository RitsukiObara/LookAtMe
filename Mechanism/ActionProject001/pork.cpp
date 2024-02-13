//===========================================
//
// ポークのメイン処理[pork.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "pork.h"
#include "pork_manager.h"
#include "useful.h"

#include "signboard.h"
#include "player.h"
#include "player_ability.h"
#include "tutorial.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define DISTINCT_SIZE		(800.0f)		// 判別するときのサイズ
#define SIGNBOARD_ADD		(0.5f)			// 看板の追加の向きの移動量
#define PORK_POS_Z			(100.0f)		// ポークの位置(Z軸)

//==============================
// コンストラクタ
//==============================
CPork::CPork() : CModel(CObject::TYPE_PORK, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pSignBoard = nullptr;	// 看板のポインタ
	m_type = TYPE_HOVER;	// 種類
	m_bTutorial = false;	// チュートリアル状況

	m_pPrev = nullptr;		// 前へのポインタ
	m_pNext = nullptr;		// 次へのポインタ

	if (CPorkManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CPorkManager::Get()->Regist(this);
	}
}

//==============================
// デストラクタ
//==============================
CPork::~CPork()
{

}

//============================
// 前のポインタの設定処理
//============================
void CPork::SetPrev(CPork* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CPork::SetNext(CPork* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CPork* CPork::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CPork* CPork::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CPork::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_pSignBoard = nullptr;		// 看板のポインタ
	m_type = TYPE_HOVER;		// 種類
	m_bTutorial = false;		// チュートリアル状況

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CPork::Uninit(void)
{
	if (m_pSignBoard != nullptr)
	{ // 看板が存在する場合

		// 終了処理
		m_pSignBoard->Uninit();
		m_pSignBoard = nullptr;
	}

	// 終了処理
	CModel::Uninit();

	if (CPorkManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CPorkManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//========================================
//ブロックの更新処理
//========================================
void CPork::Update(void)
{
	if (m_bTutorial == true)
	{ // チュートリアル状況が true の場合

		// 看板の回転処理
		SignBoardCycle();
	}
	else
	{ // 上記以外

		// プレイヤーの判別処理
		DistinctPlayer();
	}
}

//=====================================
//ブロックの描画処理
//=====================================
void CPork::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CPork::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 情報の設定処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z + PORK_POS_Z));								// 位置
	SetRot(NONE_D3DXVECTOR3);			// 向き
	SetPosOld(GetPos());				// 前回の位置
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CXFile::TYPE_PORK);		// モデルの情報

	// 全ての値を設定する
	m_type = type;				// 種類
	m_bTutorial = false;		// チュートリアル状況

	if (m_pSignBoard == nullptr)
	{ // 看板のポインタが NULL の場合

		// 看板の生成処理
		m_pSignBoard = CSignBoard::Create(GetPos());
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}

	// チュートリアルを生成する
	CTutorial::Create(GetPos(), type);
}

//=======================================
// 生成処理
//=======================================
CPork* CPork::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ローカルオブジェクトを生成
	CPork* pPork = nullptr;	// インスタンスを生成

	if (pPork == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pPork = new CPork;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pPork != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPork->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPork->SetData(pos, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ポークのポインタを返す
	return pPork;
}

//=======================================
// 看板の回転処理
//=======================================
void CPork::SignBoardCycle(void)
{
	if (m_pSignBoard != nullptr)
	{ // 看板が NULL じゃない場合

		// ローカル変数宣言
		D3DXVECTOR3 rot = m_pSignBoard->GetRot();		// 看板の向きを取得する

		// 向きを目標に近づけていく
		useful::FrameCorrect(D3DX_PI, &rot.y, SIGNBOARD_ADD);
		
		// 看板の向きを適用する
		m_pSignBoard->SetRot(rot);
	}
}

//=======================================
// プレイヤーの判別処理
//=======================================
void CPork::DistinctPlayer(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = CPlayer::Get();		// プレイヤー

	if (pPlayer != nullptr)
	{ // プレイヤーの情報があった場合

		if (pPlayer->GetPos().x <= GetPos().x + DISTINCT_SIZE &&
			pPlayer->GetPos().x >= GetPos().x - DISTINCT_SIZE)
		{ // 範囲内だった場合

			switch (m_type)
			{
			case CPork::TYPE_HOVER:		// ホバージェット

				if (pPlayer->GetAbility()->GetAbility() == CAbility::ABILITY_HOVER)
				{ // ホバージェット状態の場合

					// チュートリアル状況を true にする
					m_bTutorial = true;
				}

				break;

			case CPork::TYPE_DASH:		// ジェットダッシュ

				if (pPlayer->GetAbility()->GetAbility() == CAbility::ABILITY_JETDASH)
				{ // ジェットダッシュ状態の場合

					// チュートリアル状況を true にする
					m_bTutorial = true;
				}

				break;

			case CPork::TYPE_QUAKE:		// グラウンドクエイク

				if (pPlayer->GetAbility()->GetAbility() == CAbility::ABILITY_GROUNDQUAKE)
				{ // グラウンドクエイク状態の場合

					// チュートリアル状況を true にする
					m_bTutorial = true;
				}

				break;

			default:

				// 停止
				assert(false);

				break;
			}
		}
	}
}