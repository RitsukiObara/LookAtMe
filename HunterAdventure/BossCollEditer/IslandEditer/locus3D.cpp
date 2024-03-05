//=======================================
//
// 3D�c���̃��C������[locus3D.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "locus3D.h"
#include "renderer.h"
#include "texture.h"

//---------------------------------------
// �}�N����`
//---------------------------------------

//=========================
// �R���X�g���N�^
//=========================
CLocus3D::CLocus3D() : CBillboard(CBillboard::TYPE_LOCUS, CObject::PRIORITY_PLAYER)
{
	// �S�Ă̒l���N���A����
	m_nLife = 0;			// ����
	m_fSub = 0.0f;			// �����x�̌�����
	m_fAlpha = 0.0f;		// �����x
	m_bLightOff = false;	// ���C�e�B���O��
}

//=========================
// �f�X�g���N�^
//=========================
CLocus3D::~CLocus3D()
{

}

//=========================
// ����������
//=========================
HRESULT CLocus3D::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_nLife = 0;			// ����
	m_fSub = 0.0f;			// �����x�̌�����
	m_fAlpha = 0.0f;		// �����x
	m_bLightOff = false;	// ���C�e�B���O��

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CLocus3D::Uninit(void)
{
	// �I��
	CBillboard::Uninit();
}

//=========================
// �X�V����
//=========================
void CLocus3D::Update(void)
{
	// ���������Z����
	m_nLife--;

	// �����x�����Z����
	m_fAlpha -= m_fSub;

	if (m_fAlpha <= 0.0f)
	{ // �����x�� 0.0f �ȉ��ɂȂ����ꍇ

		// �����x��␳����
		m_fAlpha = 0.0f;
	}

	if (m_nLife <= 0)
	{ // ������ 0 �ȉ��ɂȂ����ꍇ

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// ���_���̏�����
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// �`�揈��
//=========================
void CLocus3D::Draw(void)
{
	if (m_bLightOff == true)
	{ // ���C�e�B���O�󋵂� true �̏ꍇ

		// �`�揈��
		CBillboard::DrawLightOff(true);
	}
	else
	{ // ��L�ȊO

		// �`�揈��
		CBillboard::Draw();
	}
}
//=========================
// ���̐ݒ菈��
//=========================
void CLocus3D::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx, const bool bLookY, const bool bLighting)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);			// �ʒu�ݒ�
	SetRot(0.0f);			// �����ݒ�
	SetSize(size);			// �T�C�Y�ݒ�
	SetLength();			// �����ݒ�
	SetAngle();				// �����ݒ�
	SetEnableLookY(bLookY);	// Y���ݒ�

	// �S�Ă̒l������������
	m_nLife = nLife;			// ����
	m_fSub = fAlpha / (float)(nLife);	// �����x�̌�����
	m_fAlpha = fAlpha;			// �����x
	m_bLightOff = bLighting;	// ���C�e�B���O��

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(nTexIdx);
}

//=========================
// ��������
//=========================
CLocus3D* CLocus3D::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx, const bool bLookY, const bool bLighting)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CLocus3D* pLocus3D = nullptr;	// �c���̃C���X�^���X�𐶐�

	if (pLocus3D == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pLocus3D = new CLocus3D;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pLocus3D != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pLocus3D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pLocus3D->SetData(pos, size, fAlpha, nLife, nTexIdx, bLookY, bLighting);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �c���̃|�C���^��Ԃ�
	return pLocus3D;
}