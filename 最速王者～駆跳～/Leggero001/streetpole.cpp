//===========================================
//
// 街灯(支柱)のメイン処理[streetpole.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "game.h"
#include "streetpole.h"
#include "collpolygon.h"
#include "useful.h"

#include "player.h"
#include "motion.h"
#include "orbit.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------

//==============================
// コンストラクタ
//==============================
CStreetPole::CStreetPole() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{

}

//==============================
// デストラクタ
//==============================
CStreetPole::~CStreetPole()
{

}

//==============================
// 街灯(支柱)の初期化処理
//==============================
HRESULT CStreetPole::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
// 街灯(支柱)の終了処理
//========================================
void CStreetPole::Uninit(void)
{
	// 終了処理
	CObstacle::Uninit();
}

//========================================
// 街灯(支柱)の更新処理
//========================================
void CStreetPole::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 前回の位置を設定する
	SetPosOld(pos);

	// 更新処理
	CObstacle::Update();
}

//=====================================
// 街灯(支柱)の描画処理
//=====================================
void CStreetPole::Draw(void)
{
	// 描画処理
	CObstacle::Draw();
}

//=====================================
// ヒット処理
//=====================================
bool CStreetPole::Hit(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = CScene::GetPlayer();			// プレイヤーの情報を取得する
	D3DXVECTOR3 pos;								// プレイヤーの位置
	float fDistance;								// 支柱とプレイヤーとの距離

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		if (pPlayer->GetJump() == true &&
			pPlayer->GetAction()->GetAct() != CPlayerAct::ACT_POLEDOWN &&
			pPlayer->GetAction()->GetAct() != CPlayerAct::ACT_HANGING &&
			pPlayer->GetAction()->GetAct() != CPlayerAct::ACT_LAMPJUMP)
		{ // 空中かつ、ポール下り状態以外かつ、ぶらさがり状態以外かつ、街灯ジャンプ状態以外の場合

			// プレイヤーの位置を取得する
			pos = pPlayer->GetPos();

			// オブジェクトの位置の設定処理
			pPlayer->GetAction()->SetPosObject(GetPos());

			// ポール下り状態にする
			pPlayer->GetAction()->ActionChange(CPlayerAct::ACT_POLEDOWN);

			// 距離を求める
			fDistance = GetPos().x - pos.x;

			// ポール下りモーションを設定する
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_POLEDOWN);

			if (fDistance >= 0.0f)
			{ // 距離の値が 0.0f 以上の場合

				// 位置を設定する
				pos.x -= POLE_PLAYER_DIST;
			}
			else
			{ // 上記以外

				// 位置を設定する
				pos.x += POLE_PLAYER_DIST;
			}

			// 位置の設定処理
			pPlayer->SetPos(pos);
			pPlayer->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			// 軌跡の全頂点の位置の設定
			pPlayer->GetOrbit()->PosSet(pos);

			// true を返す
			return true;
		}
		else
		{ // 上記以外

			// false を返す
			return false;
		}
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CStreetPole::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 初期化処理
	SetFileData(CXFile::TYPE_STREETPOLE);		// データの設定処理

	// 情報の設定処理
	CObstacle::SetData(pos, type);
}