//============================================
//
// タイトル画面のメイン処理[title.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "object.h"
#include "title.h"
#include "fade.h"
#include "Objectmesh.h"
#include "skybox.h"

#include "title_name.h"
#include "file.h"
#include "silhouette.h"

// マクロ定義
#define SET_RANKING_TIMER		(600)		// ランキング画面に遷移するカウント数

//=========================================
// コンストラクタ
//=========================================
CTitle::CTitle() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// 全ての値をクリアする
	m_nTransCount = 0;
}

//=========================================
// デストラクタ
//=========================================
CTitle::~CTitle()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CTitle::Init(void)
{
	//　シーンの初期化
	CScene::Init();

	// テキスト読み込み処理
	CMesh::TxtSet();

	// スカイボックスの設定処理
	CSkyBox::Create();

	// 全ての値を初期化する
	m_nTransCount = 0;

	// ステージの設定処理
	CManager::GetFile()->SetBlock();		// ブロックの設置処理
	CManager::GetFile()->SetObstacle();		// 障害物の設置処理

	// ローカル変数宣言
	CSilhouette::TYPE aType[2] = { CSilhouette::TYPE_MAX, CSilhouette::TYPE_MAX };		// 種類

	// 種類をランダムで選出する
	aType[0] = (CSilhouette::TYPE)(rand() % CSilhouette::TYPE_MAX);

	// 影絵の設定処理
	CSilhouette::Create(aType[0], false);

	do
	{ // 種類が一致している場合回す

		// 種類をランダムで選出する
		aType[1] = (CSilhouette::TYPE)(rand() % CSilhouette::TYPE_MAX);

	} while (aType[0] == aType[1]);

	// 影絵の設定処理
	CSilhouette::Create(aType[1], true);

	// タイトルを生成する
	CTitleName::Create();

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CTitle::Uninit(void)
{
	// 終了処理
	CScene::Uninit();
}

//======================================
//更新処理
//======================================
void CTitle::Update(void)
{
	// 遷移カウントを加算する
	m_nTransCount++;

	if (CManager::GetRenderer() != nullptr)
	{ // レンダラーが NULL じゃない場合

		// レンダラーの更新
		CManager::GetRenderer()->Update();
	}

	if (m_nTransCount % SET_RANKING_TIMER == 0)
	{ // 遷移カウントが一定数に達した場合

		// ランキングに遷移する
		CManager::GetFade()->SetFade(CScene::MODE_RANKING);

		// この先の処理を行わない
		return;
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	{ // ENTERキーを押した場合

		// チュートリアルに遷移する
		CManager::GetFade()->SetFade(CScene::MODE_TUTORIAL);

		// この先の処理を行わない
		return;
	}
}

//======================================
//描画処理
//======================================
void CTitle::Draw(void)
{

}