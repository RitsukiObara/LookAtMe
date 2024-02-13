//=======================================
//
//ランキングのメイン処理[Ranking.cpp]
//Author 小原立暉
//
//=======================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "ranking.h"
#include "file.h"
#include "input.h"
#include "Fade.h"
#include "Objectmesh.h"
#include "skybox.h"

#include "ranking_time.h"
#include "ranking_score.h"
#include "ranking_precedence.h"
#include "ranking_scoredisp.h"
#include "ranking_word.h"

//マクロ定義
#define SOUND_NEW_RECORD		(90)			// スコア加算で音が鳴る時間
#define SET_TITLE_TIMER			(300)			// タイトル画面に行く秒数
#define RANKING_CAMERA_POSY		(800.0f)		// カメラの位置(Y軸)
#define RANKING_CAMERA_DISTANCE	(2000.0f)		// カメラの距離

//==========================================
// コンストラクタ
//==========================================
CRanking::CRanking() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// 全ての値をクリアする
	m_nTransCount = 0;			// 自動遷移カウント
}

//==========================================
// デストラクタ
//==========================================
CRanking::~CRanking()
{

}

//==========================================
// ランキングの初期化処理
//==========================================
HRESULT CRanking::Init(void)
{	
	// シーンの初期化
	CScene::Init();

	// ステージの設定処理
	CManager::GetFile()->SetBlock();		// ブロックの設置処理
	CManager::GetFile()->SetObstacle();		// 障害物の設置処理
	CManager::GetFile()->SetItem();			// アイテムの設置処理
	CManager::GetFile()->SetCamera();		// カメラの設置処理

	// テキスト読み込み処理
	CMesh::TxtSet();

	// スカイボックスの設定処理
	CSkyBox::Create();

	// 全ての値を初期化する
	m_nTransCount = 0;			// 自動遷移カウント

	// ランキングの文字の生成
	CRankingWord::Create();

	// ランキングの記録を生成
	CRankingScore::Create();

	// 順位表示を生成
	CRankingPrece::Create();

	if (CManager::GetClear() == true)
	{ // クリア状況が true の場合

		// クリアタイムの生成
		CRankingTime::Create();

		// スコア表示の生成
		CRankingScoreDisp::Create();
	}

	if (CManager::GetCamera() != nullptr)
	{ // カメラが NULL じゃなかった場合

		// ローカル変数宣言
		D3DXVECTOR3 posR;		// 注視点
		D3DXVECTOR3 posV;		// 視点

		// リセット処理
		CManager::GetCamera()->Reset();

		// 位置を取得する
		posR = CManager::GetCamera()->GetPosR();
		posV = CManager::GetCamera()->GetPosV();

		// 位置と距離を設定する
		posR.y = RANKING_CAMERA_POSY;
		posV.y = RANKING_CAMERA_POSY;

		// カメラの設定処理
		CManager::GetCamera()->SetPosR(posR);
		CManager::GetCamera()->SetPosV(posV);
		CManager::GetCamera()->SetDistance(RANKING_CAMERA_DISTANCE);
	}

	// 成功を返す
	return S_OK;
}

//========================================
// スコアの終了処理
//========================================
void CRanking::Uninit(void)
{
	// 破棄処理
	Release();

	// クリア状況の設定処理
	CManager::SetClear(false);
}

//======================================
//ランキングの更新処理
//======================================
void CRanking::Update(void)
{
	// 自動遷移カウントを加算する
	m_nTransCount++;

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START,0) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
		m_nTransCount % SET_TITLE_TIMER == 0)
	{ // ENTERキーを押したまたは、カウントが一定数に達した場合

		// タイトルに遷移する
		CManager::GetFade()->SetFade(CScene::MODE_TITLE);

		// この先の処理を行わない
		return;
	}

	if (CManager::GetRenderer() != nullptr)
	{ // レンダラーが NULL じゃない場合

		// 更新処理
		CManager::GetRenderer()->Update();
	}
}

//=====================================
//スコアの描画処理
//=====================================
void CRanking::Draw(void)
{

}