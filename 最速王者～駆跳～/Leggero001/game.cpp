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
#include "editAll.h"
#include "renderer.h"

#include "player.h"
#include "score.h"
#include "game_time.h"
#include "Item.h"
#include "goal.h"
#include "speedUI.h"
#include "Objectmesh.h"
#include "pause.h"
#include "item_generator.h"
#include "enemy.h"
#include "debugproc.h"
#include "camera_range.h"
#include "announce.h"
#include "sound.h"

#include "countdown.h"
#include "skybox.h"
#include "arrowsign.h"

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
CScore* CGame::m_pScore = nullptr;							// �X�R�A�̏��
CGameTime* CGame::m_pTime = nullptr;						// �^�C���̏��
CPause* CGame::m_pPause = nullptr;							// �|�[�Y�̏��
CGame::STATE CGame::m_GameState = STATE_START;				// �Q�[���̐i�s���
int CGame::m_nHitStopCount = 0;								// �q�b�g�X�g�b�v�J�E���g
int CGame::m_nFinishCount = 0;								// �I���J�E���g
bool CGame::m_bStartSkip = false;							// �X�^�[�g���o�̃X�L�b�v

#ifdef _DEBUG		// �f�o�b�O�p

bool CGame::m_bEdit = false;								// �G�f�B�b�g�̏��
CEditAll* CGame::m_pEditAll = nullptr;						// �G�f�B�b�g�I�[���̏��

#endif

//=========================================
// �R���X�g���N�^
//=========================================
CGame::CGame() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	m_nHitStopCount = 0;		// �q�b�g�X�g�b�v�J�E���g
	m_nFinishCount = 0;			// �I���J�E���g
	m_GameState = STATE_START;	// ���
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
	// �X�e�[�W�̐ݒ菈��
	CManager::GetFile()->SetBlock();		// �u���b�N�̐ݒu����
	CManager::GetFile()->SetObstacle();		// ��Q���̐ݒu����
	CManager::GetFile()->SetItem();			// �A�C�e���̐ݒu����
	CManager::GetFile()->SetCamera();		// �J�����̐ݒu����

	// �e�L�X�g�ǂݍ��ݏ���
	CMesh::TxtSet();

	// �X�J�C�{�b�N�X�̐ݒ菈��
	CSkyBox::Create();

	//if (m_pField == NULL)
	//{ // �t�B�[���h�ւ̃|�C���^�� NULL �̏ꍇ

	//	// �t�B�[���h�̐ݒ菈��
	//	m_pField = CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1000.0f, 0.0f, 1000.0f));
	//}

	// �V�[���̏�����
	CScene::Init();

	// ���̏�����
	m_nHitStopCount = 0;			// �q�b�g�X�g�b�v�J�E���g
	m_nFinishCount = 0;				// �I���J�E���g

	// ���Ŕ̐�������
	CArrowSign::Create(D3DXVECTOR3(300.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));

	// ������Ԃ�
	return S_OK;
}

//=============================================
//�I������
//=============================================
void CGame::Uninit(void)
{
	// �|�C���^�� NULL �ɂ���
	m_pScore = nullptr;			// �X�R�A
	m_pTime = nullptr;			// �^�C��
	m_pPause = nullptr;			// �|�[�Y

	// ��������������
	m_GameState = STATE_START;	// �Q�[���̐i�s���

	// �I���J�E���g������������
	m_nFinishCount = 0;

#ifdef _DEBUG

	if (m_pEditAll != nullptr)
	{ // �G�f�B�b�g�I�[���� NULL ����Ȃ��ꍇ

		// ���������J������
		delete m_pEditAll;
		m_pEditAll = nullptr;
	}

#endif

	// �I������
	CScene::Uninit();
}

//======================================
//�X�V����
//======================================
void CGame::Update(void)
{
	// �q�b�g�X�g�b�v����
	HitStop();

	// �|�[�Y����
	Pause();

#ifdef _DEBUG		// �f�o�b�O�p

	// �G�f�B�b�g�̐؂�ւ�����
	EditChange();

	// �X�e�[�W�̃Z�[�u����
	StageSave();

	// �G�f�B�b�g�̃A�b�v�f�[�g����
	EditUpdate();

	if (CManager::GetInputKeyboard() != nullptr)
	{ // �L�[�{�[�h�� NULL ����Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
		{ // 0�L�[���������ꍇ

			if (CManager::GetCamera() != nullptr)
			{ // �J������ NULL ����Ȃ��ꍇ

				// �J�����̑���󋵂̐؂�ւ�����
				CManager::GetCamera()->ChangeControl();
			}
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RSHIFT) == true)
		{ // �ESHIFT�L�[���������ꍇ

			if (m_pPause != nullptr)
			{ // �|�[�Y�̃|�C���^�� NULL �̏ꍇ

				// �`��󋵂̐؂�ւ�����
				m_pPause->ChangeDisp();
			}
		}
	}

#else

	// �Q�[�����̍X�V����
	GameUpdate();
#endif

	// �J�ڏ���
	Transition();

	CManager::GetDebugProc()->Print("��ԁF%d", m_GameState);
}

//======================================
//�`�揈��
//======================================
void CGame::Draw(void)
{

}

//======================================
// ���̐ݒ菈��
//======================================
void CGame::SetData(const MODE mode)
{
	// ���̐ݒ菈��
	CScene::SetData(mode);

	if (m_pScore == nullptr)
	{ // �X�R�A�ւ̃|�C���^�� NULL �̏ꍇ

		// �X�R�A�̃��������m�ۂ���
		m_pScore = CScore::Create(CManager::GetFile()->GetGameInfo().ScorePos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CManager::GetFile()->GetGameInfo().ScoreSize);
	}

	if (m_pTime == nullptr)
	{ // �^�C��UI�ւ̃|�C���^�� NULL �̏ꍇ

		// ���Ԃ̐�������
		m_pTime = CGameTime::Create(CManager::GetFile()->GetGameInfo().TimePos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CManager::GetFile()->GetGameInfo().TimeSize);
	}

	// �S�[���̐ݒ�
	CGoal::Create(CManager::GetFile()->GetGameInfo().GoalPos, MODE_GAME);

	// �c�苗���̍��m�̐ݒ�
	CAnnounce::Create();

	if (m_pPause == nullptr)
	{ // �|�[�Y�ւ̃|�C���^�� NULL �̏ꍇ

		// �|�[�Y�̐�������
		m_pPause = CPause::Create();
	}

	if (m_bStartSkip == false)
	{ // �X�^�[�g���o���X�L�b�v����ꍇ

		// �X�^�[�g��Ԃɂ���
		m_GameState = STATE_START;
	}
	else
	{ // �X�L�b�v���Ȃ��ꍇ

		// �J�E���g�_�E����Ԃɂ���
		m_GameState = STATE_COUNTDOWN;
	}

	// ���̏�����
	m_nHitStopCount = 0;			// �q�b�g�X�g�b�v�J�E���g
	m_nFinishCount = 0;				// �I���J�E���g

	if (m_GameState == STATE_START)
	{ // �X�^�[�g��Ԃ̏ꍇ

		if (CManager::GetCamera() != nullptr)
		{ // �J�����̏�� NULL ����Ȃ��ꍇ

			// �X�e�[�W���e�J�����ɐݒ肷��
			CManager::GetCamera()->SetType(CCamera::TYPE_STARTSTAGE);
		}

		// �v���C���[�̈ʒu��ݒ肷��
		GetPlayer()->SetPos(CManager::GetFile()->GetGameInfo().PlayerPos);
	}
	else
	{ // ��L�ȊO

		if (CManager::GetCamera() != nullptr)
		{ // �J�����̏�� NULL ����Ȃ��ꍇ

			// �ʏ�J�����ɐݒ肷��
			CManager::GetCamera()->SetType(CCamera::TYPE_NONE);
		}

		// �v���C���[�̈ʒu��ݒ肷��
		GetPlayer()->SetPos(CManager::GetFile()->GetGameInfo().StartPos);

		// �J�E���g�_�E���̐���
		CCountdown::Create();
	}

	// ���o�X�L�b�v�� ON �ɂ���
	m_bStartSkip = true;
}

//======================================
// �Q�[�����̍X�V����
//======================================
void CGame::GameUpdate(void)
{
	if (CManager::GetRenderer() != nullptr)
	{ // �����_���[�� NULL ����Ȃ��ꍇ

		if (CGame::GetState() != CGame::STATE_HITSTOP)
		{ // �q�b�g�X�g�b�v��ԈȊO�̏ꍇ

			// �����_���[�̍X�V
			CManager::GetRenderer()->Update();
		}
	}
}

//======================================
// �q�b�g�X�g�b�v����
//======================================
void CGame::HitStop(void)
{
	if (m_GameState == STATE_HITSTOP)
	{ // �Q�[���̐i�s��Ԃ��q�b�g�X�g�b�v�̏ꍇ

		// �q�b�g�X�g�b�v�J�E���g�����Z����
		m_nHitStopCount--;

		if (m_nHitStopCount <= 0)
		{ // �q�b�g�X�g�b�v�J�E���g�� 0 �ȉ��ɂȂ����ꍇ

			// �Q�[���̐i�s��Ԃ�ݒ肷��
			m_GameState = STATE_PLAY;
		}
	}
}

//======================================
// �|�[�Y����
//======================================
void CGame::Pause(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_BACK, 0) == true)
	{ // P�L�[���������ꍇ

		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE &&
			m_GameState != STATE_GOAL &&
			m_GameState != STATE_FINISH)
		{ // �t�F�[�h������Ԃ��A�v���C���̏ꍇ

			if (m_pPause->GetPause() == false)
			{ // �|�[�Y�� false �������ꍇ

				// �|�[�Y�󋵂� true �ɂ���
				m_pPause->SetPause(true);
			}
			else
			{ // �|�[�Y�� true �������ꍇ

				// �|�[�Y�󋵂� false �ɂ���
				m_pPause->SetPause(false);
			}

			// ���艹��炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//======================================
// �J�ڏ���
//======================================
void CGame::Transition(void)
{
	if (m_GameState == STATE_FINISH)
	{ // �Q�[�����I����Ԃ̏ꍇ

		// �I���J�E���g�����Z����
		m_nFinishCount++;

		if (m_nFinishCount % 80 == 0)
		{ // �I���J�E���g����萔�𒴂����ꍇ

			// ���U���g�ɑJ�ڂ���
			CManager::GetFade()->SetFade(CScene::MODE_RESULT);
		}
	}
}

//======================================
// �X�R�A�̎擾����
//======================================
CScore* CGame::GetScore(void)
{
	// �X�R�A�̏���Ԃ�
	return m_pScore;
}

//======================================
// �^�C���̎擾����
//======================================
CGameTime* CGame::GetTime(void)
{
	// �^�C���̏���Ԃ�
	return m_pTime;
}

//======================================
// �|�[�Y�̎擾����
//======================================
CPause* CGame::GetPause(void)
{
	// �|�[�Y�̏���Ԃ�
	return m_pPause;
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
// �Q�[���̃q�b�g�X�g�b�v�̐ݒ菈��
//======================================
void CGame::SetHitStop(const int nCount)
{
	// �Q�[���̐i�s��Ԃ�ݒ肷��
	m_GameState = STATE_HITSTOP;

	// �q�b�g�X�g�b�v�J�E���g��ݒ肷��
	m_nHitStopCount = nCount;
}

//======================================
// �X�R�A��NULL������
//======================================
void CGame::DeleteScore(void)
{
	// �X�R�A�̃|�C���^�� NULL �ɂ���
	m_pScore = nullptr;
}

//======================================
// �^�C����NULL������
//======================================
void CGame::DeleteTime(void)
{
	// �^�C���̃|�C���^�� NULL �ɂ���
	m_pTime = nullptr;
}

//======================================
// �|�[�Y��NULL������
//======================================
void CGame::DeletePause(void)
{
	// �|�[�Y�̃|�C���^�� NULL �ɂ���
	m_pPause = nullptr;
}

#ifdef _DEBUG		// �f�o�b�O�p

//======================================
// �G�f�B�b�g�̐؂�ւ�����
//======================================
void CGame::EditChange(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9) == true)
	{ // F9�L�[���������ꍇ

		// �G�f�B�b�g���[�h�����ւ���
		m_bEdit = m_bEdit ? false : true;

		if (m_bEdit == true)
		{ // �G�f�B�b�g���[�h�� ON �ɂ����ꍇ

			if (m_pEditAll == nullptr)
			{ // �G�f�B�b�g�I�[���� NULL �̏ꍇ

				// �G�f�B�b�g�̐�������
				m_pEditAll = m_pEditAll->CEditAll::Create();
			}
		}
		else
		{ // �G�f�B�b�g���[�h�� OFF �ɂ����ꍇ

			if (m_pEditAll != nullptr)
			{ // �|�C���^�� NULL ����Ȃ��ꍇ

				// �G�f�B�b�g�̏I������
				m_pEditAll->Uninit();

				// ���������J������
				delete m_pEditAll;
				m_pEditAll = nullptr;
			}
		}
	}
}

//======================================
// �X�e�[�W�Z�[�u����
//======================================
void CGame::StageSave(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7) == true)
	{ // 7�L�[���������ꍇ

		// �u���b�N�̃Z�[�u����
		CManager::GetFile()->Save(CFile::TYPE_BLOCK);

		// ��Q���̃Z�[�u����
		CManager::GetFile()->Save(CFile::TYPE_OBSTACLE);

		// �A�C�e���̃Z�[�u����
		CManager::GetFile()->Save(CFile::TYPE_ITEM);

		// �J�����͈͂̃Z�[�u����
		CManager::GetFile()->Save(CFile::TYPE_CAMERA);
	}
}

//======================================
// �G�f�B�b�g�̃A�b�v�f�[�g����
//======================================
void CGame::EditUpdate(void)
{
	if (m_bEdit == true)
	{ // �G�f�B�b�g���[�h�̏ꍇ

		if (m_pEditAll != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �X�V����
			m_pEditAll->Update();

			for (int nCntPriority = 0; nCntPriority < CObject::PRIORITY_MAX; nCntPriority++)
			{
				// ���S���菈��
				CObject::DeathDecision(nCntPriority);
			}
		}
	}
	else
	{ // ��L�ȊO

		// �Q�[�����̍X�V����
		GameUpdate();
	}
}

//======================================
// �G�f�B�b�g�̎擾����
//======================================
bool CGame::GetEdit(void)
{
	// �G�f�B�b�g���[�h�̏���Ԃ�
	return m_bEdit;
}

//======================================
// �G�f�B�b�g�I�[���̎擾����
//======================================
CEditAll* CGame::GetEditAll(void)
{
	// �G�f�B�b�g�I�[���̏���Ԃ�
	return m_pEditAll;
}

#endif