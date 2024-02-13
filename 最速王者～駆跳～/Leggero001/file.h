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
#include "block.h"
#include "obstacle.h"
#include "camera.h"
#include "result.h"
#include "ranking.h"
#include "balloon.h"

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

	// ファイルの種類
	typedef enum
	{
		TYPE_ENEMY = 0,			// 敵の情報
		TYPE_BLOCK,				// ブロックの情報
		TYPE_BGOBJECT,			// 背景オブジェクト
		TYPE_OBSTACLE,			// 障害物
		TYPE_ITEM,				// アイテム
		TYPE_CAMERA,			// カメラ
		TYPE_RESULT,			// リザルト
		TYPE_RANKING,			// ランキング
		TYPE_GAME,				// ゲーム
		TYPE_TUTORIAL,			// チュートリアル
		TYPE_MAX				// この列挙型の総数
	}TYPE;

	// ファイルの種類
	typedef enum
	{
		SCENE_TYPE_GAME = 0,	// ゲーム画面
		SCENE_TYPE_TUTORIAL,	// チュートリアル画面
		SCENE_TYPE_MAX			// この列挙型の総数
	}SCENE_TYPE;

	// ファイルで読み込む基本情報
	typedef struct
	{
		D3DXVECTOR3 pos;					// 位置
		D3DXVECTOR3 rot;					// 向き
		D3DXVECTOR3 scale;					// 拡大率
	}SFileInfo;

	// ブロックファイルで読み込む情報
	typedef struct
	{
		SFileInfo info;						// 基本情報
		CBlock::TYPE type;					// 種類
		D3DXMATERIAL aMat[MAX_MATERIAL];	// マテリアルデータ
		bool bCliffRight;					// 右崖の判定
		bool bCliffLeft;					// 左崖の判定
	}SBlockInfo;

	// ブロックファイルの総合情報
	typedef struct
	{
		SBlockInfo info[MAX_FILE_DATA];		// 情報へのポインタ
		int nNumData;						// データの数
		bool bSuccess;						// 成功状況
	}SBlockFile;

	// 背景ファイルで読み込む情報
	typedef struct
	{
		SFileInfo info;						// 基本情報
		CXFile::TYPE type;					// 種類
		D3DXMATERIAL aMat[MAX_MATERIAL];	// マテリアルデータ
	}SBgObjectInfo;

	// 背景ファイルの総合情報
	typedef struct
	{
		SBgObjectInfo info[MAX_FILE_DATA];	// 情報へのポインタ
		int nNumData;							// データの数
		bool bSuccess;						// 成功状況
	}SBgObjectFile;

	// 障害物ファイルで読み込む情報
	typedef struct
	{
		D3DXVECTOR3 pos;					// 位置
		CObstacle::TYPE type;				// 種類
	}SObstacleInfo;

	// 障害物ファイルの総合情報
	typedef struct
	{
		SObstacleInfo info[MAX_FILE_DATA];	// 情報へのポインタ
		int nNumData;						// データの数
		bool bSuccess;						// 成功状況
	}SObstacleFile;

	// アイテムファイルの総合情報
	typedef struct
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];		// 位置
		int nNumItemData;					// アイテムのデータの数
		int nNumItemGeneData;				// アイテム生成器のデータの数
		bool bSuccess;						// 成功状況
	}SItemFile;

	// カメラファイルで読み込む情報
	typedef struct
	{
		D3DXVECTOR3 pos;					// 位置
		D3DXVECTOR3 size;					// サイズ
		CCamera::TYPE type;					// 種類
	}SCameraInfo;

	// カメラファイルの総合情報
	typedef struct
	{
		SCameraInfo info[MAX_FILE_DATA];	// 位置
		int nNumData;						// データの数
		bool bSuccess;						// 成功状況
	}SCameraFile;

	// リザルトの情報
	typedef struct
	{
		int aRankTime[CResult::RANK_MAX];	// 時間のランクの基準
		int aRankScore[CResult::RANK_MAX];	// スコアのランクの基準
		bool bSuccess;						// 成功状況
	}SResultFile;

	// ランキングの情報
	typedef struct
	{
		int aRank[MAX_RANKING];				// ランキングのデータ
		bool bSuccess;						// 成功状況
	}SRankingFile;

	// ゲームモードの情報
	typedef struct
	{
		D3DXVECTOR3 PlayerPos;				// プレイヤーの初期位置
		D3DXVECTOR3 StartPos;				// プレイヤーのスタート位置
		D3DXVECTOR3 GoalPos;				// ゴールの位置
		D3DXVECTOR3 StageLeftPos;			// ステージの左端の位置
		D3DXVECTOR3 ScorePos;				// スコアの位置
		D3DXVECTOR3 ScoreSize;				// スコアのサイズ
		D3DXVECTOR3 TimePos;				// タイムの位置
		D3DXVECTOR3 TimeSize;				// タイムのサイズ
		bool bSuccess;						// 成功状況
	}SGameFile;

	// 吹き出しの種類ごとのデータ
	typedef struct
	{
		D3DXVECTOR3 size;					// サイズ
		char aTextureName[MAX_STRING];		// テクスチャ名
	}SBalloonData;

	// 吹き出しの情報
	typedef struct
	{
		D3DXVECTOR3 pos;					// 位置
		CBalloon::TYPE type;				// 種類
	}SBalloonInfo;

	// 吹き出しの総合情報
	typedef struct
	{
		SBalloonInfo info[MAX_FILE_DATA];	// 情報へのポインタ
		int nNumData;						// データの数
	}SBalloonFile;

	// チュートリアルの情報
	typedef struct
	{
		SBalloonFile BalloonInfo;						// 吹き出しの情報
		SBlockFile BlockInfo;							// ブロックの情報
		SBgObjectFile BgObjInfo;						// 背景のオブジェクトの情報
		SObstacleFile ObstaInfo;						// 障害物の情報
		D3DXVECTOR3 PlayerPos;							// プレイヤーの初期位置
		D3DXVECTOR3 GoalPos;							// ゴールの位置
		bool bSuccess;									// 成功状況
	}STutorialFile;

	CFile();					// コンストラクタ
	~CFile();					// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	HRESULT Save(TYPE type);	// セーブ処理
	HRESULT Load(TYPE type);	// ロード処理

	// 使用する際に使う関数
	void SetBlock(void);					// ブロックの設置処理
	void SetObstacle(void);					// 障害物の設置処理
	void SetItem(void);						// アイテムの設置処理
	void SetCamera(void);					// カメラの範囲の設置処理
	SResultFile GetResultInfo(void);		// リザルト情報の設置処理
	void SetRankingInfo(int* pRank);		// ランキングの設定処理
	SRankingFile GetRankingInfo(void);		// ランキングの取得処理
	SGameFile GetGameInfo(void);			// ゲーム画面の取得処理
	void SetTutorial(void);					// チュートリアルの設定処理
	SBalloonData GetBalloon(const CBalloon::TYPE type);			// 吹き出しの情報の取得処理

private:

	// メンバ関数
	HRESULT LoadEnemy(void);		// 敵のロード処理

	HRESULT SaveBlock(void);		// ブロックのセーブ処理
	HRESULT LoadBlock(void);		// ブロックのロード処理

	void LoadBlockProcess(FILE *pFile, SBlockFile *pInfo);			// ブロックのロード処理の中身

	HRESULT SaveBgObject(void);		// 背景オブジェクトのセーブ処理
	HRESULT LoadBgObject(void);		// 背景オブジェクトのロード処理

	HRESULT SaveObstacle(void);		// 障害物のセーブ処理
	HRESULT LoadObstacle(void);		// 障害物のロード処理

	void LoadObstacleProcess(FILE *pFile, SObstacleFile *pInfo);	// 障害物のロード処理の中身

	HRESULT SaveItem(void);			// アイテムのセーブ処理
	HRESULT LoadItem(void);			// アイテムのロード処理

	HRESULT SaveCameraRange(void);	// カメラの範囲のセーブ処理
	HRESULT LoadCameraRange(void);	// カメラの範囲のロード処理

	HRESULT LoadResultFile(void);	// リザルトのロード処理

	HRESULT SaveRankingFile(void);	// ランキングのセーブ処理
	HRESULT LoadRankingFile(void);	// ランキングのロード処理

	HRESULT LoadGameFile(void);		// ゲームのロード処理

	HRESULT LoadTutorialFile(void);	// チュートリアルのロード処理

	void LoadBalloonProcess(FILE *pFile, SBalloonFile *pInfo);			// 吹き出しのロード処理の中身

	// メンバ変数
	SBlockFile m_BlockInfo;			// ブロックの総合情報
	SObstacleFile m_ObstacleInfo;	// 障害物の総合情報
	SItemFile m_ItemInfo;			// アイテムの総合情報
	SCameraFile m_CameraInfo;		// カメラの総合情報
	SResultFile m_ResultInfo;		// リザルトの総合情報
	SRankingFile m_RankingInfo;		// ランキングの総合情報
	SGameFile m_GameInfo;			// ゲームの総合情報
	SBalloonData m_aBalloonData[CBalloon::TYPE_MAX];		// 吹き出しの種類ごとの情報
	STutorialFile m_TutorialInfo;	// チュートリアルの総合情報

	// 静的メンバ変数
	static const char* c_apFileName[TYPE_MAX];		// ファイルの名前
	static const char* c_apBooleanDisp[2];			// bool型の表示
};

#endif