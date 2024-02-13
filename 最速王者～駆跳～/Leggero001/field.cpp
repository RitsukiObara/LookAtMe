//============================================
//
// �t�B�[���h�̃��C������[object3D.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "field.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

#include "input.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define FIELD_TEXTURE			"data/TEXTURE/FatherFace.png"		// �t�B�[���h�̃e�N�X�`��

//=========================================
// �R���X�g���N�^
//=========================================
CField::CField() : CObject3D(CObject::TYPE_FIELD, CObject::PRIORITY_BG)
{

}

//=========================================
// �f�X�g���N�^
//=========================================
CField::~CField()
{

}

//===========================================
// �t�B�[���h�̏���������
//===========================================
HRESULT CField::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �t�B�[���h�̏I������
//===========================================
void CField::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//===========================================
// �t�B�[���h�̍X�V����
//===========================================
void CField::Update(void)
{

}

//===========================================
// �t�B�[���h�̕`�揈��
//===========================================
void CField::Draw(void)
{
	// �`�揈��
	CObject3D::Draw();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CField::SetData(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// ���̐ݒ�
	SetPos(pos);								// �ʒu
	SetPosOld(pos);								// �O��̈ʒu
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ����
	SetSize(size);								// �T�C�Y

	// ���_���̐ݒ菈��
	SetVertexElev(300.0f);

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::GetTexture()->Regist(FIELD_TEXTURE));
}

//===========================================
// ��������
//===========================================
CField* CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CField* pField = nullptr;	// �t�B�[���h�̃C���X�^���X�𐶐�

	if (pField == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pField = new CField;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pField != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pField->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�t�B�[���h�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pField->SetData(pos, size);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �t�B�[���h�̃|�C���^��Ԃ�
	return pField;
}