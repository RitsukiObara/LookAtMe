//============================================
//
// ボーナス処理[bonus.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "bonus.h"
#include "objectbillboard.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define NUMBER_TEXTURE_PATTERN		(0.1f)		// 数字のテクスチャのパターン数
#define MAX_BONUS					(999)		// ボーナスの最大数

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CBonus* CBonus::m_pBonus = nullptr;						// スコアの情報

//========================
// コンストラクタ
//========================
CBonus::CBonus() : CObject(TYPE_BONUSCOMBO, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < NUM_BONUS_DIGIT; nCnt++)
	{
		m_aNumber[nCnt] = nullptr;		// 数字の情報
	}
	m_posShift = NONE_D3DXVECTOR3;		// 位置の設定処理
	m_nBonusCombo = 0;					// ボーナス数
}

//========================
// デストラクタ
//========================
CBonus::~CBonus()
{

}

//========================
// 初期化処理
//========================
HRESULT CBonus::Init(void)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < NUM_BONUS_DIGIT; nCnt++)
	{
		if (m_aNumber[nCnt] == nullptr)
		{ // 情報が NULL じゃない場合

			// メモリを確保する
			m_aNumber[nCnt] = new CBillboardAnim(TYPE_NONE, PRIORITY_UI);
		}
		else
		{ // 上記以外

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}

		if (m_aNumber[nCnt] != nullptr)
		{ // 数字の情報が NULL じゃない場合

			if (FAILED(m_aNumber[nCnt]->Init()))
			{ // 初期化に失敗した場合

				// 停止
				assert(false);

				// 失敗を返す
				return E_FAIL;
			}
		}
		else
		{ // 上記以外

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 全ての値を初期化する
	m_posShift = NONE_D3DXVECTOR3;		// 位置の設定処理
	m_nBonusCombo = 0;					// ボーナス数

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CBonus::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_BONUS_DIGIT; nCnt++)
	{
		if (m_aNumber[nCnt] != nullptr)
		{ // 数字の情報がある場合

			// 終了処理
			m_aNumber[nCnt]->Uninit();
			m_aNumber[nCnt] = nullptr;
		}
	}

	// 本体の終了処理
	Release();

	// ボーナスを NULL にする
	m_pBonus = nullptr;
}

//========================
// 更新処理
//========================
void CBonus::Update(void)
{
	// ずらす幅の設定処理
	SetShift();

	// ローカル変数宣言
	int aNum[NUM_BONUS_DIGIT];		// 数値

	// 10進数への計算処理
	useful::DecimalCalculation(NUM_BONUS_DIGIT, m_nBonusCombo, &aNum[0]);

	for (int nCnt = 0; nCnt < NUM_BONUS_DIGIT; nCnt++)
	{
		//値の計算
		m_aNumber[nCnt]->SetVtxAnim(NUMBER_TEXTURE_PATTERN, aNum[nCnt]);
	}
}

//========================
// 描画処理
//========================
void CBonus::Draw(void)
{
	for (int nCnt = 0; nCnt < NUM_BONUS_DIGIT; nCnt++)
	{
		if (m_aNumber[nCnt] != nullptr)
		{ // 数字の情報が NULL じゃない場合

			// 描画処理
			m_aNumber[nCnt]->DrawShift(D3DXVECTOR3(m_posShift.x * nCnt, m_posShift.y * nCnt, m_posShift.z * nCnt));
		}
	}
}

//========================
// 情報の設定処理
//========================
void CBonus::SetData(const D3DXVECTOR3& pos)
{
	for (int nCnt = 0; nCnt < NUM_BONUS_DIGIT; nCnt++)
	{
		if (m_aNumber[nCnt] != nullptr)
		{ // 数字の情報が NULL の場合

			// 情報を設定する
			m_aNumber[nCnt]->SetPos(pos);		// 位置
			m_aNumber[nCnt]->SetPosOld(pos);	// 前回の位置
			m_aNumber[nCnt]->SetSize(D3DXVECTOR3(30.0f, 45.0f, 0.0f));		// サイズ
			m_aNumber[nCnt]->SetAnim(0, 10);	// テクスチャのパターン

			// 頂点情報を設定
			m_aNumber[nCnt]->SetVertex();

			// テクスチャ座標の設定処理
			m_aNumber[nCnt]->SetVtxAnim(NUMBER_TEXTURE_PATTERN, 0);

			// テクスチャの割り当て処理
			m_aNumber[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist("data\\TEXTURE\\Number.png"));
		}
	}

	// 全ての値を初期化する
	m_posShift = NONE_D3DXVECTOR3;		// 位置の設定処理
	m_nBonusCombo = 0;					// ボーナス数
}

//========================
// ボーナスの取得処理
//========================
int CBonus::GetBonus(void) const
{
	// ボーナス数を返す
	return m_nBonusCombo;
}

//========================
// ボーナスの加算処理
//========================
void CBonus::AddBonus(void)
{
	// ボーナス数を加算する
	m_nBonusCombo++;

	if (m_nBonusCombo > MAX_BONUS)
	{ // ボーナスの最大数を超過した場合

		// ボーナス数を補正する
		m_nBonusCombo = MAX_BONUS;
	}
}

//========================
// スコアの取得処理
//========================
CBonus* CBonus::Get(void)
{
	if (m_pBonus != nullptr)
	{ // ボーナスの情報がある場合

		// ボーナスのポインタを返す
		return m_pBonus;
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// ボーナスのポインタを返す
		return m_pBonus;
	}
}

//========================
// 生成処理
//========================
CBonus* CBonus::Create(const D3DXVECTOR3& pos)
{
	if (m_pBonus == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		m_pBonus = new CBonus;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (m_pBonus != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(m_pBonus->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		m_pBonus->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return m_pBonus;
}

//========================
// ずらす幅の設定処理
//========================
void CBonus::SetShift(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posR = CManager::Get()->Get()->GetCamera()->GetPosR();		// 注視点
	D3DXVECTOR3 posV = CManager::Get()->Get()->GetCamera()->GetPosV();		// 視点
	float fRot;

	// 向きを設定する
	fRot = atan2f(posR.x - posV.x, posR.z - posV.z);

	// 向きを加算する
	fRot += D3DX_PI * 0.5f;

	// 向きを正規化する
	useful::RotNormalize(&fRot);

	// ずらす幅を設定する
	m_posShift.x = sinf(fRot) * 60.0f;		// ずらす幅
	m_posShift.y = 0.0f;					// ずらす幅
	m_posShift.z = cosf(fRot) * 60.0f;		// ずらす幅
}