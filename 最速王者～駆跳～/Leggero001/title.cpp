//============================================
//
// �^�C�g����ʂ̃��C������[title.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "object.h"
#include "title.h"
#include "fade.h"
#include "Objectmesh.h"
#include "skybox.h"

#include "title_name.h"
#include "file.h"
#include "silhouette.h"

// �}�N����`
#define SET_RANKING_TIMER		(600)		// �����L���O��ʂɑJ�ڂ���J�E���g��

//=========================================
// �R���X�g���N�^
//=========================================
CTitle::CTitle() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	m_nTransCount = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CTitle::~CTitle()
{

}

//=========================================
//����������
//=========================================
HRESULT CTitle::Init(void)
{
	//�@�V�[���̏�����
	CScene::Init();

	// �e�L�X�g�ǂݍ��ݏ���
	CMesh::TxtSet();

	// �X�J�C�{�b�N�X�̐ݒ菈��
	CSkyBox::Create();

	// �S�Ă̒l������������
	m_nTransCount = 0;

	// �X�e�[�W�̐ݒ菈��
	CManager::GetFile()->SetBlock();		// �u���b�N�̐ݒu����
	CManager::GetFile()->SetObstacle();		// ��Q���̐ݒu����

	// ���[�J���ϐ��錾
	CSilhouette::TYPE aType[2] = { CSilhouette::TYPE_MAX, CSilhouette::TYPE_MAX };		// ���

	// ��ނ������_���őI�o����
	aType[0] = (CSilhouette::TYPE)(rand() % CSilhouette::TYPE_MAX);

	// �e�G�̐ݒ菈��
	CSilhouette::Create(aType[0], false);

	do
	{ // ��ނ���v���Ă���ꍇ��

		// ��ނ������_���őI�o����
		aType[1] = (CSilhouette::TYPE)(rand() % CSilhouette::TYPE_MAX);

	} while (aType[0] == aType[1]);

	// �e�G�̐ݒ菈��
	CSilhouette::Create(aType[1], true);

	// �^�C�g���𐶐�����
	CTitleName::Create();

	// ������Ԃ�
	return S_OK;
}

//=============================================
//�I������
//=============================================
void CTitle::Uninit(void)
{
	// �I������
	CScene::Uninit();
}

//======================================
//�X�V����
//======================================
void CTitle::Update(void)
{
	// �J�ڃJ�E���g�����Z����
	m_nTransCount++;

	if (CManager::GetRenderer() != nullptr)
	{ // �����_���[�� NULL ����Ȃ��ꍇ

		// �����_���[�̍X�V
		CManager::GetRenderer()->Update();
	}

	if (m_nTransCount % SET_RANKING_TIMER == 0)
	{ // �J�ڃJ�E���g����萔�ɒB�����ꍇ

		// �����L���O�ɑJ�ڂ���
		CManager::GetFade()->SetFade(CScene::MODE_RANKING);

		// ���̐�̏������s��Ȃ�
		return;
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	{ // ENTER�L�[���������ꍇ

		// �`���[�g���A���ɑJ�ڂ���
		CManager::GetFade()->SetFade(CScene::MODE_TUTORIAL);

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//======================================
//�`�揈��
//======================================
void CTitle::Draw(void)
{

}