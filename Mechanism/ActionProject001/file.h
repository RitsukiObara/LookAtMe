//============================================
//
// ファイルヘッダー[file.h]
// Author：小原立暉
//
//============================================
#ifndef _FILE_H_			//このマクロ定義がされていなかったら
#define _FILE_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"
#include "enemy.h"
#include "block.h"
#include "ranking.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_FILE_DATA		(512)		// ファイルのデータの最大数

//--------------------------------------------
// クラス(ファイル読み込みクラス)
//--------------------------------------------
class CFile
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_ITEM = 0,		// アイテム
		TYPE_MACCHINA,		// マキナ草
		TYPE_ENEMY,			// 敵
		TYPE_TABLE,			// 台
		TYPE_AIRPLANE,		// 飛行機
		TYPE_NEEDLE,		// 棘
		TYPE_BLOCK,			// ブロック
		TYPE_RANKING,		// ランキング
		TYPE_MAX			// この列挙型の総数
	};

	// 構造体定義(アイテムの情報)
	struct SItemInfo
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];		// 位置
		int nNum;							// 総数
		bool bSuccess;						// 成功状況
	};

	// 構造体定義(マキナ草の情報)
	struct SMacchinaInfo
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];		// 位置
		int nNum;							// 総数
		bool bSuccess;						// 成功状況
	};

	// 構造体定義(敵の情報)
	struct SEnemyInfo
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];		// 位置
		CEnemy::TYPE type[MAX_FILE_DATA];	// 種類
		int nNum;							// 総数
		bool bSuccess;						// 成功状況
	};

	// 構造体定義(台の情報)
	struct STableInfo
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];		// 位置
		int nNum;							// 総数
		bool bSuccess;						// 成功状況
	};

	// 構造体定義(飛行機の情報)
	struct SAirplaneInfo
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];		// 位置
		bool bFront[MAX_FILE_DATA];			// 前後状況
		int nNum;							// 総数
		bool bSuccess;						// 成功状況
	};

	// 構造体定義(棘の情報)
	struct SNeedleInfo
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];		// 位置
		D3DXVECTOR3 rot[MAX_FILE_DATA];		// 向き
		int nNum;							// 総数
		bool bSuccess;						// 成功状況
	};

	// 列挙型定義(ブロックの情報)
	struct SBlockInfo
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];				// 位置
		D3DXVECTOR3 rot[MAX_FILE_DATA];				// 向き
		D3DXVECTOR3 scale[MAX_FILE_DATA];			// 拡大率
		CBlock::TYPE type[MAX_FILE_DATA];			// 種類
		CBlock::DROPTYPE dropType[MAX_FILE_DATA];	// ドロップの種類
		int nNum;									// 総数
		bool bSuccess;								// 成功状況
	};

	// ランキングの情報
	struct SRankingInfo
	{
		int aRank[MAX_RANKING];				// ランキングのデータ
		bool bSuccess;						// 成功状況
	};

	CFile();					// コンストラクタ
	~CFile();					// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理

	HRESULT Save(const TYPE type);	// セーブ処理
	HRESULT Load(const TYPE type);	// ロード処理

	void SetRankingInfo(int* pRank);		// ランキングの設定処理
	SRankingInfo GetRankingInfo(void);		// ランキングの取得処理

private:		// 自分のみアクセスできる

	// メンバ関数(セーブ関係)
	HRESULT SaveItem(void);			// アイテムのセーブ処理
	HRESULT SaveMacchina(void);		// マキナ草のセーブ処理
	HRESULT SaveEnemy(void);		// 敵のセーブ処理
	HRESULT SaveTable(void);		// 台のセーブ処理
	HRESULT SaveAirplane(void);		// 飛行機のセーブ処理
	HRESULT SaveNeedle(void);		// 棘のセーブ処理
	HRESULT SaveBlock(void);		// ブロックのセーブ処理
	HRESULT SaveRanking(void);		// ランキングのセーブ処理

	// メンバ関数(ロード関係)
	HRESULT LoadItem(void);			// アイテムのロード処理
	HRESULT LoadMacchina(void);		// マキナ草のロード処理
	HRESULT LoadEnemy(void);		// 敵のロード処理
	HRESULT LoadTable(void);		// 台のロード処理
	HRESULT LoadAirplane(void);		// 飛行機のロード処理
	HRESULT LoadNeedle(void);		// 棘のロード処理
	HRESULT LoadBlock(void);		// ブロックのロード処理
	HRESULT LoadRanking(void);		// ランキングのロード処理

	// メンバ変数
	SItemInfo m_ItemInfo;			// アイテムの情報
	SMacchinaInfo m_MacchinaInfo;	// マキナ草の情報
	SEnemyInfo m_EnemyInfo;			// 敵の情報
	STableInfo m_TableInfo;			// 台の情報
	SAirplaneInfo m_AirplaneInfo;	// 飛行機の情報
	SNeedleInfo m_NeedleInfo;		// 棘の情報
	SBlockInfo m_BlockInfo;			// ブロックの情報
	SRankingInfo m_RankingInfo;		// ランキングの情報

	// 静的メンバ変数
	static const char* c_apBooleanDisp[2];			// bool型の表示
};

#endif