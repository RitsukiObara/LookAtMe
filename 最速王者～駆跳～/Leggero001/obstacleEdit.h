#ifdef _DEBUG	// デバッグ処理
//===================================
//
// 障害物エディットヘッダー[obstacleEdit.h]
// Author 小原立暉
//
//===================================
#ifndef _OBSTACLEEDIT_H_
#define _OBSTACLEEDIT_H_

#include "main.h"
#include "obstacle.h"

//------------------------------------------
// クラス定義(エディット)
//------------------------------------------
class CObstacleEdit : public CObstacle
{
public:			// 誰でもアクセスできる

	CObstacleEdit();		// コンストラクタ
	~CObstacleEdit();		// デストラクタ

	//プロトタイプ宣言
	HRESULT Init(void);		// オブジェクトの初期化処理
	void Uninit(void);		// オブジェクトの終了処理
	void Update(void);		// オブジェクトの更新処理
	void Draw(void);		// オブジェクトの描画処理

	void SetData(const D3DXVECTOR3& pos);					// 情報の設定処理

	// 静的メンバ関数
	static CObstacleEdit* Create(const D3DXVECTOR3& pos);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void TypeChange(void);		// 種類変更処理
	void Move(void);			// 移動処理
	void Set(void);				// オブジェクトの設定処理
	void Delete(void);			// オブジェクトの消去
	void Reset(void);			// オブジェクトの情報リセット処理
	void PlayerPosSet(void);	// オブジェクトのプレイヤー位置移動
};

#endif

#endif