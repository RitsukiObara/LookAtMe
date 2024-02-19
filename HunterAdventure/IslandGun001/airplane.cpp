//===========================================
//
// ��s�@�̃��C������[airplane.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "airplane.h"
#include "useful.h"

#include "game.h"
#include "player.h"
#include "propeller.h"
#include "camera.h"

//-------------------------------------------
// �萔��`
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Airplane.x";		// ���f���̃p�X��
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, 2000.0f, 30000.0f);		// �ʒu
	const float MOVE_MAGNI = 0.005f;					// �ړ��ʂ̔{��
}

//==============================
// �R���X�g���N�^
//==============================
CAirplane::CAirplane() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pPropeller = nullptr;			// �v���y���̏��
	m_posDest = NONE_D3DXVECTOR3;	// �ړI�̈ʒu
	m_move = NONE_D3DXVECTOR3;		// �ړ���
	m_state = STATE_MOVE;			// ���
}

//==============================
// �f�X�g���N�^
//==============================
CAirplane::~CAirplane()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CAirplane::Init(void)
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
void CAirplane::Uninit(void)
{
	if (m_pPropeller != nullptr)
	{ // �v���y���� NULL ����Ȃ��ꍇ

		// �I������
		m_pPropeller->Uninit();
		m_pPropeller = nullptr;
	}

	// �I������
	CModel::Uninit();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CAirplane::Update(void)
{
	// �ė�����
	Fall();

	if (m_pPropeller != nullptr)
	{ // �v���y���� NULL ����Ȃ��ꍇ

		// �X�V����
		m_pPropeller->Update();
	}
}

//=====================================
// �X�V����(���ێg������)
//=====================================
void CAirplane::Update(CPlayer* pPlayer)
{
	// �ړ�����
	Move(pPlayer);

	if (m_pPropeller != nullptr)
	{ // �v���y���� NULL ����Ȃ��ꍇ

		// �X�V����
		m_pPropeller->Update();
	}
}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CAirplane::Draw(void)
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
void CAirplane::SetData(const D3DXVECTOR3& posDest)
{
	// ���̐ݒ菈��
	SetPos(POS);						// �ʒu
	SetPosOld(POS);						// �O��̈ʒu
	SetScale(NONE_SCALE);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f�����

	// �S�Ă̒l��ݒ肷��
	m_pPropeller = CPropeller::Create(GetMatrixPoint());		// �v���y���̏��
	m_posDest = posDest;				// �ړI�̈ʒu
	m_state = STATE_MOVE;			// ���

	// �ړ��ʂ�ݒ�
	m_move.x = fabsf((posDest.x - POS.x) * MOVE_MAGNI);
	m_move.y = 0.0f;
	m_move.z = fabsf((posDest.z - POS.z) * MOVE_MAGNI);
}

//=======================================
// ��������
//=======================================
CAirplane* CAirplane::Create(const D3DXVECTOR3& posDest)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAirplane* pAirplane = nullptr;	// �C���X�^���X�𐶐�

	if (pAirplane == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pAirplane = new CAirplane;
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
		pAirplane->SetData(posDest);
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
void CAirplane::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CAirplane::STATE CAirplane::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// �ړ�����
//=======================================
void CAirplane::Move(CPlayer* pPlayer)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	if (useful::FrameCorrect(m_posDest.x, &pos.x, m_move.x) == true ||
		useful::FrameCorrect(m_posDest.z, &pos.z, m_move.z) == true)
	{ // �ړI�̈ʒu�ɒ������ꍇ

		// �ړI�̈ʒu�ɕ␳����
		pos.x = m_posDest.x;
		pos.z = m_posDest.z;

		// �v���C���[�o���Ԃɂ���
		CManager::Get()->GetCamera()->SetType(CCamera::TYPE_PLAYERAPPEAR);

		// �J�����̎��_��ݒ肷��
		CManager::Get()->GetCamera()->SetPosV(D3DXVECTOR3(pos.x - 1000.0f, pos.y + 1500.0f, pos.z));

		// �ړ��ʂ̐ݒ菈��
		pPlayer->SetMove(D3DXVECTOR3(0.0f, 30.0f, 10.0f));

		// ��s�@�̊Ǌ��O������
		pPlayer->RemoveAirplane();

		// �ė���Ԃɂ���
		m_state = STATE_FALL;

		// ��ނ��s�@�ɕς���
		SetType(TYPE_AIRPLANE);
	}

	// �ʒu��K�p
	SetPos(pos);
}

//=======================================
// �ė�����
//=======================================
void CAirplane::Fall(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	// �ړ�����
	pos.x -= m_move.x;
	pos.y -= 5.0f;
	pos.z -= m_move.z;

	// �ʒu��K�p
	SetPos(pos);
}