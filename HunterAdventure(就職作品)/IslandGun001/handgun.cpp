//===========================================
//
// ���e�̃��C������[handgun.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "handgun.h"
#include "useful.h"

// �������O���
namespace
{
	const char* MODEL = "data\\MODEL\\Pistol.x";		// ���f���̖��O
	const float ADD_ROT_MOVE = 0.04f;					// �����̈ړ��ʂ̉��Z��
	const float MAX_ROT_MOVE = 0.4f;					// �����̈ړ��ʂ̍ő吔
	const float RELOAD_COUNT = 50;						// �����[�h�J�E���g
	const float DEST_LENGTH = 200.0f;					// �ړI�̒���
	const float RELOAD_CORRECT = 5.0f;					// �����[�h���̕␳�{��
	const float RETURN_CORRECT = 16.0f;					// �߂��Ԃ̕␳�{��
	const float RETURN_ROT_MOVE = 0.9f;					// �߂��Ԃ̌����̈ړ���
}

//==============================
// �R���X�g���N�^
//==============================
CHandgun::CHandgun() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_PLAYER)
{
	// �S�Ă̒l���N���A����
	m_posDest = NONE_D3DXVECTOR3;	// �ړI�̈ʒu
	m_posInit = NONE_D3DXVECTOR3;	// �����ʒu
	m_rotInit = NONE_D3DXVECTOR3;	// ��������
	m_pMtxParent = nullptr;			// �e�̃}�g���b�N�X
	m_state = STATE_NONE;			// ���
	m_nReloadCount = 0;				// �����[�h�J�E���g
	m_fRotMove = 0.0f;				// �����̈ړ���
	m_bDisp = true;					// �`���
}

//==============================
// �f�X�g���N�^
//==============================
CHandgun::~CHandgun()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CHandgun::Init(void)
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
void CHandgun::Uninit(void)
{
	// �I������
	CModel::Uninit();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CHandgun::Update(void)
{
	switch (m_state)
	{
	case CHandgun::STATE_NONE:

		// ���ɂȂ�

		break;

	case CHandgun::STATE_RELOAD:

		// �����̈ړ�����
		RotMove();

		// �ʒu�̈ړ�����
		PosMove();

		// �����[�h�J�E���g�����Z����
		m_nReloadCount++;

		if (m_nReloadCount >= RELOAD_COUNT)
		{ // �����[�h�J�E���g�����ȏ�̏ꍇ

			// �����[�h�J�E���g������������
			m_nReloadCount = 0;

			// �߂��Ԃɂ���
			m_state = STATE_RETURN;

			// �����̈ړ��ʂ�����������
			m_fRotMove = 0.0f;
		}

		break;

	case CHandgun::STATE_RETURN:

		// �߂���
		ReturnMove();

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CHandgun::Draw(void)
{
	if (m_bDisp == true)
	{ // �`��󋵂� true �̏ꍇ

		// �`�揈��
		CModel::DrawMatrix(*m_pMtxParent);
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CHandgun::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot, D3DXMATRIX* mtx)
{
	// ���̐ݒ菈��
	SetPos(pos);						// �ʒu
	SetPosOld(pos);						// �O��̈ʒu
	SetRot(rot);						// ����
	SetScale(NONE_SCALE);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f�����

	// �S�Ă̒l���N���A����
	m_posInit = pos;		// �����ʒu
	m_rotInit = rot;		// ��������
	m_pMtxParent = mtx;		// �e�̃}�g���b�N�X
	m_state = STATE_NONE;	// ���
	m_fRotMove = 0.0f;		// �����̈ړ���
	m_bDisp = true;			// �`���

	// �ړI�̈ʒu��ݒ肷��
	m_posDest = D3DXVECTOR3(m_posInit.x + sinf(m_rotInit.y) * -DEST_LENGTH, m_posInit.y, m_posInit.z + DEST_LENGTH);
}

//=======================================
// ��������
//=======================================
CHandgun* CHandgun::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot, D3DXMATRIX* mtx)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CHandgun* pGun = nullptr;	// �C���X�^���X�𐶐�

	if (pGun == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pGun = new CHandgun;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pGun != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pGun->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pGun->SetData(pos, rot, mtx);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���e�̃|�C���^��Ԃ�
	return pGun;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CHandgun::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CHandgun::STATE CHandgun::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// �`��󋵂̐ݒ菈��
//=======================================
void CHandgun::SetEnableDisp(const bool bDisp)
{
	// �`��󋵂�ݒ肷��
	m_bDisp = bDisp;
}

//=======================================
// �`��󋵂̎擾����
//=======================================
bool CHandgun::IsDisp(void) const
{
	// �`��󋵂�ݒ肷��
	return m_bDisp;
}

//=======================================
// �ʒu�̈ړ�����
//=======================================
void CHandgun::PosMove(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �ʒu�̋ϓ��ȕ␳����
	useful::FrameCorrect(m_posDest.x, &pos.x, RELOAD_CORRECT);
	useful::FrameCorrect(m_posDest.y, &pos.y, RELOAD_CORRECT);
	useful::FrameCorrect(m_posDest.z, &pos.z, RELOAD_CORRECT);
	
	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// �����̈ړ�����
//=======================================
void CHandgun::RotMove(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// ���������Z����
	rot.y += m_fRotMove;

	// �����̐��K������
	useful::RotNormalize(&rot.y);

	// ������K�p����
	SetRot(rot);

	if (m_fRotMove < MAX_ROT_MOVE)
	{ // �����̈ړ��ʂ��ő�l�����̏ꍇ

		// �����̈ړ��ʂ����Z����
		m_fRotMove += ADD_ROT_MOVE;
	}
}

//=======================================
// �߂��Ԃ̈ړ�����
//=======================================
void CHandgun::ReturnMove(void)
{
	// �ʒu�ƌ������擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// ���������Z����
	rot.y += RETURN_ROT_MOVE;

	// �����̐��K������
	useful::RotNormalize(&rot.y);

	// �ʒu�̋ϓ��ȕ␳����
	if (useful::FrameCorrect(m_posInit.x, &pos.x, RETURN_CORRECT) == true ||
		useful::FrameCorrect(m_posInit.y, &pos.y, RETURN_CORRECT) == true ||
		useful::FrameCorrect(m_posInit.z, &pos.z, RETURN_CORRECT) == true)
	{ // �␳���I������ꍇ

		// �����ʒu�ɒ���
		pos = m_posInit;

		// ���������ɒ���
		rot = m_rotInit;

		// �ʏ��Ԃɂ���
		m_state = STATE_NONE;
	}

	// �ʒu�ƌ�����K�p����
	SetPos(pos);
	SetRot(rot);
}