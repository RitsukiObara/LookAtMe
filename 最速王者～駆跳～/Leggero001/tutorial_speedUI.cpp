//============================================
//
// �`���[�g���A���̑��xUI����[tutorial_speedUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "tutorial.h"
#include "tutorial_speedUI.h"
#include "object2D.h"
#include "balloonUI.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define SPEEDUI_MARK_POS		(D3DXVECTOR3(60.0f, 200.0f, 0.0f))													// ���xUI�̃}�[�N�̈ʒu
#define SPEEDUI_GAGE_POS		(D3DXVECTOR3(SPEEDUI_MARK_POS.x + SPEEDUI_MARK_SIZE.x, SPEEDUI_MARK_POS.y, 0.0f))	// ���xUI�̃Q�[�W�̈ʒu
#define SPEEDUI_METER_POS		(SPEEDUI_GAGE_POS)																	// ���xUI�̃��[�^�[�̈ʒu
#define DEST_DIFF				(500.0f)																			// �ړI�Ƃ̍���
#define MOVE_CORRECT			(0.3f)																				// �ړ��ʂ̕␳��

//============================
// �R���X�g���N�^
//============================
CTutorialSpeedUI::CTutorialSpeedUI() : CSpeedUI(CObject::TYPE_SPEEDUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest[nCnt] = 0.0f;		// ���_���W
	}
}

//============================
// �f�X�g���N�^
//============================
CTutorialSpeedUI::~CTutorialSpeedUI()
{

}

//============================
// ����������
//============================
HRESULT CTutorialSpeedUI::Init(void)
{
	if (FAILED(CSpeedUI::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest[nCnt] = 0.0f;		// ���_���W
	}

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CTutorialSpeedUI::Uninit(void)
{
	// �I������
	CSpeedUI::Uninit();
}

//============================
// �X�V����
//============================
void CTutorialSpeedUI::Update(void)
{
	// �X�V����
	CSpeedUI::Update();

	if (CTutorial::GetState() == CTutorial::STATE_BOOST ||
		CTutorial::GetState() == CTutorial::STATE_SPEED)
	{ // �u�[�X�g��Ԃ܂��́A���xUI�\����Ԃ������ꍇ

		// �ړI�̈ʒu��ݒ肷��
		m_posDest[TYPE::TYPE_GAGE] = SPEEDUI_GAGE_POS.x;
		m_posDest[TYPE::TYPE_METER] = SPEEDUI_METER_POS.x;
		m_posDest[TYPE::TYPE_MARK] = SPEEDUI_MARK_POS.x;
	}
	else
	{ // ��L�ȊO

		// �ړI�̈ʒu��ݒ肷��
		m_posDest[TYPE::TYPE_GAGE] = SPEEDUI_GAGE_POS.x - DEST_DIFF;
		m_posDest[TYPE::TYPE_METER] = SPEEDUI_METER_POS.x - DEST_DIFF;
		m_posDest[TYPE::TYPE_MARK] = SPEEDUI_MARK_POS.x - DEST_DIFF;
	}

	// �␳����
	PosCorrect();
}

//============================
// �`�揈��
//============================
void CTutorialSpeedUI::Draw(void)
{
	// �`�揈��
	CSpeedUI::Draw();
}

//============================
// ���̐ݒ菈��
//============================
void CTutorialSpeedUI::SetData(void)
{
	// �ʒu��ݒ肷��
	GetObject2D(TYPE::TYPE_GAGE)->SetPos(D3DXVECTOR3(SPEEDUI_GAGE_POS.x - DEST_DIFF, SPEEDUI_GAGE_POS.y, 0.0f));		// �Q�[�W
	GetObject2D(TYPE::TYPE_METER)->SetPos(D3DXVECTOR3(SPEEDUI_METER_POS.x - DEST_DIFF, SPEEDUI_METER_POS.y, 0.0f));		// ���[�^�[
	GetObject2D(TYPE::TYPE_MARK)->SetPos(D3DXVECTOR3(SPEEDUI_MARK_POS.x - DEST_DIFF, SPEEDUI_MARK_POS.y, 0.0f));		// �}�[�N

	// �ړI�̈ʒu��ݒ肷��
	m_posDest[TYPE::TYPE_GAGE] = SPEEDUI_GAGE_POS.x;
	m_posDest[TYPE::TYPE_METER] = SPEEDUI_METER_POS.x;
	m_posDest[TYPE::TYPE_MARK] = SPEEDUI_MARK_POS.x;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// ���_���W�̐ݒ菈��
		GetObject2D((TYPE)(nCnt))->SetVertex();
	}
}

//============================
// �␳����
//============================
void CTutorialSpeedUI::PosCorrect(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;		// �ʒu�̕ϐ�

	for (int nCnt = 0; nCnt < CSpeedUI::TYPE_MAX; nCnt++)
	{
		// �ʒu��ݒ肷��
		pos = GetObject2D((CSpeedUI::TYPE)(nCnt))->GetPos();

		// �ʒu�̕␳����
		Correct(m_posDest[nCnt], &pos.x, MOVE_CORRECT);

		// �ʒu��ݒ肷��
		GetObject2D((CSpeedUI::TYPE)(nCnt))->SetPos(pos);
	}
}