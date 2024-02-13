//============================================
//
// 敵のメイン処理[enemy.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "main.h"
#include "enemy.h"
#include "game.h"
#include "motion.h"
#include "renderer.h"
#include "useful.h"

#include "shot_enemy.h"
#include "kick_enemy.h"

#include "score.h"

//===========================================
// コンストラクタ
//===========================================
CEnemy::CEnemy() : CCharacter(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// コンストラクタ(オーバーロード)
//===========================================
CEnemy::CEnemy(CObject::TYPE type, PRIORITY priority) : CCharacter(type, priority)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// デストラクタ
//===========================================
CEnemy::~CEnemy()
{

}

//===========================================
// コンストラクタの箱
//===========================================
void CEnemy::Box(void)
{
	// 全ての値をクリアする
	m_type = TYPE_SHOT;			// 種類
	m_state = STATE_NONE;		// 状態
}

#if 0

//===========================================
// 初期化処理
//===========================================
HRESULT CEnemy::Init(const D3DXVECTOR3& pos)
{
	// 成功を返す
	return S_OK;
}

#endif

//===========================================
// 終了処理
//===========================================
void CEnemy::Uninit(void)
{
	// モーションのメモリを開放する
	delete m_pMotion;
	m_pMotion = nullptr;

	// 終了
	CCharacter::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CEnemy::Update(void)
{

}

//===========================================
// 描画処理
//===========================================
void CEnemy::Draw(void)
{
	// 描画処理
	CCharacter::Draw();
}

//===========================================
// ヒット処理
//===========================================
void CEnemy::Hit(void)
{
	// 死亡処理
	m_state = STATE_DEATH;

	// 種類の設定処理
	SetType(TYPE_DEATHENEMY);
}

//===========================================
// 状態の設定処理
//===========================================
void CEnemy::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//===========================================
// 状態の取得処理
//===========================================
CEnemy::STATE CEnemy::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//===========================================
// モーションの設定(生成)処理
//===========================================
void CEnemy::SetMotion(void)
{
	// モーションの生成処理
	m_pMotion = CMotion::Create();
}

//===========================================
// モーションの取得処理
//===========================================
CMotion* CEnemy::GetMotion(void)
{
	// モーションの情報を返す
	return m_pMotion;
}

//===========================================
// 生成処理
//===========================================
CEnemy* CEnemy::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ローカルオブジェクトを生成
	CEnemy* pEnemy = nullptr;	// プレイヤーのインスタンスを生成

	if (pEnemy == nullptr)
	{ // オブジェクトが NULL の場合

		switch (type)
		{
		case TYPE_SHOT:			// 弾撃ち敵

			// 弾撃ち敵を生成
			pEnemy = new CShotEnemy;

			break;

		case TYPE_KICK:			// 障害物

			// 障害物キック敵を生成
			pEnemy = new CKickEnemy;

			break;

		default:

			// 警告文
			MessageBox(NULL, "敵の生成で規定外の数値が検出されました！", "警告！", MB_ICONWARNING);

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

			// 警告文
			MessageBox(NULL, "敵の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報を初期化する
		pEnemy->m_type = type;			// 種類
		pEnemy->m_state = STATE_NONE;	// 状態

		// 情報の設定処理
		pEnemy->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pEnemy;
}