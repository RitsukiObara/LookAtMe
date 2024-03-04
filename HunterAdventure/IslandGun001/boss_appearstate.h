//===================================
//
// �{�X�̓o���ԃw�b�_�[[boss_appearstate.h]
// Author ��������
//
//===================================
#ifndef _BOSS_APPEAR_STATE_H_
#define _BOSS_APPEAR_STATE_H_
//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "boss_state.h"

// �N���X��`(�{�X�̏o����ԃN���X)
class CBossAppearState : public CBossState
{
public:

	CBossAppearState();		// �R���X�g���N�^
	~CBossAppearState();	// �f�X�g���N�^

	void Process(CBoss* pBoss) override;		// ��ԏ���
	void SetData(CBoss* pBoss) override;		// ���̐ݒ菈��

private:

	// �����o�֐�
	void Length(void);			// �����֌W����
	void PosRot(CBoss* pBoss);	// �ʒu�����ݒ菈��
	void Ripple(CBoss* pBoss);	// �g��̐ݒu����
	void LightCol(void);		// ���̐F�̐ݒ菈��

	// �����o�ϐ�
	int m_nCount;			// �o�߃J�E���g
	float m_fLangeRot;		// ��������
	float m_fLength;		// ����
};

#endif