//===========================================
//
// 風攻撃のメイン処理[wind_shot.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "wind_shot.h"
#include "texture.h"
#include "useful.h"

#include "game.h"
#include "player.h"
#include "objectElevation.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* TORNADO_TEXTURE = "data\\TEXTURE\\Wind.png";		// テクスチャ
	const float TORNADO_WIDTH = 100.0f;			// トルネードの幅
	const float TORNADO_HEIGHT = 200.0f;		// トルネードの高さ
	const int TORNADO_VORTEX = 6;				// トルネードの段数
	const float TORNADO_SHIFT = 100.0f;			// トルネードのずらす幅
	const float TORNADO_CIRCUM = 100.0f;		// トルネードの円周
	const CMesh::SGrid TORNADO_DIVI = { 8,1 };	// トルネードの分割数

	const float HOMING_CORRECT = 0.03f;			// ホーミング時の向きの補正係数
	const float HOMING_SPEED = 25.0f;			// ホーミング時の速度
	const float GRAVITY = -25.0f;				// 重力
	const float ADD_ROT = 0.05f;				// 向きの加算量
}

// 静的メンバ変数
CListManager<CWindShot*> CWindShot::m_list = {};				// リスト

//==============================
// コンストラクタ
//==============================
CWindShot::CWindShot() : CMeshTornado(CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_state = STATE_ATTACK;			// 状態
	m_fRotMove = 0.0f;				// 移動方向
	m_fAlpha = 1.0f;				// 透明度

	// 種類を設定する
	CObject::SetType(TYPE_WINDSHOT);

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CWindShot::~CWindShot()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CWindShot::Init(void)
{
	if (FAILED(CMeshTornado::Init()))
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
void CWindShot::Uninit(void)
{
	// 終了処理
	CMeshTornado::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
//ブロックの更新処理
//========================================
void CWindShot::Update(void)
{
	switch (m_state)
	{
	case CWindShot::STATE_ATTACK:

		// 攻撃状態処理
		Attack();

		break;

	case CWindShot::STATE_DELETE:

		// 透明度を減算する
		m_fAlpha -= 0.05f;

		// 頂点カラーの設定処理
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

		if (m_fAlpha <= 0.0f)
		{ // 透明度が0以下になった場合

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

	// 向き移動処理
	RotMove();

	// 起伏地面との当たり判定
	ElevationCollision();
}

//=====================================
//ブロックの描画処理
//=====================================
void CWindShot::Draw(void)
{
	// 描画処理
	CMeshTornado::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CWindShot::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	CMeshTornado::SetData
	(
		pos,
		NONE_D3DXVECTOR3,
		TORNADO_WIDTH,
		TORNADO_HEIGHT,
		TORNADO_VORTEX,
		TORNADO_SHIFT,
		TORNADO_CIRCUM,
		TORNADO_DIVI
	);

	BindTexture(CManager::Get()->GetTexture()->Regist(TORNADO_TEXTURE));		// テクスチャの割り当て処理
	SetLighting(true);		// ライティングの設定処理
	SetCulling(true);		// カリングの設定処理

	// 頂点座標の設定処理
	SetVertex();

	// 全ての値を設定する
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_state = STATE_ATTACK;			// 状態
	m_fRotMove = 0.0f;				// 移動方向
	m_fAlpha = 1.0f;				// 透明度
}

//=======================================
// 生成処理
//=======================================
CWindShot* CWindShot::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CWindShot* pWind = nullptr;		// インスタンスを生成

	if (pWind == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pWind = new CWindShot;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pWind != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 情報の設定処理
		pWind->SetData(pos);

		// 初期化処理
		if (FAILED(pWind->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 風のポインタを返す
	return pWind;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CWindShot*> CWindShot::GetList(void)
{
	// リストの情報を返す
	return m_list;
}

//=======================================
// 状態の設定処理
//=======================================
void CWindShot::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CWindShot::STATE CWindShot::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 起伏地面との当たり判定
//=======================================
void CWindShot::ElevationCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;	// 先頭の小判
	CElevation* pElevEnd = nullptr;	// 末尾の値
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
}

//=======================================
// 向き移動処理
//=======================================
void CWindShot::RotMove(void)
{
	// 向きを取得する
	D3DXVECTOR3 rot = GetRot();

	// 向きを加算する
	rot.y += ADD_ROT;

	// 向きの正規化
	useful::RotNormalize(&rot.y);

	// 向きを適用する
	SetRot(rot);
}

//=======================================
// 攻撃状態処理
//=======================================
void CWindShot::Attack(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		D3DXVECTOR3 pos = GetPos();
		float fRotDest = atan2f(posPlayer.x - pos.x, posPlayer.z - pos.z);

		// 向きの補正処理
		useful::RotCorrect(fRotDest, &m_fRotMove, HOMING_CORRECT);

		// 移動量を設定する
		m_move.x = sinf(m_fRotMove) * HOMING_SPEED;
		m_move.y = GRAVITY;
		m_move.z = cosf(m_fRotMove) * HOMING_SPEED;

		// 移動する
		pos += m_move;

		// 位置を適用する
		SetPos(pos);
	}
}