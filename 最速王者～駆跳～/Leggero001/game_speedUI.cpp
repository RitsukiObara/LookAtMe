//============================================
//
// �`���[�g���A���̑��xUI����[tutorial_speedUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "game_speedUI.h"
#include "object2D.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define SPEEDUI_MARK_POS			(D3DXVECTOR3(60.0f, 200.0f, 0.0f))													// ���xUI�̃}�[�N�̈ʒu
#define SPEEDUI_GAGE_POS			(D3DXVECTOR3(SPEEDUI_MARK_POS.x + SPEEDUI_MARK_SIZE.x, SPEEDUI_MARK_POS.y, 0.0f))	// ���xUI�̃Q�[�W�̈ʒu
#define SPEEDUI_METER_POS			(SPEEDUI_GAGE_POS)																	// ���xUI�̃��[�^�[�̈ʒu

//============================
// �R���X�g���N�^
//============================
CGameSpeedUI::CGameSpeedUI() : CSpeedUI(CObject::TYPE_SPEEDUI, PRIORITY_UI)
{

}

//============================
// �f�X�g���N�^
//============================
CGameSpeedUI::~CGameSpeedUI()
{

}

//============================
// ����������
//============================
HRESULT CGameSpeedUI::Init(void)
{
	if (FAILED(CSpeedUI::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CGameSpeedUI::Uninit(void)
{
	// �I������
	CSpeedUI::Uninit();
}

//============================
// �X�V����
//============================
void CGameSpeedUI::Update(void)
{
	// �X�V����
	CSpeedUI::Update();
}

//============================
// �`�揈��
//============================
void CGameSpeedUI::Draw(void)
{
	// �`�揈��
	CSpeedUI::Draw();
}

//============================
// ���̐ݒ菈��
//============================
void CGameSpeedUI::SetData(void)
{
	// �ʒu��ݒ肷��
	GetObject2D(TYPE::TYPE_GAGE)->SetPos(SPEEDUI_GAGE_POS);
	GetObject2D(TYPE::TYPE_METER)->SetPos(SPEEDUI_METER_POS);
	GetObject2D(TYPE::TYPE_MARK)->SetPos(SPEEDUI_MARK_POS);
}