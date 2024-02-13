//============================================
//
// ���ԏ���[time.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "game_time.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "texture.h"

#include "pause.h"

// �}�N����`
#define GAMETIME_DOT_SIZE		(D3DXVECTOR3(8.0f, 8.0f, 0.0f))			// �Q�[���^�C�}�[�̓_�̃T�C�Y
#define GAMETIME_WIDTH_SHIFT	(50.0f)									// �Q�[���^�C�}�[�̂��炷����

//========================
// �R���X�g���N�^
//========================
CGameTime::CGameTime() : CTime(CObject::TYPE_TIME, CObject::PRIORITY_UI)
{
	// �S�Ă̒l������������
	m_StartTime = 0;		// �J�n����
	m_PauseTime = 0;		// �|�[�Y���̎���
	m_bGoal = false;		// �S�[����
}

//========================
// �f�X�g���N�^
//========================
CGameTime::~CGameTime()
{

}

//========================
// ����������
//========================
HRESULT CGameTime::Init(void)
{
	// �S�Ă̒l������������
	m_StartTime = 0;		// �J�n����
	m_bGoal = false;		// �S�[����
	m_PauseTime = 0;		// �|�[�Y���̎���

	if (FAILED(CTime::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CGameTime::Uninit(void)
{
	// �^�C���̏I������
	CTime::Uninit();

	// �^�C����NULL������
	CGame::DeleteTime();
}

//========================
// �X�V����
//========================
void CGameTime::Update(void)
{
	if (CGame::GetPause() != nullptr)
	{ // �|�[�Y�̏�񂪂���ꍇ

		if (CGame::GetPause()->GetPause() == true &&
			CGame::GetState() == CGame::STATE_PLAY)
		{ // �|�[�Y�����A�v���C���̏ꍇ

			// �|�[�Y���̏���
			PauseProcess();
		}
		else
		{ // ��L�ȊO

			if (CGame::GetState() != CGame::STATE_START &&
				CGame::GetState() != CGame::STATE_COUNTDOWN)
			{ // �X�^�[�g��Ԃ܂��́A�J�E���g�_�E����ԈȊO�̏ꍇ

				// �ʏ�̍X�V����
				Basic();
			}
		}
	}
	else
	{ // ��L�ȊO

		if (CGame::GetState() != CGame::STATE_START &&
			CGame::GetState() != CGame::STATE_COUNTDOWN)
		{ // �X�^�[�g��Ԃ܂��́A�J�E���g�_�E����ԈȊO�̏ꍇ

			// �ʏ�̍X�V����
			Basic();
		}
	}
}

//========================
// �`�揈��
//========================
void CGameTime::Draw(void)
{
	// �`�揈��
	CTime::Draw();
}

//========================
// �^�C���̊J�n����
//========================
void CGameTime::TimeStart(void)
{
	// ���݂̎������擾����
	m_StartTime = timeGetTime();
}

//========================
// �ʏ�̍X�V����
//========================
void CGameTime::Basic(void)
{
	if (CGame::GetState() == CGame::STATE_GOAL)
	{ // �S�[����Ԃ̏ꍇ

		// �S�[���󋵂� true �ɂ���
		m_bGoal = true;
	}

	if (m_bGoal == false)
	{ // �S�[���󋵂� false �̏ꍇ

		// �v�Z����
		Calculate();

		// 1�����Ƃ̃e�N�X�`���̐ݒ菈��
		SetTexture();
	}
}

//========================
// �|�[�Y���̏���
//========================
void CGameTime::PauseProcess(void)
{
	// ���[�J���ϐ��錾
	DWORD NowTime = timeGetTime();		// ���݂̎���
	DWORD TotalTime = GetTotalTime();	// ���v����

	// �|�[�Y���̎��Ԃ�ݒ肷��
	m_PauseTime = (NowTime - m_StartTime) - TotalTime;
}

//========================
// �v�Z����
//========================
void CGameTime::Calculate(void)
{
	// ���[�J���ϐ��錾
	DWORD NowTime;				// ���݂̎���

	// ���ݎ������擾����
	NowTime = timeGetTime();

	// �o�߂����b���𑪂�
	SetTotalTime((NowTime - m_StartTime) - m_PauseTime);

	if (GetTotalTime() >= MAX_TIME)
	{ // �o�ߎ��Ԃ� �ő吔 �𒴂����ꍇ

		// �o�ߎ��Ԃ�␳����
		SetTotalTime(MAX_TIME);
	}

	// ���l�̐ݒ菈��
	SetNumber();
}

//========================
// ��������
//========================
CGameTime* CGameTime::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CGameTime* pTime = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pTime == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pTime = new CGameTime;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pTime != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pTime->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���Ԃ̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pTime->SetData(pos, rot, size, GAMETIME_DOT_SIZE, GAMETIME_WIDTH_SHIFT);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���Ԃ̃|�C���^��Ԃ�
	return pTime;
}