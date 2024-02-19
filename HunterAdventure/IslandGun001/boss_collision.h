//===================================
//
// �{�X�̓����蔻��w�b�_�[[boss_collision.h]
// Author ��������
//
//===================================
#ifndef _BOSS_COLLISION_H_
#define _BOSS_COLLISION_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CBoss;		// �{�X

// �N���X��`(�{�X�̏o����ԃN���X)
class CBossCollision
{
public:

	// �\���̒�`(�����蔻��)
	struct SCollision
	{
		D3DXVECTOR3 offset;			// �I�t�Z�b�g���W
		float fRadius;				// ���a
		bool bWeakness;				// ��_��
	};

	CBossCollision();				// �R���X�g���N�^
	~CBossCollision();				// �f�X�g���N�^

	void Uninit(void);				// �I������

	void SetData(const int nNum);	// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetColl(const SCollision& coll, const int nIdx);				// �����蔻��̏��̐ݒ菈��
	SCollision GetColl(const int nIdx) const;							// �����蔻��̏��̎擾����
	void SetCollOffset(const D3DXVECTOR3& offset, const int nIdx);		// �I�t�Z�b�g���W�̐ݒ菈��
	D3DXVECTOR3 GetCollOffset(const int nIdx) const;					// �I�t�Z�b�g���W�̎擾����
	void SetRadius(const float fRadius, const int nIdx);				// ���a�̐ݒ菈��
	float GetRadius(const int nIdx) const;								// ���a�̎擾����
	void SetEnableWeakness(const bool bWeak, const int nIdx);			// ��_�󋵂̐ݒ菈��
	bool IsWeakness(const int nIdx) const;								// ��_�󋵂̎擾����
	void SetNumColl(const int nNum);				// �����蔻��̑����̐ݒ菈��
	int GetNumColl(void) const;						// �����蔻��̑����̎擾����

	// �ÓI�����o�֐�
	static CBossCollision* Create(const int nNum);	// ��������

	// �萔��`
	static const int MAX_COLLISION = 10;	// �����蔻��̍ő吔

private:

	// �����o�֐�
	SCollision m_coll[MAX_COLLISION];		// �����蔻��̏��
	int m_nNumColl;							// �����蔻��̑���
};

#endif