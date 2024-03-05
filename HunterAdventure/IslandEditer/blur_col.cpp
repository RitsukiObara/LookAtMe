//=======================================
//
// ���[�V�����u���[(�F)�̃��C������[blur_col.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "blur_col.h"
#include "useful.h"

//=========================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================
CBlurCol::CBlurCol(PRIORITY priority) : CBlur(priority)
{
	// �S�Ă̒l���N���A����
	m_col = NONE_D3DXCOLOR;		// �F
}

//=========================
// �f�X�g���N�^
//=========================
CBlurCol::~CBlurCol()
{

}

//=========================
// ����������
//=========================
HRESULT CBlurCol::Init(void)
{
	if (FAILED(CBlur::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_col = NONE_D3DXCOLOR;		// �F

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CBlurCol::Uninit(void)
{
	// �I��
	CBlur::Uninit();
}

//=========================
// �X�V����
//=========================
void CBlurCol::Update(void)
{
	// �����x�����Z����
	m_col.a -= GetSub();

	if (m_col.a <= 0.0f)
	{ // �����x�� 0.0f �ȉ��ɂȂ����ꍇ

		// �����x��␳����
		m_col.a = 0.0f;
	}

	// �X�V����
	CBlur::Update();
}

//=========================
// �`�揈��
//=========================
void CBlurCol::Draw(void)
{
	// �`�揈��
	CModel::Draw(m_col);
}

//=========================
// ���̐ݒ菈��
//=========================
void CBlurCol::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const CXFile::SXFile& filedata, const int nLife)
{
	// ���̐ݒ菈��
	CBlur::SetData(pos, rot, scale, filedata, nLife, col.a);

	// �S�Ă̒l��ݒ肷��
	m_col = col;		// ����
}

//=========================
// ��������
//=========================
CBlurCol* CBlurCol::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const CXFile::SXFile& filedata, const int nLife, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBlurCol* pLocus = nullptr;	// �c���̃C���X�^���X�𐶐�

	if (pLocus == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pLocus = new CBlurCol(priority);
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
		pLocus->SetData(pos, rot, scale, col, filedata, nLife);
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