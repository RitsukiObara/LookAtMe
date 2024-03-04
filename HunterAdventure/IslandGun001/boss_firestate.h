//===================================
//
// �{�X�̉��U����ԃw�b�_�[[boss_firestate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_FIRE_STATE_H_
#define _BOSS_FIRE_STATE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

//-----------------------------------
// �N���X��`(�{�X�̉��U����ԃN���X)
//-----------------------------------
class CBossFireState : public CBossState
{
public:

	CBossFireState();		// �R���X�g���N�^
	~CBossFireState();		// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�ϐ�
	int m_nCount;				// �o�߃J�E���g
};

#endif