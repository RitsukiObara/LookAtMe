//===========================================
//
// 当たり判定のメイン処理[collision.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "game.h"
#include "collision.h"
#include "shadowCircle.h"
#include "objectElevation.h"
#include "elevation_manager.h"
#include "player.h"
#include "player_act.h"
#include "player_ability.h"
#include "combo.h"
#include "combo_magni.h"
#include "screwUI.h"
#include "airplane.h"
#include "airplane_manager.h"
#include "screw.h"
#include "screw_manager.h"
#include "enemy.h"
#include "enemy_manager.h"
#include "table.h"
#include "table_manager.h"
#include "macchina.h"
#include "macchina_manager.h"
#include "needle.h"
#include "needle_manager.h"
#include "block.h"
#include "block_manager.h"
#include "goal.h"
#include "useful.h"

//===============================
// マクロ定義
//===============================
#define COLLISION_ADD_DIFF_LENGTH		(0.01f)			// 僅かな誤差を埋めるためのマクロ定義(突っかかり防止)
#define AIRPLANE_COLL_RADIUS			(55.0f)			// 飛行機の当たり判定時の半径
#define PLAYER_SIZE						(D3DXVECTOR3(20.0f, 150.0f, 20.0f))		// プレイヤーのサイズ
#define PLAYER_HALF_HEIGHT				(PLAYER_SIZE.y / 2)						// プレイヤーの高さの半分
#define SCREW_COLL_RADIUS				(20.0f)			// ネジの当たり判定時の半径
#define QUAKE_HIT_RANGE					(D3DXVECTOR3(220.0f, 120.0f, 100.0f))		// 地震の当たり判定の範囲

//===============================
// 丸影の当たり判定処理
//===============================
void collision::ShadowCollision(const D3DXVECTOR3& pos, int nIdx)
{
	// ローカル変数宣言
	CShadowCircle* pShadow = CShadowCircle::GetShadow(nIdx);	// 影のポインタ
	D3DXVECTOR3 posPlayer = pos;				// プレイヤーの位置
	D3DXVECTOR3 posCand = NONE_D3DXVECTOR3;		// 影の位置の候補

	if (pShadow != nullptr)
	{ // 影のポインタが NULL じゃなかった場合

		// 影の位置を取得する
		posCand = D3DXVECTOR3(posPlayer.x, pShadow->GetPos().y, posPlayer.z);

		// 丸影の起伏地面の当たり判定
		ShadowElevCollision(posCand);

		// 丸影とブロックの当たり判定
		ShadowBlockCollision(posCand, posPlayer);

		// 丸影と台の当たり判定
		ShadowTableCollision(posCand, posPlayer);

		// 位置を設定する
		pShadow->SetPos(posCand);
	}
}

//===============================
// 丸影と起伏地面の当たり判定
//===============================
void collision::ShadowElevCollision(D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	CElevation* pMesh = CElevationManager::Get()->GetTop();		// メッシュのポインタを取得する

	while (pMesh != nullptr)
	{ // オブジェクトのが NULL じゃない限り回す

		// 影の位置を設定する
		pos.y = pMesh->ElevationCollision(pos);

		// 次のオブジェクトを代入する
		pMesh = pMesh->GetNext();
	}
}

//===============================
// 丸影とブロックの当たり判定
//===============================
void collision::ShadowBlockCollision(D3DXVECTOR3& pos, const D3DXVECTOR3& posPlayer)
{
	// ローカル変数宣言
	CBlock* pBlock = CBlockManager::Get()->GetTop();		// ブロックのポインタを取得する

	while (pBlock != nullptr)
	{ // オブジェクトのが NULL じゃない限り回す

		if (pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x >= pos.x &&
			pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x <= pos.x &&
			pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y <= posPlayer.y &&
			pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y >= pos.y)
		{ // プレイヤーと影の間にブロックがあった場合

			// 影の候補の位置を設定する
			pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y;
		}

		// 次のオブジェクトを代入する
		pBlock = pBlock->GetNext();
	}
}

//===============================
// 丸影と台の当たり判定
//===============================
void collision::ShadowTableCollision(D3DXVECTOR3& pos, const D3DXVECTOR3& posPlayer)
{
	// ローカル変数宣言
	CTable* pTable = CTableManager::Get()->GetTop();		// 台のポインタを取得する

	while (pTable != nullptr)
	{ // オブジェクトのが NULL じゃない限り回す

		if (pTable->GetPos().x + pTable->GetFileData().vtxMax.x >= pos.x &&
			pTable->GetPos().x + pTable->GetFileData().vtxMin.x <= pos.x &&
			pTable->GetPos().y + pTable->GetFileData().vtxMax.y <= posPlayer.y &&
			pTable->GetPos().y + pTable->GetFileData().vtxMax.y >= pos.y)
		{ // プレイヤーと影の間に台があった場合

			// 影の候補の位置を設定する
			pos.y = pTable->GetPos().y + pTable->GetFileData().vtxMax.y;
		}

		// 次のオブジェクトを代入する
		pTable = pTable->GetNext();
	}
}

//===============================
// プレイヤーと飛行機との当たり判定
//===============================
void collision::AirplaneHit(CPlayer& player)
{
	// ローカル変数宣言
	CAirplane* pAirplane = nullptr;	// 影のポインタ
	D3DXVECTOR3 PosPlayer = D3DXVECTOR3(player.GetPos().x, player.GetPos().y + PLAYER_HALF_HEIGHT, player.GetPos().z);		// プレイヤーの位置
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(PLAYER_SIZE.x, PLAYER_HALF_HEIGHT, PLAYER_SIZE.z);										// 最大値
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(-PLAYER_SIZE.x, -PLAYER_HALF_HEIGHT, -PLAYER_SIZE.z);									// 最小値
	D3DXVECTOR3 AirplaneVtxMax = D3DXVECTOR3(AIRPLANE_COLL_RADIUS, AIRPLANE_COLL_RADIUS, AIRPLANE_COLL_RADIUS);			// 最大値
	D3DXVECTOR3 AirplaneVtxMin = D3DXVECTOR3(-AIRPLANE_COLL_RADIUS, -AIRPLANE_COLL_RADIUS, -AIRPLANE_COLL_RADIUS);		// 最小値

	// オブジェクトのポインタを取得する
	pAirplane = CAirplaneManager::Get()->GetTop();

	while (pAirplane != nullptr)
	{ // オブジェクトのが NULL じゃない限り回す

		if (useful::RectangleCollisionXY(pAirplane->GetPos(), PosPlayer, AirplaneVtxMax, VtxMax, AirplaneVtxMin, VtxMin) == true &&
			useful::RectangleCollisionXZ(pAirplane->GetPos(), PosPlayer, AirplaneVtxMax, VtxMax, AirplaneVtxMin, VtxMin) == true &&
			pAirplane->GetCollision() == true)
		{ // 矩形の当たり判定が true かつ、当たり判定がある場合

			if (player.GetAction()->GetState() != CPlayerAct::STATE_CANNON &&
				player.GetAction()->GetState() != CPlayerAct::STATE_FLY)
			{ // 状態が大砲待機以外の場合

				// プレイヤーの位置を設定する
				player.SetPos(pAirplane->GetPos());

				// 状態の設定処理
				player.GetAction()->SetState(CPlayerAct::STATE_CANNON);
			}

			// この先の処理を抜け出す
			return;
		}

		// 次のオブジェクトを代入する
		pAirplane = pAirplane->GetNext();
	}
}

//===============================
// プレイヤーとネジとの当たり判定
//===============================
void collision::ScrewHit(CPlayer& player)
{
	// ローカル変数宣言
	CScrew* pScrew = CScrewManager::Get()->GetTop();	// ネジのポインタ
	CScrew* pScrewNext = nullptr;						// 次のネジのポインタ
	D3DXVECTOR3 PosPlayer = D3DXVECTOR3(player.GetPos().x, player.GetPos().y + PLAYER_HALF_HEIGHT, player.GetPos().z);		// プレイヤーの位置
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(PLAYER_SIZE.x, PLAYER_HALF_HEIGHT, PLAYER_SIZE.z);										// 最大値
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(-PLAYER_SIZE.x, -PLAYER_HALF_HEIGHT, -PLAYER_SIZE.z);									// 最小値
	D3DXVECTOR3 AirplaneVtxMax = D3DXVECTOR3(AIRPLANE_COLL_RADIUS, AIRPLANE_COLL_RADIUS, AIRPLANE_COLL_RADIUS);			// 最大値
	D3DXVECTOR3 AirplaneVtxMin = D3DXVECTOR3(-AIRPLANE_COLL_RADIUS, -AIRPLANE_COLL_RADIUS, -AIRPLANE_COLL_RADIUS);		// 最小値

	while (pScrew != nullptr)
	{ // オブジェクトのが NULL じゃない限り回す

		// 次のネジの情報を取得する
		pScrewNext = pScrew->GetNext();

		if (useful::RectangleCollisionXY(pScrew->GetPos(), PosPlayer, AirplaneVtxMax, VtxMax, AirplaneVtxMin, VtxMin) == true &&
			useful::RectangleCollisionXZ(pScrew->GetPos(), PosPlayer, AirplaneVtxMax, VtxMax, AirplaneVtxMin, VtxMin) == true &&
			pScrew->IsHit() == true)
		{ // 矩形の当たり判定が true の場合

			// ヒット処理
			pScrew->Hit(pScrew->GetPos());

			// スコアの加算処理
			player.GetScrewUI()->AddScore(1);

			// コンボの加算処理
			player.GetCombo()->AddCombo(10);
		}

		// 次のオブジェクトを代入する
		pScrew = pScrewNext;
	}
}

//===============================
// プレイヤーと敵との当たり判定
//===============================
void collision::EnemyHit(CPlayer& player)
{
	// ローカル変数宣言
	CEnemy* pEnemy = CEnemyManager::Get()->GetTop();		// 敵の情報
	CEnemy* pEnemyNext = nullptr;							// 次の敵の情報
	D3DXVECTOR3 pos = player.GetPos();						// プレイヤーの位置
	D3DXVECTOR3 posOld = player.GetPosOld();				// プレイヤーの前回の位置
	D3DXVECTOR3 EnemyVtxMin = NONE_D3DXVECTOR3;				// 敵の最小値

	while (pEnemy != nullptr)
	{ // 敵の情報が NULL じゃない場合

		// 敵の情報を取得する
		pEnemyNext = pEnemy->GetNext();

		// 敵の最小値を設定する
		EnemyVtxMin = D3DXVECTOR3(-pEnemy->GetCollSize().x, 0.0f, -pEnemy->GetCollSize().z);

		if (pEnemy->GetPos().z + pEnemy->GetFileData().collsize.z >= pos.z &&
			pEnemy->GetPos().z - pEnemy->GetFileData().collsize.z <= pos.z &&
			pEnemy->IsCollision() == true)
		{ // 敵とZ軸が合っているかつ、当たり判定状況が true の場合

			if (posOld.y >= pEnemy->GetPosOld().y + pEnemy->GetCollSize().y &&
				pos.y <= pEnemy->GetPos().y + pEnemy->GetCollSize().y &&
				pos.x + PLAYER_SIZE.x >= pEnemy->GetPos().x + EnemyVtxMin.x &&
				pos.x - PLAYER_SIZE.x <= pEnemy->GetPos().x + pEnemy->GetCollSize().x)
			{ // 上からの当たり判定

				if (pEnemy->IsStep() == true)
				{ // 踏みつけられるの場合

					// 敵のヒット処理
					pEnemy->Hit();

					// 踏みつけ時の処理
					player.StepHit();
				}
				else if (player.GetAction()->GetState() == CPlayerAct::STATE_FLY ||
					player.GetAction()->GetState() == CPlayerAct::STATE_CANNON)
				{ // 大砲状態または、飛行状態の場合

					// 敵のヒット処理
					pEnemy->SmashHit();

					// コンボの加算処理
					player.GetCombo()->AddCombo(100);
				}
				else
				{ // 上記以外

					if (pEnemy->IsStun() == true)
					{ // 気絶状況が true の場合

						// バウンド処理
						player.BoundHit();
					}
					else
					{ // 上記以外

						// プレイヤーのヒット処理
						player.Hit();
					}
				}
			}
			else if (posOld.y + PLAYER_SIZE.y <= pEnemy->GetPosOld().y + EnemyVtxMin.y &&
				pos.y + PLAYER_SIZE.y >= pEnemy->GetPos().y + EnemyVtxMin.y &&
				pos.x + PLAYER_SIZE.x >= pEnemy->GetPos().x + EnemyVtxMin.x &&
				pos.x - PLAYER_SIZE.x <= pEnemy->GetPos().x + pEnemy->GetCollSize().x)
			{ // 下からの当たり判定

				if (player.GetAction()->GetState() == CPlayerAct::STATE_FLY ||
					player.GetAction()->GetState() == CPlayerAct::STATE_CANNON ||
					player.GetAbility()->GetAbility() == CAbility::ABILITY_JETDASH || 
					player.GetAbility()->GetAbility() == CAbility::ABILITY_DASHJUMP)
				{ // ジェットダッシュまたは、ダッシュジャンプ状態の場合

					// 敵のヒット処理
					pEnemy->SmashHit();

					// コンボの加算処理
					player.GetCombo()->AddCombo(100);
				}
				else
				{ // 上記以外

					if (pEnemy->IsStun() == true)
					{ // 気絶状況が true の場合

						// バウンド処理
						player.BoundHit();
					}
					else
					{ // 上記以外

						// プレイヤーのヒット処理
						player.Hit();
					}
				}
			}
			else if (posOld.x + PLAYER_SIZE.x <= pEnemy->GetPosOld().x + EnemyVtxMin.x &&
				pos.x + PLAYER_SIZE.x >= pEnemy->GetPos().x + EnemyVtxMin.x &&
				pos.y + PLAYER_SIZE.y >= pEnemy->GetPos().y + EnemyVtxMin.y &&
				pos.y <= pEnemy->GetPos().y + pEnemy->GetCollSize().y)
			{ // 左からの当たり判定

				if (player.GetAction()->GetState() == CPlayerAct::STATE_FLY ||
					player.GetAction()->GetState() == CPlayerAct::STATE_CANNON ||
					player.GetAbility()->GetAbility() == CAbility::ABILITY_JETDASH ||
					player.GetAbility()->GetAbility() == CAbility::ABILITY_DASHJUMP)
				{ // ジェットダッシュ状態の場合

					// 敵のヒット処理
					pEnemy->SmashHit();

					// コンボの加算処理
					player.GetCombo()->AddCombo(100);
				}
				else
				{ // 上記以外

					if (pEnemy->IsStun() == true)
					{ // 気絶状況が true の場合

						// バウンド処理
						player.BoundHit();
					}
					else
					{ // 上記以外

						// プレイヤーのヒット処理
						player.Hit();
					}
				}
			}
			else if (posOld.x - PLAYER_SIZE.x >= pEnemy->GetPosOld().x + pEnemy->GetCollSize().x &&
				pos.x - PLAYER_SIZE.x <= pEnemy->GetPos().x + pEnemy->GetCollSize().x &&
				pos.y + PLAYER_SIZE.y >= pEnemy->GetPos().y + EnemyVtxMin.y &&
				pos.y <= pEnemy->GetPos().y + pEnemy->GetCollSize().y)
			{ // 右からの当たり判定

				if (player.GetAction()->GetState() == CPlayerAct::STATE_FLY ||
					player.GetAction()->GetState() == CPlayerAct::STATE_CANNON ||
					player.GetAbility()->GetAbility() == CAbility::ABILITY_JETDASH ||
					player.GetAbility()->GetAbility() == CAbility::ABILITY_DASHJUMP)
				{ // ジェットダッシュ状態の場合

					// 敵の吹き飛びヒット処理
					pEnemy->SmashHit();

					// コンボの加算処理
					player.GetCombo()->AddCombo(100);
				}
				else
				{ // 上記以外

					if (pEnemy->IsStun() == true)
					{ // 気絶状況が true の場合

						// バウンド処理
						player.BoundHit();
					}
					else
					{ // 上記以外

						// プレイヤーのヒット処理
						player.Hit();
					}
				}
			}
		}

		// 次のオブジェクトを代入する
		pEnemy = pEnemyNext;
	}
}

//===============================
// プレイヤーと敵とのめりこみ判定
//===============================
void collision::EnemyPenetrate(CPlayer& player)
{
	// ローカル変数宣言
	CEnemy* pEnemy = CEnemyManager::Get()->GetTop();		// 敵の情報
	CEnemy* pEnemyNext = nullptr;							// 次の敵の情報
	D3DXVECTOR3 PosPlayer = D3DXVECTOR3(player.GetPos().x, player.GetPos().y + PLAYER_HALF_HEIGHT, player.GetPos().z);		// プレイヤーの位置
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(PLAYER_SIZE.x, PLAYER_HALF_HEIGHT, PLAYER_SIZE.z);										// 最大値
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(-PLAYER_SIZE.x, -PLAYER_HALF_HEIGHT, -PLAYER_SIZE.z);									// 最小値
	D3DXVECTOR3 EnemyVtxMin = NONE_D3DXVECTOR3;				// 敵の最小値

	while (pEnemy != nullptr)
	{ // 敵の情報が NULL じゃない場合

		// 敵の情報を取得する
		pEnemyNext = pEnemy->GetNext();

		// 敵の最小値を設定する
		EnemyVtxMin = D3DXVECTOR3(-pEnemy->GetCollSize().x, 0.0f, -pEnemy->GetCollSize().z);

		if (useful::RectangleCollisionXY(pEnemy->GetPos(), PosPlayer, pEnemy->GetCollSize(), VtxMax, EnemyVtxMin, VtxMin) == true &&
			useful::RectangleCollisionXZ(pEnemy->GetPos(), PosPlayer, pEnemy->GetCollSize(), VtxMax, EnemyVtxMin, VtxMin) == true &&
			pEnemy->IsCollision() == true)
		{ // 敵とプレイヤーが重なっている時

			if (player.GetAction()->GetState() == CPlayerAct::STATE_FLY ||
				player.GetAction()->GetState() == CPlayerAct::STATE_CANNON ||
				player.GetAbility()->GetAbility() == CAbility::ABILITY_JETDASH ||
				player.GetAbility()->GetAbility() == CAbility::ABILITY_DASHJUMP)
			{ // 飛んでいる場合

				// 吹き飛ばし処理
				pEnemy->SmashHit();

				// コンボの加算処理
				player.GetCombo()->AddCombo(100);
			}
			else
			{ // 上記以外

				if (pEnemy->IsStun() == true)
				{ // 気絶状況が true の場合

					// バウンド処理
					player.BoundHit();
				}
				else
				{ // 上記以外

					// プレイヤーのヒット処理
					player.Hit();
				}
			}
		}

		// 次のオブジェクトを代入する
		pEnemy = pEnemyNext;
	}
}

////===============================
//// 敵同士の当たり判定処理
////===============================
//void collision::EnemyToEnemy(CEnemy* pTarget)
//{
//	// ローカル変数宣言
//	CEnemy* pEnemy = CEnemyManager::Get()->GetTop();		// 敵の情報
//	D3DXVECTOR3 pos = pTarget->GetPos();					// 位置
//	D3DXVECTOR3 posOld = pTarget->GetPosOld();				// 前回の位置
//	D3DXVECTOR3 EnemyVtxMin = NONE_D3DXVECTOR3;				// 敵の最小値
//	float fRot = 0.0f;										// 向き
//
//	while (pEnemy != nullptr)
//	{ // 敵の情報が NULL じゃない場合
//
//		if (pEnemy != pTarget)
//		{ // 敵が自分じゃない場合
//
//			// 敵の最小値を設定する
//			EnemyVtxMin = D3DXVECTOR3(-pEnemy->GetCollSize().x, 0.0f, -pEnemy->GetCollSize().z);
//
//			if (pEnemy->GetPos().z + pEnemy->GetFileData().collsize.z >= pos.z &&
//				pEnemy->GetPos().z - pEnemy->GetFileData().collsize.z <= pos.z &&
//				pEnemy->IsCollision() == true)
//			{ // 敵とZ軸が合っているかつ、当たり判定状況が true の場合
//
//				// 円の当たり判定(XY平面)
//				if (useful::CircleCollisionXY(pos, pEnemy->GetPos(), pTarget->GetCollSize().x, pEnemy->GetCollSize().x) == true)
//				{ // 当たり判定が true の場合
//
//					// 向きを設定する
//					fRot = atan2f(pEnemy->GetPos().x - pos.x, pEnemy->GetPos().y - pos.y);
//
//					// 位置を設定する
//					pos.x = pEnemy->GetPos().x + sinf(fRot) * pEnemy->GetCollSize().x;
//					pos.y = pEnemy->GetPos().y + cosf(fRot) * pEnemy->GetCollSize().y;
//				}
//			}
//		}
//
//		// 次のオブジェクトを代入する
//		pEnemy = pEnemy->GetNext();
//	}
//
//	// 位置を適用する
//	pTarget->SetPos(pos);
//}

//===============================
// 敵の気絶処理
//===============================
void collision::EnemyStun(CPlayer& player)
{
	// ローカル変数宣言
	CEnemy* pEnemy = CEnemyManager::Get()->GetTop();		// マキナ草の情報

	while (pEnemy != nullptr)
	{ // 敵の情報が NULL じゃない場合

		if (useful::RectangleCollisionXZ(player.GetPos(), pEnemy->GetPos(), QUAKE_HIT_RANGE, NONE_D3DXVECTOR3, -QUAKE_HIT_RANGE, NONE_D3DXVECTOR3) &&
			fabsf(player.GetPos().y - pEnemy->GetPos().y) <= QUAKE_HIT_RANGE.y &&
			pEnemy->IsCollision() == true)
		{ // 当たり判定に当たった場合

			// ヒット処理
			pEnemy->StunHit();
		}

		// 次のオブジェクトを代入する
		pEnemy = pEnemy->GetNext();
	}
}

//===============================
// 台との当たり判定
//===============================
bool collision::TableCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth, const float fDepth)
{
	// ローカル変数宣言
	CTable* pTable = CTableManager::Get()->GetTop();		// 敵の情報
	bool bLand = false;										// 着地判定

	while (pTable != nullptr)
	{ // 敵の情報が NULL じゃない場合

		if (posOld.y >= pTable->GetPosOld().y + pTable->GetFileData().vtxMax.y &&
			pPos->y <= pTable->GetPosOld().y + pTable->GetFileData().vtxMax.y &&
			pPos->x + fWidth >= pTable->GetPos().x + pTable->GetFileData().vtxMin.x &&
			pPos->x - fWidth <= pTable->GetPos().x + pTable->GetFileData().vtxMax.x &&
			pPos->z + fDepth >= pTable->GetPos().z + pTable->GetFileData().vtxMin.z &&
			pPos->z - fDepth <= pTable->GetPos().z + pTable->GetFileData().vtxMax.z)
		{ // 上からの当たり判定

			// 位置を補正
			pPos->y = pTable->GetPos().y + pTable->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

			// 着地判定を付与する
			bLand = true;
		}

		// 次のオブジェクトを代入する
		pTable = pTable->GetNext();
	}

	// 着地判定を返す
	return bLand;
}

//===============================
// プレイヤーとマキナ草との当たり判定
//===============================
void collision::MacchinaHit(CPlayer& player)
{
	// ローカル変数宣言
	CMacchina* pMacchina = CMacchinaManager::Get()->GetTop();		// マキナ草の情報

	while (pMacchina != nullptr)
	{ // 敵の情報が NULL じゃない場合

		if (useful::RectangleCollisionXZ(player.GetPos(), pMacchina->GetPos(), QUAKE_HIT_RANGE, NONE_D3DXVECTOR3, -QUAKE_HIT_RANGE, NONE_D3DXVECTOR3) &&
			fabsf(player.GetPos().y - pMacchina->GetPos().y) <= QUAKE_HIT_RANGE.y &&
			pMacchina->GetCollision() == true)
		{ // 当たり判定に当たった場合

			// ヒット処理
			pMacchina->Hit();
		}

		// 次のオブジェクトを代入する
		pMacchina = pMacchina->GetNext();
	}
}

//===============================
// プレイヤーとゴールとの当たり判定
//===============================
void collision::GoalHit(CPlayer& player)
{
	// ローカルポインタ宣言
	CModel* pGoal = nullptr;		// ゴールのポインタ
	D3DXVECTOR3 PosPlayer = D3DXVECTOR3(player.GetPos().x, player.GetPos().y + PLAYER_HALF_HEIGHT, player.GetPos().z);		// プレイヤーの位置
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(PLAYER_SIZE.x, PLAYER_HALF_HEIGHT, PLAYER_SIZE.z);										// 最大値
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(-PLAYER_SIZE.x, -PLAYER_HALF_HEIGHT, -PLAYER_SIZE.z);									// 最小値

	if (CGoal::Get() != nullptr)
	{ // ゴールが NULL じゃない場合

		// ゴールの情報を取得する
		pGoal = CGoal::Get()->GetModel(CGoal::MODEL_BODY);

		if (useful::RectangleCollisionXY(pGoal->GetPos(), PosPlayer, pGoal->GetFileData().vtxMax, VtxMax, pGoal->GetFileData().vtxMin, VtxMin) == true &&
			useful::RectangleCollisionXZ(pGoal->GetPos(), PosPlayer, pGoal->GetFileData().vtxMax, VtxMax, pGoal->GetFileData().vtxMin, VtxMin) == true)
		{ // 矩形の当たり判定が true の場合

			// ヒット処理
			CGoal::Get()->Hit();
		}
	}
}

//===============================
// プレイヤーと棘の当たり判定
//===============================
void collision::NeedleHit(CPlayer& player)
{
	// ローカル変数宣言
	CNeedle* pNeedle = CNeedleManager::Get()->GetTop();		// 棘の情報
	D3DXVECTOR3 pos = player.GetPos();						// 位置
	D3DXVECTOR3 posOld = player.GetPosOld();				// 前回の位置
	D3DXVECTOR3 move = player.GetMove();					// 移動量

	while (pNeedle != nullptr)
	{ // 棘の情報が NULL じゃない場合

		if (pNeedle->GetPos().z + pNeedle->GetFileData().collsize.z >= pos.z &&
			pNeedle->GetPos().z - pNeedle->GetFileData().collsize.z <= pos.z)
		{ // 棘とZ軸が合っているかつ、当たり判定状況が true の場合

			if (posOld.y >= pNeedle->GetPosOld().y + pNeedle->GetFileData().vtxMax.y &&
				pos.y <= pNeedle->GetPos().y + pNeedle->GetFileData().vtxMax.y &&
				pos.x + PLAYER_SIZE.x >= pNeedle->GetPos().x + pNeedle->GetFileData().vtxMin.x &&
				pos.x - PLAYER_SIZE.x <= pNeedle->GetPos().x + pNeedle->GetFileData().vtxMax.x)
			{ // 上からの当たり判定

				// 位置を設定する
				pos.y = pNeedle->GetPos().y + pNeedle->GetFileData().vtxMax.y;

				// 移動量を 0.0f にする
				move.y = 0.0f;

				// ジャンプ状況を true にする
				player.SetEnableJump(false);

				// プレイヤーのヒット処理
				player.Hit();
			}
			else if (posOld.y + PLAYER_SIZE.y <= pNeedle->GetPosOld().y + pNeedle->GetFileData().vtxMin.y &&
				pos.y + PLAYER_SIZE.y >= pNeedle->GetPos().y + pNeedle->GetFileData().vtxMin.y &&
				pos.x + PLAYER_SIZE.x >= pNeedle->GetPos().x + pNeedle->GetFileData().vtxMin.x &&
				pos.x - PLAYER_SIZE.x <= pNeedle->GetPos().x + pNeedle->GetFileData().vtxMax.x)
			{ // 下からの当たり判定

				// 位置を設定する
				pos.y = pNeedle->GetPos().y + pNeedle->GetFileData().vtxMin.y - PLAYER_SIZE.y;

				// 移動量を 0.0f にする
				move.y = 0.0f;

				// プレイヤーのヒット処理
				player.Hit();
			}
			else if (posOld.x + PLAYER_SIZE.x <= pNeedle->GetPosOld().x + pNeedle->GetFileData().vtxMin.x &&
				pos.x + PLAYER_SIZE.x >= pNeedle->GetPos().x + pNeedle->GetFileData().vtxMin.x &&
				pos.y + PLAYER_SIZE.y >= pNeedle->GetPos().y + pNeedle->GetFileData().vtxMin.y &&
				pos.y <= pNeedle->GetPos().y + pNeedle->GetFileData().vtxMax.y)
			{ // 左からの当たり判定

				// 位置を設定する
				pos.x = pNeedle->GetPos().x + pNeedle->GetFileData().vtxMin.x - PLAYER_SIZE.x;

				// 移動量を 0.0f にする
				move.x = 0.0f;

				// プレイヤーのヒット処理
				player.Hit();
			}
			else if (posOld.x - PLAYER_SIZE.x >= pNeedle->GetPosOld().x + pNeedle->GetFileData().vtxMax.x &&
				pos.x - PLAYER_SIZE.x <= pNeedle->GetPos().x + pNeedle->GetFileData().vtxMax.x &&
				pos.y + PLAYER_SIZE.y >= pNeedle->GetPos().y + pNeedle->GetFileData().vtxMin.y &&
				pos.y <= pNeedle->GetPos().y + pNeedle->GetFileData().vtxMax.y)
			{ // 右からの当たり判定

				// 位置を設定する
				pos.x = pNeedle->GetPos().x + pNeedle->GetFileData().vtxMax.x + PLAYER_SIZE.x;

				// 移動量を 0.0f にする
				move.x = 0.0f;

				// プレイヤーのヒット処理
				player.Hit();
			}
		}

		// 次のオブジェクトを代入する
		pNeedle = pNeedle->GetNext();
	}

	// 情報を適用する
	player.SetPos(pos);		// 位置を適用する
	player.SetMove(move);	// 移動量を適用する
}

//===============================
// ブロックとの当たり判定
//===============================
bool collision::BlockCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth, const float fHeight, const bool bJump, CBlock** ppRide)
{
	// ローカル変数宣言
	CBlock* pBlock = CBlockManager::Get()->GetTop();		// ブロックの情報
	bool bLand = false;				// 着地状況

	while (pBlock != nullptr)
	{ // ブロックの情報が NULL じゃない場合

		if (pBlock->GetPos().z + pBlock->GetFileData().vtxMax.z >= pPos->z &&
			pBlock->GetPos().z + pBlock->GetFileData().vtxMin.z <= pPos->z)
		{ // 敵とZ軸が合っているかつ、当たり判定状況が true の場合

			if (bJump == false)
			{ // 地上に立っている場合

				if (posOld.y >= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMax.y &&
					pPos->y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
					pPos->x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pPos->x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // 上からの当たり判定

					// 位置を設定する
					pPos->y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y;

					// 着地判定を通す
					bLand = true;

					if (ppRide != nullptr)
					{ // 乗っているブロックのポインタが NULL じゃない場合

						// ブロックの情報を入れる
						*ppRide = pBlock;
					}
				}
				else if (posOld.y + fHeight <= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMin.y &&
					pPos->y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pPos->x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pPos->x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // 下からの当たり判定

					// 位置を設定する
					pPos->y = pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y - fHeight;
				}
				else if (posOld.x + fWidth <= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMin.x &&
					pPos->x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pPos->y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pPos->y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 左からの当たり判定

					// 位置を設定する
					pPos->x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - fWidth;
				}
				else if (posOld.x - fWidth >= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMax.x &&
					pPos->x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pPos->y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pPos->y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 右からの当たり判定

					// 位置を設定する
					pPos->x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + fWidth;
				}
			}
			else
			{ // 上記以外

				if (posOld.x + fWidth <= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMin.x &&
					pPos->x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pPos->y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pPos->y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 左からの当たり判定

					// 位置を設定する
					pPos->x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - fWidth;
				}
				else if (posOld.x - fWidth >= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMax.x &&
					pPos->x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pPos->y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pPos->y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 右からの当たり判定

					// 位置を設定する
					pPos->x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + fWidth;
				}
				else if (posOld.y >= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMax.y &&
					pPos->y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
					pPos->x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pPos->x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // 上からの当たり判定

					// 位置を設定する
					pPos->y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y;

					// 着地判定を通す
					bLand = true;

					if (ppRide != nullptr)
					{ // 乗っているブロックのポインタが NULL じゃない場合

						// ブロックの情報を入れる
						*ppRide = pBlock;
					}
				}
				else if (posOld.y + fHeight <= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMin.y &&
					pPos->y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pPos->x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pPos->x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // 下からの当たり判定

					// 位置を設定する
					pPos->y = pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y - fHeight;
				}
			}
		}

		// 次のオブジェクトを代入する
		pBlock = pBlock->GetNext();
	}

	// 着地判定を返す
	return bLand;
}

//===============================
// 起伏地面の範囲外の当たり判定
//===============================
bool collision::ElevOutRangeCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth)
{
	// ローカル変数宣言
	CElevation* pElev = CElevationManager::Get()->GetTop();		// 先頭の起伏地面を取得する
	bool bCollision = false;									// 当たり判定状況

	while (pElev != nullptr)
	{ // ブロックの情報が NULL じゃない場合

		if (pElev->GetPos().z + (pElev->GetSize().z * 0.5f) >= pPos->z &&
			pElev->GetPos().z - (pElev->GetSize().z * 0.5f) <= pPos->z &&
			pElev->GetPos().y > pPos->y)
		{ // 起伏地面より下にいる場合

			if (posOld.x + fWidth <= pElev->GetPos().x - (pElev->GetSize().x * 0.5f) &&
				pPos->x + fWidth >= pElev->GetPos().x - (pElev->GetSize().x * 0.5f))
			{ // 左の当たり判定

				// 位置を設定する
				pPos->x = pElev->GetPos().x - (pElev->GetSize().x * 0.5f) - fWidth;

				// 当たり判定状況を true にする
				bCollision = true;
			}
			else if (posOld.x - fWidth >= pElev->GetPos().x + (pElev->GetSize().x * 0.5f) &&
				pPos->x - fWidth <= pElev->GetPos().x + (pElev->GetSize().x * 0.5f))
			{ // 右の当たり判定

				// 位置を設定する
				pPos->x = pElev->GetPos().x + (pElev->GetSize().x * 0.5f) + fWidth;

				// 当たり判定状況を true にする
				bCollision = true;
			}
		}

		// 次のオブジェクトを代入する
		pElev = pElev->GetNext();
	}

	// 当たり判定状況を返す
	return bCollision;
}