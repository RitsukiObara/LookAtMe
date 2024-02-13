//===========================================
//
// �u���b�N�̃��C������[block.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "block.h"
#include "useful.h"
#include "shadowCircle.h"

#include "dirtblock.h"
#include "rockblock.h"
#include "iceblock.h"
#include "magmablock.h"

#include "cliffchecker.h"

//==============================
// �R���X�g���N�^
//==============================
CBlock::CBlock() : CModel(CObject::TYPE_BLOCK, CObject::PRIORITY_BLOCK)
{
	// �R���X�g���N�^�̔�
	Box();
}

//==============================
// �R���X�g���N�^
//==============================
CBlock::CBlock(CObject::TYPE type, CObject::PRIORITY priority) : CModel(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//==============================
// �f�X�g���N�^
//==============================
CBlock::~CBlock()
{

}

//==============================
// �R���X�g���N�^�̔�
//==============================
void CBlock::Box(void)
{
	// �S�Ă̒l���N���A����
	m_type = CBlock::TYPE_DIRT;				// ���

	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		// �}�e���A���ւ̃|�C���^
		ZeroMemory(&m_aMat[nCntMat], sizeof(D3DXMATERIAL));
	}

	m_bCliffRight = false;						// �E�����R���܂�o���邩
	m_bCliffLeft = false;						// �������R���܂�o���邩

// �f�o�b�O����
#ifdef _DEBUG

	m_bDelete = false;							// ������
#endif 
}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CBlock::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_type = TYPE_DIRT;		// ���

	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		// �}�e���A���ւ̃|�C���^
		ZeroMemory(&m_aMat[nCntMat], sizeof(D3DXMATERIAL));
	}

	m_bCliffRight = false;		// �E�����R���܂�o���邩
	m_bCliffLeft = false;		// �������R���܂�o���邩

// �f�o�b�O����
#ifdef _DEBUG

	m_bDelete = false;			// ������
#endif 

	// �l��Ԃ�
	return S_OK;
}

//========================================
//�u���b�N�̏I������
//========================================
void CBlock::Uninit(void)
{
	// �I������
	CModel::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CBlock::Update(void)
{

}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CBlock::Draw(void)
{
	// �`�揈��
	CModel::DrawBlock(&m_aMat[0]);
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CBlock::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const TYPE type, const bool bCliffR, const bool bCliffL)
{
	for (int nCntMat = 0; nCntMat < (int)GetFileData().dwNumMat; nCntMat++)
	{
		// �}�e���A���ւ̃|�C���^
		m_aMat[nCntMat] = pMat[nCntMat];
	}

	// �R���܂��Ԃ�ݒ肷��
	m_bCliffRight = bCliffR;		// �E�R
	m_bCliffLeft = bCliffL;			// ���R
	m_type = type;					// ���

	// �f�o�b�O����
#ifdef _DEBUG

	m_bDelete = false;								// ������
#endif 

	// ����������
	SetPos(pos);			// �ʒu�ݒ�
	SetPosOld(pos);			// �O��̈ʒu�ݒ菈��
	SetRot(rot);			// �����ݒ�
	SetScale(scale);		// �g�嗦�ݒ�

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

	// ���[�J���ϐ��錾
	D3DXVECTOR3 Blockpos;		// �ʒu

	if (GetCliffLeft() == true)
	{ // ���̊R�ɒ͂܂��ꍇ

		// �ʒu���擾����
		Blockpos = GetPos();

		// �ʒu��ݒ肷��
		Blockpos.x += GetFileData().vtxMin.x;
		Blockpos.y += GetFileData().vtxMax.y;
		Blockpos.z += GetFileData().vtxMin.z;

		// �R�߂܂�`�F�b�J�[�̐�������
		CCliffChecker::Create(Blockpos);
	}

	if (GetCliffRight() == true)
	{ // �E�̊R�ɒ͂܂��ꍇ

		// �ʒu���擾����
		Blockpos = GetPos();

		// �ʒu��ݒ肷��
		Blockpos.x += GetFileData().vtxMax.x;
		Blockpos.y += GetFileData().vtxMax.y;
		Blockpos.z += GetFileData().vtxMin.z;

		// �R�߂܂�`�F�b�J�[�̐�������
		CCliffChecker::Create(Blockpos);
	}
}

//=======================================
// ��ނ̐ݒ菈��
//=======================================
void CBlock::SetType(const TYPE type)
{
	// ��ނ�ݒ肷��
	m_type = type;
}

//=======================================
// ��ނ̎擾����
//=======================================
CBlock::TYPE CBlock::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//=======================================
// �E�R�̐ݒ菈��
//=======================================
void CBlock::SetCliffRight(const bool bCliff)
{
	// �E�R�̔����ݒ肷��
	m_bCliffRight = bCliff;
}

//=======================================
// �E�R�̎擾����
//=======================================
bool CBlock::GetCliffRight(void) const
{
	// �E�R�̔����Ԃ�
	return m_bCliffRight;
}

//=======================================
// ���R�̐ݒ菈��
//=======================================
void CBlock::SetCliffLeft(const bool bCliff)
{
	// ���R�̔����ݒ肷��
	m_bCliffLeft = bCliff;
}

//=======================================
// ���R�̎擾����
//=======================================
bool CBlock::GetCliffLeft(void) const
{
	// ���R�̔����Ԃ�
	return m_bCliffLeft;
}

//=======================================
// �}�e���A���̎擾����
//=======================================
D3DXMATERIAL CBlock::GetMaterial(int nIdx) const
{
	// �}�e���A���̏���Ԃ�
	return m_aMat[nIdx];
}

//=======================================
// ��������
//=======================================
CBlock* CBlock::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const TYPE type, const bool bCliffR, const bool bCliffL)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBlock* pBlock = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pBlock == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		switch (type)
		{
		case TYPE_DIRT:			// �n�ʃu���b�N

			// �n�ʃu���b�N�𐶐�
			pBlock = new CDirtBlock;

			break;

		case TYPE_ROCK:			// ��u���b�N

			// ��u���b�N�𐶐�
			pBlock = new CRockBlock;

			break;

		case TYPE_ICE:			// �X�u���b�N

			// �X�u���b�N�𐶐�
			pBlock = new CIceBlock;

			break;

		case TYPE_MAGMA:		// �}�O�}�u���b�N

			// �}�O�}�u���b�N�𐶐�
			pBlock = new CMagmaBlock;

			break;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBlock != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBlock->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�u���b�N�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBlock->SetData(pos, rot, scale, pMat, type, bCliffR, bCliffL);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �v���C���[�̃|�C���^��Ԃ�
	return pBlock;
}

// �f�o�b�O����
#ifdef _DEBUG

//=======================================
// �����󋵂̐ݒ菈��
//=======================================
void CBlock::SetDelete(const bool bDelete)
{
	// �����󋵂�ݒ肷��
	m_bDelete = bDelete;
}

//=======================================
// �����󋵂̎擾����
//=======================================
bool CBlock::GetDelete(void) const
{
	// �����󋵂�Ԃ�
	return m_bDelete;
}

#endif