//================================================================================================================
//
// �{�X�̃`���[�W��ԏ��� [boss_chargestate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_chargestate.h"
#include "motion.h"

#include "game.h"
#include "player.h"

//==========================
// �I�[�o�[���[�h�R���X�g���N�^
//==========================
CBossChargeState::CBossChargeState(CBossState* next, const int nCount)
{
	// �S�Ă̒l���N���A����
	m_pNext = next;				// ���̏��
	m_nCount = nCount;			// �o�߃J�E���g
}

//==========================
// �f�X�g���N�^
//==========================
CBossChargeState::~CBossChargeState()
{
	if (m_pNext != nullptr)
	{ // ���̏�Ԃ����������ꍇ

		// �I������
		m_pNext = nullptr;
	}
}

//==========================
// ���S��������
//==========================
void CBossChargeState::Delete(void)
{
	if (m_pNext != nullptr)
	{ // ���̏�Ԃ����������ꍇ

		// �I������
		m_pNext->Uninit();
		m_pNext = nullptr;
	}

	// �{�̂̏�������
	CBossState::Delete();
}

//==========================
// ��ԏ���
//==========================
void CBossChargeState::Process(CBoss* pBoss)
{
	// �o�߃J�E���g�����Z����
	m_nCount--;

	if (m_nCount <= 0)
	{ // ���J�E���g�o�߂����ꍇ

		// �o�߃J�E���g������������
		m_nCount = 0;

		// ���̏�Ԃɕς���
		pBoss->ChangeState(m_pNext);
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossChargeState::SetData(CBoss* pBoss)
{
	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_CHARGE)
	{ // �`���[�W���[�V�����ȊO�̏ꍇ

		// �`���[�W���[�V�����ɂ���
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_CHARGE);
	}
}