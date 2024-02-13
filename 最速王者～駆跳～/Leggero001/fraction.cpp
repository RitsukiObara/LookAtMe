//============================================
//
// 破片のメイン処理[fraction.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "fraction.h"
#include "manager.h"
#include "renderer.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define NUM_FRACTION			(15)		// 通常の破片の生成数
#define NUM_CLIFFCATCH			(40)		// 崖捕まり時の破片の生成数
#define FRACTION_GRAVITY		(0.5f)		// 破片の重力

// 障害物の破片
#define OBSTACLE_FRACTION_POS_SHIFT_XZ	(51)		// 障害物の破片の位置のずらす幅(XZ軸)
#define OBSTACLE_FRACTION_POS_SHIFT_Y	(100)		// 障害物の破片の位置のずらす幅(Y軸)
#define OBSTACLE_FRACTION_SCALE_RAND	(50)		// 障害物の破片の拡大率のランダム幅
#define OBSTACLE_FRACTION_SCALE_MIN		(0.5f)		// 障害物の破片の拡大率の最低限数
#define OBSTACLE_FRACTION_MOVE_RAND_XZ	(20)		// 障害物の破片の移動量のランダム値(XZ軸)
#define OBSTACLE_FRACTION_MOVE_RAND_Y	(5)			// 障害物の破片の移動量のランダム値(Y軸)
#define OBSTACLE_FRACTION_MOVE_MIN		(3)			// 障害物の破片の移動量の最低限値

// 壁キックの破片
#define WALLKICK_FRACTION_POS_SHIFT_XZ	(20)		// 壁キックの破片の位置のずらす幅(XZ軸)
#define WALLKICK_FRACTION_POS_SHIFT_Y	(5)			// 壁キックの破片の位置のずらす幅(Y軸)
#define WALLKICK_FRACTION_SCALE_RAND	(10)		// 壁キックの破片の拡大率のランダム幅
#define WALLKICK_FRACTION_SCALE_MIN		(0.2f)		// 壁キックの破片の拡大率の最低限数
#define WALLKICK_FRACTION_MOVE_RAND_XZ	(10)		// 壁キックの破片の移動量のランダム値(XZ軸)
#define WALLKICK_FRACTION_MOVE_RAND_Y	(10)		// 壁キックの破片の移動量のランダム値(Y軸)
#define WALLKICK_FRACTION_MOVE_MIN		(2)			// 壁キックの破片の移動量の最低限値

// 崖掴みの破片
#define CLIFFCATCH_FRACTION_SCALE_RAND		(30)		// 崖掴み時の破片の拡大率のランダム幅
#define CLIFFCATCH_FRACTION_SCALE_MIN		(0.4f)		// 崖掴み時の破片の拡大率の最低限数
#define CLIFFCATCH_FRACTION_MOVE_RAND_XZ	(15)		// 崖掴み時の破片の移動量のランダム値(XZ軸)
#define CLIFFCATCH_FRACTION_MOVE_RAND_Y		(5)			// 崖掴み時の破片の移動量のランダム値(Y軸)
#define CLIFFCATCH_FRACTION_MOVE_MIN		(3)			// 崖掴み時の破片の移動量の最低限値

//===========================================
// コンストラクタ
//===========================================
CFraction::CFraction() : CModel(CObject::TYPE_FRACTION, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
}

//===========================================
// デストラクタ
//===========================================
CFraction::~CFraction()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CFraction::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CFraction::Uninit(void)
{
	// 終了
	CModel::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CFraction::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置
	D3DXVECTOR3 rot = GetRot();		// 向き

	// 前回の位置の設定
	SetPosOld(pos);

	// 重力処理
	Gravity(&m_move.y, pos, FRACTION_GRAVITY);

	// 位置と向きを加算する
	pos += m_move;

	// 位置と向きを更新する
	SetPos(pos);
	SetRot(rot);

	if (pos.y <= 0.0f)
	{ // Y座標が 0.0f を超えた場合

		// 終了処理
		Uninit();
	}
}

//===========================================
// 描画処理
//===========================================
void CFraction::Draw(void)
{
	// 描画処理
	CModel::Draw(m_col);
}

//===========================================
// 情報の設定処理
//===========================================
void CFraction::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3 scale, const D3DXVECTOR3& move, const int nTexIdx, const D3DXCOLOR col)
{
	// スクロールの設定処理
	SetPos(pos);							// 位置の設定
	SetPosOld(pos);							// 前回の位置の設定
	SetRot(rot);							// 向きの設定
	SetScale(scale);						// 拡大率の設定
	SetFileData(CXFile::TYPE_FRACTION);		// ファイルの設定処理

	CXFile::SXFile file = GetFileData();

	// 最大値を反映する
	file.vtxMax.x *= scale.x;
	file.vtxMax.y *= scale.y;
	file.vtxMax.z *= scale.z;

	// 最小値を反映する
	file.vtxMin.x *= scale.x;
	file.vtxMin.y *= scale.y;
	file.vtxMin.z *= scale.z;

	// テクスチャの割り当て処理
	file.m_nTexIdx[0] = nTexIdx;

	// データの設定処理
	SetFileData(file);

	// 全ての値を初期化する
	m_move = move;			// 移動量
	m_col = col;			// 色
}

//===========================================
// 設定処理
//===========================================
void CFraction::SetObstacle(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 scale;		// 拡大率
	D3DXVECTOR3 move;		// 移動量

	for (int nCnt = 0; nCnt < NUM_FRACTION; nCnt++)
	{
		// 位置を設定する
		pos.x = center.x + rand() % OBSTACLE_FRACTION_POS_SHIFT_XZ - (int)(OBSTACLE_FRACTION_POS_SHIFT_XZ * 0.5f);
		pos.y = center.y + rand() % OBSTACLE_FRACTION_POS_SHIFT_Y;
		pos.z = center.z + rand() % OBSTACLE_FRACTION_POS_SHIFT_XZ - (int)(OBSTACLE_FRACTION_POS_SHIFT_XZ * 0.5f);

		// 向きを設定する
		rot.x = (float)((rand() % 629 - 314) * 0.01f);
		rot.y = (float)((rand() % 629 - 314) * 0.01f);
		rot.z = (float)((rand() % 629 - 314) * 0.01f);

		// 拡大率を設定する
		scale.x = (float)((rand() % OBSTACLE_FRACTION_SCALE_RAND) * 0.01f) + OBSTACLE_FRACTION_SCALE_MIN;
		scale.y = (float)((rand() % OBSTACLE_FRACTION_SCALE_RAND) * 0.01f) + OBSTACLE_FRACTION_SCALE_MIN;
		scale.z = (float)((rand() % OBSTACLE_FRACTION_SCALE_RAND) * 0.01f) + OBSTACLE_FRACTION_SCALE_MIN;

		// 移動量を設定する
		move.x = (float)(rand() % OBSTACLE_FRACTION_MOVE_RAND_XZ + OBSTACLE_FRACTION_MOVE_MIN) - (int)(OBSTACLE_FRACTION_MOVE_RAND_XZ * 0.5f);
		move.y = (float)(rand() % OBSTACLE_FRACTION_MOVE_RAND_Y + OBSTACLE_FRACTION_MOVE_MIN);
		move.z = (float)(rand() % OBSTACLE_FRACTION_MOVE_RAND_XZ + OBSTACLE_FRACTION_MOVE_MIN) - (int)(OBSTACLE_FRACTION_MOVE_RAND_XZ * 0.5f);

		// 破片の生成処理
		CFraction::Create(pos, rot, scale, move, nTexIdx, col);
	}
}

//===========================================
// 壁キック時の破片の設定処理
//===========================================
void CFraction::SetWallKick(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 scale;		// 拡大率
	D3DXVECTOR3 move;		// 移動量

	for (int nCnt = 0; nCnt < NUM_FRACTION; nCnt++)
	{
		// 位置を設定する
		pos.x = center.x + rand() % WALLKICK_FRACTION_POS_SHIFT_XZ - (int)(WALLKICK_FRACTION_POS_SHIFT_XZ * 0.5f);
		pos.y = center.y + rand() % WALLKICK_FRACTION_POS_SHIFT_Y;
		pos.z = center.z + rand() % WALLKICK_FRACTION_POS_SHIFT_XZ - (int)(WALLKICK_FRACTION_POS_SHIFT_XZ * 0.5f);

		// 向きを設定する
		rot.x = (float)((rand() % 629 - 314) * 0.01f);
		rot.y = (float)((rand() % 629 - 314) * 0.01f);
		rot.z = (float)((rand() % 629 - 314) * 0.01f);

		// 拡大率を設定する
		scale.x = (float)((rand() % WALLKICK_FRACTION_SCALE_RAND) * 0.01f) + WALLKICK_FRACTION_SCALE_MIN;
		scale.y = (float)((rand() % WALLKICK_FRACTION_SCALE_RAND) * 0.01f) + WALLKICK_FRACTION_SCALE_MIN;
		scale.z = (float)((rand() % WALLKICK_FRACTION_SCALE_RAND) * 0.01f) + WALLKICK_FRACTION_SCALE_MIN;

		// 移動量を設定する
		move.x = (float)(rand() % WALLKICK_FRACTION_MOVE_RAND_XZ + WALLKICK_FRACTION_MOVE_MIN) - (int)(WALLKICK_FRACTION_MOVE_RAND_XZ * 0.5f);
		move.y = (float)(rand() % WALLKICK_FRACTION_MOVE_RAND_Y + WALLKICK_FRACTION_MOVE_MIN);
		move.z = (float)(rand() % WALLKICK_FRACTION_MOVE_RAND_XZ + WALLKICK_FRACTION_MOVE_MIN) - (int)(WALLKICK_FRACTION_MOVE_RAND_XZ * 0.5f);

		// 破片の生成処理
		CFraction::Create(pos, rot, scale, move, nTexIdx, col);
	}
}

//===========================================
// 左崖掴み時の破片の設定処理
//===========================================
void CFraction::SetLCliffCatch(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 scale;		// 拡大率
	D3DXVECTOR3 move;		// 破片の移動量

	for (int nCnt = 0; nCnt < NUM_CLIFFCATCH; nCnt++)
	{
		// 向きを設定する
		rot.x = (float)((rand() % 629 - 314) * 0.01f);
		rot.y = (float)((rand() % 629 - 314) * 0.01f);
		rot.z = (float)((rand() % 629 - 314) * 0.01f);

		// 拡大率を設定する
		scale.x = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;
		scale.y = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;
		scale.z = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;

		// 移動量を設定する
		move.x = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_XZ + CLIFFCATCH_FRACTION_MOVE_MIN);
		move.y = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_Y + CLIFFCATCH_FRACTION_MOVE_MIN);
		move.z = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_XZ + CLIFFCATCH_FRACTION_MOVE_MIN);

		// 破片の生成処理
		CFraction::Create(center, rot, scale, move, nTexIdx, col);
	}
}

//===========================================
// 右崖掴み時の破片の設定処理
//===========================================
void CFraction::SetRCliffCatch(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 scale;		// 拡大率
	D3DXVECTOR3 move;		// 破片の移動量

	for (int nCnt = 0; nCnt < NUM_CLIFFCATCH; nCnt++)
	{
		// 向きを設定する
		rot.x = (float)((rand() % 629 - 314) * 0.01f);
		rot.y = (float)((rand() % 629 - 314) * 0.01f);
		rot.z = (float)((rand() % 629 - 314) * 0.01f);

		// 拡大率を設定する
		scale.x = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;
		scale.y = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;
		scale.z = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;

		// 移動量を設定する
		move.x = (float)-(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_XZ + CLIFFCATCH_FRACTION_MOVE_MIN);
		move.y = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_Y + CLIFFCATCH_FRACTION_MOVE_MIN);
		move.z = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_XZ + CLIFFCATCH_FRACTION_MOVE_MIN);

		// 破片の生成処理
		CFraction::Create(center, rot, scale, move, nTexIdx, col);
	}
}

//===========================================
// 生成処理
//===========================================
CFraction* CFraction::Create(const D3DXVECTOR3& pos,const D3DXVECTOR3& rot,const D3DXVECTOR3 scale,const D3DXVECTOR3& move, const int nTexIdx, const D3DXCOLOR col)
{
	// ローカルオブジェクトを生成
	CFraction* pFraction = nullptr;	// プレイヤーのインスタンスを生成

	if (pFraction == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pFraction = new CFraction;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pFraction != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pFraction->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "破片の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pFraction->SetData(pos, rot, scale, move, nTexIdx, col);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pFraction;
}