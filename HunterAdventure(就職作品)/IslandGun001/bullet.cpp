//=======================================
//
// 弾のメイン処理[bullet.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "bullet.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "locus3D.h"
#include "collision.h"

//=======================================
// 無名名前空間
//=======================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Bullet.png";		// 弾のテクスチャ
	const D3DXVECTOR3 RADIUS[CBullet::TYPE_MAX] =
	{
		D3DXVECTOR3(20.0f,20.0f,0.0f),		// 二丁拳銃
		D3DXVECTOR3(10.0f,10.0f,0.0f),		// 散弾銃
	};
	const float SPEED = 45.0f;		// 速度
	const int LIFE = 90;			// 寿命
	const int DAMAGE[CBullet::TYPE_MAX] =	// ダメージ
	{
		8,
		2,
	};
	const float KNOCKBACK[CBullet::TYPE_MAX] =	// ノックバックの値
	{
		2.0f,
		15.0f,
	};
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CBullet*> CBullet::m_list = {};		// リスト

//=========================
// コンストラクタ
//=========================
CBullet::CBullet() : CBillboard(CObject::TYPE_BULLET, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_move = NONE_D3DXVECTOR3;	// 移動量
	m_type = TYPE_HANDGUN;		// 種類
	m_nLife = LIFE;				// 寿命
	m_nDamage = 0;				// ダメージ
	m_fKnockBack = 0.0f;		// ノックバック値

	// リストに追加する
	m_list.Regist(this);
}

//=========================
// デストラクタ
//=========================
CBullet::~CBullet()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CBullet::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_move = NONE_D3DXVECTOR3;	// 移動量
	m_type = TYPE_HANDGUN;		// 種類
	m_nLife = LIFE;				// 寿命
	m_nDamage = 0;				// ダメージ
	m_fKnockBack = 0.0f;		// ノックバック値

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CBullet::Uninit(void)
{
	// 終了
	CBillboard::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//=========================
// 更新処理
//=========================
void CBullet::Update(void)
{
	// 前回の位置を保存する
	SetPosOld(GetPos());

	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 移動量を設定する
	pos += m_move;

	// 位置を適用する
	SetPos(pos);

	// 寿命を減算する
	m_nLife--;

	if (m_nLife % 2 == 0)
	{ // 一定時間ごとに

		// 残像の生成
		CLocus3D::Create(GetPos(), GetSize(), 0.5f, 20, GetTexIdx(), true, true);
	}

	if (m_nLife <= 0)
	{ // 寿命が0になった場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	if (Hit() == true)
	{ // 何かに当たった場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点座標の設定処理
	SetVertex();
}

//=========================
// 描画処理
//=========================
void CBullet::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff(true);
}

//=========================
// 情報の設定処理
//=========================
void CBullet::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// 情報の設定処理
	SetPos(pos);			// 位置
	SetPosOld(pos);			// 前回の位置
	SetRot(0.0f);			// 向き
	SetSize(RADIUS[type]);	// サイズ
	SetAngle();				// 方向
	SetLength();			// 長さ
	SetEnableLookY(true);	// Y軸状況

	// 全ての値を設定する
	m_type = type;				// 種類
	m_nLife = LIFE;				// 寿命
	m_nDamage = DAMAGE[m_type];	// ダメージ
	m_fKnockBack = KNOCKBACK[m_type];		// ノックバック値

	// 移動量を設定する
	m_move.x = sinf(rot.y) * SPEED;	// X軸
	m_move.y = cosf(rot.x) * SPEED;	// Y軸
	m_move.z = cosf(rot.y) * SPEED;	// Z軸

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
}

//=========================
// 生成処理
//=========================
CBullet* CBullet::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ローカルオブジェクトを生成
	CBullet* pBullet = nullptr;	// 弾のインスタンスを生成

	if (pBullet == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pBullet = new CBullet;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBullet != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBullet->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBullet->SetData(pos, rot, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 弾のポインタを返す
	return pBullet;
}

//=========================
// ダメージの取得処理
//=========================
int CBullet::GetDamage(void) const
{
	// ダメージを返す
	return m_nDamage;
}

//=========================
// ノックバック値の取得処理
//=========================
float CBullet::GetKnockBack(void) const
{
	// ノックバック値を返す
	return m_fKnockBack;
}

//=========================
// リストの取得処理
//=========================
CListManager<CBullet*> CBullet::GetList(void)
{
	// リストマネージャーを返す
	return m_list;
}

//=========================
// ヒット処理
//=========================
bool CBullet::Hit(void)
{
	D3DXVECTOR3 pos = GetPos();			// 位置
	D3DXVECTOR3 posOld = GetPosOld();	// 前回の位置
	D3DXVECTOR3 size = GetSize();		// サイズ
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(size.x, size.y, size.x);		// 頂点の最大値
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-size.x, -size.y, -size.x);	// 頂点の最小値

	// 敵と銃の当たり判定
	if (collision::EnemyHitToGun(*this) == true ||
		collision::BangFlowerHit(pos, size.x, size.y) == true ||
		collision::BombHitToGun(pos, posOld, size.x) == true ||
		collision::BlockHit(&pos, posOld, vtxMax, vtxMin) == true ||
		collision::RockCollision(&pos, vtxMax.x, vtxMax.y) == true ||
		collision::TreeCollision(&pos, vtxMax.x) == true ||
		collision::WallCollision(&pos, posOld, vtxMax, vtxMin) == true)
	{ // 敵に当たった場合

		// true を返す
		return true;
	}

	// false を返す
	return false;
}