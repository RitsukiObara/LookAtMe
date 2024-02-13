//============================================
//
// �^�C�g�����S����[title_logo.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "title_logo.h"
#include "texture.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const D3DXVECTOR3 LOGO_SIZE = D3DXVECTOR3(600.0f, 200.0f, 0.0f);
	const char* TEXTURE = "data/TEXTURE/TitleLogo.png";
}

//============================
// �R���X�g���N�^
//============================
CTitleLogo::CTitleLogo() : CObject2D(CObject::TYPE_TITLELOGO, PRIORITY_UI)
{

}

//============================
// �f�X�g���N�^
//============================
CTitleLogo::~CTitleLogo()
{

}

//============================
// ����������
//============================
HRESULT CTitleLogo::Init(void)
{
	if (FAILED(CObject2D::Init()))
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
void CTitleLogo::Uninit(void)
{
	// �I������
	CObject2D::Uninit();
}

//============================
// �X�V����
//============================
void CTitleLogo::Update(void)
{

}

//============================
// �`�揈��
//============================
void CTitleLogo::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//============================
// ���̐ݒ菈��
//============================
void CTitleLogo::SetData(void)
{
	// ���̐ݒ菈��
	SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));	// �ʒu
	SetPosOld(GetPos());			// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetSize(LOGO_SIZE);				// �T�C�Y
	SetAngle();						// ����
	SetLength();					// ����

	// ���_���W�̐ݒ菈��
	SetVertex();

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
}

//============================
//��������
//============================
CTitleLogo* CTitleLogo::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CTitleLogo* pLogoMark = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pLogoMark == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pLogoMark = new CTitleLogo;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pLogoMark != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pLogoMark->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pLogoMark->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���S�}�[�N�̃|�C���^��Ԃ�
	return pLogoMark;
}