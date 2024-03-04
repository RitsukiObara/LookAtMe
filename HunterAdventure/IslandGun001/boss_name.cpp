//============================================
//
// ボスの名前表示処理[boss_name.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "boss_name.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

//--------------------------------------------
// 無名名前空間
//--------------------------------------------
namespace
{
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(-400.0f, SCREEN_HEIGHT * 0.5f, 0.0f);			// 初期位置
	const D3DXVECTOR3 NAME_SIZE = D3DXVECTOR3(100.0f, 300.0f, 0.0f);						// サイズ
	const char* TEXTURE = "data\\TEXTURE\\BossName.png";		// テクスチャ
	const D3DXVECTOR3 APPEAR_DEST_POS = D3DXVECTOR3(1100.0f, SCREEN_HEIGHT * 0.5f, 0.0f);	// 出現状態の目的の位置
	const float APPEAR_MOVE = 100.0f;		// 出現状態の移動量
	const int STOP_COUNT = 5;				// 停止状態のカウント数
	const D3DXVECTOR3 SHADOW_DEST_POS = D3DXVECTOR3(1095.0f, SCREEN_HEIGHT * 0.5f + 5.0f, 0.0f);		// 影の目的の位置
	const D3DXVECTOR3 SHADOW_MOVE = D3DXVECTOR3(0.1f, 0.1f, 0.0f);			// 影の移動量
	const int SHADOW_COUNT = 70;			// 影状態のカウント
	const float LEAVE_DEST_POS = 1500.0f;	// 退場状態の目的の位置
}

//========================
// コンストラクタ
//========================
CBossName::CBossName() : CObject(TYPE_BOSSNAME, DIM_2D, PRIORITY_UI)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject[nCnt] = nullptr;		// 名前表示の情報
	}
	m_state = STATE_APPEAR;				// 状態
	m_nStateCount = 0;					// 状態カウント
}

//========================
// デストラクタ
//========================
CBossName::~CBossName()
{

}

//========================
// 初期化処理
//========================
HRESULT CBossName::Init(void)
{
	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CBossName::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // 残弾の情報が NULL じゃない場合

			// 残弾の終了処理
			m_apObject[nCnt]->Uninit();
			m_apObject[nCnt] = nullptr;
		}
	}

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CBossName::Update(void)
{
	switch (m_state)
	{
	case CBossName::STATE_APPEAR:

		if (Appear(m_apObject[TYPE::TYPE_NAMESHADOW]) == true ||
			Appear(m_apObject[TYPE::TYPE_NAME]) == true)
		{ // 目的の位置に着いた場合

			// 出現状態遷移処理
			AppearArrival();
		}

		break;

	case CBossName::STATE_STOP:

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % STOP_COUNT == 0)
		{ // 状態カウントが一定数に達した場合

			// 状態カウントをリセットする
			m_nStateCount = 0;

			// 影状態にする
			m_state = STATE_SHADOW;
		}

		break;

	case CBossName::STATE_SHADOW:

		// 影状態処理
		Shadow();

		break;

	case CBossName::STATE_LEAVE:

		if (Leave(m_apObject[TYPE::TYPE_NAMESHADOW]) == true ||
			Leave(m_apObject[TYPE::TYPE_NAME]) == true)
		{ // 目的の位置に着いた場合

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

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // 名前表示が NULL じゃない場合

			// 頂点座標の設定処理
			m_apObject[nCnt]->SetVertex();
		}
	}
}

//========================
// 描画処理
//========================
void CBossName::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // 残弾が描画できる状態の場合

			// 描画処理
			m_apObject[nCnt]->Draw();
		}
	}
}

//========================
// 情報の設定処理
//========================
void CBossName::SetData(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] == nullptr)
		{ // 名前表示が NULL じゃない場合

			// 名前表示を生成
			m_apObject[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

			// 設定処理
			m_apObject[nCnt]->SetPos(INIT_POS);			// 位置
			m_apObject[nCnt]->SetPosOld(INIT_POS);		// 前回の位置
			m_apObject[nCnt]->SetRot(NONE_D3DXVECTOR3);	// 向き
			m_apObject[nCnt]->SetSize(NAME_SIZE);		// サイズ
			m_apObject[nCnt]->SetAngle();				// 方向
			m_apObject[nCnt]->SetLength();				// 長さ

			// テクスチャの割り当て処理
			m_apObject[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

			// 頂点座標の設定処理
			m_apObject[nCnt]->SetVertex();
		}
	}
	// 頂点カラーの設定処理
	m_apObject[TYPE_NAMESHADOW]->SetVtxColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	m_state = STATE_APPEAR;				// 状態
	m_nStateCount = 0;					// 状態カウント
}

//========================
// 生成処理
//========================
CBossName* CBossName::Create(void)
{
	// ローカルオブジェクトを生成
	CBossName* pName = nullptr;			// 名前のインスタンスを生成

	if (pName == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pName = new CBossName;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pName != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pName->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pName->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 名前のポインタを返す
	return pName;
}

//========================
// 出現状態処理
//========================
bool CBossName::Appear(CObject2D* pObject)
{
	// 位置を取得
	D3DXVECTOR3 pos = pObject->GetPos();
	bool bArrival = false;		// 到着状況

	// 移動する
	bArrival = useful::FrameCorrect(APPEAR_DEST_POS.x, &pos.x, APPEAR_MOVE);

	// 位置を適用
	pObject->SetPos(pos);

	// 到着状況を返す
	return bArrival;
}

//========================
// 出現状態遷移処理
//========================
void CBossName::AppearArrival(void)
{
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 位置を取得
		pos = m_apObject[nCnt]->GetPos();

		// 位置を設定する
		pos = APPEAR_DEST_POS;

		// 位置を適用
		m_apObject[nCnt]->SetPos(pos);
	}

	// 停止状態にする
	m_state = STATE_STOP;
}

//========================
// 影状態処理
//========================
void CBossName::Shadow(void)
{
	// 状態カウントを加算sjr￥￥する
	m_nStateCount++;

	// 位置を取得
	D3DXVECTOR3 pos = m_apObject[TYPE_NAMESHADOW]->GetPos();

	// 移動させる
	useful::FrameCorrect(SHADOW_DEST_POS.x, &pos.x, SHADOW_MOVE.x);
	useful::FrameCorrect(SHADOW_DEST_POS.y, &pos.y, SHADOW_MOVE.y);

	if(m_nStateCount % SHADOW_COUNT == 0)
	{ // 一定カウント経過した場合

		// 位置を補正
		pos = SHADOW_DEST_POS;

		// 退場状態にする
		m_state = STATE_LEAVE;
	}

	// 位置を適用
	m_apObject[TYPE_NAMESHADOW]->SetPos(pos);
}

//========================
// 退場状態処理
//========================
bool CBossName::Leave(CObject2D* pObject)
{
	// 位置を取得
	D3DXVECTOR3 pos = pObject->GetPos();
	bool bEnd = false;		// 到着状況

	// 移動する
	bEnd = useful::FrameCorrect(LEAVE_DEST_POS, &pos.x, APPEAR_MOVE);

	// 位置を適用
	pObject->SetPos(pos);

	//終了状況を返す
	return bEnd;
}