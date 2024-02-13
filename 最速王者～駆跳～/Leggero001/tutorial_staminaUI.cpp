//============================================
//
// �Q�[���̃X�^�~�iUI����[game_staminaUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "tutorial.h"
#include "tutorial_staminaUI.h"
#include "object2D.h"
#include "animobject.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define STAMINAUI_MARK_POS			(D3DXVECTOR3(60.0f, 100.0f, 0.0f))														// �X�^�~�iUI�̃}�[�N�̈ʒu
#define STAMINAUI_GAGE_POS			(D3DXVECTOR3(STAMINAUI_MARK_POS.x + STAMINAUI_MARK_SIZE.x, STAMINAUI_MARK_POS.y, 0.0f))	// �X�^�~�iUI�̈ʒu
#define DEST_DIFF					(500.0f)																				// �ړI�Ƃ̍���
#define MOVE_CORRECT				(0.3f)																					// �ړ��ʂ̕␳��

//============================
// �R���X�g���N�^
//============================
CTutorialStaminaUI::CTutorialStaminaUI() : CStaminaUI(TYPE_STAMINAUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest2D[nCnt] = 0.0f;		// �ړI�̈ʒu
	}

	m_posDestAnim = 0.0f;				// �ړI�̈ʒu
}

//============================
// �f�X�g���N�^
//============================
CTutorialStaminaUI::~CTutorialStaminaUI()
{

}

//============================
// ����������
//============================
HRESULT CTutorialStaminaUI::Init(void)
{
	if (FAILED(CStaminaUI::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest2D[nCnt] = 0.0f;		// �ړI�̈ʒu
	}

	m_posDestAnim = 0.0f;				// �ړI�̈ʒu

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CTutorialStaminaUI::Uninit(void)
{
	// �I������
	CStaminaUI::Uninit();
}

//============================
// �X�V����
//============================
void CTutorialStaminaUI::Update(void)
{
	// �X�V����
	CStaminaUI::Update();

	if (CTutorial::GetState() == CTutorial::STATE_STAMINA)
	{ // �X�^�~�i��Ԃ������ꍇ

		// �ړI�̈ʒu��ݒ肷��
		m_posDest2D[TYPE::TYPE_GAGE] = STAMINAUI_GAGE_POS.x;
		m_posDest2D[TYPE::TYPE_METER] = STAMINAUI_GAGE_POS.x;
		m_posDestAnim = STAMINAUI_MARK_POS.x;
	}
	else
	{ // ��L�ȊO

		// �ړI�̈ʒu��ݒ肷��
		m_posDest2D[TYPE::TYPE_GAGE] = STAMINAUI_GAGE_POS.x - DEST_DIFF;
		m_posDest2D[TYPE::TYPE_METER] = STAMINAUI_GAGE_POS.x - DEST_DIFF;
		m_posDestAnim = STAMINAUI_MARK_POS.x - DEST_DIFF;
	}

	// �ʒu�̕␳����
	PosCorrect();
}

//============================
// �`�揈��
//============================
void CTutorialStaminaUI::Draw(void)
{
	// �`�揈��
	CStaminaUI::Draw();
}

//============================
// ���̐ݒ菈��
//============================
void CTutorialStaminaUI::SetData(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �ʒu��ݒ肷��
		GetObject2D((TYPE)(nCnt))->SetPos(D3DXVECTOR3(STAMINAUI_GAGE_POS.x - DEST_DIFF, STAMINAUI_GAGE_POS.y, STAMINAUI_GAGE_POS.z));

		// ���_���W�̐ݒ菈��
		GetObject2D((TYPE)(nCnt))->SetVertex();
	}

	// �}�[�N
	GetAnim()->SetPos(D3DXVECTOR3(STAMINAUI_MARK_POS.x - DEST_DIFF, STAMINAUI_MARK_POS.y, STAMINAUI_MARK_POS.z));

	// �ݒ���W�̐ݒ菈��
	GetAnim()->SetVertex();

	// �S�Ă̒l��ݒ肷��
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest2D[nCnt] = STAMINAUI_GAGE_POS.x;		// �ړI�̈ʒu
	}

	m_posDestAnim = STAMINAUI_MARK_POS.x;	// �ړI�̈ʒu
}

//============================
// �␳����
//============================
void CTutorialStaminaUI::PosCorrect(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;		// �ʒu�̕ϐ�

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �ʒu��ݒ肷��
		pos = GetObject2D((TYPE)(nCnt))->GetPos();

		// �ʒu�̕␳����
		Correct(m_posDest2D[nCnt], &pos.x, MOVE_CORRECT);

		// �ʒu���X�V����
		GetObject2D((TYPE)(nCnt))->SetPos(pos);
	}

	// �ʒu��ݒ肷��
	pos = GetAnim()->GetPos();

	// �ʒu�̕␳����
	Correct(m_posDestAnim, &pos.x, MOVE_CORRECT);

	// �ʒu���X�V����
	GetAnim()->SetPos(pos);
}