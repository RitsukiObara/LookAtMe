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
	void CoinCollision(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);			// �����Ƃ̓����蔻��

	// �G�Ƃ̓����蔻��
	bool EnemyHitToGun(const CBullet& bullet);			// �G�Əe�̓����蔻��
	bool EnemyHitToDagger(const D3DXVECTOR3& pos, const float fHeight, const float fRadius);				// �G�ƃ_�K�[�̓����蔻��

	// ���̍��Ƃ̓����蔻��
	void GoldBoneCollision(const CPlayer& pPlayer, const D3DXVECTOR3& size);		// ���̍��Ƃ̓����蔻��

	// �؂Ƃ̓����蔻��
	bool TreeCollision(D3DXVECTOR3* pos, const float fRadius);			// �؂̓����蔻��
	void TreeAttack(const CPlayer& pPlayer, const float fHeight);		// �؂ւ̍U�����菈��

	// ���V�̎��Ƃ̓����蔻��
	void PalmFruitHit(CPlayer* pPlayer, const float fHeight);			// ���V�̖؂Ƃ̓����蔻��

	// ��Ƃ̓����蔻��
	void RockCollision(D3DXVECTOR3* pos, const float fRadius, const float fHeight);		// ��Ƃ̓����蔻��
	
	// �ėp�I�ȓ����蔻��
	bool HexahedronCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock);		// �Z�ʑ̂̓����蔻��
	bool HexahedronHit(const D3DXVECTOR3& pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock);		// �Z�ʑ̂̃q�b�g����
	SCollision HexahedronClush(D3DXVECTOR3* pos, const D3DXVECTOR3& posBlock, const D3DXVECTOR3& posOld, const D3DXVECTOR3& posOldBlock, const D3DXVECTOR3& min, const D3DXVECTOR3& minBlock, const D3DXVECTOR3& max, const D3DXVECTOR3& maxBlock);		// �Z�ʑ̂̓����蔻��(�ǂ̖ʂɏ�������̔���t��)
}

#endif