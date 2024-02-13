//===========================================
//
// 街灯(光源)のメイン処理[streetlamp.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "streetlamp.h"
#include "manager.h"
#include "game.h"
#include "Effect.h"
#include "Particle.h"
#include "player.h"
#include "motion.h"
#include "renderer.h"
#include "sound.h"
#include "useful.h"

#include "orbit.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------

//==============================
// コンストラクタ
//==============================
CStreetLamp::CStreetLamp() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{

}

//==============================
// デストラクタ
//==============================
CStreetLamp::~CStreetLamp()
{

}

//==============================
// 街灯(光源)の初期化処理
//==============================
HRESULT CStreetLamp::Init(void)
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
// 街灯(光源)の終了処理
//========================================
void CStreetLamp::Uninit(void)
{
	// 終了処理
	CObstacle::Uninit();
}

//========================================
// 街灯(光源)の更新処理
//========================================
void CStreetLamp::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 前回の位置を設定する
	SetPosOld(pos);

	// プレイヤーの情報を取得する
	CPlayer* pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		if (pPlayer->GetStreetLamp() == this)
		{ // ぶら下がっている街灯と一致した場合

			// 向きを設定する
			rot.x = (fabsf(pPlayer->GetRot().x) - (D3DX_PI * 0.5f)) * 0.1f;
		}
		else
		{ // 上記以外

			// 向きを設定する
			rot.x = 0.0f;
		}
	}
	else
	{ // 上記以外

		// 向きを設定する
		rot.x = 0.0f;
	}

	// 向きを更新する
	SetRot(rot);

	// 更新処理
	CObstacle::Update();
}

//=====================================
// 街灯(光源)の描画処理
//=====================================
void CStreetLamp::Draw(void)
{
	// プレイヤーの情報を取得する
	CPlayer* pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		if (pPlayer->GetStreetLamp() == this)
		{ // ぶら下がっている街灯と一致した場合

			// デバイスの取得
			LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

			// 描画処理
			CModel::Draw(0.8f);

			// Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Zテストの設定
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zテストの有効/無効設定
		}
		else
		{ // 上記以外

			// 描画処理
			CObstacle::Draw();
		}
	}
	else
	{ // 上記以外

		// 描画処理
		CObstacle::Draw();
	}
}

//=====================================
// プレイヤーのキャッチ処理
//=====================================
void CStreetLamp::PlayerCatch(D3DXVECTOR3* pos)
{
	// ローカル変数宣言
	CPlayer* pPlayer = nullptr;		// プレイヤーのポインタ
	float fRotDest = 0.0f;			// 目標の向き

	// プレイヤーの情報を取得する
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// 位置を設定する
		pos->x = GetPos().x;
		pos->y = GetPos().y - PLAYER_HEIGHT;
		pos->z = 0.0f;

		if (pPlayer->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
		{ // 行動状況がぶら下がり状態じゃない場合

			// 行動状況を設定する
			pPlayer->GetAction()->ActionChange(CPlayerAct::ACT_HANGING);

			// ぶら下がりモーションを設定する
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_HANGING);

			// 目標の向きを設定する
			fRotDest = pPlayer->GetMove().x * 0.15f;

			if (fRotDest >= D3DX_PI * 0.5f)
			{ // 目標の向きが半周を超えていた場合

				// 目標の向きを補正する
				fRotDest = D3DX_PI * 0.5f;
			}

			if (fRotDest <= -D3DX_PI * 0.5f)
			{ // 目標の向きが半周を超えていた場合

				// 目標の向きを補正する
				fRotDest = -D3DX_PI * 0.5f;
			}

			if (fRotDest == 0.0f)
			{ // 目標の向きが 0.0f の場合

				// 目標の向きに加算する
				fRotDest += 0.01f;
			}

			// 向きの正規化
			RotNormalize(&fRotDest);

			// 向きの移動量を設定する
			pPlayer->GetAction()->SetRotDest(fRotDest);

			if (fRotDest >= 0.0f)
			{ // 目標の向きがの場合

				// 右向き状況の設定処理
				pPlayer->SetRight(true);
			}
			else
			{ // 上記以外

				// 右向き状況の設定処理
				pPlayer->SetRight(false);
			}
		}

		// オブジェクトの位置の設定処理
		pPlayer->GetAction()->SetPosObject(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z));

		// 街灯の情報設定処理
		pPlayer->SetStreetLamp(this);

		// 軌跡の全頂点の位置の設定
		pPlayer->GetOrbit()->PosSet(pPlayer->GetPos());

		// 埃の設定処理
		CParticle::Create(GetPos(), CEffect::TYPE_CATCH);

		// 街灯掴む音を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCATCH);
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CStreetLamp::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 初期化処理
	SetFileData(CXFile::TYPE_STREETLAMP);		// データの設定処理

	CXFile::SXFile file = GetFileData();

	// 最大値を反映する
	file.vtxMax.x = CXFile::GetXFile(CXFile::TYPE_STREETPOLE).vtxMax.x;

	// 最小値を反映する
	file.vtxMin.x = CXFile::GetXFile(CXFile::TYPE_STREETPOLE).vtxMin.x;

	// データの設定処理
	SetFileData(file);

	// 情報の設定処理
	CObstacle::SetData(pos, type);
}