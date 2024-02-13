//===========================================
//
// 爆弾のメイン処理[bomb.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "bomb.h"
#include "texture.h"
#include "collision.h"
#include "useful.h"

#include "bomb_fuse.h"
#include "objectElevation.h"
#include "bomb_explosion.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\BombBody.x";		// モデルの名前
	const float GROWTH_SCALE_DEST = 1.0f;				// 成長時の目標の拡大率
	const float GROWTH_SCALE_ADD = 0.01f;				// 成長時の追加の拡大率
	const float GRAVITY = 0.5f;							// 重力
	const float BOUND_GRAVITY_MAGNI = -0.7f;			// バウンドの重力の倍率
	const int BOUND_COUNT = 3;							// バウンドの回数
	const float ROT_DEST = 0.0f;						// 目的の向き
	const float ROT_CORRECT = 0.1f;						// 向きの補正倍率
	const float FUSE_EXPLOSION_POS_Y = 81.0f;			// 導火線の爆発する位置
	const float EXPLO_SCALE_MAGNI = 0.003f;				// 爆発時の拡大率の倍率
	const int EXPLO_COUNT = 240;						// 爆発状態のカウント
	const float EXPLO_SCALE_MAX = 3.0f;					// 爆発時の拡大率の最大値
	const float EXPLO_SCALE_MIN = 0.1f;					// 爆発時の拡大率の最小値
	const D3DXCOLOR COL_ADD =							// 色の加算数
	{
		0.834f / EXPLO_COUNT,
		-0.166f / EXPLO_COUNT,
		-0.166f / EXPLO_COUNT,
		0.0f
	};
	const float SMASH_HEIGHT = 8.0f;			// 吹き飛ぶ高さ
	const float SMASH_FUSE_SUB_MABNI = 0.1f;	// 吹き飛び時の導火線の減算量
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CBomb*> CBomb::m_list = {};			// リスト

//==============================
// コンストラクタ
//==============================
CBomb::CBomb() : CModel(TYPE_NONE, PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pFuse = nullptr;			// 導火線の情報
	m_col = NONE_D3DXCOLOR;		// 色
	m_move = NONE_D3DXVECTOR3;	// 移動量
	m_state = STATE_GROWTH;		// 状態
	m_nBoundCount = 0;			// バウンドカウント
	m_nExplosionCount = 0;		// 爆発カウント
	m_bAdd = false;				// 加算状況

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CBomb::~CBomb()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CBomb::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CBomb::Uninit(void)
{
	if (m_pFuse != nullptr)
	{ // 導火線が NULL じゃない場合

		// 終了処理
		m_pFuse->Uninit();
		m_pFuse = nullptr;
	}

	// 終了処理
	CModel::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
//ブロックの更新処理
//========================================
void CBomb::Update(void)
{
	// 前回の位置を設定する
	SetPosOld(GetPos());

	switch (m_state)
	{
	case CBomb::STATE_GROWTH:		// 成長状態

		// 成長処理
		Growth();

		break;

	case CBomb::STATE_RIPEN:		// 実り状態

		break;

	case CBomb::STATE_BOUND:		// バウンド状態

		// バウンド処理
		Bound();

		break;

	case CBomb::STATE_DETONATION:	// 起爆状態

		// 起爆状態処理
		Detonation();

		// 移動処理
		Move();

		// 重力処理
		Gravity();

		// 当たり判定
		Collision();

		// 起伏地面との当たり判定
		ElevationCollision();

		break;

	case CBomb::STATE_EXPLOSION:	// 爆発状態

		// 爆発状態処理
		if (Explosion() == true)
		{
			// 爆発の生成
			CBombExplosion::Create(GetPos());
				
			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=====================================
//ブロックの描画処理
//=====================================
void CBomb::Draw(void)
{
	switch (m_state)
	{
	case CBomb::STATE_EXPLOSION:

		// 描画処理
		CModel::Draw(m_col);

		break;

	default:

		// 描画処理
		CModel::Draw();

		break;
	}

	if (m_pFuse != nullptr)
	{ // 導火線が NULL じゃない場合

		// 導火線の描画処理
		m_pFuse->Draw();
	}
}

//=====================================
// ヒット処理
//=====================================
void CBomb::Hit(const float fRot, const float fSpeed)
{
	// 移動量を設定する
	m_move.x = sinf(fRot) * fSpeed;
	m_move.y = SMASH_HEIGHT;
	m_move.z = cosf(fRot) * fSpeed;

	// 位置を取得する
	D3DXVECTOR3 pos = m_pFuse->GetPos();

	// 位置を下げる
	pos.y -= fSpeed * SMASH_FUSE_SUB_MABNI;

	// 位置を適用する
	m_pFuse->SetPos(pos);
}

//=====================================
// 情報の設定処理
//=====================================
void CBomb::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(rot);					// 向き
	SetScale(NONE_D3DXVECTOR3);		// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデルの情報

	// 全ての値を設定する
	m_pFuse = CBombFuse::Create(GetMatrixPoint());		// 導火線の情報
	m_col = NONE_D3DXCOLOR;			// 色
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_state = STATE_GROWTH;			// 状態
	m_nBoundCount = 0;				// バウンドカウント
	m_nExplosionCount = 0;			// 爆発カウント
	m_bAdd = false;					// 加算状況
}

//=======================================
// 生成処理
//=======================================
CBomb* CBomb::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカルオブジェクトを生成
	CBomb* pBomb = nullptr;			// 爆弾を生成

	if (pBomb == nullptr)
	{ // オブジェクトが NULL の場合

		// ブロックを生成する
		pBomb = new CBomb;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBomb != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBomb->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBomb->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 爆弾のポインタを返す
	return pBomb;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CBomb*> CBomb::GetList(void)
{
	// リストマネージャーを返す
	return m_list;
}

//=======================================
// 状態の設定処理
//=======================================
void CBomb::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CBomb::STATE CBomb::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 向きの移動処理
//=======================================
void CBomb::RotMove(void)
{
	// 向きを取得する
	D3DXVECTOR3 rot = GetRot();

	// 補正処理
	useful::FrameCorrect(ROT_DEST, &rot.x, ROT_CORRECT);

	// 向きの正規化
	useful::RotNormalize(&rot.x);

	// 向きを適用する
	SetRot(rot);
}

//=======================================
// 移動処理
//=======================================
void CBomb::Move(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 移動させる
	pos.x += m_move.x;
	pos.z += m_move.z;

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 重力処理
//=======================================
void CBomb::Gravity(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 重力処理
	useful::Gravity(&m_move.y, &pos.y, GRAVITY);

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 起伏地面の当たり判定処理
//=======================================
bool CBomb::ElevationCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ

	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;		// 先頭の小判
	CElevation* pElevEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pElev = list.GetTop();

		// 末尾の値を取得する
		pElevEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 当たり判定を取る
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // 当たり判定の位置が高かった場合

				// 高さを設定する
				pos.y = fHeight;

				// 位置を更新する
				SetPos(pos);

				// 移動量を0にする
				m_move = NONE_D3DXVECTOR3;

				// true を返す
				return true;
			}

			if (pElev == pElevEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pElev = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// 位置を更新する
	SetPos(pos);

	// false を返す
	return false;
}

//=======================================
// 当たり判定処理
//=======================================
void CBomb::Collision(void)
{
	D3DXVECTOR3 pos = GetPos();					// 位置
	D3DXVECTOR3 posOld = GetPosOld();			// 前回の位置
	D3DXVECTOR3 vtxMax = GetFileData().vtxMax;	// 最大値
	D3DXVECTOR3 vtxMin = GetFileData().vtxMin;	// 最小値

	// 当たり判定
	collision::RockCollision(&pos, vtxMax.x, vtxMax.y);
	collision::TreeCollision(&pos, vtxMax.x);
	collision::WallCollision(&pos, posOld, vtxMax, vtxMin);
	collision::BlockHit(&pos, posOld, vtxMax, vtxMin);

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 成長状態処理
//=======================================
void CBomb::Growth(void)
{
	// 拡大率を取得する
	D3DXVECTOR3 scale = GetScale();

	// 拡大する
	if (useful::FrameCorrect(GROWTH_SCALE_DEST, &scale.x, GROWTH_SCALE_ADD) == true ||
		useful::FrameCorrect(GROWTH_SCALE_DEST, &scale.y, GROWTH_SCALE_ADD) == true ||
		useful::FrameCorrect(GROWTH_SCALE_DEST, &scale.z, GROWTH_SCALE_ADD) == true)
	{ // 拡大が終わった場合

		// 実り状態にする
		m_state = STATE_RIPEN;

		// 拡大率を設定する
		scale = NONE_SCALE;
	}

	// 拡大率を設定する
	SetScale(scale);
}

//=======================================
// バウンド状態処理
//=======================================
void CBomb::Bound(void)
{
	// 向きの移動処理
	RotMove();

	// 重力処理
	Gravity();

	if (ElevationCollision() == true)
	{ // 起伏地面に当たった場合

		// バウンド回数を加算する
		m_nBoundCount++;

		// バウンドさせる
		m_move.y *= BOUND_GRAVITY_MAGNI;
	}

	if (m_nBoundCount >= BOUND_COUNT)
	{ // バウンドカウントが一定数以上になった場合

		// バウンドカウントを 0 にする
		m_nBoundCount = 0;

		// 重力を 0 にする
		m_move.y = 0.0f;

		// 起爆状態にする
		m_state = STATE_DETONATION;
	}
}

//=======================================
// 起爆状態処理
//=======================================
void CBomb::Detonation(void)
{
	if (m_pFuse != nullptr)
	{ // 導火線が NULL じゃない場合

		// 導火線の更新処理
		m_pFuse->Update();

		if (m_pFuse->GetPos().y <= FUSE_EXPLOSION_POS_Y)
		{ // 導火線が一定の位置まで下がった場合

			// 終了処理
			m_pFuse->Uninit();
			m_pFuse = nullptr;

			// 起爆状態にする
			m_state = STATE_EXPLOSION;

			// 色を取得する
			m_col = GetMaterial(0).MatD3D.Diffuse;
		}
	}
}

//=======================================
// 爆発状態処理
//=======================================
bool CBomb::Explosion(void)
{
	// 拡大率を取得する
	D3DXVECTOR3 scale = GetScale();

	// 爆発カウントを加算する
	m_nExplosionCount++;

	if (m_bAdd == true)
	{ // 加算状況が true の場合
		
		// 拡大する
		scale.x += EXPLO_SCALE_MAGNI * m_nExplosionCount;
		scale.y += EXPLO_SCALE_MAGNI * m_nExplosionCount;
		scale.z += EXPLO_SCALE_MAGNI * m_nExplosionCount;

		if (scale.x >= EXPLO_SCALE_MAX ||
			scale.y >= EXPLO_SCALE_MAX ||
			scale.z >= EXPLO_SCALE_MAX)
		{ // 拡大率が超えた場合

			// 減算状況にする
			m_bAdd = false;

			// 拡大率を補正する
			scale = NONE_SCALE;
		}
	}
	else
	{ // 上記以外

		// 縮小する
		scale.x -= EXPLO_SCALE_MAGNI * m_nExplosionCount;
		scale.y -= EXPLO_SCALE_MAGNI * m_nExplosionCount;
		scale.z -= EXPLO_SCALE_MAGNI * m_nExplosionCount;

		if (scale.x <= EXPLO_SCALE_MIN ||
			scale.y <= EXPLO_SCALE_MIN ||
			scale.z <= EXPLO_SCALE_MIN)
		{ // 拡大率が超えた場合

			// 加算状況にする
			m_bAdd = true;

			// 拡大率を補正する
			scale = NONE_D3DXVECTOR3;
		}
	}

	// 拡大率を適用する
	SetScale(scale);

	// 色を設定する
	m_col += COL_ADD;

	if (m_nExplosionCount >= EXPLO_COUNT)
	{ // 爆発カウントが一定数になった場合

		// true を返す
		return true;
	}

	// false を返す
	return false;
}