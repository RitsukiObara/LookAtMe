//============================================
//
// �Q�[���̃v���C���[�̃��C������[game_player.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "game_player.h"
#include "motion.h"
#include "manager.h"
#include "file.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"

// �}�N����`
#define EMERGENCY_STAGELEFT		(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f))		// �ً}�p�̃X�e�[�W�̍��[

//=========================================
// �R���X�g���N�^
//=========================================
CGamePlayer::CGamePlayer() : CPlayer(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// �S�Ă̒l���N���A����
	m_stageLeft = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �X�e�[�W�̍��[
}

//=========================================
// �f�X�g���N�^
//=========================================
CGamePlayer::~CGamePlayer()
{

}

//===========================================
// �v���C���[�̏���������
//===========================================
HRESULT CGamePlayer::Init(void)
{
	if (FAILED(CPlayer::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	if (CManager::GetFile() != nullptr)
	{ // �t�@�C���̏�� NULL ����Ȃ��ꍇ

		// �X�e�[�W�̍��[��ݒ肷��
		m_stageLeft = CManager::GetFile()->GetGameInfo().StageLeftPos;
	}
	else
	{ // ��L�ȊO

		// �X�e�[�W�̍��[��ݒ肷��
		m_stageLeft = EMERGENCY_STAGELEFT;
	}

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �v���C���[�̏I������
//===========================================
void CGamePlayer::Uninit(void)
{
	// �I��
	CPlayer::Uninit();
}

//===========================================
// �v���C���[�̍X�V����
//===========================================
void CGamePlayer::Update(void)
{
	switch (CGame::GetState())
	{
	case CGame::STATE_START:		// �X�^�[�g���

		// �X�^�[�g��Ԃ̍X�V����
		StartUpdate();

		break;

	case CGame::STATE_COUNTDOWN:	// �J�E���g�_�E�����

		// �J�E���g�_�E����Ԃ̍X�V����
		CountDownUpdate();

		break;

	case CGame::STATE_PLAY:			// �Q�[�����

		// �ʏ��Ԃ̍X�V����
		MainUpdate();

		break;

	case CGame::STATE_GOAL:			// �S�[�����

		// �S�[����Ԃ̍X�V����
		GoalUpdate();

		break;

	case CGame::STATE_FINISH:		// �I�����

		// �I����Ԃ̍X�V����
		FinishUpdate();

		break;
	}

	// �X�e�[�W�̍��[�Ƃ̓����蔻��
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
void CGamePlayer::Draw(void)
{
	// �`�揈��
	CPlayer::Draw();
}

//===========================================
// �X�e�[�W�̍��[�̓����蔻��
//===========================================
void CGamePlayer::CollisionStageLeft(void)
{
	// �ʒu��ݒ肷��
	D3DXVECTOR3 pos = GetPos();

	if (pos.x <= m_stageLeft.x)
	{ // �X�e�[�W�̍��[��荶�ɂ����ꍇ

		// �v���C���[�̈ʒu���X�e�[�W�̍��[�ɓ�����
		pos.x = m_stageLeft.x;
	}

	// �ʒu���X�V����
	SetPos(pos);
}