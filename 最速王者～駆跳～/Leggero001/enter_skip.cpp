//============================================
//
// ENTER�X�L�b�v����[enter_skip.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "enter_skip.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define ENTER_SKIP_TEXTURE	"data/TEXTURE/EnterSkip.png"			// �e�N�X�`��
#define ENTER_SKIP_POS		(D3DXVECTOR3(200.0f, 690.0f, 0.0f))		// �ʒu
#define ENTER_SKIP_SIZE		(D3DXVECTOR3(200.0f, 20.0f, 0.0f))		// �T�C�Y

//============================
// �R���X�g���N�^
//============================
CEnterSkip::CEnterSkip() : CObject2D(CObject::TYPE_ENTERSKIP, PRIORITY_UI)
{

}

//============================
// �f�X�g���N�^
//============================
CEnterSkip::~CEnterSkip()
{

}

//============================
// ����������
//============================
HRESULT CEnterSkip::Init(void)
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
void CEnterSkip::Uninit(void)
{
	// �I��
	CObject2D::Uninit();
}

//============================
// �X�V����
//============================
void CEnterSkip::Update(void)
{

}

//============================
// �`�揈��
//============================
void CEnterSkip::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//============================
// ���̐ݒ菈��
//============================
void CEnterSkip::SetData(void)
{
	// ���̏�����
	SetPos(ENTER_SKIP_POS);						// �ʒu
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ����
	SetSize(ENTER_SKIP_SIZE);					// �T�C�Y
	SetAngle();									// ����
	SetLength();								// ����

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(ENTER_SKIP_TEXTURE));
}

//============================
//��������
//============================
CEnterSkip* CEnterSkip::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CEnterSkip* pEnterSkip = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pEnterSkip == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pEnterSkip = new CEnterSkip;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEnterSkip != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEnterSkip->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�X�L�b�v�\���̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEnterSkip->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �^�C�g���̑I�����̃|�C���^��Ԃ�
	return pEnterSkip;
}