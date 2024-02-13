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
#include "renderer.h"
#include "texture.h"

#include "pause.h"
#include "debugproc.h"
#include "sound.h"

#include "objectElevation.h"
#include "skybox.h"
#include "motion.h"

#include "player.h"
#include "ocean.h"
#include "boss.h"
#include "enemy.h"
#include "list_manager.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define TRANS_COUNT		(80)		// �������̑J�ڃJ�E���g

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
CPause* CGame::m_pPause = nullptr;							// �|�[�Y�̏��
CPlayer* CGame::m_pPlayer = nullptr;						// �v���C���[�̏��
CGame::STATE CGame::m_GameState = CGame::STATE_START;		// �Q�[���̐i�s���
int CGame::m_nFinishCount = 0;								// �I���J�E���g
bool CGame::m_bPause = false;								// �|�[�Y��

//=========================================
// �R���X�g���N�^
//=========================================
CGame::CGame()
{
	// �S�Ă̒l���N���A����
	m_pPause = nullptr;			// �|�[�Y
	m_pPlayer = nullptr;		// �v���C���[
	m_nFinishCount = 0;			// �I���J�E���g
	m_GameState = STATE_START;	// ���
	m_bPause = false;			// �|�[�Y��
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
	// �e�L�X�g�ǂݍ��ݏ���
	CElevation::TxtSet();

	// ���b�V���̃e�L�X�g�ǂݍ���
	//CMesh::TxtSet();

	//if (m_pField == NULL)
	//{ // �t�B�[���h�ւ̃|�C���^�� NULL �̏ꍇ

	//	// �t�B�[���h�̐ݒ菈��
	//	m_pField = CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1000.0f, 0.0f, 1000.0f));
	//}

	// ���[�V�����̓ǂݍ��ݏ���
	CMotion::Load(CMotion::STYLE_PLAYER);		// �v���C���[
	CMotion::Load(CMotion::STYLE_TORDLE);		// �^�[�h��
	CMotion::Load(CMotion::STYLE_IWAKARI);		// �C���J��
	CMotion::Load(CMotion::STYLE_BOSS);			// �{�X

	// �}�b�v�����[�h
	CManager::Get()->GetFile()->Load(CFile::TYPE_ENEMY);
	CManager::Get()->GetFile()->Load(CFile::TYPE_COIN);
	CManager::Get()->GetFile()->Load(CFile::TYPE_GOLDBONE);
	CManager::Get()->GetFile()->Load(CFile::TYPE_TREE);
	CManager::Get()->GetFile()->Load(CFile::TYPE_ROCK);
	CManager::Get()->GetFile()->Load(CFile::TYPE_BLOCK);
	CManager::Get()->GetFile()->Load(CFile::TYPE_BANGFLOWER);
	CManager::Get()->GetFile()->Load(CFile::TYPE_WALL);

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

	// ���̏�����
	m_nFinishCount = 0;			// �I���J�E���g
	m_GameState = STATE_START;	// ���
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

	// ��������������
	m_GameState = STATE_START;	// �Q�[���̐i�s���
	m_bPause = false;			// �|�[�Y��

	// �I���J�E���g������������
	m_nFinishCount = 0;

	// �I������
	CScene::Uninit();
}

//======================================
//�X�V����
//======================================
void CGame::Update(void)
{
	switch (m_GameState)
	{
	case CGame::STATE_START:

		// �|�[�Y����
		Pause();

		break;

	case CGame::STATE_PLAY:

		// �|�[�Y����
		Pause();

		break;

	case CGame::STATE_GOAL:

		// �J�ڏ���
		Transition();

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	//if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
	//	CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
	//	CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	//{ // ENTER�L�[���������ꍇ

	//	// �`���[�g���A���ɑJ�ڂ���
	//	CManager::Get()->GetFade()->SetFade(CScene::MODE_RESULT);

	//	// ���̐�̏������s��Ȃ�
	//	return;
	//}

	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // 0�L�[��������

		// �{�X�̐�������
		CBoss::Create(NONE_D3DXVECTOR3, NONE_D3DXVECTOR3);
	}

	if (m_bPause == true)
	{ // �|�[�Y�󋵂� true �̏ꍇ

		// �X�V����
		m_pPause->Update();
	}
	else
	{ // ��L�ȊO

		if (CManager::Get()->GetRenderer() != nullptr)
		{ // �����_���[�� NULL ����Ȃ��ꍇ

			// �����_���[�̍X�V
			CManager::Get()->GetRenderer()->Update();
		}

		if (CEnemy::GetList().IsEmpty() == true &&
			m_GameState != STATE_GOAL)
		{ // �G��S�ē|�����ꍇ

			// �S�[����Ԃɂ���
			m_GameState = STATE_GOAL;
		}
	}

	CManager::Get()->GetDebugProc()->Print("��ԁF%d", m_GameState);
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
	m_GameState = STATE_START;	// �X�^�[�g��Ԃɂ���
	m_bPause = false;			// �|�[�Y��

	// ���̏�����
	m_nFinishCount = 0;				// �I���J�E���g
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
	m_nFinishCount++;

	if (m_nFinishCount % TRANS_COUNT == 0)
	{ // �I���J�E���g����萔�𒴂����ꍇ

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
	m_GameState = state;
}

//======================================
// �Q�[���̐i�s��Ԃ̎擾����
//======================================
CGame::STATE CGame::GetState(void)
{
	// �Q�[���̐i�s��Ԃ�Ԃ�
	return m_GameState;
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