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
#include "title.h"
#include "fade.h"
#include "file.h"
#include "input.h"
#include "renderer.h"

#include "skybox.h"
#include "title_logo.h"
#include "objectElevation.h"
#include "Objectmesh.h"

// �}�N����`
#define SET_RANKING_TIMER		(600)		// �����L���O��ʂɑJ�ڂ���J�E���g��

//=========================================
// �R���X�g���N�^
//=========================================
CTitle::CTitle()
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
	CElevation::TxtSet();

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
	CManager::Get()->GetFile()->SetBlock();
	CManager::Get()->GetFile()->SetBangFlower();
	CManager::Get()->GetFile()->SetWall();

	// �e�L�X�g�ǂݍ��ݏ���
	CMesh::TxtSet();

	// �X�J�C�{�b�N�X�̐�������
	CSkyBox::Create();

	// �^�C�g�����S�̐���
	CTitleLogo::Create();

	// �S�Ă̒l������������
	m_nTransCount = 0;

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

	// �����_���[�̍X�V
	CManager::Get()->GetRenderer()->Update();

	//if (m_nTransCount % SET_RANKING_TIMER == 0)
	//{ // �J�ڃJ�E���g����萔�ɒB�����ꍇ

	//	// �����L���O�ɑJ�ڂ���
	//	CManager::Get()->GetFade()->SetFade(CScene::MODE_RANKING);

	//	// ���̐�̏������s��Ȃ�
	//	return;
	//}

	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	{ // ENTER�L�[���������ꍇ

		// �Q�[���ɑJ�ڂ���
		CManager::Get()->GetFade()->SetFade(CScene::MODE_GAME);

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