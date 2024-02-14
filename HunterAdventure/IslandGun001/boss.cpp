//============================================================
//
// ボス処理 [boss.cpp]
// Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "model.h"
#include "boss.h"
#include "manager.h"
#include "useful.h"

#include "motion.h"
#include "boss_appearstate.h"
#include "objectElevation.h"

//------------------------------------------------------------
// 無名名前空間
//------------------------------------------------------------
namespace
{
	const float LAND_GRAVITY = -50.0f;		// 着地時の重力
}

// 静的メンバ変数
CListManager<CBoss*> CBoss::m_list = {};				// リスト

//================================
// コンストラクタ
//================================
CBoss::CBoss() : CCharacter(CObject::TYPE_BOSS, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pMotion = nullptr;			// モーション
	m_pState = nullptr;				// 状態の情報

	// リストに追加する
	m_list.Regist(this);
}

//================================
// デストラクタ
//================================
CBoss::~CBoss()
{

}

//================================
// 初期化処理
//================================
HRESULT CBoss::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// モデルの総数を設定
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_BOSS));

	// データの設定処理
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // モーションが NULL だった場合

		if (m_pMotion == nullptr)
		{ // モーションが NULL の場合

			// モーションを生成する
			m_pMotion = CMotion::Create();
		}
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pMotion != nullptr)
	{ // ポインタが NULL じゃない場合

		// モーションの情報を取得する
		m_pMotion->SetInfo(CMotion::STYLE_BOSS, GetHierarchy(), GetNumModel());
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	// 値を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CBoss::Uninit(void)
{
	if (m_pMotion != nullptr)
	{ // モーションが NULL じゃない場合

		// モーションの終了処理
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pState != nullptr)
	{ // 状態が NULL じゃない場合

		// 状態の終了処理
		m_pState->Uninit();
		m_pState = nullptr;
	}

	// 終了処理
	CCharacter::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//================================
// 更新処理
//================================
void CBoss::Update(void)
{
	if (m_pState != nullptr)
	{ // 状態が NULL じゃない場合

		// 状態処理
		m_pState->Process(this);
	}

	// モーションの更新処理
	m_pMotion->Update();
}

//================================
// 描画処理
//================================
void CBoss::Draw(void)
{
	// 描画処理
	CCharacter::Draw();
}

//================================
// ヒット処理
//================================
void CBoss::Hit(void)
{
	// 停止
	assert(false);
}

//================================
// 情報の設定処理
//================================
void CBoss::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 全ての値を初期化する
	SetPos(pos);				// 位置
	SetPosOld(GetPos());		// 前回の位置
	SetRot(rot);				// 向き
	SetScale(NONE_SCALE);		// 拡大率

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// 初期化処理
		GetHierarchy(nCntData)->SetPos(pos);										// 位置
		GetHierarchy(nCntData)->SetPosOld(pos);										// 前回の位置
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);								// 拡大率
	}

	// モーションのリセット処理
	m_pMotion->ResetMotion(MOTIONTYPE_FLY);

	// 状態の切り替え処理
	ChangeState(new CBossAppearState);
}

//===========================================
// 生成処理
//===========================================
CBoss* CBoss::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	if (m_list.IsEmpty() == false)
	{ // 空白状態の場合

		// 生成しない(応急処置)
		return nullptr;
	}

	// ボスのインスタンス
	CBoss* pBoss = nullptr;

	if (pBoss == nullptr)
	{ // オブジェクトが NULL の場合

		// ボスを生成する
		pBoss = new CBoss;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pBoss;
	}

	if (pBoss != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBoss->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBoss->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// ボスのポインタを返す
	return pBoss;
}

//===========================================
// リストの取得処理
//===========================================
CListManager<CBoss*> CBoss::GetList(void)
{
	// リストの情報を返す
	return m_list;
}

//===========================================
// 起伏地面との当たり判定
//===========================================
bool CBoss::ElevationCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;	// 先頭の小判
	CElevation* pElevEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pElev = list.GetTop();

		// 末尾の値を取得する
		pElevEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 当たり判定を取る
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // 当たり判定の位置が高かった場合

				// 高さを設定する
				pos.y = fHeight;

				// 位置を更新する
				SetPos(pos);

				// true を返す
				return true;
			}

			if (pElev == pElevEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pElev = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// 位置を更新する
	SetPos(pos);

	// false を返す
	return false;
}

//===========================================
// 状態の遷移処理
//===========================================
void CBoss::ChangeState(CBossState* pNext)
{
	if (m_pState != nullptr)
	{ // 状態が NULL じゃない場合

		// 状態の終了処理
		m_pState->Uninit();
		m_pState = nullptr;
	}

	// 次の状態を設定する
	m_pState = pNext;

	if (m_pState != nullptr)
	{ // 状態が NULL じゃない場合

		// 情報の設定処理
		m_pState->SetData(this);
	}
}

//===========================================
// モーションの取得処理
//===========================================
CMotion* CBoss::GetMotion(void)
{
	// モーションの情報を返す
	return m_pMotion;
}