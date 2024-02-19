//===========================================
//
// �v���y���̃��C������[propeller.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "propeller.h"
#include "useful.h"

//-------------------------------------------
// �萔��`
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Propeller.x";		// ���f���̃p�X��
	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(0.0f, 180.0f, -1570.0f);		// �I�t�Z�b�g���W
	const float ROT_MOVE = 1.3f;		// �����̈ړ���
}

//==============================
// �R���X�g���N�^
//==============================
CPropeller::CPropeller() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pMtxParent = nullptr;		// �e�̃}�g���b�N�X
}

//==============================
// �f�X�g���N�^
//==============================
CPropeller::~CPropeller()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CPropeller::Init(void)
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
// �j�Ђ̏I������
//========================================
void CPropeller::Uninit(void)
{
	// �I������
	CModel::Uninit();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CPropeller::Update(void)
{
	// �������擾
	D3DXVECTOR3 rot = GetRot();

	// ���������Z����
	rot.z += ROT_MOVE;

	// �����̐��K��
	useful::RotNormalize(&rot.z);

	// ������K�p
	SetRot(rot);
}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CPropeller::Draw(void)
{
	// �`�揈��
	CModel::DrawMatrix(*m_pMtxParent);
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CPropeller::SetData(D3DXMATRIX* pMtx)
{
	// ���̐ݒ菈��
	SetPos(OFFSET);						// �ʒu
	SetPosOld(OFFSET);					// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);			// ����
	SetScale(NONE_SCALE);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f�����

	// �S�Ă̒l��ݒ肷��
	m_pMtxParent = pMtx;		// �e�̃}�g���b�N�X
}

//=======================================
// ��������
//=======================================
CPropeller* CPropeller::Create(D3DXMATRIX* pMtx)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CPropeller* pPro = nullptr;		// �C���X�^���X�𐶐�

	if (pPro == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pPro = new CPropeller;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pPro != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pPro->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pPro->SetData(pMtx);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �v���y���̃|�C���^��Ԃ�
	return pPro;
}