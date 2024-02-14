//=======================================
//
// ���[�V�����u���[�̃��C������[blur.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "blur.h"
#include "useful.h"

//=========================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================
CBlur::CBlur(PRIORITY priority) : CModel(CObject::TYPE_LOCUSMODEL, priority)
{
	// �S�Ă̒l���N���A����
	m_nLife = 0;		// ����
	m_fSub = 0.0f;		// ���Z��
}

//=========================
// �f�X�g���N�^
//=========================
CBlur::~CBlur()
{

}

//=========================
// ����������
//=========================
HRESULT CBlur::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_nLife = 0;		// ����
	m_fSub = 0.0f;		// ���Z��

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CBlur::Uninit(void)
{
	// �I��
	CModel::Uninit();
}

//=========================
// �X�V����
//=========================
void CBlur::Update(void)
{
	// ���������炵�Ă���
	m_nLife--;

	if (m_nLife <= 0)
	{ // �G�t�F�N�g�̎������s������

		// �j������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//=========================
// �`�揈��
//=========================
void CBlur::Draw(void)
{

}

//=========================
// ���̐ݒ菈��
//=========================
void CBlur::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile& filedata, const int nLife, const float fAlpha)
{
	// ���̐ݒ菈��
	SetPos(pos);				// �ʒu
	SetPosOld(pos);				// �O��̈ʒu
	SetRot(rot);				// ����
	SetScale(scale);			// �g�嗦
	SetFileData(filedata);		// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_nLife = nLife;			// ����
	m_fSub = fAlpha / nLife;	// ���Z��
}

//=========================
// ���Z�ʂ̎擾����
//=========================
float CBlur::GetSub(void) const
{
	// ���Z�ʂ��擾����
	return m_fSub;
}