//===================================
//
// 当たり判定ヘッダー[collision.h]
// Author 小原立暉
//
//===================================
#ifndef _COLLISION_H_			//このマクロ定義がされていなかったら
#define _COLLISION_H_			//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CPlayer;			// プレイヤー
class CBullet;			// 弾
class CHierarchy;		// 階層モデル
class CShadowCircle;	// 丸影
class CObstacle;		// 障害物
class CEnemy;			// 敵
class CCrawlEnemy;		// 巡回敵
class CGoal;			// ゴール

// 名前空間(当たり判定)
namespace collision
{
	// プレイヤーの当たり判定
	bool PlayerBlockCollision(bool* pRight, bool* pLeft, CPlayer *player, const float fWidth, const float fHeight);		// プレイヤーとブロックの当たり判定
	bool PlayerObstacleCollision(CPlayer *player, const float fWidth, const float fHeight);								// プレイヤーと障害物の当たり判定
	void PlayerObstcleAttack(CPlayer *player);																			// プレイヤーの障害物に対する攻撃判定処理
	bool PlayerCollPolygonCollision(CPlayer *player, const float fWidth, const float fHeight);							// プレイヤーと当たり判定ポリゴンの当たり判定
	void PlayerItemCollision(CPlayer &player);			// プレイヤーとアイテムの当たり判定
	void PlayerBlockPenetration(CPlayer *player, const float fWidth, const float fHeight);			// プレイヤーとブロックのめり込み判定
	void PlayerObstaclePenetration(CPlayer *player, const float fWidth, const float fHeight);		// プレイヤーと障害物のめり込み判定
	void PlayerCollPolygonPenetration(CPlayer *player, const float fWidth, const float fHeight);	// プレイヤーと当たり判定ポリゴンのめり込み判定
	void PlayerCameraRangeCollision(CPlayer& player, const float fWidth, const float fHeight);		// プレイヤーとカメラの範囲の当たり判定処理
	void PlayerBalloonHit(CPlayer* pPlayer);			// プレイヤーと吹き出しの当たり判定

	// 弾の当たり判定
	bool BulletBlockCollision(CBullet bullet);			// 弾とブロックの当たり判定
	bool BulletPlayerCollision(CBullet bullet);			// 弾とプレイヤーの当たり判定
	bool BulletEnemyCollision(CBullet bullet);			// 弾と敵の当たり判定処理

	// 丸影の当たり判定
	void ShadowCollision(const D3DXVECTOR3& pos, int nIdx);		// 丸影の当たり判定

	// 障害物の当たり判定
	bool ObstacleBlockCollision(CObstacle *pTarget);	// 障害物とブロックの当たり判定(着地判定)
	bool ObstacleBlockCrush(CObstacle *pTarget);		// 障害物とブロックの当たり判定(ヒット判定)
	bool ObstacleMutualCollision(CObstacle* pTarget);	// 障害物同士の当たり判定
	bool ObstacleWoodBoxCollision(CObstacle* pTarget);	// 障害物と木箱の当たり判定

	// ゴールの当たり判定
	bool GoalPlayerCollision(CGoal* pGoal);				// ゴールとプレイヤーの当たり判定

	// 敵の当たり判定
	bool EnemyBlockCollision(CEnemy* pEnemy);			// 敵とブロックの当たり判定
}

#endif