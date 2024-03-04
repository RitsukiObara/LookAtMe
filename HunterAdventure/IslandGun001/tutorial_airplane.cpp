//===========================================
//
// �`���[�g���A����s�@�̃��C������[tutorial_airplane.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "tutorial_airplane.h"
#include "useful.h"

#include "propeller.h"
#include "tutorial.h"

//-------------------------------------------
// �萔��`
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Airplane.x";		// ���f���̃p�X��
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 2500.0f);			// ��s�@�̏����ʒu
	const D3DXVECTOR3 INIT_ROT = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);	// ��s�@�̏�������
	const float MOVE_WIDTH = 50.0f;			// �ړ��ʂ̕�
	const float MOVE_HEIGHT = 8.0f;			// �ړ��ʂ̍���
	const float MOVE_ROT_MOVE = 0.004f;		// �ړ����̌����̈ړ���
}

//==============================
// �R���X�g���N�^
//==============================
CTutorialAirplane::CTutorialAirplane() : CModel(CObject::TYPE_AIRPLANE, CObject::PRIORITY_BLOCK)
{
	// �S�Ă̒l���N���A����
	m_pPropeller = nullptr;			// �v���y���̏��
	m_state = STATE_STOP;			// ���
}

//==============================
// �f�X�g���N�^
//==============================
CTutorialAirplane::~CTutorialAirplane()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CTutorialAirplane::Init(void)
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
void CTutorialAirplane::Uninit(void)
{
	if (m_pPropeller != nullptr)
	{ // �v���y���� NULL ����Ȃ��ꍇ

		// �I������
		m_pPropeller->Uninit();
		m_pPropeller = nullptr;
	}

	// �I������
	CModel::Uninit();

	// ��s�@��NULL�ɂ���
	CTutorial::DeleteAirplane();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CTutorialAirplane::Update(void)
{
	switch (m_state)
	{
	case CTutorialAirplane::STATE_STOP:


		break;

	case CTutorialAirplane::STATE_FLY:

		// ��s����
		Fly();

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	if (m_pPropeller != nullptr)
	{ // �v���y���� NULL ����Ȃ��ꍇ

		// �X�V����
		m_pPropeller->Update();
	}
}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CTutorialAirplane::Draw(void)
{
	// �`�揈��
	CModel::Draw();

	if (m_pPropeller != nullptr)
	{ // �v���y���̏�� NULL ����Ȃ��ꍇ

		// �v���y���̕`�揈��
		m_pPropeller->Draw();
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CTutorialAirplane::SetData(void)
{
	// ���̐ݒ菈��
	SetPos(INIT_POS);					// �ʒu
	SetPosOld(INIT_POS);				// �O��̈ʒu
	SetRot(INIT_ROT);					// ����
	SetScale(NONE_SCALE);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f�����

	// �S�Ă̒l��ݒ肷��
	m_pPropeller = CPropeller::Create(GetMatrixPoint());		// �v���y���̏��
	m_state = STATE_STOP;				// ���
}

//=======================================
// ��������
//=======================================
CTutorialAirplane* CTutorialAirplane::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CTutorialAirplane* pAirplane = nullptr;	// �C���X�^���X�𐶐�

	if (pAirplane == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pAirplane = new CTutorialAirplane;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pAirplane != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pAirplane->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pAirplane->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ��s�@�̃|�C���^��Ԃ�
	return pAirplane;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CTutorialAirplane::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CTutorialAirplane::STATE CTutorialAirplane::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// ��s����
//=======================================
void CTutorialAirplane::Fly(void)
{
	// �ʒu�ƌ������擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// �ړ�����
	pos.x += MOVE_WIDTH;
	pos.y += MOVE_HEIGHT;

	// �ǂ�ǂ�X���Ă���
	rot.x += MOVE_ROT_MOVE;

	// �����̐��K��
	useful::RotNormalize(&rot.x);

	// �ʒu�ƌ�����K�p
	SetPos(pos);
	SetRot(rot);
}