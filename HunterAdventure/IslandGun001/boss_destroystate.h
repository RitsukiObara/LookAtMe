//===================================
//
// �{�X�̌��j��ԃw�b�_�[[boss_destroystate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_DESTROY_STATE_H_
#define _BOSS_DESTROY_STATE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

// �N���X��`(�{�X�̌��j��ԃN���X)
class CBossDestroyState : public CBossState
{
public:

	CBossDestroyState();		// �R���X�g���N�^
	~CBossDestroyState();		// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�֐�
	void Move(CBoss* pBoss);	// �ړ�����

	// �����o�ϐ�
	int m_nCount;			// �o�߃J�E���g
	float m_fAddHeight;		// �����̉��Z��
};

#endif