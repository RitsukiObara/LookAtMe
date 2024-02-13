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

#include "shadowCircle.h"
#include "objectElevation.h"
#include "coin.h"
#include "game_score.h"
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
	const float DAGGER_RADIUS = 180.0f;				// ダガーの半径
	const int DAGGER_DAMAGE = 40;					// ダガーのダメージ
	const float DAGGER_KNOCKBACK = 100.0f;			// ダガーのノックバック
	const int EXPLOSION_DAMAGE = 30;				// 爆風のダメージ
	const float EXPLOSION_KNOCKBACK = 150.0f;		// 爆風のノックバック

	const int COIN_SCORE = 100;						// コインのスコア

	const float BOMB_BULLET_SMASH = 10.0f;			// 銃弾で爆弾の吹き飛ぶ速度
	const float BOMB_DAGGER_SMASH = 23.0f;			// ダガーで爆弾の吹き飛ぶ速度
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
void collision::CoinCollision(CPlayer* pPlayer, const D3DXVECTOR3 size)
{
	// ローカル変数宣言
	D3DXVECTOR3 posCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMaxCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMinCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 vtxMax = size;		// 最大値
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(size);	// 最小値
	CListManager<CCoin*> list = CCoin::GetList();
	CCoin* pCoin = nullptr;			// 先頭の小判
	CCoin* pCoinEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pCoin = list.GetTop();

		// 末尾の値を取得する
		pCoinEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// コインの変数を取得する
			posCoin = pCoin->GetPos();
			vtxMaxCoin = pCoin->GetFileData().vtxMax;
			vtxMinCoin = pCoin->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(posPlayer, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true &&
				useful::RectangleCollisionXZ(posPlayer, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true &&
				useful::RectangleCollisionYZ(posPlayer, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true)
			{ // 小判と重なった場合

				if (pCoin->GetState() == CCoin::STATE_NONE)
				{ // 無状態の場合

					// 取得処理
					pCoin->Hit();

					if (pPlayer->GetGameScore() != nullptr)
					{ // ゲームスコアが NULL じゃない場合

						// コイン分のスコアを加算する
						pPlayer->GetGameScore()->SetScore(pPlayer->GetGameScore()->GetScore() + COIN_SCORE);
					}
				}
			}

			if (pCoin == pCoinEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pCoin = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
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

			if (pos.y <= posEnemy.y + sizeEnemy.y &&
				pos.y + fHeight >= posEnemy.y &&
				useful::CircleCollisionXZ(pos, posEnemy, fRadius, (sizeEnemy.x + sizeEnemy.z) * 0.5f) == true)
			{ // 敵と重なった場合

				// ヒット処理
				pEnemy->Hit(DAGGER_DAMAGE, DAGGER_KNOCKBACK);

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

			if (posPlayer.y <= posEnemy.y + sizeEnemy.y &&
				posPlayer.y + fHeight >= posEnemy.y &&
				useful::CircleCollisionXZ(posPlayer, posEnemy, fRadius, (sizeEnemy.x + sizeEnemy.z) * 0.5f) == true)
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
bool collision::TreeCollision(D3DXVECTOR3* pos, const float fRadius)
{
	// ローカル変数宣言
	D3DXVECTOR3 posTree = NONE_D3DXVECTOR3;			// 木の位置
	float fObjectRadius;							// 半径
	bool bCollision = false;						// 当たり判定状況
	CListManager<CTree*> list = CTree::GetList();
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
void collision::TreeAttack(const CPlayer& pPlayer, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posTree = NONE_D3DXVECTOR3;			// 木の位置
	D3DXVECTOR3 posPlayer = pPlayer.GetPos();		// プレイヤーの位置
	CListManager<CTree*> list = CTree::GetList();
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

			if (useful::CircleCollisionXZ(posPlayer, posTree, DAGGER_RADIUS, TREE_RADIUS[pTree->GetType()]) &&
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
void collision::PalmFruitHit(CPlayer* pPlayer, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posFruit = NONE_D3DXVECTOR3;		// 木の位置
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

			posFruit = pFruit->GetPos();					// 木の位置
			fRadiusFruit = pFruit->GetFileData().vtxMax.x;	// 半径

			if (posPlayer.y <= posFruit.y + pFruit->GetFileData().vtxMax.y &&
				posPlayer.y + fHeight >= posFruit.y &&
				useful::CircleCollisionXZ(posPlayer, posFruit, DAGGER_RADIUS, fRadiusFruit) == true)
			{ // 円柱の当たり判定に入った場合

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
// 岩との当たり判定
//===============================
bool collision::RockCollision(D3DXVECTOR3* pos, const float fRadius, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posRock = NONE_D3DXVECTOR3;		// 岩の位置
	float fRadiusRock = 0.0f;					// 岩の半径
	float fTopRock = 0.0f;						// 岩の上の高さ
	float fBottomRock = 0.0f;					// 岩の下の高さ
	bool bCollision = false;					// 当たり判定状況

	CListManager<CRock*> list = CRock::GetList();
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

			// 岩の上の高さを取得する
			fTopRock = pRock->GetTopHeight();

			// 岩の下の高さを取得する
			fBottomRock = pRock->GetBottomHeight();

			if (pos->y <= posRock.y + fTopRock &&
				pos->y + fHeight >= posRock.y + fBottomRock)
			{ // 範囲内にいた場合

				// 円柱の当たり判定
				if (useful::CylinderCollision(pos, posRock, fRadiusRock + fRadius) == true)
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

				if (pos.y + fHeight >= posBomb.y &&
					pos.y <= posBomb.y + fHeightBomb &&
					useful::CircleCollisionXZ(pos, posBomb, fRadius, fRadiusBomb) == true)
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
bool collision::BombHitToDagger(const D3DXVECTOR3& pos, const float fHeight)
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

				if (pos.y + fHeight >= posBomb.y &&
					pos.y <= posBomb.y + fHeightBomb &&
					useful::CircleCollisionXZ(pos, posBomb, DAGGER_RADIUS, fRadiusBomb) == true)
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
// 爆風と敵との当たり判定
//===============================
void collision::ExplosionHitToRock(const D3DXVECTOR3& pos, const float fRadius, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posRock = NONE_D3DXVECTOR3;		// 岩の位置
	float fRadiusRock = 0.0f;					// 岩の半径
	float fTopRock = 0.0f;						// 岩の上の高さ
	float fBottomRock = 0.0f;					// 岩の下の高さ

	CListManager<CRock*> list = CRock::GetList();
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

				if (pos.y <= posRock.y + fTopRock &&
					pos.y + fHeight >= posRock.y + fBottomRock &&
					useful::CircleCollisionXZ(pos, posRock, fRadius, fRadiusRock) == true)
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

				if (pos.y <= posEnemy.y + fHeightEnemy &&
					pos.y + fHeight >= posEnemy.y &&
					useful::CircleCollisionXZ(pos, posEnemy, fRadius, fRadiusEnemy) == true)
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

			if (pos.y <= posExpl.y + fHeightExpl &&
				pos.y + fHeight >= posExpl.y - fHeightExpl &&
				useful::CircleCollisionXZ(pos, posExpl, fRadius, fRadiusExpl) == true)
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
bool collision::WallCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin)
{
	// ローカル変数宣言
	D3DXVECTOR3 posWall = NONE_D3DXVECTOR3;			// 壁の位置
	D3DXVECTOR3 posOldWall = NONE_D3DXVECTOR3;		// 壁の前回の位置
	D3DXVECTOR3 vtxMaxWall = NONE_D3DXVECTOR3;		// 壁の最大値
	D3DXVECTOR3 vtxMinWall = NONE_D3DXVECTOR3;		// 壁の最小値

	CListManager<CWall*> list = CWall::GetList();
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
bool collision::BlockHit(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin)
{
	// ローカル変数宣言
	D3DXVECTOR3 posBlock = NONE_D3DXVECTOR3;		// 壁の位置
	D3DXVECTOR3 posOldBlock = NONE_D3DXVECTOR3;		// 壁の前回の位置
	D3DXVECTOR3 vtxMaxBlock = NONE_D3DXVECTOR3;		// 壁の最大値
	D3DXVECTOR3 vtxMinBlock = NONE_D3DXVECTOR3;		// 壁の最小値

	CListManager<CBlock*> list = CBlock::GetList();
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
void collision::BossHit(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// ローカル変数宣言
	D3DXVECTOR3 posBoss = NONE_D3DXVECTOR3;
	D3DXVECTOR3 posPart = NONE_D3DXVECTOR3;

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

			// 変数を宣言
			D3DXMATRIX   mtxScale, mtxRot, mtxTrans, mtx;	// 計算用マトリックス
			D3DXMATRIX mtxWorld = pBoss->GetMatrix();		// マトリックスを取得する

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

			// ボスの位置を取得する
			posBoss = pBoss->GetPos();

			for (int nCnt = 0; nCnt < pBoss->GetNumModel(); nCnt++)
			{
				// マトリックスの計算処理
				pBoss->GetHierarchy(nCnt)->MatrixCalc(&mtx, mtxWorld);

				// 位置を設定する
				posPart.x = mtx._41;
				posPart.y = mtx._42;
				posPart.z = mtx._43;

				// 半径を算出する
				D3DXVECTOR3 Radius = (pBoss->GetHierarchy(nCnt)->GetFileData().vtxMax + pBoss->GetHierarchy(nCnt)->GetFileData().vtxMin) * 0.5f;
				float fRadius = (Radius.x + Radius.y + Radius.z) * 0.3f;

				// 位置を中心にする
				posPart += Radius;

				if (useful::CircleCollisionXY(pos, posPart, size.y, fRadius) == true &&
					useful::CircleCollisionYZ(pos, posPart, size.y, fRadius) == true &&
					useful::CircleCollisionXZ(pos, posPart, size.y, fRadius) == true)
				{ // 当たり判定に当たった場合

					// ヒット処理
					pBoss->Hit();

					// この先の処理を行わない
					return;
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