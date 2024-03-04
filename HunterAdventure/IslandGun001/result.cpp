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
#include "objectElevation.h"
#include "skybox.h"
#include "ocean.h"
#include "result_score.h"

//--------------------------------------------
// �萔��`
//--------------------------------------------
namespace
{
	const char* ELEVATION_TXT = "data/TXT/Elevation.txt";		// �N���n�ʂ̃e�L�X�g
}

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

	// �J�����̃��Z�b�g����
	CManager::Get()->GetCamera()->Reset();

	// �e�L�X�g�ǂݍ��ݏ���
	CElevation::TxtSet(ELEVATION_TXT);

	// �}�b�v�̐���
	CManager::Get()->GetFile()->SetCoin();
	CManager::Get()->GetFile()->SetGoldBone();
	CManager::Get()->GetFile()->SetTree();
	CManager::Get()->GetFile()->SetRock();
	CManager::Get()->GetFile()->SetBlock();
	CManager::Get()->GetFile()->SetBangFlower();
	CManager::Get()->GetFile()->SetWall();

	// �X�J�C�{�b�N�X�̐�������
	CSkyBox::Create();

	// �e�L�X�g�ǂݍ��ݏ���
	CMesh::TxtSet();

	// �C�̐���
	COcean::Create();

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
		CManager::Get()->GetFade()->SetFade(CScene::MODE_RANKING);

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