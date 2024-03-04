//===========================================
//
// 金の骨のメイン処理[gold_bone.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "gold_bone.h"
#include "renderer.h"
#include "useful.h"

#include "game.h"
#include "alter.h"
#include "alter_pole.h"
#include "blur_org.h"
#include "gold_bone_light.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\GoldBone.x";		// モデルの名前
	const float ROT_MOVE = 0.01f;			// モデルの向きの移動量
	const float GET_HEIGHT = 300.0f;		// 取得時の高さ
	const float ADD_HEIGHT = 30.0f;			// 高さの加算数
	const float ADD_ROT_MOVE = 0.008f;		// 向きの移動量の加算数

	const float UP_ROT_MOVE = 0.8f;			// 上昇状態に遷移する時の向きの移動量
	const float UP_DEST_HEIGHT = 5000.0f;	// 上昇状態の目標の高さ
	const float UP_MOVE_Y = 100.0f;			// 上昇状態の移動量
	const float UP_BLUR_ALPHA = 0.6f;		// 上昇状態の残像の透明度
	const int UP_BLUR_LIFE = 7;				// 上昇状態の残像の寿命

	const float ALTER_SET_MOVE_Y = 30.0f;	// 祭壇設定状態の移動量
	const float ALTER_SET_ROT_MOVE = 0.0f;	// 祭壇設定状態の向きの移動量
	const float ALTER_ROT_CORRECT = 0.02f;	// 祭壇設定状態の向きの補正数
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CGoldBone*> CGoldBone::m_list = {};		// リスト

//==============================
// コンストラクタ
//==============================
CGoldBone::CGoldBone() : CModel(CObject::TYPE_GOLDBONE, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pLight = nullptr;			// ライトの情報
	m_state = STATE_NONE;		// 状態
	m_fDestHeight = 0.0f;		// 目的の高さ
	m_fRotMove = ROT_MOVE;		// 向きの移動量

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CGoldBone::~CGoldBone()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CGoldBone::Init(void)
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
void CGoldBone::Uninit(void)
{
	if (m_pLight != nullptr)
	{ // ライトが NULL じゃない場合

		// 終了処理
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	// 終了処理
	CModel::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
//ブロックの更新処理
//========================================
void CGoldBone::Update(void)
{
	switch (m_state)
	{
	case CGoldBone::STATE_NONE:

		break;

	case CGoldBone::STATE_GET:

		// 取得処理
		Get();

		break;

	case CGoldBone::STATE_UP:

		// 上昇処理
		Up();

		break;

	case CGoldBone::STATE_ALTERSET:

		// 祭壇設定処理
		AlterSet();

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 回転処理
	Cycle();

	if (m_pLight != nullptr)
	{ // ライトが NULL じゃない場合

		// 描画処理
		m_pLight->Update();

		if (m_pLight->GetCircum() <= 0.0f)
		{ // 円周が0以下の場合

			// 終了処理
			m_pLight->Uninit();
			m_pLight = nullptr;
		}
	}
}

//=====================================
//ブロックの描画処理
//=====================================
void CGoldBone::Draw(void)
{
	// 描画処理
	CModel::Draw();

	if (m_pLight != nullptr)
	{ // ライトが NULL じゃない場合

		// 描画処理
		m_pLight->Draw();
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CGoldBone::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);							// 位置
	SetPosOld(pos);							// 前回の位置
	SetRot(NONE_D3DXVECTOR3);				// 向き
	SetScale(NONE_SCALE);					// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));		// モデルの情報

	// 全ての値を設定する
	m_state = STATE_NONE;		// 状態
	m_fDestHeight = 0.0f;		// 目的の高さ
	m_fRotMove = ROT_MOVE;		// 向きの移動量
}

//=======================================
// 生成処理
//=======================================
CGoldBone* CGoldBone::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CGoldBone* pBone = nullptr;	// インスタンスを生成

	if (pBone == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pBone = new CGoldBone;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBone != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBone->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBone->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

// 金の骨のポインタを返す
return pBone;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CGoldBone*> CGoldBone::GetList(void)
{
	// リストマネージャーを返す
	return m_list;
}

//=======================================
// ヒット処理
//=======================================
void CGoldBone::Hit(void)
{
	// 取得状態にする
	m_state = STATE_GET;

	// 目的の高さを設定する
	m_fDestHeight = GetPos().y + GET_HEIGHT;
}

//=======================================
// 状態の設定処理
//=======================================
void CGoldBone::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CGoldBone::STATE CGoldBone::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 光の設定処理
//=======================================
void CGoldBone::SetLight(void)
{
	if (m_pLight == nullptr)
	{ // 光が NULL の場合

		// 光を生成
		m_pLight = CGoldBoneLight::Create(GetPos());
	}
}

//=======================================
// 光の取得処理
//=======================================
CGoldBoneLight* CGoldBone::GetLight(void)
{
	// 光の情報を返す
	return m_pLight;
}

//=======================================
// 回転処理
//=======================================
void CGoldBone::Cycle(void)
{
	// 向きを取得する
	D3DXVECTOR3 rot = GetRot();

	// 回す
	rot.y += m_fRotMove;

	// 向きの正規化
	useful::RotNormalize(&rot.y);

	// 向きを適用する
	SetRot(rot);
}

//=======================================
// 取得処理
//=======================================
void CGoldBone::Get(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 位置を一定の場所まで上げる
	useful::FrameCorrect(m_fDestHeight, &pos.y, ADD_HEIGHT);

	// 位置を適用する
	SetPos(pos);

	// 向きの移動量を加算する
	m_fRotMove += ADD_ROT_MOVE;

	if (m_fRotMove >= UP_ROT_MOVE)
	{ // 向きの移動量が一定数以上になった場合

		// 上昇状態にする
		m_state = STATE_UP;

		// 目標の高さを設定する
		m_fDestHeight = UP_DEST_HEIGHT;
	}
}

//=======================================
// 上昇処理
//=======================================
void CGoldBone::Up(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 位置を一定の場所まで上げる
	if (useful::FrameCorrect(m_fDestHeight, &pos.y, UP_MOVE_Y) == true)
	{
		// 祭壇の情報を取得
		CAlter* pAlter = CGame::GetAlter();
		CAlterPole* pPole = nullptr;

		if (pAlter != nullptr)
		{ // 祭壇が NULL じゃない場合

			for (int nCnt = 0; nCnt < CAlter::NUM_POLE; nCnt++)
			{
				// 石柱の情報を取得
				pPole = pAlter->GetPole(nCnt);

				if (pPole != nullptr &&
					pPole->IsEmpty() == true)
				{ // 石柱に金の骨が無い場合

					// 位置を設定する
					pos.x = pPole->GetPos().x;
					pos.y = pPole->GetPos().y + m_fDestHeight;
					pos.z = pPole->GetPos().z;

					// 目的の高さを設定する
					m_fDestHeight = pPole->GetPos().y + pPole->GetFileData().vtxMax.y;

					// 石柱のインデックスを設定する
					m_nPoleIdx = nCnt;

					// 金の骨を供える
					pPole->SetEnableEmpty(false);
					pPole->SetGoldBone(this);

					//祭壇設定状態にする
					m_state = STATE_ALTERSET;

					// for文を抜ける
					break;
				}
			}
		}
	}

	// 位置を適用する
	SetPos(pos);

	// 向きの移動量を加算する
	m_fRotMove += ADD_ROT_MOVE;

	// 残像の生成処理
	CBlurOrg::Create(pos, GetRot(), GetScale(), UP_BLUR_ALPHA, GetFileData(), UP_BLUR_LIFE, PRIORITY_EFFECT);
}

//=======================================
// 祭壇設定処理
//=======================================
void CGoldBone::AlterSet(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	if (useful::FrameCorrect(m_fDestHeight, &pos.y, ALTER_SET_MOVE_Y) == true)
	{ // 石柱に到着したとき

		// 祭壇の情報を取得
		CAlter* pAlter = CGame::GetAlter();
		CAlterPole* pPole = nullptr;

		if (pAlter != nullptr)
		{ // 祭壇が NULL じゃない場合

			// 石柱の情報を取得
			pPole = pAlter->GetPole(m_nPoleIdx);

			if (pPole != nullptr)
			{ // ポールが NULL じゃない場合

				// 到着状況を true にする
				pPole->SetEnableArrival(true);
			}
		}
	}

	// 位置を適用する
	SetPos(pos);

	// 向きの移動量を0にしていく
	useful::RotCorrect(ALTER_SET_ROT_MOVE, &m_fRotMove, ALTER_ROT_CORRECT);
}