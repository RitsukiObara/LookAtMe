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
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "model.h"
#include "object.h"
#include "player.h"
#include "Item.h"
#include "block.h"
#include "obstacle.h"
#include "useful.h"
#include "bullet.h"
#include "enemy.h"
#include "input.h"
#include "shadowCircle.h"
#include "collpolygon.h"
#include "fraction.h"
#include "goal.h"
#include "ripple.h"
#include "goalUI.h"
#include "debugproc.h"
#include "streetlamp.h"
#include "Effect.h"
#include "Particle.h"
#include "motion.h"
#include "camera_range.h"
#include "score.h"
#include "aura.h"
#include "actionUI.h"
#include "balloon.h"
#include "balloonUI.h"
#include "staminaUI.h"
#include "sound.h"

//===============================
// マクロ定義
//===============================
#define COLLISION_ADD_ATTACK_LENGTH		(D3DXVECTOR3(40.0f, 40.0f, 40.0f))		// 攻撃の当たり判定の周辺の加算数
#define COLLISION_ADHE_ATTACK_LENGTH	(D3DXVECTOR3(0.01f, 0.01f, 0.01f))		// 攻撃の密着判定の周辺の加算数
#define COLLISION_ADD_DIFF_LENGTH		(0.01f)									// 僅かな誤差を埋めるためのマクロ定義(突っかかり防止)
#define COLLISION_CLIFF_ADD_HEIGHT		(120.0f)								// 崖掴まり判定で追加する高さ
#define COLLISION_ADD_BALLOON_LENGTH	(D3DXVECTOR3(30.0f, 30.0f, 0.0f))		// 吹き出しの追加の当たり判定

//===============================
// プレイヤーとブロックの当たり判定
//===============================
bool collision::PlayerBlockCollision(bool* pRight, bool* pLeft, CPlayer *player, const float fWidth, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player->GetPos();				// 位置
	D3DXVECTOR3 posOld = player->GetPosOld();		// 前回の位置
	D3DXVECTOR3 move = player->GetMove();			// 移動量

	bool bLand = false;				//着地したかどうか

	// ローカルポインタ宣言
	CObject *pObj = nullptr;		// オブジェクトへのポインタ
	CBlock *pBlock = nullptr;		// モデルへのポインタ

	// オブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // 種類がブロックの場合

			// モデルの情報を取得する
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // モデルが NULL じゃない場合

				if (player->GetJump() == false)
				{ // ジャンプしていなかった場合

					if (posOld.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
					{ // 上からの当たり判定

						// 位置を補正
						pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

						// 移動量を設定する
						move.y = 0.0f;

						// 着地判定を付与する
						bLand = true;

						// 下からの判定の設定処理
						player->SetCrushBottom(true);
					}
					else if (posOld.y + fHeight <= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
					{ // 下からの当たり判定

						// 位置を補正
						pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y - PLAYER_COLLISION_HEIGHT;

						// 移動量を設定する
						move.y = 0.0f;

						// 上からの判定の設定処理
						player->SetCrushTop(true);
					}
					else if (posOld.x + fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x&&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // 左からの当たり判定

						// 位置を補正
						pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;

						// 移動量を設定する
						move.x = 0.0f;

						// 右からの判定の設定処理
						player->SetCrushRight(true);
					}
					else if (posOld.x - fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // 右からの当たり判定

						// 位置を補正
						pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;

						// 移動量を設定する
						move.x = 0.0f;

						// 左からの判定の設定処理
						player->SetCrushLeft(true);
					}
				}							// ジャンプしていなかった時
				else if (player->GetJump() == true)
				{ // ジャンプしていた場合

					if (posOld.x + fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // 左からの当たり判定

						if (pos.y + fHeight <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y - COLLISION_CLIFF_ADD_HEIGHT &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFFALL &&
							pBlock->GetCliffLeft() == true)
						{ // 崖掴まり判定

							// 位置を設定する
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;
							pos.y = pBlock->GetPos().y + (pBlock->GetFileData().vtxMax.y - fHeight);

							// 崖つかまり状態にする
							player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFCATCH);

							// 崖捕まりモーションを設定する
							player->GetMotion()->Set(CPlayer::MOTIONTYPE_CLIFFCATCH);

							{ // エフェクト生成

								// エフェクトの位置
								D3DXVECTOR3 EffectPos(pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x, pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y, pBlock->GetPos().z);

								// 埃の設定処理
								CParticle::Create(EffectPos, CEffect::TYPE_CATCH);

								// 破片の設定処理
								CFraction::SetRCliffCatch(EffectPos, pBlock->GetFileData().m_nTexIdx[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}

							// 崖捕まり音を鳴らす
							CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFCATCH);
						}
						else if (pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH)
						{ // 角に激突

							// 位置を補正
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x;
							pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH * 2.0f;

							// 通常状態にする
							player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);
						}
						else if (player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB)
						{ // 上記以外(左壁についている場合)

							// 左壁についている
							*pLeft = true;

							// 位置を補正
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;
						}

						// 移動量を設定する
						move.x = 0.0f;

						// 右からの判定の設定処理
						player->SetCrushRight(true);
					}
					else if (posOld.x - fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // 右からの当たり判定

						if (pos.y + fHeight <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y - COLLISION_CLIFF_ADD_HEIGHT &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFFALL &&
							pBlock->GetCliffRight() == true)
						{ // 崖掴まり判定

							// 位置を設定する
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;
							pos.y = pBlock->GetPos().y + (pBlock->GetFileData().vtxMax.y - fHeight);

							// 崖つかまり状態にする
							player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFCATCH);

							// 崖捕まりモーションを設定する
							player->GetMotion()->Set(CPlayer::MOTIONTYPE_CLIFFCATCH);

							{ // エフェクト生成

								// エフェクトの位置
								D3DXVECTOR3 EffectPos(pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x, pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y, pBlock->GetPos().z);

								// 埃の設定処理
								CParticle::Create(EffectPos, CEffect::TYPE_CATCH);

								// 破片の設定処理
								CFraction::SetLCliffCatch(EffectPos, pBlock->GetFileData().m_nTexIdx[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}

							// 崖捕まり音を鳴らす
							CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFCATCH);
						}
						else if (pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH)
						{ // 角に激突

							// 位置を補正
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x;
							pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH * 2.0f;

							// 通常状態にする
							player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);
						}
						else if (player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB)
						{ // 上記以外(右壁についている場合)

							// 右壁についている
							*pRight = true;

							// 位置を補正
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;
						}

						// 移動量を設定する
						move.x = 0.0f;

						// 左からの判定の設定処理
						player->SetCrushLeft(true);
					}
					else if (posOld.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
					{ // 上からの当たり判定

						if (player->GetAction()->GetAct() == CPlayerAct::ACT_CLIFFCLIMB)
						{ // 崖離し状態の場合

							// 崖離し着地状態にする
							player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFLAND);

							// 移動量を設定する
							move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

							// 崖着地音を鳴らす
							CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFLAND);
						}
						else if(player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW)
						{ // 着地余韻状態以外

							// 通常状態にする
							player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);

							if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
							{ // 重力が一定以上の場合

								// 着地モーションを設定する
								player->GetMotion()->Set(CPlayer::MOTIONTYPE_LANDING);

								// 高いところからの着地音を鳴らす
								CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BIGLANDING);
							}
							else
							{ // 上記以外

								// 着地音を鳴らす
								CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
							}
						}

						// 位置を補正
						pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

						if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
						{ // 重力が一定以上の場合

							// 着地の波紋を生成する
							CRipple::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

							// 着地埃の生成
							CParticle::Create(pos, CEffect::TYPE_LANDDUST);

							if (CManager::GetCamera() != nullptr &&
								CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST)
							{ // カメラの情報があるかつ、ブースト状態以外の場合

								// 着地カメラに設定する
								CManager::GetCamera()->SetType(CCamera::TYPE_LAND);
							}
						}

						// 移動量を設定する
						move.y = 0.0f;

						// 着地判定を付与する
						bLand = true;

						// 下からの判定の設定処理
						player->SetCrushBottom(true);
					}
					else if (posOld.y + fHeight <= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
					{ // 下からの当たり判定

						// 位置を補正
						pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y - PLAYER_COLLISION_HEIGHT;

						// 移動量を設定する
						move.y = 0.0f;

						// 上からの判定の設定処理
						player->SetCrushTop(true);
					}
				}						// ジャンプしていた場合
			}							// モデルが NULL じゃない場合
		}								// 種類がブロックの場合

		// 次のオブジェクトを設定する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合

	//if (bCrushBottom == true && bCrushTop == true)
	//{//縦から押しつぶされている場合
	//	//圧死させる
	//	*pLife -= 1;
	//}
	
	//if (bCrushRight == true && bCrushLeft == true)
	//{//横から押しつぶされている場合
	//	//圧死させる
	//	*pLife -= 1;
	//}

	// 位置の設定処理
	player->SetPos(pos);
	player->SetMove(move);

	return bLand;			//着地の変数を返す
}

//===============================
// プレイヤーと障害物の当たり判定
//===============================
bool collision::PlayerObstacleCollision(CPlayer *player, const float fWidth, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player->GetPos();				// 位置
	D3DXVECTOR3 posOld = player->GetPosOld();		// 前回の位置
	D3DXVECTOR3 move = player->GetMove();			// 移動量

	bool bLand = false;				//着地したかどうか

	// ローカルポインタ宣言
	CObject *pObj = nullptr;			// オブジェクトへのポインタ
	CObstacle *pObstacle = nullptr;		// モデルへのポインタ
	CStreetLamp *pLamp = nullptr;		// 街灯(光源)へのポインタ

	// オブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // 種類が障害物の場合

			// モデルの情報を取得する
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // モデルが NULL じゃない場合

				if (pObstacle->GetType() != CObstacle::TYPE_POLE)
				{ // 街灯(支柱以外)

					if (pObstacle->GetCollision() == true)
					{ // 当たり判定状況が true の場合

						if (player->GetJump() == false)
						{ // ジャンプしていなかった場合

							if (posOld.y >= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMax.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
							{ // 上からの当たり判定

								// 位置を補正
								pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

								// 移動量を設定する
								move.y = 0.0f;

								// 着地判定を付与する
								bLand = true;

								// 下からの判定の設定処理
								player->SetCrushBottom(true);
							}
							else if (posOld.y + fHeight <= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
							{ // 下からの当たり判定

								// 位置を補正
								pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y - PLAYER_COLLISION_HEIGHT;

								// 移動量を設定する
								move.y = 0.0f;

								// 上からの判定の設定処理
								player->SetCrushTop(true);
							}
							else if (posOld.x + fWidth <= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
							{ // 左からの当たり判定

								// 位置を補正
								pos.x = pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;

								// 移動量を設定する
								move.x = 0.0f;

								// 右からの判定の設定処理
								player->SetCrushRight(true);
							}
							else if (posOld.x - fWidth >= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMax.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
							{ // 右からの当たり判定

								// 位置を補正
								pos.x = pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;

								// 移動量を設定する
								move.x = 0.0f;

								// 左からの判定の設定処理
								player->SetCrushLeft(true);
							}
						}							// ジャンプしていなかった時
						else if (player->GetJump() == true)
						{ // ジャンプしていた場合

							if (posOld.x + fWidth <= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
							{ // 左からの当たり判定

								if (pObstacle->GetType() == CObstacle::TYPE_LAMP &&
									player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
								{ // 種類が街灯(光源)の場合

									// 街灯(光源)にキャストする
									pLamp = dynamic_cast<CStreetLamp*>(pObstacle);

									if (pLamp != nullptr)
									{ // キャストに成功した場合

										// プレイヤーのキャッチ処理
										pLamp->PlayerCatch(&pos);
									}
								}
								else if (player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
								{ // 上記以外

									// 位置を補正
									pos.x = pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;
								}

								// 右からの判定の設定処理
								player->SetCrushRight(true);
							}
							else if (posOld.x - fWidth >= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMax.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
							{ // 右からの当たり判定

								if (pObstacle->GetType() == CObstacle::TYPE_LAMP &&
									player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
								{ // 種類が街灯(光源)かつ、ぶら下がり状態以外の場合

									// 街灯(光源)にキャストする
									pLamp = dynamic_cast<CStreetLamp*>(pObstacle);

									if (pLamp != nullptr)
									{ // キャストに成功した場合

										// プレイヤーのキャッチ処理
										pLamp->PlayerCatch(&pos);
									}
								}
								else if (player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
								{ // 上記以外

									// 位置を補正
									pos.x = pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;
								}

								// 左からの判定の設定処理
								player->SetCrushLeft(true);
							}
							else if (posOld.y >= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMax.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
							{ // 上からの当たり判定

								if (player->GetAction()->GetAct() == CPlayerAct::ACT_CLIFFCLIMB)
								{ // 崖離し状態の場合

									// 崖離し着地状態にする
									player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFLAND);

									// 移動量を設定する
									move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								}
								else if (player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW)
								{ // 着地余韻状態以外の場合

									// 通常状態にする
									player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);

									if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
									{ // 重力が一定以上の場合

										// 着地モーションを設定する
										player->GetMotion()->Set(CPlayer::MOTIONTYPE_LANDING);

										// 高いところからの着地音を鳴らす
										CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BIGLANDING);
									}
									else
									{ // 上記以外

										// 着地音を鳴らす
										CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
									}
								}

								// 位置を補正
								pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

								if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
								{ // 重力が一定以上の場合

									// 着地の波紋を生成する
									CRipple::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

									// 着地埃の生成
									CParticle::Create(pos, CEffect::TYPE_LANDDUST);

									if (CManager::GetCamera() != nullptr &&
										CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST)
									{ // カメラの情報があるかつ、ブースト状態以外の場合

										// 着地カメラに設定する
										CManager::GetCamera()->SetType(CCamera::TYPE_LAND);
									}
								}

								// 移動量を設定する
								move.y = 0.0f;

								// 着地判定を付与する
								bLand = true;

								// 下からの判定の設定処理
								player->SetCrushBottom(true);
							}
							else if (posOld.y + fHeight <= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
							{ // 下からの当たり判定

								// 位置を補正
								pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y - PLAYER_COLLISION_HEIGHT;

								// 移動量を設定する
								move.y = 0.0f;

								// 上からの判定の設定処理
								player->SetCrushTop(true);
							}
						}					// ジャンプしていた場合
					}						// 当たり判定状況が true の場合
				}
			}							// モデルが NULL じゃない場合
		}								// 種類がブロックの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合

	//if (bCrushBottom == true && bCrushTop == true)
	//{//縦から押しつぶされている場合
	//	//圧死させる
	//	*pLife -= 1;
	//}

	//if (bCrushRight == true && bCrushLeft == true)
	//{//横から押しつぶされている場合
	//	//圧死させる
	//	*pLife -= 1;
	//}

	// 位置の設定処理
	player->SetPos(pos);
	player->SetMove(move);

	return bLand;			//着地の変数を返す
}

//===============================
// プレイヤーの障害物に対する攻撃判定処理
//===============================
void collision::PlayerObstcleAttack(CPlayer *player)
{
	// ローカル変数宣言
	CObject *pObj = nullptr;			// オブジェクトへのポインタ
	CObstacle *pObstacle = nullptr;		// アイテムへのポインタ
	D3DXVECTOR3 VtxMax, VtxMin;			// 頂点の最大値,最小値
	D3DXVECTOR3 EffectPos;				// エフェクトの位置

	// 先頭のオブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // 種類がプレイヤーだった場合

			// モデルの情報を取得する
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // モデルが NULL じゃない場合

				if (pObstacle->GetAttack() == true &&
					pObstacle->GetType() != CObstacle::TYPE_WOODBOX &&
					pObstacle->GetType() != CObstacle::TYPE_LAMP)
				{ // アクション判定が true かつ、木箱以外かつ、街灯(光源)以外の場合

					if (pObstacle->GetType() == CObstacle::TYPE_DRUM)
					{ // ドラム缶の場合

						// 最大値・最小値を設定する
						VtxMax = D3DXVECTOR3(pObstacle->GetFileData().vtxMax.x, -pObstacle->GetFileData().vtxMin.z, 0.0f) + COLLISION_ADD_ATTACK_LENGTH;
						VtxMin = D3DXVECTOR3(pObstacle->GetFileData().vtxMin.x, -pObstacle->GetFileData().vtxMax.z, 0.0f) - COLLISION_ADD_ATTACK_LENGTH;
					}
					else
					{ // 上記以外

						// 最大値・最小値を設定する
						VtxMax = pObstacle->GetFileData().vtxMax + COLLISION_ADD_ATTACK_LENGTH;
						VtxMin = pObstacle->GetFileData().vtxMin - COLLISION_ADD_ATTACK_LENGTH;
					}

					// 矩形の当たり判定
					if (RectangleCollisionXY
					(
						player->GetPos(),
						pObstacle->GetPos(),
						D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f),
						VtxMax,
						D3DXVECTOR3(-PLAYER_WIDTH, 0.0f, 0.0f),
						VtxMin
					) == true)
					{ // 矩形の当たり判定に成功した場合

						if (player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
						{ // ぶら下がり状態以外

							if (pObstacle->GetType() == CObstacle::TYPE_POLE)
							{ // 街灯の支柱の場合

								if (player->GetJump() == true &&
									player->GetAction()->GetAct() != CPlayerAct::ACT_POLEDOWN)
								{ // ポール下り状態以外かつ、ジャンプしている場合

									// 攻撃範囲内に設定する
									pObstacle->SetRange(true);

									// エフェクトの位置を設定する
									EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
									EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
									EffectPos.z = 0.0f;

									// オーラを生成する
									CAura::Create(player->GetPos() + EffectPos, AURA_NORMAL_LIFE);
								}
								else
								{ // 上記以外

									// 攻撃範囲外に設定する
									pObstacle->SetRange(false);
								}
							}
							else
							{ // 上記以外

								// 攻撃範囲内に設定する
								pObstacle->SetRange(true);

								// エフェクトの位置を設定する
								EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
								EffectPos.y = (float)(rand() % (int)(PLAYER_HEIGHT));
								EffectPos.z = 0.0f;

								// オーラを生成する
								CAura::Create(player->GetPos() + EffectPos, AURA_NORMAL_LIFE);
							}
						}
						else
						{ // 上記以外

							// 攻撃範囲外に設定する
							pObstacle->SetRange(false);
						}

						if (pObstacle->GetType() == CObstacle::TYPE_DRUM)
						{ // ドラム缶の場合

							// 最大値・最小値を設定する
							VtxMax = D3DXVECTOR3(pObstacle->GetFileData().vtxMax.x, -pObstacle->GetFileData().vtxMin.z, 0.0f) + COLLISION_ADHE_ATTACK_LENGTH;
							VtxMin = D3DXVECTOR3(pObstacle->GetFileData().vtxMin.x, -pObstacle->GetFileData().vtxMax.z, 0.0f) - COLLISION_ADHE_ATTACK_LENGTH;
						}
						else
						{ // 上記以外

							// 最大値・最小値を設定する
							VtxMax = pObstacle->GetFileData().vtxMax + COLLISION_ADHE_ATTACK_LENGTH;
							VtxMin = pObstacle->GetFileData().vtxMin - COLLISION_ADHE_ATTACK_LENGTH;
						}

						if (RectangleCollisionXY
						(
							player->GetPos(),
							pObstacle->GetPos(),
							D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f),
							VtxMax,
							D3DXVECTOR3(-PLAYER_WIDTH, 0.0f, 0.0f),
							VtxMin
						) == true &&
							player->GetButton().nButtonCount >= PLAYER_ATTACK_BTNCOUNT &&
							player->GetButton().bSpacePush == true &&
							(player->GetAction()->GetAct() == CPlayerAct::ACT_NONE || player->GetAction()->GetAct() == CPlayerAct::ACT_BIGJUMP))
						{ // SPACEキーを一定時間押したかつ、通常状態か超ジャンプ状態の場合

							if (pObstacle->Hit() == true)
							{ // 当たり判定が true の場合

								// 攻撃のアクションUIを生成する
								CActionUI::Create(CActionUI::TYPE_ATTACK, PLAYER_OBSTACLE_SCORE);

								// ボタンの押し状況を false にする
								player->SetButtonPush(false);

								// ボタンカウントを初期化する
								player->SetButtonCount(0);

								if (pObstacle->GetType() != CObstacle::TYPE_POLE)
								{ // ポール以外の場合

									// 攻撃モーションを設定する
									player->GetMotion()->Set(CPlayer::MOTIONTYPE_ATTACK);
								}

								// 速度を上げる
								player->SetSpeed(player->GetSpeed() + PLAYER_ACTION_ADD_SPEED);
							}
						}
						else if (player->GetButton().nButtonCount >= PLAYER_ATTACK_BTNCOUNT &&
							player->GetButton().nButtonCount < PLAYER_BIGJUMP_BTNCOUNT &&
							player->GetButton().bSpacePush == true &&
							(player->GetAction()->GetAct() == CPlayerAct::ACT_NONE || player->GetAction()->GetAct() == CPlayerAct::ACT_BIGJUMP))
						{ // SPACEキーを一定時間押したかつ、通常状態か超ジャンプ状態の場合

							if (pObstacle->Hit() == true)
							{ // 当たり判定が true の場合

								// 攻撃のアクションUIを生成する
								CActionUI::Create(CActionUI::TYPE_ATTACK, PLAYER_OBSTACLE_SCORE);

								// ボタンの押し状況を false にする
								player->SetButtonPush(false);

								// ボタンカウントを初期化する
								player->SetButtonCount(0);

								if (pObstacle->GetType() != CObstacle::TYPE_POLE)
								{ // ポール以外の場合

									// 攻撃モーションを設定する
									player->GetMotion()->Set(CPlayer::MOTIONTYPE_ATTACK);
								}

								// 速度を上げる
								player->SetSpeed(player->GetSpeed() + PLAYER_ACTION_ADD_SPEED);
							}
						}
					}
					else
					{ // 当たり判定に失敗した場合

						// 攻撃範囲外に設定する
						pObstacle->SetRange(false);
					}
				}
				else
				{ // 上記以外

					// 攻撃範囲外に設定する
					pObstacle->SetRange(false);
				}
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}
}

//===============================
// プレイヤーと当たり判定ポリゴンの当たり判定
//===============================
bool collision::PlayerCollPolygonCollision(CPlayer *player, const float fWidth, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player->GetPos();				// 位置
	D3DXVECTOR3 posOld = player->GetPosOld();		// 前回の位置
	D3DXVECTOR3 move = player->GetMove();			// 移動量

	bool bLand = false;				//着地したかどうか

	// ローカルポインタ宣言
	CObject *pObj = nullptr;				// オブジェクトへのポインタ
	CCollPolygon *pColl = nullptr;		// モデルへのポインタ

	// オブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BG);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_COLLPOLY)
		{ // 種類が当たり判定ポリゴンの場合

			// モデルの情報を取得する
			pColl = dynamic_cast<CCollPolygon*>(pObj);

			if (pColl != nullptr)
			{ // モデルが NULL じゃない場合

				if (pColl->GetPos().z == 0.0f)
				{ // Z軸が 0.0f だった場合

					if (player->GetJump() == false && 
						player->GetAction()->GetAct() != CPlayerAct::ACT_POLEDOWN &&
						player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING &&
						player->GetAction()->GetAct() != CPlayerAct::ACT_LAMPJUMP)
					{ // ジャンプしていないかつ、一定の状態以外の場合

						if (posOld.y >= pColl->GetPos().y + pColl->GetVtxMax().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x)
						{ // 上からの当たり判定

							// 位置を補正
							pos.y = pColl->GetPos().y + pColl->GetVtxMax().y + COLLISION_ADD_DIFF_LENGTH;

							// 移動量を設定する
							move.y = 0.0f;

							// 着地判定を付与する
							bLand = true;

							// 下からの判定の設定処理
							player->SetCrushBottom(true);
						}
						else if (posOld.y + fHeight <= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x)
						{ // 下からの当たり判定

							// 位置を補正
							pos.y = pColl->GetPos().y + pColl->GetVtxMin().y - PLAYER_COLLISION_HEIGHT;

							// 移動量を設定する
							move.y = 0.0f;

							// 上からの判定の設定処理
							player->SetCrushTop(true);
						}
						else if (posOld.x + fWidth <= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y)
						{ // 左からの当たり判定

							// 位置を補正
							pos.x = pColl->GetPos().x + pColl->GetVtxMin().x - PLAYER_COLLISION_WIDTH;

							// 右からの判定の設定処理
							player->SetCrushRight(true);
						}
						else if (posOld.x - fWidth >= pColl->GetPos().x + pColl->GetVtxMax().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y)
						{ // 右からの当たり判定

							// 位置を補正
							pos.x = pColl->GetPos().x + pColl->GetVtxMax().x + PLAYER_COLLISION_WIDTH;

							// 左からの判定の設定処理
							player->SetCrushLeft(true);
						}
					}							// ジャンプしていなかった時
					else if (player->GetJump() == true)
					{ // ジャンプしていた場合

						if (posOld.x + fWidth <= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y)
						{ // 左からの当たり判定

							// 位置を補正
							pos.x = pColl->GetPos().x + pColl->GetVtxMin().x - PLAYER_COLLISION_WIDTH;

							// 右からの判定の設定処理
							player->SetCrushRight(true);
						}
						else if (posOld.x - fWidth >= pColl->GetPos().x + pColl->GetVtxMax().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y)
						{ // 右からの当たり判定

							// 位置を補正
							pos.x = pColl->GetPos().x + pColl->GetVtxMax().x + PLAYER_COLLISION_WIDTH;

							// 左からの判定の設定処理
							player->SetCrushLeft(true);
						}
						else if (posOld.y >= pColl->GetPos().y + pColl->GetVtxMax().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x)
						{ // 上からの当たり判定

							if (player->GetAction()->GetAct() == CPlayerAct::ACT_CLIFFCLIMB)
							{ // 崖離し状態の場合

								// 崖離し着地状態にする
								player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFLAND);

								// 移動量を設定する
								move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
							else if (player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW)
							{ // 着地余韻状態以外の場合

								// 通常状態にする
								player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);

								if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
								{ // 重力が一定以上の場合

									// 着地モーションを設定する
									player->GetMotion()->Set(CPlayer::MOTIONTYPE_LANDING);

									// 高いところからの着地音を鳴らす
									CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BIGLANDING);
								}
								else
								{ // 上記以外

									// 着地音を鳴らす
									CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
								}
							}

							// 位置を補正
							pos.y = pColl->GetPos().y + pColl->GetVtxMax().y;

							if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
							{ // 重力が一定以上の場合

								// 着地の波紋を生成する
								CRipple::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

								// 着地埃の生成
								CParticle::Create(pos, CEffect::TYPE_LANDDUST);

								if (CManager::GetCamera() != nullptr &&
									CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST)
								{ // カメラの情報があるかつ、ブースト状態以外の場合

									// 着地カメラに設定する
									CManager::GetCamera()->SetType(CCamera::TYPE_LAND);
								}
							}

							// 移動量を設定する
							move.y = 0.0f;

							// 着地判定を付与する
							bLand = true;

							// 下からの判定の設定処理
							player->SetCrushBottom(true);
						}
						else if (posOld.y + fHeight <= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x)
						{ // 下からの当たり判定

							// 位置を補正
							pos.y = pColl->GetPos().y + pColl->GetVtxMin().y - PLAYER_COLLISION_HEIGHT;

							// 移動量を設定する
							move.y = 0.0f;

							// 上からの判定の設定処理
							player->SetCrushTop(true);
						}
					}					// ジャンプしていた場合
				}						// Z軸が0.0fだった場合
			}							// モデルが NULL じゃない場合
		}								// 種類が当たり判定ポリゴンの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合

	//if (bCrushBottom == true && bCrushTop == true)
	//{//縦から押しつぶされている場合
	//	//圧死させる
	//	*pLife -= 1;
	//}
	
	//if (bCrushRight == true && bCrushLeft == true)
	//{//横から押しつぶされている場合
	//	//圧死させる
	//	*pLife -= 1;
	//}
	
	// 位置の設定処理
	player->SetPos(pos);
	player->SetMove(move);

	return bLand;			//着地の変数を返す
}

//===============================
// アイテムの当たり判定
//===============================
void collision::PlayerItemCollision(CPlayer &player)
{
	// ローカルポインタ宣言
	CObject *pObj = nullptr;		// オブジェクトへのポインタ
	CItem *pItem = nullptr;		// アイテムへのポインタ

	// オブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_ITEM)
		{ // 種類がプレイヤーだった場合

			// モデルの情報を取得する
			pItem = dynamic_cast<CItem*>(pObj);

			if (pItem != nullptr)
			{ // モデルが NULL じゃない場合

				// 円の当たり判定
				if (CircleCollisionXY(player.GetPos(), pItem->GetPos(), PLAYER_WIDTH, pItem->GetFileData().fRadius) == true)
				{ // アイテムに触れた場合

					// アイテムの終了処理
					pItem->Hit();

					if (CManager::GetMode() == CScene::MODE_GAME &&
						player.GetStaminaUI() != nullptr)
					{ // ゲームモードかつ、スタミナUIのポインタが NULL じゃない場合

						// 回復状態にする
						player.GetStaminaUI()->SetState(CStaminaUI::STATE_HEAL);
					}

					// この先の処理を行わない
					return;
				}
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}
}

//===============================
// プレイヤーとブロックのめり込み判定
//===============================
void collision::PlayerBlockPenetration(CPlayer *player, const float fWidth, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player->GetPos();				// 位置

	// ローカルポインタ宣言
	CObject *pObj = nullptr;		// オブジェクトへのポインタ
	CBlock *pBlock = nullptr;		// モデルへのポインタ

	// オブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // 種類がブロックの場合

			// モデルの情報を取得する
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // モデルが NULL じゃない場合

				if (pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 左がつぶれている場合

					// 左の当たり判定を true にする
					player->SetCrushLeft(true);
				}
				else if (pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 右がつぶれている場合

					// 右の当たり判定を true にする
					player->SetCrushRight(true);
				}
			}							// モデルが NULL じゃない場合
		}								// 種類がブロックの場合

		// 次のオブジェクトを設定する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合
}

//===============================
// プレイヤーと障害物のめり込み判定
//===============================
void collision::PlayerObstaclePenetration(CPlayer *player, const float fWidth, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player->GetPos();				// 位置

	// ローカルポインタ宣言
	CObject *pObj = nullptr;			// オブジェクトへのポインタ
	CObstacle *pObstacle = nullptr;		// モデルへのポインタ

	// オブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // 種類が障害物の場合

			// モデルの情報を取得する
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // モデルが NULL じゃない場合

				if (pObstacle->GetType() != CObstacle::TYPE_LAMP &&
					pObstacle->GetType() != CObstacle::TYPE_POLE)
				{ // 街灯以外の場合

					if (pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
						pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
						pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
					{ // 左がつぶれている場合

						// 左の当たり判定を true にする
						player->SetCrushLeft(true);
					}
					else if (pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
					{ // 右がつぶれている場合

						// 右の当たり判定を true にする
						player->SetCrushRight(true);
					}
				}						// 街灯以外の場合
			}							// モデルが NULL じゃない場合
		}								// 種類がブロックの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合
}

//===============================
// プレイヤーと当たり判定ポリゴンのめり込み判定
//===============================
void collision::PlayerCollPolygonPenetration(CPlayer *player, const float fWidth, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player->GetPos();				// 位置

	// ローカルポインタ宣言
	CObject *pObj = nullptr;				// オブジェクトへのポインタ
	CCollPolygon *pColl = nullptr;		// モデルへのポインタ

	// オブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BG);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_COLLPOLY)
		{ // 種類が当たり判定ポリゴンの場合

			// モデルの情報を取得する
			pColl = dynamic_cast<CCollPolygon*>(pObj);

			if (pColl != nullptr)
			{ // モデルが NULL じゃない場合

				if (pos.x - fWidth <= pColl->GetPos().x + pColl->GetSize().x &&
					pos.x + fWidth >= pColl->GetPos().x + pColl->GetSize().x &&
					pos.y + fHeight >= pColl->GetPos().y - pColl->GetSize().y &&
					pos.y <= pColl->GetPos().y + pColl->GetSize().y)
				{ // 左がつぶれている場合

					// 左の当たり判定を true にする
					player->SetCrushLeft(true);
				}
				else if (pos.x + fWidth >= pColl->GetPos().x - pColl->GetSize().x &&
					pos.x - fWidth <= pColl->GetPos().x - pColl->GetSize().x &&
					pos.y + fHeight >= pColl->GetPos().y - pColl->GetSize().y &&
					pos.y <= pColl->GetPos().y + pColl->GetSize().y)
				{ // 右がつぶれている場合

					// 右の当たり判定を true にする
					player->SetCrushRight(true);
				}
			}							// モデルが NULL じゃない場合
		}								// 種類が当たり判定ポリゴンの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合
}

//===============================
// プレイヤーとカメラの範囲の当たり判定処理
//===============================
void collision::PlayerCameraRangeCollision(CPlayer& player, const float fWidth, const float fHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 VtxMax, VtxMin, PlayerPos;	// プレイヤーの最大値、プレイヤーの最小値、プレイヤーの位置
	CObject* pObj;							// オブジェクトのポインタ
	CCameraRange* pCameRange;				// カメラの範囲のポインタ

	if (CManager::GetMode() == CScene::MODE_GAME &&
		CGame::GetState() != CGame::STATE_START)
	{ // ゲームモードかつ、開始状態以外の場合

		// プレイヤーの位置を設定する
		PlayerPos = D3DXVECTOR3(player.GetPos().x, player.GetPos().y + (fHeight * 0.5f), player.GetPos().z);

		// プレイヤーの最大値を設定する
		VtxMax = D3DXVECTOR3(fWidth, fHeight * 0.5f, 0.0f);

		// プレイヤーの最小値を設定する
		VtxMin = D3DXVECTOR3(-fWidth, -(fHeight * 0.5f), 0.0f);

		// オブジェクトを取得
		pObj = CObject::GetTop(CObject::PRIORITY_BG);

		while (pObj != nullptr)
		{ // オブジェクトが NULL じゃない限り回す

			if (pObj->GetType() == CObject::TYPE_CAMERARANGE)
			{ // 種類がカメラの範囲の場合

				// モデルの情報を取得する
				pCameRange = dynamic_cast<CCameraRange*>(pObj);

				if (pCameRange != nullptr)
				{ // ポインタが NULL じゃない場合

					// 矩形の当たり判定
					if (RectangleCollisionXY
					(
						PlayerPos,						// プレイヤーの位置
						pCameRange->GetPos(),			// カメラの範囲の位置
						VtxMax,							// プレイヤーの最大値
						pCameRange->GetSize(),			// カメラの範囲の最大値
						VtxMin,							// プレイヤーの最小値
						-pCameRange->GetSize()			// カメラの範囲の最小値
					) == true)
					{ // 当たり判定に入った場合

						if (pCameRange->GetType() == CCamera::TYPE_PRECEDENCE &&
							CManager::GetCamera()->GetType() == CCamera::TYPE_PRECEDENCE)
						{ // 既に先行投影カメラになっていた場合

							// この処理を抜ける
							return;
						}

						// カメラの種類を設定する
						CManager::GetCamera()->SetType(pCameRange->GetType());

						// この先の処理を行わない
						return;
					}
				}							// ポインタが NULL じゃない場合
			}								// 種類がカメラの範囲の場合

			// 次のオブジェクトを代入する
			pObj = pObj->GetNext();
		}									// オブジェクトが NULL じゃない限り回す

		if (CManager::GetCamera() != nullptr)
		{ // カメラがある場合

			if (CManager::GetCamera()->GetType() != CCamera::TYPE_LAND &&
				CManager::GetCamera()->GetType() != CCamera::TYPE_WALLKICK &&
				CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST &&
				CManager::GetCamera()->GetType() != CCamera::TYPE_PRECEDENCE)
			{ // 一定の種類以外の場合

				// カメラの種類を設定する
				CManager::GetCamera()->SetType(CCamera::TYPE_NONE);
			}
		}
	}
}

//===============================
// プレイヤーと吹き出しの当たり判定
//===============================
void collision::PlayerBalloonHit(CPlayer* pPlayer)
{
	// ローカル変数宣言
	CObject* pObj;			// オブジェクトのポインタ
	CBalloon* pBalloon;		// 吹き出しのポインタ
	D3DXVECTOR3 pos;		// プレイヤーの位置
	D3DXVECTOR3 VtxMax;		// プレイヤーの最大値
	D3DXVECTOR3 VtxMin;		// プレイヤーの最小値

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{ // チュートリアルの場合

		// 情報を設定する
		pos = D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + (PLAYER_HEIGHT * 0.5f), 0.0f);		// 位置
		VtxMax = D3DXVECTOR3(PLAYER_WIDTH, (PLAYER_HEIGHT * 0.5f), 0.0f);								// プレイヤーの最大値
		VtxMin = D3DXVECTOR3(-PLAYER_WIDTH, -(PLAYER_HEIGHT * 0.5f), 0.0f);								// プレイヤーに最小値

		// 先頭のオブジェクトを取得する
		pObj = CObject::GetTop(CObject::PRIORITY_SHADOW);

		while (pObj != nullptr)
		{ // オブジェクトのポインタが NULL じゃない場合

			if (pObj->GetType() == CObject::TYPE_BALLOON)
			{ // 種類が吹き出しだった場合

				// 吹き出しにダイナミックキャストする
				pBalloon = dynamic_cast<CBalloon*>(pObj);

				if (pBalloon != nullptr)
				{ // 吹き出しのポインタが NULL じゃなかった場合

					if (RectangleCollisionXY
					(
						pos,
						pBalloon->GetPos(),
						VtxMax,
						pBalloon->GetSize() + COLLISION_ADD_BALLOON_LENGTH,
						VtxMin,
						-pBalloon->GetSize() - COLLISION_ADD_BALLOON_LENGTH
					) == true)
					{// 吹き出しに当たった場合

						// 表示状態に設定する
						pBalloon->SetState(CBalloon::STATE_DISP);

						if (CTutorial::GetBalloonUI() != nullptr)
						{ // ふきだし2Dが NULL じゃない場合

							// ふきだし2Dの情報を設定する
							CTutorial::GetBalloonUI()->SetType(pBalloon->GetType());
						}

						if (pBalloon->GetType() == CBalloon::TYPE_SPEED)
						{ // 吹き出しが速度関係だった場合

							// 速度UI表示状態を設定する
							CTutorial::SetState(CTutorial::STATE_SPEED);
						}
						else if (pBalloon->GetType() == CBalloon::TYPE_BOOST)
						{ // 吹き出しがブーストの場合

							// ブースト状態を設定する
							CTutorial::SetState(CTutorial::STATE_BOOST);
						}
						else if (pBalloon->GetType() == CBalloon::TYPE_POLEJUMP)
						{ // 吹き出しが街灯ジャンプの場合

							// 街灯ジャンプ状態を設定する
							CTutorial::SetState(CTutorial::STATE_POLE);
						}
						else if (pBalloon->GetType() == CBalloon::TYPE_STAMINA)
						{ // ふきだしがスタミナの場合

							// スタミナ状態を設定する
							CTutorial::SetState(CTutorial::STATE_STAMINA);
						}
						else
						{ // 吹き出しが上記以外の場合

							// 状態を設定する
							CTutorial::SetState(CTutorial::STATE_NONE);
						}
					}
					else
					{ // 上記以外

						// 収縮状態に設定する
						pBalloon->SetState(CBalloon::STATE_SMALL);
					}
				}
			}
			
			// 次のオブジェクトを取得する
			pObj = pObj->GetNext();
		}
	}
}

//===============================
// 弾とブロックの当たり判定
//===============================
bool collision::BulletBlockCollision(CBullet bullet)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = bullet.GetPos();			// 位置
	D3DXVECTOR3 posOld = bullet.GetPosOld();	// 前回の位置
	D3DXVECTOR3 size = bullet.GetSize();		// サイズ

		// ローカルポインタ宣言
	CObject *pObj = nullptr;		// オブジェクトへのポインタ
	CBlock *pBlock = nullptr;	// オブジェクト2Dへのポインタ

	// 先頭のオブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // 種類がブロックの場合

			// オブジェクト2Dの情報を取得する
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // オブジェクト2Dが NULL の場合

				if (posOld.y - size.y >= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMax.y &&
					pos.y - size.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
					pos.x - size.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.x + size.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x)
				{ // 前回の位置がブロックより上かつ、現在の位置がブロックよりも下かつ、ブロックの幅の中だった場合(上に立つ処理)

					// true を返す
					return true;
				}							//上端の処理
				else if (posOld.y + size.y <= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMin.y &&
					pos.y + size.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.x - size.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.x + size.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x)
				{ // 前回の位置がブロックより下かつ、現在の位置がブロックよりも上かつ、ブロックの幅の中だった場合(頭をぶつける処理)

					// true を返す
					return true;
				}							//下端の処理
				else if (posOld.x + size.x <= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + size.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.y + size.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y - size.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 前回の位置がブロックより左かつ、現在の位置がブロックより右かつ、ブロックの高さの中だった場合(左の処理)

					// true を返す
					return true;
				}							//左端の処理
				else if (posOld.x - size.x >= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMax.x &&
					pos.x - size.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.y + size.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y - size.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 前回の位置がブロックより右かつ、現在の位置がブロックより右かつ、ブロックの高さの中だった場合(右の処理)

					// true を返す
					return true;
				}						//右端の処理
			}
		}								//種類がブロックの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}

	// false を返す
	return false;
}

//===============================
// 弾とプレイヤーの当たり判定
//===============================
bool collision::BulletPlayerCollision(CBullet bullet)
{
	// ローカル変数を宣言
	D3DXVECTOR3 pos = bullet.GetPos();		// 位置
	D3DXVECTOR3 size = bullet.GetSize();	// サイズ

		// ローカルポインタ宣言
	CObject *pObj = nullptr;		// オブジェクトへのポインタ
	CPlayer *pPlayer = nullptr;	// オブジェクト2Dへのポインタ

	// オブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_PLAYER);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_PLAYER)
		{ // 種類がプレイヤーだった場合

			// オブジェクト2Dの情報を取得する
			pPlayer = dynamic_cast<CPlayer*>(pObj);

			if (pPlayer != nullptr)
			{ // オブジェクト2Dが NULL の場合

				if (pos.x + size.x >= pPlayer->GetPos().x - PLAYER_WIDTH &&
					pos.x - size.x <= pPlayer->GetPos().x + PLAYER_WIDTH &&
					pos.y + size.y >= pPlayer->GetPos().y &&
					pos.y - size.y <= pPlayer->GetPos().y + PLAYER_HEIGHT)
				{ // 弾がプレイヤーの当たり判定に当たった

					// プレイヤーのヒット処理
					pPlayer->Hit();

					// true を返す
					return true;
				}
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}

	// false を返す
	return false;
}

//===============================
// 弾と敵の当たり判定処理
//===============================
bool collision::BulletEnemyCollision(CBullet bullet)
{
	// ローカル変数を宣言
	D3DXVECTOR3 pos = bullet.GetPos();		// 位置
	D3DXVECTOR3 size = bullet.GetSize();	// サイズ

		// ローカルポインタ宣言
	CObject *pObj = nullptr;		// オブジェクトへのポインタ
	CEnemy *pEnemy = nullptr;	// オブジェクト2Dへのポインタ

	// 先頭のオブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_ENEMY)
		{ // 種類がプレイヤーだった場合

			// オブジェクト2Dの情報を取得する
			pEnemy = dynamic_cast<CEnemy*>(pObj);

			if (pEnemy != nullptr)
			{ // オブジェクト2Dが NULL の場合

				if (pos.x + size.x >= pEnemy->GetPos().x - ENEMY_WIDTH &&
					pos.x - size.x <= pEnemy->GetPos().x + ENEMY_WIDTH &&
					pos.y + size.y >= pEnemy->GetPos().y &&
					pos.y - size.y <= pEnemy->GetPos().y + ENEMY_HEIGHT)
				{ // 弾がプレイヤーの当たり判定に当たった

					// プレイヤーの終了処理
					pEnemy->Hit();

					// true を返す
					return true;
				}
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}

	// false を返す
	return false;
}

//===============================
// 影の当たり判定処理
//===============================
void collision::ShadowCollision(const D3DXVECTOR3& pos, int nIdx)
{
	// ローカル変数宣言
	CShadowCircle* pShadow = CShadowCircle::GetShadow(nIdx);	// 影のポインタ
	CObject* pObj = nullptr;			// オブジェクトのポインタ
	CBlock* pBlock = nullptr;			// ブロックのポインタ
	D3DXVECTOR3 posCandi = pos;			// 候補の影の位置
	float posComp = 0.0f;				// 比較用の影の位置
	bool bCollision = false;			// 当たり判定状況

	if (pShadow != nullptr)
	{ // 影のポインタが NULL じゃなかった場合

		// オブジェクトのポインタを取得する
		pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // オブジェクトのが NULL じゃない限り回す

			if (pObj->GetType() == CObject::TYPE_BLOCK)
			{ // 種類がブロックの場合

				// ブロックの情報を取得する
				pBlock = dynamic_cast<CBlock*>(pObj);

				if (pBlock != nullptr)
				{ // ブロックのポインタが NULL じゃない場合

					if (pShadow->GetPos().x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pShadow->GetPos().x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // ブロックの範囲内に入っているかつ、対象より下にブロックがある場合

						if (bCollision == false)
						{ // 当たり判定が false の場合

							// 当たり判定を true にする
							bCollision = true;

							// 候補の変数に代入する
							posCandi.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y;
						}
						else
						{ // 上記以外

							// 比較用の変数に代入する
							posComp = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y;

							if (posCandi.y <= posComp)
							{ // 比較した位置の方が高かった場合

							  // 候補の位置を設定する
								posCandi.y = posComp;
							}
						}
					}
				}
			}

			// 次のオブジェクトを代入する
			pObj = pObj->GetNext();
		}

		if (bCollision == false)
		{ // 当たり判定が false の場合

			// 位置を設定する
			posCandi.y = 0.0f;
		}

		// 位置を設定する
		pShadow->SetPos(posCandi);
	}
}

//===============================
// 障害物とブロックの当たり判定(着地判定)
//===============================
bool collision::ObstacleBlockCollision(CObstacle *pTarget)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = pTarget->GetPos();			// 位置
	D3DXVECTOR3 posOld = pTarget->GetPosOld();		// 前回の位置

	// ローカルポインタ宣言
	CObject *pObj = nullptr;		// オブジェクトへのポインタ
	CBlock *pBlock = nullptr;		// モデルへのポインタ

	bool bLand = false;				//着地したかどうか

	// 先頭のオブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // 種類がブロックの場合

			// モデルの情報を取得する
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // モデルが NULL じゃない場合

				if (posOld.y + pTarget->GetFileData().vtxMin.y >= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMax.y &&
					pos.y + pTarget->GetFileData().vtxMin.y < pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
					pos.x + pTarget->GetFileData().vtxMax.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + pTarget->GetFileData().vtxMin.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // 上からの当たり判定

					// 位置を補正
					pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y - pTarget->GetFileData().vtxMin.y;

					// 着地判定を付与する
					bLand = true;
				}
			}							// モデルが NULL じゃない場合
		}								// 種類がブロックの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合

	// 位置の設定処理
	pTarget->SetPos(pos);

	return bLand;			//着地の変数を返す
}

//===============================
// 障害物とブロックの当たり判定(ヒット判定)
//===============================
bool collision::ObstacleBlockCrush(CObstacle *pTarget)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = pTarget->GetPos();			// 位置
	D3DXVECTOR3 posOld = pTarget->GetPosOld();		// 前回の位置

	// ローカルポインタ宣言
	CObject *pObj = nullptr;		// オブジェクトへのポインタ
	CBlock *pBlock = nullptr;		// モデルへのポインタ

	bool bHit = false;			// 当たったかどうか

	// 先頭のオブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // 種類がブロックの場合

			// モデルの情報を取得する
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // モデルが NULL じゃない場合

				if (posOld.y + pTarget->GetFileData().vtxMax.y <= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMin.y &&
					pos.y + pTarget->GetFileData().vtxMax.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.x + pTarget->GetFileData().vtxMax.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + pTarget->GetFileData().vtxMin.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // 下からの当たり判定

					// 位置を補正
					pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y - pTarget->GetFileData().vtxMax.y;

					// 当たり判定を付与する
					bHit = true;
				}
				else if (posOld.x + pTarget->GetFileData().vtxMax.x <= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + pTarget->GetFileData().vtxMax.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.y + pTarget->GetFileData().vtxMax.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y + pTarget->GetFileData().vtxMin.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 左からの当たり判定

					// 位置を補正
					pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - pTarget->GetFileData().vtxMax.x;

					// 当たり判定を付与する
					bHit = true;
				}
				else if (posOld.x + pTarget->GetFileData().vtxMin.x >= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMax.x &&
					pos.x + pTarget->GetFileData().vtxMin.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.y + pTarget->GetFileData().vtxMax.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y + pTarget->GetFileData().vtxMin.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 右からの当たり判定

					// 位置を補正
					pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + pTarget->GetFileData().vtxMin.x;

					// 当たり判定を付与する
					bHit = true;
				}
			}							// モデルが NULL じゃない場合
		}								// 種類がブロックの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合

	// 位置の設定処理
	pTarget->SetPos(pos);

	// 当たり判定を返す
	return bHit;
}

//===============================
// 障害物同士の当たり判定
//===============================
bool collision::ObstacleMutualCollision(CObstacle* pTarget)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = pTarget->GetPos();			// 位置
	D3DXVECTOR3 posOld = pTarget->GetPosOld();		// 前回の位置
	bool bLand = false;								// 上に乗ったかどうか

	// ローカルポインタ宣言
	CObject *pObj = nullptr;				// オブジェクトへのポインタ
	CObstacle *pObstacle = nullptr;		// モデルへのポインタ

	// 先頭のオブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // 種類が障害物の場合

			// モデルの情報を取得する
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // モデルが NULL じゃない場合

				if (pObstacle != pTarget &&
					pObstacle->GetCollision() == true)
				{ // 対象自身じゃないかつ、当たり判定が true の場合

					if (posOld.y + pTarget->GetFileData().vtxMin.y >= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMax.y &&
						pos.y + pTarget->GetFileData().vtxMin.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y &&
						pos.x + pTarget->GetFileData().vtxMax.x >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x + pTarget->GetFileData().vtxMin.x <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
					{ // 上からの当たり判定

						// 位置を補正
						pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y + pTarget->GetFileData().vtxMin.y;

						// 着地判定を付与する
						bLand = true;
					}
				}						// 対象自身じゃないかつ、当たり判定が true の場合
			}							// モデルが NULL じゃない場合
		}								// 種類がブロックの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合

	// 位置の設定処理
	pTarget->SetPos(pos);

	// 着地判定を返す
	return bLand;
}

//===============================
// 障害物と木箱の当たり判定
//===============================
bool collision::ObstacleWoodBoxCollision(CObstacle* pTarget)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = pTarget->GetPos();			// 位置
	D3DXVECTOR3 posOld = pTarget->GetPosOld();		// 前回の位置
	bool bHit = false;			// 当たったかどうか

	// ローカルポインタ宣言
	CObject *pObj = nullptr;				// オブジェクトへのポインタ
	CObstacle *pObstacle = nullptr;		// モデルへのポインタ

	// 先頭のオブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // 種類が障害物の場合

			// モデルの情報を取得する
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // モデルが NULL じゃない場合

				if (pObstacle != pTarget &&
					pObstacle->GetType() == CObstacle::TYPE_WOODBOX)
				{ // 対象自身じゃないかつ、木箱だった場合

					if (posOld.y + pTarget->GetFileData().vtxMin.y >= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMax.y &&
						pos.y + pTarget->GetFileData().vtxMin.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y &&
						pos.x + pTarget->GetFileData().vtxMax.x >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x + pTarget->GetFileData().vtxMin.x <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
					{ // 上からの当たり判定

						// 破片の設定処理
						CFraction::SetObstacle(pObstacle->GetPos(), pObstacle->GetFileData().m_nTexIdx[0], pObstacle->GetMaterial(0).MatD3D.Diffuse);

						// 破壊音を鳴らす
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

						// 木箱の終了処理
						pObstacle->Uninit();

						// 当たり判定を付与する
						bHit = true;
					}
					else if (posOld.y + pTarget->GetFileData().vtxMax.y <= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y + pTarget->GetFileData().vtxMax.y >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.x + pTarget->GetFileData().vtxMax.x >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x + pTarget->GetFileData().vtxMin.x <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
					{ // 下からの当たり判定

						// 破片の設定処理
						CFraction::SetObstacle(pObstacle->GetPos(), pObstacle->GetFileData().m_nTexIdx[0], pObstacle->GetMaterial(0).MatD3D.Diffuse);

						// 破壊音を鳴らす
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

						// 木箱の終了処理
						pObstacle->Uninit();

						// 当たり判定を付与する
						bHit = true;
					}
					else if (posOld.x + pTarget->GetFileData().vtxMax.x <= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x + pTarget->GetFileData().vtxMax.x >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.y + pTarget->GetFileData().vtxMax.y >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y + pTarget->GetFileData().vtxMin.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
					{ // 左からの当たり判定

						// 破片の設定処理
						CFraction::SetObstacle(pObstacle->GetPos(), pObstacle->GetFileData().m_nTexIdx[0], pObstacle->GetMaterial(0).MatD3D.Diffuse);

						// 破壊音を鳴らす
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

						// 木箱の終了処理
						pObstacle->Uninit();

						// 当たり判定を付与する
						bHit = true;
					}
					else if (posOld.x + pTarget->GetFileData().vtxMin.x >= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMax.x &&
						pos.x + pTarget->GetFileData().vtxMin.x <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
						pos.y + pTarget->GetFileData().vtxMax.y >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y + pTarget->GetFileData().vtxMin.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
					{ // 右からの当たり判定

						// 破片の設定処理
						CFraction::SetObstacle(pObstacle->GetPos(), pObstacle->GetFileData().m_nTexIdx[0], pObstacle->GetMaterial(0).MatD3D.Diffuse);

						// 破壊音を鳴らす
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

						// 木箱の終了処理
						pObstacle->Uninit();

						// 当たり判定を付与する
						bHit = true;
					}
				}						// 対象自身じゃないかつ、当たり判定が true の場合
			}							// モデルが NULL じゃない場合
		}								// 種類がブロックの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合

	// 当たり判定を返す
	return bHit;
}

//===============================
// ゴールとプレイヤーの当たり判定
//===============================
bool collision::GoalPlayerCollision(CGoal* pGoal)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトのポインタ
	CPlayer* pPlayer = nullptr;	// オブジェクトのポインタ
	D3DXVECTOR3 posU, posD;		// 上座標、下座標

	// 先頭のオブジェクトの情報を取得する
	pObj = CObject::GetTop(CObject::PRIORITY_PLAYER);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_PLAYER)
		{ // プレイヤーのオブジェクトの場合

			// プレイヤーの情報を取得する
			pPlayer = dynamic_cast<CPlayer*>(pObj);

			if (pPlayer != nullptr)
			{ // プレイヤーが NULL の場合

				// 座標の設定
				posU = D3DXVECTOR3(pGoal->GetPos().x - pGoal->GetSize().x, pGoal->GetPos().y + pGoal->GetSize().y, 0.0f);
				posD = D3DXVECTOR3(pGoal->GetPos().x - pGoal->GetSize().x, pGoal->GetPos().y - pGoal->GetSize().y, 0.0f);

				if (LineOuterProductXY(posU, posD, pPlayer->GetPos()) >= 0)
				{ // 外積の当たり判定に入った場合

					// true を返す
					return true;
				}
				else
				{ // 外積の当たり判定に入って居なかった場合

					// while文を抜け出す
					break;
				}
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}

	// false を返す
	return false;
}

//===============================
// 敵とブロックの当たり判定
//===============================
bool collision::EnemyBlockCollision(CEnemy* pEnemy)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = pEnemy->GetPos();				// 位置
	D3DXVECTOR3 posOld = pEnemy->GetPosOld();		// 前回の位置
	bool bLand = false;			// 着地したかどうか

	// ローカルポインタ宣言
	CObject *pObj = nullptr;		// オブジェクトへのポインタ
	CBlock *pBlock = nullptr;		// モデルへのポインタ

	// 先頭のオブジェクトを取得
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // 種類が障害物の場合

			// モデルの情報を取得する
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // モデルが NULL じゃない場合

				if (posOld.y >= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMax.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
					pos.x + ENEMY_WIDTH >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.x - ENEMY_WIDTH <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // 上からの当たり判定

					// 位置を補正
					pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

					// 当たり判定を付与する
					bLand = true;
				}
				else if (posOld.x + ENEMY_WIDTH <= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + ENEMY_WIDTH >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.y + ENEMY_HEIGHT >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 左からの当たり判定

					// 位置を補正
					pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - ENEMY_WIDTH - COLLISION_ADD_DIFF_LENGTH;
				}
				else if (posOld.x - ENEMY_WIDTH >= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMax.x &&
					pos.x - ENEMY_WIDTH <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.y + ENEMY_HEIGHT >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // 右からの当たり判定

					// 位置を補正
					pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + ENEMY_WIDTH + COLLISION_ADD_DIFF_LENGTH;
				}
			}							// モデルが NULL じゃない場合
		}								// 種類がブロックの場合

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}									// オブジェクトが使用されている場合

	// 位置の設定処理
	pEnemy->SetPos(pos);

	// 着地判定を返す
	return bLand;
}