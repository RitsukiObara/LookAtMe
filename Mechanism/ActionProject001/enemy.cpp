//===========================================
//
// 敵のメイン処理[enemy.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "enemy_manager.h"
#include "collision.h"
#include "useful.h"

#include "player.h"
#include "objectElevation.h"
#include "elevation_manager.h"
#include "stun.h"
#include "sound.h"

#include "itocan.h"
#include "machidori.h"
#include "garrejante.h"

// マクロ定義
#define GRAVITY			(0.5f)				// 重力
#define SMASH_MOVE		(8.5f)				// 吹き飛ばす移動量
#define SMASH_JUMP		(16.0f)				// 吹き飛ばすジャンプ量
#define SMASH_ADD_RIGHT	(14.0f)				// 右に吹き飛ばす追加の移動量
#define SMASH_ADD_LEFT	(12.0f)				// 左に吹き飛ばす追加の移動量

//==============================
// コンストラクタ
//==============================
CEnemy::CEnemy() : CModel(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// コンストラクタの箱
	Box();
}

//==============================
// オーバーロードコンストラクタ
//==============================
CEnemy::CEnemy(CObject::TYPE type, PRIORITY priority) : CModel(type, priority)
{
	// コンストラクタの箱
	Box();
}

//==============================
// デストラクタ
//==============================
CEnemy::~CEnemy()
{

}

//==============================
// コンストラクタの箱
//==============================
void CEnemy::Box(void)
{
	// 全ての値をクリアする
	m_pStun = nullptr;				// 気絶演出のポインタ
	m_posInit = NONE_D3DXVECTOR3;	// 初期位置
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_sizeColl = NONE_D3DXVECTOR3;	// 当たり判定のサイズ
	m_type = TYPE_ITOCAN;			// 種類
	m_nStunCount = 0;				// 気絶カウント
	m_bStep = false;				// 踏みつけ状況
	m_bCollision = true;			// 当たり判定を通るかどうか
	m_bStun = false;				// 気絶状況
	m_pPrev = nullptr;				// 前のポインタ
	m_pNext = nullptr;				// 次のポインタ

	if (CEnemyManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CEnemyManager::Get()->Regist(this);
	}
}

//============================
// 前のポインタの設定処理
//============================
void CEnemy::SetPrev(CEnemy* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CEnemy::SetNext(CEnemy* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CEnemy* CEnemy::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CEnemy* CEnemy::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CEnemy::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_pStun = nullptr;				// 気絶演出のポインタ
	m_posInit = NONE_D3DXVECTOR3;	// 初期位置
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_sizeColl = NONE_D3DXVECTOR3;	// 当たり判定のサイズ
	m_type = TYPE_ITOCAN;			// 種類
	m_nStunCount = 0;				// 気絶カウント
	m_bStep = false;				// 踏みつけ状況
	m_bCollision = true;			// 当たり判定を通るかどうか
	m_bStun = false;				// 気絶状況

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CEnemy::Uninit(void)
{
	if (m_pStun != nullptr)
	{ // 気絶演出が NULL じゃない場合

		// 終了処理
		m_pStun->Uninit();
		m_pStun = nullptr;
	}

	// 終了処理
	CModel::Uninit();

	if (CEnemyManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CEnemyManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//=====================================
//ブロックの描画処理
//=====================================
void CEnemy::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// ヒット処理
//=====================================
void CEnemy::Hit(void)
{
	if (m_pStun != nullptr)
	{ // 気絶演出が NULL じゃない場合

		// 終了処理
		m_pStun->Uninit();
		m_pStun = nullptr;
	}

	// 気絶状況を false にする
	m_bStun = false;

	// 気絶カウントを初期化する
	m_nStunCount = 0;
}

//=====================================
// 吹き飛びヒット処理
//=====================================
void CEnemy::SmashHit(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posCamera = CManager::Get()->GetCamera()->GetPosV();		// 視点の位置を取得する
	CPlayer* player = CPlayer::Get();										// プレイヤーの情報を取得する
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する
	float fSmashRot;				// 吹き飛ばす向き
	
	// 吹き飛ばす向きを設定する
	fSmashRot = atan2f(posCamera.x - pos.x, posCamera.z - pos.z);

	// 向きを0.0fにする
	rot.y = 0.0f;

	if (player->IsRight() == true)
	{ // 右向きの場合

		// 向きを設定する
		m_move.x = sinf(fSmashRot) * SMASH_MOVE + sinf(player->GetRot().y) * SMASH_ADD_RIGHT;
		m_move.y = SMASH_JUMP;
		m_move.z = cosf(fSmashRot) * SMASH_MOVE;
	}
	else
	{ // 上記以外

		// 向きを設定する
		m_move.x = sinf(fSmashRot) * SMASH_MOVE + sinf(player->GetRot().y) * SMASH_ADD_LEFT;
		m_move.y = SMASH_JUMP;
		m_move.z = cosf(fSmashRot) * SMASH_MOVE;
	}

	// 情報を適用する
	SetRot(rot);		// 向き

	if (m_pStun != nullptr)
	{ // 気絶演出が NULL じゃない場合

		// 終了処理
		m_pStun->Uninit();
		m_pStun = nullptr;
	}

	// 気絶状況を false にする
	m_bStun = false;

	// 気絶カウントを初期化する
	m_nStunCount = 0;

	// 吹き飛ばし音を鳴らす
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SMASH);

	// 吹き飛び中の音を鳴らす
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SMASHFLY);
}

//=====================================
// 気絶のヒット処理
//=====================================
void CEnemy::StunHit(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	if (m_pStun == nullptr)
	{ // 気絶の情報が NULL の場合

		// 気絶演出の生成処理
		m_pStun = CStun::Create(D3DXVECTOR3(pos.x, pos.y + m_sizeColl.y, pos.z));
	}

	// 気絶状況を true にする
	m_bStun = true;

	// 気絶カウントを初期化する
	m_nStunCount = 0;
}

//=====================================
// 情報の設定処理
//=====================================
void CEnemy::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetScale(NONE_SCALE);			// 拡大率

	// 全ての値を設定する
	m_posInit = pos;				// 初期位置
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_sizeColl = NONE_D3DXVECTOR3;	// 当たり判定のサイズ
	m_type = type;					// 種類
	m_nStunCount = 0;				// 気絶カウント
	m_bStep = false;				// 踏みつけ状況
	m_bCollision = true;			// 当たり判定を通るかどうか
	m_bStun = false;				// 気絶状況
}

//=======================================
// 生成処理
//=======================================
CEnemy* CEnemy::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ローカルオブジェクトを生成
	CEnemy* pEnemy = nullptr;	// インスタンスを生成

	if (pEnemy == nullptr)
	{ // オブジェクトが NULL の場合

		switch (type)
		{
		case TYPE_ITOCAN:		// イトキャン

			// インスタンスを生成
			pEnemy = new CItocan;

			break;

		case TYPE_MACHIDORI:	// マシンドリー

			// インスタンスを生成
			pEnemy = new CMachidori;

			break;

		case TYPE_GARREJANTE:

			// インスタンスを生成
			pEnemy = new CGarrejante;

			break;

		default:

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pEnemy != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pEnemy->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEnemy->SetData(pos, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 敵のポインタを返す
	return pEnemy;
}

//=======================================
// 移動量の設定処理
//=======================================
void CEnemy::SetMove(const D3DXVECTOR3& move)
{
	// 移動量を設定する
	m_move = move;
}

//=======================================
// 移動量の取得処理
//=======================================
D3DXVECTOR3 CEnemy::GetMove(void) const
{
	// 移動量を返す
	return m_move;
}

//=======================================
// 当たり判定のサイズの設定処理
//=======================================
void CEnemy::SetCollSize(const D3DXVECTOR3& size)
{
	// 当たり判定のサイズを設定する
	m_sizeColl = size;
}

//=======================================
// 当たり判定のサイズの取得処理
//=======================================
D3DXVECTOR3 CEnemy::GetCollSize(void) const
{
	// 当たり判定のサイズを返す
	return m_sizeColl;
}

//=======================================
// 種類の取得処理
//=======================================
CEnemy::TYPE CEnemy::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//=======================================
// 気絶カウントの設定処理
//=======================================
void CEnemy::SetStunCount(const int nCount)
{
	// 気絶カウントを設定する
	m_nStunCount = nCount;
}

//=======================================
// 気絶カウントの取得処理
//=======================================
int CEnemy::GetStunCount(void) const
{
	// 気絶カウントを返す
	return m_nStunCount;
}

//=======================================
// 踏みつけ状況の設定処理
//=======================================
void CEnemy::SetEnableStep(const bool bStep)
{
	// 踏みつけ状況を設定する
	m_bStep = bStep;
}

//=======================================
// 踏みつけ状況の取得処理
//=======================================
bool CEnemy::IsStep(void) const
{
	// 踏みつけ状況を返す
	return m_bStep;
}

//=======================================
// 気絶演出の取得処理
//=======================================
CStun* CEnemy::GetStun(void) const
{
	// 気絶演出を返す
	return m_pStun;
}

//=======================================
// 気絶状態の消去処理
//=======================================
void CEnemy::DeleteStun(void)
{
	if (m_pStun != nullptr)
	{ // 気絶の状態が NULL じゃない場合

		// メモリを解放する
		m_pStun->Uninit();
		m_pStun = nullptr;
	}
}

//=======================================
// 初期位置の取得処理
//=======================================
D3DXVECTOR3 CEnemy::GetInitPos(void) const
{
	// 初期位置を返す
	return m_posInit;
}

//=======================================
// 当たり判定状況の設定処理
//=======================================
void CEnemy::SetEnableCollision(const bool bColl)
{
	// 当たり判定状況を設定する
	m_bCollision = bColl;
}

//=======================================
// 当たり判定状況の取得処理
//=======================================
bool CEnemy::IsCollision(void) const
{
	// 当たり判定を返す
	return m_bCollision;
}

//=======================================
// 気絶状況の設定処理
//=======================================
void CEnemy::SetEnableStun(const bool bStun)
{
	// 気絶状況を設定する
	m_bStun = bStun;
}

//=======================================
// 気絶状況の取得処理
//=======================================
bool CEnemy::IsStun(void) const
{
	// 気絶状況を返す
	return m_bStun;
}

//=======================================
// 重力処理
//=======================================
void CEnemy::Gravity(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置を取得する

	// 重力処理
	useful::Gravity(&m_move.y, pos, GRAVITY);

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 起伏地面の当たり判定処理
//=======================================
bool CEnemy::ElevationCollision(void)
{
	// ローカル変数宣言
	CElevation* pMesh = CElevationManager::Get()->GetTop();		// 起伏の先頭のオブジェクトを取得する
	D3DXVECTOR3 pos = GetPos();				// 位置を取得する
	float fHeight = 0.0f;					// 高さ
	bool bLand = false;						// 着地判定

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

			// 着地判定を付ける
			bLand = true;
		}

		// 次のポインタを取得する
		pMesh = pMesh->GetNext();
	}

	// 位置を更新する
	SetPos(pos);

	// 着地判定を返す
	return bLand;
}

//=======================================
// ブロックとの当たり判定
//=======================================
bool CEnemy::BlockCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置
	D3DXVECTOR3 posOld = GetPosOld();	// 前回の位置
	bool bLand = false;					// 着地判定

	// 着地判定を取得する
	bLand = collision::BlockCollision(&pos, posOld, m_sizeColl.x, m_sizeColl.y, false, nullptr);

	// 位置を適用させる
	SetPos(pos);

	// 着地判定を返す
	return bLand;
}

//=======================================
// 台との当たり判定
//=======================================
bool CEnemy::TableCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置
	D3DXVECTOR3 posOld = GetPosOld();	// 前回の位置
	bool bLand = false;					// 着地判定

	// 着地判定を取得する
	bLand = collision::TableCollision(&pos, posOld, m_sizeColl.x, m_sizeColl.z);

	// 位置を適用させる
	SetPos(pos);

	// 着地判定を返す
	return bLand;
}