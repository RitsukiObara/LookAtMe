//============================================
//
// �}�l�[�W���[�̃��C������[manager.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "renderer.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "result_time.h"
#include "Objectmesh.h"
#include "skybox.h"

#include "file.h"
#include "camera.h"

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
CResult::STATE CResult::m_state = STATE_TIMEDISP;		// ���

//=========================================
// �R���X�g���N�^
//=========================================
CResult::CResult() : CScene(TYPE_NONE, PRIORITY_BG)
{
	m_state = STATE_TIMEDISP;
}

//=========================================
// �f�X�g���N�^
//=========================================
CResult::~CResult()
{

}

//=========================================
//����������
//=========================================
HRESULT CResult::Init(void)
{
	// �V�[���̏�����
	CScene::Init();

	// �X�e�[�W�̐ݒ菈��
	CManager::GetFile()->SetBlock();		// �u���b�N�̐ݒu����
	CManager::GetFile()->SetObstacle();		// ��Q���̐ݒu����
	CManager::GetFile()->SetItem();			// �A�C�e���̐ݒu����
	CManager::GetFile()->SetCamera();		// �J�����̐ݒu����

	// �e�L�X�g�ǂݍ��ݏ���
	CMesh::TxtSet();

	// �X�J�C�{�b�N�X�̐ݒ菈��
	CSkyBox::Create();

	// �S�Ă̒l������������
	m_state = STATE_TIMEDISP;		// ���

	// ���U���g�^�C�}�[�̐�������
	CResultTime::Create();

	// ������Ԃ�
	return S_OK;
}

//=============================================
//�I������
//=============================================
void CResult::Uninit(void)
{
	// �j������
	Release();
}

//======================================
//�X�V����
//======================================
void CResult::Update(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A,0) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true)
	{ // ENTER�L�[���������ꍇ

		// �����L���O�ɑJ�ڂ���
		CManager::GetFade()->SetFade(CScene::MODE_RANKING);

		// ���̐�̏������s��Ȃ�
		return;
	}

	if (CManager::GetRenderer() != nullptr)
	{ // �����_���[�� NULL ����Ȃ��ꍇ

		// �X�V����
		CManager::GetRenderer()->Update();
	}
}

//======================================
//�`�揈��
//======================================
void CResult::Draw(void)
{

}

//======================================
// ��Ԃ̐ݒ菈��
//======================================
void CResult::SetState(const CResult::STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//======================================
// ��Ԃ̎擾����
//======================================
CResult::STATE CResult::GetState(void)
{
	// ��Ԃ�Ԃ�
	return m_state;
}