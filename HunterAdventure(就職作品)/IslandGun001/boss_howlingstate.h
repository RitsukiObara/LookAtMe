//===================================
//
// �{�X�̗Y�����я�ԃw�b�_�[[boss_howlingstate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_HOWLING_STATE_H_
#define _BOSS_HOWLING_STATE_H_
//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

// �N���X��`(�{�X�̉��i����ԃN���X)
class CBossHowlingState : public CBossState
{
public:

	CBossHowlingState();	// �R���X�g���N�^
	~CBossHowlingState();	// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�ϐ�
	int m_nCount;		// �o�߃J�E���g
};

#endif