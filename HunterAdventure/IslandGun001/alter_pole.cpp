//===========================================
//
// 石柱のメイン処理[alter_pole.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "alter_pole.h"
#include "texture.h"
#include "useful.h"

#include "gold_bone.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\AlterPole.x";		// モデルの名前
	const float BONE_ROT_MOVE = 0.1f;					// 金の骨の向きの移動量
	const float SUB_DISTANCE = 1.5f;					// 金の骨の距離の減算量
}

//==============================
// コンストラクタ
//==============================
CAlterPole::CAlterPole() : CModel(TYPE_NONE, PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pGoldBone = nullptr;		// 金の骨の情報
	m_fBoneRot = 0.0f;			// 金の骨への向き
	m_bEmpty = true;			// 空白状況
	m_bArrival = false;			// 到着状況
}

//==============================
// デストラクタ
//==============================
CAlterPole::~CAlterPole()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CAlterPole::Init(void)
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
void CAlterPole::Uninit(void)
{
	// 金の骨を NULL にする
	m_pGoldBone = nullptr;

	// 終了処理
	CModel::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CAlterPole::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CAlterPole::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CAlterPole::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetScale(NONE_SCALE);			// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデルの情報

	// 全ての値を設定する
	m_pGoldBone = nullptr;			// 金の骨の情報
	m_fBoneRot = 0.0f;				// 金の骨への向き
	m_bEmpty = true;				// 空白状況
	m_bArrival = false;				// 到着状況
}

//=======================================
// 生成処理
//=======================================
CAlterPole* CAlterPole::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CAlterPole* pPole = nullptr;		// インスタンスを生成

	if (pPole == nullptr)
	{ // オブジェクトが NULL の場合

		// ブロックを生成する
		pPole = new CAlterPole;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pPole != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPole->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPole->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 石柱のポインタを返す
	return pPole;
}

//=======================================
// 金の骨の設定処理
//=======================================
void CAlterPole::SetGoldBone(CGoldBone* pBone)
{
	// 金の骨を取得する
	m_pGoldBone = pBone;
}

//=======================================
// 金の骨の取得処理
//=======================================
CGoldBone* CAlterPole::GetGoldBone(void) const
{
	// 金の骨を返す
	return m_pGoldBone;
}

//=======================================
// 金の骨への向きの設定処理
//=======================================
void CAlterPole::SetBoneRot(const float fRot)
{
	// 金の骨への向きを設定する
	m_fBoneRot = fRot;
}

//=======================================
// 金の骨への向きの取得処理
//=======================================
float CAlterPole::GetBoneRot(void) const
{
	// 金の骨への向きを返す
	return m_fBoneRot;
}

//=======================================
// 金の骨への距離の設定処理
//=======================================
void CAlterPole::SetBoneDist(const float fDist)
{
	// 金の骨への距離を設定する
	m_fBoneDist = fDist;
}

//=======================================
// 金の骨への距離の取得処理
//=======================================
float CAlterPole::GetBoneDist(void) const
{
	// 金の骨への距離を返す
	return m_fBoneDist;
}

//=======================================
// 空白状況の設定処理
//=======================================
void CAlterPole::SetEnableEmpty(const bool bEmpty)
{
	// 空白状況を設定する
	m_bEmpty = bEmpty;
}

//=======================================
// 空白状況の取得処理
//=======================================
bool CAlterPole::IsEmpty(void) const
{
	// 空白状況を返す
	return m_bEmpty;
}

//=======================================
// 到着状況の設定処理
//=======================================
void CAlterPole::SetEnableArrival(const bool bArv)
{
	// 到着状況を設定する
	m_bArrival = bArv;
}

//=======================================
// 到着状況の取得処理
//=======================================
bool CAlterPole::IsArrival(void) const
{
	// 到着状況を返す
	return m_bArrival;
}

//=======================================
// チャージ処理
//=======================================
void CAlterPole::Charge(void)
{
	// 金の骨への向きを加算する
	m_fBoneRot += BONE_ROT_MOVE;

	// 向きの正規化
	useful::RotNormalize(&m_fBoneRot);

	// 距離を縮める
	useful::FrameCorrect(0.0f, &m_fBoneDist, SUB_DISTANCE);
}

//=======================================
// 金の骨の位置設定処理
//=======================================
void CAlterPole::GoldBonePosSet(const D3DXVECTOR3& pos)
{
	// 金の骨の位置を取得
	D3DXVECTOR3 posBone = m_pGoldBone->GetPos();

	// 位置を設定する
	posBone.x = pos.x + sinf(m_fBoneRot) * m_fBoneDist;
	posBone.z = pos.z + cosf(m_fBoneRot) * m_fBoneDist;

	// 位置を適用
	m_pGoldBone->SetPos(posBone);
}