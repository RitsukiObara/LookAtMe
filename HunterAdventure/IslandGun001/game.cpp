//============================================
//
// �Q�[���̃��C������[game.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "file.h"
#include "light.h"
#include "renderer.h"
#include "texture.h"

#include "debugproc.h"
#include "sound.h"

#include "objectElevation.h"
#include "skybox.h"
#include "motion.h"

#include "pause.h"
#include "player.h"
#include "ocean.h"
#include "game_score.h"
#include "alter.h"
#include "continueUI.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
namespace
{
	const int TRANS_COUNT = 80;			// �J�ڃJ�E���g
	const int GAMEOVER_COUNT = 150;		// �Q�[���I�[�o�[�J�E���g
	const char* ELEVATION_TXT = "data/TXT/Elevation.txt";		// �N���n�ʂ̃e�L�X�g
}

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
CPause* CGame::m_pPause = nullptr;							// �|�[�Y�̏��
CPlayer* CGame::m_pPlayer = nullptr;						// �v���C���[�̏��
CGameScore* CGame::m_pGameScore = nullptr;					// �Q�[���X�R�A�̏��
CAlter* CGame::m_pAlter = nullptr;							// �Ւd�̏��
CGame::STATE CGame::m_state = CGame::STATE_START;			// �Q�[���̐i�s���
int CGame::m_nScore = 0;									// �X�R�A
bool CGame::m_bPause = false;								// �|�[�Y��

//=========================================
// �R���X�g���N�^
//=========================================
CGame::CGame()
{
	// �S�Ă̒l���N���A����
	m_nStateCount = 0;			// �I���J�E���g

	m_pPause = nullptr;			// �|�[�Y
	m_pPlayer = nullptr;		// �v���C���[
	m_pGameScore = nullptr;		// �X�R�A
	m_pAlter = nullptr;			// �Ւd
	m_state = STATE_START;		// ���
	m_nScore = 0;				// �X�R�A
	m_bPause = false;			// �|�[�Y��

	// ���_�̌��Z�ʂ̃��Z�b�g����
	CContinueUI::SubScoreReset();
}

//=========================================
// �f�X�g���N�^
//=========================================
CGame::~CGame()
{

}

//=========================================
//����������
//=========================================
HRESULT CGame::Init(void)
{
	// �X�^�[�g�J�����ŕ`��
	CManager::Get()->GetCamera()->Reset();
	CManager::Get()->GetCamera()->SetType(CCamera::TYPE_START);

	// �e�L�X�g�ǂݍ��ݏ���
	CElevation::TxtSet(ELEVATION_TXT);

	// ���[�V�����̓ǂݍ��ݏ���
	CMotion::Load(CMotion::STYLE_PLAYER);		// �v���C���[
	CMotion::Load(CMotion::STYLE_TORDLE);		// �^�[�h��
	CMotion::Load(CMotion::STYLE_IWAKARI);		// �C���J��
	CMotion::Load(CMotion::STYLE_BOSS);			// �{�X

	// �}�b�v�̐���
	CManager::Get()->GetFile()->SetEnemy();
	CManager::Get()->GetFile()->SetCoin();
	CManager::Get()->GetFile()->SetGoldBone();
	CManager::Get()->GetFile()->SetTree();
	CManager::Get()->GetFile()->SetRock();
	CManager::Get()->GetFile()->SetBlock ();
	CManager::Get()->GetFile()->SetBangFlower();
	CManager::Get()->GetFile()->SetWall();

	// �X�J�C�{�b�N�X�̐�������
	CSkyBox::Create();

	// �V�[���̏�����
	CScene::Init();

	// �C�̐���
	COcean::Create();

	// �v���C���[�̐�������
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-400.0f, 0.0f, 300.0f));

	// �Q�[���X�R�A�̐���
	m_pGameScore = CGameScore::Create();

	// �Ւd�̐���
	m_pAlter = CAlter::Create();

	// ���̏�����
	m_nStateCount = 0;			// �I���J�E���g
	m_state = STATE_START;		// ���
	m_bPause = false;			// �|�[�Y��

	// ������Ԃ�
	return S_OK;
}

//=============================================
//�I������
//=============================================
void CGame::Uninit(void)
{
	if (m_pPause != nullptr)
	{ // �|�[�Y�� NULL ����Ȃ��ꍇ

		// �I������
		m_pPause->Uninit();
	}

	// �|�C���^�� NULL �ɂ���
	m_pPause = nullptr;			// �|�[�Y
	m_pPlayer = nullptr;		// �v���C���[
	m_pGameScore = nullptr;		// �Q�[���X�R�A

	// ��������������
	m_state = STATE_START;		// �Q�[���̐i�s���
	m_bPause = false;			// �|�[�Y��

	// �I���J�E���g������������
	m_nStateCount = 0;

	if (CManager::Get()->GetLight() != nullptr)
	{ // ���C�g�� NULL ����Ȃ��ꍇ

		// ���C�g�̐F�����Z�b�g����
		CManager::Get()->GetLight()->ResetCol();
	}

	if (CManager::Get()->GetRenderer() != nullptr)
	{ // �����_���[�� NULL ����Ȃ��ꍇ

		// ���̐F�����Z�b�g����
		CManager::Get()->GetRenderer()->SetFogCol(NONE_D3DXCOLOR);
	}

	// �I������
	CScene::Uninit();
}

//======================================
//�X�V����
//======================================
void CGame::Update(void)
{
	switch (m_state)
	{
	case CGame::STATE_START:

		if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
			CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
			CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
		{ // ENTER�L�[���������ꍇ

			if (m_pPlayer != nullptr)
			{ // �v���C���[�����݂���ꍇ

				// ��s�@��������
				m_pPlayer->ArrivalAirplane();
			}
		}

		// �|�[�Y����
		Pause();

		break;

	case CGame::STATE_PLAY:
	case CGame::STATE_BOSSMOVIE:

		// �|�[�Y����
		Pause();

		break;

	case CGame::STATE_GAMEOVER:

		if (m_pGameScore->GetScore() >= CContinueUI::GetSubScore())
		{ // �����o����قǃX�R�A������ꍇ

			// �|�[�Y����
			Pause();

			// �I���J�E���g�����Z����
			m_nStateCount++;

			if (m_nStateCount % GAMEOVER_COUNT == 0)
			{ // ��ԃJ�E���g����萔�ȏ�ɂȂ����ꍇ

				// ��ԃJ�E���g�����Z�b�g����
				m_nStateCount = 0;

				// �R���e�B�j���[��Ԃɂ���
				m_state = STATE_CONTINUE;

				// �R���e�B�j���[UI�𐶐�
				CContinueUI::Create();
			}
		}
		else
		{ // ��L�ȊO

			// �I�������ɂ���
			m_state = STATE_FINISH;
		}

		break;

	case CGame::STATE_CONTINUE:

		// �R���e�B�j���[UI�݂̂𓮂���
		CObject::AnyUpdate(CObject::TYPE_CONTINUEUI);

		break;

	case CGame::STATE_FINISH:

		// �J�ڏ���
		Transition();

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	if (m_bPause == true)
	{ // �|�[�Y�󋵂� true �̏ꍇ

		// �X�V����
		m_pPause->Update();
	}
	else if(m_state != STATE_CONTINUE)
	{ // ��L�ȊO

		if (CManager::Get()->GetRenderer() != nullptr)
		{ // �����_���[�� NULL ����Ȃ��ꍇ

			// �����_���[�̍X�V
			CManager::Get()->GetRenderer()->Update();
		}
	}

	CManager::Get()->GetDebugProc()->Print("��ԁF%d", m_state);
}

//======================================
//�`�揈��
//======================================
void CGame::Draw(void)
{
	if (m_pPause != nullptr &&
		m_bPause == true)
	{ // �|�[�Y���̏ꍇ

		// �`�揈��
		m_pPause->Draw();
	}
}

//======================================
// ���̐ݒ菈��
//======================================
void CGame::SetData(const MODE mode)
{
	// ���̐ݒ菈��
	CScene::SetData(mode);

	// �S�Ă̒l��ݒ肷��
	m_state = STATE_START;		// �X�^�[�g��Ԃɂ���
	m_bPause = false;			// �|�[�Y��

	// ���̏�����
	m_nStateCount = 0;				// �I���J�E���g
}

//======================================
// �|�[�Y����
//======================================
void CGame::Pause(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_P) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_BACK, 0) == true)
	{ // P�L�[���������ꍇ

		if (CManager::Get()->GetFade()->GetFade() == CFade::FADE_NONE)
		{ // �t�F�[�h������Ԃ��A�I���ȊO�̏ꍇ

			if (m_bPause == false)
			{ // �|�[�Y�� false �������ꍇ

				if (m_pPause == nullptr)
				{ // �|�[�Y�ւ̃|�C���^�� NULL �̏ꍇ

					// �|�[�Y�̐�������
					m_pPause = CPause::Create();
				}
				else
				{ // ��L�ȊO
					
					// ��~
					assert(false);
				}

				// �|�[�Y�󋵂� true �ɂ���
				m_bPause = true;
			}
			else
			{ // �|�[�Y�� true �������ꍇ

				if (m_pPause != nullptr)
				{ // �|�[�Y�ւ̃|�C���^�� NULL ����Ȃ��ꍇ

					// �I������
					m_pPause->Uninit();
					m_pPause = nullptr;
				}

				// �|�[�Y�󋵂� false �ɂ���
				m_bPause = false;
			}

			// ���艹��炷
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//======================================
// �J�ڏ���
//======================================
void CGame::Transition(void)
{
	// �I���J�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount % TRANS_COUNT == 0)
	{ // �I���J�E���g����萔�𒴂����ꍇ

		if (m_pGameScore != nullptr)
		{ // �Q�[���X�R�A�̏�� NULL ����Ȃ��ꍇ

			// �X�R�A���擾����
			m_nScore = m_pGameScore->GetScore();
		}

		// ���U���g�ɑJ�ڂ���
		CManager::Get()->GetFade()->SetFade(CScene::MODE_RESULT);
	}
}

//======================================
// �|�[�Y�󋵂̐ݒ菈��
//======================================
void CGame::SetEnablePause(const bool bPause)
{
	// �|�[�Y�󋵂�ݒ肷��
	m_bPause = bPause;
}

//======================================
// �|�[�Y�󋵂̎擾����
//======================================
bool CGame::IsPause(void)
{
	// �|�[�Y�󋵂��擾����
	return m_bPause;
}

//======================================
// �Q�[���̐i�s��Ԃ̐ݒ菈��
//======================================
void CGame::SetState(const STATE state)
{
	// �Q�[���̐i�s��Ԃ�ݒ肷��
	m_state = state;
}

//======================================
// �Q�[���̐i�s��Ԃ̎擾����
//======================================
CGame::STATE CGame::GetState(void)
{
	// �Q�[���̐i�s��Ԃ�Ԃ�
	return m_state;
}

//======================================
// �v���C���[�̎擾����
//======================================
CPlayer* CGame::GetPlayer(void)
{
	// �v���C���[�̃|�C���^��Ԃ�
	return m_pPlayer;
}

//======================================
// �Q�[���X�R�A�̎擾����
//======================================
CGameScore* CGame::GetGameScore(void)
{
	// �Q�[���X�R�A�̃|�C���^��Ԃ�
	return m_pGameScore;
}

//======================================
// �Ւd�̎擾����
//======================================
CAlter* CGame::GetAlter(void)
{
	// �Ւd�̃|�C���^��Ԃ�
	return m_pAlter;
}

//======================================
// �����X�R�A�̎擾����
//======================================
int CGame::GetScore(void)
{
	// �����X�R�A��Ԃ�
	return m_nScore;
}

//======================================
// �|�[�Y��NULL������
//======================================
void CGame::DeletePause(void)
{
	// �|�[�Y�̃|�C���^�� NULL �ɂ���
	m_pPause = nullptr;
}

//======================================
// �v���C���[��NULL������
//======================================
void CGame::DeletePlayer(void)
{
	// �v���C���[�̃|�C���^�� NULL �ɂ���
	m_pPlayer = nullptr;
}

//======================================
// �X�R�A��NULL������
//======================================
void CGame::DeleteGameScore(void)
{
	// �X�R�A�̃|�C���^�� NULL �ɂ���
	m_pGameScore = nullptr;
}

//======================================
// �Ւd��NULL������
//======================================
void CGame::DeleteAlter(void)
{
	// �Ւd�̃|�C���^�� NULL �ɂ���
	m_pAlter = nullptr;
}