//============================================
//
// ファイルのメイン処理[file.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "scene.h"
#include "file.h"
#include "object.h"
#include "obstacle.h"
#include "block.h"
#include "camera_range.h"
#include "Item.h"
#include "item_generator.h"
#include "enemy.h"
#include "time.h"
#include "player.h"
#include "balloon.h"
#include "goal.h"

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CFile::c_apFileName[TYPE_MAX] =				// ファイルの名前
{
	"data/TXT/Enemy.txt",				// 敵のテキスト
	"data/TXT/Block.txt",				// ブロックのテキスト
	"data/TXT/BgObject.txt",			// 背景オブジェクトのテキスト
	"data/TXT/Obstacle.txt",			// 障害物のテキスト
	"data/TXT/Item.txt",				// アイテムのテキスト
	"data/TXT/CameraRange.txt",			// カメラの範囲のテキスト
	"data/TXT/Result.txt",				// リザルト画面のテキスト
	"data/BIN/Ranking.bin",				// ランキング画面のバイナリ
	"data/TXT/Game.txt",				// ゲーム画面のテキスト
	"data/TXT/Tutorial.txt",			// チュートリアル画面のテキスト
};

const char* CFile::c_apBooleanDisp[2] =					// bool型の表示
{
	"FALSE",							// TRUE
	"TRUE",								// FALSE
};

//===========================================
// コンストラクタ
//===========================================
CFile::CFile()
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < MAX_FILE_DATA; nCnt++)
	{
		// ブロック情報
		m_BlockInfo.info[nCnt] = {};

		// 障害物情報
		m_ObstacleInfo.info[nCnt] = {};

		// アイテム情報
		m_ItemInfo.pos[nCnt] = {};

		// カメラの範囲情報
		m_CameraInfo.info[nCnt] = {};

		// チュートリアルの情報
		m_TutorialInfo.BlockInfo.info[nCnt] = {};	// ブロックの情報
		m_TutorialInfo.BgObjInfo.info[nCnt] = {};	// 背景オブジェクトの情報
		m_TutorialInfo.ObstaInfo.info[nCnt] = {};	// 障害物の情報
		m_TutorialInfo.BalloonInfo.info[nCnt] = {};	// 吹き出しの情報
	}

	for (int nCntRsl = 0; nCntRsl < CResult::RANK_MAX; nCntRsl++)
	{
		// リザルトのランク
		m_ResultInfo.aRankTime[nCntRsl] = 0;		// 時間のランクの基準
		m_ResultInfo.aRankScore[nCntRsl] = 0;		// スコアのランクの基準
	}

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// ランキングの記録
		m_RankingInfo.aRank[nCntRank] = 0;
	}

	for (int nCntTuto = 0; nCntTuto < CBalloon::TYPE_MAX; nCntTuto++)
	{
		// 吹き出しの種類ごとの情報
		m_aBalloonData[nCntTuto] = {};
	}

	// ゲームの情報
	m_GameInfo = {};

	// チュートリアルの情報
	m_TutorialInfo.GoalPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ゴールの位置
	m_TutorialInfo.PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// プレイヤーの位置

	// データの総数
	m_BlockInfo.nNumData = 0;						// ブロックの総数
	m_BlockInfo.bSuccess = false;					// 成功状況
	m_ObstacleInfo.nNumData = 0;					// 障害物の総数
	m_ObstacleInfo.bSuccess = false;				// 成功状況
	m_ItemInfo.nNumItemData = 0;					// アイテムの総数
	m_ItemInfo.nNumItemGeneData = 0;				// アイテム生成器の総数
	m_ItemInfo.bSuccess = false;					// 成功状況
	m_CameraInfo.nNumData = 0;						// カメラの範囲の総数
	m_CameraInfo.bSuccess = false;					// 成功状況
	m_ResultInfo.bSuccess = false;					// 成功状況
	m_RankingInfo.bSuccess = false;					// 成功状況
	m_GameInfo.bSuccess = false;					// 成功状況
	m_TutorialInfo.BlockInfo.nNumData = 0;			// ブロックの総数
	m_TutorialInfo.BgObjInfo.nNumData = 0;			// 背景オブジェクトの総数
	m_TutorialInfo.ObstaInfo.nNumData = 0;			// 障害物の総数
	m_TutorialInfo.BalloonInfo.nNumData = 0;		// 吹き出しの総数
	m_TutorialInfo.bSuccess = false;				// 成功状況
}

//===========================================
// デストラクタ
//===========================================
CFile::~CFile()
{

}

//===========================================
// ロード処理
//===========================================
HRESULT CFile::Load(TYPE type)
{
	switch (type)
	{
	case TYPE_ENEMY:	// 敵のファイル

		// 敵のロード処理
		LoadEnemy();

		break;

	case TYPE_BLOCK:	// ブロックのファイル

		// ブロックのロード処理
		if (FAILED(LoadBlock()))
		{ // ブロックのロードが失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_OBSTACLE:			// 障害物

		// 障害物のロード処理
		if (FAILED(LoadObstacle()))
		{ // 障害物のロードが失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_ITEM:				// アイテム

		// アイテムのロード処理
		if (FAILED(LoadItem()))
		{ // アイテムのロード処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_CAMERA:

		// カメラの範囲のロード処理
		if (FAILED(LoadCameraRange()))
		{ // カメラの範囲のロード処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_RESULT:			// リザルト

		// リザルトのロード処理
		if (FAILED(LoadResultFile()))
		{ // リザルトのロードが失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_RANKING:			// ランキング

		// ランキングのロード処理
		if (FAILED(LoadRankingFile()))
		{ // ランキングのロードが失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_GAME:				// ゲーム

		if (FAILED(LoadGameFile()))
		{ // ゲームのロードが失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_TUTORIAL:

		if (FAILED(LoadTutorialFile()))
		{ // チュートリアルのロードが失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;
	}

	// 結果を返す
	return S_OK;
}

//===========================================
// セーブ処理
//===========================================
HRESULT CFile::Save(TYPE type)
{
	switch (type)
	{
	case TYPE_ENEMY:	// 敵のファイル

		break;

	case TYPE_BLOCK:	// ブロックのファイル

		// ブロックのセーブ処理
		if (FAILED(SaveBlock()))
		{ // セーブに失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_OBSTACLE:			// 障害物

		// 障害物のセーブ処理
		if (FAILED(SaveObstacle()))
		{ // セーブに失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_ITEM:				// アイテム

		// アイテムのセーブ処理
		if (FAILED(SaveItem()))
		{ // セーブに失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_CAMERA:

		// カメラの範囲のセーブ処理
		if (FAILED(SaveCameraRange()))
		{ // セーブに失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_RANKING:			// ランキング

		// ランキングのセーブ処理
		if (FAILED(SaveRankingFile()))
		{ // セーブに失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// ブロックの設置処理
//===========================================
void CFile::SetBlock(void)
{
	if (m_BlockInfo.bSuccess == true)
	{ // ロードに成功していた場合

		for (int nCnt = 0; nCnt < m_BlockInfo.nNumData; nCnt++)
		{
			// ブロックの生成処理
			CBlock::Create
			(
				m_BlockInfo.info[nCnt].info.pos,
				m_BlockInfo.info[nCnt].info.rot,
				m_BlockInfo.info[nCnt].info.scale,
				&m_BlockInfo.info[nCnt].aMat[0],
				m_BlockInfo.info[nCnt].type,
				m_BlockInfo.info[nCnt].bCliffRight,
				m_BlockInfo.info[nCnt].bCliffLeft
			);
		}
	}
}

//===========================================
// 障害物の設置処理
//===========================================
void CFile::SetObstacle(void)
{
	if (m_ObstacleInfo.bSuccess == true)
	{ // ロードに成功していた場合

		for (int nCnt = 0; nCnt < m_ObstacleInfo.nNumData; nCnt++)
		{
			// ブロックの生成処理
			CObstacle::Create
			(
				m_ObstacleInfo.info[nCnt].pos,
				m_ObstacleInfo.info[nCnt].type
			);
		}
	}
}

//===========================================
// アイテムの設置処理
//===========================================
void CFile::SetItem(void)
{
	if (m_ItemInfo.bSuccess == true)
	{ // ロードに成功していた場合

		for (int nCntItem = 0; nCntItem < m_ItemInfo.nNumItemData; nCntItem++)
		{
			// アイテムの生成処理
			CItem::Create
			(
				m_ItemInfo.pos[nCntItem],
				CItem::STATE_NONE
			);
		}

		for (int nCntGene = 0; nCntGene < m_ItemInfo.nNumItemGeneData; nCntGene++)
		{
			// アイテム生成器の生成処理
			CItemGenerator::Create
			(
				m_ItemInfo.pos[nCntGene + m_ItemInfo.nNumItemData]
			);
		}
	}
}

//===========================================
// カメラの範囲の設置処理
//===========================================
void CFile::SetCamera(void)
{
	if (m_CameraInfo.bSuccess == true)
	{ // ロードに成功していた場合

		for (int nCnt = 0; nCnt < m_CameraInfo.nNumData; nCnt++)
		{
			// ブロックの生成処理
			CCameraRange::Create
			(
				m_CameraInfo.info[nCnt].pos,
				m_CameraInfo.info[nCnt].size,
				m_CameraInfo.info[nCnt].type
			);
		}
	}
}

//===========================================
// リザルト情報の設置処理
//===========================================
CFile::SResultFile CFile::GetResultInfo(void)
{
	if (m_ResultInfo.bSuccess == false)
	{ // 成功していない場合

		// エラーメッセージボックス
		MessageBox(NULL, "リザルトの情報が読み込めてません！", "警告！", MB_ICONWARNING);
	}

	// リザルトの情報を返す
	return m_ResultInfo;
}

//===========================================
// ランキングの設定処理
//===========================================
void CFile::SetRankingInfo(int* pRank)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++, pRank++)
	{
		// ランキングの情報を設定する
		m_RankingInfo.aRank[nCnt] = *pRank;
	}
}

//===========================================
// ランキングの取得処理
//===========================================
CFile::SRankingFile CFile::GetRankingInfo(void)
{
	// ランキングの情報を返す
	return m_RankingInfo;
}

//===========================================
// ゲーム画面の設定処理
//===========================================
CFile::SGameFile CFile::GetGameInfo(void)
{
	if (m_GameInfo.bSuccess == false)
	{ // 成功していない場合

		// エラーメッセージボックス
		MessageBox(NULL, "ゲームの情報が読み込めてません！", "警告！", MB_ICONWARNING);
	}

	// ゲームの情報を返す
	return m_GameInfo;
}

//===========================================
// チュートリアルの設定処理
//===========================================
void CFile::SetTutorial(void)
{
	if (m_TutorialInfo.bSuccess == true)
	{ // チュートリアルの読み込みに成功していた場合

		for (int nCntBal = 0; nCntBal < m_TutorialInfo.BalloonInfo.nNumData; nCntBal++)
		{
			// 吹き出しの生成処理
			CBalloon::Create(m_TutorialInfo.BalloonInfo.info[nCntBal].pos, m_TutorialInfo.BalloonInfo.info[nCntBal].type);
		}

		for (int nCntBlock = 0; nCntBlock < m_TutorialInfo.BlockInfo.nNumData; nCntBlock++)
		{
			// ブロックの生成処理
			CBlock::Create
			(
				m_TutorialInfo.BlockInfo.info[nCntBlock].info.pos,
				m_TutorialInfo.BlockInfo.info[nCntBlock].info.rot,
				m_TutorialInfo.BlockInfo.info[nCntBlock].info.scale,
				m_TutorialInfo.BlockInfo.info[nCntBlock].aMat,
				m_TutorialInfo.BlockInfo.info[nCntBlock].type,
				m_TutorialInfo.BlockInfo.info[nCntBlock].bCliffRight,
				m_TutorialInfo.BlockInfo.info[nCntBlock].bCliffLeft
			);
		}

		for (int nCntObsta = 0; nCntObsta < m_TutorialInfo.ObstaInfo.nNumData; nCntObsta++)
		{
			// 障害物の生成処理
			CObstacle::Create
			(
				m_TutorialInfo.ObstaInfo.info[nCntObsta].pos,
				m_TutorialInfo.ObstaInfo.info[nCntObsta].type
			);
		}

		if (CScene::GetPlayer() != nullptr)
		{ // プレイヤーが NULL じゃない場合

			// プレイヤーの位置設定処理
			CScene::GetPlayer()->SetPos(m_TutorialInfo.PlayerPos);
		}

		// ゴールを設置する
		CGoal::Create(m_TutorialInfo.GoalPos, CScene::MODE_TUTORIAL);
	}
	else
	{ // 上記以外

		// エラーメッセージボックス
		MessageBox(NULL, "チュートリアルの情報が読み込めてません！", "警告！", MB_ICONWARNING);
	}
}

//===========================================
// 吹き出しの情報の取得処理
//===========================================
CFile::SBalloonData CFile::GetBalloon(const CBalloon::TYPE type)
{
	// 吹き出しの情報を返す
	return m_aBalloonData[type];
}

//===========================================
// 初期化処理
//===========================================
HRESULT CFile::Init(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_FILE_DATA; nCnt++)
	{
		// ブロック情報
		m_BlockInfo.info[nCnt] = {};

		// 障害物情報
		m_ObstacleInfo.info[nCnt] = {};

		// アイテム情報
		m_ItemInfo.pos[nCnt] = {};

		// カメラの範囲情報
		m_CameraInfo.info[nCnt] = {};
	}

	for (int nCntRsl = 0; nCntRsl < CResult::RANK_MAX; nCntRsl++)
	{
		// リザルトのランク
		m_ResultInfo.aRankTime[nCntRsl] = 0;		// 時間のランクの基準
		m_ResultInfo.aRankScore[nCntRsl] = 0;		// スコアのランクの基準
	}

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// ランキングの記録
		m_RankingInfo.aRank[nCntRank] = 0;
	}

	// ゲームの情報
	m_GameInfo = {};

	// データの総数
	m_BlockInfo.nNumData = 0;			// ブロックの総数
	m_BlockInfo.bSuccess = false;		// 成功状況
	m_ObstacleInfo.nNumData = 0;		// 障害物の総数
	m_ObstacleInfo.bSuccess = false;	// 成功状況
	m_ItemInfo.nNumItemData = 0;		// アイテムの総数
	m_ItemInfo.nNumItemGeneData = 0;	// アイテム生成器の総数
	m_CameraInfo.nNumData = 0;			// カメラの範囲の総数
	m_CameraInfo.bSuccess = false;		// 成功状況
	m_ItemInfo.bSuccess = false;		// 成功状況
	m_ResultInfo.bSuccess = false;		// 成功状況
	m_RankingInfo.bSuccess = false;		// 成功状況
	m_GameInfo.bSuccess = false;		// 成功状況

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CFile::Uninit(void)
{
	// 全ての値の破棄
	for (int nCnt = 0; nCnt < MAX_FILE_DATA; nCnt++)
	{
		// 情報の終了
		m_BlockInfo.info[nCnt].info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		m_BlockInfo.info[nCnt].info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		m_BlockInfo.info[nCnt].info.scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 拡大率
		m_BlockInfo.info[nCnt].type = CBlock::TYPE_DIRT;						// ブロックの種類

		for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
		{
			ZeroMemory(&m_BlockInfo.info[nCnt].aMat[nCntMat], sizeof(D3DXMATERIAL));		// マテリアルデータ
		}

		// 情報の終了
		m_ObstacleInfo.info[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		m_ObstacleInfo.info[nCnt].type = CObstacle::TYPE_HYDRANT;			// ブロックの種類

		// 情報の終了
		m_ItemInfo.pos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置

		m_CameraInfo.info[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		m_CameraInfo.info[nCnt].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
		m_CameraInfo.info[nCnt].type = CCamera::TYPE_NONE;					// 種類
	}

	for (int nCntRsl = 0; nCntRsl < CResult::RANK_MAX; nCntRsl++)
	{
		// リザルトのランク
		m_ResultInfo.aRankTime[nCntRsl] = 0;		// 時間のランクの基準
		m_ResultInfo.aRankScore[nCntRsl] = 0;		// スコアのランクの基準
	}

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// ランキングの記録
		m_RankingInfo.aRank[nCntRank] = 0;
	}

	// データの総数
	m_BlockInfo.nNumData = 0;			// ブロックの総数
	m_BlockInfo.bSuccess = false;		// 成功状況
	m_ObstacleInfo.nNumData = 0;		// 障害物の総数
	m_ObstacleInfo.bSuccess = false;	// 成功状況
	m_ItemInfo.nNumItemData = 0;		// アイテムの総数
	m_ItemInfo.nNumItemGeneData = 0;	// アイテム生成器の総数
	m_ItemInfo.bSuccess = false;		// 成功状況
	m_CameraInfo.nNumData = 0;			// カメラの範囲の総数
	m_CameraInfo.bSuccess = false;		// 成功状況
	m_ResultInfo.bSuccess = false;		// 成功状況
	m_RankingInfo.bSuccess = false;		// 成功状況
}

//===========================================
// 更新処理
//===========================================
void CFile::Update(void)
{
	
}

//===========================================
// 描画処理
//===========================================
void CFile::Draw(void)
{
	
}

//===========================================
// 敵のロード処理
//===========================================
HRESULT CFile::LoadEnemy(void)
{
	//// 変数を宣言
	//int nEnd;							// テキスト読み込み終了の確認用
	//char aString[MAX_STRING];			// テキストの文字列の代入用
	//m_BlockInfo[TYPE_ENEMY].nNumData = 0;	// 総数を初期化する

	//// ポインタを宣言
	//FILE *pFile;						// ファイルポインタ

	//// ファイルを読み込み形式で開く
	//pFile = fopen(c_apFileName[TYPE_ENEMY], "r");

	//if (pFile != NULL)
	//{ // ファイルが開けた場合

	//	do
	//	{ // 読み込んだ文字列が EOF ではない場合ループ

	//		// ファイルから文字列を読み込む
	//		nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

	//		if (strcmp(&aString[0], "SET_ENEMY") == 0)
	//		{ // 読み込んだ文字列が SET_ENEMY の場合

	//			if (m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData] == NULL)
	//			{ // ポインタが NULL の場合

	//				// メモリを確保する
	//				m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData] = new SInfo;
	//			}

	//			// 情報の初期化
	//			m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	//			m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	//			m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	//			m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->type = 0;									// 種類

	//			do
	//			{ // 読み込んだ文字列が END_SET_ENEMY ではない場合ループ

	//				// ファイルから文字列を読み込む
	//				fscanf(pFile, "%s", &aString[0]);

	//				if (strcmp(&aString[0], "POS") == 0)
	//				{ // 読み込んだ文字列が POS の場合

	//					fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
	//					fscanf(pFile, "%f%f%f", 
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->pos.x, 
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->pos.y, 
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->pos.z);		// 位置を読み込む
	//				}
	//				else if (strcmp(&aString[0], "ROT") == 0)
	//				{ // 読み込んだ文字列が ROT の場合

	//					fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
	//					fscanf(pFile, "%f%f%f",
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->rot.x,
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->rot.y,
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->rot.z);		// 向きを読み込む
	//				}
	//				else if (strcmp(&aString[0], "SIZE") == 0)
	//				{ // 読み込んだ文字列が SIZE の場合

	//					fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
	//					fscanf(pFile, "%f%f%f",
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->size.x,
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->size.y,
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->size.z);		// サイズを読み込む
	//				}
	//			} while (strcmp(&aString[0], "END_SET_ENEMY") != 0);		// 読み込んだ文字列が END_SET_ENEMY ではない場合ループ

	//			// データの総数を増やす
	//			m_BlockInfo[TYPE_ENEMY].nNumData++;
	//		}
	//	} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

	//	// ファイルを閉じる
	//	fclose(pFile);
	//}
	//else
	//{ // ファイルが開けなかった場合

	//	// エラーメッセージボックス
	//	MessageBox(NULL, "敵の情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

	//	// 失敗を返す
	//	return E_FAIL;
	//}

	// 成功を返す
	return S_OK;
}

//===========================================
// ブロックのセーブ処理
//===========================================
HRESULT CFile::SaveBlock(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトへのポインタ
	CBlock* pBlock = nullptr;		// ブロックへのポインタ

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_BLOCK], "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		// 先頭のオブジェクトの情報を取得する
		pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // オブジェクトへのポインタが NULL じゃなかった場合

			if (pObj->GetType() == CObject::TYPE_BLOCK)
			{ // ブロックだった場合

				// 影の情報を取得する
				pBlock = dynamic_cast<CBlock*>(pObj);

				if (pBlock != nullptr)
				{ // ブロックが NULL じゃない場合

					// 文字列を書き込む
					fprintf(pFile, "SET_BLOCK\n");		// ブロックの設定を書き込む

					fprintf(pFile, "\tPOS = ");			// 位置の設定を書き込む
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetPos().x, pBlock->GetPos().y, pBlock->GetPos().z);			// 位置を書き込む

					fprintf(pFile, "\tROT = ");			// 向きの設定を書き込む
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetRot().x, pBlock->GetRot().y, pBlock->GetRot().z);			// 向きを書き込む

					fprintf(pFile, "\tSCALE = ");		// サイズの設定を書き込む
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetScale().x, pBlock->GetScale().y, pBlock->GetScale().z);	// 拡大率を書き込む

					fprintf(pFile, "\tTYPE = ");		// 種類の設定を書き込む
					fprintf(pFile, "%d\n", pBlock->GetType());		// 種類を書き込む

					fprintf(pFile, "\tMAT\n");			// マテリアルを書き込む

					for (int nCntMat = 0; nCntMat < (int)pBlock->GetFileData().dwNumMat; nCntMat++)
					{
						fprintf(pFile, "\tMAT%d = ", nCntMat + 1);	// マテリアルの番号を書き込む
						fprintf(pFile, "%.1f %.1f %.1f %.1f\n",		// マテリアルを書き込む
							pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.r,
							pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.g,
							pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.b,
							pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.a
						);
					}

					fprintf(pFile, "\tRCLIFF = ");		// 右崖の判定の設定を書き込む
					fprintf(pFile, "%s\n", c_apBooleanDisp[(int)(pBlock->GetCliffRight())]);		// 右崖の判定を書き込む

					fprintf(pFile, "\tLCLIFF = ");		// 左崖の判定の設定を書き込む
					fprintf(pFile, "%s\n", c_apBooleanDisp[(int)(pBlock->GetCliffLeft())]);			// 左崖の判定を書き込む

					// 文字列を書き込む
					fprintf(pFile, "END_SET_BLOCK\n\n");			// ブロックの設定の終了を書き込む
				}
			}

			// 次のオブジェクトを代入する
			pObj = pObj->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ブロックの情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// ブロックのロード処理
//===========================================
HRESULT CFile::LoadBlock(void)
{
	// 変数を宣言
	int nEnd = 0;					// テキスト読み込み終了の確認用
	char aString[MAX_STRING];		// テキストの文字列の代入用
	m_BlockInfo.nNumData = 0;		// 総数を初期化する
	m_BlockInfo.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_BLOCK], "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_BLOCK") == 0 &&
				nEnd != EOF)
			{ // 読み込んだ文字列が SET_BLOCK の場合

				// ブロックのロード処理の中身
				LoadBlockProcess(pFile, &m_BlockInfo);
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_BlockInfo.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ブロックの情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// ブロックのロード処理の中身
//===========================================
void CFile::LoadBlockProcess(FILE *pFile, SBlockFile *pInfo)
{
	// ローカル変数宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用

	do
	{ // 読み込んだ文字列が END_SET_BLOCK ではない場合ループ

		// ファイルから文字列を読み込む
		fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合

			fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
			fscanf(pFile, "%f%f%f",
				&pInfo->info[pInfo->nNumData].info.pos.x,
				&pInfo->info[pInfo->nNumData].info.pos.y,
				&pInfo->info[pInfo->nNumData].info.pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // 読み込んだ文字列が ROT の場合

			fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
			fscanf(pFile, "%f%f%f",
				&pInfo->info[pInfo->nNumData].info.rot.x,
				&pInfo->info[pInfo->nNumData].info.rot.y,
				&pInfo->info[pInfo->nNumData].info.rot.z);	// 向きを読み込む
		}
		else if (strcmp(&aString[0], "SCALE") == 0)
		{ // 読み込んだ文字列が SCALE の場合

			fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
			fscanf(pFile, "%f%f%f",
				&pInfo->info[pInfo->nNumData].info.scale.x,
				&pInfo->info[pInfo->nNumData].info.scale.y,
				&pInfo->info[pInfo->nNumData].info.scale.z);	// サイズを読み込む
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{ // 読み込んだ文字列が TYPE の場合
			fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
			fscanf(pFile, "%d",
				&pInfo->info[pInfo->nNumData].type);	// 種類を読み込む
		}
		else if (strcmp(&aString[0], "MAT") == 0)
		{ // 読み込んだ文字列が MAT の場合

			for (int nCntMat = 0; nCntMat < (int)CXFile::GetXFile((CXFile::TYPE)(INIT_BLOCK + pInfo->info[pInfo->nNumData].type)).dwNumMat; nCntMat++)
			{
				fscanf(pFile, "%s", &aString[0]);						// マテリアルの番号 を読み込む (不要)
				fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
				fscanf(pFile, "%f %f %f %f",							// 種類を読み込む
					&pInfo->info[pInfo->nNumData].aMat[nCntMat].MatD3D.Diffuse.r,
					&pInfo->info[pInfo->nNumData].aMat[nCntMat].MatD3D.Diffuse.g,
					&pInfo->info[pInfo->nNumData].aMat[nCntMat].MatD3D.Diffuse.b,
					&pInfo->info[pInfo->nNumData].aMat[nCntMat].MatD3D.Diffuse.a);
			}
		}
		else if (strcmp(&aString[0], "RCLIFF") == 0)
		{ // 読み込んだ文字列が RCLIFF の場合

			fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);						// 判定を読み込む

			if (strcmp(&aString[0], c_apBooleanDisp[0]) == 0)
			{ // FALSE だった場合

				// false を設定する
				pInfo->info[pInfo->nNumData].bCliffRight = false;
			}
			else
			{ // TRUE だった場合

				// true を設定する
				pInfo->info[pInfo->nNumData].bCliffRight = true;
			}
		}
		else if (strcmp(&aString[0], "LCLIFF") == 0)
		{ // 読み込んだ文字列が LCLIFF の場合

			fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
			fscanf(pFile, "%s", &aString[0]);						// 判定を読み込む

			if (strcmp(&aString[0], c_apBooleanDisp[0]) == 0)
			{ // FALSE だった場合

				// false を設定する
				pInfo->info[pInfo->nNumData].bCliffLeft = false;
			}
			else
			{ // TRUE だった場合

				// true を設定する
				pInfo->info[pInfo->nNumData].bCliffLeft = true;
			}
		}

	} while (strcmp(&aString[0], "END_SET_BLOCK") != 0);		// 読み込んだ文字列が END_SET_BLOCK ではない場合ループ

	// データの総数を増やす
	pInfo->nNumData++;
}

//===========================================
// 背景オブジェクトのセーブ処理
//===========================================
HRESULT CFile::SaveBgObject(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトへのポインタ
	CBlock* pBlock = nullptr;		// ブロックへのポインタ

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_BLOCK], "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		// 先頭のオブジェクトの情報を取得する
		pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // オブジェクトが NULL じゃない限り回す

			if (pObj->GetType() == CObject::TYPE_BLOCK)
			{ // ブロックだった場合

				// 影の情報を取得する
				pBlock = dynamic_cast<CBlock*>(pObj);

				if (pBlock != nullptr)
				{ // ブロックが NULL じゃない場合

					// 文字列を書き込む
					fprintf(pFile, "SET_BLOCK\n");		// ブロックの設定を書き込む

					fprintf(pFile, "\tPOS = ");			// 位置の設定を書き込む
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetPos().x, pBlock->GetPos().y, pBlock->GetPos().z);			// 位置を書き込む

					fprintf(pFile, "\tROT = ");			// 向きの設定を書き込む
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetRot().x, pBlock->GetRot().y, pBlock->GetRot().z);			// 向きを書き込む

					fprintf(pFile, "\tSCALE = ");		// サイズの設定を書き込む
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetScale().x, pBlock->GetScale().y, pBlock->GetScale().z);	// 拡大率を書き込む

					fprintf(pFile, "\tTYPE = ");		// 種類の設定を書き込む
					fprintf(pFile, "%d\n", pBlock->GetType());		// 種類を書き込む

					fprintf(pFile, "\tMAT\n");			// マテリアルを書き込む

					for (int nCntMat = 0; nCntMat < (int)pBlock->GetFileData().dwNumMat; nCntMat++)
					{
						fprintf(pFile, "\tMAT%d = ", nCntMat + 1);	// マテリアルの番号を書き込む
						fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.r, pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.g, pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.b);	// マテリアルを書き込む
					}

					// 文字列を書き込む
					fprintf(pFile, "END_SET_BLOCK\n\n");			// ブロックの設定の終了を書き込む
				}
			}

			// 次のオブジェクトを代入する
			pObj = pObj->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ブロックの情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 背景オブジェクトのロード処理
//===========================================
HRESULT CFile::LoadBgObject(void)
{
	return E_FAIL;
}

//===========================================
// 障害物のセーブ処理
//===========================================
HRESULT CFile::SaveObstacle(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトへのポインタ
	CObstacle* pObstacle = nullptr;		// ブロックへのポインタ

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_OBSTACLE], "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		// 先頭のオブジェクトの情報を取得する
		pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // オブジェクトが NULL じゃない限り回す

			if (pObj->GetType() == CObject::TYPE_OBSTACLE)
			{ // ブロックだった場合

				// 影の情報を取得する
				pObstacle = dynamic_cast<CObstacle*>(pObj);

				if (pObstacle != nullptr)
				{ // ブロックが NULL じゃない場合

					// 文字列を書き込む
					fprintf(pFile, "SET_OBSTACLE\n");		// ブロックの設定を書き込む

					fprintf(pFile, "\tPOS = ");			// 位置の設定を書き込む
					fprintf(pFile, "%.1f %.1f %.1f\n", pObstacle->GetPos().x, pObstacle->GetPos().y, pObstacle->GetPos().z);			// 位置を書き込む

					fprintf(pFile, "\tTYPE = ");		// 種類の設定を書き込む
					fprintf(pFile, "%d\n", pObstacle->GetType());		// 種類を書き込む

					// 文字列を書き込む
					fprintf(pFile, "END_SET_OBSTACLE\n\n");			// ブロックの設定の終了を書き込む
				}
			}

			// 次のオブジェクトを代入する
			pObj = pObj->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "障害物の情報の書き込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 障害物のロード処理
//===========================================
HRESULT CFile::LoadObstacle(void)
{
	// 変数を宣言
	int nEnd = 0;						// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_ObstacleInfo.nNumData = 0;		// 総数を初期化する
	m_ObstacleInfo.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_OBSTACLE], "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_OBSTACLE") == 0 &&
				nEnd != EOF)
			{ // 読み込んだ文字列が SET_OBSTACLE の場合

				// 障害物のロード処理の中身
				LoadObstacleProcess(pFile, &m_ObstacleInfo);
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_ObstacleInfo.bSuccess = true;	
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "障害物の情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 障害物のロード処理の中身
//===========================================
void CFile::LoadObstacleProcess(FILE *pFile, SObstacleFile *pInfo)
{
	// ローカル変数宣言
	char aString[MAX_STRING];			// テキストの文字列の代入用

	do
	{ // 読み込んだ文字列が END_SET_BLOCK ではない場合ループ

		// ファイルから文字列を読み込む
		fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合

			fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
			fscanf(pFile, "%f%f%f",
				&pInfo->info[pInfo->nNumData].pos.x,
				&pInfo->info[pInfo->nNumData].pos.y,
				&pInfo->info[pInfo->nNumData].pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{ // 読み込んだ文字列が TYPE の場合

			fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
			fscanf(pFile, "%d",
				&pInfo->info[pInfo->nNumData].type);	// 種類を読み込む
		}

	} while (strcmp(&aString[0], "END_SET_OBSTACLE") != 0);		// 読み込んだ文字列が END_SET_BLOCK ではない場合ループ

	// データの総数を増やす
	pInfo->nNumData++;
}

//===========================================
// アイテムのセーブ処理
//===========================================
HRESULT CFile::SaveItem(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;				// オブジェクトへのポインタ

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_ITEM], "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		{ // アイテムの書き込み

			// アイテムへのポインタ
			CItem* pItem = nullptr;

			// 文字列を書き込む
			fprintf(pFile, "ITEM\n\n");		// アイテムの印

			// 先頭のオブジェクトの情報を取得する
			pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

			while (pObj != nullptr)
			{ // オブジェクトが NULL じゃない限り回す

				if (pObj->GetType() == CObject::TYPE_ITEM)
				{ // アイテムだった場合

					// アイテムの情報を取得する
					pItem = dynamic_cast<CItem*>(pObj);

					if (pItem != nullptr)
					{ // ブロックが NULL じゃない場合

						// 文字列を書き込む
						fprintf(pFile, "\tSET_ITEM\n");			// アイテムの設定を書き込む

						// 位置
						fprintf(pFile, "\t\tPOS = ");
						fprintf(pFile, "%.1f %.1f %.1f\n", pItem->GetPos().x, pItem->GetPos().y, pItem->GetPos().z);			// 位置を書き込む

						// 文字列を書き込む
						fprintf(pFile, "\tEND_SET_ITEM\n\n");	// アイテムの設定の終了を書き込む
					}
				}

				// 次のオブジェクトを代入する
				pObj = pObj->GetNext();
			}

			// 文字列を書き込む
			fprintf(pFile, "END_ITEM\n\n");		// アイテムの印
		}

		{ // アイテム生成器の書き込み

			// アイテム生成器へのポインタ
			CItemGenerator* pItemGene = nullptr;	

			// 文字列を書き込む
			fprintf(pFile, "ITEMGENERATOR\n\n");		// アイテム生成器の印

			// 先頭のオブジェクトの情報を取得する
			pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

			while (pObj != nullptr)
			{ // オブジェクトが NULL じゃない限り回す

				if (pObj->GetType() == CObject::TYPE_ITEMGENERATOR)
				{ // アイテム生成器だった場合

					// アイテム生成器の情報を取得する
					pItemGene = dynamic_cast<CItemGenerator*>(pObj);

					if (pItemGene != nullptr)
					{ // ブロックが NULL じゃない場合

						// 文字列を書き込む
						fprintf(pFile, "\tSET_ITEMGENE\n");			// アイテム生成器の設定を書き込む

						// 位置
						fprintf(pFile, "\t\tPOS = ");
						fprintf(pFile, "%.1f %.1f %.1f\n", pItemGene->GetPos().x, pItemGene->GetPos().y, pItemGene->GetPos().z);			// 位置を書き込む

						// 文字列を書き込む
						fprintf(pFile, "\tEND_SET_ITEMGENE\n\n");	// アイテム生成器の設定の終了を書き込む
					}
				}

				// 次のオブジェクトを代入する
				pObj = pObj->GetNext();
			}

			// 文字列を書き込む
			fprintf(pFile, "END_ITEMGENERATOR\n");		// アイテム生成器の印
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "アイテムの情報の書き込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// アイテムのロード処理
//===========================================
HRESULT CFile::LoadItem(void)
{
	// 変数を宣言
	int nEnd = 0;						// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_ItemInfo.nNumItemData = 0;		// アイテムの総数を初期化する
	m_ItemInfo.nNumItemGeneData = 0;	// アイテム生成器の総数を初期化する
	m_ItemInfo.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_ITEM], "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "ITEM") == 0 &&
				nEnd != EOF)
			{ // アイテムだった場合

				do
				{ // 読み込んだ文字列が END_ITEM ではない場合ループ

					// ファイルから文字列を読み込む
					nEnd = fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SET_ITEM") == 0 &&
						nEnd != EOF)
					{ // 読み込んだ文字列が SET_ITEM の場合

						do
						{ // 読み込んだ文字列が END_SET_ITEM ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f%f%f",
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].x,
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].y,
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].z);		// 位置を読み込む
							}

						} while (strcmp(&aString[0], "END_SET_ITEM") != 0);		// 読み込んだ文字列が END_SET_ITEM ではない場合ループ

						// アイテムのデータの総数を増やす
						m_ItemInfo.nNumItemData++;
					}
				} while (strcmp(&aString[0], "END_ITEM") != 0);		// 読み込んだ文字列が SET_ITEM ではない場合ループ
			}						// アイテムだった場合
			else if (strcmp(&aString[0], "ITEMGENERATOR") == 0 &&
				nEnd != EOF)
			{ // アイテム生成器だった場合

				do
				{ // 読み込んだ文字列が END_ITEMGENE ではない場合ループ

					// ファイルから文字列を読み込む
					nEnd = fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SET_ITEMGENE") == 0 &&
						nEnd != EOF)
					{ // 読み込んだ文字列が SET_ITEM の場合

						do
						{ // 読み込んだ文字列が END_SET_ITEM ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f%f%f",
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].x,
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].y,
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].z);		// 位置を読み込む
							}

						} while (strcmp(&aString[0], "END_SET_ITEMGENE") != 0);		// 読み込んだ文字列が END_SET_ITEM ではない場合ループ

						// アイテムのデータの総数を増やす
						m_ItemInfo.nNumItemGeneData++;
					}
				} while (strcmp(&aString[0], "END_ITEMGENERATOR") != 0);		// 読み込んだ文字列が SET_ITEMGENE ではない場合ループ
			}							// アイテム生成器だった場合
		} while (nEnd != EOF);			// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_ItemInfo.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "アイテムの情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// カメラの範囲のセーブ処理
//===========================================
HRESULT CFile::SaveCameraRange(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトへのポインタ
	CCameraRange* pCameraRange = nullptr;		// ブロックへのポインタ

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_CAMERA], "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		// 先頭のオブジェクトの情報を取得する
		pObj = CObject::GetTop(CObject::PRIORITY_BG);

		while (pObj != nullptr)
		{ // オブジェクトが NULL じゃない限り回す

			if (pObj->GetType() == CObject::TYPE_CAMERARANGE)
			{ // ブロックだった場合

				// カメラの範囲の情報を取得する
				pCameraRange = dynamic_cast<CCameraRange*>(pObj);

				if (pCameraRange != nullptr)
				{ // ブロックが NULL じゃない場合

					// 文字列を書き込む
					fprintf(pFile, "SET_CAMERA\n");		// ブロックの設定を書き込む

					// 位置
					fprintf(pFile, "\tPOS = ");
					fprintf(pFile, "%.1f %.1f %.1f\n", pCameraRange->GetPos().x, pCameraRange->GetPos().y, pCameraRange->GetPos().z);

					// サイズ
					fprintf(pFile, "\tSIZE = ");
					fprintf(pFile, "%.1f %.1f %.1f\n", pCameraRange->GetSize().x, pCameraRange->GetSize().y, pCameraRange->GetSize().z);

					// 種類
					fprintf(pFile, "\tTYPE = ");
					fprintf(pFile, "%d\n", pCameraRange->GetType());

					// 文字列を書き込む
					fprintf(pFile, "END_SET_CAMERA\n\n");			// ブロックの設定の終了を書き込む
				}
			}

			// 次のオブジェクトを代入する
			pObj = pObj->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "カメラの範囲の情報の書き込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// カメラの範囲のロード処理
//===========================================
HRESULT CFile::LoadCameraRange(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_CameraInfo.nNumData = 0;		// 総数を初期化する
	m_CameraInfo.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_CAMERA], "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_CAMERA") == 0)
			{ // 読み込んだ文字列が SET_CAMERA の場合

				do
				{ // 読み込んだ文字列が END_SET_CAMERA ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_CameraInfo.info[m_CameraInfo.nNumData].pos.x,
							&m_CameraInfo.info[m_CameraInfo.nNumData].pos.y,
							&m_CameraInfo.info[m_CameraInfo.nNumData].pos.z);	// 位置を読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_CameraInfo.info[m_CameraInfo.nNumData].size.x,
							&m_CameraInfo.info[m_CameraInfo.nNumData].size.y,
							&m_CameraInfo.info[m_CameraInfo.nNumData].size.z);	// 位置を読み込む
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合
						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%d",
							&m_CameraInfo.info[m_CameraInfo.nNumData].type);	// 種類を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_CAMERA") != 0);		// 読み込んだ文字列が END_SET_CAMERA ではない場合ループ

				// データの総数を増やす
				m_CameraInfo.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_CameraInfo.bSuccess = true;	
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "カメラの範囲の情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// リザルトのロード処理
//===========================================
HRESULT CFile::LoadResultFile(void)
{
	// ローカル変数宣言
	int nEnd;						// テキスト読み込み終了の確認用
	char aString[MAX_STRING];		// テキストの文字列の代入用
	m_ResultInfo.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_RESULT], "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "RANK_TIME") == 0)
			{ // 読み込んだ文字列が RANK_TIME の場合

				for (int nCntTime = 0; nCntTime < CResult::RANK_MAX; nCntTime++)
				{
					fscanf(pFile, "%s", &aString[0]);							// ランク を読み込む (不要)
					fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
					fscanf(pFile, "%d", &m_ResultInfo.aRankTime[nCntTime]);		// タイムを読み込む
				}
			}
			if (strcmp(&aString[0], "RANK_SCORE") == 0)
			{ // 読み込んだ文字列が RANK_SCORE の場合

				for (int nCntTime = 0; nCntTime < CResult::RANK_MAX; nCntTime++)
				{
					fscanf(pFile, "%s", &aString[0]);							// ランク を読み込む (不要)
					fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
					fscanf(pFile, "%d", &m_ResultInfo.aRankScore[nCntTime]);	// スコアを読み込む
				}
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_ResultInfo.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "リザルトの情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// ランキングのセーブ処理
//===========================================
HRESULT CFile::SaveRankingFile(void)
{
	FILE *pFile;												// ファイルポインタを宣言
	m_RankingInfo.bSuccess = false;								// 成功状況

	// ファイルを開く
	pFile = fopen(c_apFileName[TYPE_RANKING], "wb");			// バイナリファイルに書き込むために開く

	// ファイルを比較する
	if (pFile != NULL)
	{ // ファイルが開けた場合

		// ファイルから数値を書き出す
		fwrite(&m_RankingInfo.aRank[0], sizeof(int), MAX_RANKING, pFile);

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_RankingInfo.bSuccess = true;

		// 成功を返す
		return S_OK;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ランキングの情報の書き込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}
}

//===========================================
// ランキングのロード処理
//===========================================
HRESULT CFile::LoadRankingFile(void)
{
	FILE *pFile;						// ファイルポインタを宣言
	m_RankingInfo.bSuccess = false;		// 成功状況

	// ファイルを開く
	pFile = fopen(c_apFileName[TYPE_RANKING], "rb");			// バイナリファイルから読み込むために開く

	// ファイルを比較する
	if (pFile != NULL)
	{ // ファイルが開けた場合

		// ファイルから数値を読み込む
		fread(&m_RankingInfo.aRank[0], sizeof(int), MAX_RANKING, pFile);

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_RankingInfo.bSuccess = true;

		// 成功を返す
		return S_OK;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ランキングの情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 数値を設定する
		m_RankingInfo.aRank[0] = MAX_TIME;
		m_RankingInfo.aRank[1] = MAX_TIME;
		m_RankingInfo.aRank[2] = MAX_TIME;
		m_RankingInfo.aRank[3] = MAX_TIME;
		m_RankingInfo.aRank[4] = MAX_TIME;

		// 失敗を返す
		return E_FAIL;
	}
}

//===========================================
// ゲームのロード処理
//===========================================
HRESULT CFile::LoadGameFile(void)
{
	// ローカル変数宣言
	int nEnd;						// テキスト読み込み終了の確認用
	char aString[MAX_STRING];		// テキストの文字列の代入用
	m_GameInfo.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_GAME], "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "PLAYER_POS") == 0)
			{ // 読み込んだ文字列が PLAYER_POS の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// プレイヤーの初期位置を読み込む
					&m_GameInfo.PlayerPos.x, &m_GameInfo.PlayerPos.y, &m_GameInfo.PlayerPos.z);
			}
			else if (strcmp(&aString[0], "START_POS") == 0)
			{ // 読み込んだ文字列が START_POS の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// プレイヤーのスタート位置を読み込む
					&m_GameInfo.StartPos.x, &m_GameInfo.StartPos.y, &m_GameInfo.StartPos.z);
			}
			else if (strcmp(&aString[0], "GOAL_POS") == 0)
			{ // 読み込んだ文字列が GOAL_POS の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// ゴールの位置を読み込む
					&m_GameInfo.GoalPos.x, &m_GameInfo.GoalPos.y, &m_GameInfo.GoalPos.z);
			}
			else if (strcmp(&aString[0], "STAGE_LEFT") == 0)
			{ //  読み込んだ文字列が STAGE_LEFT の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// ステージの左端を読み込む
					&m_GameInfo.StageLeftPos.x, &m_GameInfo.StageLeftPos.y, &m_GameInfo.StageLeftPos.z);
			}
			else if (strcmp(&aString[0], "SCORE_POS") == 0)
			{ //  読み込んだ文字列が SCORE_POS の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// スコアの位置を読み込む
					&m_GameInfo.ScorePos.x, &m_GameInfo.ScorePos.y, &m_GameInfo.ScorePos.z);
			}
			else if (strcmp(&aString[0], "SCORE_SIZE") == 0)
			{ //  読み込んだ文字列が SCORE_SIZE の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// スコアのサイズを読み込む
					&m_GameInfo.ScoreSize.x, &m_GameInfo.ScoreSize.y, &m_GameInfo.ScoreSize.z);
			}
			else if (strcmp(&aString[0], "TIME_POS") == 0)
			{ //  読み込んだ文字列が TIME_POS の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// タイムの位置を読み込む
					&m_GameInfo.TimePos.x, &m_GameInfo.TimePos.y, &m_GameInfo.TimePos.z);
			}
			else if (strcmp(&aString[0], "TIME_SIZE") == 0)
			{ //  読み込んだ文字列が TIME_SIZE の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// タイムのサイズを読み込む
					&m_GameInfo.TimeSize.x, &m_GameInfo.TimeSize.y, &m_GameInfo.TimeSize.z);
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_GameInfo.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ゲームの情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// チュートリアルのロード処理
//===========================================
HRESULT CFile::LoadTutorialFile(void)
{
	// ローカル変数宣言
	int nEnd = 0;						// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_TutorialInfo.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(c_apFileName[TYPE_TUTORIAL], "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "BALLOON_DATA") == 0 &&
				nEnd != EOF)
			{ // 読み込んだ文字列が BALLOON_DATA の場合

				for (int nCnt = 0; nCnt < CBalloon::TYPE_MAX; nCnt++)
				{
					fscanf(pFile, "%s", &aString[0]);		// TYPE を読み込む (不要)
					fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
					fscanf(pFile, "%f %f %f",				// 吹き出しのサイズを読み込む
						&m_aBalloonData[nCnt].size.x, 
						&m_aBalloonData[nCnt].size.y, 
						&m_aBalloonData[nCnt].size.z);
					fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
					fscanf(pFile, "%s", &m_aBalloonData[nCnt].aTextureName[0]);	// 吹き出しのテクスチャを読み込む
				}
			}
			else if (strcmp(&aString[0], "SET_BALLOON") == 0 &&
				nEnd != EOF)
			{ // 読み込んだ文字列が SET_BALLOON の場合

				// 吹き出しのロード処理の中身
				LoadBalloonProcess(pFile, &m_TutorialInfo.BalloonInfo);
			}
			else if (strcmp(&aString[0], "SET_BLOCK") == 0 &&
				nEnd != EOF)
			{ // 読み込んだ文字列が SET_BLOCK の場合

				// ブロックのロード処理の中身
				LoadBlockProcess(pFile, &m_TutorialInfo.BlockInfo);
			}
			else if (strcmp(&aString[0], "SET_BGOBJECT") == 0 &&
				nEnd != EOF)
			{ // 読み込んだ文字列が SET_BGOBJECT の場合

			}
			else if (strcmp(&aString[0], "SET_OBSTACLE") == 0 &&
				nEnd != EOF)
			{ // 読み込んだ文字列が SET_OBSTACLE の場合

				// 障害物のロード処理の中身
				LoadObstacleProcess(pFile, &m_TutorialInfo.ObstaInfo);
			}
			else if (strcmp(&aString[0], "PLAYER_POS") == 0)
			{ //  読み込んだ文字列が PLAYER_POS の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// プレイヤーの位置を読み込む
					&m_TutorialInfo.PlayerPos.x, &m_TutorialInfo.PlayerPos.y, &m_TutorialInfo.PlayerPos.z);
			}
			else if (strcmp(&aString[0], "GOAL_POS") == 0)
			{ //  読み込んだ文字列が GOAL_POS の場合

				fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
				fscanf(pFile, "%f %f %f",									// ゴールの位置を読み込む
					&m_TutorialInfo.GoalPos.x, &m_TutorialInfo.GoalPos.y, &m_TutorialInfo.GoalPos.z);
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_TutorialInfo.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "チュートリアルの情報の読み込みに失敗!", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 吹き出しのロード処理の中身
//===========================================
void CFile::LoadBalloonProcess(FILE *pFile, SBalloonFile *pInfo)
{
	// ローカル変数宣言
	char aString[MAX_STRING];			// テキストの文字列の代入用

	do
	{ // 読み込んだ文字列が END_SET_BALLOON ではない場合ループ

		// ファイルから文字列を読み込む
		fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // 読み込んだ文字列が POS の場合

			fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
			fscanf(pFile, "%f %f %f",
				&pInfo->info[pInfo->nNumData].pos.x,
				&pInfo->info[pInfo->nNumData].pos.y,
				&pInfo->info[pInfo->nNumData].pos.z);	// 位置を読み込む
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{ // 読み込んだ文字列が TYPE の場合

			fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
			fscanf(pFile, "%d",
				&pInfo->info[pInfo->nNumData].type);	// 種類を読み込む
		}
	} while (strcmp(&aString[0], "END_SET_BALLOON") != 0);		// 読み込んだ文字列が END_SET_BALLOON ではない場合ループ

	// 吹き出しの総数を加算する
	pInfo->nNumData++;
}