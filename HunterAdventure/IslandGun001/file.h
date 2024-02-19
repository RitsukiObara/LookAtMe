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
#include "ranking.h"
#include "boss_collision.h"
#include "motion.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_FILE_DATA		(2048)		// ファイルのデータの最大数

//--------------------------------------------
// クラス(ファイル読み込みクラス)
//--------------------------------------------
class CFile
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_RANKING = 0,	// ランキング
		TYPE_ENEMY,			// 敵
		TYPE_COIN,			// コイン
		TYPE_GOLDBONE,		// 金の骨
		TYPE_TREE,			// 木
		TYPE_ROCK,			// 岩
		TYPE_BLOCK,			// ブロック
		TYPE_BANGFLOWER,	// 爆弾花
		TYPE_WALL,			// 壁
		TYPE_BOSSCOLL,		// ボスの当たり判定
		TYPE_MAX			// この列挙型の総数
	};

	// ランキングの情報
	struct SRankingInfo
	{
		int aRank[MAX_RANKING];				// ランキングのデータ
		bool bSuccess;						// 成功状況
	};

	// 敵の情報
	struct SEnemyInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		int nType;				// 種類
	};

	// コインの情報
	struct SCoinInfo
	{
		D3DXVECTOR3 pos;		// 位置
	};

	// 金の骨の情報
	struct SGoldBoneInfo
	{
		D3DXVECTOR3 pos;		// 位置
	};

	// 木の情報
	struct STreeInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		int nType;				// 種類
	};

	// 岩の情報
	struct SRockInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 scale;		// 拡大率
		int nType;				// 種類
	};

	// ブロックの情報
	struct SBlockInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 scale;		// 拡大率
	};

	// 爆弾花の情報
	struct SBangFlowerInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
	};

	// 壁の情報
	struct SWallInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 scale;		// 拡大率
		int nType;				// 種類
		int nRotType;			// 向きの種類
	};

	// ボスの当たり判定の基盤
	struct SBossCollBase
	{
		D3DXVECTOR3 offset;		// オフセット座標
		float fRadius;			// 半径
		bool bWeakness;			// 弱点状況
	};

	// ボスの当たり判定の情報
	struct SBossCollinfo
	{
		SBossCollBase aBase[CBossCollision::MAX_COLLISION];		// 当たり判定
		int nNum;				// 総数
	};

	//************************************************************************************************************************************************
	// 情報構造体
	//************************************************************************************************************************************************
	// 敵のファイル
	struct SEnemyFile
	{
		SEnemyInfo aFile[MAX_FILE_DATA];	// 情報
		int nNumData;						// 総数
		bool bSuccess;						// 成功状況
	};

	// コインのファイル
	struct SCoinFile
	{
		SCoinInfo aFile[MAX_FILE_DATA];		// 情報
		int nNumData;						// 総数
		bool bSuccess;						// 成功状況
	};

	// 金の骨のファイル
	struct SGoldBoneFile
	{
		SGoldBoneInfo aFile[3];				// 情報
		int nNumData;						// 総数
		bool bSuccess;						// 成功状況
	};

	// 木のファイル
	struct STreeFile
	{
		STreeInfo aFile[MAX_FILE_DATA];		// 情報
		int nNumData;						// 総数
		bool bSuccess;						// 成功状況
	};

	// 岩のファイル
	struct SRockFile
	{
		SRockInfo aFile[MAX_FILE_DATA];		// 情報
		int nNumData;						// 総数
		bool bSuccess;						// 成功状況
	};

	// ブロックのファイル
	struct SBlockFile
	{
		SBlockInfo aFile[MAX_FILE_DATA];	// 情報
		int nNumData;						// 総数
		bool bSuccess;						// 成功状況
	};

	// 爆弾花のファイル
	struct SBangFlowerFile
	{
		SBangFlowerInfo aFile[MAX_FILE_DATA];	// 情報
		int nNumData;							// 総数
		bool bSuccess;							// 成功状況
	};

	// 壁のファイル
	struct SWallFile
	{
		SWallInfo aFile[MAX_FILE_DATA];			// 情報
		int nNumData;							// 総数
		bool bSuccess;							// 成功状況
	};

	// ボスの当たり判定のファイル
	struct SBossCollFile
	{
		SBossCollinfo aFile[MAX_PARTS];			// 情報
		int nNumData;							// モデルの総数
		bool bSuccess;							// 成功状況
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

	void SetEnemy(void);		// 敵の設定処理
	void SetCoin(void);			// コインの設定処理
	void SetGoldBone(void);		// 金の骨の設定処理
	void SetTree(void);			// 木の設定処理
	void SetRock(void);			// 岩の設定処理
	void SetBlock(void);		// ブロックの設定処理
	void SetBangFlower(void);	// 爆弾花の設定処理
	void SetWall(void);			// 壁の設定処理
	void SetBossColl(CBossCollision** pColl);		// ボスの当たり判定の設定処理

	// 静的メンバ関数
	static CFile* Create(void);		// 生成処理

private:		// 自分のみアクセスできる

	// メンバ関数(セーブ関係)
	HRESULT SaveRanking(void);		// ランキングのセーブ処理

	// メンバ関数(ロード関係)
	HRESULT LoadRanking(void);		// ランキングのロード処理
	HRESULT LoadEnemy(void);		// 敵のロード処理
	HRESULT LoadCoin(void);			// コインのロード処理
	HRESULT LoadGoldBone(void);		// 金の骨のロード処理
	HRESULT LoadTree(void);			// 木のロード処理
	HRESULT LoadRock(void);			// 岩のロード処理
	HRESULT LoadBlock(void);		// ブロックのロード処理
	HRESULT LoadBangFlower(void);	// 爆弾花のロード処理
	HRESULT LoadWall(void);			// 壁のロード処理
	HRESULT LoadBossColl(void);		// ボスの当たり判定のロード処理

	// メンバ変数
	SRankingInfo m_RankingInfo;			// ランキングの情報
	SEnemyFile m_EnemyFile;				// 敵の情報
	SCoinFile m_CoinFile;				// コインの情報
	SGoldBoneFile m_GoldBoneFile;		// 金の骨の情報
	STreeFile m_TreeFile;				// 木の情報
	SRockFile m_RockFile;				// 岩の情報
	SBlockFile m_BlockFile;				// ブロックの情報
	SBangFlowerFile m_BangFlowerFile;	// 爆弾花の情報
	SWallFile m_WallFile;				// 壁の情報
	SBossCollFile m_BossCollFile;		// ボスの当たり判定の情報

	// 静的メンバ変数
	static const char* c_apBooleanDisp[2];			// bool型の表示
};

#endif