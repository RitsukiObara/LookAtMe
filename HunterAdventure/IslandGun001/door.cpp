//===========================================
//
// �h�A�̃��C������[door.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "door.h"
#include "useful.h"

#include "tutorial.h"
#include "push_timing.h"
#include "tutorial_airplane.h"

//-------------------------------------------
// �萔��`
//-------------------------------------------
namespace
{
	const char* DOOR_MODEL = "data\\MODEL\\Door.x";						// �h�A���f���̖��O
	const char* FRAME_MODEL = "data\\MODEL\\DoorFrame.x";				// �h�A�t���[�����f���̖��O
	const D3DXVECTOR3 DOOR_OFFSET = D3DXVECTOR3(60.7f, 0.0f, 0.0f);		// �h�A�̃I�t�Z�b�g
	const float BUTTON_SHIFT = 300.0f;			// �{�^���̂��炷����
	const float BUTTON_SIZE = 30.0f;			// �{�^���̃T�C�Y
	const int BUTTON_INTERVAL = 10;				// �{�^���̊Ԋu

	const float OPEN_ROT = -D3DX_PI * 0.5f;		// �J����Ԃ̎��̌���
	const float ROT_MOVE = 0.1f;				// �����̈ړ���
}

//==============================
// �R���X�g���N�^
//==============================
CDoor::CDoor() : CModel(CObject::TYPE_DOOR, CObject::PRIORITY_BLOCK)
{
	// �S�Ă̒l���N���A����
	m_pDoor = nullptr;			// �h�A�̃��f��
	m_pButton = nullptr;		// �{�^���̏��
	m_state = STATE_NONE;		// ���
	m_bDisp = false;			// �{�^���̕\����
}

//==============================
// �f�X�g���N�^
//==============================
CDoor::~CDoor()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CDoor::Init(void)
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
void CDoor::Uninit(void)
{
	if (m_pDoor != nullptr)
	{ // �h�A�� NULL ����Ȃ��ꍇ

		// �h�A�̏I������
		m_pDoor->Uninit();
		m_pDoor = nullptr;
	}

	if (m_pButton != nullptr)
	{ // �{�^���� NULL ����Ȃ��ꍇ

		// �{�^���̏I������
		m_pButton->Uninit();
		m_pButton = nullptr;
	}

	// �I������
	CModel::Uninit();

	// �h�A��NULL������
	CTutorial::DeleteDoor();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CDoor::Update(void)
{
	// �O��̈ʒu��ݒ肷��
	SetPosOld(GetPos());

	switch (m_state)
	{
	case CDoor::STATE_NONE:


		break;

	case CDoor::STATE_OPEN:

		// �J������
		Open();

		// �`��󋵂� false �ɂ���
		m_bDisp = false;

		break;

	case CDoor::STATE_CLOSE:

		// ������
		Close();

		if (CTutorial::GetPlayer() != nullptr &&
			m_pDoor->GetRot().y >= -0.0001f)
		{ // �����قڕ܂����Ƃ�
			
			// ��s�@���s��Ԃɂ���
			CTutorial::GetAirplane()->SetState(CTutorialAirplane::STATE_FLY);
		}

		// �`��󋵂� false �ɂ���
		m_bDisp = false;

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	if (m_pButton != nullptr)
	{ // �{�^���� NULL ����Ȃ��ꍇ

		// �{�^���̍X�V����
		m_pButton->Update();
	}
}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CDoor::Draw(void)
{
	// �`�揈��
	CModel::Draw();

	if (m_pDoor != nullptr)
	{ // �h�A�� NULL ����Ȃ��ꍇ

		// �h�A�̕`�揈��
		m_pDoor->DrawMatrix(GetMatrix());
	}

	if (m_bDisp == true &&
		m_pButton != nullptr)
	{ // �{�^�����\���\�ȏꍇ

		// �{�^���̕`�揈��
		m_pButton->Draw();
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CDoor::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(FRAME_MODEL));	// ���f�����

	// �S�Ă̒l��ݒ肷��
	if (m_pDoor == nullptr)
	{ // �h�A�� NULL ����Ȃ��ꍇ

		// �h�A�̐���
		m_pDoor = CModel::Create(TYPE_NONE, PRIORITY_ENTITY);

		// ���̐ݒ菈��
		m_pDoor->SetPos(DOOR_OFFSET);		// �ʒu
		m_pDoor->SetPosOld(DOOR_OFFSET);	// �O��̈ʒu
		m_pDoor->SetRot(NONE_D3DXVECTOR3);	// ����
		m_pDoor->SetScale(NONE_SCALE);		// �g�嗦
		m_pDoor->SetFileData(CManager::Get()->GetXFile()->Regist(DOOR_MODEL));	// ���f�����
	}

	if (m_pButton == nullptr)
	{ // �{�^���� NULL ����Ȃ��ꍇ

		// �{�^���̐���
		m_pButton = CPushTiming::Create(D3DXVECTOR3(pos.x, pos.y + BUTTON_SHIFT, pos.z), BUTTON_SIZE, CPushTiming::TYPE_PAD_A, BUTTON_INTERVAL);
		m_pButton->CObject::SetType(TYPE_NONE);
	}
	m_state = STATE_NONE;		// ���
	m_bDisp = false;			// �`���
}

//=======================================
// ��������
//=======================================
CDoor* CDoor::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CDoor* pDoor = nullptr;		// �C���X�^���X�𐶐�

	if (pDoor == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pDoor = new CDoor;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pDoor != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pDoor->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pDoor->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �h�A�̃|�C���^��Ԃ�
	return pDoor;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CDoor::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CDoor::STATE CDoor::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// �`��󋵂̐ݒ菈��
//=======================================
void CDoor::SetEnableDisp(const bool bDisp)
{
	// �`��󋵂�ݒ肷��
	m_bDisp = bDisp;
}

//=======================================
// �`��󋵂̎擾����
//=======================================
bool CDoor::IsDisp(void) const
{
	// �`��󋵂�Ԃ�
	return m_bDisp;
}

//=======================================
// �J������
//=======================================
void CDoor::Open(void)
{
	// �������擾
	D3DXVECTOR3 rot = m_pDoor->GetRot();

	// �����̕␳����
	useful::RotCorrect(OPEN_ROT, &rot.y, ROT_MOVE);

	// ������K�p
	m_pDoor->SetRot(rot);
}

//=======================================
// ������
//=======================================
void CDoor::Close(void)
{
	// �������擾
	D3DXVECTOR3 rot = m_pDoor->GetRot();

	// �����̕␳����
	useful::RotCorrect(0.0f, &rot.y, ROT_MOVE);

	// ������K�p
	m_pDoor->SetRot(rot);
}