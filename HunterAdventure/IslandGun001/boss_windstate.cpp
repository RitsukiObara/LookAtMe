//================================================================================================================
//
// �{�X�̂��܂�������ԏ��� [boss_windstate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "manager.h"
#include "boss.h"
#include "boss_windstate.h"
#include "motion.h"
#include "sound.h"

#include "game.h"
#include "player.h"
#include "wind_shot.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const float CHASE_CORRECT = 0.01f;			// �ǐՂ̕␳��
	const int WIND_CREATE_COUNT = 70;			// ���U�������J�E���g
	const int FINISH_COUNT = 400;				// �I���J�E���g
}

//==========================
// �R���X�g���N�^
//==========================
CBossWindState::CBossWindState()
{
	// �S�Ă̒l���N���A����
	m_pWindShot = nullptr;			// ���U���̏��
	m_nCount = 0;					// �o�߃J�E���g
}

//==========================
// �f�X�g���N�^
//==========================
CBossWindState::~CBossWindState()
{
	if (m_pWindShot != nullptr)
	{ // ���U���� NULL ����Ȃ��ꍇ

		// ���U���� NULL �ɂ���
		m_pWindShot->SetState(CWindShot::STATE_DELETE);
		m_pWindShot = nullptr;
	}
}

//==========================
// ��ԏ���
//==========================
void CBossWindState::Process(CBoss* pBoss)
{
	// �J�E���g�����Z����
	m_nCount++;

	if (m_nCount == WIND_CREATE_COUNT)
	{ // ���U�������J�E���g�ɂȂ����ꍇ

		// ���U���𐶐�
		m_pWindShot = CWindShot::Create(pBoss->GetPos());

		// ���U������炷
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_WINDSHOT);
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_BOSSHOWLING);
	}

	if (m_nCount >= FINISH_COUNT)
	{ // ���J�E���g�o�߂����ꍇ

		// �ʏ��Ԃɂ���
		pBoss->ChangeState(new CBossNoneState);

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossWindState::SetData(CBoss* pBoss)
{
	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_HOWLING)
	{ // ���i�����[�V�����ȊO�̏ꍇ

		// ���i�����[�V�����ɂ���
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_HOWLING);
	}
}