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

#include "enemy.h"
#include "coin.h"
#include "gold_bone.h"
#include "tree.h"
#include "rock.h"
#include "block.h"
#include "bang_flower.h"
#include "wall.h"
#include "boss.h"
#include "balloon_spawner.h"

//--------------------------------------------
// 定数定義
//--------------------------------------------
namespace
{
	const char* RANKING_BIN = "data\\BIN\\Ranking.bin";			// ランキングのテキスト
	const char* ENEMY_TXT = "data\\TXT\\Enemy.txt";				// 敵のテキスト
	const char* COIN_TXT = "data\\TXT\\Coin.txt";				// コインのテキスト
	const char* GOLDBONE_TXT = "data\\TXT\\GoldBone.txt";		// 金の骨のテキスト
	const char* TREE_TXT = "data\\TXT\\Tree.txt";				// 木のテキスト
	const char* ROCK_TXT = "data\\TXT\\Rock.txt";				// 岩のテキスト
	const char* BLOCK_TXT = "data\\TXT\\Block.txt";				// ブロックのテキスト
	const char* BANGFLOWER_TXT = "data\\TXT\\BangFlower.txt";	// 爆弾花のテキスト
	const char* WALL_TXT = "data\\TXT\\Wall.txt";				// 壁のテキスト
	const char* BOSSCOLL_TXT = "data\\TXT\\BossColl.txt";		// ボスの当たり判定のテキスト
	const char* SIGNBOARD_TXT = "data\\TXT\\Signboard.txt";		// 看板のテキスト
	const char* BALLOON_TXT = "data\\TXT\\Balloon.txt";			// 風船のテキスト
	const char* ENEMYROUTE_TXT = "data\\TXT\\EnemyRoute.txt";	// 敵の徘徊経路のテキスト

	const int GOLDBONE_NUM = 3;		// 金の骨の数
}

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
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
	for (int nCntRank = 0; nCntRank < CRanking::MAX_RANKING; nCntRank++)
	{
		m_RankingInfo.aRank[nCntRank] = 0;				// ランキングの値
	}

	for (int nCntBoss = 0; nCntBoss < MAX_PARTS; nCntBoss++)
	{
		for (int nCntColl = 0; nCntColl < CBossCollision::MAX_COLLISION; nCntColl++)
		{
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].offset = NONE_D3DXVECTOR3;
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].fRadius = 0.0f;
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].bWeakness = false;
		}

		m_BossCollFile.aFile[nCntBoss].nNum = 0;
	}

	for (int nCntFile = 0; nCntFile < MAX_FILE_DATA; nCntFile++)
	{
		m_EnemyFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_EnemyFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_EnemyFile.aFile[nCntFile].nType = 0;

		m_CoinFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;

		m_TreeFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_TreeFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_TreeFile.aFile[nCntFile].nType = 0;

		m_RockFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].nType = 0;

		m_BlockFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_BlockFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;

		m_BangFlowerFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_BangFlowerFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;

		m_WallFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_WallFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;
		m_WallFile.aFile[nCntFile].nType = 0;
		m_WallFile.aFile[nCntFile].nRotType = 0;

		m_SignboardFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_SignboardFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_SignboardFile.aFile[nCntFile].type = CSignboard::TYPE::TYPE_JUMP;

		m_BalloonFile.pos[nCntFile] = NONE_D3DXVECTOR3;

		for (int nCntRoute = 0; nCntRoute < MAX_ENEMY_ROUTE; nCntRoute++)
		{
			m_EnemyRouteFile.aFile[nCntFile].pos[nCntRoute] = NONE_D3DXVECTOR3;		// 敵の経路の情報
			m_EnemyRouteFile.aFile[nCntFile].nNum = 0;		// 総数
		}
	}

	for (int nCntBone = 0; nCntBone < GOLDBONE_NUM; nCntBone++)
	{
		m_GoldBoneFile.aFile[nCntBone].pos = NONE_D3DXVECTOR3;
	}

	// 総数をクリアする
	m_EnemyFile.nNumData = 0;			// 敵の情報
	m_CoinFile.nNumData = 0;			// コインの情報
	m_GoldBoneFile.nNumData = 0;		// 金の骨の情報
	m_TreeFile.nNumData = 0;			// 木の情報
	m_RockFile.nNumData = 0;			// 岩の情報
	m_BlockFile.nNumData = 0;			// ブロックの情報
	m_BangFlowerFile.nNumData = 0;		// 爆弾花の情報
	m_WallFile.nNumData = 0;			// 壁の情報
	m_BossCollFile.nNumData = 0;		// ボスの当たり判定の情報
	m_SignboardFile.nNumData = 0;		// 看板の情報
	m_BalloonFile.nNumData = 0;			// 風船の情報
	m_EnemyRouteFile.nNumData = 0;		// 敵の徘徊経路の情報

	// 成功状況をクリアする
	m_RankingInfo.bSuccess = false;		// ランキング
	m_EnemyFile.bSuccess = false;		// 敵の情報
	m_CoinFile.bSuccess = false;		// コインの情報
	m_GoldBoneFile.bSuccess = false;	// 金の骨の情報
	m_TreeFile.bSuccess = false;		// 木の情報
	m_RockFile.bSuccess = false;		// 岩の情報
	m_BlockFile.bSuccess = false;		// ブロックの情報
	m_BangFlowerFile.bSuccess = false;	// 爆弾花の情報
	m_WallFile.bSuccess = false;		// 壁の情報
	m_BossCollFile.bSuccess = false;	// ボスの当たり判定の情報
	m_SignboardFile.bSuccess = false;	// 看板の情報
	m_BalloonFile.bSuccess = false;		// 風船の情報
	m_EnemyRouteFile.bSuccess = false;	// 敵の徘徊経路の情報
}

//===========================================
// デストラクタ
//===========================================
CFile::~CFile()
{

}

//===========================================
// セーブ処理
//===========================================
HRESULT CFile::Save(const TYPE type)
{
	switch (type)
	{
	case CFile::TYPE_RANKING:

		// ランキングのセーブ処理
		if (FAILED(SaveRanking()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// ロード処理
//===========================================
HRESULT CFile::Load(const TYPE type)
{
	switch (type)
	{
	case CFile::TYPE_RANKING:

		// ランキングのロード処理
		if (FAILED(LoadRanking()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_ENEMY:

		// 敵のロード処理
		if (FAILED(LoadEnemy()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_COIN:

		// コインのロード処理
		if (FAILED(LoadCoin()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_GOLDBONE:

		// 金の骨のロード処理
		if (FAILED(LoadGoldBone()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_TREE:

		// 木のロード処理
		if (FAILED(LoadTree()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_ROCK:

		// 岩のロード処理
		if (FAILED(LoadRock()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BLOCK:

		// ブロックのロード処理
		if (FAILED(LoadBlock()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BANGFLOWER:

		// 爆弾花のロード処理
		if (FAILED(LoadBangFlower()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_WALL:

		// 壁のロード処理
		if (FAILED(LoadWall()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BOSSCOLL:

		if (FAILED(LoadBossColl()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_SIGNBOARD:

		if (FAILED(LoadSignboard()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BALLOON:

		if (FAILED(LoadBalloon()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case CFile::TYPE_ENEMYROUTE:

		if (FAILED(LoadEnemyRoute()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 結果を返す
	return S_OK;
}

//===========================================
// ランキングの設定処理
//===========================================
void CFile::SetRankingInfo(int* pRank)
{
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++, pRank++)
	{
		// ランキングの情報を設定する
		m_RankingInfo.aRank[nCnt] = *pRank;
	}
}

//===========================================
// ランキングの取得処理
//===========================================
CFile::SRankingInfo CFile::GetRankingInfo(void)
{
	// ランキングの情報を返す
	return m_RankingInfo;
}

//===========================================
// 敵の設定処理
//===========================================
void CFile::SetEnemy(void)
{
	if (m_EnemyFile.bSuccess == true)
	{ // 敵が読み込めた場合

		for (int nCnt = 0; nCnt < m_EnemyFile.nNumData; nCnt++)
		{
			// 敵を生成する
			CEnemy::Create
			(
				m_EnemyFile.aFile[nCnt].pos,
				m_EnemyFile.aFile[nCnt].rot,
				(CEnemy::TYPE)m_EnemyFile.aFile[nCnt].nType
			);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// コインの設定処理
//===========================================
void CFile::SetCoin(void)
{
	if (m_CoinFile.bSuccess == true)
	{ // コインが読み込めた場合

		for (int nCnt = 0; nCnt < m_CoinFile.nNumData; nCnt++)
		{
			// コインを生成する
			CCoin::Create
			(
				m_CoinFile.aFile[nCnt].pos
			);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// 金の骨の設定処理
//===========================================
void CFile::SetGoldBone(void)
{
	if (m_GoldBoneFile.bSuccess == true)
	{ // 金の骨が読み込めた場合

		for (int nCnt = 0; nCnt < m_GoldBoneFile.nNumData; nCnt++)
		{
			// 金の骨を生成する
			CGoldBone::Create
			(
				m_GoldBoneFile.aFile[nCnt].pos
			);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// 木の設定処理
//===========================================
void CFile::SetTree(void)
{
	if (m_TreeFile.bSuccess == true)
	{ // 木が読み込めた場合

		for (int nCnt = 0; nCnt < m_TreeFile.nNumData; nCnt++)
		{
			// 木を生成する
			CTree::Create
			(
				m_TreeFile.aFile[nCnt].pos,
				m_TreeFile.aFile[nCnt].rot,
				(CTree::TYPE)m_TreeFile.aFile[nCnt].nType
			);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// 岩の設定処理
//===========================================
void CFile::SetRock(void)
{
	if (m_RockFile.bSuccess == true)
	{ // 岩が読み込めた場合

		for (int nCnt = 0; nCnt < m_RockFile.nNumData; nCnt++)
		{
			// 岩を生成する
			CRock::Create
			(
				m_RockFile.aFile[nCnt].pos,
				m_RockFile.aFile[nCnt].rot,
				m_RockFile.aFile[nCnt].scale,
				(CRock::TYPE)m_RockFile.aFile[nCnt].nType
			);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// ブロックの設定処理
//===========================================
void CFile::SetBlock(void)
{
	if (m_BlockFile.bSuccess == true)
	{ // ブロックが読み込めた場合

		for (int nCnt = 0; nCnt < m_BlockFile.nNumData; nCnt++)
		{
			// ブロックを生成する
			CBlock::Create
			(
				m_BlockFile.aFile[nCnt].pos,
				m_BlockFile.aFile[nCnt].scale
			);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// 爆弾花の設定処理
//===========================================
void CFile::SetBangFlower(void)
{
	if (m_BangFlowerFile.bSuccess == true)
	{ // 爆弾花が読み込めた場合

		for (int nCnt = 0; nCnt < m_BangFlowerFile.nNumData; nCnt++)
		{
			// 爆弾花を生成する
			CBangFlower::Create
			(
				m_BangFlowerFile.aFile[nCnt].pos,
				m_BangFlowerFile.aFile[nCnt].rot
			);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// 壁の設定処理
//===========================================
void CFile::SetWall(void)
{
	if (m_WallFile.bSuccess == true)
	{ // 壁が読み込めた場合

		for (int nCnt = 0; nCnt < m_WallFile.nNumData; nCnt++)
		{
			// 壁を生成する
			CWall::Create
			(
				m_WallFile.aFile[nCnt].pos,
				m_WallFile.aFile[nCnt].scale,
				(CWall::TYPE)(m_WallFile.aFile[nCnt].nType),
				(CWall::ROTTYPE)(m_WallFile.aFile[nCnt].nRotType)
			);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// ボスの当たり判定の設定処理
//===========================================
void CFile::SetBossColl(CBossCollision** pColl)
{
	if (m_BossCollFile.bSuccess == true)
	{ // ボスの当たり判定が読み込めた場合

		for (int nCntPart = 0; nCntPart < m_BossCollFile.nNumData; nCntPart++)
		{
			// 当たり判定を生成
			pColl[nCntPart] = CBossCollision::Create(m_BossCollFile.aFile[nCntPart].nNum);

			for (int nCntColl = 0; nCntColl < m_BossCollFile.aFile[nCntPart].nNum; nCntColl++)
			{
				pColl[nCntPart]->SetCollOffset(m_BossCollFile.aFile[nCntPart].aBase[nCntColl].offset, nCntColl);
				pColl[nCntPart]->SetRadius(m_BossCollFile.aFile[nCntPart].aBase[nCntColl].fRadius, nCntColl);
				pColl[nCntPart]->SetEnableWeakness(m_BossCollFile.aFile[nCntPart].aBase[nCntColl].bWeakness, nCntColl);
			}
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// 看板の設定処理
//===========================================
void CFile::SetSignboard(void)
{
	if (m_SignboardFile.bSuccess == true)
	{ // 看板が読み込めた場合

		for (int nCnt = 0; nCnt < m_SignboardFile.nNumData; nCnt++)
		{
			// 看板を生成する
			CSignboard::Create
			(
				m_SignboardFile.aFile[nCnt].pos,
				m_SignboardFile.aFile[nCnt].rot,
				m_SignboardFile.aFile[nCnt].type
			);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// 風船の設定処理
//===========================================
void CFile::SetBalloon(void)
{
	if (m_BalloonFile.bSuccess == true)
	{ // 看板が読み込めた場合

		for (int nCnt = 0; nCnt < m_BalloonFile.nNumData; nCnt++)
		{
			// 看板を生成する
			CBalloonSpawner::Create(m_BalloonFile.pos[nCnt]);
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//===========================================
// 敵の徘徊経路の設定処理
//===========================================
D3DXVECTOR3 CFile::GetEnemyRoute(const int nRoute, const int nNum)
{
	if (m_EnemyRouteFile.bSuccess == true && 
		m_EnemyRouteFile.nNumData > nRoute &&
		m_EnemyRouteFile.aFile[nRoute].nNum > nNum)
	{ // 情報を読み込んでいた場合

		// 徘徊経路を返す
		return m_EnemyRouteFile.aFile[nRoute].pos[nNum];
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// 緊急座標を返す
		return NONE_D3DXVECTOR3;
	}
}

//===========================================
// 敵の徘徊経路の総数の取得処理
//===========================================
int CFile::GetEnemyRouteNumPos(const int nRoute)
{
	// 総数を返す
	return m_EnemyRouteFile.aFile[nRoute].nNum;
}

//===========================================
// 敵の徘徊経路の総数の取得処理
//===========================================
int CFile::GetEnemyRouteNum(void)
{
	// 総数を返す
	return m_EnemyRouteFile.nNumData;
}

//===========================================
// ファイルの生成処理
//===========================================
CFile* CFile::Create(void)
{
	// ローカルオブジェクトを生成
	CFile* pFile = nullptr;		// ファイルのインスタンスを生成

	if (pFile == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pFile = new CFile;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pFile != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pFile->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ファイルのポインタを返す
	return pFile;
}

//===========================================
// 初期化処理
//===========================================
HRESULT CFile::Init(void)
{
	// 全ての値をクリアする
	for (int nCntRank = 0; nCntRank < CRanking::MAX_RANKING; nCntRank++)
	{
		m_RankingInfo.aRank[nCntRank] = 0;				// ランキングの値
	}

	for (int nCntBoss = 0; nCntBoss < MAX_PARTS; nCntBoss++)
	{
		for (int nCntColl = 0; nCntColl < CBossCollision::MAX_COLLISION; nCntColl++)
		{
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].offset = NONE_D3DXVECTOR3;
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].fRadius = 0.0f;
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].bWeakness = false;
		}

		m_BossCollFile.aFile[nCntBoss].nNum = 0;
	}

	for (int nCntFile = 0; nCntFile < MAX_FILE_DATA; nCntFile++)
	{
		m_EnemyFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_EnemyFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_EnemyFile.aFile[nCntFile].nType = 0;

		m_CoinFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;

		m_TreeFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_TreeFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_TreeFile.aFile[nCntFile].nType = 0;

		m_RockFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].nType = 0;

		m_BlockFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_BlockFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;

		m_BangFlowerFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_BangFlowerFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;

		m_WallFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_WallFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;
		m_WallFile.aFile[nCntFile].nType = 0;
		m_WallFile.aFile[nCntFile].nRotType = 0;

		m_SignboardFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_SignboardFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_SignboardFile.aFile[nCntFile].type = CSignboard::TYPE::TYPE_JUMP;

		m_BalloonFile.pos[nCntFile] = NONE_D3DXVECTOR3;

		for (int nCntRoute = 0; nCntRoute < MAX_ENEMY_ROUTE; nCntRoute++)
		{
			m_EnemyRouteFile.aFile[nCntFile].pos[nCntRoute] = NONE_D3DXVECTOR3;		// 敵の経路の情報
			m_EnemyRouteFile.aFile[nCntFile].nNum = 0;		// 総数
		}
	}

	for (int nCntBone = 0; nCntBone < GOLDBONE_NUM; nCntBone++)
	{
		m_GoldBoneFile.aFile[nCntBone].pos = NONE_D3DXVECTOR3;
	}

	// 総数をクリアする
	m_EnemyFile.nNumData = 0;			// 敵の情報
	m_CoinFile.nNumData = 0;			// コインの情報
	m_GoldBoneFile.nNumData = 0;		// 金の骨の情報
	m_TreeFile.nNumData = 0;			// 木の情報
	m_RockFile.nNumData = 0;			// 岩の情報
	m_BlockFile.nNumData = 0;			// ブロックの情報
	m_BangFlowerFile.nNumData = 0;		// 爆弾花の情報
	m_WallFile.nNumData = 0;			// 壁の情報
	m_BossCollFile.nNumData = 0;		// ボスの当たり判定の情報
	m_SignboardFile.nNumData = 0;		// 看板の情報
	m_BalloonFile.nNumData = 0;			// 風船の情報
	m_EnemyRouteFile.nNumData = 0;		// 敵の徘徊経路の情報

	// 成功状況をクリアする
	m_RankingInfo.bSuccess = false;		// ランキング
	m_EnemyFile.bSuccess = false;		// 敵の情報
	m_CoinFile.bSuccess = false;		// コインの情報
	m_GoldBoneFile.bSuccess = false;	// 金の骨の情報
	m_TreeFile.bSuccess = false;		// 木の情報
	m_RockFile.bSuccess = false;		// 岩の情報
	m_BlockFile.bSuccess = false;		// ブロックの情報
	m_BangFlowerFile.bSuccess = false;	// 爆弾花の情報
	m_WallFile.bSuccess = false;		// 壁の情報
	m_BossCollFile.bSuccess = false;	// ボスの当たり判定の情報
	m_SignboardFile.bSuccess = false;	// 看板の情報
	m_BalloonFile.bSuccess = false;		// 風船の情報
	m_EnemyRouteFile.bSuccess = false;	// 敵の徘徊経路の情報

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CFile::Uninit(void)
{
	// メモリを開放する
	delete this;
}

//===========================================
// ランキングのセーブ処理
//===========================================
HRESULT CFile::SaveRanking(void)
{
	FILE* pFile;												// ファイルポインタを宣言
	m_RankingInfo.bSuccess = false;								// 成功状況

	unsigned char aData[128];		// データ

	int nSize = 0;		// データの総サイズ
	int nCheckSum = 0;	// チェックサム

	// ゼロクリア
	memset(&aData[0], 0, sizeof(aData));

	// aNumの数値をコピーする
	memcpy(&aData[nSize], &m_RankingInfo.aRank, sizeof(m_RankingInfo.aRank));

	// サイズ変数にバイト数分加算する
	nSize += sizeof(m_RankingInfo.aRank);

	// チェックサムを生成する
	for (int nCnt = 0; nCnt < nSize; nCnt++)
	{
		// チェックサムに数値を加算していく
		nCheckSum += aData[nCnt];
	}

	// ファイルを開く
	pFile = fopen(RANKING_BIN, "wb");			// バイナリファイルに書き込むために開く

	// ファイルを比較する
	if (pFile != NULL)
	{ // ファイルが開けた場合

		// 総バイト数を書き込む
		fwrite(&nSize, sizeof(int), 1, pFile);

		// チェックサムを書き込む
		fwrite(&nCheckSum, sizeof(int), 1, pFile);

		// ファイルから数値を書き出す
		fwrite(&m_RankingInfo.aRank[0], sizeof(int), CRanking::MAX_RANKING, pFile);

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_RankingInfo.bSuccess = true;

		// 成功を返す
		return S_OK;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}
}

//===========================================
// ランキングのロード処理
//===========================================
HRESULT CFile::LoadRanking(void)
{
	FILE* pFile;						// ファイルポインタを宣言
	m_RankingInfo.bSuccess = false;		// 成功状況

	int nSize = 0;		// データの総サイズ
	int nCheckSum = 0;	// チェックサム
	int nCheckCnt = 0;	// カウント

	unsigned char aData[128];		// データ

	// ゼロクリア
	memset(&aData[0], 0, sizeof(aData));

	// ファイルを開く
	pFile = fopen(RANKING_BIN, "rb");			// バイナリファイルから読み込むために開く

	// ファイルを比較する
	if (pFile != NULL)
	{ // ファイルが開けた場合

		//ファイルからサイズ数を読み込む
		fread(&nSize, sizeof(int), 1, pFile);

		//ファイルからサムチェックの値を読み込む
		fread(&nCheckSum, sizeof(int), 1, pFile);

		// ファイルから全てのデータを読み込む
		fread(&aData[0], sizeof(unsigned char), nSize, pFile);

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_RankingInfo.bSuccess = true;

		for (int nCnt = 0; nCnt < nSize; nCnt++)
		{
			// カウントを加算する
			nCheckCnt += aData[nCnt];
		}

		if (nCheckSum == nCheckCnt)
		{ // サムチェック用変数と読み込んだデータが一致した場合

			// aNumの数値をコピーする
			memcpy(&m_RankingInfo.aRank, &aData[0], sizeof(m_RankingInfo.aRank));
		}
		else
		{ // サムチェック用変数と読み込んだデータが一致した場合

			// 停止
			assert(false);
		}

		// 成功を返す
		return S_OK;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		for (int nCntRank = 0; nCntRank < CRanking::MAX_RANKING; nCntRank++)
		{
			// 数値を設定する
			m_RankingInfo.aRank[0] = 0;
		}

		// 失敗を返す
		return E_FAIL;
	}
}

//===========================================
// 敵のロード処理
//===========================================
HRESULT CFile::LoadEnemy(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_EnemyFile.nNumData = 0;			// 総数
	m_EnemyFile.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(ENEMY_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_ENEMY") == 0)
			{ // 読み込んだ文字列が SET_ENEMY の場合

				do
				{ // 読み込んだ文字列が END_SET_ENEMY ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].pos.x,
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].pos.y,
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].pos.z);		// 位置を読み込む
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].rot.x,
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].rot.y,
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].rot.z);		// 向きを読み込む
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &m_EnemyFile.aFile[m_EnemyFile.nNumData].nType);		// 種類を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_ENEMY") != 0);		// 読み込んだ文字列が END_SET_ENEMY ではない場合ループ

				// データの総数を増やす
				m_EnemyFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_EnemyFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// コインのロード処理
//===========================================
HRESULT CFile::LoadCoin(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_CoinFile.nNumData = 0;			// 総数
	m_CoinFile.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(COIN_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_COIN") == 0)
			{ // 読み込んだ文字列が SET_COIN の場合

				do
				{ // 読み込んだ文字列が END_SET_COIN ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_CoinFile.aFile[m_CoinFile.nNumData].pos.x,
							&m_CoinFile.aFile[m_CoinFile.nNumData].pos.y,
							&m_CoinFile.aFile[m_CoinFile.nNumData].pos.z);		// 位置を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_COIN") != 0);		// 読み込んだ文字列が END_SET_COIN ではない場合ループ

				// データの総数を増やす
				m_CoinFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_CoinFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 金の骨のロード処理
//===========================================
HRESULT CFile::LoadGoldBone(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_GoldBoneFile.nNumData = 0;		// 総数
	m_GoldBoneFile.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(GOLDBONE_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_GOLDBONE") == 0)
			{ // 読み込んだ文字列が SET_GOLDBONE の場合

				do
				{ // 読み込んだ文字列が END_SET_GOLDBONE ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_GoldBoneFile.aFile[m_GoldBoneFile.nNumData].pos.x,
							&m_GoldBoneFile.aFile[m_GoldBoneFile.nNumData].pos.y,
							&m_GoldBoneFile.aFile[m_GoldBoneFile.nNumData].pos.z);		// 位置を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_GOLDBONE") != 0);		// 読み込んだ文字列が END_SET_GOLDBONE ではない場合ループ

				// データの総数を増やす
				m_GoldBoneFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_GoldBoneFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 木のロード処理
//===========================================
HRESULT CFile::LoadTree(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_TreeFile.nNumData = 0;		// 総数
	m_TreeFile.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(TREE_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_TREE") == 0)
			{ // 読み込んだ文字列が SET_TREE の場合

				do
				{ // 読み込んだ文字列が END_SET_TREE ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_TreeFile.aFile[m_TreeFile.nNumData].pos.x,
							&m_TreeFile.aFile[m_TreeFile.nNumData].pos.y,
							&m_TreeFile.aFile[m_TreeFile.nNumData].pos.z);		// 位置を読み込む
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_TreeFile.aFile[m_TreeFile.nNumData].rot.x,
							&m_TreeFile.aFile[m_TreeFile.nNumData].rot.y,
							&m_TreeFile.aFile[m_TreeFile.nNumData].rot.z);		// 位置を読み込む
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &m_TreeFile.aFile[m_TreeFile.nNumData].nType);		// 種類を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_TREE") != 0);		// 読み込んだ文字列が END_SET_TREE ではない場合ループ

				// データの総数を増やす
				m_TreeFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_TreeFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 岩のロード処理
//===========================================
HRESULT CFile::LoadRock(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_RockFile.nNumData = 0;			// 総数
	m_RockFile.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(ROCK_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_ROCK") == 0)
			{ // 読み込んだ文字列が SET_ROCK の場合

				do
				{ // 読み込んだ文字列が END_SET_ROCK ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_RockFile.aFile[m_RockFile.nNumData].pos.x,
							&m_RockFile.aFile[m_RockFile.nNumData].pos.y,
							&m_RockFile.aFile[m_RockFile.nNumData].pos.z);		// 位置を読み込む
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_RockFile.aFile[m_RockFile.nNumData].rot.x,
							&m_RockFile.aFile[m_RockFile.nNumData].rot.y,
							&m_RockFile.aFile[m_RockFile.nNumData].rot.z);		// 向きを読み込む
					}

					if (strcmp(&aString[0], "SCALE") == 0)
					{ // 読み込んだ文字列が SCALE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_RockFile.aFile[m_RockFile.nNumData].scale.x,
							&m_RockFile.aFile[m_RockFile.nNumData].scale.y,
							&m_RockFile.aFile[m_RockFile.nNumData].scale.z);		// 拡大率を読み込む
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &m_RockFile.aFile[m_RockFile.nNumData].nType);		// 種類を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_ROCK") != 0);		// 読み込んだ文字列が END_SET_ROCK ではない場合ループ

				// データの総数を増やす
				m_RockFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_RockFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

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
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_BlockFile.nNumData = 0;			// 総数
	m_BlockFile.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(BLOCK_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_BLOCK") == 0)
			{ // 読み込んだ文字列が SET_BLOCK の場合

				do
				{ // 読み込んだ文字列が END_SET_BLOCK ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_BlockFile.aFile[m_BlockFile.nNumData].pos.x,
							&m_BlockFile.aFile[m_BlockFile.nNumData].pos.y,
							&m_BlockFile.aFile[m_BlockFile.nNumData].pos.z);		// 位置を読み込む
					}

					if (strcmp(&aString[0], "SCALE") == 0)
					{ // 読み込んだ文字列が SCALE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_BlockFile.aFile[m_BlockFile.nNumData].scale.x,
							&m_BlockFile.aFile[m_BlockFile.nNumData].scale.y,
							&m_BlockFile.aFile[m_BlockFile.nNumData].scale.z);		// 拡大率を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_BLOCK") != 0);		// 読み込んだ文字列が END_SET_BLOCK ではない場合ループ

				// データの総数を増やす
				m_BlockFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_BlockFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 爆弾花のロード処理
//===========================================
HRESULT CFile::LoadBangFlower(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_BangFlowerFile.nNumData = 0;			// 総数
	m_BangFlowerFile.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(BANGFLOWER_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_BANGFLOWER") == 0)
			{ // 読み込んだ文字列が SET_BANGFLOWER の場合

				do
				{ // 読み込んだ文字列が END_SET_BANGFLOWER ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].pos.x,
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].pos.y,
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].pos.z);		// 位置を読み込む
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].rot.x,
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].rot.y,
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].rot.z);		// 向きを読み込む
					}

				} while (strcmp(&aString[0], "END_SET_BANGFLOWER") != 0);		// 読み込んだ文字列が END_SET_BANGFLOWER ではない場合ループ

				// データの総数を増やす
				m_BangFlowerFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_BangFlowerFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 壁のロード処理
//===========================================
HRESULT CFile::LoadWall(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_WallFile.nNumData = 0;			// 総数
	m_WallFile.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(WALL_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_WALL") == 0)
			{ // 読み込んだ文字列が SET_WALL の場合

				do
				{ // 読み込んだ文字列が END_SET_WALL ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_WallFile.aFile[m_WallFile.nNumData].pos.x,
							&m_WallFile.aFile[m_WallFile.nNumData].pos.y,
							&m_WallFile.aFile[m_WallFile.nNumData].pos.z);		// 位置を読み込む
					}

					if (strcmp(&aString[0], "SCALE") == 0)
					{ // 読み込んだ文字列が SCALE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_WallFile.aFile[m_WallFile.nNumData].scale.x,
							&m_WallFile.aFile[m_WallFile.nNumData].scale.y,
							&m_WallFile.aFile[m_WallFile.nNumData].scale.z);	// 拡大率を読み込む
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &m_WallFile.aFile[m_WallFile.nNumData].nType);		// 種類を読み込む
					}

					if (strcmp(&aString[0], "ROTTYPE") == 0)
					{ // 読み込んだ文字列が ROTTYPE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &m_WallFile.aFile[m_WallFile.nNumData].nRotType);	// 向きの種類を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_WALL") != 0);		// 読み込んだ文字列が END_SET_WALL ではない場合ループ

				// データの総数を増やす
				m_WallFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_WallFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// ボスの当たり判定のロード処理
//===========================================
HRESULT CFile::LoadBossColl(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_BossCollFile.nNumData = 0;		// 総数
	m_BossCollFile.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(BOSSCOLL_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "NUM_MODEL") == 0)
			{ // 読み込んだ文字列が NUM_MODEL の場合

				fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
				fscanf(pFile, "%d", &m_BossCollFile.nNumData);	// 位置を読み込む

				for (int nCnt = 0; nCnt < m_BossCollFile.nNumData; nCnt++)
				{
					// ファイルから文字列を読み込む
					nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

					if (strcmp(&aString[0], "SET_COLL") == 0)
					{ // 読み込んだ文字列が SET_COLL の場合

						do
						{ // 読み込んだ文字列が END_SET_WALL ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_COLL") == 0)
							{ // 読み込んだ文字列が NUM_COLL の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%d", &m_BossCollFile.aFile[nCnt].nNum);		// 球の総数を読み込む

								for (int nCntColl = 0; nCntColl < m_BossCollFile.aFile[nCnt].nNum; nCntColl++)
								{
									fscanf(pFile, "%s", &aString[0]);				// POS を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
									fscanf(pFile, "%f%f%f",
										&m_BossCollFile.aFile[nCnt].aBase[nCntColl].offset.x,
										&m_BossCollFile.aFile[nCnt].aBase[nCntColl].offset.y,
										&m_BossCollFile.aFile[nCnt].aBase[nCntColl].offset.z);		// 位置を読み込む

									fscanf(pFile, "%s", &aString[0]);				// RADIUS を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
									fscanf(pFile, "%f", &m_BossCollFile.aFile[nCnt].aBase[nCntColl].fRadius);		// 半径を読み込む

									fscanf(pFile, "%s", &aString[0]);				// WEAK を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);				// 弱点状況を読み込む

									if (strcmp(&aString[0], "TRUE") == 0)
									{ // 弱点状況が true の場合

										m_BossCollFile.aFile[nCnt].aBase[nCntColl].bWeakness = true;
									}
									else
									{ // 上記以外

										m_BossCollFile.aFile[nCnt].aBase[nCntColl].bWeakness = false;
									}
								}
							}
						} while (strcmp(&aString[0], "END_SET_COLL") != 0);		// 読み込んだ文字列が END_SET_WALL ではない場合ループ
					}
				}
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_BossCollFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 看板のロード処理
//===========================================
HRESULT CFile::LoadSignboard(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_SignboardFile.nNumData = 0;		// 総数
	m_SignboardFile.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(SIGNBOARD_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_SIGNBOARD") == 0)
			{ // 読み込んだ文字列が SET_SIGNBOARD の場合

				do
				{ // 読み込んだ文字列が END_SET_SIGNBOARD ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].pos.x,
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].pos.y,
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].pos.z);		// 位置を読み込む
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].rot.x,
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].rot.y,
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].rot.z);	// 拡大率を読み込む
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &m_SignboardFile.aFile[m_SignboardFile.nNumData].type);		// 種類を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_SIGNBOARD") != 0);		// 読み込んだ文字列が END_SET_SIGNBOARD ではない場合ループ

				// データの総数を増やす
				m_SignboardFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_SignboardFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 風船のロード処理
//===========================================
HRESULT CFile::LoadBalloon(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_BalloonFile.nNumData = 0;			// 総数
	m_BalloonFile.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(BALLOON_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_BALLOON") == 0)
			{ // 読み込んだ文字列が SET_BALLOON の場合

				do
				{ // 読み込んだ文字列が END_SET_BALLOON ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_BalloonFile.pos[m_BalloonFile.nNumData].x,
							&m_BalloonFile.pos[m_BalloonFile.nNumData].y,
							&m_BalloonFile.pos[m_BalloonFile.nNumData].z);		// 位置を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_BALLOON") != 0);		// 読み込んだ文字列が END_SET_BALLOON ではない場合ループ

				// データの総数を増やす
				m_BalloonFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_BalloonFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 敵の経路のロード処理
//===========================================
HRESULT CFile::LoadEnemyRoute(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_EnemyRouteFile.nNumData = 0;		// 総数
	m_EnemyRouteFile.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE* pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(ENEMYROUTE_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_ENEMYROUTE") == 0)
			{ // 読み込んだ文字列が SET_ENEMYROUTE の場合

				do
				{ // 読み込んだ文字列が END_SET_ENEMYROUTE ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM") == 0)
					{ // 読み込んだ文字列が NUM の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].nNum);	// 位置の総数を読み込む

						for (int nCntRoute = 0; nCntRoute < m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].nNum; nCntRoute++)
						{
							fscanf(pFile, "%s", &aString[0]);				// POS を読み込む (不要)
							fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
							fscanf(pFile, "%f%f%f",
								&m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].pos[nCntRoute].x,
								&m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].pos[nCntRoute].y,
								&m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].pos[nCntRoute].z);	// 位置を読み込む
						}
					}

				} while (strcmp(&aString[0], "END_SET_ENEMYROUTE") != 0);		// 読み込んだ文字列が END_SET_ENEMYROUTE ではない場合ループ

				// データの総数を増やす
				m_EnemyRouteFile.nNumData++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_EnemyRouteFile.bSuccess = true;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}