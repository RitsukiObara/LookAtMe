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
#include "block.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\IronBlock.x";		// ���f���̖��O
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CBlock*> CBlock::m_list = {};			// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CBlock::CBlock() : CModel(TYPE_BLOCK, PRIORITY_BLOCK)
{
	// �S�Ă̒l���N���A����
	m_vtxMax = NONE_D3DXVECTOR3;		// ���_�̍ő�l
	m_vtxMin = NONE_D3DXVECTOR3;		// ���_�̍ŏ��l

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CBlock::~CBlock()
{

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

	// ������������
	m_list.Pull(this);
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
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CBlock::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetScale(scale);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f���̏��

	// ���f���̏����擾����
	CXFile::SXFile file = GetFileData();

	// �ő�l�𔽉f����
	m_vtxMax.x = file.vtxMax.x * scale.x;
	m_vtxMax.y = file.vtxMax.y * scale.y;
	m_vtxMax.z = file.vtxMax.z * scale.z;

	// �ŏ��l�𔽉f����
	m_vtxMin.x = file.vtxMin.x * scale.x;
	m_vtxMin.y = file.vtxMin.y * scale.y;
	m_vtxMin.z = file.vtxMin.z * scale.z;
}

//=======================================
// ��������
//=======================================
CBlock* CBlock::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBlock* pBlock = nullptr;		// �C���X�^���X�𐶐�

	if (pBlock == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �u���b�N�𐶐�����
		pBlock = new CBlock;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBlock != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBlock->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBlock->SetData(pos, scale);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �u���b�N�̃|�C���^��Ԃ�
	return pBlock;
}

//=======================================
// ���_�̍ő�l�̎擾����
//=======================================
D3DXVECTOR3 CBlock::GetVtxMax(void) const
{
	// ���_�̍ő�l��Ԃ�
	return m_vtxMax;
}

//=======================================
// ���_�̍ŏ��l�̎擾����
//=======================================
D3DXVECTOR3 CBlock::GetVtxMin(void) const
{
	// ���_�̍ŏ��l��Ԃ�
	return m_vtxMin;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CBlock*> CBlock::GetList(void)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_list;
}