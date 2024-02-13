//===========================================
//
// 全エディットのメイン処理[editAll.cpp]
// Author 小原立暉
//
//===========================================
#ifdef _DEBUG	// デバッグ処理

//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"

#include "editAll.h"
#include "edit.h"
#include "bgEdit.h"
#include "obstacleEdit.h"
#include "cameraEdit.h"
#include "range_move.h"
#include "player.h"
#include "objectX.h"
#include "Item.h"
#include "item_generator.h"

// マクロ定義
#define OBJECT_DELETE_RANGE		(70.0f)									// オブジェクトの消去範囲
#define CAMERAEDIT_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		// カメラエディットの生成時の範囲

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CEdit* CEditAll::m_pEdit = nullptr;						// エディットのポインタ
CBgEdit* CEditAll::m_pBgEdit = nullptr;					// 背景エディットのポインタ
CObstacleEdit* CEditAll::m_pObstacleEdit = nullptr;		// 障害物エディットのポインタ
CCameraEdit* CEditAll::m_pCameraEdit = nullptr;			// カメラエディットのポインタ
CRangeMove* CEditAll::m_pRangeMove = nullptr;			// 範囲移動のポインタ

//==========================
// コンストラクタ
//==========================
CEditAll::CEditAll()
{
	// 全ての値をクリアする
	m_pEdit = nullptr;			// ステージエディットのポインタ
	m_pBgEdit = nullptr;		// 背景エディットのポインタ
	m_pObstacleEdit = nullptr;	// 障害物エディットのポインタ
	m_pCameraEdit = nullptr;	// カメラエディットのポインタ
	m_pRangeMove = nullptr;		// 範囲移動のポインタ

	// 全ての値をクリアする
	m_style = STYLE_BLOCK;		// スタイル
	m_saveStyle = STYLE_BLOCK;	// 保存用スタイル

	if (m_pEdit == nullptr)
	{ // ポインタが NULL の場合

		// メモリを確保する
		m_pEdit = new CEdit;
	}
	else
	{ // ポインタが NULL じゃない場合

		// 警告文
		MessageBox(NULL, "エディットのポインタが使われています！", "警告！", MB_ICONWARNING);
	}
}

//==========================
// デストラクタ
//==========================
CEditAll::~CEditAll()
{

}

//==========================
// 初期化処理
//==========================
HRESULT CEditAll::Init(void)
{
	// 全ての値をクリアする
	m_style = STYLE_BLOCK;		// スタイル
	m_saveStyle = STYLE_BLOCK;	// 保存用スタイル

	if (m_pEdit != nullptr)
	{ // ポインタが NULL じゃない場合

		// 初期化処理
		if (FAILED(m_pEdit->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "エディットの初期化に失敗！", "警告！", MB_ICONWARNING);
		}

		// 情報の設定処理
		m_pEdit->SetData(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	else
	{ // ポインタが NULL の場合

		// 警告文
		MessageBox(NULL, "エディットのポインタがNULL！", "警告！", MB_ICONWARNING);
	}

	// 成功を返す
	return S_OK;
}

//==========================
// 終了処理
//==========================
void CEditAll::Uninit(void)
{
	// 全消去処理
	AllDelete();
}

//==========================
// 更新処理
//==========================
void CEditAll::Update(void)
{
	switch (m_style)
	{
	case STYLE_BLOCK:		// ブロック(ステージ)

		// 更新処理
		m_pEdit->Update();

		break;

	case STYLE_BGOBJECT:	// 背景オブジェクト

		// 更新処理
		m_pBgEdit->Update();

		break;

	case STYLE_OBSTACLE:	// 障害物

		// 更新処理
		m_pObstacleEdit->Update();

		break;

	case STYLE_CAMERA:		// カメラの範囲

		// 更新処理
		m_pCameraEdit->Update();

		break;

	case STYLE_RANGEMOVE:	// 範囲移動

		// 更新処理
		m_pRangeMove->Update();

		break;
	}

	if (m_style != STYLE_RANGEMOVE)
	{ // 範囲移動の場合

		// スタイル変更処理
		StyleChange();
	}

	// アイテムの設定処理
	SetItem();

	// アイテムの消去処理
	DeleteItem();

	// アイテム生成器の設定処理
	SetItemGenerator();

	// アイテム生成器の消去処理
	DeleteItemGenerator();

	// 範囲移動の設定処理
	RangeMoveChange();

	// デバッグ表示
	CManager::GetDebugProc()->Print("F2キー：アイテムの設置\nF3キー：アイテムの消去\nF4キー：アイテム生成器の設置\n"
		"F5キー：アイテム生成器の消去\nBACKSPACEキー：範囲移動の切り替え");
}

//==========================
// 描画処理
//==========================
void CEditAll::Draw(void)
{
	switch (m_style)
	{
	case STYLE_BLOCK:		// ブロック(ステージ)

		// 描画処理
		m_pEdit->Draw();

		break;

	case STYLE_BGOBJECT:	// 背景オブジェクト

		// 描画処理
		m_pBgEdit->Draw();

		break;

	case STYLE_OBSTACLE:	// 障害物

		// 描画処理
		m_pObstacleEdit->Draw();

		break;

	case STYLE_CAMERA:		// カメラ

		// 描画処理
		m_pCameraEdit->Draw();

		break;

	case STYLE_RANGEMOVE:	// 範囲移動

		// 描画処理
		m_pRangeMove->Draw();

		break;
	}
}

//==========================
// スタイルの設定処理
//==========================
void CEditAll::StyleChange(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 保存用の位置の変数

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
	{ // 6キーを押した場合

		// 位置の保存処理
		posSave = GetEditPos();

		// 全消去処理
		AllDelete();

		// スタイルを設定する
		m_style = (STYLE)((m_style + 1) % STYLE_RANGEMOVE);

		// 設定処理
		Set(posSave);
	}
}

//==========================
// 範囲移動の設定処理
//==========================
void CEditAll::RangeMoveChange(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true)
	{ // BACKSPACEキーを押した場合

		// 位置の保存処理
		pos = GetEditPos();

		if (m_style == STYLE_RANGEMOVE)
		{ // 範囲移動だった場合

			// 全消去処理
			AllDelete();

			// スタイルを設定する
			m_style = m_saveStyle;

			// 設定処理
			Set(pos);
		}
		else
		{ // 上記以外

			// 保存用スタイルを設定する
			m_saveStyle = m_style;

			// スタイルを設定する
			m_style = STYLE_RANGEMOVE;

			// 消去処理
			AllDelete();

			// 範囲を生成
			m_pRangeMove = m_pRangeMove->Create(pos);
		}
	}
}

//==========================
// アイテムの設定処理
//==========================
void CEditAll::SetItem(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true)
	{ // F2キーを押した場合

		// アイテムを生成する
		CItem::Create(GetEditPos(), CItem::STATE_NONE);
	}
}

//==========================
// アイテムの消去処理
//==========================
void CEditAll::DeleteItem(void)
{
	// ローカルポインタ宣言
	CObject* pObj = nullptr;	// オブジェクトのポインタ
	CItem* pItem = nullptr;		// アイテムのポインタ
	D3DXVECTOR3 posSave = GetEditPos();		// 保存用の位置の変数

	// オブジェクトを取得する
	pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_ITEM)
		{ // オブジェクトがアイテムだった場合

			// アイテムの情報を取得する
			pItem = dynamic_cast<CItem*>(pObj);

			if (pItem != nullptr)
			{ // アイテムの情報が NULL じゃない場合

				if (posSave.x >= pItem->GetPos().x - OBJECT_DELETE_RANGE &&
					posSave.x <= pItem->GetPos().x + OBJECT_DELETE_RANGE &&
					posSave.y >= pItem->GetPos().y - OBJECT_DELETE_RANGE &&
					posSave.y <= pItem->GetPos().y + OBJECT_DELETE_RANGE)
				{ // 範囲内に入った場合

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true)
					{ // F3キーを押した場合

						// 削除する
						pItem->Uninit();
					}
				}
			}
		}

		// 次のオブジェクトを設定する
		pObj = pObj->GetNext();
	}
}

//==========================
// アイテム生成器の設定処理
//==========================
void CEditAll::SetItemGenerator(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4) == true)
	{ // F4キーを押した場合

		// アイテムを生成する
		CItemGenerator::Create(GetEditPos());
	}
}

//==========================
// アイテム生成器の消去処理
//==========================
void CEditAll::DeleteItemGenerator(void)
{
	// ローカルポインタ宣言
	CObject* pObj = nullptr;	// オブジェクトのポインタ
	CItemGenerator* pItem = nullptr;		// アイテムのポインタ
	D3DXVECTOR3 posSave = GetEditPos();		// 保存用の位置の変数

	// オブジェクトを取得する
	pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_ITEMGENERATOR)
		{ // オブジェクトがアイテム生成器だった場合

			// アイテム生成器の情報を取得する
			pItem = dynamic_cast<CItemGenerator*>(pObj);

			if (pItem != nullptr)
			{ // アイテム生成器の情報が NULL じゃない場合

				if (posSave.x >= pItem->GetPos().x - OBJECT_DELETE_RANGE &&
					posSave.x <= pItem->GetPos().x + OBJECT_DELETE_RANGE &&
					posSave.y >= pItem->GetPos().y - OBJECT_DELETE_RANGE &&
					posSave.y <= pItem->GetPos().y + OBJECT_DELETE_RANGE)
				{ // 範囲内に入った場合

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_F5) == true)
					{ // F5キーを押した場合

						// 削除する
						pItem->Uninit();
					}
				}
			}
		}

		// 次のオブジェクトを設定する
		pObj = pObj->GetNext();
	}
}

//==========================
// ポインタ全消去処理
//==========================
void CEditAll::AllDelete(void)
{
	if (m_pEdit != nullptr)
	{ // ポインタが NULL じゃない場合

		// 終了処理
		m_pEdit->Uninit();

		// ポインタを NULL にする
		m_pEdit = nullptr;
	}

	if (m_pBgEdit != nullptr)
	{ // ポインタが NULL じゃない場合

		// 終了処理
		m_pBgEdit->Uninit();

		// ポインタを NULL にする
		m_pBgEdit = nullptr;
	}

	if (m_pObstacleEdit != nullptr)
	{ // ポインタが NULL じゃない場合

		// 終了処理
		m_pObstacleEdit->Uninit();

		// ポインタを NULL にする
		m_pObstacleEdit = nullptr;
	}

	if (m_pCameraEdit != nullptr)
	{ // ポインタが NULL じゃない場合

		// 終了処理
		m_pCameraEdit->Uninit();

		// ポインタを NULL にする
		m_pCameraEdit = nullptr;
	}

	if (m_pRangeMove != nullptr)
	{ // ポインタが NULL じゃない場合

		// 終了処理
		m_pRangeMove->Uninit();

		// ポインタを NULL にする
		m_pRangeMove = nullptr;
	}
}

//==========================
// 設定処理
//==========================
void CEditAll::Set(const D3DXVECTOR3& pos)
{
	switch (m_style)
	{
	case STYLE_BLOCK:			// ブロック(ステージ)

		if (m_pEdit == nullptr)
		{ // ポインタが NULL の場合

			// 生成処理
			m_pEdit = m_pEdit->Create
			(
				D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				NONE_SCALE
			);
		}

		break;

	case STYLE_BGOBJECT:		// 背景オブジェクト

		if (m_pBgEdit == nullptr)
		{ // ポインタが NULL の場合

			// 生成処理
			m_pBgEdit = m_pBgEdit->Create
			(
				pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				NONE_SCALE,
				CXFile::GetXFile(CXFile::TYPE_ENERGYITEM)
			);
		}

		break;

	case STYLE_OBSTACLE:

		if (m_pObstacleEdit == nullptr)
		{ // ポインタが NULL の場合

			// 生成処理
			m_pObstacleEdit = m_pObstacleEdit->Create(D3DXVECTOR3(pos.x, pos.y, 0.0f));
		}

		break;

	case STYLE_CAMERA:

		if (m_pCameraEdit == nullptr)
		{ // ポインタが NULL の場合

			// 生成処理
			m_pCameraEdit = m_pCameraEdit->Create
			(
				pos,
				CAMERAEDIT_SIZE,
				CCamera::TYPE_UPWARD
			);
		}

		break;
	}
}

//==========================
// 位置の取得処理
//==========================
D3DXVECTOR3 CEditAll::GetEditPos(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 保存用の位置の変数

	switch (m_style)
	{
	case STYLE_BLOCK:			// ブロック(ステージ)

		if (m_pEdit != nullptr)
		{ // ポインタが NULL じゃない場合

			// 位置を保存しておく
			posSave = m_pEdit->GetPos();
		}

		break;

	case STYLE_BGOBJECT:		// 背景オブジェクト

		if (m_pBgEdit != nullptr)
		{ // ポインタが NULL じゃない場合

			// 位置を保存しておく
			posSave = m_pBgEdit->GetPos();
		}

		break;

	case STYLE_OBSTACLE:		// 障害物

		if (m_pObstacleEdit != nullptr)
		{ // ポインタが NULL じゃない場合

			// 位置を保存しておく
			posSave = m_pObstacleEdit->GetPos();
		}

		break;

	case STYLE_CAMERA:			// カメラの範囲

		if (m_pCameraEdit != nullptr)
		{ // ポインタが NULL じゃない場合

			// 位置を保存しておく
			posSave = m_pCameraEdit->GetPos();
		}

		break;

	case STYLE_RANGEMOVE:		// 範囲移動

		if (m_pRangeMove != nullptr)
		{ // ポインタが NULL じゃない場合

			// 位置を保存しておく
			posSave = m_pRangeMove->GetPos();
		}

		break;
	}

	// 位置を設定する
	return posSave;
}

//==========================
// スタイルの取得処理
//==========================
CEditAll::STYLE CEditAll::GetStyle(void) const
{
	// スタイルを返す
	return m_style;
}

//==========================
// 生成処理
//==========================
CEditAll* CEditAll::Create(void)
{
	// ローカルオブジェクトを生成
	CEditAll* pEditAll = nullptr;	// プレイヤーのインスタンスを生成

	if (pEditAll == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pEditAll = new CEditAll;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pEditAll != nullptr)
	{ // オブジェクトが NULL じゃない場合

	  // 初期化処理
		if (FAILED(pEditAll->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "全エディットの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

	  // NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pEditAll;
}

#endif