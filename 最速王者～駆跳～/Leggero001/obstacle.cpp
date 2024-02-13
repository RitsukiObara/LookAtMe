//===========================================
//
// ��Q���̃��C������[obstacle.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "obstacle.h"
#include "object.h"
#include "useful.h"
#include "shadowCircle.h"

#include "hydrant.h"
#include "signpole.h"
#include "drum.h"
#include "woodbox.h"
#include "streetlamp.h"
#include "streetpole.h"

//==============================
// �R���X�g���N�^
//==============================
CObstacle::CObstacle() : CModel(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// �R���X�g���N�^�̔�
	Box();
}

//==============================
// �R���X�g���N�^
//==============================
CObstacle::CObstacle(CObject::TYPE type, CObject::PRIORITY priority) : CModel(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//==============================
// �f�X�g���N�^
//==============================
CObstacle::~CObstacle()
{

}

//==============================
// �R���X�g���N�^�̔�
//==============================
void CObstacle::Box(void)
{
	// �S�Ă̒l���N���A����
	m_type = CObstacle::TYPE_HYDRANT;		// ���
	m_nShadowIdx = INIT_SHADOW;				// �e�̃C���f�b�N�X
	m_bCollision = true;					// �����蔻�肪�@�\���邩
	m_bAttack = false;						// �U���o���邩�ǂ���
	m_bRangeWithin = false;					// �U���͈͓����ǂ���

// �f�o�b�O����
#ifdef _DEBUG

	m_bDelete = false;							// ������
#endif 
}

//==============================
//��Q���̏���������
//==============================
HRESULT CObstacle::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	m_type = CObstacle::TYPE_HYDRANT;		// ���
	m_nShadowIdx = INIT_SHADOW;				// �e�̃C���f�b�N�X
	m_bCollision = true;					// �����蔻�肪�@�\���邩
	m_bAttack = false;						// �U���o���邩�ǂ���
	m_bRangeWithin = false;					// �U���͈͓����ǂ���

// �f�o�b�O����
#ifdef _DEBUG

	m_bDelete = false;							// ������
#endif

	// �l��Ԃ�
	return S_OK;
}

//========================================
//��Q���̏I������
//========================================
void CObstacle::Uninit(void)
{
	//// �e�̃|�C���^���擾����
	//CShadowCircle* pShadow = CShadowCircle::GetShadow(m_nShadowIdx);

	//if (CShadowCircle::GetShadow(m_nShadowIdx) != nullptr)
	//{ // �e�̃|�C���^���������ꍇ

	//	// �e�̏I������
	//	pShadow->Uninit();
	//	m_nShadowIdx = INIT_SHADOW;		// �e�̃C���f�b�N�X
	//}

	// �I������
	CModel::Uninit();
}

//========================================
//��Q���̍X�V����
//========================================
void CObstacle::Update(void)
{
	// �e�̈ʒu�ݒ菈��
	ShadowPosSet();
}

//=====================================
//��Q���̕`�揈��
//=====================================
void CObstacle::Draw(void)
{
	if (m_bRangeWithin == true)
	{ // �U���͈͓��̏ꍇ

		// �`�揈��
		CModel::Draw(D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f));
	}
	else
	{ // ��L�ȊO

		// �`�揈��
		CModel::Draw();
	}
}

//=======================================
// �q�b�g����
//=======================================
bool CObstacle::Hit(void)
{
	// false ��Ԃ�
	return false;
}

//=======================================
// ���̐ݒ菈��
//=======================================
void CObstacle::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// �f�o�b�O����
#ifdef _DEBUG

	m_bDelete = false;				// ������
#endif 

	m_type = type;					// ��ނ�ݒ肷��
	m_bCollision = true;			// �����蔻�肪�@�\���邩
	m_bAttack = true;				// �U���o���邩�ǂ���
	m_bRangeWithin = false;			// �U���͈͓����ǂ���

	// ����������
	SetPos(pos);								// �ʒu�ݒ�
	SetPosOld(pos);								// �O��̈ʒu�ݒ菈��
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �����ݒ�
	SetScale(NONE_SCALE);						// �g�嗦�ݒ�

	//// ���[�J���ϐ��錾
	//CShadowCircle* pShadow = nullptr;		// �e�ւ̃|�C���^

	//// �e�̐ݒ菈��
	//pShadow = pShadow->CShadowCircle::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetFileData().fRadius);

	//// �e�̃C���f�b�N�X�ݒ�
	//m_nShadowIdx = pShadow->GetNumID();
}

//=======================================
// ��ނ̐ݒ菈��
//=======================================
void CObstacle::SetType(const TYPE type)
{
	// ��ނ�ݒ肷��
	m_type = type;
}

//=======================================
// ��ނ̎擾����
//=======================================
CObstacle::TYPE CObstacle::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//=======================================
// �����蔻��̐ݒ菈��
//=======================================
void CObstacle::SetCollision(const bool bCollision)
{
	// �����蔻��󋵂�ݒ肷��
	m_bCollision = bCollision;
}

//=======================================
// �����蔻��̎擾����
//=======================================
bool CObstacle::GetCollision(void) const
{
	// �����蔻��󋵂�Ԃ�
	return m_bCollision;
}

//=======================================
// �A�N�V�����̐ݒ菈��
//=======================================
void CObstacle::SetAttack(const bool bAttack)
{
	// �A�N�V���������ݒ肷��
	m_bAttack = bAttack;
}

//=======================================
// �A�N�V�����̎擾����
//=======================================
bool CObstacle::GetAttack(void) const
{
	// �A�N�V���������Ԃ�
	return m_bAttack;
}

//=======================================
// �U���͈͓����ǂ����̐ݒ菈��
//=======================================
void CObstacle::SetRange(const bool bRange)
{
	// �U���͈͓����ǂ�����ݒ肷��
	m_bRangeWithin = bRange;
}

//=======================================
// �U���͈͓����ǂ����̎擾����
//=======================================
bool CObstacle::GetRange(void) const
{
	// �U���͈͓����ǂ�����Ԃ�
	return m_bRangeWithin;
}

//=======================================
// ��������
//=======================================
CObstacle* CObstacle::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CObstacle* pObstacle = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pObstacle == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		switch (type)
		{
		case TYPE_HYDRANT:		// ���ΐ�

			// ���ΐ��̃I�u�W�F�N�g�𐶐�
			pObstacle = new CHydrant;

			break;

		case TYPE_SIGNPOLE:		// �W��

			// �W���̃I�u�W�F�N�g�𐶐�
			pObstacle = new CSignPole;

			break;

		case TYPE_DRUM:

			// �h�����ʂ̃I�u�W�F�N�g�𐶐�
			pObstacle = new CDrum;

			break;

		case TYPE_WOODBOX:		// �ؔ�

			// �ؔ��̃I�u�W�F�N�g�𐶐�
			pObstacle = new CWoodBox;

			break;

		case TYPE_LAMP:			// �X��(����)

			// �X��(����)�̃I�u�W�F�N�g�𐶐�
			pObstacle = new CStreetLamp;

			break;

		case TYPE_POLE:			// �X��(�x��)

			// �X��(�x��)�̃I�u�W�F�N�g�𐶐�
			pObstacle = new CStreetPole;

			break;

		default:				// ��L�ȊO

			// �x����
			MessageBox(NULL, "��Q���̐����ŋK��O�̐��l�����o����܂����I", "�x���I", MB_ICONWARNING);

			break;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pObstacle != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pObstacle->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "��Q���̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pObstacle->SetData(pos, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �v���C���[�̃|�C���^��Ԃ�
	return pObstacle;
}

//=======================================
// �e�̈ʒu�ݒ菈��
//=======================================
void CObstacle::ShadowPosSet(void)
{
	//// ���[�J���ϐ��錾
	//CShadowCircle* pShadow = CShadowCircle::GetShadow(m_nShadowIdx);									// �e�̃|�C���^

	//if (pShadow != nullptr)
	//{ // �e�̃|�C���^�� NULL ����Ȃ��ꍇ

	//	// �e�̈ʒu�����ݒ菈��
	//	pShadow->SetPosRot(m_nShadowIdx, D3DXVECTOR3(GetPos().x, 0.0f, GetPos().z), GetRot());
	//}
}

// �f�o�b�O����
#ifdef _DEBUG

//=======================================
// �����󋵂̐ݒ菈��
//=======================================
void CObstacle::SetDelete(const bool bDelete)
{
	// �����󋵂�ݒ肷��
	m_bDelete = bDelete;
}

//=======================================
// �����󋵂̎擾����
//=======================================
bool CObstacle::GetDelete(void) const
{
	// �����󋵂�Ԃ�
	return m_bDelete;
}

#endif