//=======================================
//
//�����L���O�̃��C������[Ranking.cpp]
//Author ��������
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
#include "skybox.h"

#include "ranking_time.h"
#include "ranking_score.h"
#include "ranking_precedence.h"
#include "ranking_scoredisp.h"
#include "ranking_word.h"

//�}�N����`
#define SOUND_NEW_RECORD		(90)			// �X�R�A���Z�ŉ����鎞��
#define SET_TITLE_TIMER			(300)			// �^�C�g����ʂɍs���b��
#define RANKING_CAMERA_POSY		(800.0f)		// �J�����̈ʒu(Y��)
#define RANKING_CAMERA_DISTANCE	(2000.0f)		// �J�����̋���

//==========================================
// �R���X�g���N�^
//==========================================
CRanking::CRanking() : CScene(TYPE_NONE, PRIORITY_BG)
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
	m_nTransCount = 0;			// �����J�ڃJ�E���g

	// �����L���O�̕����̐���
	CRankingWord::Create();

	// �����L���O�̋L�^�𐶐�
	CRankingScore::Create();

	// ���ʕ\���𐶐�
	CRankingPrece::Create();

	if (CManager::GetClear() == true)
	{ // �N���A�󋵂� true �̏ꍇ

		// �N���A�^�C���̐���
		CRankingTime::Create();

		// �X�R�A�\���̐���
		CRankingScoreDisp::Create();
	}

	if (CManager::GetCamera() != nullptr)
	{ // �J������ NULL ����Ȃ������ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 posR;		// �����_
		D3DXVECTOR3 posV;		// ���_

		// ���Z�b�g����
		CManager::GetCamera()->Reset();

		// �ʒu���擾����
		posR = CManager::GetCamera()->GetPosR();
		posV = CManager::GetCamera()->GetPosV();

		// �ʒu�Ƌ�����ݒ肷��
		posR.y = RANKING_CAMERA_POSY;
		posV.y = RANKING_CAMERA_POSY;

		// �J�����̐ݒ菈��
		CManager::GetCamera()->SetPosR(posR);
		CManager::GetCamera()->SetPosV(posV);
		CManager::GetCamera()->SetDistance(RANKING_CAMERA_DISTANCE);
	}

	// ������Ԃ�
	return S_OK;
}

//========================================
// �X�R�A�̏I������
//========================================
void CRanking::Uninit(void)
{
	// �j������
	Release();

	// �N���A�󋵂̐ݒ菈��
	CManager::SetClear(false);
}

//======================================
//�����L���O�̍X�V����
//======================================
void CRanking::Update(void)
{
	// �����J�ڃJ�E���g�����Z����
	m_nTransCount++;

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START,0) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
		m_nTransCount % SET_TITLE_TIMER == 0)
	{ // ENTER�L�[���������܂��́A�J�E���g����萔�ɒB�����ꍇ

		// �^�C�g���ɑJ�ڂ���
		CManager::GetFade()->SetFade(CScene::MODE_TITLE);

		// ���̐�̏������s��Ȃ�
		return;
	}

	if (CManager::GetRenderer() != nullptr)
	{ // �����_���[�� NULL ����Ȃ��ꍇ

		// �X�V����
		CManager::GetRenderer()->Update();
	}
}

//=====================================
//�X�R�A�̕`�揈��
//=====================================
void CRanking::Draw(void)
{

}