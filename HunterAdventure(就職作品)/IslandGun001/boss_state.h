//===================================
//
// �{�X�̏�ԃw�b�_�[[boss_state.h]
// Author ��������
//
//===================================
#ifndef _BOSS_STATE_H_
#define _BOSS_STATE_H_
//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CBoss;		// �{�X

// �N���X��`(�{�X�̏�Ԃ̊�{�N���X)
class CBossState
{
public:

	CBossState();			// �R���X�g���N�^
	virtual ~CBossState();	// �f�X�g���N�^

	void Uninit();			// �I������

	virtual void Process(CBoss* pBoss) = 0;	// ��ԏ���
	virtual void SetData(CBoss* pBoss) = 0;	// ���̐ݒ菈��
};

#endif