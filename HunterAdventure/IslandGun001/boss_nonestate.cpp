//================================================================================================================
//
// �{�X�̒ʏ��ԏ��� [boss_nonestate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_nonestate.h"
#include "motion.h"

#include "game.h"
#include "player.h"

#include "boss_chargestate.h"
#include "boss_flystate.h"
#include "boss_windstate.h"
#include "boss_firestate.h"

//----------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int STATECHANGE_COUNT = 90;				// ��Ԃ��ω�����J�E���g��
	const float GRAVITY = 0.1f;						// �d��

	const int WIND_STATE_CHARGE = 100;				// ����Ԃ̃`���[�W�J�E���g
	const float FLYSTATE_DISTANCE = 4800.0f;		// ��s��Ԃɂ��鋗��
	const int NUM_RAND_STATE = 2;					// �����_���łȂ��Ԃ̑���
}

//==========================
// �R���X�g���N�^
//==========================
CBossNoneState::CBossNoneState()
{
	// �S�Ă̒l���N���A����
	m_fGravity = 0.0f;		// �d��
	m_nCount = 0;			// �o�߃J�E���g
}

//==========================
// �f�X�g���N�^
//==========================
CBossNoneState::~CBossNoneState()
{

}

//==========================
// ��ԏ���
//==========================
void CBossNoneState::Process(CBoss* pBoss)
{
	{ // �d�͏���

		// �ʒu���擾����
		D3DXVECTOR3 pos = pBoss->GetPos();

		// �d�͏���
		useful::Gravity(&m_fGravity, &pos.y, GRAVITY);

		// �ʒu��K�p����
		pBoss->SetPos(pos);
	}

	// �o�߃J�E���g�����Z����
	m_nCount++;

	// �N���n�ʂ̓����蔻��
	pBoss->ElevationCollision();

	if (m_nCount >= STATECHANGE_COUNT)
	{ // ��莞�Ԍo�߂����ꍇ

		// ��Ԃ̑I������
		StateSelect(pBoss);

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossNoneState::SetData(CBoss* pBoss)
{
	// �S�Ă̒l��ݒ肷��
	m_fGravity = 0.0f;		// �d��
	m_nCount = 0;			// �o�߃J�E���g

	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_NEUTRAL)
	{ // �ʏ탂�[�V�����ȊO�̏ꍇ

		// �ҋ@���[�V������ݒ肷��
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_NEUTRAL);
	}
}

//==========================
// ��Ԃ̑I������
//==========================
void CBossNoneState::StateSelect(CBoss* pBoss)
{
	// �v���C���[���擾����
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �ʒu���擾����
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		D3DXVECTOR3 posBoss = pBoss->GetPos();
		float fLength = sqrtf((posPlayer.x - posBoss.x) * (posPlayer.x - posBoss.x) + (posPlayer.z - posBoss.z) * (posPlayer.z - posBoss.z));

		if (fLength >= FLYSTATE_DISTANCE)
		{ // �v���C���[�����������ꍇ

			// ��s��Ԃɂ���
			pBoss->ChangeState(new CBossFlyState);
		}
		else
		{ // ��L�ȊO

			// ���̏�Ԃ������_���ŎZ�o����
			int nState = rand() % NUM_RAND_STATE;

			switch (nState)
			{
			case 0:

				// �`���[�W���(���U�����)�ɂ���
				pBoss->ChangeState(new CBossChargeState(new CBossWindState, WIND_STATE_CHARGE));

				break;

			case 1:

				// ���U����Ԃɂ���
				pBoss->ChangeState(new CBossFireState);

				break;

			default:

				// ��~
				assert(false);

				break;
			}
		}
	}
}