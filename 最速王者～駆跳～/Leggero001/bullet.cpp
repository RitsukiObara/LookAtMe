//============================================
//
// 弾のメイン処理[bullet.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "useful.h"

#include "collision.h"
#include "Effect.h"
#include "enemy.h"
#include "explosion.h"
#include "Particle.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define BULLET_TEXTURE		"data/TEXTURE/Bullet.png"	// 弾のテクスチャ
#define BULLET_SPEED		(10.0f)						// 弾の速度
#define BULLET_LIFE			(60)						// 弾の寿命

//===========================================
// コンストラクタ
//===========================================
CBullet::CBullet() : CBillboard(TYPE_BULLET, CObject::PRIORITY_ENTITY)
{
	// 全ての値を初期化する
	m_nLife = 0;								// 寿命
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_fMoveRot = 0.0f;							// 移動する方向
	m_type = TYPE_PLAYER;						// 弾の種類
}

//===========================================
// デストラクタ
//===========================================
CBullet::~CBullet()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CBullet::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nLife = BULLET_LIFE;						// 寿命
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_fMoveRot = 0.0f;							// 移動する方向
	m_type = TYPE_PLAYER;						// 弾の種類

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CBullet::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CBullet::Update(void)
{
	// ローカル変数を宣言
	D3DXVECTOR3 pos = GetPos();			// 弾の位置
	D3DXVECTOR3 size = GetSize();		// 弾のサイズ

	// 前回の位置の設定処理
	SetPosOld(pos);

	// 位置設定処理
	Move();

	// 寿命を減算する
	m_nLife--;

	if (m_type == TYPE_PLAYER)
	{ // 種類がプレイヤーの弾だった場合

		// 敵との当たり判定
		if (collision::BulletEnemyCollision(*this) == true)
		{ // 当たり判定で当たった場合

			// 着弾処理
			Impact(pos);

			// この先の処理を行わない
			return;
		}
	}

	if (m_type == TYPE_ENEMY)
	{ // 種類が敵の弾だった場合

		// プレイヤーとの当たり判定
		if (collision::BulletPlayerCollision(*this) == true)
		{ // 当たり判定で当たった場合

			// 着弾処理
			Impact(pos);

			// この先の処理を行わない
			return;
		}
	}

	// ブロックの当たり判定処理
	if (collision::BulletBlockCollision(*this) == true)
	{ // ブロックに当たった場合

		// 着弾処理
		Impact(pos);

		// この先の処理を行わない
		return;
	}

	if (m_nLife <= 0)
	{ // 寿命が尽きた場合

		// 着弾処理
		Impact(pos);

		// この先の処理を行わない
		return;
	}

	// 更新
	CBillboard::Update();
}

//===========================================
// 描画処理
//===========================================
void CBullet::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff();
}

//===========================================
// 情報の設定処理
//===========================================
void CBullet::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fMoveRot, TYPE type)
{
	// 全ての値を初期化する
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_type = type;								// 種類
	m_nLife = BULLET_LIFE;						// 寿命
	m_fMoveRot = fMoveRot;						// 移動方向

	// 情報の設定処理
	SetPos(pos);		// 位置
	SetSize(size);		// サイズ

	// 頂点情報の設定
	SetVertex();

	// テクスチャの読み込み処理
	BindTexture(CManager::GetTexture()->Regist(BULLET_TEXTURE));
}

//===========================================
// 移動処理
//===========================================
void CBullet::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	// 移動量を設定する
	m_move.x = sinf(m_fMoveRot) * BULLET_SPEED;
	m_move.z = cosf(m_fMoveRot) * BULLET_SPEED;

	// 位置を更新する
	pos += m_move;

	// 位置を設定する
	SetPos(pos);

	// エフェクトの設定処理
	CEffect::Create
	(
		pos,								// 位置
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 移動量
		30,									// 寿命
		40.0f,								// 半径
		CEffect::TYPE_NONE					// 種類
	);
}

//===========================================
// 着弾処理
//===========================================
void CBullet::Impact(const D3DXVECTOR3& pos)
{
	// 爆発の生成処理
	CExplosion::Create(pos, EXPLOSION_SIZE);

	// 弾の破棄処理
	Uninit();

	// パーティクルの生成処理
	CParticle::Create(pos, CEffect::TYPE_FIRE);
}

//===========================================
// 移動方向の設定処理
//===========================================
void CBullet::SetMoveRot(const float fMoveRot)
{
	// 移動方向を設定する
	m_fMoveRot = fMoveRot;
}

//===========================================
// 移動方向の取得処理
//===========================================
float CBullet::GetMoveRot(void) const
{
	// 移動方向を返す
	return m_fMoveRot;
}

//===========================================
// 移動量の設定処理
//===========================================
void CBullet::SetMove(const D3DXVECTOR3& move)
{
	// 移動量を設定する
	m_move = move;
}

//===========================================
// 移動量の取得処理
//===========================================
D3DXVECTOR3 CBullet::GetMove(void) const
{
	// 移動量を返す
	return m_move;
}

//===========================================
// 生成処理
//===========================================
CBullet* CBullet::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fMoveRot,TYPE type)
{
	// ローカルオブジェクトを生成
	CBullet* pBullet = nullptr;	// プレイヤーのインスタンスを生成

	if (pBullet == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pBullet = new CBullet;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pBullet != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBullet->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "弾の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBullet->SetData(pos, size, fMoveRot, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pBullet;
}