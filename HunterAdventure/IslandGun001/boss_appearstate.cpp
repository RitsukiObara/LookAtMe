//================================================================================================================
//
// ボスの状態処理 [boss_appearstate.cpp]
// Author：小原立暉
//
//================================================================================================================
//****************************************************************************************************************
//	インクルードファイル
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_appearstate.h"
#include "motion.h"

#include "ripple.h"

#include "boss_turnstate.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const float LENGTH = 8000.0f;		// 距離
	const float HEIGHT = 2000.0f;		// 初期高度
	const float SLOPE = -0.9f;			// 傾き
	const float ADD_ROT = 0.05f;		// 向きの追加量
	const float SUB_LENGTH = 20.0f;		// 距離の減算量
	const float SUB_SLOPE = 0.00225f;	// 傾きの減算量
	const int RIPPLE_FREQ = 6;			// 波紋の出る頻度
	const float RIPPLE_HEIGHT = 600.0f;	// 波紋の高度
	const D3DXVECTOR3 RIPPLE_SCALE = D3DXVECTOR3(100.0f, 100.0f, 100.0f);	// 波紋の拡大率
}

//==========================
// コンストラクタ
//==========================
CBossAppearState::CBossAppearState()
{
	// 全ての値をクリアする
	m_fLangeRot = 0.0f;		// 離れる向き
	m_fLength = LENGTH;		// 距離
}

//==========================
// デストラクタ
//==========================
CBossAppearState::~CBossAppearState()
{

}

//==========================
// 状態の設定処理
//==========================
void CBossAppearState::Process(CBoss* pBoss)
{
	// カウントを加算する
	m_nCount++;

	// 距離関係処理
	Length();

	if (m_fLength <= 0.0f)
	{ // 長さが 0.0f 以下になった場合

		// 位置と向きを取得する
		D3DXVECTOR3 pos;

		// 位置を設定する
		pos.x = 0.0f;
		pos.y = HEIGHT;
		pos.z = 0.0f;

		// 位置を適用する
		pBoss->SetPos(pos);

		// 向き直り状態にする
		pBoss->ChangeState(new CBossTurnState());

		// この先の処理を行わない
		return;
	}

	// 位置向き設定処理
	PosRot(pBoss);

	// 波紋の設置処理
	Ripple(pBoss);
}

//==========================
// 情報の設定処理
//==========================
void CBossAppearState::SetData(CBoss* pBoss)
{
	// 全ての値を設定する
	m_fLangeRot = 0.0f;		// 離れる向き
	m_fLength = LENGTH;		// 距離

	// 位置と向きを取得する
	D3DXVECTOR3 pos = pBoss->GetPos();
	D3DXVECTOR3 rot = pBoss->GetRot();

	// 位置を設定する
	pos.x = sinf(m_fLangeRot) * m_fLength;
	pos.y = HEIGHT;
	pos.z = cosf(m_fLangeRot) * m_fLength;

	// 向きを設定する
	rot.y = m_fLangeRot + (D3DX_PI * 0.5f);
	rot.z = SLOPE;

	// 位置と向きを適用する
	pBoss->SetPos(pos);
	pBoss->SetRot(rot);
}

//==========================
// 距離関係処理
//==========================
void CBossAppearState::Length(void)
{
	// 向きを加算する
	m_fLangeRot += ADD_ROT;

	// 向きの正規化
	useful::RotNormalize(&m_fLangeRot);

	// 距離を減算する
	m_fLength -= SUB_LENGTH;
}

//==========================
// 位置向き設定処理
//==========================
void CBossAppearState::PosRot(CBoss* pBoss)
{
	// 位置と向きを取得する
	D3DXVECTOR3 pos = pBoss->GetPos();
	D3DXVECTOR3 rot = pBoss->GetRot();

	// 位置を設定する
	pos.x = sinf(m_fLangeRot) * m_fLength;
	pos.y = HEIGHT;
	pos.z = cosf(m_fLangeRot) * m_fLength;

	// 向きを設定する
	rot.y = m_fLangeRot + (D3DX_PI * 0.5f);
	rot.z += SUB_SLOPE;

	// 向きの正規化
	useful::RotNormalize(&rot.y);

	// 位置と向きを適用する
	pBoss->SetPos(pos);
	pBoss->SetRot(rot);
}

//==========================
// 波紋の設置処理
//==========================
void CBossAppearState::Ripple(CBoss* pBoss)
{
	if (m_nCount % RIPPLE_FREQ == 0)
	{ // 一定カウントごとに

		// 位置と向きを取得する
		D3DXVECTOR3 pos = pBoss->GetPos();
		D3DXVECTOR3 rot = pBoss->GetRot();

		// 波紋の生成処理
		CRipple::Create
		(
			D3DXVECTOR3(pos.x, pos.y + RIPPLE_HEIGHT, pos.z),
			D3DXVECTOR3(D3DX_PI * 0.5f, rot.y, D3DX_PI),
			RIPPLE_SCALE
		);
	}
}