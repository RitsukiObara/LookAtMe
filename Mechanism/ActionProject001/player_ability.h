//============================================
//
// プレイヤーの能力ヘッダー[player_ability.h]
// Author：小原立暉
//
//============================================
#ifndef _PLAYER_ABILITY_H_					// このマクロ定義がされていなかったら
#define _PLAYER_ABILITY_H_					// 2重インクルード防止のマクロを定義する

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CPlayer;			// プレイヤー
class CElevation;		// 起伏地面

//--------------------------------------------
// クラス(プレイヤーの能力クラス)
//--------------------------------------------
class CAbility
{
public:			// 誰でもアクセスできる

	// 列挙型定義(能力)
	enum ABILITY
	{
		ABILITY_NONE = 0,		// 無状態
		ABILITY_HOVER,			// ホバージェット
		ABILITY_JETDASH,		// ジェットダッシュ
		ABILITY_DASHJUMP,		// ダッシュジャンプ
		ABILITY_GROUNDQUAKE,	// グラウンドクエイク
		ABILITY_MAX				// この列挙型の総数
	};

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_HOVER = 0,			// ホバージェット
		TYPE_JETDASH,			// ジェットダッシュ
		TYPE_GROUNDQUAKE,		// グラウンドクエイク
		TYPE_MAX				// この列挙型の総数
	};

	CAbility();				// コンストラクタ
	~CAbility();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(CPlayer& player);		// 更新処理

	void SetData(void);		// 情報の設定処理

	// セット・ゲット関数
	void SetAbility(const ABILITY ability, CPlayer& player);		// 能力の設定処理
	ABILITY GetAbility(void) const;				// 能力の取得処理
	void ResetAbility(void);					// 能力のリセット処理
	void SetPossible(const TYPE type, const bool bPossible);		// 可能状況の設定処理
	bool GetPossible(const TYPE type);								// 可能状況の取得処理

	// 静的メンバ関数
	static CAbility* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Ability(CPlayer& player);		// 能力操作処理
	void HoverJet(CPlayer& player);		// ホバージェット処理
	void SkyDash(CPlayer& player);		// ジェットダッシュ処理
	void DashJump(CPlayer& player);		// ダッシュジャンプ処理
	void GroundQuake(CPlayer& player);	// グラウンドクエイク処理

	void PossibleProcess(CPlayer& player);			// 可能状況判定処理
	
	void BlockBreak(CPlayer& player);				// ブロックの破壊処理
	void SearchVertex(const D3DXVECTOR3& pos);		// 起伏地面の頂点の探索処理

	// メンバ変数
	CElevation *m_pElev;				// 起伏地面のポインタ

	ABILITY m_ability;					// 能力
	int m_aAblCount[TYPE_MAX];			// 能力カウント
	bool m_aPossible[TYPE_MAX];			// 能力の使用状況
};

#endif