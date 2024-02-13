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
#include "boss.h"
#include "boss_windstate.h"
#include "motion.h"

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
	const int WIND_CREATE_COUNT = 180;			// �����o��܂ł̃J�E���g��
	const int FINISH_COUNT = 450;				// �I���J�E���g
}

//==========================
// �R���X�g���N�^
//==========================
CBossWindState::CBossWindState()
{
	// �S�Ă̒l���N���A����
	m_pWindShot = nullptr;			// ���U���̏��
	m_action = ACTION_CHARGE;		// �s����
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
		m_pWindShot = nullptr;
	}
}

//==========================
// ��ԏ���
//==========================
void CBossWindState::Process(CBoss* pBoss)
{
	switch (m_action)
	{
	case CBossWindState::ACTION_CHARGE:

		// �o�߃J�E���g�����Z����
		m_nCount++;

		if (m_nCount % WIND_CREATE_COUNT == 0)
		{ // ���J�E���g���o�߂����ꍇ

			// �o�߃J�E���g������������
			m_nCount = 0;

			// ��������Ԃɂ���
			m_action = ACTION_WIND;

			D3DXVECTOR3 pos = pBoss->GetPos();

			if (m_pWindShot == nullptr)
			{ // ���U���̏�� NULL ����Ȃ��ꍇ

				// ���𐶐�����
				m_pWindShot = CWindShot::Create(pos);
			}
		}

		break;

	case CBossWindState::ACTION_WIND:

		// �ǐՏ���
		Chase(pBoss);

		// �J�E���g�����Z����
		m_nCount++;

		if (m_nCount >= FINISH_COUNT)
		{ // ���J�E���g�o�߂����ꍇ

			// ������Ԃɂ���
			m_pWindShot->SetState(CWindShot::STATE_DELETE);

			// �ʏ��Ԃɂ���
			pBoss->ChangeState(new CBossNoneState);

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossWindState::SetData(CBoss* pBoss)
{
	// ��s���[�V�����ɂ���
	pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_CHARGE);
}

//==========================
// �ǐՏ���
//==========================
void CBossWindState::Chase(CBoss* pBoss)
{
	// �v���C���[�̏����擾����
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �ʒu�ƌ�����錾
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		D3DXVECTOR3 posBoss = pBoss->GetPos();
		D3DXVECTOR3 rotBoss = pBoss->GetRot();
		float fRotDest = atan2f(posPlayer.x - posBoss.x, posPlayer.z - posBoss.z);

		// �����̕␳����
		useful::RotCorrect(fRotDest, &rotBoss.y, CHASE_CORRECT);

		// ������K�p����
		pBoss->SetRot(rotBoss);
	}
}