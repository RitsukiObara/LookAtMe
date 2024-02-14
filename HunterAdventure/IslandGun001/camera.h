//===================================
//
// カメラヘッダー[camera.h]
// Author 小原立暉
//
//===================================
#ifndef _CAMERA_H_			//このマクロ定義がされていなかったら
#define _CAMERA_H_			//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// クラス定義(カメラ)
//-----------------------------------
class CCamera
{
public:			// 誰でもアクセス出来る

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_NONE = 0,		// 通常状態
		TYPE_VIBRATE,		// 振動状態
		TYPE_BOSSCLOSER,	// ボス寄り状態
		TYPE_BOSSHOWLING,	// ボス雄たけび状態
		TYPE_MAX			// この列挙型の総数
	};

	// 構造体定義(振動)
	struct SVibrate
	{
		TYPE nextType;		// 次の種類
		int nElapseCount;	// 経過カウント
		int nSwingCount;	// 揺れのカウント
		int nSwingRange;	// 揺れの範囲
		int nFinishCount;	// 終了カウント
		float fCorrect;		// 補正係数
		bool bDown;			// 下状況
	};

	CCamera();		// コンストラクタ
	~CCamera();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Set(void);					// 設定処理

	void Rotation(void);			// 回り込み処理
	void Reset(void);				// 情報のリセット処理

	void SetPosV(const D3DXVECTOR3 posV);		// 視点の位置の設定処理
	D3DXVECTOR3 GetPosV(void) const;			// 視点の位置の取得処理

	void SetPosR(const D3DXVECTOR3 posR);		// 注視点の位置の設定処理
	D3DXVECTOR3 GetPosR(void) const;			// 注視点の位置の取得処理

	void SetRot(const D3DXVECTOR3& rot);		// 向きの設定処理
	D3DXVECTOR3 GetRot(void) const;				// 向きの取得処理

	void SetDistance(const float fDis);			// 距離の設定処理
	float GetDistance(void) const;				// 距離の取得処理

	void SetViewport(const D3DVIEWPORT9& viewport);		// ビューポートの設定処理
	D3DVIEWPORT9 GetViewport(void) const;				// ビューポートの設定処理

	void SetVibrate(const SVibrate vib);		// 振動関係の設定処理
	SVibrate GetVibrate(void) const;			// 振動関係の取得処理

	void SetType(const TYPE type);				// 種類の設定処理
	TYPE GetType(void) const;					// 種類の取得処理

	void ChangeControl(void);					// カメラの操作状況の切り替え処理

	// 静的メンバ関数
	static CCamera* Create(void);		// 生成処理

private:		// 自分だけアクセス出来る

	// メンバ関数
	// キーボード操作
	void Control(void);				// 操作処理
	void Move(void);				// 移動処理
	void MovePosV(void);			// 視点操作処理
	void MovePosR(void);			// 注視点操作処理
	void MoveRange(void);			// 距離操作処理

	// マウス操作
	void MouseControl(void);		// マウスでのカメラ操作処理
	void MousePosR(void);			// 注視点操作処理
	void MousePosV(void);			// 視点操作処理
	void MousePosMove(void);		// 視点・注始点操作処理

	void Chase(void);				// 追跡処理
	void Vibrate(void);				// 振動処理
	void BossCloser(void);			// ボス寄り処理
	void BossHowling(void);			// ボス雄たけび処理
	void PosSet(void);				// 位置の設定処理
	void TypeProcess(void);			// カメラの種類ごとの処理

	// メンバ変数
	D3DXMATRIX m_mtxView;			// ビューマトリックス
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;				// 視点
	D3DXVECTOR3 m_posVDest;			// 目的の視点
	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_posRDest;			// 目的の注視点
	D3DXVECTOR3 m_VecU;				// 上方向ベクトル
	D3DXVECTOR3 m_rot;				// 向き
	D3DVIEWPORT9 m_viewport;		// ビューポート
	SVibrate m_vibrate;				// 振動関係
	TYPE m_type;					// 種類
	float m_Dis;					// 距離
	bool m_bControl;				// 操作状況
};

#endif