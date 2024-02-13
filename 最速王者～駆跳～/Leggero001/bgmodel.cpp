//============================================
//
// �w�i���f���̃��C������[bgmodel.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "bgmodel.h"
#include "manager.h"
#include "renderer.h"
#include "useful.h"

//===========================================
// �R���X�g���N�^
//===========================================
CBgModel::CBgModel() : CModel(TYPE_BGOBJECT, PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	m_type = CXFile::TYPE_ENERGYITEM;	// ���

	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		// �}�e���A���̃f�[�^
		ZeroMemory(&m_aMat[nCntMat], sizeof(D3DXMATERIAL));
	}
}

//===========================================
// �f�X�g���N�^
//===========================================
CBgModel::~CBgModel()
{

}

//===========================================
// ����������
//===========================================
HRESULT CBgModel::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	m_type = CXFile::TYPE_ENERGYITEM;	// ���

	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		// �}�e���A���̃f�[�^
		ZeroMemory(&m_aMat[nCntMat], sizeof(D3DXMATERIAL));
	}

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CBgModel::Uninit(void)
{
	// �I������
	CModel::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CBgModel::Update(void)
{

}

//===========================================
// �`�揈��
//===========================================
void CBgModel::Draw(void)
{
	// �`�揈��
	CModel::DrawBlock(m_aMat);
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CBgModel::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const CXFile::TYPE type)
{
	// ��ނ�ݒ肷��
	m_type = type;

	for (int nCntMat = 0; nCntMat < (int)CXFile::GetXFile(type).dwNumMat; nCntMat++)
	{
		// �}�e���A���ւ̃|�C���^
		m_aMat[nCntMat] = pMat[nCntMat];
	}

	// ����������
	SetPos(pos);			// �ʒu�ݒ�
	SetPosOld(pos);			// �O��̈ʒu�ݒ菈��
	SetRot(rot);			// �����ݒ�
	SetScale(scale);		// �g�嗦�ݒ�
	SetFileData(type);		// �f�[�^�̐ݒ菈��
		
	CXFile::SXFile file = GetFileData();

	// �ő�l�𔽉f����
	file.vtxMax.x *= scale.x;
	file.vtxMax.y *= scale.y;
	file.vtxMax.z *= scale.z;

	// �ŏ��l�𔽉f����
	file.vtxMin.x *= scale.x;
	file.vtxMin.y *= scale.y;
	file.vtxMin.z *= scale.z;

	// �f�[�^�̐ݒ菈��
	SetFileData(file);
}

//===========================================
// ��������
//===========================================
CBgModel* CBgModel::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const CXFile::TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBgModel* pBgModel = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pBgModel == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBgModel = new CBgModel;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBgModel != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBgModel->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�u���b�N�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBgModel->SetData(pos, rot, scale, pMat, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �v���C���[�̃|�C���^��Ԃ�
	return pBgModel;
}