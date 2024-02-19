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
	// �ǐՏ���
	Chase(pBoss);

	// �J�E���g�����Z����
	m_nCount++;

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
	// �ҋ@���[�V�����ɂ���
	pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_NEUTRAL);

	// ���U���𐶐�
	m_pWindShot = CWindShot::Create(pBoss->GetPos());
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