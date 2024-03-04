//===================================
//
// プレイヤーのコントローラーヘッダー[player_controller.h]
// Author 小原立暉
//
//===================================
#ifndef _PLAYER_CONTROLLER_H_			// このマクロ定義がされていなかったら
#define _PLAYER_CONTROLLER_H_			// 2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CPlayer;		// プレイヤー

//-----------------------------------
// クラス定義(プレイヤーのコントローラー)
//-----------------------------------
class CPlayerController
{
public:			// 誰でもアクセスできる

	CPlayerController();		// コンストラクタ
	~CPlayerController();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Control(CPlayer* pPlayer);		// 操作処理

	// セット・ゲット関係
	void SetSpeed(const float fSpeed);	// 速度の設定処理
	float GetSpeedInit(void) const;		// 初期速度の取得処理

	// 静的メンバ関数
	static CPlayerController* Create(void);		// 生成処理

private:		// 誰でもアクセスできる

	// メンバ関数
	void ElevationCamera(void);				// 起伏地面とカメラの当たり判定
	void CameraControl(CPlayer* pPlayer);	// カメラの操作処理
	void CameraMouse(CPlayer* pPlayer);		// マウスでのカメラの操作処理
	void RotMove(CPlayer* pPlayer);			// 向きの設定処理
	void KeyboardMove(CPlayer* pPlayer);	// キーボードでの設定処理
	void Jump(CPlayer* pPlayer);			// ジャンプ処理
	void Shot(CPlayer* pPlayer);			// 射撃処理
	void HandGun(CPlayer* pPlayer);			// 拳銃処理
	void ShotGun(CPlayer* pPlayer);			// 散弾処理
	void Avoid(CPlayer* pPlayer);			// 回避処理
	void Dagger(CPlayer* pPlayer);			// ダガー処理

	// メンバ変数
	int m_nShotCount;			// 射撃カウント
	int m_nWalkCount;			// 歩行カウント
	float m_fStickRot;			// スティックの向き
	float m_fSpeed;				// 速度
	bool m_bRightShot;			// 右で撃つかどうか
};

#endif