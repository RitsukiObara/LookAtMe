//===================================
//
// �{�X�̋C���ԃw�b�_�[[boss_stunstate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_STUN_STATE_H_
#define _BOSS_STUN_STATE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

// �N���X��`(�{�X�̋C���ԃN���X)
class CBossStunState : public CBossState
{
public:

	CBossStunState();		// �R���X�g���N�^
	~CBossStunState();		// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�ϐ�
	int m_nCount;		// �o�߃J�E���g
};

#endif