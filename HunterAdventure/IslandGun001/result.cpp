//============================================
//
// ���U���g�̃��C������[result.cpp]
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
#include "Objectmesh.h"
#include "game.h"

#include "file.h"
#include "camera.h"
#include "skybox.h"
#include "result_score.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------

//=========================================
// �R���X�g���N�^
//=========================================
CResult::CResult()
{

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

	// �X�J�C�{�b�N�X�̐�������
	CSkyBox::Create();

	// �e�L�X�g�ǂݍ��ݏ���
	CMesh::TxtSet();

	{
		// ���U���g�X�R�A�𐶐�
		CResultScore* pScore = CResultScore::Create();

		if (pScore != nullptr)
		{ // �X�R�A�� NULL ����Ȃ��ꍇ

			// �X�R�A��ݒ肷��
			pScore->SetScore(CGame::GetScore());
		}
	}

	// ������Ԃ�
	return S_OK;
}

//=============================================
//�I������
//=============================================
void CResult::Uninit(void)
{
	// �I������
	CScene::Uninit();
}

//======================================
//�X�V����
//======================================
void CResult::Update(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A,0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true)
	{ // ENTER�L�[���������ꍇ

		// �����L���O�ɑJ�ڂ���
		CManager::Get()->GetFade()->SetFade(CScene::MODE_TITLE);

		// ���̐�̏������s��Ȃ�
		return;
	}

	if (CManager::Get()->GetRenderer() != nullptr)
	{ // �����_���[�� NULL ����Ȃ��ꍇ

		// �X�V����
		CManager::Get()->GetRenderer()->Update();
	}
}

//======================================
//�`�揈��
//======================================
void CResult::Draw(void)
{

}