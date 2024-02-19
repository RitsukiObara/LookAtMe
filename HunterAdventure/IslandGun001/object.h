//============================================
//
// オブジェクトヘッダー[object.h]
// Author：小原立暉
//
//============================================
#ifndef _OBJECT_H_		//このマクロ定義がされていなかったら
#define _OBJECT_H_		//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// クラス(オブジェクトクラス)
//--------------------------------------------
class CObject
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_NONE = 0,			// 無し
		TYPE_TIME,				// 時間
		TYPE_SHADOW,			// 影
		TYPE_SCORE,				// スコア
		TYPE_3DPOLYGON,			// 3Dポリゴン
		TYPE_CHARA,				// キャラクター
		TYPE_MESH,				// メッシュ
		TYPE_FIELD,				// フィールド
		TYPE_LOGOMARK,			// ロゴマーク
		TYPE_LOCUS,				// 残像
		TYPE_EFFECT,			// エフェクト
		TYPE_COLLPOLY,			// 当たり判定ポリゴン
		TYPE_DESTRUCTION,		// 撃破
		TYPE_FRACTION,			// 破片
		TYPE_RIPPLE,			// 波紋
		TYPE_SKYBOX,			// スカイボックス
		TYPE_STUN,				// 気絶演出
		TYPE_ORBIT,				// 軌跡
		TYPE_PUSHTIMING,		// 押すタイミング
		TYPE_DANGERMARK,		// 危険マーク
		TYPE_CONFETTI,			// 紙吹雪
		TYPE_ELEVATION,			// 起伏地面
		TYPE_SCREENEFFECT,		// 画面エフェクト
		TYPE_EFFECTMODEL,		// モデルエフェクト
		TYPE_LOCUSMODEL,		// モデル残像
		TYPE_ANIMREACTION,		// アニメリアクション
		TYPE_PLAYER,			// プレイヤー
		TYPE_BULLET,			// 弾
		TYPE_ENEMY,				// 敵
		TYPE_COIN,				// 小判
		TYPE_BULLETUI,			// 弾丸UI
		TYPE_GOLDBONE,			// 金の骨
		TYPE_TREE,				// 木
		TYPE_PALMFRUIT,			// ヤシの実
		TYPE_ROCK,				// 岩
		TYPE_BLOCK,				// ブロック
		TYPE_GOLDBONEUI,		// 金の骨UI
		TYPE_GRASS,				// 草
		TYPE_LAKE,				// 水場
		TYPE_SLASHRIPPLE,		// 斬撃の波紋
		TYPE_BANGFLOWER,		// 爆弾花
		TYPE_TITLELOGO,			// タイトルロゴ
		TYPE_OCEAN,				// 海
		TYPE_BOMB,				// 爆弾
		TYPE_BOSS,				// ボス
		TYPE_LIFEUI,			// 寿命UI
		TYPE_BOMBEXPLOSION,		// 爆発
		TYPE_GAMESCORE,			// ゲームスコア
		TYPE_WALL,				// 壁
		TYPE_WINDSHOT,			// 風攻撃
		TYPE_FIREWARNING,		// 炎注意
		TYPE_FIRESHOT,			// 炎攻撃
		TYPE_RANKUI,			// 順位UI
		TYPE_AIRPLANE,			// 飛行機
		TYPE_MAX				// この列挙型の総数
	};

	// 列挙型定義(次元)
	enum DIM
	{
		DIM_3D = 0,				// 3Dオブジェクト
		DIM_2D,					// 2Dオブジェクト
		DIM_MAX					// この列挙型の総数
	};

	// 列挙型定義(優先順位)
	enum PRIORITY
	{
		PRIORITY_BG = 0,			// 背景関係
		PRIORITY_BLOCK,				// ブロック・障害物系
		PRIORITY_EFFECT,			// エフェクト系
		PRIORITY_SHADOW,			// 影関係
		PRIORITY_ENTITY,			// 敵・アイテム関係
		PRIORITY_PLAYER,			// プレイヤー関係
		PRIORITY_UI,				// UI関係
		PRIORITY_PAUSE,				// ポーズ(絶対前に出す画面)
		PRIORITY_MAX				// この列挙型の総数
	};

	CObject(TYPE type, DIM dim, PRIORITY priority = PRIORITY_ENTITY);	// オーバーロードコンストラクタ
	virtual ~CObject();											// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) = 0;	// 初期化処理
	virtual void Uninit(void) = 0;	// 終了処理
	virtual void Update(void) = 0;	// 更新処理
	virtual void Draw(void) = 0;	// 描画処理

	// セット・ゲット関係
	static CObject* GetTop(DIM dim, PRIORITY priority);		// オブジェクトの取得処理

	CObject* GetNext(void);				// 次のポインタの取得処理

	void SetType(const TYPE type);		// 種類の設定処理
	TYPE GetType(void) const;			// 種類の取得処理

	// 静的メンバ関数
	static void ReleaseAll(void);		// 全ての破棄処理
	static void UpdateAll(void);		// 全ての更新処理
	static void DrawAll(void);			// 全ての描画処理

	static void AnyAllClear(const TYPE type);		// 汎用的な全消去処理
	static void AnyAllClear(const DIM dim, const PRIORITY priority, const TYPE type);		// 汎用的な全消去処理
	static void DeathDecision(const int dim, const int nCnt);		// 死亡判別処理

protected:		// 自分と派生クラスだけがアクセスできる

	// メンバ関数
	void Release(void);		// 破棄処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Death(void);		// 死亡処理

	// メンバ変数
	CObject* m_pPrev;		// 前のオブジェクトへのポインタ
	CObject* m_pNext;		// 次のオブジェクトへのポインタ
	TYPE m_type;			// 種類
	PRIORITY m_priority;	// オブジェクトの優先順位
	DIM m_dim;				// 次元
	bool m_bDeath;			// 死亡フラグ
	
	// 静的メンバ変数
	static CObject* m_apTop[DIM_MAX][PRIORITY_MAX];		// 先頭のオブジェクトへのポインタ
	static CObject* m_apCur[DIM_MAX][PRIORITY_MAX];		// 最後尾オブジェクトへのポインタ
	static int m_nNumAll;								// オブジェクトの総数
};

#endif