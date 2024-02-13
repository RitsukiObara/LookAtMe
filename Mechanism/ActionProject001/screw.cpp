//===========================================
//
// ネジのメイン処理[screw.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "screw.h"
#include "screw_manager.h"
#include "useful.h"

#include "Particle.h"
#include "destruction.h"
#include "collision.h"
#include "objectElevation.h"
#include "elevation_manager.h"
#include "sound.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define INIT_ROT		(D3DXVECTOR3(0.0f,0.0f,0.7f))			// ネジの向き
#define ROT_MOVE		(0.04f)									// ネジの向きの移動量
#define DSTR_SIZE		(D3DXVECTOR3(30.0f, 30.0f, 0.0f))		// 飛沫演出のサイズ
#define DSTR_COL		(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f))		// 飛沫演出の色
#define DSTR_LIFE		(6)										// 飛沫演出の寿命

//==============================
// コンストラクタ
//==============================
CScrew::CScrew() : CModel(CObject::TYPE_SCREW, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pPrev = nullptr;			// 前のポインタ
	m_pNext = nullptr;			// 次のポインタ
	m_move = NONE_D3DXVECTOR3;	// 移動量
	m_bGravity = false;			// 重力状況
	m_bHit = true;				// 当たり判定状況

	if (CScrewManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CScrewManager::Get()->Regist(this);
	}
}

//==============================
// デストラクタ
//==============================
CScrew::~CScrew()
{

}

//============================
// 前のポインタの設定処理
//============================
void CScrew::SetPrev(CScrew* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CScrew::SetNext(CScrew* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CScrew* CScrew::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CScrew* CScrew::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CScrew::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_move = NONE_D3DXVECTOR3;	// 移動量
	m_bGravity = false;			// 重力状況
	m_bHit = true;				// 当たり判定状況

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CScrew::Uninit(void)
{
	// 終了処理
	CModel::Uninit();

	if (CScrewManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CScrewManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//========================================
//ブロックの更新処理
//========================================
void CScrew::Update(void)
{
	if(m_bGravity == true)
	{ // 重力状況が true の場合

		// 重力処理
		Gravity();
	}

	// 回転処理
	Cycle();

	// 起伏地面の当たり判定
	Elevation();

	// 台との当たり判定
	Table();

	// ブロックとの当たり判定
	Block();
}

//=====================================
//ブロックの描画処理
//=====================================
void CScrew::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// ヒット処理
//=====================================
void CScrew::Hit(const D3DXVECTOR3& pos)
{
	// 終了処理
	Uninit();

	// パーティクルの生成
	CParticle::Create(pos, CParticle::TYPE_ITEMGET);

	// 撃破の生成
	CDestruction::Create(pos, DSTR_SIZE, DSTR_COL, CDestruction::TYPE_AIRY, DSTR_LIFE);

	// アイテム取得音を鳴らす
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_GETITEM);
}

//=====================================
// 情報の設定処理
//=====================================
void CScrew::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const bool bGravity)
{
	// 情報の設定処理
	SetPos(pos);						// 位置
	SetPosOld(pos);						// 前回の位置
	SetRot(INIT_ROT);					// 向き
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CXFile::TYPE_SCREW);	// モデルの情報

	// 全ての値を初期化する
	m_move = move;				// 移動量
	m_bGravity = bGravity;		// 重力状況

	if (m_bGravity == true)
	{ // 重力状況が true の場合

		// 当たり判定状況を設定する
		m_bHit = false;				// 当たり判定状況
	}
	else
	{ // 上記以外

		// 当たり判定状況を設定する
		m_bHit = true;				// 当たり判定状況
	}
}

//=======================================
// 当たり判定の取得処理
//=======================================
bool CScrew::IsHit(void) const
{
	// 当たり判定状況を返す
	return m_bHit;
}

//=======================================
// 生成処理
//=======================================
CScrew* CScrew::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const bool bGravity)
{
	// ローカルオブジェクトを生成
	CScrew* pScrew = nullptr;	// インスタンスを生成

	if (pScrew == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pScrew = new CScrew;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pScrew != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pScrew->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pScrew->SetData(pos, move, bGravity);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ネジのポインタを返す
	return pScrew;
}

//=======================================
// 重力処理
//=======================================
void CScrew::Gravity(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 重力処理
	useful::Gravity(&m_move.y, pos, 0.8f);

	if (m_move.y <= 0.0f)
	{ // 移動量が 0.0f 以下の場合

		// ヒット状況を true にする
		m_bHit = true;
	}

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 回転処理
//=======================================
void CScrew::Cycle(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向きの設定処理

	// 向きを加算する
	rot.y += ROT_MOVE;

	// 向きを適用する
	SetRot(rot);
}

//=======================================
// 起伏の当たり判定処理
//=======================================
void CScrew::Elevation(void)
{
	// ローカル変数宣言
	CElevation* pMesh = CElevationManager::Get()->GetTop();		// 起伏の先頭のオブジェクトを取得する
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ

	while (pMesh != nullptr)
	{ // 地面の情報がある限り回す

		// 当たり判定を取る
		fHeight = pMesh->ElevationCollision(pos);
		
		if (pos.y < fHeight)
		{ // 当たり判定の位置が高かった場合

			// 高さを設定する
			pos.y = fHeight;

			// 重力を設定する
			m_move.y = 0.0f;
		}

		// 次のポインタを取得する
		pMesh = pMesh->GetNext();
	}

	// 位置を更新する
	SetPos(pos);
}

//=======================================
// 台との当たり判定処理
//=======================================
void CScrew::Table(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置
	D3DXVECTOR3 posOld = GetPosOld();	// 前回の位置

	if (collision::TableCollision(&pos, posOld, GetFileData().collsize.x, GetFileData().collsize.z) == true)
	{ // 台との当たり判定が true だった場合

		// 縦の移動量を無くす
		m_move.y = 0.0f;
	}

	// 位置を適用させる
	SetPos(pos);
}

//=======================================
// ブロックとの当たり判定処理
//=======================================
void CScrew::Block(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置
	D3DXVECTOR3 posOld = GetPosOld();	// 前回の位置

	if (collision::BlockCollision(&pos, posOld, GetFileData().collsize.x, GetFileData().collsize.y, false, nullptr) == true)
	{ // 台との当たり判定が true だった場合

		// 縦の移動量を無くす
		m_move.y = 0.0f;
	}

	// 位置を適用させる
	SetPos(pos);
}