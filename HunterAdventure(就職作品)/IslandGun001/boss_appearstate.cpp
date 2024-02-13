//================================================================================================================
//
// �{�X�̏�ԏ��� [boss_appearstate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_appearstate.h"
#include "motion.h"

#include "ripple.h"

#include "boss_turnstate.h"

//----------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const float LENGTH = 8000.0f;		// ����
	const float HEIGHT = 2000.0f;		// �������x
	const float SLOPE = -0.9f;			// �X��
	const float ADD_ROT = 0.05f;		// �����̒ǉ���
	const float SUB_LENGTH = 20.0f;		// �����̌��Z��
	const float SUB_SLOPE = 0.00225f;	// �X���̌��Z��
	const int RIPPLE_FREQ = 6;			// �g��̏o��p�x
	const float RIPPLE_HEIGHT = 600.0f;	// �g��̍��x
	const D3DXVECTOR3 RIPPLE_SCALE = D3DXVECTOR3(100.0f, 100.0f, 100.0f);	// �g��̊g�嗦
}

//==========================
// �R���X�g���N�^
//==========================
CBossAppearState::CBossAppearState()
{
	// �S�Ă̒l���N���A����
	m_fLangeRot = 0.0f;		// ��������
	m_fLength = LENGTH;		// ����
}

//==========================
// �f�X�g���N�^
//==========================
CBossAppearState::~CBossAppearState()
{

}

//==========================
// ��Ԃ̐ݒ菈��
//==========================
void CBossAppearState::Process(CBoss* pBoss)
{
	// �J�E���g�����Z����
	m_nCount++;

	// �����֌W����
	Length();

	if (m_fLength <= 0.0f)
	{ // ������ 0.0f �ȉ��ɂȂ����ꍇ

		// �ʒu�ƌ������擾����
		D3DXVECTOR3 pos;

		// �ʒu��ݒ肷��
		pos.x = 0.0f;
		pos.y = HEIGHT;
		pos.z = 0.0f;

		// �ʒu��K�p����
		pBoss->SetPos(pos);

		// ���������Ԃɂ���
		pBoss->ChangeState(new CBossTurnState());

		// ���̐�̏������s��Ȃ�
		return;
	}

	// �ʒu�����ݒ菈��
	PosRot(pBoss);

	// �g��̐ݒu����
	Ripple(pBoss);
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossAppearState::SetData(CBoss* pBoss)
{
	// �S�Ă̒l��ݒ肷��
	m_fLangeRot = 0.0f;		// ��������
	m_fLength = LENGTH;		// ����

	// �ʒu�ƌ������擾����
	D3DXVECTOR3 pos = pBoss->GetPos();
	D3DXVECTOR3 rot = pBoss->GetRot();

	// �ʒu��ݒ肷��
	pos.x = sinf(m_fLangeRot) * m_fLength;
	pos.y = HEIGHT;
	pos.z = cosf(m_fLangeRot) * m_fLength;

	// ������ݒ肷��
	rot.y = m_fLangeRot + (D3DX_PI * 0.5f);
	rot.z = SLOPE;

	// �ʒu�ƌ�����K�p����
	pBoss->SetPos(pos);
	pBoss->SetRot(rot);
}

//==========================
// �����֌W����
//==========================
void CBossAppearState::Length(void)
{
	// ���������Z����
	m_fLangeRot += ADD_ROT;

	// �����̐��K��
	useful::RotNormalize(&m_fLangeRot);

	// ���������Z����
	m_fLength -= SUB_LENGTH;
}

//==========================
// �ʒu�����ݒ菈��
//==========================
void CBossAppearState::PosRot(CBoss* pBoss)
{
	// �ʒu�ƌ������擾����
	D3DXVECTOR3 pos = pBoss->GetPos();
	D3DXVECTOR3 rot = pBoss->GetRot();

	// �ʒu��ݒ肷��
	pos.x = sinf(m_fLangeRot) * m_fLength;
	pos.y = HEIGHT;
	pos.z = cosf(m_fLangeRot) * m_fLength;

	// ������ݒ肷��
	rot.y = m_fLangeRot + (D3DX_PI * 0.5f);
	rot.z += SUB_SLOPE;

	// �����̐��K��
	useful::RotNormalize(&rot.y);

	// �ʒu�ƌ�����K�p����
	pBoss->SetPos(pos);
	pBoss->SetRot(rot);
}

//==========================
// �g��̐ݒu����
//==========================
void CBossAppearState::Ripple(CBoss* pBoss)
{
	if (m_nCount % RIPPLE_FREQ == 0)
	{ // ���J�E���g���Ƃ�

		// �ʒu�ƌ������擾����
		D3DXVECTOR3 pos = pBoss->GetPos();
		D3DXVECTOR3 rot = pBoss->GetRot();

		// �g��̐�������
		CRipple::Create
		(
			D3DXVECTOR3(pos.x, pos.y + RIPPLE_HEIGHT, pos.z),
			D3DXVECTOR3(D3DX_PI * 0.5f, rot.y, D3DX_PI),
			RIPPLE_SCALE
		);
	}
}