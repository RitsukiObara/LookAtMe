//============================================================
//
// �{�X���� [boss.cpp]
// Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "model.h"
#include "boss.h"
#include "manager.h"
#include "useful.h"

#include "motion.h"
#include "boss_appearstate.h"
#include "objectElevation.h"

//------------------------------------------------------------
// �������O���
//------------------------------------------------------------
namespace
{
	const float LAND_GRAVITY = -50.0f;		// ���n���̏d��
}

// �ÓI�����o�ϐ�
CListManager<CBoss*> CBoss::m_list = {};				// ���X�g

//================================
// �R���X�g���N�^
//================================
CBoss::CBoss() : CCharacter(CObject::TYPE_BOSS, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pMotion = nullptr;			// ���[�V����
	m_pState = nullptr;				// ��Ԃ̏��

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//================================
// �f�X�g���N�^
//================================
CBoss::~CBoss()
{

}

//================================
// ����������
//================================
HRESULT CBoss::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���f���̑�����ݒ�
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_BOSS));

	// �f�[�^�̐ݒ菈��
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // ���[�V������ NULL �������ꍇ

		if (m_pMotion == nullptr)
		{ // ���[�V������ NULL �̏ꍇ

			// ���[�V�����𐶐�����
			m_pMotion = CMotion::Create();
		}
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	if (m_pMotion != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ���[�V�����̏����擾����
		m_pMotion->SetInfo(CMotion::STYLE_BOSS, GetHierarchy(), GetNumModel());
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	// �l��Ԃ�
	return S_OK;
}

//================================
// �I������
//================================
void CBoss::Uninit(void)
{
	if (m_pMotion != nullptr)
	{ // ���[�V������ NULL ����Ȃ��ꍇ

		// ���[�V�����̏I������
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pState != nullptr)
	{ // ��Ԃ� NULL ����Ȃ��ꍇ

		// ��Ԃ̏I������
		m_pState->Uninit();
		m_pState = nullptr;
	}

	// �I������
	CCharacter::Uninit();

	// ������������
	m_list.Pull(this);
}

//================================
// �X�V����
//================================
void CBoss::Update(void)
{
	if (m_pState != nullptr)
	{ // ��Ԃ� NULL ����Ȃ��ꍇ

		// ��ԏ���
		m_pState->Process(this);
	}

	// ���[�V�����̍X�V����
	m_pMotion->Update();
}

//================================
// �`�揈��
//================================
void CBoss::Draw(void)
{
	// �`�揈��
	CCharacter::Draw();
}

//================================
// �q�b�g����
//================================
void CBoss::Hit(void)
{
	// ��~
	assert(false);
}

//================================
// ���̐ݒ菈��
//================================
void CBoss::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// �S�Ă̒l������������
	SetPos(pos);				// �ʒu
	SetPosOld(GetPos());		// �O��̈ʒu
	SetRot(rot);				// ����
	SetScale(NONE_SCALE);		// �g�嗦

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// ����������
		GetHierarchy(nCntData)->SetPos(pos);										// �ʒu
		GetHierarchy(nCntData)->SetPosOld(pos);										// �O��̈ʒu
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// ����
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);								// �g�嗦
	}

	// ���[�V�����̃��Z�b�g����
	m_pMotion->ResetMotion(MOTIONTYPE_FLY);

	// ��Ԃ̐؂�ւ�����
	ChangeState(new CBossAppearState);
}

//===========================================
// ��������
//===========================================
CBoss* CBoss::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	if (m_list.IsEmpty() == false)
	{ // �󔒏�Ԃ̏ꍇ

		// �������Ȃ�(���}���u)
		return nullptr;
	}

	// �{�X�̃C���X�^���X
	CBoss* pBoss = nullptr;

	if (pBoss == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �{�X�𐶐�����
		pBoss = new CBoss;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return pBoss;
	}

	if (pBoss != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBoss->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBoss->SetData(pos, rot);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �{�X�̃|�C���^��Ԃ�
	return pBoss;
}

//===========================================
// ���X�g�̎擾����
//===========================================
CListManager<CBoss*> CBoss::GetList(void)
{
	// ���X�g�̏���Ԃ�
	return m_list;
}

//===========================================
// �N���n�ʂƂ̓����蔻��
//===========================================
bool CBoss::ElevationCollision(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	float fHeight = 0.0f;			// ����
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;	// �擪�̏���
	CElevation* pElevEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pElev = list.GetTop();

		// �����̒l���擾����
		pElevEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �����蔻������
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // �����蔻��̈ʒu�����������ꍇ

				// ������ݒ肷��
				pos.y = fHeight;

				// �ʒu���X�V����
				SetPos(pos);

				// true ��Ԃ�
				return true;
			}

			if (pElev == pElevEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pElev = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �ʒu���X�V����
	SetPos(pos);

	// false ��Ԃ�
	return false;
}

//===========================================
// ��Ԃ̑J�ڏ���
//===========================================
void CBoss::ChangeState(CBossState* pNext)
{
	if (m_pState != nullptr)
	{ // ��Ԃ� NULL ����Ȃ��ꍇ

		// ��Ԃ̏I������
		m_pState->Uninit();
		m_pState = nullptr;
	}

	// ���̏�Ԃ�ݒ肷��
	m_pState = pNext;

	if (m_pState != nullptr)
	{ // ��Ԃ� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		m_pState->SetData(this);
	}
}

//===========================================
// ���[�V�����̎擾����
//===========================================
CMotion* CBoss::GetMotion(void)
{
	// ���[�V�����̏���Ԃ�
	return m_pMotion;
}