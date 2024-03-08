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
// �}�N����`
//===============================
namespace
{
	const float COLLISION_ADD_DIFF_LENGTH = 0.01f;	// �͂��Ȍ덷�𖄂߂邽�߂̃}�N����`(�˂�������h�~)
	const float TREE_RADIUS[CTree::TYPE_MAX] =		// �؂̓����蔻�莞�̔��a
	{
		50.5f,		// �؂̔��a
	};
	
	const float COIN_COLLISION_MAGNI = 3.0f;		// �R�C���̓����蔻��̔{��

	const int DAGGER_DAMAGE = 30;					// �_�K�[�̃_���[�W
	const float DAGGER_KNOCKBACK = 100.0f;			// �_�K�[�̃m�b�N�o�b�N

	const int EXPLOSION_DAMAGE = 30;				// �����̃_���[�W
	const float EXPLOSION_KNOCKBACK = 150.0f;		// �����̃m�b�N�o�b�N

	const int PALM_FRUIT_HEALING = 30;				// ���V�̎��̉񕜒l

	const int COIN_SCORE = 100;						// �R�C���̃X�R�A

	const float SIGNBOARD_ADD_RADIUS = 50.0f;		// �Ŕ̒ǉ��̔��a

	const float ALTER_SURROUND_RADIUS = 800.0f;		// �Ւd�̎��͂̔��a
	
	const float DOOR_ADD_RADIUS = 100.0f;			// �h�A�̒ǉ��̔��a
	const float DOOR_HIT_ANGLE = 0.6f;				// �h�A�̃q�b�g���肪�o�����

	const float BOMB_BULLET_SMASH = 10.0f;			// �e�e�Ŕ��e�̐�����ԑ��x
	const float BOMB_DAGGER_SMASH = 23.0f;			// �_�K�[�Ŕ��e�̐�����ԑ��x
	const float BOMB_SLASH_RIPPLE_SMASH = 10.0f;	// �a���g��Ŕ��e�̐�����ԑ��x
	const int BOSS_DAMAGE = 5;						// �{�X�ւ̃_���[�W

	const float STAGE_RANGE = 8000.0f;				// �X�e�[�W�͈̔�
}

//===============================
// �N���n�ʂ͈̔͊O�̓����蔻��
//===============================
bool collision::ElevOutRangeCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth)
{
	// ���[�J���ϐ��錾
	bool bCollision = false;									// �����蔻���
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// �擪�̏���
	CElevation* pElevEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pElev = list.GetTop();

		// �����̒l���擾����
		pElevEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pElev->GetPos().z + (pElev->GetSize().z * 0.5f) >= pPos->z &&
				pElev->GetPos().z - (pElev->GetSize().z * 0.5f) <= pPos->z &&
				pElev->GetPos().y > pPos->y)
			{ // �N���n�ʂ�艺�ɂ���ꍇ

				if (posOld.x + fWidth <= pElev->GetPos().x - (pElev->GetSize().x * 0.5f) &&
					pPos->x + fWidth >= pElev->GetPos().x - (pElev->GetSize().x * 0.5f))
				{ // ���̓����蔻��

					// �ʒu��ݒ肷��
					pPos->x = pElev->GetPos().x - (pElev->GetSize().x * 0.5f) - fWidth;

					// �����蔻��󋵂� true �ɂ���
					bCollision = true;
				}
				else if (posOld.x - fWidth >= pElev->GetPos().x + (pElev->GetSize().x * 0.5f) &&
					pPos->x - fWidth <= pElev->GetPos().x + (pElev->GetSize().x * 0.5f))
				{ // �E�̓����蔻��

					// �ʒu��ݒ肷��
					pPos->x = pElev->GetPos().x + (pElev->GetSize().x * 0.5f) + fWidth;

					// �����蔻��󋵂� true �ɂ���
					bCollision = true;
				}
			}

			if (pElev == pElevEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pElev = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �����蔻��󋵂�Ԃ�
	return bCollision;
}

//===============================
// �����Ƃ̓����蔻��
//===============================
void collision::CoinCollision(CPlayer* pPlayer, const D3DXVECTOR3 size, const int nAreaIdx)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMaxCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMinCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 vtxMax = size;		// �ő�l
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(size);	// �ŏ��l
	CListManager<CCoin*> list = CCoin::GetList(nAreaIdx);
	CCoin* pCoin = nullptr;			// �擪�̏���
	int nNumData = list.GetNumData();		// �v�f�̑���

	for (int nCntCoin = 0; nCntCoin < nNumData; nCntCoin++)
	{
		// ���̃I�u�W�F�N�g��������
		pCoin = list.GetData(nCntCoin);

		// �R�C���̕ϐ����擾����
		posCoin = pCoin->GetPos();
		vtxMaxCoin = pCoin->GetFileData().vtxMax;
		vtxMinCoin = pCoin->GetFileData().vtxMin;

		if (pCoin->GetState() == CCoin::STATE_NONE &&
			useful::RectangleCollisionXY(posPlayer, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true &&
			useful::RectangleCollisionXZ(posPlayer, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true &&
			useful::RectangleCollisionYZ(posPlayer, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true)
		{ // �R�C���Ƒ̂��d�Ȃ����ꍇ

			// �擾����
			pCoin->Hit();

			if (CGame::GetGameScore() != nullptr)
			{ // �Q�[���X�R�A�� NULL ����Ȃ��ꍇ

				// �R�C�����̃X�R�A�����Z����
				CGame::GetGameScore()->SetScore(CGame::GetGameScore()->GetScore() + COIN_SCORE);

				// �ǉ��X�R�AUI�𐶐�
				CAddScoreUI::Create(posCoin, CAddScoreUI::TYPE_COIN);
			}
		}
	}
}

//===============================
// �G�Əe�̓����蔻��
//===============================
bool collision::EnemyHitToGun(const CBullet& bullet)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posBullet = bullet.GetPos();		// �e�̈ʒu
	D3DXVECTOR3 posOldBullet = bullet.GetPosOld();	// �O��̒e�̈ʒu
	D3DXVECTOR3 bulletMax = D3DXVECTOR3(bullet.GetSize().x, bullet.GetSize().y, bullet.GetSize().x);	// �e�̍ő�l
	D3DXVECTOR3 bulletMin = D3DXVECTOR3(-bullet.GetSize().x, -bullet.GetSize().y, -bullet.GetSize().x);	// �e�̍ŏ��l
	int nDamage = bullet.GetDamage();				// �_���[�W
	float fKnockback = bullet.GetKnockBack();		// �m�b�N�o�b�N�l
	D3DXVECTOR3 enemyMax;		// �G�̍ő�l
	D3DXVECTOR3 enemyMin;		// �G�̍ŏ��l
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;		// �擪�̓G
	CEnemy* pEnemyEnd = nullptr;	// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pEnemy = list.GetTop();

		// �����̒l���擾����
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ�
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
			{ // �G�Əd�Ȃ����ꍇ

				// �q�b�g����
				pEnemy->Hit(nDamage, fKnockback);

				// true ��Ԃ�
				return true;
			}

			if (pEnemy == pEnemyEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pEnemy = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �G�ƃ_�K�[�̓����蔻��
//===============================
bool collision::EnemyHitToDagger(const D3DXVECTOR3& pos, const float fHeight, const float fRadius)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;		// �G�̈ʒu
	D3DXVECTOR3 sizeEnemy = NONE_D3DXVECTOR3;		// �G�̃T�C�Y
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;		// �擪�̓G
	CEnemy* pEnemyEnd = nullptr;	// �����̒l
	int nIdx = 0;
	
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pEnemy = list.GetTop();

		// �����̒l���擾����
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ肷��
			posEnemy = pEnemy->GetPos();				// �G�̈ʒu
			sizeEnemy = pEnemy->GetCollSize();		// �G�̍���

			if (useful::CircleCollisionXZ(pos, posEnemy, fRadius, (sizeEnemy.x + sizeEnemy.z) * 0.5f) == true &&
				pEnemy->GetState() == CEnemy::STATE_NONE &&
				pos.y <= posEnemy.y + sizeEnemy.y &&
				pos.y + fHeight >= posEnemy.y)
			{ // �G�Əd�Ȃ����ꍇ

				switch (pEnemy->GetType())
				{
				case CEnemy::TYPE::TYPE_TORDLE:

					// �q�b�g����
					pEnemy->Hit(DAGGER_DAMAGE, DAGGER_KNOCKBACK);

					break;

				case CEnemy::TYPE::TYPE_IWAKARI:

					// �q�b�g����
					pEnemy->Hit(DAGGER_DAMAGE * DAGGER_DAMAGE, DAGGER_KNOCKBACK);

					break;

				default:

					// ��~
					assert(false);

					break;
				}

				// true ��Ԃ�
				return true;
			}

			if (pEnemy == pEnemyEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pEnemy = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �G�ƃv���C���[�Ƃ̓����蔻��
//===============================
bool collision::EnemyHitToPlayer(CPlayer* pPlayer, const float fRadius, const float fHeight)
{
	// �v���C���[�֌W
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();

	// �G�֌W
	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;		// �G�̈ʒu
	D3DXVECTOR3 sizeEnemy = NONE_D3DXVECTOR3;		// �G�̃T�C�Y
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;		// �擪�̓G
	CEnemy* pEnemyEnd = nullptr;	// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pEnemy = list.GetTop();

		// �����̒l���擾����
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ肷��
			posEnemy = pEnemy->GetPos();			// �G�̈ʒu
			sizeEnemy = pEnemy->GetCollSize();		// �G�̍���

			if (useful::CircleCollisionXZ(posPlayer, posEnemy, fRadius, (sizeEnemy.x + sizeEnemy.z) * 0.5f) == true &&
				posPlayer.y <= posEnemy.y + sizeEnemy.y &&
				posPlayer.y + fHeight >= posEnemy.y)
			{ // �G�Əd�Ȃ����ꍇ

				// �v���C���[�̃q�b�g����
				pPlayer->Hit(10, atan2f(posPlayer.x - posEnemy.x, posPlayer.z - posEnemy.z));

				// true ��Ԃ�
				return true;
			}

			if (pEnemy == pEnemyEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pEnemy = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �G���m�̓����蔻��
//===============================
void collision::EnemyHitToEnemy(CEnemy* pTarget)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posTarget = pTarget->GetPos();
	D3DXVECTOR3 posOldTarget = pTarget->GetPosOld();
	D3DXVECTOR3 vtxMaxTarget = pTarget->GetCollSize();
	D3DXVECTOR3 vtxMinTarget = useful::VtxMinConv(vtxMaxTarget);

	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;		// �G�̈ʒu
	D3DXVECTOR3 posOldEnemy = NONE_D3DXVECTOR3;		// �G�̑O��̈ʒu
	D3DXVECTOR3 vtxMaxEnemy = NONE_D3DXVECTOR3;		// �G�̒��_�̍ő�l
	D3DXVECTOR3 vtxMinEnemy = NONE_D3DXVECTOR3;		// �G�̒��_�̍ŏ��l
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;		// �擪�̓G
	CEnemy* pEnemyEnd = nullptr;	// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pEnemy = list.GetTop();

		// �����̒l���擾����
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pEnemy != pTarget)
			{ // �G��������ꍇ

				// �G�֌W�̕ϐ���ݒ肷��
				posEnemy = pEnemy->GetPos();					// �G�̈ʒu
				posOldEnemy = pEnemy->GetPosOld();				// �G�̑O��̈ʒu
				vtxMaxEnemy = pEnemy->GetCollSize();			// �G�̒��_�̍ő�l
				vtxMinEnemy = useful::VtxMinConv(vtxMaxEnemy);	// �G�̒��_�̍ŏ��l

				// �Z�ʑ̂̓����蔻��
				HexahedronCollision(&posTarget, posEnemy, posOldTarget, posOldEnemy, vtxMinTarget, vtxMinEnemy, vtxMaxTarget, vtxMaxEnemy);
			}

			if (pEnemy == pEnemyEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pEnemy = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �ʒu��K�p����
	pTarget->SetPos(posTarget);
}

//===============================
// ���̍��Ƃ̓����蔻��
//===============================
void collision::GoldBoneCollision(const CPlayer& pPlayer, const D3DXVECTOR3& size)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = pPlayer.GetPos();
	D3DXVECTOR3 posBone = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMaxBone = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMinBone = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMax = size;		// �ő�l
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(size);	// �ŏ��l
	CListManager<CGoldBone*> list = CGoldBone::GetList();
	CGoldBone* pBone = nullptr;		// �擪�̒l
	CGoldBone* pBoneEnd = nullptr;	// �����̒l
	int nIdx = 0;
	
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBone = list.GetTop();

		// �����̒l���擾����
		pBoneEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pBone->GetState() == CGoldBone::STATE_NONE)
			{ // �ʏ��Ԃ̏ꍇ

				// ���̍��̕ϐ����擾����
				posBone = pBone->GetPos();
				vtxMaxBone = pBone->GetFileData().vtxMax;
				vtxMinBone = pBone->GetFileData().vtxMin;

				if (useful::RectangleCollisionXY(pos, posBone, vtxMax, vtxMaxBone, vtxMin, vtxMinBone) == true &&
					useful::RectangleCollisionXZ(pos, posBone, vtxMax, vtxMaxBone, vtxMin, vtxMinBone) == true &&
					useful::RectangleCollisionYZ(pos, posBone, vtxMax, vtxMaxBone, vtxMin, vtxMinBone) == true)
				{ // ���̍��Əd�Ȃ����ꍇ

					// �擾����
					pBone->Hit();

					if (pPlayer.GetGoldBoneUI() != nullptr)
					{ // ���̍�UI�� NULL ����Ȃ��ꍇ

						// ���̍��擾����
						pPlayer.GetGoldBoneUI()->GetGoldBone();
					}
				}
			}

			if (pBone == pBoneEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBone = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

//===============================
// �؂̓����蔻��
//===============================
bool collision::TreeCollision(D3DXVECTOR3* pos, const float fRadius, const int nAreaIdx)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posTree = NONE_D3DXVECTOR3;			// �؂̈ʒu
	float fObjectRadius;							// ���a
	bool bCollision = false;						// �����蔻���
	CListManager<CTree*> list = CTree::GetList(nAreaIdx);
	CTree* pTree = nullptr;		// �擪�̒l
	CTree* pTreeEnd = nullptr;	// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pTree = list.GetTop();

		// �����̒l���擾����
		pTreeEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			posTree = pTree->GetPos();		// �؂̈ʒu
			fObjectRadius = fRadius + TREE_RADIUS[pTree->GetType()];	// ���a

			if (useful::CylinderCollision(pos, posTree, fObjectRadius) == true)
			{ // �~���̓����蔻��ɓ������ꍇ

				// �����蔻��󋵂� true �ɂ���
				bCollision = true;
			}

			if (pTree == pTreeEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pTree = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �����蔻��� ��Ԃ�
	return bCollision;
}

//===============================
// �؂ւ̍U�����菈��
//===============================
void collision::TreeAttack(const CPlayer& pPlayer, const float fRadius, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posTree = NONE_D3DXVECTOR3;			// �؂̈ʒu
	D3DXVECTOR3 posPlayer = pPlayer.GetPos();		// �v���C���[�̈ʒu
	CListManager<CTree*> list = CTree::GetList(pPlayer.GetAreaIdx());
	CTree* pTree = nullptr;		// �擪�̒l
	CTree* pTreeEnd = nullptr;	// �����̒l
	int nIdx = 0;
	
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pTree = list.GetTop();

		// �����̒l���擾����
		pTreeEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �؂̈ʒu��ݒ肷��
			posTree = pTree->GetPos();

			if (useful::CircleCollisionXZ(posPlayer, posTree, fRadius, TREE_RADIUS[pTree->GetType()]) &&
				posPlayer.y <= posTree.y + pTree->GetFileData().vtxMax.y &&
				posPlayer.y + fHeight >= posTree.y)
			{ // �_�K�[���؂ɐڐG�����ꍇ

				// �q�b�g����
				pTree->Hit();
			}

			if (pTree == pTreeEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pTree = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

//===============================
// ���V�̖؂Ƃ̓����蔻��
//===============================
void collision::PalmFruitHit(CPlayer* pPlayer, const float fRadius, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posFruit = NONE_D3DXVECTOR3;		// ���̈ʒu
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// �v���C���[�̈ʒu
	float fRadiusFruit = 0.0f;						// ���a

	CListManager<CPalmFruit*> list = CPalmFruit::GetList();
	CPalmFruit* pFruit = nullptr;		// �擪�̒l
	CPalmFruit* pFruitEnd = nullptr;	// �����̒l
	int nIdx = 0;
	
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pFruit = list.GetTop();

		// �����̒l���擾����
		pFruitEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			posFruit = pFruit->GetPos();					// ���̈ʒu
			fRadiusFruit = pFruit->GetFileData().vtxMax.x;	// ���a

			if (posPlayer.y <= posFruit.y + pFruit->GetFileData().vtxMax.y &&
				posPlayer.y + fHeight >= posFruit.y &&
				useful::CircleCollisionXZ(posPlayer, posFruit, fRadius, fRadiusFruit) == true &&
				pPlayer->GetState().state != CPlayer::STATE_DEATH &&
				pFruit->GetState() == CPalmFruit::STATE_STOP)
			{ // �~���̓����蔻��ɓ������ꍇ

				// �񕜏���
				pPlayer->Healing(PALM_FRUIT_HEALING);

				// �q�b�g����
				pFruit->Hit();
			}

			if (pFruit == pFruitEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pFruit = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

//===============================
// ���V�̎��ւ̍U������
//===============================
bool collision::PalmFruitAttack(const D3DXVECTOR3& pos, const float fRadius, const int nAreaIdx)
{
	// ���[�J���ϐ��錾
	CPalmFruit* pFruit = nullptr;	// ���V�̎��̏��
	D3DXVECTOR3 posFruit = NONE_D3DXVECTOR3;	// ���̈ʒu
	float fRadiusFruit = 0.0f;		// ���̔��a

	CListManager<CTree*> list = CTree::GetList(nAreaIdx);
	CTree* pTree = nullptr;			// �擪�̒l
	CTree* pTreeEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pTree = list.GetTop();

		// �����̒l���擾����
		pTreeEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pTree->GetType() == CTree::TYPE::TYPE_PALM)
			{
				// ���V�̎����擾����
				pFruit = pTree->GetFruit();

				if (pFruit != nullptr)
				{ // ���V�̎��� NULL ����Ȃ��ꍇ

					posFruit = pFruit->GetPos();					// ���̈ʒu
					fRadiusFruit = pFruit->GetFileData().vtxMax.x;	// ���̔��a

					if (useful::CircleCollisionXY(pos, posFruit, fRadius, fRadiusFruit) == true &&
						useful::CircleCollisionXZ(pos, posFruit, fRadius, fRadiusFruit) == true &&
						useful::CircleCollisionYZ(pos, posFruit, fRadius, fRadiusFruit) == true)
					{ // ���V�̎��ɓ��������ꍇ

						// �q�b�g����
						pTree->Hit();

						// ���̐�̏������s��Ȃ�
						return true;
					}
				}
			}

			if (pTree == pTreeEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pTree = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// ��Ƃ̓����蔻��
//===============================
bool collision::RockCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const float fRadius, const float fHeight, const int nAreaIdx, float* fGravity, bool* bJump)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posRock = NONE_D3DXVECTOR3;		// ��̈ʒu
	float fRadiusRock = 0.0f;					// ��̔��a
	float fTopRock = 0.0f;						// ��̏�̍���
	float fBottomRock = 0.0f;					// ��̉��̍���
	bool bCollision = false;					// �����蔻���

	CListManager<CRock*> list = CRock::GetList(nAreaIdx);
	CRock* pRock = nullptr;			// �擪�̒l
	CRock* pRockEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pRock = list.GetTop();

		// �����̒l���擾����
		pRockEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// ��̈ʒu���擾����
			posRock = pRock->GetPos();

			// ��̔��a���擾����
			fRadiusRock = pRock->GetRadius();

			if (useful::CircleCollisionXZ(*pos, posRock, fRadius, fRadiusRock) == true)
			{ // �~���ɓ������ꍇ

				// ��̏�̍������擾����
				fTopRock = pRock->GetTopHeight();

				// ��̉��̍������擾����
				fBottomRock = pRock->GetBottomHeight();

				if (posRock.y + fTopRock <= posOld.y &&
					posRock.y + fTopRock >= pos->y)
				{ // ��ɂԂ������ꍇ

					// �ʒu��ݒ肷��
					pos->y = posRock.y + fTopRock + COLLISION_ADD_DIFF_LENGTH;

					if (bJump != nullptr)
					{ // �W�����v�󋵂� NULL ����Ȃ��ꍇ

						// �W�����v�󋵂� false �ɂ���
						*bJump = false;
					}

					if (fGravity != nullptr)
					{ // �d�͂� NULL ����Ȃ��ꍇ

						// �d�͂�0�ɂ���
						*fGravity = 0.0f;
					}

					// �����蔻��󋵂� true �ɂ���
					bCollision = true;
				}
				else if (posRock.y + fBottomRock >= posOld.y + fHeight &&
					posRock.y + fBottomRock <= pos->y + fHeight)
				{ // ���ɂԂ������ꍇ

					// �ʒu��ݒ肷��
					pos->y = posRock.y + fBottomRock - (fHeight + COLLISION_ADD_DIFF_LENGTH);

					// �����蔻��󋵂� true �ɂ���
					bCollision = true;
				}
				else if (pos->y <= posRock.y + fTopRock &&
					pos->y + fHeight >= posRock.y + fBottomRock &&
					useful::CylinderCollision(pos, posRock, fRadiusRock + fRadius) == true)
				{ // ���������ꍇ

					// �����蔻��󋵂� true �ɂ���
					bCollision = true;
				}
			}

			if (pRock == pRockEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pRock = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �����蔻��󋵂�Ԃ�
	return bCollision;
}

//===============================
// ���e�̃q�b�g����
//===============================
bool collision::BangFlowerHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posBomb = NONE_D3DXVECTOR3;		// ���e�̈ʒu
	float fRadiusBomb = 0.0f;					// ���e��
	float fHeightBomb = 0.0f;					// 
	CListManager<CBangFlower*> list = CBangFlower::GetList();
	CBangFlower* pBomb = nullptr;			// �擪�̒l
	CBangFlower* pBombEnd = nullptr;		// �����̒l
	int nIdx = 0;
	bool bHit = false;		// �q�b�g����

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBomb = list.GetTop();

		// �����̒l���擾����
		pBombEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pBomb->GetBomb() != nullptr &&
				pBomb->GetBomb()->GetState() == CBomb::STATE_RIPEN)
			{ // ���e�������Ԃ̏ꍇ

				// ���e�̈ʒu���擾����
				posBomb = pBomb->GetPos();

				// ���e�̔��a���擾����
				fRadiusBomb = pBomb->GetFileData().fRadius;

				// ���e�̍������擾����
				fHeightBomb = pBomb->GetFileData().vtxMax.y;

				if (useful::CircleCollisionXZ(pos, posBomb, fRadius, fRadiusBomb) == true &&
					pos.y + fHeight >= posBomb.y &&
					pos.y <= posBomb.y + fHeightBomb)
				{ // �͈͓��ɂ����ꍇ

					// �q�b�g����
					pBomb->Hit();

					// �q�b�g����
					bHit = true;
				}
			}

			if (pBomb == pBombEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBomb = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �q�b�g�����Ԃ�
	return bHit;
}

//===============================
// ���e�̃q�b�g����
//===============================
bool collision::BombHitToGun(const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld, const float fRadius)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posBomb = NONE_D3DXVECTOR3;		// ���e�̈ʒu
	float fRadiusBomb = 0.0f;					// ���e�̔��a
	float fRot = atan2f(pos.x - posOld.x, pos.z - posOld.z);	// ������Ԍ���

	CListManager<CBomb*> list = CBomb::GetList();
	CBomb* pBomb = nullptr;			// �擪�̒l
	CBomb* pBombEnd = nullptr;		// �����̒l
	int nIdx = 0;
	bool bHit = false;		// �q�b�g����

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBomb = list.GetTop();

		// �����̒l���擾����
		pBombEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pBomb->GetState() == CBomb::STATE_DETONATION)
			{ // ���e���N����Ԃ������ꍇ

				// ���e�̈ʒu���擾����
				posBomb = pBomb->GetPos();
				posBomb.y += (pBomb->GetFileData().vtxMax.y * 0.5f);

				// ���e�̍ő�l���擾����
				fRadiusBomb = pBomb->GetFileData().fRadius;

				if (useful::CircleCollisionXY(pos, posBomb, fRadius, fRadiusBomb) == true &&
					useful::CircleCollisionYZ(pos, posBomb, fRadius, fRadiusBomb) == true &&
					useful::CircleCollisionXZ(pos, posBomb, fRadius, fRadiusBomb) == true)
				{ // �͈͓��ɂ����ꍇ

					// �q�b�g����
					pBomb->Hit(fRot, BOMB_BULLET_SMASH);

					// �q�b�g����
					bHit = true;
				}
			}

			if (pBomb == pBombEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBomb = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �q�b�g�����Ԃ�
	return bHit;
}

//===============================
// ���e�̃q�b�g����(�_�K�[)
//===============================
bool collision::BombHitToDagger(const D3DXVECTOR3& pos, const float fRadius, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posBomb = NONE_D3DXVECTOR3;		// ���e�̈ʒu
	float fRadiusBomb = 0.0f;	// ���e�̔��a
	float fHeightBomb = 0.0f;	// ���e�̍���
	float fRot = 0.0f;			// ������Ԍ���

	CListManager<CBomb*> list = CBomb::GetList();
	CBomb* pBomb = nullptr;			// �擪�̒l
	CBomb* pBombEnd = nullptr;		// �����̒l
	int nIdx = 0;
	bool bHit = false;		// �q�b�g����

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBomb = list.GetTop();

		// �����̒l���擾����
		pBombEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pBomb->GetState() == CBomb::STATE_DETONATION)
			{ // ���e���N����Ԃ������ꍇ

				// ���e�֌W�̏����擾����
				posBomb = pBomb->GetPos();
				fRadiusBomb = pBomb->GetFileData().fRadius;
				fHeightBomb = pBomb->GetFileData().vtxMax.y;

				if (useful::CircleCollisionXZ(pos, posBomb, fRadius, fRadiusBomb) == true &&
					pos.y + fHeight >= posBomb.y &&
					pos.y <= posBomb.y + fHeightBomb)
				{ // �͈͓��ɂ����ꍇ

					// �������Z�o����
					fRot = atan2f(posBomb.x - pos.x, posBomb.z - pos.z);

					// �q�b�g����
					pBomb->Hit(fRot, BOMB_DAGGER_SMASH);

					// �q�b�g����
					bHit = true;
				}
			}

			if (pBomb == pBombEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBomb = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �q�b�g�����Ԃ�
	return bHit;
}

//===============================
// ���e�̃q�b�g����(�a���g��)
//===============================
bool collision::BombHitToSlashRipple(const D3DXVECTOR3& pos, const float fRadius, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posBomb = NONE_D3DXVECTOR3;		// ���e�̈ʒu
	float fRadiusBomb = 0.0f;	// ���e�̔��a
	float fHeightBomb = 0.0f;	// ���e�̍���
	float fRot = 0.0f;			// ������Ԍ���

	CListManager<CBomb*> list = CBomb::GetList();
	CBomb* pBomb = nullptr;			// �擪�̒l
	CBomb* pBombEnd = nullptr;		// �����̒l
	int nIdx = 0;
	bool bHit = false;		// �q�b�g����

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBomb = list.GetTop();

		// �����̒l���擾����
		pBombEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pBomb->GetState() == CBomb::STATE_DETONATION)
			{ // ���e���N����Ԃ������ꍇ

				// ���e�֌W�̏����擾����
				posBomb = pBomb->GetPos();
				fRadiusBomb = pBomb->GetFileData().fRadius;
				fHeightBomb = pBomb->GetFileData().vtxMax.y;

				if (useful::CircleCollisionXZ(pos, posBomb, fRadius, fRadiusBomb) == true &&
					pos.y + fHeight >= posBomb.y &&
					pos.y <= posBomb.y + fHeightBomb)
				{ // �͈͓��ɂ����ꍇ

					// �������Z�o����
					fRot = atan2f(posBomb.x - pos.x, posBomb.z - pos.z);

					// �q�b�g����
					pBomb->Hit(fRot, BOMB_SLASH_RIPPLE_SMASH);

					// �q�b�g����
					bHit = true;
				}
			}

			if (pBomb == pBombEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBomb = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �q�b�g�����Ԃ�
	return bHit;
}

//===============================
// �����Ɗ�Ƃ̓����蔻��
//===============================
void collision::ExplosionHitToRock(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, const int nAreaIdx)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posRock = NONE_D3DXVECTOR3;		// ��̈ʒu
	float fRadiusRock = 0.0f;					// ��̔��a
	float fTopRock = 0.0f;						// ��̏�̍���
	float fBottomRock = 0.0f;					// ��̉��̍���

	CListManager<CRock*> list = CRock::GetList(nAreaIdx);
	CRock* pRock = nullptr;			// �擪�̒l
	CRock* pRockEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pRock = list.GetTop();

		// �����̒l���擾����
		pRockEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pRock->GetType() == CRock::TYPE_BREAK)
			{ // �j��ł����̏ꍇ

				// ��̈ʒu���擾����
				posRock = pRock->GetPos();

				// ��̔��a���擾����
				fRadiusRock = pRock->GetRadius();

				// ��̏�̍������擾����
				fTopRock = pRock->GetTopHeight();

				// ��̉��̍������擾����
				fBottomRock = pRock->GetBottomHeight();

				if (useful::CircleCollisionXZ(pos, posRock, fRadius, fRadiusRock) == true &&
					pos.y <= posRock.y + fTopRock &&
					pos.y + fHeight >= posRock.y + fBottomRock)
				{ // �͈͓��ɂ����ꍇ

					// �j�󏈗�
					pRock->Break();
				}
			}

			if (pRock == pRockEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pRock = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

//===============================
// �����ƓG�Ƃ̓����蔻��
//===============================
void collision::ExplosionHitToEnemy(const D3DXVECTOR3& pos, const float fRadius, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;	// �G�̈ʒu
	float fRadiusEnemy = 0.0f;					// �G�̔��a
	float fHeightEnemy = 0.0f;					// �G�̍���

	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;			// �擪�̒l
	CEnemy* pEnemyEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pEnemy = list.GetTop();

		// �����̒l���擾����
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pEnemy->GetState() == CEnemy::STATE_NONE)
			{ // �ʏ��Ԃ̏ꍇ

				// �G�֌W�̕ϐ����擾����
				posEnemy = pEnemy->GetPos();
				fRadiusEnemy = pEnemy->GetCollSize().x;
				fHeightEnemy = pEnemy->GetCollSize().y;

				if (useful::CircleCollisionXZ(pos, posEnemy, fRadius, fRadiusEnemy) == true &&
					pos.y <= posEnemy.y + fHeightEnemy &&
					pos.y + fHeight >= posEnemy.y)
				{ // �͈͓��ɂ����ꍇ

					// �j�󏈗�
					pEnemy->Hit(EXPLOSION_DAMAGE, EXPLOSION_KNOCKBACK);
				}
			}

			if (pEnemy == pEnemyEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pEnemy = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

//===============================
// �����ƃv���C���[�Ƃ̓����蔻��
//===============================
bool collision::ExplosionHitToPlayer(CPlayer* pPlayer, const float fRadius, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = pPlayer->GetPos();	// �v���C���[�̈ʒu
	D3DXVECTOR3 posExpl = NONE_D3DXVECTOR3;	// �����̈ʒu
	float fRadiusExpl = 0.0f;				// �����̔��a
	float fHeightExpl = 0.0f;				// �����̍���
	float fRotSmash = 0.0f;					// ������ԕ���

	CListManager<CBombExplosion*> list = CBombExplosion::GetList();
	CBombExplosion* pBombExplosion = nullptr;			// �擪�̒l
	CBombExplosion* pBombExplosionEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBombExplosion = list.GetTop();

		// �����̒l���擾����
		pBombExplosionEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �����֌W�̕ϐ����擾����
			posExpl = pBombExplosion->GetPos();
			fRadiusExpl = pBombExplosion->GetCircum();
			fHeightExpl = pBombExplosion->GetHeight();

			if (useful::CircleCollisionXZ(pos, posExpl, fRadius, fRadiusExpl) == true &&
				pos.y <= posExpl.y + fHeightExpl &&
				pos.y + fHeight >= posExpl.y - fHeightExpl)
			{ // �͈͓��ɂ����ꍇ

				// ������Ԍ������Z�o����
				fRotSmash = atan2f(pos.x - posExpl.x, pos.z - posExpl.z);

				// �v���C���[�̃q�b�g����
				pPlayer->Hit(EXPLOSION_DAMAGE, fRotSmash);

				// true ��Ԃ�
				return true;
			}

			if (pBombExplosion == pBombExplosionEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBombExplosion = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �ǂƂ̓����蔻��
//===============================
bool collision::WallCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posWall = NONE_D3DXVECTOR3;			// �ǂ̈ʒu
	D3DXVECTOR3 posOldWall = NONE_D3DXVECTOR3;		// �ǂ̑O��̈ʒu
	D3DXVECTOR3 vtxMaxWall = NONE_D3DXVECTOR3;		// �ǂ̍ő�l
	D3DXVECTOR3 vtxMinWall = NONE_D3DXVECTOR3;		// �ǂ̍ŏ��l

	CListManager<CWall*> list = CWall::GetList(nAreaIdx);
	CWall* pWall = nullptr;			// �擪�̒l
	CWall* pWallEnd = nullptr;		// �����̒l
	int nIdx = 0;
	bool bHit = false;		// �q�b�g����

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pWall = list.GetTop();

		// �����̒l���擾����
		pWallEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �ǂ̒��_���擾����
			vtxMaxWall = pWall->GetVtxMax();
			vtxMinWall = pWall->GetVtxMin();

			// �ǂ̈ʒu���擾����
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
			{ // �ǂɓ��������ꍇ

				// �q�b�g����
				bHit = true;
			}

			if (pWall == pWallEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pWall = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �q�b�g�����Ԃ�
	return bHit;
}

//===============================
// �u���b�N�Ƃ̃q�b�g����
//===============================
bool collision::BlockHit(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posBlock = NONE_D3DXVECTOR3;		// �ǂ̈ʒu
	D3DXVECTOR3 posOldBlock = NONE_D3DXVECTOR3;		// �ǂ̑O��̈ʒu
	D3DXVECTOR3 vtxMaxBlock = NONE_D3DXVECTOR3;		// �ǂ̍ő�l
	D3DXVECTOR3 vtxMinBlock = NONE_D3DXVECTOR3;		// �ǂ̍ŏ��l

	CListManager<CBlock*> list = CBlock::GetList(nAreaIdx);
	CBlock* pBlock = nullptr;			// �擪�̒l
	CBlock* pBlockEnd = nullptr;		// �����̒l
	int nIdx = 0;
	bool bHit = false;		// �q�b�g����

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBlock = list.GetTop();

		// �����̒l���擾����
		pBlockEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �ǂ̒��_���擾����
			vtxMaxBlock = pBlock->GetVtxMax();
			vtxMinBlock = pBlock->GetVtxMin();

			// �ǂ̈ʒu���擾����
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
			{ // �ǂɓ��������ꍇ

				// �q�b�g����
				bHit = true;
			}

			if (pBlock == pBlockEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBlock = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �q�b�g�����Ԃ�
	return bHit;
}

//===============================
// �{�X�̓����蔻��
//===============================
bool collision::BossHit(const D3DXVECTOR3& pos, const float fRadius, const int nDamage)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posPart = NONE_D3DXVECTOR3;
	CBossCollision* coll = nullptr;
	D3DXMATRIX mtxScale, mtxRot, mtxTrans, mtx, mtxColl;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxWorld;		// �}�g���b�N�X���擾����

	CListManager<CBoss*> list = CBoss::GetList();
	CBoss* pBoss = nullptr;			// �擪�̒l
	CBoss* pBossEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBoss = list.GetTop();

		// �����̒l���擾����
		pBossEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// �g�嗦�𔽉f
			D3DXMatrixScaling(&mtxScale, pBoss->GetScale().x, pBoss->GetScale().y, pBoss->GetScale().z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->GetRot().y + D3DX_PI, pBoss->GetRot().x, pBoss->GetRot().z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pBoss->GetPos().x, pBoss->GetPos().y, pBoss->GetPos().z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			for (int nCntPart = 0; nCntPart < pBoss->GetNumModel(); nCntPart++)
			{
				// �}�g���b�N�X�̌v�Z����
				pBoss->GetHierarchy(nCntPart)->MatrixCalc(&mtx, mtxWorld);

				// �����蔻��̏����擾
				coll = pBoss->GetColl(nCntPart);

				if (coll != nullptr)
				{ // �����蔻�肪 NULL ����Ȃ��ꍇ

					for (int nCntColl = 0; nCntColl < coll->GetNumColl(); nCntColl++)
					{
						// �}�g���b�N�X�̏�����
						D3DXMatrixIdentity(&mtxColl);

						// �ʒu�𔽉f
						D3DXMatrixTranslation(&mtxTrans, coll->GetCollOffset(nCntColl).x, coll->GetCollOffset(nCntColl).y, coll->GetCollOffset(nCntColl).z);
						D3DXMatrixMultiply(&mtxColl, &mtxColl, &mtxTrans);

						// �Z�o�����u�p�[�c�̃��[���h�}�g���b�N�X�v�Ɓu�e�̃}�g���b�N�X�v���|�����킹��
						D3DXMatrixMultiply
						(
							&mtxColl,
							&mtxColl,
							&mtx
						);

						// �ʒu��ݒ肷��
						posPart.x = mtxColl._41;
						posPart.y = mtxColl._42;
						posPart.z = mtxColl._43;

						if (useful::CircleCollisionXY(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true &&
							useful::CircleCollisionYZ(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true &&
							useful::CircleCollisionXZ(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true)
						{ // �������������ꍇ

							if (coll->IsWeakness(nCntColl) == true)
							{ // ��_�������ꍇ

								// �o���A�j�󏈗�
								pBoss->BarrierBreak(pos, nCntPart, nDamage);
							}
							else
							{ // ��L�ȊO

								// �q�b�g����
								pBoss->Hit(BOSS_DAMAGE);
							}

							// true ��Ԃ�
							return true;
						}
					}
				}
			}

			if (pBoss == pBossEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBoss = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �{�X�Ƃ̃q�b�g����
//===============================
bool collision::BossAttack(const D3DXVECTOR3& pos, const float fRadius, float* fRotSmash)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posPart = NONE_D3DXVECTOR3;
	CBossCollision* coll = nullptr;
	D3DXMATRIX mtxScale, mtxRot, mtxTrans, mtx, mtxColl;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxWorld;		// �}�g���b�N�X���擾����

	CListManager<CBoss*> list = CBoss::GetList();
	CBoss* pBoss = nullptr;			// �擪�̒l
	CBoss* pBossEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBoss = list.GetTop();

		// �����̒l���擾����
		pBossEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pBoss->IsHit() == true)
			{ // �q�b�g�󋵂� true �̏ꍇ

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxWorld);

				// �g�嗦�𔽉f
				D3DXMatrixScaling(&mtxScale, pBoss->GetScale().x, pBoss->GetScale().y, pBoss->GetScale().z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

				// �����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->GetRot().y + D3DX_PI, pBoss->GetRot().x, pBoss->GetRot().z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, pBoss->GetPos().x, pBoss->GetPos().y, pBoss->GetPos().z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

				for (int nCntPart = 0; nCntPart < pBoss->GetNumModel(); nCntPart++)
				{
					// �}�g���b�N�X�̌v�Z����
					pBoss->GetHierarchy(nCntPart)->MatrixCalc(&mtx, mtxWorld);

					// �����蔻��̏����擾
					coll = pBoss->GetColl(nCntPart);

					if (coll != nullptr)
					{ // �����蔻�肪 NULL ����Ȃ��ꍇ

						for (int nCntColl = 0; nCntColl < coll->GetNumColl(); nCntColl++)
						{
							// �}�g���b�N�X�̏�����
							D3DXMatrixIdentity(&mtxColl);

							// �ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, coll->GetCollOffset(nCntColl).x, coll->GetCollOffset(nCntColl).y, coll->GetCollOffset(nCntColl).z);
							D3DXMatrixMultiply(&mtxColl, &mtxColl, &mtxTrans);

							// �Z�o�����u�p�[�c�̃��[���h�}�g���b�N�X�v�Ɓu�e�̃}�g���b�N�X�v���|�����킹��
							D3DXMatrixMultiply
							(
								&mtxColl,
								&mtxColl,
								&mtx
							);

							// �ʒu��ݒ肷��
							posPart.x = mtxColl._41;
							posPart.y = mtxColl._42;
							posPart.z = mtxColl._43;

							if (useful::CircleCollisionXY(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true &&
								useful::CircleCollisionYZ(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true &&
								useful::CircleCollisionXZ(pos, posPart, fRadius, coll->GetRadius(nCntColl)) == true)
							{ // �������������ꍇ

								if (fRotSmash != nullptr)
								{ // ������Ԍ����� NULL ����Ȃ��ꍇ

									// ������Ԍ�����ݒ肷��
									*fRotSmash = atan2f(pos.x - posPart.x, pos.z - posPart.z);
								}

								// true ��Ԃ�
								return true;
							}
						}
					}
				}
			}

			if (pBoss == pBossEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBoss = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �Ւd�Ƃ̓����蔻��
//===============================
bool collision::AlterCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin)
{
	// �����蔻��̕ϐ���錾
	CAlter* pAlter = CGame::GetAlter();
	CAlterPole* pPole = nullptr;
	D3DXVECTOR3 posAlter;
	D3DXVECTOR3 posOldAlter;
	D3DXVECTOR3 vtxMinAlter;
	D3DXVECTOR3 vtxMaxAlter;

	if (pAlter != nullptr &&
		pAlter->GetState() != CAlter::STATE_BREAK)
	{ // �Ւd�����Ă��Ȃ��ꍇ

		// �ʒu�ƃT�C�Y���擾����
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
		{ // �Ւd�ɓ��������ꍇ

			// true ��Ԃ�
			return true;
		}

		for (int nCntPole = 0; nCntPole < CAlter::NUM_POLE; nCntPole++)
		{
			// �Β��̏����擾����
			pPole = pAlter->GetPole(nCntPole);

			if (pPole != nullptr)
			{ // �Β��� NULL ����Ȃ��ꍇ

				// �ʒu�ƃT�C�Y���擾����
				posAlter = pPole->GetPos();
				posOldAlter = pPole->GetPosOld();
				vtxMaxAlter = pPole->GetFileData().vtxMax;

				if (useful::CylinderCollision(pos, posAlter, vtxMax.x + vtxMaxAlter.x) == true)
				{ // �Ւd�ɓ��������ꍇ

					// true ��Ԃ�
					return true;
				}
			}
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �Ւd���͂̓����蔻��
//===============================
void collision::AlterSurrounding(const D3DXVECTOR3& pos, const float fRadius)
{
	// �����蔻��̕ϐ���錾
	CAlter* pAlter = CGame::GetAlter();
	D3DXVECTOR3 posAlter = NONE_D3DXVECTOR3;

	if (pAlter != nullptr &&
		pAlter->GetState() == CAlter::STATE_NONE)
	{ // �Ւd�ɍ��������Ă��Ȃ��ꍇ

		// �ʒu�ƃT�C�Y���擾����
		posAlter = pAlter->GetPos();

		if (useful::CircleCollisionXZ(pos,posAlter,fRadius, ALTER_SURROUND_RADIUS))
		{ // �Ւd�̎��͂ɂ����ꍇ

			// ���C�g�_���󋵂� true �ɂ���
			pAlter->SetEnableLightUp(true);

			// ���b�Z�[�W��\������
			pAlter->SetAlterMessage(CAlterMessage::Create(posAlter));
		}
		else
		{ // ��L�ȊO

			// ���C�g�_���󋵂� false �ɂ���
			pAlter->SetEnableLightUp(false);

			// ���b�Z�[�W����������
			pAlter->SetAlterMessage(nullptr);
		}
	}
}

//===============================
// �a���̔g��̓����蔻��
//===============================
bool collision::RippleHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, float* fRotSmash)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posRipple = NONE_D3DXVECTOR3;		// �ǂ̈ʒu
	float fRadiusRipple = 0.0f;						// �g��̔��a
	float fHeightRipple = 0.0f;						// �g��̍���

	CListManager<CSlashRipple*> list = CSlashRipple::GetList();
	CSlashRipple* pRipple = nullptr;			// �擪�̒l
	CSlashRipple* pRippleEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pRipple = list.GetTop();

		// �����̒l���擾����
		pRippleEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �g��֌W�̕ϐ���ݒ肷��
			posRipple = pRipple->GetPos();
			fRadiusRipple = pRipple->GetFileData().fRadius * pRipple->GetScale().x;
			fHeightRipple = pRipple->GetFileData().vtxMax.y;

			if (useful::CircleCollisionXZ(pos, posRipple, fRadius, fRadiusRipple) == true &&
				pos.y <= posRipple.y + fHeightRipple &&
				pos.y + fHeight >= posRipple.y)
			{ // �a���ɓ��������ꍇ

				if (fRotSmash != nullptr)
				{ // ������ NULL ����Ȃ��ꍇ

					// ������ݒ肷��
					*fRotSmash = atan2f(pos.x - posRipple.x, pos.z - posRipple.z);
				}

				// true ��Ԃ�
				return true;
			}

			if (pRipple == pRippleEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pRipple = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// ���U���ƃv���C���[�̓����蔻��
//===============================
bool collision::WindShotHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, float* fSmashRot)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posWind = NONE_D3DXVECTOR3;		// �ǂ̈ʒu
	float fRadiusWind = 0.0f;					// ���̔��a
	float fHeightWind = 0.0f;					// ���̍���

	CListManager<CWindShot*> list = CWindShot::GetList();
	CWindShot* pWind = nullptr;			// �擪�̒l
	CWindShot* pWindEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pWind = list.GetTop();

		// �����̒l���擾����
		pWindEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pWind->GetState() != CWindShot::STATE_DELETE)
			{ // ������ԈȊO

				// ���֌W�̕ϐ���ݒ肷��
				posWind = pWind->GetPos();
				fRadiusWind = pWind->GetCircum() + pWind->GetWidth();
				fHeightWind = pWind->GetHeight() * pWind->GetVortex();

				if (useful::CircleCollisionXZ(pos, posWind, fRadius, fRadiusWind) == true &&
					pos.y <= posWind.y + fHeightWind &&
					pos.y + fHeight >= posWind.y)
				{ // ���U���ɓ��������ꍇ

					if (fSmashRot != nullptr)
					{ // �����̃|�C���^�� NULL ����Ȃ��ꍇ

						// ������Ԍ�����ݒ肷��
						*fSmashRot = atan2f(pos.x - posWind.x, pos.z - posWind.z);
					}

					// true ��Ԃ�
					return true;
				}
			}

			if (pWind == pWindEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pWind = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// ���U���Ƃ̓����蔻��
//===============================
bool collision::FireShotHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, float* fSmashRot)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posFire = NONE_D3DXVECTOR3;		// �ǂ̈ʒu
	float fRadiusFire = 0.0f;					// ���̔��a
	float fHeightFire = 0.0f;					// ���̍���

	CListManager<CFireShot*> list = CFireShot::GetList();
	CFireShot* pFire = nullptr;			// �擪�̒l
	CFireShot* pFireEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pFire = list.GetTop();

		// �����̒l���擾����
		pFireEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// ���֌W�̕ϐ���ݒ肷��
			posFire = pFire->GetPos();
			fRadiusFire = pFire->GetCircum();
			fHeightFire = pFire->GetHeight();

			if (useful::CircleCollisionXZ(pos, posFire, fRadius, fRadiusFire) == true &&
				pos.y <= posFire.y + fHeightFire &&
				pos.y + fHeight >= posFire.y)
			{ // ���U���ɓ��������ꍇ

				if (fSmashRot != nullptr)
				{ // �����̃|�C���^�� NULL ����Ȃ��ꍇ

					// ������Ԍ�����ݒ肷��
					*fSmashRot = atan2f(pos.x - posFire.x, pos.z - posFire.z);
				}

				// true ��Ԃ�
				return true;
			}

			if (pFire == pFireEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pFire = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �ŔƂ̓����蔻��
//===============================
bool collision::SignboardCollision(const D3DXVECTOR3& pos, const float fRadius)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posSign = NONE_D3DXVECTOR3;		// �ǂ̈ʒu
	float fRadiusSign = 0.0f;					// �Ŕ̔��a

	CListManager<CSignboard*> list = CSignboard::GetList();
	CSignboard* pSign = nullptr;			// �擪�̒l
	CSignboard* pSignEnd = nullptr;			// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pSign = list.GetTop();

		// �����̒l���擾����
		pSignEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �Ŕ֌W�̕ϐ���ݒ肷��
			posSign = pSign->GetPos();
			fRadiusSign = pSign->GetFileData().fRadius;

			if (useful::CircleCollisionXZ(pos, posSign, fRadius, fRadiusSign + SIGNBOARD_ADD_RADIUS) == true)
			{ // �Ŕɋ߂Â����ꍇ

				// �{�^����`�悷��
				pSign->GetButton()->SetEnableDisp(true);

				if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
					CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
				{ // �{�^�����������ꍇ

					// �����ڍs����
					pSign->Explain();

					// true ��Ԃ�
					return true;
				}
			}
			else
			{ // ��L�ȊO

				// �{�^����`�悵�Ȃ�
				pSign->GetButton()->SetEnableDisp(false);
			}

			if (pSign == pSignEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pSign = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �I�Ƃ̓����蔻��
//===============================
bool collision::TargetHit(const D3DXVECTOR3& pos, const float fRadius)
{
	// ���[�J���ϐ��錾
	CBalloon* pBalloon = nullptr;				// ���D�̏��
	D3DXVECTOR3 posBalloon = NONE_D3DXVECTOR3;	// ���D�̈ʒu
	float fRadiusBalloon = 0.0f;				// ���D�̔��a
	CListManager<CBalloonSpawner*> list = CBalloonSpawner::GetList();
	CBalloonSpawner* pSpawner = nullptr;		// �擪�̕��D�X�|�i�[
	CBalloonSpawner* pSpawnerEnd = nullptr;		// �����̒l
	int nIdx = 0;

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pSpawner = list.GetTop();

		// �����̒l���擾����
		pSpawnerEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			if (pSpawner->GetBalloon() != nullptr)
			{ // ���D�� NULL ����Ȃ��ꍇ

				// ���D�̏����擾
				pBalloon = pSpawner->GetBalloon();

				// ����ݒ�
				posBalloon = pBalloon->GetPos();
				fRadiusBalloon = pBalloon->GetFileData().fRadius;

				if (useful::CircleCollisionXY(pos, posBalloon, fRadius, fRadiusBalloon) == true &&
					useful::CircleCollisionXZ(pos, posBalloon, fRadius, fRadiusBalloon) == true &&
					useful::CircleCollisionYZ(pos, posBalloon, fRadius, fRadiusBalloon) == true)
				{ // ���D�Əd�Ȃ����ꍇ

					// �q�b�g����
					pSpawner->Hit();

					// true ��Ԃ�
					return true;
				}
			}

			if (pSpawner == pSpawnerEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pSpawner = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �h�A�Ƃ̃q�b�g����
//===============================
bool collision::DoorHit(const D3DXVECTOR3& pos, const float fRadius)
{
	// ���[�J���ϐ��錾
	CDoor* pDoor = CTutorial::GetDoor();		// �h�A�̏��

	if (pDoor != nullptr)
	{ // �h�A�����݂���ꍇ

		// �h�A�̏����擾����
		D3DXVECTOR3 posDoor = pDoor->GetPos();									// �h�A�̈ʒu
		float fRadiusDoor = pDoor->GetFileData().vtxMax.x + DOOR_ADD_RADIUS;	// �h�A�̔��a
		float fAngle = atan2f(pos.x - posDoor.x, pos.z - posDoor.z);			// ����

		if (((fAngle <= -D3DX_PI + DOOR_HIT_ANGLE && fAngle >= -D3DX_PI) ||
			(fAngle >= D3DX_PI - DOOR_HIT_ANGLE && fAngle <= D3DX_PI)) &&
			useful::CircleCollisionXZ(pos, posDoor, fRadius, fRadiusDoor) == true)
		{ // �h�A�̋߂��ɂ����ꍇ

			// �{�^���\����`�悷��
			pDoor->GetButton()->SetEnableDisp(true);

			if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
				CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
			{ // �{�^�����������ꍇ

				// �J����Ԃɂ���
				pDoor->SetState(CDoor::STATE_OPEN);

				// �J�ڏ�Ԃɂ���
				CTutorial::SetState(CTutorial::STATE_TRANS);

				// true ��Ԃ�
				return true;
			}
		}
		else
		{ // ��L�ȊO

			// �{�^���\����`�悵�Ȃ�
			pDoor->GetButton()->SetEnableDisp(false);
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �h�A�Ƃ̓����蔻��
//===============================
bool collision::DoorCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& size)
{
	// ���[�J���ϐ��錾
	CDoor* pDoor = CTutorial::GetDoor();		// �h�A�̏��

	if (pDoor != nullptr)
	{ // �h�A�����݂���ꍇ

		// �h�A�̏����擾����
		D3DXVECTOR3 posDoor = pDoor->GetPos();					// �h�A�̈ʒu
		D3DXVECTOR3 posOldDoor = pDoor->GetPosOld();			// �h�A�̑O��̈ʒu
		D3DXVECTOR3 vtxMaxDoor = pDoor->GetFileData().vtxMax;	// �h�A�̒��_�̍ő�l
		D3DXVECTOR3 vtxMinDoor = pDoor->GetFileData().vtxMin;	// �h�A�̒��_�̍ŏ��l
		D3DXVECTOR3 vtxMax = size;								// ���_�̍ő�l
		D3DXVECTOR3 vtxMin = useful::VtxMinConv(size);			// ���_�̍ŏ��l

		// �Z�ʑ̂̓����蔻��
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
		{ // ��ɏ�����ꍇ

			// true ��Ԃ�
			return true;
		}
	}

	// false ��Ԃ�
	return false;
}

//===============================
// �X�e�[�W�̓����蔻��
//===============================
void collision::StageCollision(D3DXVECTOR3* pos, const float fWidth)
{
	if (pos->x + fWidth >= STAGE_RANGE)
	{ // �E����o���ꍇ

		// �ʒu��␳����
		pos->x = STAGE_RANGE - fWidth;
	}

	if (pos->x - fWidth <= -STAGE_RANGE)
	{ // ������o���ꍇ

		// �ʒu��␳����
		pos->x = -STAGE_RANGE + fWidth;
	}

	if (pos->z + fWidth >= STAGE_RANGE)
	{ // ������o���ꍇ

		// �ʒu��␳����
		pos->z = STAGE_RANGE - fWidth;
	}

	if (pos->z - fWidth <= -STAGE_RANGE)
	{ // ��O����o���ꍇ

		// �ʒu��␳����
		pos->z = -STAGE_RANGE + fWidth;
	}
}

/*
* @brief �Z�ʑ̂̓����蔻��
* @param pos [out] �Ώۂ̈ʒu
* @param posBlock [in] �u���b�N�̈ʒu
* @param posOld [in] �Ώۂ̑O��̈ʒu
* @param posOldBlock [in] �u���b�N�̑O��̈ʒu
* @param min [in] �ŏ��l
* @param minBlock [in] �u���b�N�̍ŏ��l
* @param max [in] �ő�l
* @param maxBlock [in] �u���b�N�̍ő�l
* @return bool �Ԃ��������ǂ���
*/
bool collision::HexahedronCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock)
{
	if (posBlock.y + maxBlock.y >= pos->y + min.y &&
		posBlock.y + minBlock.y <= pos->y + max.y &&
		posBlock.z + maxBlock.z >= pos->z + min.z &&
		posBlock.z + minBlock.z <= pos->z + max.z)
	{ // X���̔���ɓ����ꍇ

		if (posOldBlock.x + maxBlock.x <= posOld.x + min.x &&
			posBlock.x + maxBlock.x >= pos->x + min.x)
		{ // �E�ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->x = posBlock.x + maxBlock.x - (min.x - COLLISION_ADD_DIFF_LENGTH);

			// true ��Ԃ�
			return true;
		}
		else if (posOldBlock.x + minBlock.x >= posOld.x + max.x &&
			posBlock.x + minBlock.x <= pos->x + max.x)
		{ // ���ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->x = posBlock.x + minBlock.x - (max.x + COLLISION_ADD_DIFF_LENGTH);

			// true ��Ԃ�
			return true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos->x + min.x &&
		posBlock.x + minBlock.x <= pos->x + max.x &&
		posBlock.y + maxBlock.y >= pos->y + min.y &&
		posBlock.y + minBlock.y <= pos->y + max.y)
	{ // Z���̔���ɓ����ꍇ

		if (posOldBlock.z + maxBlock.z <= posOld.z + min.z &&
			posBlock.z + maxBlock.z >= pos->z + min.z)
		{ // ���ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->z = posBlock.z + maxBlock.z - (min.z - COLLISION_ADD_DIFF_LENGTH);

			// true ��Ԃ�
			return true;
		}
		else if (posOldBlock.z + minBlock.z >= posOld.z + max.z &&
			posBlock.z + minBlock.z <= pos->z + max.z)
		{ // ��O�ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->z = posBlock.z + minBlock.z - (max.z + COLLISION_ADD_DIFF_LENGTH);

			// true ��Ԃ�
			return true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos->x + min.x &&
		posBlock.x + minBlock.x <= pos->x + max.x &&
		posBlock.z + maxBlock.z >= pos->z + min.z &&
		posBlock.z + minBlock.z <= pos->z + max.z)
	{ // Y���̔���ɓ����ꍇ

		if (posOldBlock.y + maxBlock.y <= posOld.y + min.y &&
			posBlock.y + maxBlock.y >= pos->y + min.y)
		{ // ��ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->y = posBlock.y + maxBlock.y - (min.y - COLLISION_ADD_DIFF_LENGTH);

			// true ��Ԃ�
			return true;
		}
		else if (posOldBlock.y + minBlock.y >= posOld.y + max.y &&
			posBlock.y + minBlock.y <= pos->y + max.y)
		{ // ���ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->y = posBlock.y + minBlock.y - (max.y + COLLISION_ADD_DIFF_LENGTH);

			// true ��Ԃ�
			return true;
		}
	}

	// false ��Ԃ�
	return false;
}

/*
* @brief �Z�ʑ̂̃q�b�g����
* @param pos [in] �Ώۂ̈ʒu
* @param posBlock [in] �u���b�N�̈ʒu
* @param posOld [in] �Ώۂ̑O��̈ʒu
* @param posOldBlock [in] �u���b�N�̑O��̈ʒu
* @param min [in] �ŏ��l
* @param minBlock [in] �u���b�N�̍ŏ��l
* @param max [in] �ő�l
* @param maxBlock [in] �u���b�N�̍ő�l
* @return bool �Ԃ��������ǂ���
*/
bool collision::HexahedronHit(const D3DXVECTOR3& pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock)
{
		if (posBlock.y + maxBlock.y >= pos.y + min.y &&
		posBlock.y + minBlock.y <= pos.y + max.y &&
		posBlock.z + maxBlock.z >= pos.z + min.z &&
		posBlock.z + minBlock.z <= pos.z + max.z)
	{ // X���̔���ɓ����ꍇ

		if (posOldBlock.x + maxBlock.x <= posOld.x + min.x &&
			posBlock.x + maxBlock.x >= pos.x + min.x)
		{ // �E�ɂԂ������ꍇ

			// true ��Ԃ�
			return true;
		}
		else if (posOldBlock.x + minBlock.x >= posOld.x + max.x &&
			posBlock.x + minBlock.x <= pos.x + max.x)
		{ // ���ɂԂ������ꍇ

			// true ��Ԃ�
			return true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos.x + min.x &&
		posBlock.x + minBlock.x <= pos.x + max.x &&
		posBlock.y + maxBlock.y >= pos.y + min.y &&
		posBlock.y + minBlock.y <= pos.y + max.y)
	{ // Z���̔���ɓ����ꍇ

		if (posOldBlock.z + maxBlock.z <= posOld.z + min.z &&
			posBlock.z + maxBlock.z >= pos.z + min.z)
		{ // ���ɂԂ������ꍇ

			// true ��Ԃ�
			return true;
		}
		else if (posOldBlock.z + minBlock.z >= posOld.z + max.z &&
			posBlock.z + minBlock.z <= pos.z + max.z)
		{ // ��O�ɂԂ������ꍇ

			// true ��Ԃ�
			return true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos.x + min.x &&
		posBlock.x + minBlock.x <= pos.x + max.x &&
		posBlock.z + maxBlock.z >= pos.z + min.z &&
		posBlock.z + minBlock.z <= pos.z + max.z)
	{ // Y���̔���ɓ����ꍇ

		if (posOldBlock.y + maxBlock.y <= posOld.y + min.y &&
			posBlock.y + maxBlock.y >= pos.y + min.y)
		{ // ��ɂԂ������ꍇ

			// true ��Ԃ�
			return true;
		}
		else if (posOldBlock.y + minBlock.y >= posOld.y + max.y &&
			posBlock.y + minBlock.y <= pos.y + max.y)
		{ // ���ɂԂ������ꍇ

			// true ��Ԃ�
			return true;
		}
	}

	// false ��Ԃ�
	return false;
}
/*
// @brief �Z�ʑ̂̃q�b�g����(Old����)
* @param pos [in] �Ώۂ̈ʒu
* @param posBlock [in] �u���b�N�̈ʒu
* @param min [in] �ŏ��l
* @param minBlock [in] �u���b�N�̍ŏ��l
* @param max [in] �ő�l
* @param maxBlock [in] �u���b�N�̍ő�l
* @return SCollision �Z�ʑ̂̓����蔻��
*/
bool collision::HexahedronHit(const D3DXVECTOR3& pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock)
{
	if (useful::RectangleCollisionXY(pos, posBlock, max, maxBlock, min, minBlock) == true &&
		useful::RectangleCollisionYZ(pos, posBlock, max, maxBlock, min, minBlock) == true &&
		useful::RectangleCollisionXZ(pos, posBlock, max, maxBlock, min, minBlock) == true)
	{ // �Z�ʑ̓��m���G��Ă����ꍇ

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

/*
* @brief �Z�ʑ̂̓����蔻��(�ǂ̖ʂɏ�������̔���t��)
* @param pos [out] �Ώۂ̈ʒu
* @param posBlock [in] �u���b�N�̈ʒu
* @param posOld [in] �Ώۂ̑O��̈ʒu
* @param posOldBlock [in] �u���b�N�̑O��̈ʒu
* @param min [in] �ŏ��l
* @param minBlock [in] �u���b�N�̍ŏ��l
* @param max [in] �ő�l
* @param maxBlock [in] �u���b�N�̍ő�l
* @return SCollision �Z�ʑ̂̓����蔻��
*/
collision::SCollision collision::HexahedronClush(D3DXVECTOR3* pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock)
{
	// �����蔻��̕ϐ���錾
	SCollision collision = { false,false,false,false,false,false };

	if (posBlock.y + maxBlock.y >= pos->y + min.y &&
		posBlock.y + minBlock.y <= pos->y + max.y &&
		posBlock.z + maxBlock.z >= pos->z + min.z &&
		posBlock.z + minBlock.z <= pos->z + max.z)
	{ // X���̔���ɓ����ꍇ

		if (posOldBlock.x + maxBlock.x <= posOld.x + min.x &&
			posBlock.x + maxBlock.x >= pos->x + min.x)
		{ // �E�ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->x = posBlock.x + maxBlock.x - (min.x - COLLISION_ADD_DIFF_LENGTH);

			// �E�̓����蔻��� true �ɂ���
			collision.bRight = true;
		}
		else if (posOldBlock.x + minBlock.x >= posOld.x + max.x &&
			posBlock.x + minBlock.x <= pos->x + max.x)
		{ // ���ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->x = posBlock.x + minBlock.x - (max.x + COLLISION_ADD_DIFF_LENGTH);

			// ���̓����蔻��� true �ɂ���
			collision.bLeft = true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos->x + min.x &&
		posBlock.x + minBlock.x <= pos->x + max.x &&
		posBlock.y + maxBlock.y >= pos->y + min.y &&
		posBlock.y + minBlock.y <= pos->y + max.y)
	{ // Z���̔���ɓ����ꍇ

		if (posOldBlock.z + maxBlock.z <= posOld.z + min.z &&
			posBlock.z + maxBlock.z >= pos->z + min.z)
		{ // ���ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->z = posBlock.z + maxBlock.z - (min.z - COLLISION_ADD_DIFF_LENGTH);

			// ���̓����蔻��� true �ɂ���
			collision.bFar = true;
		}
		else if (posOldBlock.z + minBlock.z >= posOld.z + max.z &&
			posBlock.z + minBlock.z <= pos->z + max.z)
		{ // ��O�ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->z = posBlock.z + minBlock.z - (max.z + COLLISION_ADD_DIFF_LENGTH);

			// ��O�̓����蔻��� true �ɂ���
			collision.bNear = true;
		}
	}

	if (posBlock.x + maxBlock.x >= pos->x + min.x &&
		posBlock.x + minBlock.x <= pos->x + max.x &&
		posBlock.z + maxBlock.z >= pos->z + min.z &&
		posBlock.z + minBlock.z <= pos->z + max.z)
	{ // Y���̔���ɓ����ꍇ

		if (posOldBlock.y + maxBlock.y <= posOld.y + min.y &&
			posBlock.y + maxBlock.y >= pos->y + min.y)
		{ // ��ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->y = posBlock.y + maxBlock.y - (min.y - COLLISION_ADD_DIFF_LENGTH);

			// ��̓����蔻��� true �ɂ���
			collision.bTop = true;
		}
		else if (posOldBlock.y + minBlock.y >= posOld.y + max.y &&
			posBlock.y + minBlock.y <= pos->y + max.y)
		{ // ���ɂԂ������ꍇ

			// �ʒu��ݒ肷��
			pos->y = posBlock.y + minBlock.y - (max.y + COLLISION_ADD_DIFF_LENGTH);

			// ���̓����蔻��� true �ɂ���
			collision.bBottom = true;
		}
	}

	// �����蔻��̕ϐ���Ԃ�
	return collision;
}