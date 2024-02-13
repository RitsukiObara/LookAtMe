//===========================================
//
// �����蔻��̃��C������[collision.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
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
// �}�N����`
//===============================
#define COLLISION_ADD_ATTACK_LENGTH		(D3DXVECTOR3(40.0f, 40.0f, 40.0f))		// �U���̓����蔻��̎��ӂ̉��Z��
#define COLLISION_ADHE_ATTACK_LENGTH	(D3DXVECTOR3(0.01f, 0.01f, 0.01f))		// �U���̖�������̎��ӂ̉��Z��
#define COLLISION_ADD_DIFF_LENGTH		(0.01f)									// �͂��Ȍ덷�𖄂߂邽�߂̃}�N����`(�˂�������h�~)
#define COLLISION_CLIFF_ADD_HEIGHT		(120.0f)								// �R�͂܂蔻��Œǉ����鍂��
#define COLLISION_ADD_BALLOON_LENGTH	(D3DXVECTOR3(30.0f, 30.0f, 0.0f))		// �����o���̒ǉ��̓����蔻��

//===============================
// �v���C���[�ƃu���b�N�̓����蔻��
//===============================
bool collision::PlayerBlockCollision(bool* pRight, bool* pLeft, CPlayer *player, const float fWidth, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player->GetPos();				// �ʒu
	D3DXVECTOR3 posOld = player->GetPosOld();		// �O��̈ʒu
	D3DXVECTOR3 move = player->GetMove();			// �ړ���

	bool bLand = false;				//���n�������ǂ���

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBlock *pBlock = nullptr;		// ���f���ւ̃|�C���^

	// �I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // ��ނ��u���b�N�̏ꍇ

			// ���f���̏����擾����
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (player->GetJump() == false)
				{ // �W�����v���Ă��Ȃ������ꍇ

					if (posOld.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
					{ // �ォ��̓����蔻��

						// �ʒu��␳
						pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

						// �ړ��ʂ�ݒ肷��
						move.y = 0.0f;

						// ���n�����t�^����
						bLand = true;

						// ������̔���̐ݒ菈��
						player->SetCrushBottom(true);
					}
					else if (posOld.y + fHeight <= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
					{ // ������̓����蔻��

						// �ʒu��␳
						pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y - PLAYER_COLLISION_HEIGHT;

						// �ړ��ʂ�ݒ肷��
						move.y = 0.0f;

						// �ォ��̔���̐ݒ菈��
						player->SetCrushTop(true);
					}
					else if (posOld.x + fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x&&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // ������̓����蔻��

						// �ʒu��␳
						pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;

						// �ړ��ʂ�ݒ肷��
						move.x = 0.0f;

						// �E����̔���̐ݒ菈��
						player->SetCrushRight(true);
					}
					else if (posOld.x - fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // �E����̓����蔻��

						// �ʒu��␳
						pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;

						// �ړ��ʂ�ݒ肷��
						move.x = 0.0f;

						// ������̔���̐ݒ菈��
						player->SetCrushLeft(true);
					}
				}							// �W�����v���Ă��Ȃ�������
				else if (player->GetJump() == true)
				{ // �W�����v���Ă����ꍇ

					if (posOld.x + fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // ������̓����蔻��

						if (pos.y + fHeight <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y - COLLISION_CLIFF_ADD_HEIGHT &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFFALL &&
							pBlock->GetCliffLeft() == true)
						{ // �R�͂܂蔻��

							// �ʒu��ݒ肷��
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;
							pos.y = pBlock->GetPos().y + (pBlock->GetFileData().vtxMax.y - fHeight);

							// �R���܂��Ԃɂ���
							player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFCATCH);

							// �R�߂܂胂�[�V������ݒ肷��
							player->GetMotion()->Set(CPlayer::MOTIONTYPE_CLIFFCATCH);

							{ // �G�t�F�N�g����

								// �G�t�F�N�g�̈ʒu
								D3DXVECTOR3 EffectPos(pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x, pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y, pBlock->GetPos().z);

								// ���̐ݒ菈��
								CParticle::Create(EffectPos, CEffect::TYPE_CATCH);

								// �j�Ђ̐ݒ菈��
								CFraction::SetRCliffCatch(EffectPos, pBlock->GetFileData().m_nTexIdx[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}

							// �R�߂܂艹��炷
							CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFCATCH);
						}
						else if (pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH)
						{ // �p�Ɍ���

							// �ʒu��␳
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x;
							pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH * 2.0f;

							// �ʏ��Ԃɂ���
							player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);
						}
						else if (player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB)
						{ // ��L�ȊO(���ǂɂ��Ă���ꍇ)

							// ���ǂɂ��Ă���
							*pLeft = true;

							// �ʒu��␳
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;
						}

						// �ړ��ʂ�ݒ肷��
						move.x = 0.0f;

						// �E����̔���̐ݒ菈��
						player->SetCrushRight(true);
					}
					else if (posOld.x - fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // �E����̓����蔻��

						if (pos.y + fHeight <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y - COLLISION_CLIFF_ADD_HEIGHT &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFFALL &&
							pBlock->GetCliffRight() == true)
						{ // �R�͂܂蔻��

							// �ʒu��ݒ肷��
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;
							pos.y = pBlock->GetPos().y + (pBlock->GetFileData().vtxMax.y - fHeight);

							// �R���܂��Ԃɂ���
							player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFCATCH);

							// �R�߂܂胂�[�V������ݒ肷��
							player->GetMotion()->Set(CPlayer::MOTIONTYPE_CLIFFCATCH);

							{ // �G�t�F�N�g����

								// �G�t�F�N�g�̈ʒu
								D3DXVECTOR3 EffectPos(pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x, pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y, pBlock->GetPos().z);

								// ���̐ݒ菈��
								CParticle::Create(EffectPos, CEffect::TYPE_CATCH);

								// �j�Ђ̐ݒ菈��
								CFraction::SetLCliffCatch(EffectPos, pBlock->GetFileData().m_nTexIdx[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}

							// �R�߂܂艹��炷
							CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFCATCH);
						}
						else if (pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH)
						{ // �p�Ɍ���

							// �ʒu��␳
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x;
							pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH * 2.0f;

							// �ʏ��Ԃɂ���
							player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);
						}
						else if (player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
							player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFCLIMB)
						{ // ��L�ȊO(�E�ǂɂ��Ă���ꍇ)

							// �E�ǂɂ��Ă���
							*pRight = true;

							// �ʒu��␳
							pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;
						}

						// �ړ��ʂ�ݒ肷��
						move.x = 0.0f;

						// ������̔���̐ݒ菈��
						player->SetCrushLeft(true);
					}
					else if (posOld.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
						pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
					{ // �ォ��̓����蔻��

						if (player->GetAction()->GetAct() == CPlayerAct::ACT_CLIFFCLIMB)
						{ // �R������Ԃ̏ꍇ

							// �R�������n��Ԃɂ���
							player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFLAND);

							// �ړ��ʂ�ݒ肷��
							move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

							// �R���n����炷
							CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFLAND);
						}
						else if(player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW)
						{ // ���n�]�C��ԈȊO

							// �ʏ��Ԃɂ���
							player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);

							if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
							{ // �d�͂����ȏ�̏ꍇ

								// ���n���[�V������ݒ肷��
								player->GetMotion()->Set(CPlayer::MOTIONTYPE_LANDING);

								// �����Ƃ��납��̒��n����炷
								CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BIGLANDING);
							}
							else
							{ // ��L�ȊO

								// ���n����炷
								CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
							}
						}

						// �ʒu��␳
						pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

						if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
						{ // �d�͂����ȏ�̏ꍇ

							// ���n�̔g��𐶐�����
							CRipple::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

							// ���n���̐���
							CParticle::Create(pos, CEffect::TYPE_LANDDUST);

							if (CManager::GetCamera() != nullptr &&
								CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST)
							{ // �J�����̏�񂪂��邩�A�u�[�X�g��ԈȊO�̏ꍇ

								// ���n�J�����ɐݒ肷��
								CManager::GetCamera()->SetType(CCamera::TYPE_LAND);
							}
						}

						// �ړ��ʂ�ݒ肷��
						move.y = 0.0f;

						// ���n�����t�^����
						bLand = true;

						// ������̔���̐ݒ菈��
						player->SetCrushBottom(true);
					}
					else if (posOld.y + fHeight <= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
						pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
					{ // ������̓����蔻��

						// �ʒu��␳
						pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y - PLAYER_COLLISION_HEIGHT;

						// �ړ��ʂ�ݒ肷��
						move.y = 0.0f;

						// �ォ��̔���̐ݒ菈��
						player->SetCrushTop(true);
					}
				}						// �W�����v���Ă����ꍇ
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��ݒ肷��
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ

	//if (bCrushBottom == true && bCrushTop == true)
	//{//�c���牟���Ԃ���Ă���ꍇ
	//	//����������
	//	*pLife -= 1;
	//}
	
	//if (bCrushRight == true && bCrushLeft == true)
	//{//�����牟���Ԃ���Ă���ꍇ
	//	//����������
	//	*pLife -= 1;
	//}

	// �ʒu�̐ݒ菈��
	player->SetPos(pos);
	player->SetMove(move);

	return bLand;			//���n�̕ϐ���Ԃ�
}

//===============================
// �v���C���[�Ə�Q���̓����蔻��
//===============================
bool collision::PlayerObstacleCollision(CPlayer *player, const float fWidth, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player->GetPos();				// �ʒu
	D3DXVECTOR3 posOld = player->GetPosOld();		// �O��̈ʒu
	D3DXVECTOR3 move = player->GetMove();			// �ړ���

	bool bLand = false;				//���n�������ǂ���

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;			// �I�u�W�F�N�g�ւ̃|�C���^
	CObstacle *pObstacle = nullptr;		// ���f���ւ̃|�C���^
	CStreetLamp *pLamp = nullptr;		// �X��(����)�ւ̃|�C���^

	// �I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // ��ނ���Q���̏ꍇ

			// ���f���̏����擾����
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (pObstacle->GetType() != CObstacle::TYPE_POLE)
				{ // �X��(�x���ȊO)

					if (pObstacle->GetCollision() == true)
					{ // �����蔻��󋵂� true �̏ꍇ

						if (player->GetJump() == false)
						{ // �W�����v���Ă��Ȃ������ꍇ

							if (posOld.y >= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMax.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
							{ // �ォ��̓����蔻��

								// �ʒu��␳
								pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

								// �ړ��ʂ�ݒ肷��
								move.y = 0.0f;

								// ���n�����t�^����
								bLand = true;

								// ������̔���̐ݒ菈��
								player->SetCrushBottom(true);
							}
							else if (posOld.y + fHeight <= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
							{ // ������̓����蔻��

								// �ʒu��␳
								pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y - PLAYER_COLLISION_HEIGHT;

								// �ړ��ʂ�ݒ肷��
								move.y = 0.0f;

								// �ォ��̔���̐ݒ菈��
								player->SetCrushTop(true);
							}
							else if (posOld.x + fWidth <= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
							{ // ������̓����蔻��

								// �ʒu��␳
								pos.x = pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;

								// �ړ��ʂ�ݒ肷��
								move.x = 0.0f;

								// �E����̔���̐ݒ菈��
								player->SetCrushRight(true);
							}
							else if (posOld.x - fWidth >= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMax.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
							{ // �E����̓����蔻��

								// �ʒu��␳
								pos.x = pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;

								// �ړ��ʂ�ݒ肷��
								move.x = 0.0f;

								// ������̔���̐ݒ菈��
								player->SetCrushLeft(true);
							}
						}							// �W�����v���Ă��Ȃ�������
						else if (player->GetJump() == true)
						{ // �W�����v���Ă����ꍇ

							if (posOld.x + fWidth <= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
							{ // ������̓����蔻��

								if (pObstacle->GetType() == CObstacle::TYPE_LAMP &&
									player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
								{ // ��ނ��X��(����)�̏ꍇ

									// �X��(����)�ɃL���X�g����
									pLamp = dynamic_cast<CStreetLamp*>(pObstacle);

									if (pLamp != nullptr)
									{ // �L���X�g�ɐ��������ꍇ

										// �v���C���[�̃L���b�`����
										pLamp->PlayerCatch(&pos);
									}
								}
								else if (player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
								{ // ��L�ȊO

									// �ʒu��␳
									pos.x = pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x - PLAYER_COLLISION_WIDTH;
								}

								// �E����̔���̐ݒ菈��
								player->SetCrushRight(true);
							}
							else if (posOld.x - fWidth >= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMax.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
							{ // �E����̓����蔻��

								if (pObstacle->GetType() == CObstacle::TYPE_LAMP &&
									player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
								{ // ��ނ��X��(����)���A�Ԃ牺�����ԈȊO�̏ꍇ

									// �X��(����)�ɃL���X�g����
									pLamp = dynamic_cast<CStreetLamp*>(pObstacle);

									if (pLamp != nullptr)
									{ // �L���X�g�ɐ��������ꍇ

										// �v���C���[�̃L���b�`����
										pLamp->PlayerCatch(&pos);
									}
								}
								else if (player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
								{ // ��L�ȊO

									// �ʒu��␳
									pos.x = pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x + PLAYER_COLLISION_WIDTH;
								}

								// ������̔���̐ݒ菈��
								player->SetCrushLeft(true);
							}
							else if (posOld.y >= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMax.y &&
								pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
							{ // �ォ��̓����蔻��

								if (player->GetAction()->GetAct() == CPlayerAct::ACT_CLIFFCLIMB)
								{ // �R������Ԃ̏ꍇ

									// �R�������n��Ԃɂ���
									player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFLAND);

									// �ړ��ʂ�ݒ肷��
									move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								}
								else if (player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW)
								{ // ���n�]�C��ԈȊO�̏ꍇ

									// �ʏ��Ԃɂ���
									player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);

									if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
									{ // �d�͂����ȏ�̏ꍇ

										// ���n���[�V������ݒ肷��
										player->GetMotion()->Set(CPlayer::MOTIONTYPE_LANDING);

										// �����Ƃ��납��̒��n����炷
										CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BIGLANDING);
									}
									else
									{ // ��L�ȊO

										// ���n����炷
										CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
									}
								}

								// �ʒu��␳
								pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

								if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
								{ // �d�͂����ȏ�̏ꍇ

									// ���n�̔g��𐶐�����
									CRipple::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

									// ���n���̐���
									CParticle::Create(pos, CEffect::TYPE_LANDDUST);

									if (CManager::GetCamera() != nullptr &&
										CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST)
									{ // �J�����̏�񂪂��邩�A�u�[�X�g��ԈȊO�̏ꍇ

										// ���n�J�����ɐݒ肷��
										CManager::GetCamera()->SetType(CCamera::TYPE_LAND);
									}
								}

								// �ړ��ʂ�ݒ肷��
								move.y = 0.0f;

								// ���n�����t�^����
								bLand = true;

								// ������̔���̐ݒ菈��
								player->SetCrushBottom(true);
							}
							else if (posOld.y + fHeight <= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMin.y &&
								pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
								pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
								pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
							{ // ������̓����蔻��

								// �ʒu��␳
								pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y - PLAYER_COLLISION_HEIGHT;

								// �ړ��ʂ�ݒ肷��
								move.y = 0.0f;

								// �ォ��̔���̐ݒ菈��
								player->SetCrushTop(true);
							}
						}					// �W�����v���Ă����ꍇ
					}						// �����蔻��󋵂� true �̏ꍇ
				}
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ

	//if (bCrushBottom == true && bCrushTop == true)
	//{//�c���牟���Ԃ���Ă���ꍇ
	//	//����������
	//	*pLife -= 1;
	//}

	//if (bCrushRight == true && bCrushLeft == true)
	//{//�����牟���Ԃ���Ă���ꍇ
	//	//����������
	//	*pLife -= 1;
	//}

	// �ʒu�̐ݒ菈��
	player->SetPos(pos);
	player->SetMove(move);

	return bLand;			//���n�̕ϐ���Ԃ�
}

//===============================
// �v���C���[�̏�Q���ɑ΂���U�����菈��
//===============================
void collision::PlayerObstcleAttack(CPlayer *player)
{
	// ���[�J���ϐ��錾
	CObject *pObj = nullptr;			// �I�u�W�F�N�g�ւ̃|�C���^
	CObstacle *pObstacle = nullptr;		// �A�C�e���ւ̃|�C���^
	D3DXVECTOR3 VtxMax, VtxMin;			// ���_�̍ő�l,�ŏ��l
	D3DXVECTOR3 EffectPos;				// �G�t�F�N�g�̈ʒu

	// �擪�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // ��ނ��v���C���[�������ꍇ

			// ���f���̏����擾����
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (pObstacle->GetAttack() == true &&
					pObstacle->GetType() != CObstacle::TYPE_WOODBOX &&
					pObstacle->GetType() != CObstacle::TYPE_LAMP)
				{ // �A�N�V�������肪 true ���A�ؔ��ȊO���A�X��(����)�ȊO�̏ꍇ

					if (pObstacle->GetType() == CObstacle::TYPE_DRUM)
					{ // �h�����ʂ̏ꍇ

						// �ő�l�E�ŏ��l��ݒ肷��
						VtxMax = D3DXVECTOR3(pObstacle->GetFileData().vtxMax.x, -pObstacle->GetFileData().vtxMin.z, 0.0f) + COLLISION_ADD_ATTACK_LENGTH;
						VtxMin = D3DXVECTOR3(pObstacle->GetFileData().vtxMin.x, -pObstacle->GetFileData().vtxMax.z, 0.0f) - COLLISION_ADD_ATTACK_LENGTH;
					}
					else
					{ // ��L�ȊO

						// �ő�l�E�ŏ��l��ݒ肷��
						VtxMax = pObstacle->GetFileData().vtxMax + COLLISION_ADD_ATTACK_LENGTH;
						VtxMin = pObstacle->GetFileData().vtxMin - COLLISION_ADD_ATTACK_LENGTH;
					}

					// ��`�̓����蔻��
					if (RectangleCollisionXY
					(
						player->GetPos(),
						pObstacle->GetPos(),
						D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f),
						VtxMax,
						D3DXVECTOR3(-PLAYER_WIDTH, 0.0f, 0.0f),
						VtxMin
					) == true)
					{ // ��`�̓����蔻��ɐ��������ꍇ

						if (player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
						{ // �Ԃ牺�����ԈȊO

							if (pObstacle->GetType() == CObstacle::TYPE_POLE)
							{ // �X���̎x���̏ꍇ

								if (player->GetJump() == true &&
									player->GetAction()->GetAct() != CPlayerAct::ACT_POLEDOWN)
								{ // �|�[�������ԈȊO���A�W�����v���Ă���ꍇ

									// �U���͈͓��ɐݒ肷��
									pObstacle->SetRange(true);

									// �G�t�F�N�g�̈ʒu��ݒ肷��
									EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
									EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
									EffectPos.z = 0.0f;

									// �I�[���𐶐�����
									CAura::Create(player->GetPos() + EffectPos, AURA_NORMAL_LIFE);
								}
								else
								{ // ��L�ȊO

									// �U���͈͊O�ɐݒ肷��
									pObstacle->SetRange(false);
								}
							}
							else
							{ // ��L�ȊO

								// �U���͈͓��ɐݒ肷��
								pObstacle->SetRange(true);

								// �G�t�F�N�g�̈ʒu��ݒ肷��
								EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
								EffectPos.y = (float)(rand() % (int)(PLAYER_HEIGHT));
								EffectPos.z = 0.0f;

								// �I�[���𐶐�����
								CAura::Create(player->GetPos() + EffectPos, AURA_NORMAL_LIFE);
							}
						}
						else
						{ // ��L�ȊO

							// �U���͈͊O�ɐݒ肷��
							pObstacle->SetRange(false);
						}

						if (pObstacle->GetType() == CObstacle::TYPE_DRUM)
						{ // �h�����ʂ̏ꍇ

							// �ő�l�E�ŏ��l��ݒ肷��
							VtxMax = D3DXVECTOR3(pObstacle->GetFileData().vtxMax.x, -pObstacle->GetFileData().vtxMin.z, 0.0f) + COLLISION_ADHE_ATTACK_LENGTH;
							VtxMin = D3DXVECTOR3(pObstacle->GetFileData().vtxMin.x, -pObstacle->GetFileData().vtxMax.z, 0.0f) - COLLISION_ADHE_ATTACK_LENGTH;
						}
						else
						{ // ��L�ȊO

							// �ő�l�E�ŏ��l��ݒ肷��
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
						{ // SPACE�L�[����莞�ԉ��������A�ʏ��Ԃ����W�����v��Ԃ̏ꍇ

							if (pObstacle->Hit() == true)
							{ // �����蔻�肪 true �̏ꍇ

								// �U���̃A�N�V����UI�𐶐�����
								CActionUI::Create(CActionUI::TYPE_ATTACK, PLAYER_OBSTACLE_SCORE);

								// �{�^���̉����󋵂� false �ɂ���
								player->SetButtonPush(false);

								// �{�^���J�E���g������������
								player->SetButtonCount(0);

								if (pObstacle->GetType() != CObstacle::TYPE_POLE)
								{ // �|�[���ȊO�̏ꍇ

									// �U�����[�V������ݒ肷��
									player->GetMotion()->Set(CPlayer::MOTIONTYPE_ATTACK);
								}

								// ���x���グ��
								player->SetSpeed(player->GetSpeed() + PLAYER_ACTION_ADD_SPEED);
							}
						}
						else if (player->GetButton().nButtonCount >= PLAYER_ATTACK_BTNCOUNT &&
							player->GetButton().nButtonCount < PLAYER_BIGJUMP_BTNCOUNT &&
							player->GetButton().bSpacePush == true &&
							(player->GetAction()->GetAct() == CPlayerAct::ACT_NONE || player->GetAction()->GetAct() == CPlayerAct::ACT_BIGJUMP))
						{ // SPACE�L�[����莞�ԉ��������A�ʏ��Ԃ����W�����v��Ԃ̏ꍇ

							if (pObstacle->Hit() == true)
							{ // �����蔻�肪 true �̏ꍇ

								// �U���̃A�N�V����UI�𐶐�����
								CActionUI::Create(CActionUI::TYPE_ATTACK, PLAYER_OBSTACLE_SCORE);

								// �{�^���̉����󋵂� false �ɂ���
								player->SetButtonPush(false);

								// �{�^���J�E���g������������
								player->SetButtonCount(0);

								if (pObstacle->GetType() != CObstacle::TYPE_POLE)
								{ // �|�[���ȊO�̏ꍇ

									// �U�����[�V������ݒ肷��
									player->GetMotion()->Set(CPlayer::MOTIONTYPE_ATTACK);
								}

								// ���x���グ��
								player->SetSpeed(player->GetSpeed() + PLAYER_ACTION_ADD_SPEED);
							}
						}
					}
					else
					{ // �����蔻��Ɏ��s�����ꍇ

						// �U���͈͊O�ɐݒ肷��
						pObstacle->SetRange(false);
					}
				}
				else
				{ // ��L�ȊO

					// �U���͈͊O�ɐݒ肷��
					pObstacle->SetRange(false);
				}
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}
}

//===============================
// �v���C���[�Ɠ����蔻��|���S���̓����蔻��
//===============================
bool collision::PlayerCollPolygonCollision(CPlayer *player, const float fWidth, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player->GetPos();				// �ʒu
	D3DXVECTOR3 posOld = player->GetPosOld();		// �O��̈ʒu
	D3DXVECTOR3 move = player->GetMove();			// �ړ���

	bool bLand = false;				//���n�������ǂ���

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;				// �I�u�W�F�N�g�ւ̃|�C���^
	CCollPolygon *pColl = nullptr;		// ���f���ւ̃|�C���^

	// �I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BG);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_COLLPOLY)
		{ // ��ނ������蔻��|���S���̏ꍇ

			// ���f���̏����擾����
			pColl = dynamic_cast<CCollPolygon*>(pObj);

			if (pColl != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (pColl->GetPos().z == 0.0f)
				{ // Z���� 0.0f �������ꍇ

					if (player->GetJump() == false && 
						player->GetAction()->GetAct() != CPlayerAct::ACT_POLEDOWN &&
						player->GetAction()->GetAct() != CPlayerAct::ACT_HANGING &&
						player->GetAction()->GetAct() != CPlayerAct::ACT_LAMPJUMP)
					{ // �W�����v���Ă��Ȃ����A���̏�ԈȊO�̏ꍇ

						if (posOld.y >= pColl->GetPos().y + pColl->GetVtxMax().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x)
						{ // �ォ��̓����蔻��

							// �ʒu��␳
							pos.y = pColl->GetPos().y + pColl->GetVtxMax().y + COLLISION_ADD_DIFF_LENGTH;

							// �ړ��ʂ�ݒ肷��
							move.y = 0.0f;

							// ���n�����t�^����
							bLand = true;

							// ������̔���̐ݒ菈��
							player->SetCrushBottom(true);
						}
						else if (posOld.y + fHeight <= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x)
						{ // ������̓����蔻��

							// �ʒu��␳
							pos.y = pColl->GetPos().y + pColl->GetVtxMin().y - PLAYER_COLLISION_HEIGHT;

							// �ړ��ʂ�ݒ肷��
							move.y = 0.0f;

							// �ォ��̔���̐ݒ菈��
							player->SetCrushTop(true);
						}
						else if (posOld.x + fWidth <= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y)
						{ // ������̓����蔻��

							// �ʒu��␳
							pos.x = pColl->GetPos().x + pColl->GetVtxMin().x - PLAYER_COLLISION_WIDTH;

							// �E����̔���̐ݒ菈��
							player->SetCrushRight(true);
						}
						else if (posOld.x - fWidth >= pColl->GetPos().x + pColl->GetVtxMax().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y)
						{ // �E����̓����蔻��

							// �ʒu��␳
							pos.x = pColl->GetPos().x + pColl->GetVtxMax().x + PLAYER_COLLISION_WIDTH;

							// ������̔���̐ݒ菈��
							player->SetCrushLeft(true);
						}
					}							// �W�����v���Ă��Ȃ�������
					else if (player->GetJump() == true)
					{ // �W�����v���Ă����ꍇ

						if (posOld.x + fWidth <= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y)
						{ // ������̓����蔻��

							// �ʒu��␳
							pos.x = pColl->GetPos().x + pColl->GetVtxMin().x - PLAYER_COLLISION_WIDTH;

							// �E����̔���̐ݒ菈��
							player->SetCrushRight(true);
						}
						else if (posOld.x - fWidth >= pColl->GetPos().x + pColl->GetVtxMax().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y)
						{ // �E����̓����蔻��

							// �ʒu��␳
							pos.x = pColl->GetPos().x + pColl->GetVtxMax().x + PLAYER_COLLISION_WIDTH;

							// ������̔���̐ݒ菈��
							player->SetCrushLeft(true);
						}
						else if (posOld.y >= pColl->GetPos().y + pColl->GetVtxMax().y &&
							pos.y <= pColl->GetPos().y + pColl->GetVtxMax().y &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x)
						{ // �ォ��̓����蔻��

							if (player->GetAction()->GetAct() == CPlayerAct::ACT_CLIFFCLIMB)
							{ // �R������Ԃ̏ꍇ

								// �R�������n��Ԃɂ���
								player->GetAction()->ActionChange(CPlayerAct::ACT_CLIFFLAND);

								// �ړ��ʂ�ݒ肷��
								move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
							else if (player->GetAction()->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW)
							{ // ���n�]�C��ԈȊO�̏ꍇ

								// �ʏ��Ԃɂ���
								player->GetAction()->ActionChange(CPlayerAct::ACT_NONE);

								if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
								{ // �d�͂����ȏ�̏ꍇ

									// ���n���[�V������ݒ肷��
									player->GetMotion()->Set(CPlayer::MOTIONTYPE_LANDING);

									// �����Ƃ��납��̒��n����炷
									CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BIGLANDING);
								}
								else
								{ // ��L�ȊO

									// ���n����炷
									CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
								}
							}

							// �ʒu��␳
							pos.y = pColl->GetPos().y + pColl->GetVtxMax().y;

							if (move.y <= PLAYER_LAND_STAGING_GRAVITY)
							{ // �d�͂����ȏ�̏ꍇ

								// ���n�̔g��𐶐�����
								CRipple::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

								// ���n���̐���
								CParticle::Create(pos, CEffect::TYPE_LANDDUST);

								if (CManager::GetCamera() != nullptr &&
									CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST)
								{ // �J�����̏�񂪂��邩�A�u�[�X�g��ԈȊO�̏ꍇ

									// ���n�J�����ɐݒ肷��
									CManager::GetCamera()->SetType(CCamera::TYPE_LAND);
								}
							}

							// �ړ��ʂ�ݒ肷��
							move.y = 0.0f;

							// ���n�����t�^����
							bLand = true;

							// ������̔���̐ݒ菈��
							player->SetCrushBottom(true);
						}
						else if (posOld.y + fHeight <= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.y + fHeight >= pColl->GetPos().y + pColl->GetVtxMin().y &&
							pos.x + fWidth >= pColl->GetPos().x + pColl->GetVtxMin().x &&
							pos.x - fWidth <= pColl->GetPos().x + pColl->GetVtxMax().x)
						{ // ������̓����蔻��

							// �ʒu��␳
							pos.y = pColl->GetPos().y + pColl->GetVtxMin().y - PLAYER_COLLISION_HEIGHT;

							// �ړ��ʂ�ݒ肷��
							move.y = 0.0f;

							// �ォ��̔���̐ݒ菈��
							player->SetCrushTop(true);
						}
					}					// �W�����v���Ă����ꍇ
				}						// Z����0.0f�������ꍇ
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ������蔻��|���S���̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ

	//if (bCrushBottom == true && bCrushTop == true)
	//{//�c���牟���Ԃ���Ă���ꍇ
	//	//����������
	//	*pLife -= 1;
	//}
	
	//if (bCrushRight == true && bCrushLeft == true)
	//{//�����牟���Ԃ���Ă���ꍇ
	//	//����������
	//	*pLife -= 1;
	//}
	
	// �ʒu�̐ݒ菈��
	player->SetPos(pos);
	player->SetMove(move);

	return bLand;			//���n�̕ϐ���Ԃ�
}

//===============================
// �A�C�e���̓����蔻��
//===============================
void collision::PlayerItemCollision(CPlayer &player)
{
	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CItem *pItem = nullptr;		// �A�C�e���ւ̃|�C���^

	// �I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_ITEM)
		{ // ��ނ��v���C���[�������ꍇ

			// ���f���̏����擾����
			pItem = dynamic_cast<CItem*>(pObj);

			if (pItem != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				// �~�̓����蔻��
				if (CircleCollisionXY(player.GetPos(), pItem->GetPos(), PLAYER_WIDTH, pItem->GetFileData().fRadius) == true)
				{ // �A�C�e���ɐG�ꂽ�ꍇ

					// �A�C�e���̏I������
					pItem->Hit();

					if (CManager::GetMode() == CScene::MODE_GAME &&
						player.GetStaminaUI() != nullptr)
					{ // �Q�[�����[�h���A�X�^�~�iUI�̃|�C���^�� NULL ����Ȃ��ꍇ

						// �񕜏�Ԃɂ���
						player.GetStaminaUI()->SetState(CStaminaUI::STATE_HEAL);
					}

					// ���̐�̏������s��Ȃ�
					return;
				}
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}
}

//===============================
// �v���C���[�ƃu���b�N�̂߂荞�ݔ���
//===============================
void collision::PlayerBlockPenetration(CPlayer *player, const float fWidth, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player->GetPos();				// �ʒu

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBlock *pBlock = nullptr;		// ���f���ւ̃|�C���^

	// �I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // ��ނ��u���b�N�̏ꍇ

			// ���f���̏����擾����
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // �����Ԃ�Ă���ꍇ

					// ���̓����蔻��� true �ɂ���
					player->SetCrushLeft(true);
				}
				else if (pos.x + fWidth >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.x - fWidth <= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.y + fHeight >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // �E���Ԃ�Ă���ꍇ

					// �E�̓����蔻��� true �ɂ���
					player->SetCrushRight(true);
				}
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��ݒ肷��
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ
}

//===============================
// �v���C���[�Ə�Q���̂߂荞�ݔ���
//===============================
void collision::PlayerObstaclePenetration(CPlayer *player, const float fWidth, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player->GetPos();				// �ʒu

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;			// �I�u�W�F�N�g�ւ̃|�C���^
	CObstacle *pObstacle = nullptr;		// ���f���ւ̃|�C���^

	// �I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // ��ނ���Q���̏ꍇ

			// ���f���̏����擾����
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (pObstacle->GetType() != CObstacle::TYPE_LAMP &&
					pObstacle->GetType() != CObstacle::TYPE_POLE)
				{ // �X���ȊO�̏ꍇ

					if (pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
						pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
						pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
					{ // �����Ԃ�Ă���ꍇ

						// ���̓����蔻��� true �ɂ���
						player->SetCrushLeft(true);
					}
					else if (pos.x + fWidth >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x - fWidth <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.y + fHeight >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
					{ // �E���Ԃ�Ă���ꍇ

						// �E�̓����蔻��� true �ɂ���
						player->SetCrushRight(true);
					}
				}						// �X���ȊO�̏ꍇ
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ
}

//===============================
// �v���C���[�Ɠ����蔻��|���S���̂߂荞�ݔ���
//===============================
void collision::PlayerCollPolygonPenetration(CPlayer *player, const float fWidth, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player->GetPos();				// �ʒu

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;				// �I�u�W�F�N�g�ւ̃|�C���^
	CCollPolygon *pColl = nullptr;		// ���f���ւ̃|�C���^

	// �I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BG);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_COLLPOLY)
		{ // ��ނ������蔻��|���S���̏ꍇ

			// ���f���̏����擾����
			pColl = dynamic_cast<CCollPolygon*>(pObj);

			if (pColl != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (pos.x - fWidth <= pColl->GetPos().x + pColl->GetSize().x &&
					pos.x + fWidth >= pColl->GetPos().x + pColl->GetSize().x &&
					pos.y + fHeight >= pColl->GetPos().y - pColl->GetSize().y &&
					pos.y <= pColl->GetPos().y + pColl->GetSize().y)
				{ // �����Ԃ�Ă���ꍇ

					// ���̓����蔻��� true �ɂ���
					player->SetCrushLeft(true);
				}
				else if (pos.x + fWidth >= pColl->GetPos().x - pColl->GetSize().x &&
					pos.x - fWidth <= pColl->GetPos().x - pColl->GetSize().x &&
					pos.y + fHeight >= pColl->GetPos().y - pColl->GetSize().y &&
					pos.y <= pColl->GetPos().y + pColl->GetSize().y)
				{ // �E���Ԃ�Ă���ꍇ

					// �E�̓����蔻��� true �ɂ���
					player->SetCrushRight(true);
				}
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ������蔻��|���S���̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ
}

//===============================
// �v���C���[�ƃJ�����͈̔͂̓����蔻�菈��
//===============================
void collision::PlayerCameraRangeCollision(CPlayer& player, const float fWidth, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 VtxMax, VtxMin, PlayerPos;	// �v���C���[�̍ő�l�A�v���C���[�̍ŏ��l�A�v���C���[�̈ʒu
	CObject* pObj;							// �I�u�W�F�N�g�̃|�C���^
	CCameraRange* pCameRange;				// �J�����͈̔͂̃|�C���^

	if (CManager::GetMode() == CScene::MODE_GAME &&
		CGame::GetState() != CGame::STATE_START)
	{ // �Q�[�����[�h���A�J�n��ԈȊO�̏ꍇ

		// �v���C���[�̈ʒu��ݒ肷��
		PlayerPos = D3DXVECTOR3(player.GetPos().x, player.GetPos().y + (fHeight * 0.5f), player.GetPos().z);

		// �v���C���[�̍ő�l��ݒ肷��
		VtxMax = D3DXVECTOR3(fWidth, fHeight * 0.5f, 0.0f);

		// �v���C���[�̍ŏ��l��ݒ肷��
		VtxMin = D3DXVECTOR3(-fWidth, -(fHeight * 0.5f), 0.0f);

		// �I�u�W�F�N�g���擾
		pObj = CObject::GetTop(CObject::PRIORITY_BG);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�� NULL ����Ȃ������

			if (pObj->GetType() == CObject::TYPE_CAMERARANGE)
			{ // ��ނ��J�����͈̔͂̏ꍇ

				// ���f���̏����擾����
				pCameRange = dynamic_cast<CCameraRange*>(pObj);

				if (pCameRange != nullptr)
				{ // �|�C���^�� NULL ����Ȃ��ꍇ

					// ��`�̓����蔻��
					if (RectangleCollisionXY
					(
						PlayerPos,						// �v���C���[�̈ʒu
						pCameRange->GetPos(),			// �J�����͈̔͂̈ʒu
						VtxMax,							// �v���C���[�̍ő�l
						pCameRange->GetSize(),			// �J�����͈̔͂̍ő�l
						VtxMin,							// �v���C���[�̍ŏ��l
						-pCameRange->GetSize()			// �J�����͈̔͂̍ŏ��l
					) == true)
					{ // �����蔻��ɓ������ꍇ

						if (pCameRange->GetType() == CCamera::TYPE_PRECEDENCE &&
							CManager::GetCamera()->GetType() == CCamera::TYPE_PRECEDENCE)
						{ // ���ɐ�s���e�J�����ɂȂ��Ă����ꍇ

							// ���̏����𔲂���
							return;
						}

						// �J�����̎�ނ�ݒ肷��
						CManager::GetCamera()->SetType(pCameRange->GetType());

						// ���̐�̏������s��Ȃ�
						return;
					}
				}							// �|�C���^�� NULL ����Ȃ��ꍇ
			}								// ��ނ��J�����͈̔͂̏ꍇ

			// ���̃I�u�W�F�N�g��������
			pObj = pObj->GetNext();
		}									// �I�u�W�F�N�g�� NULL ����Ȃ������

		if (CManager::GetCamera() != nullptr)
		{ // �J����������ꍇ

			if (CManager::GetCamera()->GetType() != CCamera::TYPE_LAND &&
				CManager::GetCamera()->GetType() != CCamera::TYPE_WALLKICK &&
				CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST &&
				CManager::GetCamera()->GetType() != CCamera::TYPE_PRECEDENCE)
			{ // ���̎�ވȊO�̏ꍇ

				// �J�����̎�ނ�ݒ肷��
				CManager::GetCamera()->SetType(CCamera::TYPE_NONE);
			}
		}
	}
}

//===============================
// �v���C���[�Ɛ����o���̓����蔻��
//===============================
void collision::PlayerBalloonHit(CPlayer* pPlayer)
{
	// ���[�J���ϐ��錾
	CObject* pObj;			// �I�u�W�F�N�g�̃|�C���^
	CBalloon* pBalloon;		// �����o���̃|�C���^
	D3DXVECTOR3 pos;		// �v���C���[�̈ʒu
	D3DXVECTOR3 VtxMax;		// �v���C���[�̍ő�l
	D3DXVECTOR3 VtxMin;		// �v���C���[�̍ŏ��l

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{ // �`���[�g���A���̏ꍇ

		// ����ݒ肷��
		pos = D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + (PLAYER_HEIGHT * 0.5f), 0.0f);		// �ʒu
		VtxMax = D3DXVECTOR3(PLAYER_WIDTH, (PLAYER_HEIGHT * 0.5f), 0.0f);								// �v���C���[�̍ő�l
		VtxMin = D3DXVECTOR3(-PLAYER_WIDTH, -(PLAYER_HEIGHT * 0.5f), 0.0f);								// �v���C���[�ɍŏ��l

		// �擪�̃I�u�W�F�N�g���擾����
		pObj = CObject::GetTop(CObject::PRIORITY_SHADOW);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�̃|�C���^�� NULL ����Ȃ��ꍇ

			if (pObj->GetType() == CObject::TYPE_BALLOON)
			{ // ��ނ������o���������ꍇ

				// �����o���Ƀ_�C�i�~�b�N�L���X�g����
				pBalloon = dynamic_cast<CBalloon*>(pObj);

				if (pBalloon != nullptr)
				{ // �����o���̃|�C���^�� NULL ����Ȃ������ꍇ

					if (RectangleCollisionXY
					(
						pos,
						pBalloon->GetPos(),
						VtxMax,
						pBalloon->GetSize() + COLLISION_ADD_BALLOON_LENGTH,
						VtxMin,
						-pBalloon->GetSize() - COLLISION_ADD_BALLOON_LENGTH
					) == true)
					{// �����o���ɓ��������ꍇ

						// �\����Ԃɐݒ肷��
						pBalloon->SetState(CBalloon::STATE_DISP);

						if (CTutorial::GetBalloonUI() != nullptr)
						{ // �ӂ�����2D�� NULL ����Ȃ��ꍇ

							// �ӂ�����2D�̏���ݒ肷��
							CTutorial::GetBalloonUI()->SetType(pBalloon->GetType());
						}

						if (pBalloon->GetType() == CBalloon::TYPE_SPEED)
						{ // �����o�������x�֌W�������ꍇ

							// ���xUI�\����Ԃ�ݒ肷��
							CTutorial::SetState(CTutorial::STATE_SPEED);
						}
						else if (pBalloon->GetType() == CBalloon::TYPE_BOOST)
						{ // �����o�����u�[�X�g�̏ꍇ

							// �u�[�X�g��Ԃ�ݒ肷��
							CTutorial::SetState(CTutorial::STATE_BOOST);
						}
						else if (pBalloon->GetType() == CBalloon::TYPE_POLEJUMP)
						{ // �����o�����X���W�����v�̏ꍇ

							// �X���W�����v��Ԃ�ݒ肷��
							CTutorial::SetState(CTutorial::STATE_POLE);
						}
						else if (pBalloon->GetType() == CBalloon::TYPE_STAMINA)
						{ // �ӂ��������X�^�~�i�̏ꍇ

							// �X�^�~�i��Ԃ�ݒ肷��
							CTutorial::SetState(CTutorial::STATE_STAMINA);
						}
						else
						{ // �����o������L�ȊO�̏ꍇ

							// ��Ԃ�ݒ肷��
							CTutorial::SetState(CTutorial::STATE_NONE);
						}
					}
					else
					{ // ��L�ȊO

						// ���k��Ԃɐݒ肷��
						pBalloon->SetState(CBalloon::STATE_SMALL);
					}
				}
			}
			
			// ���̃I�u�W�F�N�g���擾����
			pObj = pObj->GetNext();
		}
	}
}

//===============================
// �e�ƃu���b�N�̓����蔻��
//===============================
bool collision::BulletBlockCollision(CBullet bullet)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = bullet.GetPos();			// �ʒu
	D3DXVECTOR3 posOld = bullet.GetPosOld();	// �O��̈ʒu
	D3DXVECTOR3 size = bullet.GetSize();		// �T�C�Y

		// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBlock *pBlock = nullptr;	// �I�u�W�F�N�g2D�ւ̃|�C���^

	// �擪�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // ��ނ��u���b�N�̏ꍇ

			// �I�u�W�F�N�g2D�̏����擾����
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // �I�u�W�F�N�g2D�� NULL �̏ꍇ

				if (posOld.y - size.y >= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMax.y &&
					pos.y - size.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
					pos.x - size.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.x + size.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x)
				{ // �O��̈ʒu���u���b�N���ォ�A���݂̈ʒu���u���b�N���������A�u���b�N�̕��̒��������ꍇ(��ɗ�����)

					// true ��Ԃ�
					return true;
				}							//��[�̏���
				else if (posOld.y + size.y <= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMin.y &&
					pos.y + size.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.x - size.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.x + size.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x)
				{ // �O��̈ʒu���u���b�N��艺���A���݂̈ʒu���u���b�N�����ォ�A�u���b�N�̕��̒��������ꍇ(�����Ԃ��鏈��)

					// true ��Ԃ�
					return true;
				}							//���[�̏���
				else if (posOld.x + size.x <= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + size.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.y + size.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y - size.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // �O��̈ʒu���u���b�N��荶���A���݂̈ʒu���u���b�N���E���A�u���b�N�̍����̒��������ꍇ(���̏���)

					// true ��Ԃ�
					return true;
				}							//���[�̏���
				else if (posOld.x - size.x >= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMax.x &&
					pos.x - size.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.y + size.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y - size.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // �O��̈ʒu���u���b�N���E���A���݂̈ʒu���u���b�N���E���A�u���b�N�̍����̒��������ꍇ(�E�̏���)

					// true ��Ԃ�
					return true;
				}						//�E�[�̏���
			}
		}								//��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �e�ƃv���C���[�̓����蔻��
//===============================
bool collision::BulletPlayerCollision(CBullet bullet)
{
	// ���[�J���ϐ���錾
	D3DXVECTOR3 pos = bullet.GetPos();		// �ʒu
	D3DXVECTOR3 size = bullet.GetSize();	// �T�C�Y

		// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CPlayer *pPlayer = nullptr;	// �I�u�W�F�N�g2D�ւ̃|�C���^

	// �I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_PLAYER);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_PLAYER)
		{ // ��ނ��v���C���[�������ꍇ

			// �I�u�W�F�N�g2D�̏����擾����
			pPlayer = dynamic_cast<CPlayer*>(pObj);

			if (pPlayer != nullptr)
			{ // �I�u�W�F�N�g2D�� NULL �̏ꍇ

				if (pos.x + size.x >= pPlayer->GetPos().x - PLAYER_WIDTH &&
					pos.x - size.x <= pPlayer->GetPos().x + PLAYER_WIDTH &&
					pos.y + size.y >= pPlayer->GetPos().y &&
					pos.y - size.y <= pPlayer->GetPos().y + PLAYER_HEIGHT)
				{ // �e���v���C���[�̓����蔻��ɓ�������

					// �v���C���[�̃q�b�g����
					pPlayer->Hit();

					// true ��Ԃ�
					return true;
				}
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �e�ƓG�̓����蔻�菈��
//===============================
bool collision::BulletEnemyCollision(CBullet bullet)
{
	// ���[�J���ϐ���錾
	D3DXVECTOR3 pos = bullet.GetPos();		// �ʒu
	D3DXVECTOR3 size = bullet.GetSize();	// �T�C�Y

		// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CEnemy *pEnemy = nullptr;	// �I�u�W�F�N�g2D�ւ̃|�C���^

	// �擪�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_ENEMY)
		{ // ��ނ��v���C���[�������ꍇ

			// �I�u�W�F�N�g2D�̏����擾����
			pEnemy = dynamic_cast<CEnemy*>(pObj);

			if (pEnemy != nullptr)
			{ // �I�u�W�F�N�g2D�� NULL �̏ꍇ

				if (pos.x + size.x >= pEnemy->GetPos().x - ENEMY_WIDTH &&
					pos.x - size.x <= pEnemy->GetPos().x + ENEMY_WIDTH &&
					pos.y + size.y >= pEnemy->GetPos().y &&
					pos.y - size.y <= pEnemy->GetPos().y + ENEMY_HEIGHT)
				{ // �e���v���C���[�̓����蔻��ɓ�������

					// �v���C���[�̏I������
					pEnemy->Hit();

					// true ��Ԃ�
					return true;
				}
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �e�̓����蔻�菈��
//===============================
void collision::ShadowCollision(const D3DXVECTOR3& pos, int nIdx)
{
	// ���[�J���ϐ��錾
	CShadowCircle* pShadow = CShadowCircle::GetShadow(nIdx);	// �e�̃|�C���^
	CObject* pObj = nullptr;			// �I�u�W�F�N�g�̃|�C���^
	CBlock* pBlock = nullptr;			// �u���b�N�̃|�C���^
	D3DXVECTOR3 posCandi = pos;			// ���̉e�̈ʒu
	float posComp = 0.0f;				// ��r�p�̉e�̈ʒu
	bool bCollision = false;			// �����蔻���

	if (pShadow != nullptr)
	{ // �e�̃|�C���^�� NULL ����Ȃ������ꍇ

		// �I�u�W�F�N�g�̃|�C���^���擾����
		pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�̂� NULL ����Ȃ������

			if (pObj->GetType() == CObject::TYPE_BLOCK)
			{ // ��ނ��u���b�N�̏ꍇ

				// �u���b�N�̏����擾����
				pBlock = dynamic_cast<CBlock*>(pObj);

				if (pBlock != nullptr)
				{ // �u���b�N�̃|�C���^�� NULL ����Ȃ��ꍇ

					if (pShadow->GetPos().x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
						pShadow->GetPos().x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
						pos.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
					{ // �u���b�N�͈͓̔��ɓ����Ă��邩�A�Ώۂ�艺�Ƀu���b�N������ꍇ

						if (bCollision == false)
						{ // �����蔻�肪 false �̏ꍇ

							// �����蔻��� true �ɂ���
							bCollision = true;

							// ���̕ϐ��ɑ������
							posCandi.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y;
						}
						else
						{ // ��L�ȊO

							// ��r�p�̕ϐ��ɑ������
							posComp = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y;

							if (posCandi.y <= posComp)
							{ // ��r�����ʒu�̕������������ꍇ

							  // ���̈ʒu��ݒ肷��
								posCandi.y = posComp;
							}
						}
					}
				}
			}

			// ���̃I�u�W�F�N�g��������
			pObj = pObj->GetNext();
		}

		if (bCollision == false)
		{ // �����蔻�肪 false �̏ꍇ

			// �ʒu��ݒ肷��
			posCandi.y = 0.0f;
		}

		// �ʒu��ݒ肷��
		pShadow->SetPos(posCandi);
	}
}

//===============================
// ��Q���ƃu���b�N�̓����蔻��(���n����)
//===============================
bool collision::ObstacleBlockCollision(CObstacle *pTarget)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = pTarget->GetPos();			// �ʒu
	D3DXVECTOR3 posOld = pTarget->GetPosOld();		// �O��̈ʒu

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBlock *pBlock = nullptr;		// ���f���ւ̃|�C���^

	bool bLand = false;				//���n�������ǂ���

	// �擪�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // ��ނ��u���b�N�̏ꍇ

			// ���f���̏����擾����
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (posOld.y + pTarget->GetFileData().vtxMin.y >= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMax.y &&
					pos.y + pTarget->GetFileData().vtxMin.y < pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
					pos.x + pTarget->GetFileData().vtxMax.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + pTarget->GetFileData().vtxMin.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // �ォ��̓����蔻��

					// �ʒu��␳
					pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y - pTarget->GetFileData().vtxMin.y;

					// ���n�����t�^����
					bLand = true;
				}
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ

	// �ʒu�̐ݒ菈��
	pTarget->SetPos(pos);

	return bLand;			//���n�̕ϐ���Ԃ�
}

//===============================
// ��Q���ƃu���b�N�̓����蔻��(�q�b�g����)
//===============================
bool collision::ObstacleBlockCrush(CObstacle *pTarget)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = pTarget->GetPos();			// �ʒu
	D3DXVECTOR3 posOld = pTarget->GetPosOld();		// �O��̈ʒu

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBlock *pBlock = nullptr;		// ���f���ւ̃|�C���^

	bool bHit = false;			// �����������ǂ���

	// �擪�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // ��ނ��u���b�N�̏ꍇ

			// ���f���̏����擾����
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (posOld.y + pTarget->GetFileData().vtxMax.y <= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMin.y &&
					pos.y + pTarget->GetFileData().vtxMax.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.x + pTarget->GetFileData().vtxMax.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + pTarget->GetFileData().vtxMin.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // ������̓����蔻��

					// �ʒu��␳
					pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y - pTarget->GetFileData().vtxMax.y;

					// �����蔻���t�^����
					bHit = true;
				}
				else if (posOld.x + pTarget->GetFileData().vtxMax.x <= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + pTarget->GetFileData().vtxMax.x >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.y + pTarget->GetFileData().vtxMax.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y + pTarget->GetFileData().vtxMin.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // ������̓����蔻��

					// �ʒu��␳
					pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - pTarget->GetFileData().vtxMax.x;

					// �����蔻���t�^����
					bHit = true;
				}
				else if (posOld.x + pTarget->GetFileData().vtxMin.x >= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMax.x &&
					pos.x + pTarget->GetFileData().vtxMin.x <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.y + pTarget->GetFileData().vtxMax.y >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y + pTarget->GetFileData().vtxMin.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // �E����̓����蔻��

					// �ʒu��␳
					pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + pTarget->GetFileData().vtxMin.x;

					// �����蔻���t�^����
					bHit = true;
				}
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ

	// �ʒu�̐ݒ菈��
	pTarget->SetPos(pos);

	// �����蔻���Ԃ�
	return bHit;
}

//===============================
// ��Q�����m�̓����蔻��
//===============================
bool collision::ObstacleMutualCollision(CObstacle* pTarget)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = pTarget->GetPos();			// �ʒu
	D3DXVECTOR3 posOld = pTarget->GetPosOld();		// �O��̈ʒu
	bool bLand = false;								// ��ɏ�������ǂ���

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;				// �I�u�W�F�N�g�ւ̃|�C���^
	CObstacle *pObstacle = nullptr;		// ���f���ւ̃|�C���^

	// �擪�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // ��ނ���Q���̏ꍇ

			// ���f���̏����擾����
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (pObstacle != pTarget &&
					pObstacle->GetCollision() == true)
				{ // �Ώێ��g����Ȃ����A�����蔻�肪 true �̏ꍇ

					if (posOld.y + pTarget->GetFileData().vtxMin.y >= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMax.y &&
						pos.y + pTarget->GetFileData().vtxMin.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y &&
						pos.x + pTarget->GetFileData().vtxMax.x >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x + pTarget->GetFileData().vtxMin.x <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
					{ // �ォ��̓����蔻��

						// �ʒu��␳
						pos.y = pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y + pTarget->GetFileData().vtxMin.y;

						// ���n�����t�^����
						bLand = true;
					}
				}						// �Ώێ��g����Ȃ����A�����蔻�肪 true �̏ꍇ
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ

	// �ʒu�̐ݒ菈��
	pTarget->SetPos(pos);

	// ���n�����Ԃ�
	return bLand;
}

//===============================
// ��Q���Ɩؔ��̓����蔻��
//===============================
bool collision::ObstacleWoodBoxCollision(CObstacle* pTarget)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = pTarget->GetPos();			// �ʒu
	D3DXVECTOR3 posOld = pTarget->GetPosOld();		// �O��̈ʒu
	bool bHit = false;			// �����������ǂ���

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;				// �I�u�W�F�N�g�ւ̃|�C���^
	CObstacle *pObstacle = nullptr;		// ���f���ւ̃|�C���^

	// �擪�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_OBSTACLE)
		{ // ��ނ���Q���̏ꍇ

			// ���f���̏����擾����
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (pObstacle != pTarget &&
					pObstacle->GetType() == CObstacle::TYPE_WOODBOX)
				{ // �Ώێ��g����Ȃ����A�ؔ��������ꍇ

					if (posOld.y + pTarget->GetFileData().vtxMin.y >= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMax.y &&
						pos.y + pTarget->GetFileData().vtxMin.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y &&
						pos.x + pTarget->GetFileData().vtxMax.x >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x + pTarget->GetFileData().vtxMin.x <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
					{ // �ォ��̓����蔻��

						// �j�Ђ̐ݒ菈��
						CFraction::SetObstacle(pObstacle->GetPos(), pObstacle->GetFileData().m_nTexIdx[0], pObstacle->GetMaterial(0).MatD3D.Diffuse);

						// �j�󉹂�炷
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

						// �ؔ��̏I������
						pObstacle->Uninit();

						// �����蔻���t�^����
						bHit = true;
					}
					else if (posOld.y + pTarget->GetFileData().vtxMax.y <= pObstacle->GetPosOld().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y + pTarget->GetFileData().vtxMax.y >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.x + pTarget->GetFileData().vtxMax.x >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x + pTarget->GetFileData().vtxMin.x <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x)
					{ // ������̓����蔻��

						// �j�Ђ̐ݒ菈��
						CFraction::SetObstacle(pObstacle->GetPos(), pObstacle->GetFileData().m_nTexIdx[0], pObstacle->GetMaterial(0).MatD3D.Diffuse);

						// �j�󉹂�炷
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

						// �ؔ��̏I������
						pObstacle->Uninit();

						// �����蔻���t�^����
						bHit = true;
					}
					else if (posOld.x + pTarget->GetFileData().vtxMax.x <= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMin.x &&
						pos.x + pTarget->GetFileData().vtxMax.x >= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMin.x &&
						pos.y + pTarget->GetFileData().vtxMax.y >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y + pTarget->GetFileData().vtxMin.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
					{ // ������̓����蔻��

						// �j�Ђ̐ݒ菈��
						CFraction::SetObstacle(pObstacle->GetPos(), pObstacle->GetFileData().m_nTexIdx[0], pObstacle->GetMaterial(0).MatD3D.Diffuse);

						// �j�󉹂�炷
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

						// �ؔ��̏I������
						pObstacle->Uninit();

						// �����蔻���t�^����
						bHit = true;
					}
					else if (posOld.x + pTarget->GetFileData().vtxMin.x >= pObstacle->GetPosOld().x + pObstacle->GetFileData().vtxMax.x &&
						pos.x + pTarget->GetFileData().vtxMin.x <= pObstacle->GetPos().x + pObstacle->GetFileData().vtxMax.x &&
						pos.y + pTarget->GetFileData().vtxMax.y >= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMin.y &&
						pos.y + pTarget->GetFileData().vtxMin.y <= pObstacle->GetPos().y + pObstacle->GetFileData().vtxMax.y)
					{ // �E����̓����蔻��

						// �j�Ђ̐ݒ菈��
						CFraction::SetObstacle(pObstacle->GetPos(), pObstacle->GetFileData().m_nTexIdx[0], pObstacle->GetMaterial(0).MatD3D.Diffuse);

						// �j�󉹂�炷
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

						// �ؔ��̏I������
						pObstacle->Uninit();

						// �����蔻���t�^����
						bHit = true;
					}
				}						// �Ώێ��g����Ȃ����A�����蔻�肪 true �̏ꍇ
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ

	// �����蔻���Ԃ�
	return bHit;
}

//===============================
// �S�[���ƃv���C���[�̓����蔻��
//===============================
bool collision::GoalPlayerCollision(CGoal* pGoal)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�̃|�C���^
	CPlayer* pPlayer = nullptr;	// �I�u�W�F�N�g�̃|�C���^
	D3DXVECTOR3 posU, posD;		// ����W�A�����W

	// �擪�̃I�u�W�F�N�g�̏����擾����
	pObj = CObject::GetTop(CObject::PRIORITY_PLAYER);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_PLAYER)
		{ // �v���C���[�̃I�u�W�F�N�g�̏ꍇ

			// �v���C���[�̏����擾����
			pPlayer = dynamic_cast<CPlayer*>(pObj);

			if (pPlayer != nullptr)
			{ // �v���C���[�� NULL �̏ꍇ

				// ���W�̐ݒ�
				posU = D3DXVECTOR3(pGoal->GetPos().x - pGoal->GetSize().x, pGoal->GetPos().y + pGoal->GetSize().y, 0.0f);
				posD = D3DXVECTOR3(pGoal->GetPos().x - pGoal->GetSize().x, pGoal->GetPos().y - pGoal->GetSize().y, 0.0f);

				if (LineOuterProductXY(posU, posD, pPlayer->GetPos()) >= 0)
				{ // �O�ς̓����蔻��ɓ������ꍇ

					// true ��Ԃ�
					return true;
				}
				else
				{ // �O�ς̓����蔻��ɓ����ċ��Ȃ������ꍇ

					// while���𔲂��o��
					break;
				}
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �G�ƃu���b�N�̓����蔻��
//===============================
bool collision::EnemyBlockCollision(CEnemy* pEnemy)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = pEnemy->GetPos();				// �ʒu
	D3DXVECTOR3 posOld = pEnemy->GetPosOld();		// �O��̈ʒu
	bool bLand = false;			// ���n�������ǂ���

	// ���[�J���|�C���^�錾
	CObject *pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBlock *pBlock = nullptr;		// ���f���ւ̃|�C���^

	// �擪�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_BLOCK)
		{ // ��ނ���Q���̏ꍇ

			// ���f���̏����擾����
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // ���f���� NULL ����Ȃ��ꍇ

				if (posOld.y >= pBlock->GetPosOld().y + pBlock->GetFileData().vtxMax.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y &&
					pos.x + ENEMY_WIDTH >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.x - ENEMY_WIDTH <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x)
				{ // �ォ��̓����蔻��

					// �ʒu��␳
					pos.y = pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y + COLLISION_ADD_DIFF_LENGTH;

					// �����蔻���t�^����
					bLand = true;
				}
				else if (posOld.x + ENEMY_WIDTH <= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMin.x &&
					pos.x + ENEMY_WIDTH >= pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x &&
					pos.y + ENEMY_HEIGHT >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // ������̓����蔻��

					// �ʒu��␳
					pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x - ENEMY_WIDTH - COLLISION_ADD_DIFF_LENGTH;
				}
				else if (posOld.x - ENEMY_WIDTH >= pBlock->GetPosOld().x + pBlock->GetFileData().vtxMax.x &&
					pos.x - ENEMY_WIDTH <= pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x &&
					pos.y + ENEMY_HEIGHT >= pBlock->GetPos().y + pBlock->GetFileData().vtxMin.y &&
					pos.y <= pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y)
				{ // �E����̓����蔻��

					// �ʒu��␳
					pos.x = pBlock->GetPos().x + pBlock->GetFileData().vtxMax.x + ENEMY_WIDTH + COLLISION_ADD_DIFF_LENGTH;
				}
			}							// ���f���� NULL ����Ȃ��ꍇ
		}								// ��ނ��u���b�N�̏ꍇ

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}									// �I�u�W�F�N�g���g�p����Ă���ꍇ

	// �ʒu�̐ݒ菈��
	pEnemy->SetPos(pos);

	// ���n�����Ԃ�
	return bLand;
}