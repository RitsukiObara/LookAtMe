//============================================
//
// �w�i�̃��C������[bg.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define BG_TEXTURE		"data/TEXTURE/SkyBox.jpg"	// �w�i�̃e�N�X�`��

//===========================================
// �R���X�g���N�^
//===========================================
CBg::CBg() : CObject3D(CObject::TYPE_BG, CObject::PRIORITY_BG)
{

}

//===========================================
// �f�X�g���N�^
//===========================================
CBg::~CBg()
{

}

//===========================================
// ����������
//===========================================
HRESULT CBg::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �X�N���[���̐ݒ菈��
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 2000.0f));	// �ʒu�ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �����ݒ�
	SetSize(D3DXVECTOR3(20000.0f, 20000.0f, 0.0f));	// �T�C�Y�ݒ�

	// ���_���̐ݒ菈��
	SetVertex();

	// �S�Ă̒l������������
	BindTexture(CManager::GetTexture()->Regist(BG_TEXTURE));			// �e�N�X�`���̓ǂݍ���

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CBg::Uninit(void)
{
	// �I��
	CObject3D::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CBg::Update(void)
{
	// �X�V
	CObject3D::Update();
}

//===========================================
// �`�揈��
//===========================================
void CBg::Draw(void)
{
	// �`�揈��
	CObject3D::DrawLightOff();
}

//===========================================
// ��������
//===========================================
CBg* CBg::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBg* pBg = nullptr;			// �v���C���[�̃C���X�^���X�𐶐�

	if (pBg == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBg = new CBg;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBg != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBg->Init()))
		{ // �w�i�̏������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�w�i�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pBg;
}