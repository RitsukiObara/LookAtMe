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

#include "screw.h"
#include "screw_manager.h"
#include "macchina.h"
#include "macchina_manager.h"
#include "enemy_manager.h"
#include "table.h"
#include "table_manager.h"
#include "airplane.h"
#include "airplane_manager.h"
#include "needle.h"
#include "needle_manager.h"
#include "block_manager.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define ITEM_TXT			"data\\TXT\\Screw.txt"			// アイテム(ネジ)のテキスト
#define MACCHINA_TXT		"data\\TXT\\Macchina.txt"		// マキナ草のテキスト
#define ENEMY_TXT			"data\\TXT\\Enemy.txt"			// 敵のテキスト
#define TABLE_TXT			"data\\TXT\\Table.txt"			// 台のテキスト
#define AIRPLANE_TXT		"data\\TXT\\Airplane.txt"		// 飛行機のテキスト
#define NEEDLE_TXT			"data\\TXT\\Needle.txt"			// 棘のテキスト
#define BLOCK_TXT			"data\\TXT\\Block.txt"			// ブロックのテキスト
#define RANKING_BIN			"data\\BIN\\Ranking.bin"		// ランキングのテキスト

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
	for (int nCnt = 0; nCnt < MAX_FILE_DATA; nCnt++)
	{
		m_ItemInfo.pos[nCnt] = NONE_D3DXVECTOR3;		// アイテムの位置
		m_MacchinaInfo.pos[nCnt] = NONE_D3DXVECTOR3;	// マキナ草の位置
		m_EnemyInfo.pos[nCnt] = NONE_D3DXVECTOR3;		// 敵の位置
		m_EnemyInfo.type[nCnt] = CEnemy::TYPE_ITOCAN;	// 敵の種類
		m_TableInfo.pos[nCnt] = NONE_D3DXVECTOR3;		// 台の位置
		m_AirplaneInfo.pos[nCnt] = NONE_D3DXVECTOR3;	// 飛行機の位置
		m_AirplaneInfo.bFront[nCnt] = false;			// 飛行機の向き
		m_NeedleInfo.pos[nCnt] = NONE_D3DXVECTOR3;		// 棘の位置
		m_NeedleInfo.rot[nCnt] = NONE_D3DXVECTOR3;		// 棘の向き
	}

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		m_RankingInfo.aRank[nCntRank] = 0;				// ランキングの値
	}

	// 総数をクリアする
	m_ItemInfo.nNum = 0;				// アイテム
	m_MacchinaInfo.nNum = 0;			// マキナ草
	m_EnemyInfo.nNum = 0;				// 敵
	m_TableInfo.nNum = 0;				// 台
	m_AirplaneInfo.nNum = 0;			// 飛行機
	m_NeedleInfo.nNum = 0;				// 棘

	// 成功状況をクリアする
	m_ItemInfo.bSuccess = false;		// アイテム
	m_MacchinaInfo.bSuccess = false;	// マキナ草
	m_EnemyInfo.bSuccess = false;		// 敵
	m_TableInfo.bSuccess = false;		// 台
	m_AirplaneInfo.bSuccess = false;	// 飛行機
	m_NeedleInfo.bSuccess = false;		// 棘
	m_RankingInfo.bSuccess = false;		// ランキング
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
	case TYPE_ITEM:			// アイテム

		// アイテムのセーブ処理
		if (FAILED(SaveItem()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_MACCHINA:		// マキナ草

		// マキナ草のセーブ処理
		if (FAILED(SaveMacchina()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_ENEMY:		// 敵

		// 敵のセーブ処理
		if(FAILED(SaveEnemy()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_TABLE:		// 台

		// テーブルのセーブ処理
		if(FAILED(SaveTable()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_AIRPLANE:		// 飛行機

		// 飛行機のセーブ処理
		if (FAILED(SaveAirplane()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_NEEDLE:		// 棘

		// 棘のセーブ処理
		if (FAILED(SaveNeedle()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_BLOCK:		// ブロック

		// ブロックのセーブ処理
		if (FAILED(SaveBlock()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	case TYPE_RANKING:

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
	case TYPE_ITEM:			// アイテム

		// アイテムのロード処理
		if(FAILED(LoadItem()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		if (m_ItemInfo.bSuccess == true)
		{ // 読み込みに成功していた場合

			for (int nCnt = 0; nCnt < m_ItemInfo.nNum; nCnt++)
			{
				// ネジの生成処理
				CScrew::Create(m_ItemInfo.pos[nCnt], NONE_D3DXVECTOR3, false);
			}
		}

		break;

	case TYPE_MACCHINA:		// マキナ草

		// マキナ草のロード処理
		if(FAILED(LoadMacchina()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		if (m_MacchinaInfo.bSuccess == true)
		{ // 読み込みに成功していた場合

			for (int nCnt = 0; nCnt < m_MacchinaInfo.nNum; nCnt++)
			{
				// マキナ草の生成処理
				CMacchina::Create(m_MacchinaInfo.pos[nCnt]);
			}
		}

		break;

	case TYPE_ENEMY:		// 敵

		// 敵のロード処理
		if(FAILED(LoadEnemy()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		if (m_EnemyInfo.bSuccess == true)
		{ // 読み込みに成功していた場合

			for (int nCnt = 0; nCnt < m_EnemyInfo.nNum; nCnt++)
			{
				// 敵の生成処理
				CEnemy::Create(m_EnemyInfo.pos[nCnt], m_EnemyInfo.type[nCnt]);
			}
		}

		break;

	case TYPE_TABLE:		// 台

		// 台のロード処理
		if(FAILED(LoadTable()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		if (m_TableInfo.bSuccess == true)
		{ // 読み込みに成功していた場合

			for (int nCnt = 0; nCnt < m_TableInfo.nNum; nCnt++)
			{
				// 台の生成処理
				CTable::Create(m_TableInfo.pos[nCnt]);
			}
		}

		break;

	case TYPE_AIRPLANE:		// 飛行機

		// 飛行機のロード処理
		if (FAILED(LoadAirplane()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		if (m_AirplaneInfo.bSuccess == true)
		{ // 読み込みに成功していた場合

			for (int nCnt = 0; nCnt < m_AirplaneInfo.nNum; nCnt++)
			{
				// 飛行機の生成処理
				CAirplane::Create(m_AirplaneInfo.pos[nCnt], m_AirplaneInfo.bFront[nCnt], CAirplane::STATE_NONE);
			}
		}

		break;

	case TYPE_NEEDLE:		// 棘

		// 棘のロード処理
		if (FAILED(LoadNeedle()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		if (m_NeedleInfo.bSuccess == true)
		{ // 読み込みに成功していた場合

			for (int nCnt = 0; nCnt < m_NeedleInfo.nNum; nCnt++)
			{
				// 棘の生成処理
				CNeedle::Create(m_NeedleInfo.pos[nCnt], m_NeedleInfo.rot[nCnt]);
			}
		}

		break;

	case TYPE_BLOCK:		// ブロック

		// ブロックのロード処理
		if (FAILED(LoadBlock()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		if (m_BlockInfo.bSuccess == true)
		{ // 読み込みに成功していた場合

			for (int nCnt = 0; nCnt < m_BlockInfo.nNum; nCnt++)
			{
				// ブロックの生成処理
				CBlock::Create(m_BlockInfo.pos[nCnt], m_BlockInfo.rot[nCnt], m_BlockInfo.scale[nCnt], m_BlockInfo.type[nCnt], m_BlockInfo.dropType[nCnt]);
			}
		}

		break;

	case TYPE_RANKING:

		// ランキングのロード処理
		if (FAILED(LoadRanking()))
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
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++, pRank++)
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
// 初期化処理
//===========================================
HRESULT CFile::Init(void)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < MAX_FILE_DATA; nCnt++)
	{
		m_ItemInfo.pos[nCnt] = NONE_D3DXVECTOR3;		// アイテムの位置
		m_MacchinaInfo.pos[nCnt] = NONE_D3DXVECTOR3;	// マキナ草の位置
		m_EnemyInfo.pos[nCnt] = NONE_D3DXVECTOR3;		// 敵の位置
		m_EnemyInfo.type[nCnt] = CEnemy::TYPE_ITOCAN;	// 敵の種類
		m_TableInfo.pos[nCnt] = NONE_D3DXVECTOR3;		// 台の位置
		m_AirplaneInfo.pos[nCnt] = NONE_D3DXVECTOR3;	// 飛行機の位置
		m_AirplaneInfo.bFront[nCnt] = false;			// 飛行機の向き
	}

	// 総数をクリアする
	m_ItemInfo.nNum = 0;				// アイテム
	m_MacchinaInfo.nNum = 0;			// マキナ草
	m_EnemyInfo.nNum = 0;				// 敵
	m_TableInfo.nNum = 0;				// 台
	m_AirplaneInfo.nNum = 0;			// 飛行機

	// 成功状況をクリアする
	m_ItemInfo.bSuccess = false;		// アイテム
	m_MacchinaInfo.bSuccess = false;	// マキナ草
	m_EnemyInfo.bSuccess = false;		// 敵
	m_TableInfo.bSuccess = false;		// 台
	m_AirplaneInfo.bSuccess = false;	// 飛行機

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CFile::Uninit(void)
{
	
}

//===========================================
// アイテムのセーブ処理
//===========================================
HRESULT CFile::SaveItem(void)
{
	// ローカル変数宣言
	CScrew* pScrew = CScrewManager::Get()->GetTop();		// 先頭のネジを代入する

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(ITEM_TXT, "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		while (pScrew != nullptr)
		{ // オブジェクトへのポインタが NULL じゃなかった場合

			// 文字列を書き込む
			fprintf(pFile, "SET_ITEM\n");		// ネジの設定を書き込む

			fprintf(pFile, "\tPOS = ");			// 位置の設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pScrew->GetPos().x, pScrew->GetPos().y, pScrew->GetPos().z);			// 位置を書き込む

			// 文字列を書き込む
			fprintf(pFile, "END_SET_ITEM\n\n");			// ネジの設定の終了を書き込む

			// 次のオブジェクトを代入する
			pScrew = pScrew->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
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
// マキナ草のセーブ処理
//===========================================
HRESULT CFile::SaveMacchina(void)
{
	// ローカル変数宣言
	CMacchina* pScrew = CMacchinaManager::Get()->GetTop();		// 先頭のマキナ草を代入する

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(MACCHINA_TXT, "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		while (pScrew != nullptr)
		{ // オブジェクトへのポインタが NULL じゃなかった場合

			// 文字列を書き込む
			fprintf(pFile, "SET_MACCHINA\n");	// マキナ草の設定を書き込む

			fprintf(pFile, "\tPOS = ");			// 位置の設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pScrew->GetPos().x, pScrew->GetPos().y, pScrew->GetPos().z);			// 位置を書き込む

			// 文字列を書き込む
			fprintf(pFile, "END_SET_MACCHINA\n\n");		// マキナ草の設定の終了を書き込む

			// 次のオブジェクトを代入する
			pScrew = pScrew->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
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
// 敵のセーブ処理
//===========================================
HRESULT CFile::SaveEnemy(void)
{
	// ローカル変数宣言
	CEnemy* pEnemy = CEnemyManager::Get()->GetTop();		// 先頭の敵を代入する

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(ENEMY_TXT, "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		while (pEnemy != nullptr)
		{ // オブジェクトへのポインタが NULL じゃなかった場合

			// 文字列を書き込む
			fprintf(pFile, "SET_ENEMY\n");		// 敵の設定を書き込む

			fprintf(pFile, "\tPOS = ");			// 位置の設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pEnemy->GetPos().x, pEnemy->GetPos().y, pEnemy->GetPos().z);			// 位置を書き込む

			fprintf(pFile, "\tTYPE = ");		// 種類の設定を書き込む
			fprintf(pFile, "%d\n", pEnemy->GetType());			// 種類を書き込む

			// 文字列を書き込む
			fprintf(pFile, "END_SET_ENEMY\n\n");		// 敵の設定の終了を書き込む

			// 次のオブジェクトを代入する
			pEnemy = pEnemy->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
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
// 台のセーブ処理
//===========================================
HRESULT CFile::SaveTable(void)
{
	// ローカル変数宣言
	CTable* pTable = CTableManager::Get()->GetTop();		// 先頭の台を代入する

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(TABLE_TXT, "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		while (pTable != nullptr)
		{ // オブジェクトへのポインタが NULL じゃなかった場合

			// 文字列を書き込む
			fprintf(pFile, "SET_TABLE\n");		// 台の設定を書き込む

			fprintf(pFile, "\tPOS = ");			// 位置の設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pTable->GetPos().x, pTable->GetPos().y, pTable->GetPos().z);			// 位置を書き込む

			// 文字列を書き込む
			fprintf(pFile, "END_SET_TABLE\n\n");	// 台の設定の終了を書き込む

			// 次のオブジェクトを代入する
			pTable = pTable->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
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
// 飛行機のセーブ処理
//===========================================
HRESULT CFile::SaveAirplane(void)
{
	// ローカル変数宣言
	CAirplane* pAirplane = CAirplaneManager::Get()->GetTop();		// 先頭の飛行機を代入する

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(AIRPLANE_TXT, "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		while (pAirplane != nullptr)
		{ // オブジェクトへのポインタが NULL じゃなかった場合

			// 文字列を書き込む
			fprintf(pFile, "SET_AIRPLANE\n");	// 飛行機の設定を書き込む

			fprintf(pFile, "\tPOS = ");			// 位置の設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pAirplane->GetPos().x, pAirplane->GetPos().y, pAirplane->GetPos().z);			// 位置を書き込む

			fprintf(pFile, "\tFRONT = ");		// 前後状況の設定を書き込む
			fprintf(pFile, "%s\n", c_apBooleanDisp[(int)(pAirplane->GetFront())]);		// 前後状況を書き込む

			// 文字列を書き込む
			fprintf(pFile, "END_SET_AIRPLANE\n\n");		// 飛行機の設定の終了を書き込む

			// 次のオブジェクトを代入する
			pAirplane = pAirplane->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
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
// 棘のセーブ処理
//===========================================
HRESULT CFile::SaveNeedle(void)
{
	// ローカル変数宣言
	CNeedle* pNeedle = CNeedleManager::Get()->GetTop();		// 先頭の棘を代入する

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(NEEDLE_TXT, "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		while (pNeedle != nullptr)
		{ // オブジェクトへのポインタが NULL じゃなかった場合

			// 文字列を書き込む
			fprintf(pFile, "SET_NEEDLE\n");			// 棘の設定を書き込む

			fprintf(pFile, "\tPOS = ");				// 位置の設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pNeedle->GetPos().x, pNeedle->GetPos().y, pNeedle->GetPos().z);		// 位置を書き込む

			fprintf(pFile, "\tROT = ");				// 向きの設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pNeedle->GetRot().x, pNeedle->GetRot().y, pNeedle->GetRot().z);		// 向きを書き込む

			// 文字列を書き込む
			fprintf(pFile, "END_SET_NEEDLE\n\n");	// 棘の設定の終了を書き込む

			// 次のオブジェクトを代入する
			pNeedle = pNeedle->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
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
// ブロックのセーブ処理
//===========================================
HRESULT CFile::SaveBlock(void)
{
	// ローカル変数宣言
	CBlock* pBlock = CBlockManager::Get()->GetTop();		// 先頭のブロックを代入する

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(BLOCK_TXT, "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		while (pBlock != nullptr)
		{ // オブジェクトへのポインタが NULL じゃなかった場合

			// 文字列を書き込む
			fprintf(pFile, "SET_BLOCK\n");			// ブロックの設定を書き込む

			fprintf(pFile, "\tPOS = ");				// 位置の設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetPos().x, pBlock->GetPos().y, pBlock->GetPos().z);			// 位置を書き込む

			fprintf(pFile, "\tROT = ");				// 向きの設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetRot().x, pBlock->GetRot().y, pBlock->GetRot().z);			// 向きを書き込む

			fprintf(pFile, "\tSCALE = ");			// 拡大率の設定を書き込む
			fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetScale().x, pBlock->GetScale().y, pBlock->GetScale().z);	// 拡大率を書き込む

			fprintf(pFile, "\tTYPE = ");					// 種類の設定を書き込む
			fprintf(pFile, "%d\n", pBlock->GetType());		// 種類を書き込む

			fprintf(pFile, "\tDROPTYPE = ");				// ドロップの種類の設定を書き込む
			fprintf(pFile, "%d\n", pBlock->GetDropType());	// ドロップの種類を書き込む

			// 文字列を書き込む
			fprintf(pFile, "END_SET_BLOCK\n\n");	// ブロックの設定の終了を書き込む

			// 次のオブジェクトを代入する
			pBlock = pBlock->GetNext();
		}

		// ファイルを閉じる
		fclose(pFile);
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
// ランキングのセーブ処理
//===========================================
HRESULT CFile::SaveRanking(void)
{
	FILE *pFile;												// ファイルポインタを宣言
	m_RankingInfo.bSuccess = false;								// 成功状況

	// ファイルを開く
	pFile = fopen(RANKING_BIN, "wb");			// バイナリファイルに書き込むために開く

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

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}
}

//===========================================
// アイテムのロード処理
//===========================================
HRESULT CFile::LoadItem(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_ItemInfo.nNum = 0;				// 総数
	m_ItemInfo.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(ITEM_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_ITEM") == 0)
			{ // 読み込んだ文字列が SET_ITEM の場合

				do
				{ // 読み込んだ文字列が END_SET_ITEM ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_ItemInfo.pos[m_ItemInfo.nNum].x,
							&m_ItemInfo.pos[m_ItemInfo.nNum].y,
							&m_ItemInfo.pos[m_ItemInfo.nNum].z);		// 位置を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_ITEM") != 0);		// 読み込んだ文字列が END_SET_ITEM ではない場合ループ

				// データの総数を増やす
				m_ItemInfo.nNum++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_ItemInfo.bSuccess = true;
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
// マキナ草のロード処理
//===========================================
HRESULT CFile::LoadMacchina(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_MacchinaInfo.nNum = 0;			// 総数
	m_MacchinaInfo.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(MACCHINA_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_MACCHINA") == 0)
			{ // 読み込んだ文字列が SET_MACCHINA の場合

				do
				{ // 読み込んだ文字列が END_SET_MACCHINA ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_MacchinaInfo.pos[m_MacchinaInfo.nNum].x,
							&m_MacchinaInfo.pos[m_MacchinaInfo.nNum].y,
							&m_MacchinaInfo.pos[m_MacchinaInfo.nNum].z);	// 位置を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_MACCHINA") != 0);		// 読み込んだ文字列が END_SET_MACCHINA ではない場合ループ

				// データの総数を増やす
				m_MacchinaInfo.nNum++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_MacchinaInfo.bSuccess = true;
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
// 敵のロード処理
//===========================================
HRESULT CFile::LoadEnemy(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_EnemyInfo.nNum = 0;				// 総数
	m_EnemyInfo.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

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
							&m_EnemyInfo.pos[m_EnemyInfo.nNum].x,
							&m_EnemyInfo.pos[m_EnemyInfo.nNum].y,
							&m_EnemyInfo.pos[m_EnemyInfo.nNum].z);		// 位置を読み込む
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d",
							&m_EnemyInfo.type[m_EnemyInfo.nNum]);		// 位置を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_ENEMY") != 0);		// 読み込んだ文字列が END_SET_ENEMY ではない場合ループ

				// データの総数を増やす
				m_EnemyInfo.nNum++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_EnemyInfo.bSuccess = true;
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
// 台のロード処理
//===========================================
HRESULT CFile::LoadTable(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_TableInfo.nNum = 0;				// 総数
	m_TableInfo.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(TABLE_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_TABLE") == 0)
			{ // 読み込んだ文字列が SET_TABLE の場合

				do
				{ // 読み込んだ文字列が END_SET_TABLE ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_TableInfo.pos[m_TableInfo.nNum].x,
							&m_TableInfo.pos[m_TableInfo.nNum].y,
							&m_TableInfo.pos[m_TableInfo.nNum].z);		// 位置を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_TABLE") != 0);		// 読み込んだ文字列が END_SET_TABLE ではない場合ループ

				// データの総数を増やす
				m_TableInfo.nNum++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_TableInfo.bSuccess = true;
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
// 飛行機のロード処理
//===========================================
HRESULT CFile::LoadAirplane(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_AirplaneInfo.nNum = 0;			// 総数
	m_AirplaneInfo.bSuccess = false;	// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(AIRPLANE_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_AIRPLANE") == 0)
			{ // 読み込んだ文字列が SET_AIRPLANE の場合

				do
				{ // 読み込んだ文字列が END_SET_AIRPLANE ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_AirplaneInfo.pos[m_AirplaneInfo.nNum].x,
							&m_AirplaneInfo.pos[m_AirplaneInfo.nNum].y,
							&m_AirplaneInfo.pos[m_AirplaneInfo.nNum].z);		// 位置を読み込む
					}
					else if (strcmp(&aString[0], "FRONT") == 0)
					{ // 読み込んだ文字列が FRONT の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%s", &aString[0]);			// 位置を読み込む

						if (strcmp(&aString[0], c_apBooleanDisp[0]) == 0)
						{ // FALSEを読み込んだ場合

							// false を設定する
							m_AirplaneInfo.bFront[m_AirplaneInfo.nNum] = false;
						}
						else
						{ // 上記以外

							// true を設定する
							m_AirplaneInfo.bFront[m_AirplaneInfo.nNum] = true;
						}
					}

				} while (strcmp(&aString[0], "END_SET_AIRPLANE") != 0);		// 読み込んだ文字列が END_SET_AIRPLANE ではない場合ループ

				// データの総数を増やす
				m_AirplaneInfo.nNum++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_AirplaneInfo.bSuccess = true;
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
// 棘のロード処理
//===========================================
HRESULT CFile::LoadNeedle(void)
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	char aString[MAX_STRING];			// テキストの文字列の代入用
	m_NeedleInfo.nNum = 0;				// 総数
	m_NeedleInfo.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(NEEDLE_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_NEEDLE") == 0)
			{ // 読み込んだ文字列が SET_NEEDLE の場合

				do
				{ // 読み込んだ文字列が END_SET_NEEDLE ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_NeedleInfo.pos[m_NeedleInfo.nNum].x,
							&m_NeedleInfo.pos[m_NeedleInfo.nNum].y,
							&m_NeedleInfo.pos[m_NeedleInfo.nNum].z);	// 位置を読み込む
					}
					if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_NeedleInfo.rot[m_NeedleInfo.nNum].x,
							&m_NeedleInfo.rot[m_NeedleInfo.nNum].y,
							&m_NeedleInfo.rot[m_NeedleInfo.nNum].z);	// 向きを読み込む
					}

				} while (strcmp(&aString[0], "END_SET_NEEDLE") != 0);		// 読み込んだ文字列が END_SET_NEEDLE ではない場合ループ

				// データの総数を増やす
				m_NeedleInfo.nNum++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_NeedleInfo.bSuccess = true;
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
	m_BlockInfo.nNum = 0;				// 総数
	m_BlockInfo.bSuccess = false;		// 成功状況

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

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
							&m_BlockInfo.pos[m_BlockInfo.nNum].x,
							&m_BlockInfo.pos[m_BlockInfo.nNum].y,
							&m_BlockInfo.pos[m_BlockInfo.nNum].z);	// 位置を読み込む
					}
					if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_BlockInfo.rot[m_BlockInfo.nNum].x,
							&m_BlockInfo.rot[m_BlockInfo.nNum].y,
							&m_BlockInfo.rot[m_BlockInfo.nNum].z);	// 向きを読み込む
					}
					if (strcmp(&aString[0], "SCALE") == 0)
					{ // 読み込んだ文字列が SCALE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f%f%f",
							&m_BlockInfo.scale[m_BlockInfo.nNum].x,
							&m_BlockInfo.scale[m_BlockInfo.nNum].y,
							&m_BlockInfo.scale[m_BlockInfo.nNum].z);		// 拡大率を読み込む
					}
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d",&m_BlockInfo.type[m_BlockInfo.nNum]);	// 種類を読み込む
					}
					if (strcmp(&aString[0], "DROPTYPE") == 0)
					{ // 読み込んだ文字列が DROPTYPE の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &m_BlockInfo.dropType[m_BlockInfo.nNum]);	// ドロップの種類を読み込む
					}

				} while (strcmp(&aString[0], "END_SET_BLOCK") != 0);		// 読み込んだ文字列が END_SET_BLOCK ではない場合ループ

				// データの総数を増やす
				m_BlockInfo.nNum++;
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_BlockInfo.bSuccess = true;
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
// ランキングのロード処理
//===========================================
HRESULT CFile::LoadRanking(void)
{
	FILE *pFile;						// ファイルポインタを宣言
	m_RankingInfo.bSuccess = false;		// 成功状況

	// ファイルを開く
	pFile = fopen(RANKING_BIN, "rb");			// バイナリファイルから読み込むために開く

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

		// 停止
		assert(false);

		for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			// 数値を設定する
			m_RankingInfo.aRank[0] = 0;
		}

		// 失敗を返す
		return E_FAIL;
	}
}