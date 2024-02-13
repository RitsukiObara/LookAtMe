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
class CPlayer;		// プレイヤー
class CEnemy;		// 敵
class CBlock;		// ブロック

//-----------------------------------
// 名前空間(当たり判定)
//-----------------------------------
namespace collision
{
	// 丸影の当たり判定
	void ShadowCollision(const D3DXVECTOR3& pos, int nIdx);							// 丸影の当たり判定
	void ShadowElevCollision(D3DXVECTOR3& pos);										// 丸影と起伏地面の当たり判定
	void ShadowBlockCollision(D3DXVECTOR3& pos, const D3DXVECTOR3& posPlayer);		// 丸影とブロックの当たり判定
	void ShadowTableCollision(D3DXVECTOR3& pos, const D3DXVECTOR3& posPlayer);		// 丸影と台の当たり判定

	// 飛行機の当たり判定
	void AirplaneHit(CPlayer& player);			// プレイヤーと飛行機との当たり判定

	// アイテムの当たり判定
	void ScrewHit(CPlayer& player);				// プレイヤーとネジとの当たり判定

	// 敵の当たり判定
	void EnemyHit(CPlayer& player);				// プレイヤーと敵との当たり判定
	void EnemyPenetrate(CPlayer& player);		// プレイヤーと敵とのめりこみ判定
	//void EnemyToEnemy(CEnemy* pTarget);		// 敵同士の当たり判定処理
	void EnemyStun(CPlayer& player);			// 敵の気絶処理

	// テーブルの当たり判定
	bool TableCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth, const float fDepth);		// 台との当たり判定

	// マキナ草の当たり判定
	void MacchinaHit(CPlayer& player);			// プレイヤーとマキナ草との当たり判定

	// ゴールの当たり判定
	void GoalHit(CPlayer& player);				// プレイヤーとゴールとの当たり判定

	// 棘の当たり判定
	void NeedleHit(CPlayer& player);			// プレイヤーと棘の当たり判定

	// ブロックの当たり判定
	bool BlockCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth, const float fHeight, const bool bJump, CBlock** ppRide);		// ブロックとの当たり判定

	// 起伏地面の当たり判定
	bool ElevOutRangeCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth);			// 起伏地面の範囲外の当たり判定
}

#endif