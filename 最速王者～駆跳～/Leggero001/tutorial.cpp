//============================================
//
// �`���[�g���A����ʂ̃��C������[tutorial.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "tutorial.h"
#include "file.h"
#include "player.h"
#include "input.h"
#include "renderer.h"
#include "object.h"
#include "fade.h"
#include "Objectmesh.h"
#include "skybox.h"
#include "sound.h"

#include "skip_circle.h"
#include "balloonUI.h"
#include "enter_skip.h"

// �}�N����`
#define SKIP_CIRCLE_POS			(D3DXVECTOR3(200.0f, 570.0f, 0.0f))		// �X�L�b�v�~�̈ʒu
#define SKIP_CIRCLE_SIZE		(40.0f)									// �X�L�b�v�~�̃T�C�Y
#define SKIP_CIRCLE_DISTANCE	(30.0f)									// �X�L�b�v�~�̒��S����̋���
#define SKIP_CIRCLE_DIVI		(32)									// �X�L�b�v�~�̕�����
#define END_COUNT				(120)									// �I���܂ł̃J�E���g��

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
CTutorial::STATE CTutorial::m_state = STATE_NONE;		// ���
CBalloonUI* CTutorial::m_pBalloonUI = nullptr;			// �ӂ�����2D�̏��

//=========================================
// �R���X�g���N�^
//=========================================
CTutorial::CTutorial() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// �S�Ă̏����N���A����
	m_state = STATE_NONE;		// ���
	m_pBalloonUI = nullptr;		// �ӂ�����2D
	m_nEndCount = 0;			// �I���܂ł̃J�E���g
}

//=========================================
// �f�X�g���N�^
//=========================================
CTutorial::~CTutorial()
{

}

//=========================================
//����������
//=========================================
HRESULT CTutorial::Init(void)
{
	// �V�[���̏�����
	CScene::Init();

	// �e�L�X�g�ǂݍ��ݏ���
	CMesh::TxtSet();

	// �X�J�C�{�b�N�X�̐ݒ菈��
	CSkyBox::Create();

	// ��ԏ�����ݒ肷��
	m_state = STATE_NONE;		// ���
	m_pBalloonUI = nullptr;		// �ӂ�����2D
	m_nEndCount = 0;			// �I���܂ł̃J�E���g

	// ������Ԃ�
	return S_OK;
}

//=============================================
//�I������
//=============================================
void CTutorial::Uninit(void)
{
	// �S�Ẵ|�C���^�� NULL �ɂ���
	m_pBalloonUI = nullptr;			// �ӂ�����2D

	// �I������
	CScene::Uninit();
}

//======================================
//�X�V����
//======================================
void CTutorial::Update(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true)
	{ // ENTER�L�[���������ꍇ

		// �X�L�b�v�~�̐�������
		SkipCreate();

		// �X�L�b�v�~�̏o������炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SKIPCIRCLE);
	}

	if (CManager::GetRenderer() != nullptr)
	{ // �����_���[�� NULL ����Ȃ��ꍇ

		// �����_���[�̍X�V
		CManager::GetRenderer()->Update();
	}

	if (m_state == STATE_END)
	{ // �I����Ԃ̏ꍇ

		// �I���J�E���g�����Z����
		m_nEndCount++;
	}

	if (m_state == STATE_SKIP ||
		m_nEndCount >= END_COUNT)
	{ // �X�L�b�v���܂��́A�I�����̏ꍇ

		// �Q�[�����[�h�ɂ���
		CManager::GetFade()->SetFade(MODE_GAME);
	}
}

//======================================
//�`�揈��
//======================================
void CTutorial::Draw(void)
{

}

//======================================
// �X�L�b�v�~�̏o������
//======================================
void CTutorial::SkipCreate(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�̃|�C���^
	CSkipCircle* pSkip = nullptr;	// �X�L�b�v�~�̃|�C���^

	// �擪�̃I�u�W�F�N�g���擾����
	pObj = CObject::GetTop(PRIORITY_UI);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ��

		if (pObj->GetType() == TYPE_SKIP)
		{ // ��ނ��X�L�b�v�̉~�������ꍇ

			// �X�L�b�v�Ƀ_�C�i�~�b�N�L���X�g����
			pSkip = dynamic_cast<CSkipCircle*>(pObj);

			if (pSkip != nullptr)
			{ // �X�L�b�v�̉~�����݂��Ă����ꍇ

				// ���̊֐��𔲂���
				return;
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}

	// �X�L�b�v�~�̐ݒ菈��
	CSkipCircle::Create(SKIP_CIRCLE_POS, SKIP_CIRCLE_SIZE, SKIP_CIRCLE_DISTANCE, SKIP_CIRCLE_DIVI);
}

//======================================
// ���̐ݒ菈��
//======================================
void CTutorial::SetData(const MODE mode)
{
	// ���̐ݒ菈��
	CScene::SetData(mode);

	if (CManager::GetCamera() != nullptr)
	{ // �J�����̏�� NULL ����Ȃ��ꍇ

		// �ʏ�J�����ɐݒ肷��
		CManager::GetCamera()->SetType(CCamera::TYPE_NONE);
	}

	if (CManager::GetFile() != nullptr)
	{ // �t�@�C���̏�� NULL ����Ȃ��ꍇ

		// �`���[�g���A���̐ݒ菈��
		CManager::GetFile()->SetTutorial();
	}

	// �ӂ�����2D�̐�������
	m_pBalloonUI = CBalloonUI::Create();

	// Enter�X�L�b�v�̐���
	CEnterSkip::Create();
}

//======================================
// ��Ԃ̐ݒ菈��
//======================================
void CTutorial::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//======================================
// ��Ԃ̎擾����
//======================================
CTutorial::STATE CTutorial::GetState(void)
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//======================================
// �ӂ�����2D�̎擾����
//======================================
CBalloonUI* CTutorial::GetBalloonUI(void)
{
	// �ӂ�����2D�̏���Ԃ�
	return m_pBalloonUI;
}