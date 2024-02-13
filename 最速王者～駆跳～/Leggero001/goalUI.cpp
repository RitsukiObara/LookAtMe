//============================================
//
// �S�[��UI����[goalUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "goalUI.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define GOALUI_SHIFT_POS_X			(-1200.0f)							// �S�[��UI�̂��炷�ʒu(X��)
#define GOALUI_SHIFT_POS_Y			(50.0f)								// �S�[��UI�̂��炷�ʒu(Y��)
#define GOALUI_SHIFT_POS_Z			(-150.0f)							// �S�[��UI�̂��炷�ʒu(Z��)
#define GOALUI_INIT_ROT				(D3DXVECTOR3(0.0f, 0.8f, 0.0f))		// �S�[��UI�̌���
#define GOALUI_INIT_MOVE			(20.0f)								// �S�[��UI�̏����ړ���
#define GOALUI_ROT_MOVE				(0.02f)								// �S�[��UI�̌����̈ړ��ʂ̉��Z��
#define GOALUI_BRAKE_ROT_MAGNI		(0.15f)								// �S�[��UI�̌����̃u���[�L�{��
#define GOALUI_STOP_CHANGE_MOVE		(0.005f)							// �S�[��UI�̒�~��Ԃ���ω�����ړ���
#define GOALUI_CLEAR_COUNT			(30)								// �S�[��UI�̏�����ԂɂȂ�J�E���g��
#define GOALUI_CLEAR_POS			(1000.0f)							// �S�[��UI�̏������肪�ʂ�ʒu
#define GOALUI_CLEAR_MOVE			(30.0f)								// �S�[��UI�̏������̑��x

//============================
// �R���X�g���N�^
//============================
CGoalUI::CGoalUI() : CModel(CObject::TYPE_GOALUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̈ړ���
	m_state = STATE_APPEAR;						// ���
	m_nStateCount = 0;							// ��ԃJ�E���g
}

//============================
// �f�X�g���N�^
//============================
CGoalUI::~CGoalUI()
{

}

//============================
// ����������
//============================
HRESULT CGoalUI::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̈ړ���
	m_state = STATE_APPEAR;						// ���
	m_nStateCount = 0;							// ��ԃJ�E���g

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CGoalUI::Uninit(void)
{
	// �I��
	CModel::Uninit();
}

//============================
// �X�V����
//============================
void CGoalUI::Update(void)
{
	switch (m_state)
	{
	case STATE_APPEAR:			// �o�����

		// �ړ�����
		Move();

		// ��]����
		Cycle();

		// �o����ԕω�����
		AppearChange();

		break;

	case STATE_STOP:			// ��~���

		// �u���[�L����
		Brake();

		// ��~��ԕω�����
		StopChange();

		if (m_nStateCount >= GOALUI_CLEAR_COUNT)
		{ // ��ԃJ�E���g����萔�ȏ�ɂȂ����ꍇ

			// ��ԃJ�E���g�� 0 �ɂ���
			m_nStateCount = 0;

			// �ړI�̈ʒu��ݒ肷��
			m_posDest.x = GetPos().x + GOALUI_CLEAR_POS;

			// �ړ��ʂ�ݒ肷��
			m_move.x = GOALUI_CLEAR_MOVE;

			// ������Ԃɂ���
			m_state = STATE_CLEAR;
		}

		break;

	case STATE_CLEAR:			// �������

		// �ړ�����
		Move();

		if (Clear() == true)
		{ // ���������� true �������ꍇ

			// �I������
			Uninit();

			// �I����Ԃɂ���
			CGame::SetState(CGame::STATE_FINISH);

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;
	}
}

//============================
// �`�揈��
//============================
void CGoalUI::Draw(void)
{
	// ��ʃN���A(Z�o�b�t�@�̃N���A)
	CManager::GetRenderer()->GetDevice()->Clear(0,
		NULL,
		D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`�揈��
	CModel::Draw();
}

//============================
// ���̐ݒ菈��
//============================
void CGoalUI::SetData(const D3DXVECTOR3& pos)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu

	// �����ʒu�ݒ菈��
	InitPos = InitPosCalc(pos);

	// �S�Ă̒l������������
	m_posDest = D3DXVECTOR3(pos.x, pos.y, pos.z);		// �ڕW�̈ʒu
	m_move = D3DXVECTOR3(GOALUI_INIT_MOVE, 0.0f, 0.0f);		// �ړ���
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �����̈ړ���
	m_state = STATE_APPEAR;									// ���
	m_nStateCount = 0;										// ��ԃJ�E���g

	// ���̐ݒ菈��
	SetPos(InitPos);						// �ʒu�̐ݒ�
	SetPosOld(InitPos);						// �O��̈ʒu�̐ݒ�
	SetRot(GOALUI_INIT_ROT);				// �����̐ݒ�
	SetScale(NONE_SCALE);					// �g�嗦�̐ݒ�
	SetFileData(CXFile::TYPE_GOALUI);		// X�t�@�C���̃f�[�^�̐ݒ菈��
}

//============================
//��������
//============================
CGoalUI* CGoalUI::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CGoalUI* pGoalUI = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pGoalUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pGoalUI = new CGoalUI;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pGoalUI != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pGoalUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�S�[��UI�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pGoalUI->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pGoalUI;
}

//============================
// �����ʒu�ݒ菈��
//============================
D3DXVECTOR3 CGoalUI::InitPosCalc(const D3DXVECTOR3& pos)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 InitPos = pos;		// �ʒu���擾����

	// �ʒu�����炷
	InitPos.x = pos.x + GOALUI_SHIFT_POS_X;			// X���W
	InitPos.y = pos.y + GOALUI_SHIFT_POS_Y;			// Y���W
	InitPos.z = pos.z + GOALUI_SHIFT_POS_Z;			// Z���W

	// �����ʒu��Ԃ�
	return InitPos;
}

//============================
// �ړ�����
//============================
void CGoalUI::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();			// �ʒu�̎擾

	// �ʒu���ړ�����
	pos += m_move;

	// �ʒu���X�V����
	SetPos(pos);
}

//============================
// �o����ԕω�����
//============================
void CGoalUI::AppearChange(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	if (pos.x >= m_posDest.x)
	{ // �ʒu�����S��荶�ɍs�����ꍇ

		// �ʒu��ݒ肷��
		pos.x = m_posDest.x;

		// ��]��Ԃɂ���
		m_state = STATE_STOP;
	}

	// �ʒu���X�V����
	SetPos(pos);
}

//============================
// ��]����
//============================
void CGoalUI::Cycle(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();			// �������擾����

	// �����̈ړ��ʂ����Z����
	m_rotmove.y += GOALUI_ROT_MOVE;

	// ���������Z����
	rot.y += m_rotmove.y;

	// �����̐��K��
	RotNormalize(&rot.y);

	// �������X�V����
	SetRot(rot);
}

//============================
// �u���[�L����
//============================
void CGoalUI::Brake(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();			// �������擾����

	// �����̈ړ��ʂ����Z����
	m_rotmove.y += (0.0f - m_rotmove.y) * GOALUI_BRAKE_ROT_MAGNI;

	// ���������Z����
	rot.y += m_rotmove.y;

	// �����̐��K��
	RotNormalize(&rot.y);

	// �������X�V����
	SetRot(rot);
}

//============================
// ��~��ԕω�����
//============================
void CGoalUI::StopChange(void)
{
	if (m_rotmove.y <= GOALUI_STOP_CHANGE_MOVE)
	{ // �ړ��ʂ����ȉ��̏ꍇ

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;
	}
}

//============================
// ��������
//============================
bool CGoalUI::Clear(void)
{
	if (GetPos().x >= m_posDest.x)
	{ // �ʒu�����ʒu�𒴂����ꍇ

		// true ��Ԃ�
		return true;
	}
	else
	{ // ��L�ȊO

		// false ��Ԃ�
		return false;
	}
}