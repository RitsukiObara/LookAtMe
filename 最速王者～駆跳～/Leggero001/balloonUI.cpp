//============================================
//
// �����o��UI����[balloonUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "balloonUI.h"
#include "file.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define BALLOON_POS				(D3DXVECTOR3(1100.0f, 610.0f, 0.0f))	// �ʒu
#define SIZE_MAGNI				(0.7f)									// �T�C�Y�̔{��

//============================
// �R���X�g���N�^
//============================
CBalloonUI::CBalloonUI() : CObject2D(CObject::TYPE_BALLOONUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_type = CBalloon::TYPE_NORMAL;		// ���
}

//============================
// �f�X�g���N�^
//============================
CBalloonUI::~CBalloonUI()
{

}

//============================
// ����������
//============================
HRESULT CBalloonUI::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_type = CBalloon::TYPE_NORMAL;		// ���

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CBalloonUI::Uninit(void)
{
	// �I��
	CObject2D::Uninit();
}

//============================
// �X�V����
//============================
void CBalloonUI::Update(void)
{

}

//============================
// �`�揈��
//============================
void CBalloonUI::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//============================
// ���̐ݒ菈��
//============================
void CBalloonUI::SetData(void)
{
	// ���[�J���ϐ��錾
	CFile::SBalloonData data;		// �f�[�^

	// �S�Ă̒l��ݒ肷��
	m_type = CBalloon::TYPE_NORMAL;		// ���

	if (CManager::GetFile() != nullptr)
	{ // �t�@�C���̃|�C���^���������ꍇ

		// �f�[�^��ݒ肷��
		data = CManager::GetFile()->GetBalloon((CBalloon::TYPE)(m_type));
	}

	// ���̏�����
	SetPos(BALLOON_POS);						// �ʒu
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ����
	SetSize(data.size * SIZE_MAGNI);			// �T�C�Y
	SetAngle();									// ����
	SetLength();								// ����

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(data.aTextureName));
}

//============================
// ��ނ̐ݒ菈��
//============================
void CBalloonUI::SetType(const CBalloon::TYPE nType)
{
	if (m_type != nType)
	{ // ��ނ�������ꍇ

		// ��ނ�ݒ肷��
		m_type = nType;

		if (CManager::GetFile() != nullptr)
		{ // �t�@�C���̃|�C���^���������ꍇ

			// ���[�J���ϐ��錾
			CFile::SBalloonData data = CManager::GetFile()->GetBalloon((CBalloon::TYPE)(nType));

			// �T�C�Y��ݒ肷��
			SetSize(data.size * SIZE_MAGNI);

			// �e�N�X�`���̊��蓖�ď���
			BindTexture(CManager::GetTexture()->Regist(data.aTextureName));
		}
	}
}

//============================
// ��ނ̎擾����
//============================
CBalloon::TYPE CBalloonUI::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================
//��������
//============================
CBalloonUI* CBalloonUI::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBalloonUI* pBalloon = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pBalloon == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBalloon = new CBalloonUI;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBalloon != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBalloon->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�����o��UI�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBalloon->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �^�C�g���̑I�����̃|�C���^��Ԃ�
	return pBalloon;
}