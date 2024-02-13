//================================================================================================================
//
// �{�X�̗Y�����я�ԏ��� [boss_howlingstate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_howlingstate.h"
#include "motion.h"

#include "manager.h"
#include "camera.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int HOWLING_COUNT = 50;				// �Y�����т܂ł̃J�E���g
	const int HOWLING_CAMERA_COUNT = 63;		// �Y�����уJ�����܂ł̃J�E���g
	const int VIBRATE_CAMERA_COUNT = 120;		// �U���J�����܂ł̃J�E���g
	const int NONESTATE_COUNT = 240;			// �ʏ��Ԃ܂ł̃J�E���g
	const CCamera::SVibrate LAND_VIB_INFO =		// ���n���̐U�����
	{
		CCamera::TYPE_BOSSCLOSER,	// ���̎��
		0,							// �o�߃J�E���g
		5,							// �h��̃J�E���g
		90,							// �h��͈̔�
		50,							// �I���J�E���g
		0.3f,						// �␳�W��
		false						// ����
	};
	const CCamera::SVibrate HOWLING_VIB_INFO =	// �Y�����ю��̐U�����
	{
		CCamera::TYPE_NONE,		// ���̎��
		0,						// �o�߃J�E���g
		3,						// �h��̃J�E���g
		400,					// �h��͈̔�
		90,						// �I���J�E���g
		0.4f,					// �␳�W��
		false					// ����
	};
}

//==========================
// �R���X�g���N�^
//==========================
CBossHowlingState::CBossHowlingState()
{
	// �S�Ă̒l���N���A����
	m_nCount = 0;		// �o�߃J�E���g
}

//==========================
// �f�X�g���N�^
//==========================
CBossHowlingState::~CBossHowlingState()
{

}

//==========================
// ��ԏ���
//==========================
void CBossHowlingState::Process(CBoss* pBoss)
{
	// �o�߃J�E���g�����Z����
	m_nCount++;

	switch (m_nCount)
	{
	case HOWLING_COUNT:				// �Y�����у��[�V����

		if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_HOWLING)
		{ // �Y�����у��[�V��������Ȃ��ꍇ

			// �Y�����у��[�V������ݒ肷��
			pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_HOWLING);
		}

		break;

	case HOWLING_CAMERA_COUNT:		// �Y�����уJ����

		if (CManager::Get()->GetCamera()->GetType() != CCamera::TYPE_BOSSHOWLING)
		{ // �Y�����я�ԈȊO�̏ꍇ

			// �Y�����уJ�����ɂ���
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_BOSSHOWLING);
		}

		break;

	case VIBRATE_CAMERA_COUNT:		// �U���J����

		if (CManager::Get()->GetCamera()->GetType() != CCamera::TYPE_VIBRATE)
		{ // �Y�����я�ԈȊO�̏ꍇ

			// �Y�����уJ�����ɂ���
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_VIBRATE);

			{// �U���̏���ݒ肷��

				// �U������K�p����
				CManager::Get()->GetCamera()->SetVibrate(HOWLING_VIB_INFO);
			}
		}

		break;

	case NONESTATE_COUNT:			// �ʏ���

		// �ʏ��Ԃɂ���
		pBoss->ChangeState(new CBossNoneState);

		// ���̐�̏������s��Ȃ�
		return;

		break;
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossHowlingState::SetData(CBoss* pBoss)
{
	// �S�Ă̒l��ݒ肷��
	m_nCount = 0;		// �o�߃J�E���g

	// �z�o�����O���[�V������ݒ肷��
	pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_LANDING);

	// �U���J�����ɂ���
	CManager::Get()->GetCamera()->SetType(CCamera::TYPE_VIBRATE);

	{// �U���̏���ݒ肷��

		// �U������K�p����
		CManager::Get()->GetCamera()->SetVibrate(LAND_VIB_INFO);
	}
}