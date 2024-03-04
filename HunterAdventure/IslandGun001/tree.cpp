//===========================================
//
// �؂̃��C������[tree.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "tree.h"
#include "useful.h"

#include "palm_tree.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* FILE_NAME[CTree::TYPE_MAX] =	// X�t�@�C���̖��O
	{
		"data\\MODEL\\PalmTree.x",		// ���V�̖�
	};
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CTree*> CTree::m_aList[area::NUM_AREA] = {};		// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CTree::CTree(CObject::TYPE type, PRIORITY priority) : CModel(type, priority)
{
	// �S�Ă̒l���N���A����
	m_type = TYPE_PALM;		// ���
	m_nAreaIdx = 0;			// �敪�̔ԍ�
}

//==============================
// �f�X�g���N�^
//==============================
CTree::~CTree()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CTree::Init(void)
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
void CTree::Uninit(void)
{
	// ������������
	m_aList[m_nAreaIdx].Pull(this);

	// �I������
	CModel::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CTree::Update(void)
{

}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CTree::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CTree::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(rot);					// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(FILE_NAME[type]));	// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_type = type;		// ���

	// �敪�̔ԍ��̐ݒ菈��
	m_nAreaIdx = area::SetFieldIdx(GetPos());

	// ���X�g�ɒǉ�����
	m_aList[m_nAreaIdx].Regist(this);
}

//=====================================
// �q�b�g����
//=====================================
void CTree::Hit(void)
{

}

//=======================================
// ��������
//=======================================
CTree* CTree::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CTree* pTree = nullptr;	// �C���X�^���X�𐶐�

	if (pTree == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		switch (type)
		{
		case CTree::TYPE_PALM:		// ���V�̖�

			// ���V�̖؂𐶐�����
			pTree = new CPalmTree;

			break;

		default:

			// ��~
			assert(false);

			break;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pTree != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pTree->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pTree->SetData(pos, rot, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �؂̃|�C���^��Ԃ�
	return pTree;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CTree*> CTree::GetList(const int nIdx)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_aList[nIdx];
}

//=======================================
// ��ނ̎擾����
//=======================================
CTree::TYPE CTree::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//=======================================
// ���V�̎��̎擾����(���V�̖؂ŕK�v)
//=======================================
CPalmFruit* CTree::GetFruit(void)
{
	// NULL ��Ԃ�
	return nullptr;
}