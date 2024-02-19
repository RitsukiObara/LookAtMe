//================================================================================================================
//
// �{�X�̋C���ԏ��� [boss_stunstate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_stunstate.h"
#include "motion.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int STUN_COUNT = 480;		// �C��J�E���g
}

//==========================
// �R���X�g���N�^
//==========================
CBossStunState::CBossStunState()
{
	// �S�Ă̒l���N���A����
	m_nCount = 0;			// �o�߃J�E���g
}

//==========================
// �f�X�g���N�^
//==========================
CBossStunState::~CBossStunState()
{

}

//==========================
// ��ԏ���
//==========================
void CBossStunState::Process(CBoss* pBoss)
{
	// �o�߃J�E���g�����Z����
	m_nCount++;

	if (m_nCount >= STUN_COUNT)
	{ // ��莞�Ԍo�ߌ�

		// �C��񕜏���
		pBoss->StunRecovery();

		// �ʏ��Ԃɂ���
		pBoss->ChangeState(new CBossNoneState);

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossStunState::SetData(CBoss* pBoss)
{
	// �S�Ă̒l��ݒ肷��
	m_nCount = 0;			// �o�߃J�E���g

	// �_�E�����[�V������ݒ肷��
	pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_DOWN);
}