//===================================
//
// �{�X�̔��j��ԃw�b�_�[[boss_explosionstate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_EXPLOSION_STATE_H_
#define _BOSS_EXPLOSION_STATE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

// �N���X��`(�{�X�̔��j��ԃN���X)
class CBossExplosionState : public CBossState
{
public:

	CBossExplosionState();		// �R���X�g���N�^
	~CBossExplosionState();		// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�ϐ�
	int m_nCount;		// �J�E���g
};

#endif