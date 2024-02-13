//===================================
//
// �{�X�̒ʏ��ԃw�b�_�[[boss_nonestate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_NONE_STATE_H_
#define _BOSS_NONE_STATE_H_
//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

// �N���X��`(�{�X�̒ʏ��ԃN���X)
class CBossNoneState : public CBossState
{
public:

	CBossNoneState();	// �R���X�g���N�^
	~CBossNoneState();	// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�֐�
	void StateSelect(CBoss* pBoss);				// ��Ԃ̑I������

	// �����o�ϐ�
	int m_nCount;		// �o�߃J�E���g
	float m_fGravity;	// �d��
};

#endif