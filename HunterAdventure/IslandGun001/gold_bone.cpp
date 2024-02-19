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
	const float ROT_MOVE = 0.01f;		// ���f���̌����̈ړ���
	const float GET_HEIGHT = 300.0f;	// �擾���̍���
	const float ADD_HEIGHT = 30.0f;		// �����̉��Z��
	const float ADD_ROT_MOVE = 0.01f;	// �����̈ړ��ʂ̉��Z��
	const float DELETE_ROT_MOVE = 0.8f;	// ������������̈ړ���
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
	// �S�Ă̒l���N���A����
	m_state = STATE_NONE;		// ���
	m_fDestHeight = 0.0f;		// �ړI�̍���
	m_fRotMove = ROT_MOVE;		// �����̈ړ���

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
	switch (m_state)
	{
	case CGoldBone::STATE_NONE:

		break;

	case CGoldBone::STATE_GET:

		// �擾����
		if (Get() == true)
		{ // �������ꍇ

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ��]����
	Cycle();
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

	// �S�Ă̒l��ݒ肷��
	m_state = STATE_NONE;		// ���
	m_fDestHeight = 0.0f;		// �ړI�̍���
	m_fRotMove = ROT_MOVE;		// �����̈ړ���
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
	// �擾��Ԃɂ���
	m_state = STATE_GET;

	// �ړI�̍�����ݒ肷��
	m_fDestHeight = GetPos().y + GET_HEIGHT;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CGoldBone::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CGoldBone::STATE CGoldBone::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// ��]����
//=======================================
void CGoldBone::Cycle(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// ��
	rot.y += m_fRotMove;

	// �����̐��K��
	useful::RotNormalize(&rot.y);

	// ������K�p����
	SetRot(rot);
}

//=======================================
// �擾����
//=======================================
bool CGoldBone::Get(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �ʒu�����̏ꏊ�܂ŏグ��
	useful::FrameCorrect(m_fDestHeight, &pos.y, ADD_HEIGHT);

	// �ʒu��K�p����
	SetPos(pos);

	// �����̈ړ��ʂ����Z����
	m_fRotMove += ADD_ROT_MOVE;

	if (m_fRotMove >= DELETE_ROT_MOVE)
	{ // �����̈ړ��ʂ���萔�ȏ�ɂȂ����ꍇ

		// �I������
		Uninit();

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}