//=======================================
//
// アイテム生成器のメイン処理[item_generator.cpp]
// Author 小原立暉
//
//=======================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "item_generator.h"
#include "sound.h"
#include "useful.h"

#include "Item.h"

// マクロ定義
#define SET_STATE_COUNT			(90)			// 設定状態のカウント数
#define VIBRATE_STATE_COUNT		(30)			// 振動状態のカウント数
#define SWING_COUNT				(5)				// 振動するカウント数
#define POS_Z					(80.0f)			// 設置する位置
#define ROT_CORRECT_MAGNI		(0.5f)			// 向きの補正倍率
#define ROT_CHANGE_MAGNI		(-0.6f)			// 向きの変化時の倍率

//==========================================
// コンストラクタ
//==========================================
CItemGenerator::CItemGenerator() : CModel(CObject::TYPE_ITEMGENERATOR, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pItemAddress = nullptr;	// アイテムのアドレス
	m_state = STATE_SET;		// 状態
	m_nStateCount = 0;			// 生成カウント
	m_fRotDest = 0.0f;			// 目標の向き
}

//==========================================
// デストラクタ
//==========================================
CItemGenerator::~CItemGenerator()
{

}

//==========================================
// アイテムの初期化処理
//==========================================
HRESULT CItemGenerator::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_pItemAddress = nullptr;	// アイテムのアドレス
	m_state = STATE_SET;		// 状態
	m_nStateCount = 0;			// 生成カウント
	m_fRotDest = 0.0f;			// 目標の向き

	// 成功を返す
	return S_OK;
}

//========================================
// アイテムの終了処理
//========================================
void CItemGenerator::Uninit(void)
{
	// アイテムのアドレスを NULL にする
	m_pItemAddress = nullptr;

	// 終了処理
	CModel::Uninit();
}

//========================================
// アイテムの更新処理
//========================================
void CItemGenerator::Update(void)
{

	// リリース版のみ
#ifndef _DEBUG

	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置
	D3DXVECTOR3 rot = GetRot();		// 向き

	switch (m_state)
	{
	case STATE_SET:		// 設定状態

		// アイテムのアドレスを NULL にする
		m_pItemAddress = nullptr;

		// 生成カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % SET_STATE_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 向きを設定する
			m_fRotDest = 0.3f;

			// 通常状態にする
			m_state = STATE_VIBRATE;

			if (CManager::GetMode() == CScene::MODE_GAME &&
				CGame::GetState() == CGame::STATE_PLAY)
			{ // プレイ中の場合

				// 自販機の揺れる音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_VENDING_SWING);
			}
		}

		break;

	case STATE_VIBRATE:	// 振動状態

		// アイテムのアドレスを NULL にする
		m_pItemAddress = nullptr;

		// 生成カウントを加算する
		m_nStateCount++;

		// 向きの補正処理
		RotCorrect(m_fRotDest, &rot.z, ROT_CORRECT_MAGNI);

		if (m_nStateCount % SWING_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// 向きを変える
			m_fRotDest *= ROT_CHANGE_MAGNI;
		}

		if (m_nStateCount % VIBRATE_STATE_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// アイテムの生成処理
			m_pItemAddress = CItem::Create(D3DXVECTOR3(GetPos().x, GetPos().y, 0.0f), CItem::STATE_APPEAR);

			if (CManager::GetMode() == CScene::MODE_GAME &&
				CGame::GetState() == CGame::STATE_PLAY)
			{ // プレイ中の場合

				// アイテムの出現音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ITEM_APPEAR);
			}

			// 向きを設定する
			rot.z = 0.0f;

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 通常状態にする
			m_state = STATE_NONE;
		}

		break;

	case STATE_NONE:	// 通常状態

		// 判別処理
		Discriminate();

		break;
	}

	// 向きを更新する
	SetRot(rot);

#endif // _DEBUG
}

//===========================================
// アイテムの描画処理
//===========================================
void CItemGenerator::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//===========================================
// 情報の設定処理
//===========================================
void CItemGenerator::SetData(const D3DXVECTOR3& pos)
{
	// 全ての値を初期化する
	m_pItemAddress = nullptr;	// アイテムのアドレス
	m_state = STATE_SET;		// 状態
	m_nStateCount = 0;			// 生成カウント
	m_fRotDest = 0.0f;			// 目標の向き

	// 情報の設定処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, POS_Z));		// 位置設定
	SetPosOld(D3DXVECTOR3(pos.x, pos.y, POS_Z));	// 前回の位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// 向き設定
	SetScale(NONE_SCALE);							// 拡大率設定
	SetFileData(CXFile::TYPE_ITEMGENERATOR);		// データの設定処理
}

//===========================================
// 判別処理
//===========================================
void CItemGenerator::Discriminate(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトへのポインタ
	CObject* pObjNext = nullptr;	// 次のオブジェクトへのポインタ
	STATE state = STATE_SET;		// 状態の変数

	// オブジェクトの情報を取得する
	pObj = CObject::GetTop(PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // オブジェクトへのポインタが NULL じゃない限り回す

		// 次のオブジェクトを代入する
		pObjNext = pObj->GetNext();

		if (pObj->GetType() == TYPE_ITEM &&
			pObj == m_pItemAddress)
		{ // オブジェクトの種類がアイテムかつ、アドレスが一致した場合
			
			// 状態を設定する
			state = STATE_NONE;

			// while文を抜け出す
			break;
		}

		// 次のオブジェクトを代入する
		pObj = pObjNext;
	}

	// 状態を代入する
	m_state = state;
}

//===========================================
// 生成処理
//===========================================
CItemGenerator* CItemGenerator::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CItemGenerator* pItem = nullptr;	// プレイヤーのインスタンスを生成

	if (pItem == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pItem = new CItemGenerator;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pItem != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pItem->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "アイテム生成器の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pItem->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pItem;
}