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

	// 列挙型定義(カメラの種類)
	typedef enum
	{
		TYPE_NONE = 0,		// 無し
		TYPE_UPWARD,		// 上向き
		TYPE_PRECEDENCE,	// 先行追跡
		TYPE_ZOOMOUT,		// ズームアウト
		TYPE_ZOOMIN,		// ズームイン
		TYPE_STARTSTAGE,	// スタートカメラ(ステージ)
		TYPE_STARTRUN,		// スタートカメラ(走り姿)
		TYPE_STARTBACK,		// スタートカメラ(戻る)
		TYPE_LAND,			// 着地
		TYPE_WALLKICK,		// 壁キック
		TYPE_BOOST,			// ブーストカメラ
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	CCamera();		// コンストラクタ
	~CCamera();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Set(void);					// 設定処理

	void Rotation(void);			// 回り込み処理
	void Reset(void);				// 情報のリセット処理

	// セット・ゲット関係
	void SetType(const TYPE type);	// 種類の設定処理
	TYPE GetType(void) const;		// 種類の取得処理

	void SetPosV(const D3DXVECTOR3 posV);		// 視点の位置の設定処理
	D3DXVECTOR3 GetPosV(void) const;			// 視点の位置の取得処理

	void SetPosR(const D3DXVECTOR3 posR);		// 注視点の位置の設定処理
	D3DXVECTOR3 GetPosR(void) const;			// 注視点の位置の取得処理

	void SetRot(const D3DXVECTOR3& rot);		// 向きの設定処理
	D3DXVECTOR3 GetRot(void) const;				// 向きの取得処理

	void SetDistance(const float fDis);			// 距離の設定処理
	float GetDistance(void) const;				// 距離の取得処理

	void ChangeControl(void);					// カメラの操作状況の切り替え処理

private:		// 自分だけアクセス出来る

	// メンバ関数
	void Move(void);				// 移動処理
	void MovePosV(void);			// 視点操作処理
	void MovePosR(void);			// 注視点操作処理
	void MoveRange(void);			// 距離操作処理
	void Chase(void);				// 追跡処理
	void Control(void);				// 操作処理
	void PosSet(void);				// 位置の設定処理
	void TypeProcess(void);			// 種類の更新処理

	// 種類による処理
	void Upward(void);				// 上向き処理
	void Precedence(void);			// 先行追跡処理
	void ZoomOut(void);				// ズームアウト処理
	void ZoomIn(void);				// ズームイン処理
	void StartStage(void);			// ステージ投影の処理
	void StartRun(void);			// 走り姿の処理
	void StartBack(void);			// 足元の処理
	void Land(void);				// 着地処理
	void WallKick(void);			// 壁キックの処理
	void Boost(void);				// ブーストカメラの処理

	// 種類による設定処理
	void SetUpward(void);			// 上向きカメラの設定処理
	void SetPrecedence(void);		// 先行追跡カメラの設定処理
	void SetZoomOut(void);			// ズームアウトカメラの設定処理
	void SetZoomIn(void);			// ズームインカメラの設定処理
	void SetStartStage(void);		// ステージ投影カメラの設定処理
	void SetStartRun(void);			// 走り姿カメラの設定処理
	void SetStartBack(void);		// 足元カメラの設定処理
	void SetLand(void);				// 着地カメラの設定処理
	void SetWallKick(void);			// 壁キックの設定処理
	void SetBoost(void);			// ブーストカメラの設定処理

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
	TYPE m_type;					// 種類
	float m_rotDest;				// 目的の向き
	float m_Dis;					// 距離
	float m_DisDest;				// 目的の距離
	int m_nSwingCount;				// 揺れのカウント
	bool m_bControl;				// 操作状況
};

#endif