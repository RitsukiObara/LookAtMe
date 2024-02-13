//============================================
//
// �`���[�g���A���̃v���C���[�̃��C������[tutorial_player.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "tutorial_player.h"
#include "tutorial.h"
#include "motion.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "collision.h"

#include "balloon.h"
#include "balloonUI.h"
#include "push_timing.h"
#include "speedUI.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define POLEJUMP_SPEED			(11.0f)			// �X���W�����v�̃X�s�[�h
#define PUSH_TIMING_ADD_POS		(D3DXVECTOR3(-90.0f, 80.0f, 0.0f))		// �����^�C�~���O�̒ǉ��ʒu
#define PUSH_CLIFFCLIMB_COUNT	(13)			// �R�o�莞�̉����^�C�~���O�̕\������J�E���g
#define PUSH_CLIFFLAND_COUNT	(5)				// �R���n���̉����^�C�~���O�̕\������J�E���g
#define PUSH_HANGING_ROT		(0.9f)			// �Ԃ牺���莞�̉����^�C�~���O�̕\���������
#define STAGE_LEFT				(-700.0f)		// �X�e�[�W�̍��[

//=========================================
// �R���X�g���N�^
//=========================================
CTutorialPlayer::CTutorialPlayer() : CPlayer(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// �S�Ă̒l���N���A����
	m_pPushTiming = nullptr;		// �����^�C�~���O�̃|�C���^
}

//=========================================
// �f�X�g���N�^
//=========================================
CTutorialPlayer::~CTutorialPlayer()
{

}

//===========================================
// �v���C���[�̏���������
//===========================================
HRESULT CTutorialPlayer::Init(void)
{
	if (FAILED(CPlayer::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	m_pPushTiming = nullptr;		// �����^�C�~���O�̃|�C���^

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �v���C���[�̏I������
//===========================================
void CTutorialPlayer::Uninit(void)
{
	if (m_pPushTiming != nullptr)
	{ // �����^�C�~���O�� NULL ����Ȃ��ꍇ

		// �I������
		m_pPushTiming->Uninit();
		m_pPushTiming = nullptr;
	}

	// �I��
	CPlayer::Uninit();
}

//===========================================
// �v���C���[�̍X�V����
//===========================================
void CTutorialPlayer::Update(void)
{
	// �v���C���[�̐����o���Ɠ����蔻��
	collision::PlayerBalloonHit(this);

	if (CTutorial::GetState() != CTutorial::STATE_STAMINA)
	{ // �X�^�~�i���[�h�ȊO�̏ꍇ

		// �X�^�~�i��S�񕜂�����
		CalcStamina(PLAYER_MAX_STAMINA);
	}

	if (CTutorial::GetState() == CTutorial::STATE_BOOST)
	{ // �u�[�X�g���[�h�̏ꍇ

		if (GetSpeed() < PLAYER_MAX_SPEED)
		{ // ���x���ő�l�����̏ꍇ

			// ���x���ő�ɂ���
			SetSpeed(PLAYER_MAX_SPEED);
		}
	}
	else if (CTutorial::GetState() == CTutorial::STATE_POLE)
	{ // �X���W�����v�̏ꍇ

		if (GetSpeed() < POLEJUMP_SPEED)
		{ // ���x�����̑��x�ȉ��ɂȂ����ꍇ

			// ���x��ݒ肷��
			SetSpeed(POLEJUMP_SPEED);
		}

		if (GetState() == STATE_BOOST)
		{ // �u�[�X�g��Ԃ̏ꍇ

			// �ʏ��Ԃɂ���
			SetState(STATE_NONE);
		}
	}
	else
	{ // ��L�ȊO

		if (GetState() == STATE_BOOST)
		{ // �u�[�X�g��Ԃ̏ꍇ

			// �ʏ��Ԃɂ���
			SetState(STATE_NONE);
		}
	}

	if (CTutorial::GetState() == CTutorial::STATE_END)
	{ // �I����Ԃ̏ꍇ

		// �E����������
		SetRight(true);

		// �I�����̍X�V����
		FinishUpdate();
	}
	else
	{ // ��L�ȊO

		// �ʏ��Ԃ̍X�V����
		MainUpdate();
	}

	// �����^�C�~���O�̈ʒu�ݒ菈��
	m_pPushTiming->SetPos(D3DXVECTOR3(GetPos().x + PUSH_TIMING_ADD_POS.x, GetPos().y + PUSH_TIMING_ADD_POS.y, GetPos().z));

	if (GetAction()->GetAct() == CPlayerAct::ACT_CLIFFCLIMB &&
		GetAction()->GetCliffCatch() == true &&
		GetAction()->GetActCount() >= PUSH_CLIFFCLIMB_COUNT)
	{ // �R�o��̏ꍇ
		
		// �`��󋵂�ݒ肷��
		m_pPushTiming->SetDisp(true);
	}
	else if (GetAction()->GetAct() == CPlayerAct::ACT_CLIFFLAND &&
		GetAction()->GetCliffCatch() == true &&
		GetAction()->GetActCount() <= PUSH_CLIFFLAND_COUNT)
	{ // �R���n�̏ꍇ

		// �`��󋵂�ݒ肷��
		m_pPushTiming->SetDisp(true);
	}
	else if (GetAction()->GetAct() == CPlayerAct::ACT_HANGING &&
		GetRot().x >= PUSH_HANGING_ROT)
	{ // �Ԃ牺����̏ꍇ

		// �`��󋵂�ݒ肷��
		m_pPushTiming->SetDisp(true);
	}
	else if (GetAction()->GetAct() == CPlayerAct::ACT_WALLFRIC &&
		GetStamina() >= PLAYER_WALLKICK_STAMINA)
	{ // �ǎC���Ԃ̏ꍇ

		// �`��󋵂�ݒ肷��
		m_pPushTiming->SetDisp(true);
	}
	else
	{ // ��L�ȊO

		// �`��󋵂�ݒ肷��
		m_pPushTiming->SetDisp(false);
	}

	// �X�e�[�W�̍��[�̓����蔻��
	CollisionStageLeft();

	// �v���C���[�̃f�o�b�O�E������@�\��
	CManager::GetDebugProc()->Print
	(
		"�ʒu[%f %f %f]\n\n�ړ�[A/D]\n�W�����v[SPACE]\n\n�����F%f\n�W�����v�󋵁F%d\n�s���󋵁F%d\n���x�F%f\n���[�V�����̎�ށF%d\n"
		, GetPos().x, GetPos().y, GetPos().z, GetRot().y, GetJump(), GetAction()->GetAct(), GetSpeed(), GetMotion()->GetType()
	);
}

//===========================================
// �v���C���[�̕`�揈��
//===========================================
void CTutorialPlayer::Draw(void)
{
	// �`�揈��
	CPlayer::Draw();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CTutorialPlayer::SetData(const D3DXVECTOR3& pos, const CScene::MODE mode)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 PushPos;		// �����^�C�~���O�̈ʒu

	// ���̐ݒ菈��
	CPlayer::SetData(pos, mode);

	// �����^�C�~���O�̈ʒu��ݒ肷��
	PushPos = D3DXVECTOR3(pos.x + PUSH_TIMING_ADD_POS.x, pos.y + PUSH_TIMING_ADD_POS.y, pos.z);

	// �����^�C�~���O�𐶐�
	m_pPushTiming = CPushTiming::Create(PushPos);
}

//===========================================
// �X�e�[�W�̍��[�Ƃ̓����蔻��
//===========================================
void CTutorialPlayer::CollisionStageLeft(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();

	if (pos.x <= STAGE_LEFT)
	{ // �X�e�[�W�̍��[��荶�ɂ����ꍇ

		// �v���C���[�̈ʒu�𓮂���
		pos.x = STAGE_LEFT;
	}

	// �ʒu���X�V����
	SetPos(pos);
}