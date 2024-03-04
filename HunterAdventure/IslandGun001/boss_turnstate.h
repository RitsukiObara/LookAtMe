//===================================
//
// �{�X�̌��������ԃw�b�_�[[boss_turnstate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_TURN_STATE_H_
#define _BOSS_TURN_STATE_H_
//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

// �N���X��`(�{�X�̌��������ԃN���X)
class CBossTurnState : public CBossState
{
public:

	CBossTurnState();	// �R���X�g���N�^
	~CBossTurnState();	// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�֐�
	void Fraction(CBoss* pBoss);		// �j�Д�������

	// �����o�ϐ�
	int m_nCount;		// �o�߃J�E���g
};

#endif