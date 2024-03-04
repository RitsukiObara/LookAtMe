//================================================================================================================
//
// �{�X�̔�s��ԏ��� [boss_flystate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_flystate.h"
#include "motion.h"
#include "collision.h"

#include "ripple.h"
#include "game.h"
#include "player.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int FLYMOTION_COUNT = 50;		// ��s���[�V�����ɂȂ�J�E���g��
	const int MOVE_COUNT = 115;			// �����܂ł̃J�E���g��
	const int NONESTATE_COUNT = 150;	// �ʏ��Ԃɖ߂�J�E���g��
	const float ROT_CORRECT = 0.08f;	// �����̕␳�W��
	const float STANDBY_MOVE_Y = 0.2f;	// �X�^���o�C����Y���̈ړ���
	const float MOVE_CORRECT = 0.1f;	// �ړ��̕␳�W��
	const float MOVE_SPEED = 300.0f;	// �ړ����x
	const int RIPPLE_COUNT = 4;			// �g��̃J�E���g
	const float RIPPLE_HEIGHT = 600.0f;	// �g��̍��x
	const D3DXVECTOR3 RIPPLE_SCALE = D3DXVECTOR3(100.0f, 100.0f, 100.0f);	// �g��̊g�嗦
	const float STAGE_COLLISION_WIDTH = 300.0f;		// �X�e�[�W�̓����蔻��̕�
}

//==========================
// �R���X�g���N�^
//==========================
CBossFlyState::CBossFlyState()
{
	// �S�Ă̒l���N���A����
	m_moveDest = NONE_D3DXVECTOR3;	// �ړI�̈ړ���
	m_move = NONE_D3DXVECTOR3;		// �ړ���
	m_nCount = 0;					// �o�߃J�E���g
}

//==========================
// �f�X�g���N�^
//==========================
CBossFlyState::~CBossFlyState()
{

}

//==========================
// ��ԏ���
//==========================
void CBossFlyState::Process(CBoss* pBoss)
{
	// �o�߃J�E���g�����Z����
	m_nCount++;
	
	if (m_nCount >= NONESTATE_COUNT)
	{ // �ʏ��Ԃɖ߂�悤�ɂȂ����ꍇ

		// �q�b�g�󋵂� false �ɂ���
		pBoss->SetEnableHit(false);

		// ��Ԃ̐؂�ւ�����
		pBoss->ChangeState(new CBossNoneState);

		// ���̐�̏������s��Ȃ�
		return;
	}
	else if (m_nCount >= MOVE_COUNT)
	{ // ��莞�Ԍo�߂����ꍇ

		// ��s����
		Fly(pBoss);

		// �g��̐�������
		Ripple(pBoss);

		// �q�b�g�󋵂� true �ɂ���
		pBoss->SetEnableHit(true);
	}
	else if (m_nCount >= FLYMOTION_COUNT)
	{ // ��莞�Ԍo�߂����ꍇ

		// �X�^���o�C����
		Standby(pBoss);
	}
	else
	{ // ��L�ȊO

		// �����̈ړ�����
		RotMove(pBoss);
	}

	// �X�e�[�W�̓����蔻��
	StageCollision(pBoss);
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossFlyState::SetData(CBoss* pBoss)
{
	// �S�Ă̒l��ݒ肷��
	m_moveDest = NONE_D3DXVECTOR3;	// �ړI�̈ړ���
	m_move = NONE_D3DXVECTOR3;		// �ړ���
	m_nCount = 0;					// �o�߃J�E���g
}

//==========================
// �����̈ړ�����
//==========================
void CBossFlyState::RotMove(CBoss* pBoss)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 pos = pBoss->GetPos();
	D3DXVECTOR3 rot = pBoss->GetRot();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// �v���C���[�̈ʒu���擾����
		float fRotDestY = 0.0f;							// �ړI�̌���

		// �ړI�̌������Z�o����
		fRotDestY = atan2f(posPlayer.x - pos.x, posPlayer.z - pos.z);

		// �����̕␳����
		useful::RotCorrect(fRotDestY, &rot.y, ROT_CORRECT);
	}

	// ������K�p����
	pBoss->SetRot(rot);
}

//==========================
// �X�^���o�C���� 
//==========================
void CBossFlyState::Standby(CBoss* pBoss)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = pBoss->GetPos();

	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_FLYING)
	{ // ��s���[�V��������Ȃ��ꍇ

		// ��s���[�V������ݒ肷��
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_FLYING);
	}

	// �ʒu�����Z����
	pos.y += STANDBY_MOVE_Y;

	// �ʒu��K�p����
	pBoss->SetPos(pos);
}

//==========================
// ��s����
//==========================
void CBossFlyState::Fly(CBoss* pBoss)
{
	// �ʒu�ƌ������擾����
	D3DXVECTOR3 pos = pBoss->GetPos();
	D3DXVECTOR3 rot = pBoss->GetRot();

	// �ړI�̈ړ��ʂ�ݒ肷��
	m_moveDest.x = sinf(rot.y) * MOVE_SPEED;
	m_moveDest.z = cosf(rot.y) * MOVE_SPEED;

	// �ړ��ʂ̕␳����
	useful::Correct(m_moveDest.x, &m_move.x, MOVE_CORRECT);
	useful::Correct(m_moveDest.z, &m_move.z, MOVE_CORRECT);

	// �ʒu�����Z����
	pos.x += m_move.x;
	pos.z += m_move.z;

	// �ʒu��K�p����
	pBoss->SetPos(pos);
}

//==========================
// �g��̐�������
//==========================
void CBossFlyState::Ripple(CBoss* pBoss)
{
	if (m_nCount % RIPPLE_COUNT == 0)
	{ // ��������

		// �g��̏o��ʒu���߂�
		D3DXVECTOR3 pos = pBoss->GetPos();
		pos.y += RIPPLE_HEIGHT;

		// �g��̏o��������߂�
		D3DXVECTOR3 rot = pBoss->GetRot();
		rot.x = D3DX_PI * 0.5f;
		rot.z = D3DX_PI;

		// �g��𐶐�����
		CRipple::Create
		(
			pos,
			rot,
			RIPPLE_SCALE
		);
	}
}

//==========================
// �X�e�[�W�̓����蔻��
//==========================
void CBossFlyState::StageCollision(CBoss* pBoss)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = pBoss->GetPos();

	// �X�e�[�W�̓����蔻��
	collision::StageCollision(&pos, STAGE_COLLISION_WIDTH);

	// �ʒu��K�p
	pBoss->SetPos(pos);
}