//===========================================
//
// ドラム缶のメイン処理[drum.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "woodbox.h"
#include "collision.h"
#include "useful.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define WOODBOX_GRAVITY			(0.5f)			// 木箱の重力

//==============================
// コンストラクタ
//==============================
CWoodBox::CWoodBox() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_fGravity = 0.0f;			// 重力
}

//==============================
// デストラクタ
//==============================
CWoodBox::~CWoodBox()
{

}

//==============================
// 木箱の初期化処理
//==============================
HRESULT CWoodBox::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_fGravity = 0.0f;			// 重力

	// 値を返す
	return S_OK;
}

//========================================
// 木箱の終了処理
//========================================
void CWoodBox::Uninit(void)
{
	// 終了処理
	CObstacle::Uninit();
}

//========================================
// 木箱の更新処理
//========================================
void CWoodBox::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 前回の位置を設定する
	SetPosOld(pos);

	// 重力処理
	Gravity(&m_fGravity, pos, WOODBOX_GRAVITY);

	if (pos.y <= 0.0f)
	{ // Y座標が 0.0f 以下になった場合

		// 位置を補正する
		pos.y = 0.0f;

		// 重力を初期化する
		m_fGravity = 0.0f;
	}

	// 位置を更新する
	SetPos(pos);

	// 障害物同士の当たり判定処理
	if (collision::ObstacleMutualCollision(this) == true)
	{ // 着地した場合

		// 重力を初期化する
		m_fGravity = 0.0f;
	}

	if (collision::ObstacleBlockCollision(this) == true)
	{  // 着地した場合

		// 重力を初期化する
		m_fGravity = 0.0f;
	}

	// 更新処理
	CObstacle::Update();
}

//=====================================
// 木箱の描画処理
//=====================================
void CWoodBox::Draw(void)
{
	// 描画処理
	CObstacle::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CWoodBox::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 初期化処理
	SetFileData(CXFile::TYPE_WOODBOX);		// データの設定処理

	// 全ての値を初期化する
	m_fGravity = 0.0f;			// 重力

	// 情報の設定処理
	CObstacle::SetData(pos, type);
}