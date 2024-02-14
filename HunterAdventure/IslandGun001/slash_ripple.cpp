//=======================================
//
// �a���̔g��̃��C������[slash_ripple.cpp]
// Author�F��������
//
//=======================================
#include "manager.h"
#include "slash_ripple.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//==========================================
// �R���X�g���N�^
//==========================================
CSlashRipple::CSlashRipple() : CRipple(TYPE_SLASHRIPPLE, PRIORITY_PLAYER)
{

}

//==========================================
// �f�X�g���N�^
//==========================================
CSlashRipple::~CSlashRipple()
{

}

//==========================================
// �g��̏���������
//==========================================
HRESULT CSlashRipple::Init(void)
{
	if (FAILED(CRipple::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//========================================
// �g��̏I������
//========================================
void CSlashRipple::Uninit(void)
{
	// �I������
	CRipple::Uninit();
}

//========================================
// �g��̍X�V����
//========================================
void CSlashRipple::Update(void)
{
	// �X�V����
	CRipple::Update();
}

//===========================================
// �g��̕`�揈��
//===========================================
void CSlashRipple::Draw(void)
{
	// �`�揈��
	CRipple::Draw();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CSlashRipple::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���̐ݒ菈��
	CRipple::SetData(pos, rot, NONE_SCALE);
}

//===========================================
// ��������
//===========================================
CSlashRipple* CSlashRipple::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CSlashRipple* pRipple = nullptr;	// �g��̃C���X�^���X�𐶐�

	if (pRipple == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pRipple = new CSlashRipple;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRipple != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pRipple->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pRipple->SetData(pos, rot);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �g��̃|�C���^��Ԃ�
	return pRipple;
}