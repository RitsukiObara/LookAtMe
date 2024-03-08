//===================================
//
// �{�X�̔����w�b�_�[[boss_explosion.h]
// Author ��������
//
//===================================
#ifndef _BOSS_EXPLOSION_H_
#define _BOSS_EXPLOSION_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CMeshSphere;			// ���b�V���X�t�B�A
class CMeshDonut;			// ���b�V���h�[�i�c

//-----------------------------------
// �N���X��`(�{�X�̔���)
//-----------------------------------
class CBossExplosion : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	CBossExplosion();		// �R���X�g���N�^
	~CBossExplosion();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBossExplosion* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// ��������

	// �萔��`
	static const int NUM_SHOCK = 2;		// �Ռ��̑���

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CMeshSphere* m_pExplosion;			// ����
	CMeshDonut* m_apShock[NUM_SHOCK];	// �Ռ�

	float m_fAddExpl;		// �����̒ǉ���
	float m_fAddShock;		// �Ռ��̒ǉ���
	float m_fAlpha;			// �����x
};

#endif