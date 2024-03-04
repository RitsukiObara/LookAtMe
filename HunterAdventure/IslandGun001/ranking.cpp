//=======================================
//
// �����L���O�̃��C������[Ranking.cpp]
// Author ��������
//
//=======================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "ranking.h"
#include "file.h"
#include "input.h"
#include "Fade.h"

#include "Objectmesh.h"
#include "game.h"
#include "objectElevation.h"
#include "skybox.h"
#include "ocean.h"
#include "ranking_rank.h"
#include "ranking_table.h"

//---------------------------------------
// �萔��`
//---------------------------------------
namespace
{
	const char* ELEVATION_TXT = "data/TXT/Elevation.txt";		// �N���n�ʂ̃e�L�X�g
	const int SET_TITLE_TIMER = 300;		// �^�C�g����ʂɍs���b��
}

//==========================================
// �R���X�g���N�^
//==========================================
CRanking::CRanking()
{
	// �S�Ă̒l���N���A����
	m_nTransCount = 0;			// �����J�ڃJ�E���g
}

//==========================================
// �f�X�g���N�^
//==========================================
CRanking::~CRanking()
{

}

//==========================================
// �����L���O�̏���������
//==========================================
HRESULT CRanking::Init(void)
{
	// �V�[���̏�����
	CScene::Init();

	// �J�����̃��Z�b�g����
	CManager::Get()->GetCamera()->Reset();

	// �e�L�X�g�ǂݍ��ݏ���
	CElevation::TxtSet(ELEVATION_TXT);

	// �}�b�v�̐���
	CManager::Get()->GetFile()->SetTree();
	CManager::Get()->GetFile()->SetRock();
	CManager::Get()->GetFile()->SetBlock();
	CManager::Get()->GetFile()->SetBangFlower();
	CManager::Get()->GetFile()->SetWall();

	// �e�L�X�g�ǂݍ��ݏ���
	CMesh::TxtSet();

	// �X�J�C�{�b�N�X�̐�������
	CSkyBox::Create();

	// �C�̐���
	COcean::Create();

	// ����UI�̐���
	CRankUI::Create();

	// �����L���O�̐���
	CRankingTable::Create();

	// �S�Ă̒l������������
	m_nTransCount = 0;			// �����J�ڃJ�E���g

	// ������Ԃ�
	return S_OK;
}

//========================================
// �X�R�A�̏I������
//========================================
void CRanking::Uninit(void)
{
	// �I������
	CScene::Uninit();
}

//======================================
//�����L���O�̍X�V����
//======================================
void CRanking::Update(void)
{
	// �����J�ڃJ�E���g�����Z����
	m_nTransCount++;

	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START,0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
		m_nTransCount % SET_TITLE_TIMER == 0)
	{ // ENTER�L�[���������܂��́A�J�E���g����萔�ɒB�����ꍇ

		// �^�C�g���ɑJ�ڂ���
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

//=====================================
//�X�R�A�̕`�揈��
//=====================================
void CRanking::Draw(void)
{

}