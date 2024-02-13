//=======================================
//
// �Q�[���̃S�[���̃��C������[game_goal.cpp]
// Author�F��������
//
//=======================================
#include "game.h"
#include "manager.h"
#include "game_goal.h"

#include "collision.h"
#include "game_time.h"
#include "goalUI.h"
#include "player.h"
#include "score.h"
#include "sound.h"

//---------------------------------------
// �}�N����`
//---------------------------------------

//==========================================
// �R���X�g���N�^
//==========================================
CGameGoal::CGameGoal() : CGoal(CObject::TYPE_GOAL, CObject::PRIORITY_PLAYER)
{

}

//==========================================
// �f�X�g���N�^
//==========================================
CGameGoal::~CGameGoal()
{

}

//==========================================
// �S�[���̏���������
//==========================================
HRESULT CGameGoal::Init(void)
{
	if (FAILED(CGoal::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//========================================
// �S�[���̏I������
//========================================
void CGameGoal::Uninit(void)
{
	// �I������
	CGoal::Uninit();
}

//========================================
// �S�[���̍X�V����
//========================================
void CGameGoal::Update(void)
{
	// �����x�̕ω�
	AlphaChange();

	// �S�[���ƃv���C���[�̓����蔻��
	if (collision::GoalPlayerCollision(this) == true)
	{ // �����蔻�肪 true �������ꍇ

		// �S�[�����̊�����炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GOALCHEERS);

		if (CScene::GetPlayer() != nullptr)
		{ // �v���C���[�� NULL ����Ȃ��ꍇ

			// �S�[��UI�̐�������
			CGoalUI::Create(CScene::GetPlayer()->GetPos());
		}

		if (CGame::GetTime() != nullptr)
		{ // �^�C���̏�񂪂������ꍇ

			// �N���A�^�C����ݒ肷��
			CManager::SetClearTime(CGame::GetTime()->GetTotalTime());
		}

		if (CGame::GetScore() != nullptr)
		{ // �X�R�A�̏�񂪂������ꍇ

			// �A�N�V�����X�R�A��ݒ肷��
			CManager::SetScore(CGame::GetScore()->GetScore());
		}

		// �N���A�󋵂̐ݒ菈��
		CManager::SetClear(true);

		// �Q�[���̐i�s��Ԃ̐ݒ菈��
		CGame::SetState(CGame::STATE_GOAL);

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//===========================================
// �S�[���̕`�揈��
//===========================================
void CGameGoal::Draw(void)
{
	// �`�揈��
	CGoal::Draw();
}