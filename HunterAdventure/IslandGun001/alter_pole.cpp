//===========================================
//
// �Β��̃��C������[alter_pole.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "alter_pole.h"
#include "texture.h"
#include "useful.h"

#include "gold_bone.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\AlterPole.x";		// ���f���̖��O
	const float BONE_ROT_MOVE = 0.1f;					// ���̍��̌����̈ړ���
	const float SUB_DISTANCE = 1.5f;					// ���̍��̋����̌��Z��
}

//==============================
// �R���X�g���N�^
//==============================
CAlterPole::CAlterPole() : CModel(TYPE_NONE, PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pGoldBone = nullptr;		// ���̍��̏��
	m_fBoneRot = 0.0f;			// ���̍��ւ̌���
	m_bEmpty = true;			// �󔒏�
	m_bArrival = false;			// ������
}

//==============================
// �f�X�g���N�^
//==============================
CAlterPole::~CAlterPole()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CAlterPole::Init(void)
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
void CAlterPole::Uninit(void)
{
	// ���̍��� NULL �ɂ���
	m_pGoldBone = nullptr;

	// �I������
	CModel::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CAlterPole::Update(void)
{

}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CAlterPole::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CAlterPole::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_pGoldBone = nullptr;			// ���̍��̏��
	m_fBoneRot = 0.0f;				// ���̍��ւ̌���
	m_bEmpty = true;				// �󔒏�
	m_bArrival = false;				// ������
}

//=======================================
// ��������
//=======================================
CAlterPole* CAlterPole::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAlterPole* pPole = nullptr;		// �C���X�^���X�𐶐�

	if (pPole == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �u���b�N�𐶐�����
		pPole = new CAlterPole;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pPole != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pPole->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pPole->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �Β��̃|�C���^��Ԃ�
	return pPole;
}

//=======================================
// ���̍��̐ݒ菈��
//=======================================
void CAlterPole::SetGoldBone(CGoldBone* pBone)
{
	// ���̍����擾����
	m_pGoldBone = pBone;
}

//=======================================
// ���̍��̎擾����
//=======================================
CGoldBone* CAlterPole::GetGoldBone(void) const
{
	// ���̍���Ԃ�
	return m_pGoldBone;
}

//=======================================
// ���̍��ւ̌����̐ݒ菈��
//=======================================
void CAlterPole::SetBoneRot(const float fRot)
{
	// ���̍��ւ̌�����ݒ肷��
	m_fBoneRot = fRot;
}

//=======================================
// ���̍��ւ̌����̎擾����
//=======================================
float CAlterPole::GetBoneRot(void) const
{
	// ���̍��ւ̌�����Ԃ�
	return m_fBoneRot;
}

//=======================================
// ���̍��ւ̋����̐ݒ菈��
//=======================================
void CAlterPole::SetBoneDist(const float fDist)
{
	// ���̍��ւ̋�����ݒ肷��
	m_fBoneDist = fDist;
}

//=======================================
// ���̍��ւ̋����̎擾����
//=======================================
float CAlterPole::GetBoneDist(void) const
{
	// ���̍��ւ̋�����Ԃ�
	return m_fBoneDist;
}

//=======================================
// �󔒏󋵂̐ݒ菈��
//=======================================
void CAlterPole::SetEnableEmpty(const bool bEmpty)
{
	// �󔒏󋵂�ݒ肷��
	m_bEmpty = bEmpty;
}

//=======================================
// �󔒏󋵂̎擾����
//=======================================
bool CAlterPole::IsEmpty(void) const
{
	// �󔒏󋵂�Ԃ�
	return m_bEmpty;
}

//=======================================
// �����󋵂̐ݒ菈��
//=======================================
void CAlterPole::SetEnableArrival(const bool bArv)
{
	// �����󋵂�ݒ肷��
	m_bArrival = bArv;
}

//=======================================
// �����󋵂̎擾����
//=======================================
bool CAlterPole::IsArrival(void) const
{
	// �����󋵂�Ԃ�
	return m_bArrival;
}

//=======================================
// �`���[�W����
//=======================================
void CAlterPole::Charge(void)
{
	// ���̍��ւ̌��������Z����
	m_fBoneRot += BONE_ROT_MOVE;

	// �����̐��K��
	useful::RotNormalize(&m_fBoneRot);

	// �������k�߂�
	useful::FrameCorrect(0.0f, &m_fBoneDist, SUB_DISTANCE);
}

//=======================================
// ���̍��̈ʒu�ݒ菈��
//=======================================
void CAlterPole::GoldBonePosSet(const D3DXVECTOR3& pos)
{
	// ���̍��̈ʒu���擾
	D3DXVECTOR3 posBone = m_pGoldBone->GetPos();

	// �ʒu��ݒ肷��
	posBone.x = pos.x + sinf(m_fBoneRot) * m_fBoneDist;
	posBone.z = pos.z + cosf(m_fBoneRot) * m_fBoneDist;

	// �ʒu��K�p
	m_pGoldBone->SetPos(posBone);
}