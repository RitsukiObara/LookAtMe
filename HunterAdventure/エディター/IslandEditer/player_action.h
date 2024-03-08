//===================================
//
// プレイヤーの行動ヘッダー[player_action.h]
// Author 小原立暉
//
//===================================
#ifndef _PLAYER_ACTION_H_			// このマクロ定義がされていなかったら
#define _PLAYER_ACTION_H_			// 2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CPlayer;		// プレイヤー

//-----------------------------------
// クラス定義(プレイヤーの行動)
//-----------------------------------
class CPlayerAction
{
public:			// 誰でもアクセスできる

	// 列挙型定義(行動)
	enum ACTION
	{
		ACTION_NONE = 0,	// 通常状態
		ACTION_SHOT,		// 射撃状態
		ACTION_DAGGER,		// ダガー状態
		ACTION_DODGE,		// 回避状態
		ACTION_SHOTGUN,		// 散弾(J+銃)状態
		ACTION_SWOOP,		// 急降下(J+ダガー)状態
		ACTION_RELOAD,		// リロード状態
		ACTION_MAX			// この列挙型の総数
	};

	CPlayerAction();		// コンストラクタ
	~CPlayerAction();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(CPlayer* pPlayer);		// 更新処理

	// セット・ゲット関数
	void SetMoveRecoil(const D3DXVECTOR3& move);	// 移動量の設定処理
	D3DXVECTOR3 GetMoveRecoil(void) const;			// 移動量の取得処理

	void SetAction(const ACTION action);			// 行動の設定処理
	ACTION GetAction(void) const;					// 行動の取得処理

	void SetDodgeRot(const float fRot);				// 回避する向きの設定処理

	void SetDodgeInterval(const int nInterval);		// 回避インターバルの設定処理

	void SetEnableDodgeUse(const bool bUse);		// 回避使用可能状態の設定処理
	bool IsDodgeUse(void) const;					// 回避使用可能状況の取得処理

	void SetEnableRecoil(const bool bRecoil);		// 反動状況の設定処理
	bool IsRecoil(void) const;						// 反動状況の取得処理

	// 静的メンバ関数
	static CPlayerAction* Create(void);		// 生成処理

private:		// 誰でもアクセスできる

	// メンバ関数
	void NoneProcess(void);					// 通常状態処理
	void ShotProcess(CPlayer* pPlayer);		// 射撃状態処理
	void DaggerPrecess(CPlayer* pPlayer);	// ダガー状態処理
	void DodgeProcess(CPlayer* pPlayer);	// 回避状態処理
	void ShotgunProcess(CPlayer* pPlayer);	// 散弾(J+銃)状態
	void SwoopProcess(CPlayer* pPlayer);	// 急降下状態処理
	void ReloadProcess(CPlayer* pPlayer);	// リロード処理

	// メンバ変数
	D3DXVECTOR3 m_moveRecoil;	// 反動の移動量
	ACTION m_action;			// 行動
	int m_nActionCount;			// 行動カウント
	int m_nDodgeInterval;		// 回避のインターバルカウント
	float m_fDodgeRot;			// 回避する向き
	bool m_bDodgeUse;			// 回避使用可能状況
	bool m_bRecoil;				// 反動状況
};

#endif