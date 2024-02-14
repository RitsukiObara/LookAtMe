//=======================================
//
// ���[�V�����u���[(���̐F)�̃��C������[blur_org.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "blur_org.h"
#include "useful.h"

//=========================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================
CBlurOrg::CBlurOrg(PRIORITY priority) : CBlur(priority)
{
	// �S�Ă̒l���N���A����
	m_fAlpha = 0.0f;		// �����x
}

//=========================
// �f�X�g���N�^
//=========================
CBlurOrg::~CBlurOrg()
{

}

//=========================
// ����������
//=========================
HRESULT CBlurOrg::Init(void)
{
	if (FAILED(CBlur::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_fAlpha = 0.0f;		// �����x

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CBlurOrg::Uninit(void)
{
	// �I��
	CBlur::Uninit();
}

//=========================
// �X�V����
//=========================
void CBlurOrg::Update(void)
{
	// �����x�����Z����
	m_fAlpha -= GetSub();

	if (m_fAlpha <= 0.0f)
	{ // �����x�� 0.0f �ȉ��ɂȂ����ꍇ

		// �����x��␳����
		m_fAlpha = 0.0f;
	}

	// �X�V����
	CBlur::Update();
}

//=========================
// �`�揈��
//=========================
void CBlurOrg::Draw(void)
{
	// �`�揈��
	CModel::Draw(m_fAlpha);
}

//=========================
// ���̐ݒ菈��
//=========================
void CBlurOrg::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const float fAlpha, const CXFile::SXFile& filedata, const int nLife)
{
	// ���̐ݒ菈��
	CBlur::SetData(pos, rot, scale, filedata, nLife, fAlpha);

	// �S�Ă̒l��ݒ肷��
	m_fAlpha = fAlpha;		// �����x
}

//=========================
// ��������
//=========================
CBlurOrg* CBlurOrg::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const float fAlpha, const CXFile::SXFile& filedata, const int nLife, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBlurOrg* pLocus = nullptr;	// �c���̃C���X�^���X�𐶐�

	if (pLocus == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pLocus = new CBlurOrg(priority);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pLocus != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pLocus->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pLocus->SetData(pos, rot, scale, fAlpha, filedata, nLife);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �c���̃|�C���^��Ԃ�
	return pLocus;
}