//===================================
//
// �����蔻��w�b�_�[[collision.h]
// Author ��������
//
//===================================
#ifndef _COLLISION_H_			//���̃}�N����`������Ă��Ȃ�������
#define _COLLISION_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CPlayer;			// �v���C���[
class CEnemy;			// �G
class CBullet;			// �e��

//-----------------------------------
// ���O���(�����蔻��)
//-----------------------------------
namespace collision
{
	// �\���̒�`(�����蔻��)
	struct SCollision
	{
		bool bTop;		// ��̓����蔻��
		bool bBottom;	// ���̓����蔻��
		bool bRight;	// �E�̓����蔻��
		bool bLeft;		// ���̓����蔻��
		bool bFar;		// ���̓����蔻��
		bool bNear;		// ��O�̓����蔻��
	};

	// �N���n�ʂ̓����蔻��
	bool ElevOutRangeCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth);			// �N���n�ʂ͈̔͊O�̓����蔻��

	// �����Ƃ̓����蔻��
	void CoinCollision(CPlayer* pPlayer, const D3DXVECTOR3 size, const int nAreaIdx);				// �����Ƃ̓����蔻��

	// �G�Ƃ̓����蔻��
	bool EnemyHitToGun(const CBullet& bullet);			// �G�Əe�̓����蔻��
	bool EnemyHitToDagger(const D3DXVECTOR3& pos, const float fHeight, const float fRadius);		// �G�ƃ_�K�[�̓����蔻��
	bool EnemyHitToPlayer(CPlayer* pPlayer, const float fRadius, const float fHeight);				// �G�ƃv���C���[�Ƃ̓����蔻��
	void EnemyHitToEnemy(CEnemy* pTarget);		// �G���m�̓����蔻��

	// ���̍��Ƃ̓����蔻��
	void GoldBoneCollision(const CPlayer& pPlayer, const D3DXVECTOR3& size);		// ���̍��Ƃ̓����蔻��

	// �؂Ƃ̓����蔻��
	bool TreeCollision(D3DXVECTOR3* pos, const float fRadius, const int nAreaIdx);			// �؂̓����蔻��
	void TreeAttack(const CPlayer& pPlayer, const float fRadius, const float fHeight);		// �؂ւ̍U�����菈��

	// ���V�̎��Ƃ̓����蔻��
	void PalmFruitHit(CPlayer* pPlayer, const float fRadius, const float fHeight);			// ���V�̎��Ƃ̓����蔻��
	bool PalmFruitAttack(const D3DXVECTOR3& pos, const float fRadius, const int nAreaIdx);	// ���V�̎��ւ̍U������

	// ��Ƃ̓����蔻��
	bool RockCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const float fRadius, const float fHeight, const int nAreaIdx, float* fGravity = nullptr, bool* bJump = nullptr);		// ��Ƃ̓����蔻��

	// ���e�ԂƂ̓����蔻��
	bool BangFlowerHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight);			// ���e�Ԃ̃q�b�g����
	bool BombHitToGun(const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld, const float fRadius);		// ���e�̃q�b�g����(�e�e)
	bool BombHitToDagger(const D3DXVECTOR3& pos, const float fRadius, const float fHeight);			// ���e�̃q�b�g����(�_�K�[)
	bool BombHitToSlashRipple(const D3DXVECTOR3& pos, const float fRadius, const float fHeight);	// ���e�̃q�b�g����(�a���g��)

	// �����Ƃ̓����蔻��
	void ExplosionHitToRock(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, const int nAreaIdx);		// �����Ɗ�Ƃ̓����蔻��
	void ExplosionHitToEnemy(const D3DXVECTOR3& pos, const float fRadius, const float fHeight);		// �����ƓG�Ƃ̓����蔻��
	bool ExplosionHitToPlayer(CPlayer* pPlayer, const float fRadius, const float fHeight);			// �����ƃv���C���[�Ƃ̓����蔻��
	
	// �ǂƂ̓����蔻��
	bool WallCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx);		// �ǂƂ̓����蔻��

	// �u���b�N�Ƃ̓����蔻��
	bool BlockHit(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx);			// �u���b�N�Ƃ̃q�b�g����

	// �{�X�Ƃ̓����蔻��
	bool BossHit(const D3DXVECTOR3& pos, const float fRadius, const int nDamage);				// �{�X�̓����蔻��
	bool BossAttack(const D3DXVECTOR3& pos, const float fRadius, float* fRotSmash = nullptr);	// �{�X�Ƃ̃q�b�g����

	// �Ւd�Ƃ̓����蔻��
	bool AlterCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin);		// �Ւd�Ƃ̓����蔻��
	void AlterSurrounding(const D3DXVECTOR3& pos, const float fRadius);			// �Ւd���͂̓����蔻��

	// �a���g��Ƃ̓����蔻��
	bool RippleHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, float* fRotSmash = nullptr);		// �a���̔g��̓����蔻��
	
	// ���U���Ƃ̓����蔻��
	bool WindShotHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, float* fSmashRot = nullptr);		// ���U���Ƃ̓����蔻��

	// ���U���Ƃ̓����蔻��
	bool FireShotHit(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, float* fSmashRot = nullptr);		// ���U���Ƃ̓����蔻��

	// �Ŕ̓����蔻��
	bool SignboardCollision(const D3DXVECTOR3& pos, const float fRadius);		// �ŔƂ̓����蔻��

	// �I�̓����蔻��
	bool TargetHit(const D3DXVECTOR3& pos, const float fRadius);				// �I�Ƃ̓����蔻��

	// �h�A�Ƃ̓����蔻��
	bool DoorHit(const D3DXVECTOR3& pos, const float fRadius);					// �h�A�Ƃ̃q�b�g����
	bool DoorCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& size);		// �h�A�Ƃ̓����蔻��

	// �X�e�[�W�̓����蔻��
	void StageCollision(D3DXVECTOR3* pos, const float fWidth);					// �X�e�[�W�̓����蔻��

	// �ėp�I�ȓ����蔻��
	bool HexahedronCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock);		// �Z�ʑ̂̓����蔻��
	bool HexahedronHit(const D3DXVECTOR3& pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock);		// �Z�ʑ̂̃q�b�g����
	bool HexahedronHit(const D3DXVECTOR3& pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock);																	// �Z�ʑ̂̃q�b�g����(Old����)
	SCollision HexahedronClush(D3DXVECTOR3* pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock);		// �Z�ʑ̂̓����蔻��(�ǂ̖ʂɏ�������̔���t��)
}

#endif