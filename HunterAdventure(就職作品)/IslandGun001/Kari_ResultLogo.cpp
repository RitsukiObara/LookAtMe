//============================================
//
// ���U���g���S����[Kari_ResutlLogo.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "Kari_ResultLogo.h"
#include "texture.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const D3DXVECTOR3 LOGO_SIZE = D3DXVECTOR3(400.0f, 100.0f, 0.0f);
	const char* TEXTURE = "data/TEXTURE/KariResult.png";
}

//============================
// �R���X�g���N�^
//============================
CResultLogo::CResultLogo() : CObject2D(CObject::TYPE_TITLELOGO, PRIORITY_UI)
{

}

//============================
// �f�X�g���N�^
//============================
CResultLogo::~CResultLogo()
{

}

//============================
// ����������
//============================
HRESULT CResultLogo::Init(void)
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
void CResultLogo::Uninit(void)
{
	// �I������
	CObject2D::Uninit();
}

//============================
// �X�V����
//============================
void CResultLogo::Update(void)
{

}

//============================
// �`�揈��
//============================
void CResultLogo::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//============================
// ���̐ݒ菈��
//============================
void CResultLogo::SetData(void)
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
CResultLogo* CResultLogo::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CResultLogo* pLogoMark = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pLogoMark == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pLogoMark = new CResultLogo;
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