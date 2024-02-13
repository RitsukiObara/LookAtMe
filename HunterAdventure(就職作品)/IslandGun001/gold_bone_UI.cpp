//============================================
//
// 金の骨UI処理[gold_bone_UI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "gold_bone_UI.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

namespace
{
	const char* TEXTURE = "data\\TEXTURE\\BoneMark.png";					// テクスチャ
	const D3DXVECTOR3 MARK_SIZE = D3DXVECTOR3(30.0f, 20.0f, 0.0f);			// マークのサイズ
	const D3DXVECTOR3 INIT_MARK_POS = D3DXVECTOR3(40.0f, 80.0f, 0.0f);		// マークの初期位置
	const D3DXVECTOR3 EXTEND_MARK_SIZE = D3DXVECTOR3(45.0f, 30.0f, 0.0f);	// マークの拡大時のサイズ
	const float MARK_SHIFT = 70.0f;											// マークのずらす幅
	const D3DXCOLOR SHADOW_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);			// 影の色
	const D3DXVECTOR3 STATE_CORRECT = D3DXVECTOR3(1.5f, 1.0f, 0.0f);		// 状態による補正の倍率
}

//========================
// コンストラクタ
//========================
CGoldBoneUI::CGoldBoneUI() : CObject(TYPE_GOLDBONEUI, DIM_2D, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		m_aBoneMark[nCnt].pShadow = nullptr;		// 影の情報
		m_aBoneMark[nCnt].pMark = nullptr;			// 影のマーク
		m_aBoneMark[nCnt].state = STATE_NONE;		// 状態
	}
}

//========================
// デストラクタ
//========================
CGoldBoneUI::~CGoldBoneUI()
{

}

//========================
// 初期化処理
//========================
HRESULT CGoldBoneUI::Init(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pShadow == nullptr)
		{ // 影の情報が NULL じゃない場合

			// 影の情報
			m_aBoneMark[nCnt].pShadow = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		}

		m_aBoneMark[nCnt].pMark = nullptr;			// マークの情報
		m_aBoneMark[nCnt].state = STATE_NONE;		// 状態
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CGoldBoneUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pShadow != nullptr)
		{ // 影の情報が NULL じゃない場合

			// 影の情報の終了
			m_aBoneMark[nCnt].pShadow->Uninit();
			m_aBoneMark[nCnt].pShadow = nullptr;
		}

		if (m_aBoneMark[nCnt].pMark != nullptr)
		{ // マークの情報が NULL じゃない場合

			// マークの情報の終了
			m_aBoneMark[nCnt].pMark->Uninit();
			m_aBoneMark[nCnt].pMark = nullptr;
		}
	}

	// 本体の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CGoldBoneUI::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		switch (m_aBoneMark[nCnt].state)
		{
		case STATE_NONE:		// 無状態

			break;

		case STATE_EXTEND:		// 拡大状態

			if (m_aBoneMark[nCnt].pMark != nullptr)
			{ // マークの情報が NULL じゃない場合

				// サイズを取得する
				D3DXVECTOR3 size = m_aBoneMark[nCnt].pMark->GetSize();

				// 均等な補正処理
				if (useful::FrameCorrect(EXTEND_MARK_SIZE.x, &size.x, STATE_CORRECT.x) == true ||
					useful::FrameCorrect(EXTEND_MARK_SIZE.y, &size.y, STATE_CORRECT.y) == true)
				{ // 一定のサイズになった場合

					// 縮小状態にする
					m_aBoneMark[nCnt].state = STATE_SHRINK;
				}

				// サイズを適用する
				m_aBoneMark[nCnt].pMark->SetSize(size);
			}

			break;

		case STATE_SHRINK:		// 縮小状態

			if (m_aBoneMark[nCnt].pMark != nullptr)
			{ // マークの情報が NULL じゃない場合

				// サイズを取得する
				D3DXVECTOR3 size = m_aBoneMark[nCnt].pMark->GetSize();

				// 均等な補正処理
				if (useful::FrameCorrect(MARK_SIZE.x, &size.x, STATE_CORRECT.x) == true ||
					useful::FrameCorrect(MARK_SIZE.y, &size.y, STATE_CORRECT.y) == true)
				{ // 一定のサイズになった場合

					// 停止状態にする
					m_aBoneMark[nCnt].state = STATE_STOP;
				}

				// サイズを適用する
				m_aBoneMark[nCnt].pMark->SetSize(size);
			}

			break;

		case STATE_STOP:		// 停止状態

			break;

		default:

			// 停止
			assert(false);

			break;
		}

		if (m_aBoneMark[nCnt].pMark != nullptr)
		{ // マークが NULL じゃない場合

			// 頂点座標の設定処理
			m_aBoneMark[nCnt].pMark->SetVertex();
		}
	}
}

//========================
// 描画処理
//========================
void CGoldBoneUI::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pShadow != nullptr)
		{ // 影の情報が NULL じゃない場合

			// 影の情報の終了
			m_aBoneMark[nCnt].pShadow->Draw();
		}
	}

	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pMark != nullptr)
		{ // マークの情報が NULL じゃない場合

			// マークの情報の終了
			m_aBoneMark[nCnt].pMark->Draw();
		}
	}
}

//========================
// 情報の設定処理
//========================
void CGoldBoneUI::SetData(void)
{
	// マークの位置を宣言する
	D3DXVECTOR3 posMark = NONE_D3DXVECTOR3;

	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pShadow != nullptr)
		{ // 影の情報が NULL じゃない場合

			// マークの位置を設定する
			posMark.x = INIT_MARK_POS.x + (MARK_SHIFT * nCnt);
			posMark.y = INIT_MARK_POS.y;
			posMark.z = 0.0f;

			// 設定処理
			m_aBoneMark[nCnt].pShadow->SetPos(posMark);				// 位置
			m_aBoneMark[nCnt].pShadow->SetPosOld(posMark);			// 前回の位置
			m_aBoneMark[nCnt].pShadow->SetRot(NONE_D3DXVECTOR3);	// 向き
			m_aBoneMark[nCnt].pShadow->SetSize(MARK_SIZE);			// サイズ
			m_aBoneMark[nCnt].pShadow->SetAngle();					// 方向
			m_aBoneMark[nCnt].pShadow->SetLength();					// 長さ

			// テクスチャの割り当て処理
			m_aBoneMark[nCnt].pShadow->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

			// 頂点座標の設定処理
			m_aBoneMark[nCnt].pShadow->SetVertex();

			// 頂点カラーの設定処理
			m_aBoneMark[nCnt].pShadow->SetVtxColor(SHADOW_COL);
		}
	}
}

//========================
// 生成処理
//========================
CGoldBoneUI* CGoldBoneUI::Create(void)
{
	// ローカルオブジェクトを生成
	CGoldBoneUI* pUI = nullptr;	// UIのインスタンスを生成

	if (pUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pUI = new CGoldBoneUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pUI != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pUI->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pUI->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// UIのポインタを返す
	return pUI;
}

//========================
// 金の骨のゲット処理
//========================
void CGoldBoneUI::GetGoldBone(void)
{
	// マークの位置を宣言する
	D3DXVECTOR3 posMark = NONE_D3DXVECTOR3;

	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pMark == nullptr &&
			m_aBoneMark[nCnt].state == STATE_NONE)
		{ // マークがまだ出現していない番号の場合

			// マークの位置を設定する
			posMark.x = INIT_MARK_POS.x + (MARK_SHIFT * nCnt);
			posMark.y = INIT_MARK_POS.y;
			posMark.z = 0.0f;

			// マークの情報
			m_aBoneMark[nCnt].pMark = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

			// 設定処理
			m_aBoneMark[nCnt].pMark->SetPos(posMark);				// 位置
			m_aBoneMark[nCnt].pMark->SetPosOld(posMark);			// 前回の位置
			m_aBoneMark[nCnt].pMark->SetRot(NONE_D3DXVECTOR3);		// 向き
			m_aBoneMark[nCnt].pMark->SetSize(NONE_D3DXVECTOR3);		// サイズ
			m_aBoneMark[nCnt].pMark->SetAngle();					// 方向
			m_aBoneMark[nCnt].pMark->SetLength();					// 長さ

			// テクスチャの割り当て処理
			m_aBoneMark[nCnt].pMark->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

			// 頂点座標の設定処理
			m_aBoneMark[nCnt].pMark->SetVertex();

			// 拡大状態を設定する
			m_aBoneMark[nCnt].state = STATE_EXTEND;

			// 抜け出す
			break;
		}
	}
}