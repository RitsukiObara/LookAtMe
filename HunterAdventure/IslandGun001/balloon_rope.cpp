//===========================================
//
// ���D�̕R�̃��C������[balloon_rope.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "balloon_rope.h"
#include "useful.h"

//-------------------------------------------
// �萔��`
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Balloon_rope.x";			// �R���f���̖��O
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, -75.0f, 0.0f);	// �ʒu
}

//==============================
// �R���X�g���N�^
//==============================
CBalloonRope::CBalloonRope() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pMtxParent = nullptr;			// �e�̃}�g���b�N�X
}

//==============================
// �f�X�g���N�^
//==============================
CBalloonRope::~CBalloonRope()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CBalloonRope::Init(void)
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
void CBalloonRope::Uninit(void)
{
	// �e�̃}�g���b�N�X�� NULL �ɂ���
	m_pMtxParent = nullptr;

	// �I������
	CModel::Uninit();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CBalloonRope::Update(void)
{

}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CBalloonRope::Draw(void)
{
	// �`�揈��
	CModel::DrawMatrix(*m_pMtxParent);
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CBalloonRope::SetData(D3DXMATRIX* mtx)
{
	// ���̐ݒ菈��
	SetPos(POS);					// �ʒu
	SetPosOld(POS);					// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f�����

	// �S�Ă̒l���N���A����
	m_pMtxParent = mtx;				// �e�̃}�g���b�N�X
}

//=======================================
// ��������
//=======================================
CBalloonRope* CBalloonRope::Create(D3DXMATRIX* mtx)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBalloonRope* pRope = nullptr;		// �C���X�^���X�𐶐�

	if (pRope == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pRope = new CBalloonRope;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRope != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pRope->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pRope->SetData(mtx);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �R�̃|�C���^��Ԃ�
	return pRope;
}