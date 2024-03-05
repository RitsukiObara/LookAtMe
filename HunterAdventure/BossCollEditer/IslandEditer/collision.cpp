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

#include "shadowCircle.h"
#include "objectElevation.h"
#include "coin.h"
#include "enemy.h"
#include "tree.h"
#include "gold_bone.h"
#include "player.h"
#include "player_action.h"
#include "gold_bone_UI.h"
#include "palm_fruit.h"
#include "bullet.h"
#include "rock.h"

//===============================
// �}�N����`
//===============================
namespace
{
	const float COLLISION_ADD_DIFF_LENGTH = 0.01f;			// �͂��Ȍ덷�𖄂߂邽�߂̃}�N����`(�˂�������h�~)
	const float TREE_RADIUS[CTree::TYPE_MAX] =				// �؂̓����蔻�莞�̔��a
	{
		50.5f,		// �؂̔��a
	};
	const float DAGGER_RADIUS = 100.0f;						// �_�K�[�̔��a
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

	// while������
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
void collision::CoinCollision(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMaxCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMinCoin = NONE_D3DXVECTOR3;
	D3DXVECTOR3 vtxMax = size;		// �ő�l
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-size.x, 0.0f, -size.z);	// �ŏ��l
	CListManager<CCoin*> list = CCoin::GetList();
	CCoin* pCoin = nullptr;			// �擪�̏���
	CCoin* pCoinEnd = nullptr;		// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pCoin = list.GetTop();

		// �����̒l���擾����
		pCoinEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �R�C���̕ϐ����擾����
			posCoin = pCoin->GetPos();
			vtxMaxCoin = pCoin->GetFileData().vtxMax;
			vtxMinCoin = pCoin->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true &&
				useful::RectangleCollisionXZ(pos, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true &&
				useful::RectangleCollisionYZ(pos, posCoin, vtxMax, vtxMaxCoin, vtxMin, vtxMinCoin) == true)
			{ // �����Əd�Ȃ����ꍇ

				if (pCoin->GetState() == CCoin::STATE_NONE)
				{ // ����Ԃ̏ꍇ

					// �擾����
					pCoin->Hit();
				}
			}

			if (pCoin == pCoinEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pCoin = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
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

	// while������
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
			enemyMin = D3DXVECTOR3
			(
				-pEnemy->GetCollSize().x,
				0.0f,
				-pEnemy->GetCollSize().z
			);

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

	// while������
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

			if (pos.y <= posEnemy.y + sizeEnemy.y &&
				pos.y + fHeight >= posEnemy.y &&
				useful::CircleCollisionXZ(pos, posEnemy, fRadius, (sizeEnemy.x + sizeEnemy.z) * 0.5f) == true)
			{ // �G�Əd�Ȃ����ꍇ

				// �q�b�g����
				pEnemy->Hit(80, 10.0f);

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
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-size.x, 0.0f, -size.z);	// �ŏ��l
	CListManager<CGoldBone*> list = CGoldBone::GetList();
	CGoldBone* pBone = nullptr;		// �擪�̒l
	CGoldBone* pBoneEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBone = list.GetTop();

		// �����̒l���擾����
		pBoneEnd = list.GetEnd();

		while (true)
		{ // �������[�v

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
bool collision::TreeCollision(D3DXVECTOR3* pos, const float fRadius)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posTree = NONE_D3DXVECTOR3;			// �؂̈ʒu
	float fObjectRadius;							// ���a
	bool bCollision = false;						// �����蔻���
	CListManager<CTree*> list = CTree::GetList();
	CTree* pTree = nullptr;		// �擪�̒l
	CTree* pTreeEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
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
void collision::TreeAttack(const CPlayer& pPlayer, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posTree = NONE_D3DXVECTOR3;			// �؂̈ʒu
	D3DXVECTOR3 posPlayer = pPlayer.GetPos();		// �v���C���[�̈ʒu
	CListManager<CTree*> list = CTree::GetList();
	CTree* pTree = nullptr;		// �擪�̒l
	CTree* pTreeEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
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

			if (useful::CircleCollisionXZ(posPlayer, posTree, DAGGER_RADIUS, TREE_RADIUS[pTree->GetType()]) &&
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
void collision::PalmFruitHit(CPlayer* pPlayer, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posFruit = NONE_D3DXVECTOR3;		// �؂̈ʒu
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// �v���C���[�̈ʒu
	float fRadiusFruit = 0.0f;						// ���a
	CListManager<CPalmFruit*> list = CPalmFruit::GetList();
	CPalmFruit* pFruit = nullptr;		// �擪�̒l
	CPalmFruit* pFruitEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pFruit = list.GetTop();

		// �����̒l���擾����
		pFruitEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			posFruit = pFruit->GetPos();					// �؂̈ʒu
			fRadiusFruit = pFruit->GetFileData().vtxMax.x;	// ���a

			if (posPlayer.y <= posFruit.y + pFruit->GetFileData().vtxMax.y &&
				posPlayer.y + fHeight >= posFruit.y &&
				useful::CircleCollisionXZ(posPlayer, posFruit, DAGGER_RADIUS, fRadiusFruit) == true)
			{ // �~���̓����蔻��ɓ������ꍇ

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
// ��Ƃ̓����蔻��
//===============================
void collision::RockCollision(D3DXVECTOR3* pos, const float fRadius, const float fHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posRock = NONE_D3DXVECTOR3;		// ��̈ʒu
	CXFile::SXFile filedata;					// ��̃��f�����
	CListManager<CRock*> list = CRock::GetList();
	CRock* pRock = nullptr;			// �擪�̒l
	CRock* pRockEnd = nullptr;		// �����̒l
	int nIdx = 0;

	// while������
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

			// ���f���̏����擾����
			filedata = pRock->GetFileData();

			if (pos->y <= posRock.y + filedata.vtxMax.y &&
				pos->y + fHeight >= posRock.y + filedata.vtxMin.y)
			{ // �͈͓��ɂ����ꍇ

				// �~���̓����蔻��
				useful::CylinderCollision(pos, posRock, filedata.fRadius + fRadius);
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