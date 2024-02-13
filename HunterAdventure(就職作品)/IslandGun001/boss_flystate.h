//===================================
//
// �{�X�̔�s��ԃw�b�_�[[boss_flystate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_FLY_STATE_H_
#define _BOSS_FLY_STATE_H_
//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

// �N���X��`(�{�X�̔�s��ԃN���X)
class CBossFlyState : public CBossState
{
public:

	CBossFlyState();	// �R���X�g���N�^
	~CBossFlyState();	// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�֐�
	void RotMove(CBoss* pBoss);		// �����̈ړ�����
	void Standby(CBoss* pBoss);		// �X�^���o�C���� 
	void Fly(CBoss* pBoss);			// ��s����
	void Ripple(CBoss* pBoss);		// �g��̐�������

	// �����o�ϐ�
	D3DXVECTOR3 m_moveDest;			// �ړI�̈ړ���
	D3DXVECTOR3 m_move;				// �ړ���
	int m_nCount;					// �o�߃J�E���g
};

#endif