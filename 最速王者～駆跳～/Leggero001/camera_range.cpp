//============================================
//
// �J�����͈̔͂̃��C������[camera_range.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "renderer.h"
#include "camera_range.h"

//===========================================
// �R���X�g���N�^
//===========================================
CCameraRange::CCameraRange() : CObject3D(TYPE_CAMERARANGE, CObject::PRIORITY_BG)
{
	// �R���X�g���N�^�̔�
	Box();
}

//===========================================
// �I�[�o�[���[�h�R���X�g���N�^
//===========================================
CCameraRange::CCameraRange(CObject::TYPE type, PRIORITY priority) : CObject3D(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//===========================================
// �f�X�g���N�^
//===========================================
CCameraRange::~CCameraRange()
{

}

//===========================================
// �R���X�g���N�^�̔�
//===========================================
void CCameraRange::Box(void)
{
	// �S�Ă̒l���N���A����
	m_type = CCamera::TYPE_NONE;					// �J�����̎��
}

//===========================================
// ����������
//===========================================
HRESULT CCameraRange::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CCameraRange::Uninit(void)
{
	// �I��
	CObject3D::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CCameraRange::Update(void)
{
	// ���[�J���ϐ���錾
	D3DXVECTOR3 pos = GetPos();			// �e�̈ʒu

	// �O��̈ʒu�̐ݒ菈��
	SetPosOld(pos);
}

//===========================================
// �`�揈��
//===========================================
void CCameraRange::Draw(void)
{
// �f�o�b�O�p
#ifdef _DEBUG

	// �`�揈��
	CObject3D::Draw();

#endif

}

//===========================================
// ���̐ݒ菈��
//===========================================
void CCameraRange::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type)
{
	// ���̐ݒ菈��
	SetPos(pos);								// �ʒu
	SetPosOld(pos);								// �O��̈ʒu
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ����
	SetSize(size);								// �T�C�Y

	// �S�Ă̒l������������
	m_type = type;

	// ���_���W�̐ݒ菈��
	SetVertex();

	SetVtxColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
}

//===========================================
// �J�����̎�ނ̐ݒ菈��
//===========================================
void CCameraRange::SetType(const CCamera::TYPE type)
{
	// ��ނ�ݒ肷��
	m_type = type;
}

//===========================================
// �J�����̎�ނ̎擾����
//===========================================
CCamera::TYPE CCameraRange::GetType(void) const
{
	// �J�����̎�ނ�Ԃ�
	return m_type;
}

//===========================================
// ��������
//===========================================
CCameraRange* CCameraRange::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CCameraRange* pCameraRange = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pCameraRange == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pCameraRange = new CCameraRange;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pCameraRange != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pCameraRange->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�J�����͈̔͂̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pCameraRange->SetData(pos, size, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �J�����͈̔͂̃|�C���^��Ԃ�
	return pCameraRange;
}