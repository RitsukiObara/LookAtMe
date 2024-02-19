//===================================
//
// �{�X�̂��܂�������ԃw�b�_�[[boss_windstate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_WIND_STATE_H_
#define _BOSS_WIND_STATE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CWindShot;			// ���U��

//-----------------------------------
// �N���X��`(�{�X�̂��܂�������ԃN���X)
//-----------------------------------
class CBossWindState : public CBossState
{
public:

	CBossWindState();		// �R���X�g���N�^
	~CBossWindState();		// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�֐�
	void Chase(CBoss* pBoss);			// �ǐՏ���

	// �����o�ϐ�
	CWindShot* m_pWindShot;		// ���U���̏��
	int m_nCount;				// �o�߃J�E���g
};

#endif