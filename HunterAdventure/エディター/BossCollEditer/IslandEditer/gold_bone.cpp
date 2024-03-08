//===========================================
//
// ���̍��̃��C������[gold_bone.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "gold_bone.h"
#include "renderer.h"
#include "useful.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\GoldBone.x";		// ���f���̖��O
	const float ROT_MOVE = 0.01f;						// ���f���̌����̈ړ���
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CGoldBone*> CGoldBone::m_list = {};		// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CGoldBone::CGoldBone() : CModel(CObject::TYPE_GOLDBONE, CObject::PRIORITY_ENTITY)
{
	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CGoldBone::~CGoldBone()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CGoldBone::Init(void)
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
void CGoldBone::Uninit(void)
{
	// �I������
	CModel::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CGoldBone::Update(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// ��
	rot.y += ROT_MOVE;

	// �����̐��K��
	useful::RotNormalize(&rot.y);

	// ������K�p����
	SetRot(rot);
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CGoldBone::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CGoldBone::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos);							// �ʒu
	SetPosOld(pos);							// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);				// ����
	SetScale(NONE_SCALE);					// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));		// ���f���̏��
}

//=======================================
// ��������
//=======================================
CGoldBone* CGoldBone::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CGoldBone* pBone = nullptr;	// �C���X�^���X�𐶐�

	if (pBone == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pBone = new CGoldBone;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBone != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBone->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBone->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���̍��̃|�C���^��Ԃ�
	return pBone;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CGoldBone*> CGoldBone::GetList(void)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_list;
}

//=======================================
// �q�b�g����
//=======================================
void CGoldBone::Hit(void)
{
	// �I������
	Uninit();
}

//=======================================
// �擾��ԏ���
//=======================================
void CGoldBone::Get(void)
{

}