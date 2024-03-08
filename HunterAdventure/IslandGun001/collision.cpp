//===========================================
//
// 当たり判定のメイン処理[collision.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "collision.h"
#include "useful.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "shadowCircle.h"
#include "objectElevation.h"
#include "coin.h"
#include "game_score.h"
#include "addscoreUI.h"
#include "enemy.h"
#include "tree.h"
#include "gold_bone.h"
#include "player.h"
#include "player_action.h"
#include "gold_bone_UI.h"
#include "palm_fruit.h"
#include "bullet.h"
#include "rock.h"
#include "bang_flower.h"
#include "bomb.h"
#include "bomb_explosion.h"
#include "wall.h"
#include "block.h"
#include "boss.h"
#include "boss_collision.h"
#include "alter.h"
#include "alter_pole.h"
#include "alter_message.h"
#include "slash_ripple.h"
#include "wind_shot.h"
#include "fire_shot.h"
#include "signboard.h"
#include "game.h"
#include "tutorial.h"
#include "balloon.h"
#include "balloon_spawner.h"
#include "door.h"
#include "push_timing.h"

//===============================
// マクロ定義
//===============================
namespace
{
	const float COLLISION_ADD_DIFF_LENGTH = 0.01f;	// 僅かな誤差を埋めるためのマクロ定義(突っかかり防止)
	const float TREE_RADIUS[CTree::TYPE_MAX] =		// 木の当たり判定時の半径
	{
		50.5f,		// 木の半径
	};
	
	const float COIN_COLLISION_MAGNI = 3.0f;		// コインの当たり判定の倍率

	const int DAGGER_DAMAGE = 30;					// ダガーのダメージ
	const float DAGGER_KNOCKBACK = 100.0f;			// ダガーのノックバック

	const int EXPLOSION_DAMAGE = 30;				// 爆風のダメージ
	const float EXPLOSION_KNOCKBACK = 150.0f;		// 爆風のノックバック

	const int PALM_FRUIT_HEALING = 30;				// ヤシの実の回復値

	const int COIN_SCORE = 100;						// コインのスコア

	const float SIGNBOARD_ADD_RADIUS = 50.0f;		// 看板の追加の半径

	const float ALTER_SURROUND_RADIUS = 800.0f;		// 祭壇の周囲の半径
	
	const float DOOR_ADD_RADIUS = 100.0f;			// ドアの追加の半径
	const float DOOR_HIT_ANGLE = 0.6f;				// ドアのヒット判定が出る方向

	const float BOMB_BULLET_SMASH = 10.0f;			// 銃弾で爆弾の吹き飛ぶ速度
	const float BOMB_DAGGER_SMASH = 23.0f;			// ダガーで爆弾の吹き飛ぶ速度
	const float BOMB_SLASH_RIPPLE_SMASH = 10.0f;	// 斬撃波紋で爆弾の吹き飛ぶ速度
	const int BOSS_DAMAGE = 5;						// ボスへのダメージ

	const float STAGE_RANGE = 8000.0f;				// ステージの範囲
}

//===============================
// 起伏地面の範囲外の当たり判定
//===============================
bool collision::ElevOutRangeCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth)
{
	// ローカル変数宣言
	bool bCollision = false;									// 当たり判定状況
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// 先頭の小判
	CElevation* pElevEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pElev = list.GetTop();

		// 末尾の値を取得する
		pElevEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

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

	// 当たり判定状況を返す
	return bCollision;
}

//===============================
// 小判との当たり判定
//===============================
void collision::CoinCollision(CPlayer* pPlayer, const D3DXVECTOR3 size, const int nAreaIdx)
{
	// ローカル変数宣言
	D3DXVECTOR3 posCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMaxCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMinCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 vtxMax = size;		// 最大値
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(size);	// 最小値
	CListManager<CCoin*> list = CCoin::GetList(nAreaIdx);
	CCoin* pCoin = nullptr;			// 先頭の小判
	int nNumData = list.GetNumData();		// 要素の総数

	for (int nCntCoin = 0; nCntCoin < nNumData; nCntCoin++)
	{
		// 次のオブジェクトを代入する
		pCoin = list.GetData(nCntCoin);

		// コインの変数を取得する
		posCoin = pCoin->GetPos();
		vtxMaxCoin = pCoin->GetFileData().vtxMax;
		vtxMinCoin = pCoin->GetFileData().vtxMin;

		if (pCoin->GetState() == CCoin::STATE_NONE &&
			useful::RectangleCollisionXY(posPlayer, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true &&
			useful::RectangleCollisionXZ(posPlayer, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true &&
			useful::RectangleCollisionYZ(posPlayer, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true)
		{ // コインと体が重なった場合

			// 取得処理
			pCoin->Hit();

			if (CGame::GetGameScore() != nullptr)
			{ // ゲームスコアが NULL じゃない場合

				// コイン分のスコアを加算する
				CGame::GetGameScore()->SetScore(CGame::GetGameScore()->GetScore() + COIN_SCORE);

				// 追加スコアUIを生成
				CAddScoreUI::Create(posCoin, CAddScoreUI::TYPE_COIN);
			}
		}
	}
}

//===============================
// 敵と銃の当たり判定
//===============================
bool collision::EnemyHitToGun(const CBullet& bullet)
{
	// ローカル変数宣言
	D3DXVECTOR3 posBullet = bullet.GetPos();		// 弾の位置
	D3DXVECTOR3 posOldBullet = bullet.GetPosOld();	// 前回の弾の位置
	D3DXVECTOR3 bulletMax = D3DXVECTOR3(bullet.GetSize().x, bullet.GetSize().y, bullet.GetSize().x);	// 弾の最大値
	D3DXVECTOR3 bulletMin = D3DXVECTOR3(-bullet.GetSize().x, -bullet.GetSize().y, -bullet.GetSize().x);	// 弾の最小値
	int nDamage = bullet.GetDamage();				// ダメージ
	float fKnockback = bullet.GetKnockBack();		// ノックバック値
	D3DXVECTOR3 enemyMax;		// 敵の最大値
	D3DXVECTOR3 enemyMin;		// 敵の最小値
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;		// 先頭の敵
	CEnemy* pEnemyEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pEnemy = list.GetTop();

		// 末尾の値を取得する
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定
			enemyMax = pEnemy->GetCollSize();
			enemyMin = useful::VtxMinConv(enemyMax);

			if (HexahedronHit
			(
				posBullet,
				pEnemy->GetPos(),
				posOldBullet,
				pEnemy->GetPosOld(),
				bulletMin,
				enemyMin,
				bulletMax,
				enemyMax) == true)
			{ // 敵と重なった場合

				// ヒット処理
				pEnemy->Hit(nDamage, fKnockback);

				// true を返す
				return true;
			}

			if (pEnemy == pEnemyEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pEnemy = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 敵とダガーの当たり判定
//===============================
bool collision::EnemyHitToDagger(const D3DXVECTOR3& pos, const float fHeight, const float fRadius)
{
	// ローカル変数宣言
	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;		// 敵の位置
	D3DXVECTOR3 sizeEnemy = NONE_D3DXVECTOR3;		// 敵のサイズ
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;		// 先頭の敵
	CEnemy* pEnemyEnd = nullptr;	// 末尾の値
	int nIdx = 0;
	
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pEnemy = list.GetTop();

		// 末尾の値を取得する
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定する
			posEnemy = pEnemy->GetPos();				// 敵の位置
			sizeEnemy = pEnemy->GetCollSize();		// 敵の高さ

			if (useful::CircleCollisionXZ(pos, posEnemy, fRadius, (sizeEnemy.x + sizeEnemy.z) * 0.5f) == true &&
				pEnemy->GetState() == CEnemy::STATE_NONE &&
				pos.y <= posEnemy.y + sizeEnemy.y &&
				pos.y + fHeight >= posEnemy.y)
			{ // 敵と重なった場合

				switch (pEnemy->GetType())
				{
				case CEnemy::TYPE::TYPE_TORDLE:

					// ヒット処理
					pEnemy->Hit(DAGGER_DAMAGE, DAGGER_KNOCKBACK);

					break;

				case CEnemy::TYPE::TYPE_IWAKARI:

					// ヒット処理
					pEnemy->Hit(DAGGER_DAMAGE * DAGGER_DAMAGE, DAGGER_KNOCKBACK);

					break;

				default:

					// 停止
					assert(false);

					break;
				}

				// true を返す
				return true;
			}

			if (pEnemy == pEnemyEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pEnemy = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 敵とプレイヤーとの当たり判定
//===============================
bool collision::EnemyHitToPlayer(CPlayer* pPlayer, const float fRadius, const float fHeight)
{
	// プレイヤー関係
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();

	// 敵関係
	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;		// 敵の位置
	D3DXVECTOR3 sizeEnemy = NONE_D3DXVECTOR3;		// 敵のサイズ
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;		// 先頭の敵
	CEnemy* pEnemyEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pEnemy = list.GetTop();

		// 末尾の値を取得する
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定する
			posEnemy = pEnemy->GetPos();			// 敵の位置
			sizeEnemy = pEnemy->GetCollSize();		// 敵の高さ

			if (useful::CircleCollisionXZ(posPlayer, posEnemy, fRadius, (sizeEnemy.x + sizeEnemy.z) * 0.5f) == true &&
				posPlayer.y <= posEnemy.y + sizeEnemy.y &&
				posPlayer.y + fHeight >= posEnemy.y)
			{ // 敵と重なった場合

				// プレイヤーのヒット処理
				pPlayer->Hit(10, atan2f(posPlayer.x - posEnemy.x, posPlayer.z - posEnemy.z));

				// true を返す
				return true;
			}

			if (pEnemy == pEnemyEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pEnemy = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 敵同士の当たり判定
//===============================
void collision::EnemyHitToEnemy(CEnemy* pTarget)
{
	// ローカル変数宣言
	D3DXVECTOR3 posTarget = pTarget->GetPos();
	D3DXVECTOR3 posOldTarget = pTarget->GetPosOld();
	D3DXVECTOR3 vtxMaxTarget = pTarget->GetCollSize();
	D3DXVECTOR3 vtxMinTarget = useful::VtxMinConv(vtxMaxTarget);

	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;		// 敵の位置
	D3DXVECTOR3 posOldEnemy = NONE_D3DXVECTOR3;		// 敵の前回の位置
	D3DXVECTOR3 vtxMaxEnemy = NONE_D3DXVECTOR3;		// 敵の頂点の最大値
	D3DXVECTOR3 vtxMinEnemy = NONE_D3DXVECTOR3;		// 敵の頂点の最小値
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;		// 先頭の敵
	CEnemy* pEnemyEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pEnemy = list.GetTop();

		// 末尾の値を取得する
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pEnemy != pTarget)
			{ // 敵が違った場合

				// 敵関係の変数を設定する
				posEnemy = pEnemy->GetPos();					// 敵の位置
				posOldEnemy = pEnemy->GetPosOld();				// 敵の前回の位置
				vtxMaxEnemy = pEnemy->GetCollSize();			// 敵の頂点の最大値
				vtxMinEnemy = useful::VtxMinConv(vtxMaxEnemy);	// 敵の頂点の最小値

				// 六面体の当たり判定
				HexahedronCollision(&posTarget, posEnemy, posOldTarget, posOldEnemy, vtxMinTarget, vtxMinEnemy, vtxMaxTarget, vtxMaxEnemy);
			}

			if (pEnemy == pEnemyEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pEnemy = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// 位置を適用する
	pTarget->SetPos(posTarget);
}

//===============================
// 金の骨との当たり判定
//===============================
void collision::GoldBoneCollision(const CPlayer& pPlayer, const D3DXVECTOR3& size)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = pPlayer.GetPos();
	D3DXVECTOR3 posBone = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMaxBone = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMinBone = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMax = size;		// 最大値
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(size);	// 最小値
	CListManager<CGoldBone*> list = CGoldBone::GetList();
	CGoldBone* pBone = nullptr;		// 先頭の値
	CGoldBone* pBoneEnd = nullptr;	// 末尾の値
	int nIdx = 0;
	
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBone = list.GetTop();

		// 末尾の値を取得する
		pBoneEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pBone->GetState() == CGoldBone::STATE_NONE)
			{ // 通常状態の場合

				// 金の骨の変数を取得する
				posBone = pBone->GetPos();
				vtxMaxBone = pBone->GetFileData().vtxMax;
				vtxMinBone = pBone->GetFileData().vtxMin;

				if (useful::RectangleCollisionXY(pos, posBone, vtxMax, vtxMaxBone, vtxMin, vtxMinBone) == true &&
					useful::RectangleCollisionXZ(pos, posBone, vtxMax, vtxMaxBone, vtxMin, vtxMinBone) == true &&
					useful::RectangleCollisionYZ(pos, posBone, vtxMax, vtxMaxBone, vtxMin, vtxMinBone) == true)
				{ // 金の骨と重なった場合

					// 取得処理
					pBone->Hit();

					if (pPlayer.GetGoldBoneUI() != nullptr)
					{ // 金の骨UIが NULL じゃない場合

						// 金の骨取得処理
						pPlayer.GetGoldBoneUI()->GetGoldBone();
					}
				}
			}

			if (pBone == pBoneEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBone = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

//===============================
// 木の当たり判定
//===============================
bool collision::TreeCollision(D3DXVECTOR3* pos, const float fRadius, const int nAreaIdx)
{
	// ローカル変数宣言
	D3DXVECTOR3 posTree = NONE_D3DXVECTOR3;			// 木の位置
	float fObjectRadius;							// 半径
	bool bCollision = false;						// 当たり判定状況
	CListManager<CTree*> list = CTree::GetList(nAreaIdx);
	CTree* pTree = nullptr;		// 先頭の値
	CTree* pTreeEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pTree = list.GetTop();

		// 末尾の値を取得する
		pTreeEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			posTree = pTree->GetPos();		// 木の位置
			fObjectRadius = fRadius + TREE_RADIUS[pTree->GetType()];	// 半径

			if (useful::CylinderCollision(pos, posTree, fObjectRadius) == true)
			{ // 円柱の当たり判定に入った場合

				// 当たり判定状況を true にする
				bCollision = true;
			}

			if (pTree == pTreeEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pTree = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// 当たり判定状況 を返す
	return bCollision;
}

//===============================
// 木への攻撃判定処理
//===============================
void collision::TreeAttack(const CPlayer& pPlayer, const float fRadius, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posTree = NONE_D3DXVECTOR3;			// 木の位置
	D3DXVECTOR3 posPlayer = pPlayer.GetPos();		// プレイヤーの位置
	CListManager<CTree*> list = CTree::GetList(pPlayer.GetAreaIdx());
	CTree* pTree = nullptr;		// 先頭の値
	CTree* pTreeEnd = nullptr;	// 末尾の値
	int nIdx = 0;
	
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pTree = list.GetTop();

		// 末尾の値を取得する
		pTreeEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 木の位置を設定する
			posTree = pTree->GetPos();

			if (useful::CircleCollisionXZ(posPlayer, posTree, fRadius, TREE_RADIUS[pTree->GetType()]) &&
				posPlayer.y <= posTree.y + pTree->GetFileData().vtxMax.y &&
				posPlayer.y + fHeight >= posTree.y)
			{ // ダガーが木に接触した場合

				// ヒット処理
				pTree->Hit();
			}

			if (pTree == pTreeEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pTree = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

//===============================
// ヤシの木との当たり判定
//===============================
void collision::PalmFruitHit(CPlayer* pPlayer, const float fRadius, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posFruit = NONE_D3DXVECTOR3;		// 実の位置
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// プレイヤーの位置
	float fRadiusFruit = 0.0f;						// 半径

	CListManager<CPalmFruit*> list = CPalmFruit::GetList();
	CPalmFruit* pFruit = nullptr;		// 先頭の値
	CPalmFruit* pFruitEnd = nullptr;	// 末尾の値
	int nIdx = 0;
	
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pFruit = list.GetTop();

		// 末尾の値を取得する
		pFruitEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			posFruit = pFruit->GetPos();					// 実の位置
			fRadiusFruit = pFruit->GetFileData().vtxMax.x;	// 半径

			if (posPlayer.y <= posFruit.y + pFruit->GetFileData().vtxMax.y &&
				posPlayer.y + fHeight >= posFruit.y &&
				useful::CircleCollisionXZ(posPlayer, posFruit, fRadius, fRadiusFruit) == true &&
				pPlayer->GetState().state != CPlayer::STATE_DEATH &&
				pFruit->GetState() == CPalmFruit::STATE_STOP)
			{ // 円柱の当たり判定に入った場合

				// 回復処理
				pPlayer->Healing(PALM_FRUIT_HEALING);

				// ヒット処理
				pFruit->Hit();
			}

			if (pFruit == pFruitEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pFruit = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

//===============================
// ヤシの実への攻撃判定
//===============================
bool collision::PalmFruitAttack(const D3DXVECTOR3& pos, const float fRadius, const int nAreaIdx)
{
	// ローカル変数宣言
	CPalmFruit* pFruit = nullptr;	// ヤシの実の情報
	D3DXVECTOR3 posFruit = NONE_D3DXVECTOR3;	// 実の位置
	float fRadiusFruit = 0.0f;		// 実の半径

	CListManager<CTree*> list = CTree::GetList(nAreaIdx);
	CTree* pTree = nullptr;			// 先頭の値
	CTree* pTreeEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pTree = list.GetTop();

		// 末尾の値を取得する
		pTreeEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pTree->GetType() == CTree::TYPE::TYPE_PALM)
			{
				// ヤシの実を取得する
				pFruit = pTree->GetFruit();

				if (pFruit != nullptr)
				{ // ヤシの実が NULL じゃない場合

					posFruit = pFruit->GetPos();					// 実の位置
					fRadiusFruit = pFruit->GetFileData().vtxMax.x;	// 実の半径

					if (useful::CircleCollisionXY(pos, posFruit, fRadius, fRadiusFruit) == true &&
						useful::CircleCollisionXZ(pos, posFruit, fRadius, fRadiusFruit) == true &&
						useful::CircleCollisionYZ(pos, posFruit, fRadius, fRadiusFruit) == true)
					{ // ヤシの実に当たった場合

						// ヒット処理
						pTree->Hit();

						// この先の処理を行わない
						return true;
					}
				}
			}

			if (pTree == pTreeEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pTree = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 岩との当たり判定
//===============================
bool collision::RockCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const float fRadius, const float fHeight, const int nAreaIdx, float* fGravity, bool* bJump)
{
	// ローカル変数宣言
	D3DXVECTOR3 posRock = NONE_D3DXVECTOR3;		// 岩の位置
	float fRadiusRock = 0.0f;					// 岩の半径
	float fTopRock = 0.0f;						// 岩の上の高さ
	float fBottomRock = 0.0f;					// 岩の下の高さ
	bool bCollision = false;					// 当たり判定状況

	CListManager<CRock*> list = CRock::GetList(nAreaIdx);
	CRock* pRock = nullptr;			// 先頭の値
	CRock* pRockEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pRock = list.GetTop();

		// 末尾の値を取得する
		pRockEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 岩の位置を取得する
			posRock = pRock->GetPos();

			// 岩の半径を取得する
			fRadiusRock = pRock->GetRadius();

			if (useful::CircleCollisionXZ(*pos, posRock, fRadius, fRadiusRock) == true)
			{ // 円周に入った場合

				// 岩の上の高さを取得する
				fTopRock = pRock->GetTopHeight();

				// 岩の下の高さを取得する
				fBottomRock = pRock->GetBottomHeight();

				if (posRock.y + fTopRock <= posOld.y &&
					posRock.y + fTopRock >= pos->y)
				{ // 上にぶつかった場合

					// 位置を設定する
					pos->y = posRock.y + fTopRock + COLLISION_ADD_DIFF_LENGTH;

					if (bJump != nullptr)
					{ // ジャンプ状況が NULL じゃない場合

						// ジャンプ状況を false にする
						*bJump = false;
					}

					if (fGravity != nullptr)
					{ // 重力が NULL じゃない場合

						// 重力を0にする
						*fGravity = 0.0f;
					}

					// 当たり判定状況を true にする
					bCollision = true;
				}
				else if (posRock.y + fBottomRock >= posOld.y + fHeight &&
					posRock.y + fBottomRock <= pos->y + fHeight)
				{ // 下にぶつかった場合

					// 位置を設定する
					pos->y = posRock.y + fBottomRock - (fHeight + COLLISION_ADD_DIFF_LENGTH);

					// 当たり判定状況を true にする
					bCollision = true;
				}
				else if (pos->y <= posRock.y + fTopRock &&
					pos->y + fHeight >= posRock.y + fBottomRock &&
					useful::CylinderCollision(pos, posRock, fRadiusRock + fRadius) == true)
				{ // 当たった場合

					// 当たり判定状況を true にする
					bCollision = true;
				}
			}

			if (pRock == pRockEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pRock = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// 当たり判定状況を返す
	return bCollision;
}

//===============================
// 爆弾のヒット判定
//===============================
bool collision::BangFlowerHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posBomb = NONE_D3DXVECTOR3;		// 爆弾の位置
	float fRadiusBomb = 0.0f;					// 爆弾の
	float fHeightBomb = 0.0f;					// 
	CListManager<CBangFlower*> list = CBangFlower::GetList();
	CBangFlower* pBomb = nullptr;			// 先頭の値
	CBangFlower* pBombEnd = nullptr;		// 末尾の値
	int nIdx = 0;
	bool bHit = false;		// ヒット判定

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBomb = list.GetTop();

		// 末尾の値を取得する
		pBombEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pBomb->GetBomb() != nullptr &&
				pBomb->GetBomb()->GetState() == CBomb::STATE_RIPEN)
			{ // 爆弾が実り状態の場合

				// 爆弾の位置を取得する
				posBomb = pBomb->GetPos();

				// 爆弾の半径を取得する
				fRadiusBomb = pBomb->GetFileData().fRadius;

				// 爆弾の高さを取得する
				fHeightBomb = pBomb->GetFileData().vtxMax.y;

				if (useful::CircleCollisionXZ(pos, posBomb, fRadius, fRadiusBomb) == true &&
					pos.y + fHeight >= posBomb.y &&
					pos.y <= posBomb.y + fHeightBomb)
				{ // 範囲内にいた場合

					// ヒット処理
					pBomb->Hit();

					// ヒットした
					bHit = true;
				}
			}

			if (pBomb == pBombEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBomb = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// ヒット判定を返す
	return bHit;
}

//===============================
// 爆弾のヒット判定
//===============================
bool collision::BombHitToGun(const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld, const float fRadius)
{
	// ローカル変数宣言
	D3DXVECTOR3 posBomb = NONE_D3DXVECTOR3;		// 爆弾の位置
	float fRadiusBomb = 0.0f;					// 爆弾の半径
	float fRot = atan2f(pos.x - posOld.x, pos.z - posOld.z);	// 吹き飛ぶ向き

	CListManager<CBomb*> list = CBomb::GetList();
	CBomb* pBomb = nullptr;			// 先頭の値
	CBomb* pBombEnd = nullptr;		// 末尾の値
	int nIdx = 0;
	bool bHit = false;		// ヒット判定

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBomb = list.GetTop();

		// 末尾の値を取得する
		pBombEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pBomb->GetState() == CBomb::STATE_DETONATION)
			{ // 爆弾が起爆状態だった場合

				// 爆弾の位置を取得する
				posBomb = pBomb->GetPos();
				posBomb.y += (pBomb->GetFileData().vtxMax.y * 0.5f);

				// 爆弾の最大値を取得する
				fRadiusBomb = pBomb->GetFileData().fRadius;

				if (useful::CircleCollisionXY(pos, posBomb, fRadius, fRadiusBomb) == true &&
					useful::CircleCollisionYZ(pos, posBomb, fRadius, fRadiusBomb) == true &&
					useful::CircleCollisionXZ(pos, posBomb, fRadius, fRadiusBomb) == true)
				{ // 範囲内にいた場合

					// ヒット処理
					pBomb->Hit(fRot, BOMB_BULLET_SMASH);

					// ヒットした
					bHit = true;
				}
			}

			if (pBomb == pBombEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBomb = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// ヒット判定を返す
	return bHit;
}

//===============================
// 爆弾のヒット判定(ダガー)
//===============================
bool collision::BombHitToDagger(const D3DXVECTOR3& pos, const float fRadius, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posBomb = NONE_D3DXVECTOR3;		// 爆弾の位置
	float fRadiusBomb = 0.0f;	// 爆弾の半径
	float fHeightBomb = 0.0f;	// 爆弾の高さ
	float fRot = 0.0f;			// 吹き飛ぶ向き

	CListManager<CBomb*> list = CBomb::GetList();
	CBomb* pBomb = nullptr;			// 先頭の値
	CBomb* pBombEnd = nullptr;		// 末尾の値
	int nIdx = 0;
	bool bHit = false;		// ヒット判定

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBomb = list.GetTop();

		// 末尾の値を取得する
		pBombEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pBomb->GetState() == CBomb::STATE_DETONATION)
			{ // 爆弾が起爆状態だった場合

				// 爆弾関係の情報を取得する
				posBomb = pBomb->GetPos();
				fRadiusBomb = pBomb->GetFileData().fRadius;
				fHeightBomb = pBomb->GetFileData().vtxMax.y;

				if (useful::CircleCollisionXZ(pos, posBomb, fRadius, fRadiusBomb) == true &&
					pos.y + fHeight >= posBomb.y &&
					pos.y <= posBomb.y + fHeightBomb)
				{ // 範囲内にいた場合

					// 向きを算出する
					fRot = atan2f(posBomb.x - pos.x, posBomb.z - pos.z);

					// ヒット処理
					pBomb->Hit(fRot, BOMB_DAGGER_SMASH);

					// ヒットした
					bHit = true;
				}
			}

			if (pBomb == pBombEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBomb = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// ヒット判定を返す
	return bHit;
}

//===============================
// 爆弾のヒット判定(斬撃波紋)
//===============================
bool collision::BombHitToSlashRipple(const D3DXVECTOR3& pos, const float fRadius, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posBomb = NONE_D3DXVECTOR3;		// 爆弾の位置
	float fRadiusBomb = 0.0f;	// 爆弾の半径
	float fHeightBomb = 0.0f;	// 爆弾の高さ
	float fRot = 0.0f;			// 吹き飛ぶ向き

	CListManager<CBomb*> list = CBomb::GetList();
	CBomb* pBomb = nullptr;			// 先頭の値
	CBomb* pBombEnd = nullptr;		// 末尾の値
	int nIdx = 0;
	bool bHit = false;		// ヒット判定

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBomb = list.GetTop();

		// 末尾の値を取得する
		pBombEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pBomb->GetState() == CBomb::STATE_DETONATION)
			{ // 爆弾が起爆状態だった場合

				// 爆弾関係の情報を取得する
				posBomb = pBomb->GetPos();
				fRadiusBomb = pBomb->GetFileData().fRadius;
				fHeightBomb = pBomb->GetFileData().vtxMax.y;

				if (useful::CircleCollisionXZ(pos, posBomb, fRadius, fRadiusBomb) == true &&
					pos.y + fHeight >= posBomb.y &&
					pos.y <= posBomb.y + fHeightBomb)
				{ // 範囲内にいた場合

					// 向きを算出する
					fRot = atan2f(posBomb.x - pos.x, posBomb.z - pos.z);

					// ヒット処理
					pBomb->Hit(fRot, BOMB_SLASH_RIPPLE_SMASH);

					// ヒットした
					bHit = true;
				}
			}

			if (pBomb == pBombEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBomb = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// ヒット判定を返す
	return bHit;
}

//===============================
// 爆風と岩との当たり判定
//===============================
void collision::ExplosionHitToRock(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, const int nAreaIdx)
{
	// ローカル変数宣言
	D3DXVECTOR3 posRock = NONE_D3DXVECTOR3;		// 岩の位置
	float fRadiusRock = 0.0f;					// 岩の半径
	float fTopRock = 0.0f;						// 岩の上の高さ
	float fBottomRock = 0.0f;					// 岩の下の高さ

	CListManager<CRock*> list = CRock::GetList(nAreaIdx);
	CRock* pRock = nullptr;			// 先頭の値
	CRock* pRockEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pRock = list.GetTop();

		// 末尾の値を取得する
		pRockEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pRock->GetType() == CRock::TYPE_BREAK)
			{ // 破壊できる岩の場合

				// 岩の位置を取得する
				posRock = pRock->GetPos();

				// 岩の半径を取得する
				fRadiusRock = pRock->GetRadius();

				// 岩の上の高さを取得する
				fTopRock = pRock->GetTopHeight();

				// 岩の下の高さを取得する
				fBottomRock = pRock->GetBottomHeight();

				if (useful::CircleCollisionXZ(pos, posRock, fRadius, fRadiusRock) == true &&
					pos.y <= posRock.y + fTopRock &&
					pos.y + fHeight >= posRock.y + fBottomRock)
				{ // 範囲内にいた場合

					// 破壊処理
					pRock->Break();
				}
			}

			if (pRock == pRockEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pRock = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

//===============================
// 爆風と敵との当たり判定
//===============================
void collision::ExplosionHitToEnemy(const D3DXVECTOR3& pos, const float fRadius, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;	// 敵の位置
	float fRadiusEnemy = 0.0f;					// 敵の半径
	float fHeightEnemy = 0.0f;					// 敵の高さ

	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;			// 先頭の値
	CEnemy* pEnemyEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pEnemy = list.GetTop();

		// 末尾の値を取得する
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pEnemy->GetState() == CEnemy::STATE_NONE)
			{ // 通常状態の場合

				// 敵関係の変数を取得する
				posEnemy = pEnemy->GetPos();
				fRadiusEnemy = pEnemy->GetCollSize().x;
				fHeightEnemy = pEnemy->GetCollSize().y;

				if (useful::CircleCollisionXZ(pos, posEnemy, fRadius, fRadiusEnemy) == true &&
					pos.y <= posEnemy.y + fHeightEnemy &&
					pos.y + fHeight >= posEnemy.y)
				{ // 範囲内にいた場合

					// 破壊処理
					pEnemy->Hit(EXPLOSION_DAMAGE, EXPLOSION_KNOCKBACK);
				}
			}

			if (pEnemy == pEnemyEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pEnemy = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

//===============================
// 爆風とプレイヤーとの当たり判定
//===============================
bool collision::ExplosionHitToPlayer(CPlayer* pPlayer, const float fRadius, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = pPlayer->GetPos();	// プレイヤーの位置
	D3DXVECTOR3 posExpl = NONE_D3DXVECTOR3;	// 爆風の位置
	float fRadiusExpl = 0.0f;				// 爆風の半径
	float fHeightExpl = 0.0f;				// 爆風の高さ
	float fRotSmash = 0.0f;					// 吹き飛ぶ方向

	CListManager<CBombExplosion*> list = CBombExplosion::GetList();
	CBombExplosion* pBombExplosion = nullptr;			// 先頭の値
	CBombExplosion* pBombExplosionEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBombExplosion = list.GetTop();

		// 末尾の値を取得する
		pBombExplosionEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 爆発関係の変数を取得する
			posExpl = pBombExplosion->GetPos();
			fRadiusExpl = pBombExplosion->GetCircum();
			fHeightExpl = pBombExplosion->GetHeight();

			if (useful::CircleCollisionXZ(pos, posExpl, fRadius, fRadiusExpl) == true &&
				pos.y <= posExpl.y + fHeightExpl &&
				pos.y + fHeight >= posExpl.y - fHeightExpl)
			{ // 範囲内にいた場合

				// 吹き飛ぶ向きを算出する
				fRotSmash = atan2f(pos.x - posExpl.x, pos.z - posExpl.z);

				// プレイヤーのヒット処理
				pPlayer->Hit(EXPLOSION_DAMAGE, fRotSmash);

				// true を返す
				return true;
			}

			if (pBombExplosion == pBombExplosionEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBombExplosion = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 壁との当たり判定
//===============================
bool collision::WallCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx)
{
	// ローカル変数宣言
	D3DXVECTOR3 posWall = NONE_D3DXVECTOR3;			// 壁の位置
	D3DXVECTOR3 posOldWall = NONE_D3DXVECTOR3;		// 壁の前回の位置
	D3DXVECTOR3 vtxMaxWall = NONE_D3DXVECTOR3;		// 壁の最大値
	D3DXVECTOR3 vtxMinWall = NONE_D3DXVECTOR3;		// 壁の最小値

	CListManager<CWall*> list = CWall::GetList(nAreaIdx);
	CWall* pWall = nullptr;			// 先頭の値
	CWall* pWallEnd = nullptr;		// 末尾の値
	int nIdx = 0;
	bool bHit = false;		// ヒット判定

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pWall = list.GetTop();

		// 末尾の値を取得する
		pWallEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 壁の頂点を取得する
			vtxMaxWall = pWall->GetVtxMax();
			vtxMinWall = pWall->GetVtxMin();

			// 壁の位置を取得する
			posWall = pWall->GetPos();
			posOldWall = pWall->GetPosOld();

			if (HexahedronCollision
			(
				pos,
				posWall,
				posOld,
				posOldWall,
				vtxMin,
				vtxMinWall,
				vtxMax,
				vtxMaxWall
			) == true)
			{ // 壁に当たった場合

				// ヒットした
				bHit = true;
			}

			if (pWall == pWallEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pWall = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// ヒット判定を返す
	return bHit;
}

//===============================
// ブロックとのヒット判定
//===============================
bool collision::BlockHit(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx)
{
	// ローカル変数宣言
	D3DXVECTOR3 posBlock = NONE_D3DXVECTOR3;		// 壁の位置
	D3DXVECTOR3 posOldBlock = NONE_D3DXVECTOR3;		// 壁の前回の位置
	D3DXVECTOR3 vtxMaxBlock = NONE_D3DXVECTOR3;		// 壁の最大値
	D3DXVECTOR3 vtxMinBlock = NONE_D3DXVECTOR3;		// 壁の最小値

	CListManager<CBlock*> list = CBlock::GetList(nAreaIdx);
	CBlock* pBlock = nullptr;			// 先頭の値
	CBlock* pBlockEnd = nullptr;		// 末尾の値
	int nIdx = 0;
	bool bHit = false;		// ヒット判定

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBlock = list.GetTop();

		// 末尾の値を取得する
		pBlockEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 壁の頂点を取得する
			vtxMaxBlock = pBlock->GetVtxMax();
			vtxMinBlock = pBlock->GetVtxMin();

			// 壁の位置を取得する
			posBlock = pBlock->GetPos();
			posOldBlock = pBlock->GetPosOld();

			if (HexahedronCollision
			(
				pos,
				posBlock,
				posOld,
				posOldBlock,
				vtxMin,
				vtxMinBlock,
				vtxMax,
				vtxMaxBlock
			) == true)
			{ // 壁に当たった場合

				// ヒットした
				bHit = true;
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

	// ヒット判定を返す
	return bHit;
}

//===============================
// ボスの当たり判定
//===============================
bool collision::BossHit(const D3DXVECTOR3& pos, const float fRadius, const int nDamage)
{
	// ローカル変数宣言
	D3DXVECTOR3 posPart = NONE_D3DXVECTOR3;
	CBossCollision* coll = nullptr;
	D3DXMATRIX mtxScale, mtxRot, mtxTrans, mtx, mtxColl;	// 計算用マトリックス
	D3DXMATRIX mtxWorld;		// マトリックスを取得する

	CListManager<CBoss*> list = CBoss::GetList();
	CBoss* pBoss = nullptr;			// 先頭の値
	CBoss* pBossEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBoss = list.GetTop();

		// 末尾の値を取得する
		pBossEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// 拡大率を反映
			D3DXMatrixScaling(&mtxScale, pBoss->GetScale().x, pBoss->GetScale().y, pBoss->GetScale().z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->GetRot().y + D3DX_PI, pBoss->GetRot().x, pBoss->GetRot().z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pBoss->GetPos().x, pBoss->GetPos().y, pBoss->GetPos().z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			for (int nCntPart = 0; nCntPart < pBoss->GetNumModel(); nCntPart++)
			{
				// マトリックスの計算処理
				pBoss->GetHierarchy(nCntPart)->MatrixCalc(&mtx, mtxWorld);

				// 当たり判定の情報を取得
				coll = pBoss->GetColl(nCntPart);

				if (coll != nullptr)
				{ // 当たり判定が NULL じゃない場合

					for (int nCntColl = 0; nCntColl < coll->GetNumColl(); nCntColl++)
					{
						// マトリックスの初期化
						D3DXMatrixIdentity(&mtxColl);

						// 位置を反映
						D3DXMatrixTranslation(&mtxTrans, coll->GetCollOffset(nCntColl).x, coll->GetCollOffset(nCntColl).y, coll->GetCollOffset(nCntColl).z);
						D3DXMatrixMultiply(&mtxColl, &mtxColl, &mtxTrans);

						// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
						D3DXMatrixMultiply
						(
							&mtxColl,
							&mtxColl,
							&mtx
						);

						// 位置を設定する
						posPart.x = mtxColl._41;
						posPart.y = mtxColl._42;
						posPart.z = mtxColl._43;

						if (useful::CircleCollisionXY(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true &&
							useful::CircleCollisionYZ(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true &&
							useful::CircleCollisionXZ(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true)
						{ // 球が当たった場合

							if (coll->IsWeakness(nCntColl) == true)
							{ // 弱点だった場合

								// バリア破壊処理
								pBoss->BarrierBreak(pos, nCntPart, nDamage);
							}
							else
							{ // 上記以外

								// ヒット処理
								pBoss->Hit(BOSS_DAMAGE);
							}

							// true を返す
							return true;
						}
					}
				}
			}

			if (pBoss == pBossEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBoss = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// ボスとのヒット判定
//===============================
bool collision::BossAttack(const D3DXVECTOR3& pos, const float fRadius, float* fRotSmash)
{
	// ローカル変数宣言
	D3DXVECTOR3 posPart = NONE_D3DXVECTOR3;
	CBossCollision* coll = nullptr;
	D3DXMATRIX mtxScale, mtxRot, mtxTrans, mtx, mtxColl;	// 計算用マトリックス
	D3DXMATRIX mtxWorld;		// マトリックスを取得する

	CListManager<CBoss*> list = CBoss::GetList();
	CBoss* pBoss = nullptr;			// 先頭の値
	CBoss* pBossEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pBoss = list.GetTop();

		// 末尾の値を取得する
		pBossEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pBoss->IsHit() == true)
			{ // ヒット状況が true の場合

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorld);

				// 拡大率を反映
				D3DXMatrixScaling(&mtxScale, pBoss->GetScale().x, pBoss->GetScale().y, pBoss->GetScale().z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

				// 向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->GetRot().y + D3DX_PI, pBoss->GetRot().x, pBoss->GetRot().z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, pBoss->GetPos().x, pBoss->GetPos().y, pBoss->GetPos().z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

				for (int nCntPart = 0; nCntPart < pBoss->GetNumModel(); nCntPart++)
				{
					// マトリックスの計算処理
					pBoss->GetHierarchy(nCntPart)->MatrixCalc(&mtx, mtxWorld);

					// 当たり判定の情報を取得
					coll = pBoss->GetColl(nCntPart);

					if (coll != nullptr)
					{ // 当たり判定が NULL じゃない場合

						for (int nCntColl = 0; nCntColl < coll->GetNumColl(); nCntColl++)
						{
							// マトリックスの初期化
							D3DXMatrixIdentity(&mtxColl);

							// 位置を反映
							D3DXMatrixTranslation(&mtxTrans, coll->GetCollOffset(nCntColl).x, coll->GetCollOffset(nCntColl).y, coll->GetCollOffset(nCntColl).z);
							D3DXMatrixMultiply(&mtxColl, &mtxColl, &mtxTrans);

							// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
							D3DXMatrixMultiply
							(
								&mtxColl,
								&mtxColl,
								&mtx
							);

							// 位置を設定する
							posPart.x = mtxColl._41;
							posPart.y = mtxColl._42;
							posPart.z = mtxColl._43;

							if (useful::CircleCollisionXY(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true &&
								useful::CircleCollisionYZ(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true &&
								useful::CircleCollisionXZ(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true)
							{ // 球が当たった場合

								if (fRotSmash != nullptr)
								{ // 吹き飛ぶ向きが NULL じゃない場合

									// 吹き飛ぶ向きを設定する
									*fRotSmash = atan2f(pos.x - posPart.x, pos.z - posPart.z);
								}

								// true を返す
								return true;
							}
						}
					}
				}
			}

			if (pBoss == pBossEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pBoss = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 祭壇との当たり判定
//===============================
bool collision::AlterCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin)
{
	// 当たり判定の変数を宣言
	CAlter* pAlter = CGame::GetAlter();
	CAlterPole* pPole = nullptr;
	D3DXVECTOR3 posAlter;
	D3DXVECTOR3 posOldAlter;
	D3DXVECTOR3 vtxMinAlter;
	D3DXVECTOR3 vtxMaxAlter;

	if (pAlter != nullptr &&
		pAlter->GetState() != CAlter::STATE_BREAK)
	{ // 祭壇が壊れていない場合

		// 位置とサイズを取得する
		posAlter = pAlter->GetPos();
		posOldAlter = pAlter->GetPosOld();
		vtxMinAlter = pAlter->GetFileData().vtxMin;
		vtxMaxAlter = pAlter->GetFileData().vtxMax;

		if (HexahedronCollision
		(
			pos,
			posAlter,
			posOld,
			posOldAlter,
			vtxMin,
			vtxMinAlter,
			vtxMax,
			vtxMaxAlter
		) == true)
		{ // 祭壇に当たった場合

			// true を返す
			return true;
		}

		for (int nCntPole = 0; nCntPole < CAlter::NUM_POLE; nCntPole++)
		{
			// 石柱の情報を取得する
			pPole = pAlter->GetPole(nCntPole);

			if (pPole != nullptr)
			{ // 石柱が NULL じゃない場合

				// 位置とサイズを取得する
				posAlter = pPole->GetPos();
				posOldAlter = pPole->GetPosOld();
				vtxMaxAlter = pPole->GetFileData().vtxMax;

				if (useful::CylinderCollision(pos, posAlter, vtxMax.x + vtxMaxAlter.x) == true)
				{ // 祭壇に当たった場合

					// true を返す
					return true;
				}
			}
		}
	}

	// false を返す
	return false;
}

//===============================
// 祭壇周囲の当たり判定
//===============================
void collision::AlterSurrounding(const D3DXVECTOR3& pos, const float fRadius)
{
	// 当たり判定の変数を宣言
	CAlter* pAlter = CGame::GetAlter();
	D3DXVECTOR3 posAlter = NONE_D3DXVECTOR3;

	if (pAlter != nullptr &&
		pAlter->GetState() == CAlter::STATE_NONE)
	{ // 祭壇に骨が揃っていない場合

		// 位置とサイズを取得する
		posAlter = pAlter->GetPos();

		if (useful::CircleCollisionXZ(pos,posAlter,fRadius, ALTER_SURROUND_RADIUS))
		{ // 祭壇の周囲にいた場合

			// ライト点灯状況を true にする
			pAlter->SetEnableLightUp(true);

			// メッセージを表示する
			pAlter->SetAlterMessage(CAlterMessage::Create(posAlter));
		}
		else
		{ // 上記以外

			// ライト点灯状況を false にする
			pAlter->SetEnableLightUp(false);

			// メッセージを消去する
			pAlter->SetAlterMessage(nullptr);
		}
	}
}

//===============================
// 斬撃の波紋の当たり判定
//===============================
bool collision::RippleHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, float* fRotSmash)
{
	// ローカル変数宣言
	D3DXVECTOR3 posRipple = NONE_D3DXVECTOR3;		// 壁の位置
	float fRadiusRipple = 0.0f;						// 波紋の半径
	float fHeightRipple = 0.0f;						// 波紋の高さ

	CListManager<CSlashRipple*> list = CSlashRipple::GetList();
	CSlashRipple* pRipple = nullptr;			// 先頭の値
	CSlashRipple* pRippleEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pRipple = list.GetTop();

		// 末尾の値を取得する
		pRippleEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 波紋関係の変数を設定する
			posRipple = pRipple->GetPos();
			fRadiusRipple = pRipple->GetFileData().fRadius * pRipple->GetScale().x;
			fHeightRipple = pRipple->GetFileData().vtxMax.y;

			if (useful::CircleCollisionXZ(pos, posRipple, fRadius, fRadiusRipple) == true &&
				pos.y <= posRipple.y + fHeightRipple &&
				pos.y + fHeight >= posRipple.y)
			{ // 斬撃に当たった場合

				if (fRotSmash != nullptr)
				{ // 向きが NULL じゃない場合

					// 向きを設定する
					*fRotSmash = atan2f(pos.x - posRipple.x, pos.z - posRipple.z);
				}

				// true を返す
				return true;
			}

			if (pRipple == pRippleEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pRipple = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 風攻撃とプレイヤーの当たり判定
//===============================
bool collision::WindShotHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, float* fSmashRot)
{
	// ローカル変数宣言
	D3DXVECTOR3 posWind = NONE_D3DXVECTOR3;		// 壁の位置
	float fRadiusWind = 0.0f;					// 風の半径
	float fHeightWind = 0.0f;					// 風の高さ

	CListManager<CWindShot*> list = CWindShot::GetList();
	CWindShot* pWind = nullptr;			// 先頭の値
	CWindShot* pWindEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pWind = list.GetTop();

		// 末尾の値を取得する
		pWindEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pWind->GetState() != CWindShot::STATE_DELETE)
			{ // 消去状態以外

				// 風関係の変数を設定する
				posWind = pWind->GetPos();
				fRadiusWind = pWind->GetCircum() + pWind->GetWidth();
				fHeightWind = pWind->GetHeight() * pWind->GetVortex();

				if (useful::CircleCollisionXZ(pos, posWind, fRadius, fRadiusWind) == true &&
					pos.y <= posWind.y + fHeightWind &&
					pos.y + fHeight >= posWind.y)
				{ // 風攻撃に当たった場合

					if (fSmashRot != nullptr)
					{ // 向きのポインタが NULL じゃない場合

						// 吹き飛ぶ向きを設定する
						*fSmashRot = atan2f(pos.x - posWind.x, pos.z - posWind.z);
					}

					// true を返す
					return true;
				}
			}

			if (pWind == pWindEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pWind = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 炎攻撃との当たり判定
//===============================
bool collision::FireShotHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, float* fSmashRot)
{
	// ローカル変数宣言
	D3DXVECTOR3 posFire = NONE_D3DXVECTOR3;		// 壁の位置
	float fRadiusFire = 0.0f;					// 炎の半径
	float fHeightFire = 0.0f;					// 炎の高さ

	CListManager<CFireShot*> list = CFireShot::GetList();
	CFireShot* pFire = nullptr;			// 先頭の値
	CFireShot* pFireEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pFire = list.GetTop();

		// 末尾の値を取得する
		pFireEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 炎関係の変数を設定する
			posFire = pFire->GetPos();
			fRadiusFire = pFire->GetCircum();
			fHeightFire = pFire->GetHeight();

			if (useful::CircleCollisionXZ(pos, posFire, fRadius, fRadiusFire) == true &&
				pos.y <= posFire.y + fHeightFire &&
				pos.y + fHeight >= posFire.y)
			{ // 炎攻撃に当たった場合

				if (fSmashRot != nullptr)
				{ // 向きのポインタが NULL じゃない場合

					// 吹き飛ぶ向きを設定する
					*fSmashRot = atan2f(pos.x - posFire.x, pos.z - posFire.z);
				}

				// true を返す
				return true;
			}

			if (pFire == pFireEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pFire = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 看板との当たり判定
//===============================
bool collision::SignboardCollision(const D3DXVECTOR3& pos, const float fRadius)
{
	// ローカル変数宣言
	D3DXVECTOR3 posSign = NONE_D3DXVECTOR3;		// 壁の位置
	float fRadiusSign = 0.0f;					// 看板の半径

	CListManager<CSignboard*> list = CSignboard::GetList();
	CSignboard* pSign = nullptr;			// 先頭の値
	CSignboard* pSignEnd = nullptr;			// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pSign = list.GetTop();

		// 末尾の値を取得する
		pSignEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 看板関係の変数を設定する
			posSign = pSign->GetPos();
			fRadiusSign = pSign->GetFileData().fRadius;

			if (useful::CircleCollisionXZ(pos, posSign, fRadius, fRadiusSign + SIGNBOARD_ADD_RADIUS) == true)
			{ // 看板に近づいた場合

				// ボタンを描画する
				pSign->GetButton()->SetEnableDisp(true);

				if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
					CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
				{ // ボタンを押した場合

					// 説明移行処理
					pSign->Explain();

					// true を返す
					return true;
				}
			}
			else
			{ // 上記以外

				// ボタンを描画しない
				pSign->GetButton()->SetEnableDisp(false);
			}

			if (pSign == pSignEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pSign = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// 的との当たり判定
//===============================
bool collision::TargetHit(const D3DXVECTOR3& pos, const float fRadius)
{
	// ローカル変数宣言
	CBalloon* pBalloon = nullptr;				// 風船の情報
	D3DXVECTOR3 posBalloon = NONE_D3DXVECTOR3;	// 風船の位置
	float fRadiusBalloon = 0.0f;				// 風船の半径
	CListManager<CBalloonSpawner*> list = CBalloonSpawner::GetList();
	CBalloonSpawner* pSpawner = nullptr;		// 先頭の風船スポナー
	CBalloonSpawner* pSpawnerEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pSpawner = list.GetTop();

		// 末尾の値を取得する
		pSpawnerEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			if (pSpawner->GetBalloon() != nullptr)
			{ // 風船が NULL じゃない場合

				// 風船の情報を取得
				pBalloon = pSpawner->GetBalloon();

				// 情報を設定
				posBalloon = pBalloon->GetPos();
				fRadiusBalloon = pBalloon->GetFileData().fRadius;

				if (useful::CircleCollisionXY(pos, posBalloon, fRadius, fRadiusBalloon) == true &&
					useful::CircleCollisionXZ(pos, posBalloon, fRadius, fRadiusBalloon) == true &&
					useful::CircleCollisionYZ(pos, posBalloon, fRadius, fRadiusBalloon) == true)
				{ // 風船と重なった場合

					// ヒット処理
					pSpawner->Hit();

					// true を返す
					return true;
				}
			}

			if (pSpawner == pSpawnerEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pSpawner = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//===============================
// ドアとのヒット判定
//===============================
bool collision::DoorHit(const D3DXVECTOR3& pos, const float fRadius)
{
	// ローカル変数宣言
	CDoor* pDoor = CTutorial::GetDoor();		// ドアの情報

	if (pDoor != nullptr)
	{ // ドアが存在する場合

		// ドアの情報を取得する
		D3DXVECTOR3 posDoor = pDoor->GetPos();									// ドアの位置
		float fRadiusDoor = pDoor->GetFileData().vtxMax.x + DOOR_ADD_RADIUS;	// ドアの半径
		float fAngle = atan2f(pos.x - posDoor.x, pos.z - posDoor.z);			// 方向

		if (((fAngle <= -D3DX_PI + DOOR_HIT_ANGLE && fAngle >= -D3DX_PI) ||
			(fAngle >= D3DX_PI - DOOR_HIT_ANGLE && fAngle <= D3DX_PI)) &&
			useful::CircleCollisionXZ(pos, posDoor, fRadius, fRadiusDoor) == true)
		{ // ドアの近くにいた場合

			// ボタン表示を描画する
			pDoor->GetButton()->SetEnableDisp(true);

			if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
				CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
			{ // ボタンを押した場合

				// 開き状態にする
				pDoor->SetState(CDoor::STATE_OPEN);

				// 遷移状態にする
				CTutorial::SetState(CTutorial::STATE_TRANS);

				// true を返す
				return true;
			}
		}
		else
		{ // 上記以外

			// ボタン表示を描画しない
			pDoor->GetButton()->SetEnableDisp(false);
		}
	}

	// false を返す
	return false;
}

//===============================
// ドアとの当たり判定
//===============================
bool collision::DoorCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& size)
{
	// ローカル変数宣言
	CDoor* pDoor = CTutorial::GetDoor();		// ドアの情報

	if (pDoor != nullptr)
	{ // ドアが存在する場合

		// ドアの情報を取得する
		D3DXVECTOR3 posDoor = pDoor->GetPos();					// ドアの位置
		D3DXVECTOR3 posOldDoor = pDoor->GetPosOld();			// ドアの前回の位置
		D3DXVECTOR3 vtxMaxDoor = pDoor->GetFileData().vtxMax;	// ドアの頂点の最大値
		D3DXVECTOR3 vtxMinDoor = pDoor->GetFileData().vtxMin;	// ドアの頂点の最小値
		D3DXVECTOR3 vtxMax = size;								// 頂点の最大値
		D3DXVECTOR3 vtxMin = useful::VtxMinConv(size);			// 頂点の最小値

		// 六面体の当たり判定
		if (HexahedronClush
		(
			pos,
			posDoor,
			posOld,
			posOldDoor,
			vtxMin,
			vtxMinDoor,
			vtxMax,
			vtxMaxDoor
		).bTop == true)
		{ // 上に乗った場合

			// true を返す
			return true;
		}
	}

	// false を返す
	return false;
}

//===============================
// ステージの当たり判定
//===============================
void collision::StageCollision(D3DXVECTOR3* pos, const float fWidth)
{
	if (pos->x + fWidth >= STAGE_RANGE)
	{ // 右から出た場合

		// 位置を補正する
		pos->x = STAGE_RANGE - fWidth;
	}

	if (pos->x - fWidth <= -STAGE_RANGE)
	{ // 左から出た場合

		// 位置を補正する
		pos->x = -STAGE_RANGE + fWidth;
	}

	if (pos->z + fWidth >= STAGE_RANGE)
	{ // 奥から出た場合

		// 位置を補正する
		pos->z = STAGE_RANGE - fWidth;
	}

	if (pos->z - fWidth <= -STAGE_RANGE)
	{ // 手前から出た場合

		// 位置を補正する
		pos->z = -STAGE_RANGE + fWidth;
	}
}

/*
* @brief 六面体の当たり判定
* @param pos [out] 対象の位置
* @param posBlock [in] ブロックの位置
* @param posOld [in] 対象の前回の位置
* @param posOldBlock [in] ブロックの前回の位置
* @param min [in] 最小値
* @param minBlock [in] ブロックの最小値
* @param max [in] 最大値
* @param maxBlock [in] ブロックの最大値
* @return bool ぶつかったかどうか
*/
bool collision::HexahedronCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock)
{
	if (posBlock.y + maxBlock.y >= pos->y + min.y &&
		posBlock.y + minBlock.y <= pos->y + max.y &&
		posBlock.z + maxBlock.z >= pos->z + min.z &&
		posBlock.z + minBlock.z <= pos->z + max.z)
	{ // X軸の判定に入れる場合

		if (posOldBlock.x + maxBlock.x <= posOld.x + min.x &&
			posBlock.x + maxBlock.x >= pos->x + min.x)
		{ // 右にぶつかった場合

			// 位置を設定する
			pos->x = posBlock.x + maxBlock.x - (min.x - COLLISION_ADD_DIFF_LENGTH);

			// true を返す
			return true;
		}
		else if (posOldBlock.x + minBlock.x >= posOld.x + max.x &&
			posBlock.x + minBlock.x <= pos->x + max.x)
		{ // 左にぶつかった場合

			// 位置を設定する
			pos->x = posBlock.x + minBlock.x - (max.x + COLLISION_ADD_DIFF_LENGTH);

			// true を返す
			return true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos->x + min.x &&
		posBlock.x + minBlock.x <= pos->x + max.x &&
		posBlock.y + maxBlock.y >= pos->y + min.y &&
		posBlock.y + minBlock.y <= pos->y + max.y)
	{ // Z軸の判定に入れる場合

		if (posOldBlock.z + maxBlock.z <= posOld.z + min.z &&
			posBlock.z + maxBlock.z >= pos->z + min.z)
		{ // 奥にぶつかった場合

			// 位置を設定する
			pos->z = posBlock.z + maxBlock.z - (min.z - COLLISION_ADD_DIFF_LENGTH);

			// true を返す
			return true;
		}
		else if (posOldBlock.z + minBlock.z >= posOld.z + max.z &&
			posBlock.z + minBlock.z <= pos->z + max.z)
		{ // 手前にぶつかった場合

			// 位置を設定する
			pos->z = posBlock.z + minBlock.z - (max.z + COLLISION_ADD_DIFF_LENGTH);

			// true を返す
			return true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos->x + min.x &&
		posBlock.x + minBlock.x <= pos->x + max.x &&
		posBlock.z + maxBlock.z >= pos->z + min.z &&
		posBlock.z + minBlock.z <= pos->z + max.z)
	{ // Y軸の判定に入れる場合

		if (posOldBlock.y + maxBlock.y <= posOld.y + min.y &&
			posBlock.y + maxBlock.y >= pos->y + min.y)
		{ // 上にぶつかった場合

			// 位置を設定する
			pos->y = posBlock.y + maxBlock.y - (min.y - COLLISION_ADD_DIFF_LENGTH);

			// true を返す
			return true;
		}
		else if (posOldBlock.y + minBlock.y >= posOld.y + max.y &&
			posBlock.y + minBlock.y <= pos->y + max.y)
		{ // 下にぶつかった場合

			// 位置を設定する
			pos->y = posBlock.y + minBlock.y - (max.y + COLLISION_ADD_DIFF_LENGTH);

			// true を返す
			return true;
		}
	}

	// false を返す
	return false;
}

/*
* @brief 六面体のヒット判定
* @param pos [in] 対象の位置
* @param posBlock [in] ブロックの位置
* @param posOld [in] 対象の前回の位置
* @param posOldBlock [in] ブロックの前回の位置
* @param min [in] 最小値
* @param minBlock [in] ブロックの最小値
* @param max [in] 最大値
* @param maxBlock [in] ブロックの最大値
* @return bool ぶつかったかどうか
*/
bool collision::HexahedronHit(const D3DXVECTOR3& pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock)
{
		if (posBlock.y + maxBlock.y >= pos.y + min.y &&
		posBlock.y + minBlock.y <= pos.y + max.y &&
		posBlock.z + maxBlock.z >= pos.z + min.z &&
		posBlock.z + minBlock.z <= pos.z + max.z)
	{ // X軸の判定に入れる場合

		if (posOldBlock.x + maxBlock.x <= posOld.x + min.x &&
			posBlock.x + maxBlock.x >= pos.x + min.x)
		{ // 右にぶつかった場合

			// true を返す
			return true;
		}
		else if (posOldBlock.x + minBlock.x >= posOld.x + max.x &&
			posBlock.x + minBlock.x <= pos.x + max.x)
		{ // 左にぶつかった場合

			// true を返す
			return true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos.x + min.x &&
		posBlock.x + minBlock.x <= pos.x + max.x &&
		posBlock.y + maxBlock.y >= pos.y + min.y &&
		posBlock.y + minBlock.y <= pos.y + max.y)
	{ // Z軸の判定に入れる場合

		if (posOldBlock.z + maxBlock.z <= posOld.z + min.z &&
			posBlock.z + maxBlock.z >= pos.z + min.z)
		{ // 奥にぶつかった場合

			// true を返す
			return true;
		}
		else if (posOldBlock.z + minBlock.z >= posOld.z + max.z &&
			posBlock.z + minBlock.z <= pos.z + max.z)
		{ // 手前にぶつかった場合

			// true を返す
			return true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos.x + min.x &&
		posBlock.x + minBlock.x <= pos.x + max.x &&
		posBlock.z + maxBlock.z >= pos.z + min.z &&
		posBlock.z + minBlock.z <= pos.z + max.z)
	{ // Y軸の判定に入れる場合

		if (posOldBlock.y + maxBlock.y <= posOld.y + min.y &&
			posBlock.y + maxBlock.y >= pos.y + min.y)
		{ // 上にぶつかった場合

			// true を返す
			return true;
		}
		else if (posOldBlock.y + minBlock.y >= posOld.y + max.y &&
			posBlock.y + minBlock.y <= pos.y + max.y)
		{ // 下にぶつかった場合

			// true を返す
			return true;
		}
	}

	// false を返す
	return false;
}
/*
// @brief 六面体のヒット判定(Old無し)
* @param pos [in] 対象の位置
* @param posBlock [in] ブロックの位置
* @param min [in] 最小値
* @param minBlock [in] ブロックの最小値
* @param max [in] 最大値
* @param maxBlock [in] ブロックの最大値
* @return SCollision 六面体の当たり判定
*/
bool collision::HexahedronHit(const D3DXVECTOR3& pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock)
{
	if (useful::RectangleCollisionXY(pos, posBlock, max, maxBlock, min, minBlock) == true &&
		useful::RectangleCollisionYZ(pos, posBlock, max, maxBlock, min, minBlock) == true &&
		useful::RectangleCollisionXZ(pos, posBlock, max, maxBlock, min, minBlock) == true)
	{ // 六面体同士が触れていた場合

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

/*
* @brief 六面体の当たり判定(どの面に乗ったかの判定付き)
* @param pos [out] 対象の位置
* @param posBlock [in] ブロックの位置
* @param posOld [in] 対象の前回の位置
* @param posOldBlock [in] ブロックの前回の位置
* @param min [in] 最小値
* @param minBlock [in] ブロックの最小値
* @param max [in] 最大値
* @param maxBlock [in] ブロックの最大値
* @return SCollision 六面体の当たり判定
*/
collision::SCollision collision::HexahedronClush(D3DXVECTOR3* pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock)
{
	// 当たり判定の変数を宣言
	SCollision collision = { false,false,false,false,false,false };

	if (posBlock.y + maxBlock.y >= pos->y + min.y &&
		posBlock.y + minBlock.y <= pos->y + max.y &&
		posBlock.z + maxBlock.z >= pos->z + min.z &&
		posBlock.z + minBlock.z <= pos->z + max.z)
	{ // X軸の判定に入れる場合

		if (posOldBlock.x + maxBlock.x <= posOld.x + min.x &&
			posBlock.x + maxBlock.x >= pos->x + min.x)
		{ // 右にぶつかった場合

			// 位置を設定する
			pos->x = posBlock.x + maxBlock.x - (min.x - COLLISION_ADD_DIFF_LENGTH);

			// 右の当たり判定を true にする
			collision.bRight = true;
		}
		else if (posOldBlock.x + minBlock.x >= posOld.x + max.x &&
			posBlock.x + minBlock.x <= pos->x + max.x)
		{ // 左にぶつかった場合

			// 位置を設定する
			pos->x = posBlock.x + minBlock.x - (max.x + COLLISION_ADD_DIFF_LENGTH);

			// 左の当たり判定を true にする
			collision.bLeft = true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos->x + min.x &&
		posBlock.x + minBlock.x <= pos->x + max.x &&
		posBlock.y + maxBlock.y >= pos->y + min.y &&
		posBlock.y + minBlock.y <= pos->y + max.y)
	{ // Z軸の判定に入れる場合

		if (posOldBlock.z + maxBlock.z <= posOld.z + min.z &&
			posBlock.z + maxBlock.z >= pos->z + min.z)
		{ // 奥にぶつかった場合

			// 位置を設定する
			pos->z = posBlock.z + maxBlock.z - (min.z - COLLISION_ADD_DIFF_LENGTH);

			// 奥の当たり判定を true にする
			collision.bFar = true;
		}
		else if (posOldBlock.z + minBlock.z >= posOld.z + max.z &&
			posBlock.z + minBlock.z <= pos->z + max.z)
		{ // 手前にぶつかった場合

			// 位置を設定する
			pos->z = posBlock.z + minBlock.z - (max.z + COLLISION_ADD_DIFF_LENGTH);

			// 手前の当たり判定を true にする
			collision.bNear = true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos->x + min.x &&
		posBlock.x + minBlock.x <= pos->x + max.x &&
		posBlock.z + maxBlock.z >= pos->z + min.z &&
		posBlock.z + minBlock.z <= pos->z + max.z)
	{ // Y軸の判定に入れる場合

		if (posOldBlock.y + maxBlock.y <= posOld.y + min.y &&
			posBlock.y + maxBlock.y >= pos->y + min.y)
		{ // 上にぶつかった場合

			// 位置を設定する
			pos->y = posBlock.y + maxBlock.y - (min.y - COLLISION_ADD_DIFF_LENGTH);

			// 上の当たり判定を true にする
			collision.bTop = true;
		}
		else if (posOldBlock.y + minBlock.y >= posOld.y + max.y &&
			posBlock.y + minBlock.y <= pos->y + max.y)
		{ // 下にぶつかった場合

			// 位置を設定する
			pos->y = posBlock.y + minBlock.y - (max.y + COLLISION_ADD_DIFF_LENGTH);

			// 下の当たり判定を true にする
			collision.bBottom = true;
		}
	}

	// 当たり判定の変数を返す
	return collision;
}