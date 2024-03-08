//============================================
//
// コンティニューUI処理[continueUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "continueUI.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "useful.h"

#include "object2D.h"
#include "game.h"
#include "player.h"
#include "camera.h"
#include "game_score.h"

//--------------------------------------------
// 無名名前空間
//--------------------------------------------
namespace
{
	const char* TEXTURE[CContinueUI::TYPE_MAX] =			// テクスチャ
	{
		nullptr,
		"data\\TEXTURE\\ContGround.png",
		"data\\TEXTURE\\ContQuery.png",
		"data\\TEXTURE\\ContSubScore.png",
		"data\\TEXTURE\\ContYes.png",
		"data\\TEXTURE\\ContNo.png",
	};
	const D3DXVECTOR3 POS[CContinueUI::TYPE_MAX] =			// 位置
	{
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f,200.0f,0.0f),
		D3DXVECTOR3(750.0f,330.0f,0.0f),
		D3DXVECTOR3(390.0f,480.0f,0.0f),
		D3DXVECTOR3(890.0f,480.0f,0.0f),
	};
	const D3DXVECTOR3 SELECT_SIZE = D3DXVECTOR3(200.0f, 100.0f, 0.0f);	// 選択中の選択肢のサイズ
	const D3DXVECTOR3 NONE_SIZE = D3DXVECTOR3(160.0f, 80.0f, 0.0f);		// 選択していない選択肢のサイズ

	const D3DXVECTOR3 POLY_SIZE[CContinueUI::TYPE_MAX] =	// サイズ
	{
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f),
		D3DXVECTOR3(550.0f,330.0f,0.0f),
		D3DXVECTOR3(500.0f,80.0f,0.0f),
		D3DXVECTOR3(80.0f,40.0f,0.0f),
		NONE_SIZE,
		NONE_SIZE,
	};
	const D3DXCOLOR SCREEN_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f);		// 背景の色
	const int CONTINUE_LIFE = 100;			// コンティニューしたときの体力
	const int INIT_SUB_SCORE = 100;			// スコアの減算量の初期値
	const int MAX_SUB_SCORE = 9999;			// スコアの減算量の最大数

	const D3DXVECTOR3 NUMBER_POS = D3DXVECTOR3(460.0f, 330.0f, 0.0f);		// 数字の位置
	const D3DXVECTOR3 NUMBER_SIZE = D3DXVECTOR3(30.0f, 40.0f, 0.0f);		// 数字のサイズ
	const float NUMBER_SHIFT = 60.0f;		// 数字のずらす幅
	const char* NUMBER_TEXTURE = "data\\TEXTURE\\Number.png";				// 数字のテクスチャ
}

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
int CContinueUI::m_nSubScore = INIT_SUB_SCORE;		// 得点の減算量

//========================
// コンストラクタ
//========================
CContinueUI::CContinueUI() : CObject(TYPE_CONTINUEUI, DIM_2D, PRIORITY_UI)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject[nCnt] = nullptr;			// ポリゴンの情報
	}
	for (int nCntCost = 0; nCntCost < NUM_COSTDIGIT; nCntCost++)
	{
		m_apNumber[nCntCost] = nullptr;		// 番号
	}
	m_select = SELECT_YES;			// 選択肢
}

//========================
// デストラクタ
//========================
CContinueUI::~CContinueUI()
{

}

//========================
// 初期化処理
//========================
HRESULT CContinueUI::Init(void)
{
	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CContinueUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // ポリゴンの情報が NULL じゃない場合

			// ポリゴンの終了処理
			m_apObject[nCnt]->Uninit();
			m_apObject[nCnt] = nullptr;
		}
	}

	for (int nCntCost = 0; nCntCost < NUM_COSTDIGIT; nCntCost++)
	{
		if (m_apNumber[nCntCost] != nullptr)
		{ // 数字の情報が NULL じゃない場合

			// 終了処理
			m_apNumber[nCntCost]->Uninit();
			m_apNumber[nCntCost] = nullptr;
		}
		m_apNumber[nCntCost] = nullptr;		// 番号
	}

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CContinueUI::Update(void)
{
	// 選択処理
	Select();

	if (Decide() == true)
	{ // 決定した場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}
}

//========================
// 描画処理
//========================
void CContinueUI::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // ポリゴンが NULL じゃない場合

			// 描画処理
			m_apObject[nCnt]->Draw();
		}
	}

	for (int nCntCost = 0; nCntCost < NUM_COSTDIGIT; nCntCost++)
	{
		if (m_apNumber[nCntCost] != nullptr)
		{ // 数字が NULL じゃない場合

			// 描画処理
			m_apNumber[nCntCost]->Draw();
		}
	}
}

//========================
// 情報の設定処理
//========================
void CContinueUI::SetData(void)
{
	int aNum[NUM_COSTDIGIT];

	// 数値の桁ごとの算出
	useful::DecimalCalculation(NUM_COSTDIGIT, m_nSubScore, &aNum[0]);

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] == nullptr)
		{ // ポリゴンが NULL じゃない場合

			// ポリゴンを生成
			m_apObject[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

			// 設定処理
			m_apObject[nCnt]->SetPos(POS[nCnt]);			// 位置
			m_apObject[nCnt]->SetPosOld(POS[nCnt]);			// 前回の位置
			m_apObject[nCnt]->SetRot(NONE_D3DXVECTOR3);		// 向き
			m_apObject[nCnt]->SetSize(POLY_SIZE[nCnt]);		// サイズ
			m_apObject[nCnt]->SetAngle();					// 方向
			m_apObject[nCnt]->SetLength();					// 長さ

			// テクスチャの割り当て処理
			m_apObject[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[nCnt]));

			// 頂点座標の設定処理
			m_apObject[nCnt]->SetVertexRot();

			if (nCnt == TYPE::TYPE_SCREEN)
			{ // 背景の場合

				// 色を設定する
				m_apObject[nCnt]->SetVtxColor(SCREEN_COL);
			}
		}
	}
	m_select = SELECT_YES;			// 選択肢

	for (int nCntCost = 0; nCntCost < NUM_COSTDIGIT; nCntCost++)
	{
		if (m_apNumber[nCntCost] == nullptr)
		{ // 数字が NULL の場合
			
			// 数値を生成する
			m_apNumber[nCntCost] = CNumber::Create();

			// 情報の設定処理
			m_apNumber[nCntCost]->SetPos(D3DXVECTOR3(NUMBER_POS.x + (nCntCost * NUMBER_SHIFT), NUMBER_POS.y, 0.0f));		// 位置設定
			m_apNumber[nCntCost]->SetRot(NONE_D3DXVECTOR3);		// 向き設定
			m_apNumber[nCntCost]->SetSize(NUMBER_SIZE);			// サイズ設定
			m_apNumber[nCntCost]->SetLength();					// 長さ設定
			m_apNumber[nCntCost]->SetAngle();					// 方向設定

			m_apNumber[nCntCost]->SetType(CNumber::TYPE_DECIMAL);	// 番号の種類
			m_apNumber[nCntCost]->SetNumber(aNum[nCntCost]);		// 番号を設定する

			// 頂点情報の設定処理
			m_apNumber[nCntCost]->SetVertex();

			// テクスチャの割り当て処理
			m_apNumber[nCntCost]->BindTexture(CManager::Get()->GetTexture()->Regist(NUMBER_TEXTURE));

			// テクスチャの設定処理(アニメーションバージョン)
			m_apNumber[nCntCost]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, aNum[nCntCost]);
		}
	}
}

//========================
// 生成処理
//========================
CContinueUI* CContinueUI::Create(void)
{
	// ローカルオブジェクトを生成
	CContinueUI* pUI = nullptr;			// UIのインスタンスを生成

	if (pUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pUI = new CContinueUI;
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
// 得点の減算量のリセット処理
//========================
void CContinueUI::SubScoreReset(void)
{
	// 得点の減算量をリセットする
	m_nSubScore = INIT_SUB_SCORE;
}

//========================
// 得点の減算量の取得処理
//========================
int CContinueUI::GetSubScore(void)
{
	// 得点の減算量を返す
	return m_nSubScore;
}

//========================
// 選択処理
//========================
void CContinueUI::Select(void)
{
	if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_LEFT, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_RIGHT, 0) == true ||
		CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_A) == true ||
		CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_D) == true)
	{ // 右か左を押した場合

		// 選択肢を変える
		m_select = (SELECT)((m_select + 1) % SELECT_MAX);
	}

	if (m_select == SELECT_YES)
	{ // YESを選択していた場合

		// 選択肢のサイズを設定
		m_apObject[TYPE_YES]->SetSize(SELECT_SIZE);
		m_apObject[TYPE_YES]->SetLength();
		m_apObject[TYPE_YES]->SetAngle();
		m_apObject[TYPE_YES]->SetVertexRot();
		m_apObject[TYPE_YES]->SetVtxColor(NONE_D3DXCOLOR);

		m_apObject[TYPE_NO]->SetSize(NONE_SIZE);
		m_apObject[TYPE_NO]->SetLength();
		m_apObject[TYPE_NO]->SetAngle();
		m_apObject[TYPE_NO]->SetVertexRot();
		m_apObject[TYPE_NO]->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));
	}
	else
	{ // 上記以外

		// 選択肢のサイズを設定
		m_apObject[TYPE_YES]->SetSize(NONE_SIZE);
		m_apObject[TYPE_YES]->SetLength();
		m_apObject[TYPE_YES]->SetAngle();
		m_apObject[TYPE_YES]->SetVertexRot();
		m_apObject[TYPE_YES]->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));

		m_apObject[TYPE_NO]->SetSize(SELECT_SIZE);
		m_apObject[TYPE_NO]->SetLength();
		m_apObject[TYPE_NO]->SetAngle();
		m_apObject[TYPE_NO]->SetVertexRot();
		m_apObject[TYPE_NO]->SetVtxColor(NONE_D3DXCOLOR);
	}
}

//========================
// 決定処理
//========================
bool CContinueUI::Decide(void)
{
	if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
	{ // 決定キーを押した場合

		switch (m_select)
		{
		case CContinueUI::SELECT_YES:

			// コンティニュー処理
			Continue();

			// ゲームBGMを復活させる
			CManager::Get()->GetSound()->Replay(CSound::SOUND_LABEL_BGM_GAME);

			// ゲームBGMを等速にする
			CManager::Get()->GetSound()->SetFrequency(CSound::SOUND_LABEL_BGM_GAME, 1.0f);

			break;


		case CContinueUI::SELECT_NO:

			// 終了状態にする
			CGame::SetState(CGame::STATE_FINISH);

			break;

		default:

			// 停止処理
			assert(false);

			break;
		}

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//========================
// コンティニュー処理
//========================
void CContinueUI::Continue(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();		// プレイヤーの情報
	CGameScore* pScore = CGame::GetGameScore();	// ゲームスコアの情報

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// 体力を設定する
		pPlayer->SetLife(CONTINUE_LIFE);

		// プレイヤーを無敵状態に設定
		CPlayer::SState state = pPlayer->GetState();
		state.state = CPlayer::STATE_INVINSIBLE;
		pPlayer->SetState(state);

		// プレイモードにする
		CGame::SetState(CGame::STATE_PLAY);

		// 通常カメラにする
		CManager::Get()->GetCamera()->SetType(CCamera::TYPE_NONE);

		// スコアを減らす
		pScore->SetScore(pScore->GetScore() - m_nSubScore);

		// スコアの減算量をどんどん倍にしていく
		m_nSubScore *= 2;

		if (m_nSubScore >= MAX_SUB_SCORE)
		{ // 得点の減算量が最大数を超えた場合

			// 数値を補正する
			m_nSubScore = MAX_SUB_SCORE;
		}
	}
}