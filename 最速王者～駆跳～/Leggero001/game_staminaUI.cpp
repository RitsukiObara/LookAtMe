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
#include "game_staminaUI.h"
#include "object2D.h"
#include "animobject.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define STAMINAUI_MARK_POS			(D3DXVECTOR3(60.0f, 100.0f, 0.0f))														// �X�^�~�iUI�̃}�[�N�̈ʒu
#define STAMINAUI_GAGE_POS			(D3DXVECTOR3(STAMINAUI_MARK_POS.x + STAMINAUI_MARK_SIZE.x, STAMINAUI_MARK_POS.y, 0.0f))	// �X�^�~�iUI�̈ʒu

//============================
// �R���X�g���N�^
//============================
CGameStaminaUI::CGameStaminaUI() : CStaminaUI(TYPE_STAMINAUI, PRIORITY_UI)
{

}

//============================
// �f�X�g���N�^
//============================
CGameStaminaUI::~CGameStaminaUI()
{

}

//============================
// ����������
//============================
HRESULT CGameStaminaUI::Init(void)
{
	if (FAILED(CStaminaUI::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CGameStaminaUI::Uninit(void)
{
	// �I������
	CStaminaUI::Uninit();
}

//============================
// �X�V����
//============================
void CGameStaminaUI::Update(void)
{
	// �X�V����
	CStaminaUI::Update();
}

//============================
// �`�揈��
//============================
void CGameStaminaUI::Draw(void)
{
	// �`�揈��
	CStaminaUI::Draw();
}

//============================
// ���̐ݒ菈��
//============================
void CGameStaminaUI::SetData(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �ʒu��ݒ肷��
		GetObject2D((TYPE)(nCnt))->SetPos(STAMINAUI_GAGE_POS);

		// ���_���W�̐ݒ菈��
		GetObject2D((TYPE)(nCnt))->SetVertex();
	}

	// �}�[�N
	GetAnim()->SetPos(STAMINAUI_MARK_POS);

	// �ݒ���W�̐ݒ菈��
	GetAnim()->SetVertex();
}