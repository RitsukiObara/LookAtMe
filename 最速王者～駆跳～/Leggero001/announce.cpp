//============================================
//
// 残り距離の告知処理[announce.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "announce.h"
#include "object2D.h"
#include "animobject.h"
#include "manager.h"
#include "file.h"
#include "renderer.h"
#include "texture.h"
#include "goal.h"
#include "player.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define ROAD_SIZE			(D3DXVECTOR3(1000.0f, 10.0f, 0.0f))		// 道のりのサイズ
#define GOAL_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		// ゴールのサイズ
#define PLAYER_SIZE			(D3DXVECTOR3(15.0f, 30.0f, 0.0f))		// プレイヤーのサイズ

#define ROAD_POS			(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - (ROAD_SIZE.x * 0.5f), 700.0f, 0.0f))		// 道のりの位置
#define ROAD_TEX			(100.0f)								// 道のりのテクスチャ座標
#define PLAYER_ANIM_COUNT	(10)									// プレイヤーのアニメーションのカウント
#define PLAYER_ANIM_PATTERN	(4)										// プレイヤーのアニメーションのパターン数

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CAnnounce::c_apTextureName[TEX_MAX] =				// テクスチャの名前
{
	"data/TEXTURE/Road.png",			// 道のり
	"data/TEXTURE/GoalMark.png",		// ゴールマーク
	"data/TEXTURE/PlayerMark.png",		// プレイヤーマーク
};

//========================
// コンストラクタ
//========================
CAnnounce::CAnnounce() : CObject(TYPE_ANNOUNCE, PRIORITY_UI)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		// 数字の情報
		m_apObject2D[nCnt] = nullptr;
	}

	m_pPlayer = nullptr;	// プレイヤー
	m_fTotalDist = 0.0f;	// 総合の距離
	m_nDist = 0;			// 距離
}

//========================
// デストラクタ
//========================
CAnnounce::~CAnnounce()
{

}

//========================
// 初期化処理
//========================
HRESULT CAnnounce::Init(void)
{
	// 全ての値を初期化する
	m_fTotalDist = 0.0f;	// 総合の距離
	m_nDist = 0;			// 距離

	// メモリを確保する
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] == nullptr)
		{ // ポインタが NULL の場合

			// オブジェクトを生成する
			m_apObject2D[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, PRIORITY_UI);

			if (m_apObject2D[nCnt] != nullptr)
			{ // ポリゴンのポインタが NULL じゃない場合

				// テクスチャの割り当て処理
				m_apObject2D[nCnt]->BindTexture(CManager::GetTexture()->Regist(c_apTextureName[nCnt]));
			}
			else
			{ // 上記以外

				// 警告文
				MessageBox(NULL, "告知のポインタがNULLです！", "警告！", MB_ICONWARNING);

				// 失敗を返す
				return E_FAIL;
			}
		}
		else
		{ // ポインタが NULL じゃない場合

			// 警告文
			MessageBox(NULL, "告知のメモリが既に使われている！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}
	}

	if (m_pPlayer == nullptr)
	{ // ポインタが NULL の場合

		// プレイヤーを生成する
		m_pPlayer = CAnim::Create(CObject::TYPE_NONE, PRIORITY_UI);

		if (m_pPlayer != nullptr)
		{ // ポインタが NULL じゃない場合

			// テクスチャの割り当て処理
			m_pPlayer->BindTexture(CManager::GetTexture()->Regist(c_apTextureName[TEX_PLAYER]));
		}
		else
		{ // 上記以外

			// 警告文
			MessageBox(NULL, "告知のポインタがNULLです！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CAnnounce::Uninit(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // ポリゴンのポインタが NULL じゃない場合

			// 終了処理
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = nullptr;
		}
	}

	if (m_pPlayer != nullptr)
	{ // プレイヤーが使用されている場合

		// 終了処理
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CAnnounce::Update(void)
{
	// 距離測定処理
	DistanceMeasure();

	// プレイヤーの移動処理
	PlayerMove();

	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (nCnt == POLY_ROAD)
		{ // 道のりの場合

			// 頂点座標の設定処理
			m_apObject2D[nCnt]->SetVtxWidthGage();
		}
		else
		{ // 上記以外

			// 頂点座標の設定処理
			m_apObject2D[nCnt]->SetVertex();
		}
	}
}

//========================
// 描画処理
//========================
void CAnnounce::Draw(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		// 描画処理
		m_apObject2D[nCnt]->Draw();
	}

	// 描画処理
	m_pPlayer->Draw();
}

//========================
// 情報の設定処理
//========================
void CAnnounce::SetData(void)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // オブジェクトの情報が NULL の場合

			// 情報の設定処理
			switch (nCnt)
			{
			case POLY_ROAD:		// 道のり

				m_apObject2D[nCnt]->SetPos(ROAD_POS);		// 位置設定
				m_apObject2D[nCnt]->SetSize(ROAD_SIZE);		// サイズ設定

				// 頂点情報の設定処理
				m_apObject2D[nCnt]->SetVtxWidthGage();

				// 横長のテクスチャ座標を設定する
				m_apObject2D[nCnt]->SetVtxTextureWidth(ROAD_TEX);

				break;

			case POLY_GOAL:		// ゴール

				m_apObject2D[nCnt]->SetPos(D3DXVECTOR3(m_apObject2D[POLY_ROAD]->GetPos().x + m_apObject2D[POLY_ROAD]->GetSize().x, m_apObject2D[POLY_ROAD]->GetPos().y - m_apObject2D[POLY_ROAD]->GetSize().y - GOAL_SIZE.y, 0.0f));		// 位置設定
				m_apObject2D[nCnt]->SetSize(GOAL_SIZE);		// サイズ設定

				// 頂点情報の設定処理
				m_apObject2D[nCnt]->SetVertex();

				break;
			}

			// 設定処理
			m_apObject2D[nCnt]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き設定
			m_apObject2D[nCnt]->SetLength();			// 長さ設定
			m_apObject2D[nCnt]->SetAngle();				// 方向設定
		}
	}

	// 情報の設定処理
	m_pPlayer->SetPos(D3DXVECTOR3(m_apObject2D[POLY_ROAD]->GetPos().x, m_apObject2D[POLY_ROAD]->GetPos().y - m_apObject2D[POLY_ROAD]->GetSize().y - PLAYER_SIZE.y, 0.0f));		// 位置設定
	m_pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き設定
	m_pPlayer->SetSize(PLAYER_SIZE);		// サイズ設定
	m_pPlayer->SetLength();					// 長さ設定
	m_pPlayer->SetAngle();					// 方向設定

	// アニメーションの設定処理
	m_pPlayer->SetAnim(PLAYER_ANIM_COUNT, PLAYER_ANIM_PATTERN);

	// 頂点情報の設定処理
	m_pPlayer->SetVertex();

	// テクスチャ座標の設定処理
	m_pPlayer->SetVtxTextureAnim(m_pPlayer->GetTexPattern(), m_pPlayer->GetPattern());

	// 距離測定処理
	DistanceMeasure();

	if (CManager::GetFile() != nullptr)
	{ // ファイルが NULL じゃない場合

		// 総合の距離を求める
		m_fTotalDist = CManager::GetFile()->GetGameInfo().GoalPos.x - CManager::GetFile()->GetGameInfo().StartPos.x;
	}
}

//========================
// 距離測定処理
//========================
void CAnnounce::DistanceMeasure(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトのポインタ
	CGoal* pGoal = nullptr;			// ゴールのポインタ

	// 先頭のポインタを取得する
	pObj = CObject::GetTop(PRIORITY_PLAYER);

	while (pObj != nullptr)
	{ // ポインタが NULL じゃない限り回す

		if (pObj->GetType() == TYPE_GOAL)
		{ // 種類がゴールの場合

			// ゴールの情報を取得する
			pGoal = dynamic_cast<CGoal*>(pObj);

			if (pGoal != nullptr)
			{ // ゴールが NULL の場合

				// 抜け出す
				break;
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}

	if (pGoal != nullptr &&
		CScene::GetPlayer() != nullptr)
	{ // ゴールのポインタが NULL じゃない場合

		// 距離を設定する
		m_nDist = (int)((pGoal->GetPos().x - pGoal->GetSize().x) - CScene::GetPlayer()->GetPos().x);
	}
	else
	{ // 上記以外

		// 距離を設定する
		m_nDist = 0;
	}
}

//========================
// プレイヤーの移動処理
//========================
void CAnnounce::PlayerMove(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = m_pPlayer->GetPos();		// 位置
	float fSize;								// 総合の距離の割合を取る

	// 総合の距離の割合を測る
	fSize = ROAD_SIZE.x / m_fTotalDist;

	// 位置を現在の位置に設定する
	pos.x = (m_apObject2D[POLY_ROAD]->GetPos().x + m_apObject2D[POLY_ROAD]->GetSize().x) - fSize * m_nDist;

	// プレイヤーの位置を更新する
	m_pPlayer->SetPos(pos);

	// 頂点情報を更新する
	m_pPlayer->SetVertex();

	// アニメーションの更新処理
	m_pPlayer->Update();
}

//========================
// 生成処理
//========================
CAnnounce* CAnnounce::Create(void)
{
	// ローカルオブジェクトを生成
	CAnnounce* pAnnou = nullptr;	// プレイヤーのインスタンスを生成

	if (pAnnou == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pAnnou = new CAnnounce;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pAnnou != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pAnnou->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "告知の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pAnnou->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pAnnou;
}