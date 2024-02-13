//=======================================
//
//アイテムのメイン処理[Item.cpp]
//Author 小原立暉
//
//=======================================
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "Item.h"
#include "sound.h"
#include "useful.h"

#include "Particle.h"
#include "player.h"

//---------------------------------------
// マクロ定義
//---------------------------------------
#define ITEM_ROT_MOVE		(0.05f)			// アイテムの向きの移動量

//==========================================
// コンストラクタ
//==========================================
CItem::CItem() : CModel(CObject::TYPE_ITEM, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_state = STATE_APPEAR;							// 状態
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きの移動量
}

//==========================================
// デストラクタ
//==========================================
CItem::~CItem()
{

}

//==========================================
// アイテムの初期化処理
//==========================================
HRESULT CItem::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_state = STATE_APPEAR;							// 状態
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きの移動量

	// 成功を返す
	return S_OK;
}

//========================================
// アイテムの終了処理
//========================================
void CItem::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//========================================
// アイテムの更新処理
//========================================
void CItem::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向き
	D3DXVECTOR3 scale = GetScale();	// 拡大率

	switch (m_state)
	{
	case STATE_APPEAR:		// 出現状態

		// 拡大率を加算する
		scale.x += 0.1f;
		scale.y += 0.1f;
		scale.z += 0.1f;

		if (scale.x >= NONE_SCALE.x)
		{ // 拡大率が 1.0f を超えた場合

			// 拡大率を補正する
			scale = NONE_SCALE;

			// 通常状態にする
			m_state = STATE_NONE;
		}

		break;

	case STATE_NONE:		// 通常状態

		// 向きの移動量を加算する
		rot.y += m_rotmove.y;

		// 向きの正規化
		RotNormalize(&rot.y);

		break;
	}

	// 情報を更新する
	SetRot(rot);			// 向き
	SetScale(scale);		// 拡大率
}

//===========================================
// アイテムの描画処理
//===========================================
void CItem::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//===========================================
// ヒット処理
//===========================================
void CItem::Hit(void)
{
	// パーティクルの生成処理
	CParticle::Create(D3DXVECTOR3(GetPos().x, GetPos().y + GetFileData().vtxMax.y, GetPos().z), CEffect::TYPE_ITEMGET);

	// ドリンクゲット音を鳴らす
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DRINKGET);

	// スタミナの回復処理
	HealingStamina();

	// アイテムの破棄処理
	Uninit();
}

//===========================================
// 情報の設定処理
//===========================================
void CItem::SetData(const D3DXVECTOR3& pos, const STATE state)
{
	// 全ての値を初期化する
	m_state = state;										// 状態
	m_rotmove = D3DXVECTOR3(0.0f, ITEM_ROT_MOVE, 0.0f);		// 向きの移動量

	// スクロールの設定処理
	SetPos(pos);			// 位置設定
	SetPosOld(pos);			// 前回の位置設定
	SetRot(ITEM_ROT);		// 向き設定

	switch (m_state)
	{
	case STATE_APPEAR:		// 出現状態

		// 拡大率設定
		SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		break;

	default:				// 上記以外

		// 拡大率設定
		SetScale(NONE_SCALE);

		break;
	}

	SetFileData(CXFile::TYPE_ENERGYITEM);	// データの設定処理
}

//===========================================
// 生成処理
//===========================================
CItem* CItem::Create(const D3DXVECTOR3& pos, STATE state)
{
	// ローカルオブジェクトを生成
	CItem* pItem = nullptr;	// プレイヤーのインスタンスを生成

	if (pItem == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pItem = new CItem;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pItem != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pItem->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "アイテムの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pItem->SetData(pos, state);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pItem;
}

//===========================================
// スタミナの回復処理
//===========================================
void CItem::HealingStamina(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = nullptr;		// プレイヤーのポインタ

	// プレイヤーの情報を取得する
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーの情報が NULL じゃない場合

		// スタミナの計算処理
		pPlayer->CalcStamina(PLAYER_MAX_STAMINA);
	}
}