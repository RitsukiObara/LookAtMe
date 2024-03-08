//============================================
//
// プレイヤーのメイン処理[player.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "useful.h"

#include "player.h"
#include "player_action.h"
#include "handgun.h"
#include "dagger.h"
#include "aim.h"
#include "bulletUI.h"
#include "gold_bone_UI.h"
#include "player_controller.h"

#include "collision.h"
#include "camera.h"
#include "objectElevation.h"
#include "motion.h"
#include "block.h"
#include "orbit.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
namespace
{
	const float GRAVITY = 1.0f;						// 重力
	const float LAND_GRAVITY = -50.0f;				// 着地時の重力
	const float INIT_POSV_CAMERA_Y = 250.0f;		// カメラの視点のY座標
	const float ROT_CORRECT = 0.2f;					// 向きの補正倍率
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(40.0f, 190.0f, 40.0f);		// 当たり判定時のサイズ
	const D3DXVECTOR3 GUN_POS[NUM_HANDGUN] =		// 拳銃の位置
	{
		D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
		D3DXVECTOR3(+10.0f, 0.0f, 0.0f)
	};
	const D3DXVECTOR3 GUN_ROT[NUM_HANDGUN] =		// 拳銃の向き
	{
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, -D3DX_PI * 0.5f),
		D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, D3DX_PI * 0.5f)
	};
	const int INIT_GUN_IDX = 9;						// 銃の初期値のインデックス
	const int INIT_DAGGER_IDX = 9;					// ダガーのインデックス
	const int MAX_LIFE = 100;						// 体力の最大値
	const int DAMAGE_COUNT = 7;						// ダメージ状態のカウント数
	const D3DXCOLOR DAMAGE_COLOR = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		// ダメージ状態の色
	const int INVINCIBLE_COUNT = 90;				// 無敵状態のカウント数
	const int INVINCIBLE_FLUSH_COUNT = 10;			// 無敵状態の点滅のカウント
}

//=========================================
// コンストラクタ
//=========================================
CPlayer::CPlayer() : CCharacter(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_pMotion = nullptr;					// モーションの情報
	m_pAction = nullptr;					// プレイヤーの行動の情報
	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		m_apHandGun[nCntGun] = nullptr;		// 拳銃の情報
	}
	m_pAim = nullptr;						// エイムの情報
	m_pDagger = nullptr;					// ダガーの情報
	m_pBulletUI = nullptr;					// 弾丸UIの情報
	m_pGoldBoneUI = nullptr;				// 金の骨のUIの情報
	m_pController = nullptr;				// プレイヤーのコントローラーの情報

	m_stateInfo.state = STATE_NONE;			// 状態
	m_stateInfo.nCount = 0;					// 状態カウント
	m_stateInfo.col = NONE_D3DXCOLOR;		// 体の色
	m_rotDest = NONE_D3DXVECTOR3;			// 目標の向き
	m_move = NONE_D3DXVECTOR3;				// 移動量
	m_nLife = MAX_LIFE;						// 体力
	m_bMove = false;						// 移動状況
	m_bJump = false;						// ジャンプ状況
}

//=========================================
// デストラクタ
//=========================================
CPlayer::~CPlayer()
{

}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CPlayer::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// モデルの総数を設定する
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_PLAYER));

	// データの設定処理
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // モーションが NULL だった場合

		// モーションの生成処理
		m_pMotion = CMotion::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pMotion != nullptr)
	{ // ポインタが NULL じゃない場合

		// モーションの情報を取得する
		m_pMotion->SetInfo(CMotion::STYLE_PLAYER, GetHierarchy(), GetNumModel());
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	// モーションの設定処理
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	if (m_pAction == nullptr)
	{ // 行動の情報が NULL の場合

		// プレイヤーアクションの生成処理
		m_pAction = CPlayerAction::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pBulletUI == nullptr)
	{ // 残弾UIが NULL の場合

		// 弾丸UIの生成
		m_pBulletUI = CBulletUI::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pGoldBoneUI == nullptr)
	{ // 金の骨UIが NULL の場合

		// 金の骨のUIの生成
		m_pGoldBoneUI = CGoldBoneUI::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pController == nullptr)
	{ // プレイヤーのコントローラーの情報が NULL の場合

		// プレイヤーコントローラーの生成
		m_pController = CPlayerController::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	// 全ての値を初期化する
	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		m_apHandGun[nCntGun] = nullptr;	// 拳銃の情報
	}
	m_pAim = nullptr;					// エイムの情報
	m_pDagger = nullptr;				// ダガーの情報

	m_stateInfo.state = STATE_NONE;		// 状態
	m_stateInfo.nCount = 0;				// 状態カウント
	m_stateInfo.col = NONE_D3DXCOLOR;	// 体の色
	m_rotDest = NONE_D3DXVECTOR3;		// 目標の向き
	m_move = NONE_D3DXVECTOR3;			// 移動量
	m_nLife = MAX_LIFE;					// 体力
	m_bMove = false;					// 移動状況
	m_bJump = false;					// ジャンプ状況

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CPlayer::Uninit(void)
{
	if (m_pMotion != nullptr)
	{ // モーションが NULL じゃない場合

		// モーションのメモリを開放する
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pAction != nullptr)
	{ // 行動が NULL じゃない場合

		// 行動のメモリを開放する
		m_pAction->Uninit();
		m_pAction = nullptr;
	}

	if (m_pAim != nullptr)
	{ // エイムが NULL じゃない場合

		// エイムのメモリを開放する
		m_pAim->Uninit();
		m_pAim = nullptr;
	}

	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		if (m_apHandGun[nCntGun] != nullptr)
		{ // 拳銃の情報が NULL じゃない場合

			// 拳銃の終了処理
			m_apHandGun[nCntGun]->Uninit();
			m_apHandGun[nCntGun] = nullptr;
		}
	}

	if (m_pDagger != nullptr)
	{ // ダガーの情報が NULL じゃない場合

		// ダガーの終了処理
		m_pDagger->Uninit();
		m_pDagger = nullptr;
	}

	if (m_pBulletUI != nullptr)
	{ // 残弾UIの情報が NULL じゃない場合

		// 残弾UIを NULL にする
		m_pBulletUI = nullptr;
	}

	if (m_pGoldBoneUI != nullptr)
	{ // 金の骨の情報が NULL じゃない場合

		// 金の骨UIを NULL にする
		m_pGoldBoneUI = nullptr;
	}

	if (m_pController != nullptr)
	{ // コントローラーの情報が NULL じゃない場合

		// 終了処理
		m_pController->Uninit();
		m_pController = nullptr;
	}

	// 終了処理
	CCharacter::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CPlayer::Update(void)
{
	// 前回の位置の設定処理
	SetPosOld(GetPos());

	// 状態管理処理
	StateManager();

	// 操作処理
	m_pController->Control(this);

	// 移動処理
	Move();

	if (m_pMotion != nullptr)
	{ // モーションが NULL じゃない場合

		// モーションの更新処理
		m_pMotion->Update();
	}

	if (m_pAction != nullptr)
	{ // 行動が NULL じゃない場合

		// 行動の更新処理
		m_pAction->Update(this);
	}

	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		if (m_apHandGun[nCntGun] != nullptr)
		{ // 拳銃が NULL じゃない場合

			// 更新処理
			m_apHandGun[nCntGun]->Update();
		}
	}

	if (m_pAim != nullptr)
	{ // エイムが NULL じゃない場合

		// エイムの更新処理
		m_pAim->Update();
	}

	// 小判との当たり判定
	collision::CoinCollision(GetPos(), COLLISION_SIZE);

	// 木との当たり判定
	TreeCollision();

	// 起伏地面との当たり判定処理
	ElevationCollision();

	// ブロックとの当たり判定処理
	BlockCollision();

	// 岩との当たり判定
	RockCollision();

	// 金の骨との当たり判定
	collision::GoldBoneCollision(*this, COLLISION_SIZE);

	CManager::Get()->GetDebugProc()->Print("位置：%f %f %f", GetPos().x, GetPos().y, GetPos().z);
}

//===========================================
// プレイヤーの描画処理
//===========================================
void CPlayer::Draw(void)
{
	switch (m_stateInfo.state)
	{
	case STATE_NONE:		// 通常状態
	case STATE_DEATH:		// 死亡状態

		// 描画処理
		CCharacter::Draw();

		break;

	case STATE_DAMAGE:		// ダメージ状態

		// 描画処理
		CCharacter::Draw(m_stateInfo.col);

		break;

	case STATE_INVINSIBLE:	// 無敵状態

		// 描画処理
		CCharacter::Draw(m_stateInfo.col.a);

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		if (m_apHandGun[nCntGun] != nullptr)
		{ // 拳銃の情報が NULL じゃない場合

			// 拳銃の描画処理
			m_apHandGun[nCntGun]->Draw();
		}
	}

	if (m_pDagger != nullptr)
	{ // 拳銃の情報が NULL じゃない場合

		// 拳銃の描画処理
		m_pDagger->Draw();
	}

	if (m_pAim != nullptr)
	{ // エイムの情報が NULL じゃない場合

		// エイムの描画処理
		m_pAim->Draw();
	}
}

//===========================================
// ヒット処理
//===========================================
void CPlayer::Hit(const int nDamage)
{
	// 体力を減らす
	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{ // 体力が0以下になった場合

		// 体力を0にする
		m_nLife = 0;

		// 死亡状態にする
		m_stateInfo.state = STATE_DEATH;
	}
	else
	{ // 上記以外

		// ダメージ状態にする
		m_stateInfo.state = STATE_DAMAGE;

		// カウントを0にする
		m_stateInfo.nCount = 0;
	}
}

//===========================================
// モーションの情報の設定処理
//===========================================
CMotion* CPlayer::GetMotion(void) const
{
	// モーションの情報を返す
	return m_pMotion;
}

//===========================================
// 行動の情報の取得処理
//===========================================
CPlayerAction* CPlayer::GetAction(void) const
{
	// 行動の情報を返す
	return m_pAction;
}

//===========================================
// 拳銃の情報の取得処理
//===========================================
CHandgun* CPlayer::GetHandGun(const int nCount) const
{
	// 拳銃の情報を返す
	return m_apHandGun[nCount];
}

//===========================================
// ダガーの情報の取得処理
//===========================================
CDagger* CPlayer::GetDagger(void) const
{
	// ダガーの情報を返す
	return m_pDagger;
}

//===========================================
// エイムの取得処理
//===========================================
CAim* CPlayer::GetAim(void) const
{
	// エイムの情報を返す
	return m_pAim;
}

//===========================================
// 残弾UIの情報の取得処理
//===========================================
CBulletUI* CPlayer::GetBulletUI(void) const
{
	// 残弾UIの情報を返す
	return m_pBulletUI;
}

//===========================================
// 金の骨UIの情報の取得処理
//===========================================
CGoldBoneUI* CPlayer::GetGoldBoneUI(void) const
{
	// 金の骨UIの情報を返す
	return m_pGoldBoneUI;
}

//=======================================
// 情報の設定処理
//=======================================
void CPlayer::SetData(const D3DXVECTOR3& pos)
{
	// 全ての値を初期化する
	SetPos(pos);					// 位置
	SetPosOld(GetPos());			// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetScale(NONE_SCALE);			// 拡大率

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// 初期化処理
		GetHierarchy(nCntData)->SetPos(pos);					// 位置
		GetHierarchy(nCntData)->SetPosOld(pos);					// 前回の位置
		GetHierarchy(nCntData)->SetRot(NONE_D3DXVECTOR3);		// 向き
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);			// 拡大率
	}

	// モーションの設定処理
	m_pMotion->ResetMotion(MOTIONTYPE_NEUTRAL);

	for (int nCnt = 0; nCnt < NUM_HANDGUN; nCnt++)
	{
		// 拳銃の情報を生成する
		m_apHandGun[nCnt] = CHandgun::Create(GUN_POS[nCnt], GUN_ROT[nCnt], GetHierarchy(INIT_GUN_IDX + nCnt)->GetMatrixP());
	}

	// ダガーを生成する
	m_pDagger = CDagger::Create(GetHierarchy(INIT_DAGGER_IDX)->GetMatrixP());

	// エイムを生成する
	m_pAim = CAim::Create(GetPos());
	
	// 全ての値を設定する
	m_stateInfo.state = STATE_NONE;		// 状態
	m_stateInfo.nCount = 0;				// 状態カウント
	m_stateInfo.col = NONE_D3DXCOLOR;	// 体の色
	m_rotDest = NONE_D3DXVECTOR3;		// 目標の向き
	m_move = NONE_D3DXVECTOR3;			// 移動量
	m_nLife = MAX_LIFE;					// 体力
	m_bMove = false;					// 移動状況
	m_bJump = false;					// ジャンプ状況
}

//===========================================
// 生成処理
//===========================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos)
{
	// プレイヤーのインスタンスを生成
	CPlayer* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pPlayer = new CPlayer;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pPlayer;
	}

	if (pPlayer != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPlayer->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pPlayer;
}

//=======================================
// 目的の向きの設定処理
//=======================================
void CPlayer::SetRotDest(const D3DXVECTOR3& rot)
{
	// 目的の向きを設定する
	m_rotDest = rot;
}

//=======================================
// 目的の向きの取得処理
//=======================================
D3DXVECTOR3 CPlayer::GetRotDest(void) const
{
	// 目的の向きを返す
	return m_rotDest;
}

//=======================================
// 移動量の設定処理
//=======================================
void CPlayer::SetMove(const D3DXVECTOR3& move)
{
	// 移動量を設定する
	m_move = move;
}

//=======================================
// 移動量の取得処理
//=======================================
D3DXVECTOR3 CPlayer::GetMove(void) const
{
	// 移動量を返す
	return m_move;
}

//=======================================
// 状態の取得処理
//=======================================
CPlayer::SState CPlayer::GetState(void) const
{
	// 状態関連の情報を返す
	return m_stateInfo;
}

//=======================================
// ジャンプ状況の設定処理
//=======================================
void CPlayer::SetEnableJump(const bool bJump)
{
	// ジャンプ状況を設定する
	m_bJump = bJump;
}

//=======================================
// ジャンプ状況の取得処理
//=======================================
bool CPlayer::IsJump(void) const
{
	//ジャンプ状況を返す
	return m_bJump;
}

//=======================================
// 起伏地面の当たり判定処理
//=======================================
void CPlayer::ElevationCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ
	bool bJump = true;				// ジャンプ状況
	bool bRange = false;			// 範囲内状況
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// 先頭の小判
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
			fHeight = pElev->ElevationCollision(pos, &bRange);

			if (pos.y < fHeight)
			{ // 当たり判定の位置が高かった場合

				// 高さを設定する
				pos.y = fHeight;

				// 重力を設定する
				m_move.y = LAND_GRAVITY;

				// ジャンプ状況を false にする
				bJump = false;
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

	// ジャンプ状況を代入する
	m_bJump = bJump;

	// 位置を更新する
	SetPos(pos);
}

//=======================================
// 状態管理処理
//=======================================
void CPlayer::StateManager(void)
{
	switch (m_stateInfo.state)
	{
	case CPlayer::STATE_NONE:


		break;

	case CPlayer::STATE_DAMAGE:

		// 状態カウントを加算する
		m_stateInfo.nCount++;

		// 体の色を変える
		m_stateInfo.col = DAMAGE_COLOR;

		if (m_stateInfo.nCount >= DAMAGE_COUNT)
		{ // 状態カウントが一定数以上になった場合

			// 状態カウントを0にする
			m_stateInfo.nCount = 0;

			// 体の色を元に戻す
			m_stateInfo.col = NONE_D3DXCOLOR;

			// 無敵状態にする
			m_stateInfo.state = STATE_INVINSIBLE;
		}

		break;

	case CPlayer::STATE_INVINSIBLE:

		// 状態カウントを加算する
		m_stateInfo.nCount++;

		if (m_stateInfo.nCount % INVINCIBLE_FLUSH_COUNT == 0)
		{ // カウントが一定数以上になった場合

			// 色を変える
			m_stateInfo.col.a = (m_stateInfo.col.a >= 1.0f) ? 0.0f : 1.0f;
		}

		if (m_stateInfo.nCount >= INVINCIBLE_COUNT)
		{ // 状態カウントが一定数以上になった場合

			// 状態カウントを0にする
			m_stateInfo.nCount = 0;

			// 通常状態にする
			m_stateInfo.state = STATE_NONE;
		}

		break;

	case CPlayer::STATE_DEATH:


		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=======================================
// 移動処理
//=======================================
void CPlayer::Move(void)
{
	// 位置と向きを取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 向きの補正処理
	useful::RotCorrect(m_rotDest.y, &rot.y, ROT_CORRECT);

	if (m_pAction->IsRecoil() == false)
	{ // 反動状況が false の場合

		// 位置を移動させる
		pos.x += m_move.x;
		pos.z += m_move.z;
	}

	// 重力をかける
	useful::Gravity(&m_move.y, &pos.y, GRAVITY);

	// 位置と向きを適用する
	SetPos(pos);
	SetRot(rot);
}

//=======================================
// 木との当たり判定
//=======================================
void CPlayer::TreeCollision(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 木との当たり判定
	collision::TreeCollision(&pos, COLLISION_SIZE.x);

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// ブロックとの当たり判定
//=======================================
void CPlayer::BlockCollision(void)
{
	// ローカル変数宣言
	collision::SCollision coll = { false,false,false,false,false,false };				// 当たり判定の変数
	D3DXVECTOR3 pos = GetPos();							// 位置を取得する
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-COLLISION_SIZE.x, 0.0f, -COLLISION_SIZE.z);		// 最小値を取得する
	D3DXVECTOR3 vtxMax = COLLISION_SIZE;				// 最大値を取得する
	CListManager<CBlock*> list = CBlock::GetList();
	CBlock* pBlock = nullptr;		// 先頭の値
	CBlock* pBlockEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBlock = list.GetTop();

		// 末尾の値を取得する
		pBlockEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 六面体の当たり判定
			coll = collision::HexahedronClush
			(
				&pos,
				pBlock->GetPos(),
				GetPosOld(),
				pBlock->GetPosOld(),
				vtxMin,
				pBlock->GetVtxMin(),
				vtxMax,
				pBlock->GetVtxMax()
			);

			if (coll.bTop == true)
			{ // 上に乗った場合

				// 移動量を設定する
				m_move.y = 0.0f;

				// ジャンプしていない
				m_bJump = false;
			}

			if (pBlock == pBlockEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBlock = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 岩との当たり判定
//=======================================
void CPlayer::RockCollision(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 岩との当たり判定
	collision::RockCollision(&pos, COLLISION_SIZE.x, COLLISION_SIZE.y);

	// 位置の設定処理
	SetPos(pos);
}