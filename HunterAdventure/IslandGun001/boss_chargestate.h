//===================================
//
// �{�X�̃`���[�W��ԃw�b�_�[[boss_chargestate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_CHARGE_STATE_H_
#define _BOSS_CHARGE_STATE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

//-----------------------------------
// �N���X��`(�`���[�W��ԃN���X)
//-----------------------------------
class CBossChargeState : public CBossState
{
public:

	CBossChargeState(CBossState* next, const int nCount);		// �I�[�o�[���[�h�R���X�g���N�^
	~CBossChargeState();	// �f�X�g���N�^

	void Delete(void) override;	// ���S��������

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�ϐ�
	CBossState* m_pNext;		// ���̏��
	int m_nCount;				// �o�߃J�E���g
};

#endif