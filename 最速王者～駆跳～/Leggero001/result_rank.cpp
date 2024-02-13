//============================================
//
// ���U���g�����N����[result_rank.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "result_rank.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"

//========================
// �R���X�g���N�^
//========================
CResultRank::CResultRank() : CModel(TYPE_RESULTRANK, PRIORITY_UI)
{

}

//========================
// �f�X�g���N�^
//========================
CResultRank::~CResultRank()
{

}

//========================
// ����������
//========================
HRESULT CResultRank::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CResultRank::Uninit(void)
{
	// �I������
	CModel::Uninit();
}

//========================
// �X�V����
//========================
void CResultRank::Update(void)
{

}

//========================
// �`�揈��
//========================
void CResultRank::Draw(void)
{
	// ��ʃN���A(Z�o�b�t�@�̃N���A)
	CManager::GetRenderer()->GetDevice()->Clear(0,
		NULL,
		D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`�揈��
	CModel::Draw();
}

//========================
// ���̐ݒ菈��
//========================
void CResultRank::SetData(const D3DXVECTOR3 pos, const CResult::RANK rank)
{
	// ��������������
	SetPos(pos);
	SetPosOld(pos);
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetScale(NONE_SCALE);

	switch (rank)
	{
	case CResult::RANK_SS:		// SS�����N

		// ���f���̐ݒ菈��
		SetFileData(CXFile::TYPE_RANK_SS);

		break;

	case CResult::RANK_S:		// S�����N

		// ���f���̐ݒ菈��
		SetFileData(CXFile::TYPE_RANK_S);

		break;

	case CResult::RANK_A:		// A�����N

		// ���f���̐ݒ菈��
		SetFileData(CXFile::TYPE_RANK_A);

		break;

	case CResult::RANK_B:		// B�����N

		// ���f���̐ݒ菈��
		SetFileData(CXFile::TYPE_RANK_B);

		break;

	case CResult::RANK_C:		// C�����N

		// ���f���̐ݒ菈��
		SetFileData(CXFile::TYPE_RANK_C);

		break;
	}
}

//========================
// ��������
//========================
CResultRank* CResultRank::Create(const D3DXVECTOR3 pos, const CResult::RANK rank)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CResultRank* pRank = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pRank == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pRank = new CResultRank;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRank != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pRank->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���U���g�����N�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pRank->SetData(pos, rank);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pRank;
}