//============================================================
//
// �C���J������ [iwakari.cpp]
// Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "iwakari.h"
#include "motion.h"
#include "manager.h"
#include "useful.h"

#include "iwakari_shell.h"

#include "game.h"
#include "player.h"

//------------------------------------------------------------
// �������O���
//------------------------------------------------------------
namespace
{
	const float SPEED = 3.0f;				// �ړ���
	const float MOVE_LENGTH = 900.0f;		// �ǐՂ��鋗��
	const float KNOCKBACK_HEIGHT = 10.0f;	// �m�b�N�o�b�N�l�̍���
}

//================================
// �R���X�g���N�^
//================================
CIwakari::CIwakari() : CEnemy()
{
	// �S�Ă̒l���N���A����
	m_pShell = nullptr;				// �k�̏��
	m_rotDest = NONE_D3DXVECTOR3;	// �ڕW�̌���
	m_action = ACTION_NONE;			// ���
	m_fMoveX = 0.0f;				// �ړ���(X��)
	m_fMoveZ = 0.0f;				// �ړ���(Z��)
}

//================================
// �f�X�g���N�^
//================================
CIwakari::~CIwakari()
{

}

//================================
// ����������
//================================
HRESULT CIwakari::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���f���̑�����ݒ�
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_IWAKARI));

	// �f�[�^�̐ݒ菈��
	CCharacter::SetData();

	if (GetMotion() == nullptr)
	{ // ���[�V������ NULL �������ꍇ

		// ���[�V�����̐�������
		CreateMotion();
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	if (GetMotion() != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ���[�V�����̏����擾����
		GetMotion()->SetInfo(CMotion::STYLE_IWAKARI, GetHierarchy(), GetNumModel());
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
void CIwakari::Uninit(void)
{
	if (m_pShell != nullptr)
	{ // �k�̏�� NULL ����Ȃ��ꍇ

		// �k�̏I������
		m_pShell->Uninit();
		m_pShell = nullptr;
	}

	// �I������
	CEnemy::Uninit();
}

//================================
// �X�V����
//================================
void CIwakari::Update(void)
{
	// �O��̈ʒu��ݒ肷��
	SetPosOld(GetPos());

	// �ǐՏ���
	Chase();

	switch (m_action)
	{
	case CIwakari::ACTION_NONE:		// �ʏ���



		break;

	case CIwakari::ACTION_MOVE:		// �ړ����

		if (GetState() == STATE_NONE)
		{ // �ʏ��Ԃ̏ꍇ

			// �����̈ړ�����
			RotMove();
		}

		// �ړ�����
		Move();

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// �X�V����
	CEnemy::Update();

	if (m_pShell != nullptr)
	{ // �k�� NULL ����Ȃ��ꍇ

		// �k�̈ʒu�ƌ�����ݒ肷��
		m_pShell->SetPos(GetPos());
		m_pShell->SetRot(GetRot());
	}
}

//================================
// �`�揈��
//================================
void CIwakari::Draw(void)
{
	// �`�揈��
	CEnemy::Draw();

	if (m_pShell != nullptr)
	{ // �k�� NULL ����Ȃ��ꍇ

		// �k�̕`�揈��
		m_pShell->Draw();
	}
}

//================================
// ���̐ݒ菈��
//================================
void CIwakari::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ���̐ݒ菈��
	CEnemy::SetData(pos, rot, type);

	// �S�Ă̒l��ݒ肷��
	m_pShell = CIwakariShell::Create(pos, rot);		// �k�̏��
	m_rotDest = NONE_D3DXVECTOR3;	// �ڕW�̌���
	m_action = ACTION_NONE;			// ���
	m_fMoveX = 0.0f;				// �ړ���(X��)
	m_fMoveZ = 0.0f;				// �ړ���(Z��)

	// ���[�V�����̃��Z�b�g����
	GetMotion()->ResetMotion(MOTIONTYPE_NEUTRAL);
}

//===========================================
// �q�b�g����
//===========================================
void CIwakari::Hit(const int nDamage, const float fKnockback)
{
	// �̗͂̕ϐ���錾����
	int nLife;

	if (m_pShell != nullptr)
	{ // �k�����݂���ꍇ

		// �k�̗̑͂��擾����
		nLife = m_pShell->GetLife();

		// �k�̗̑͂����炷
		nLife -= nDamage;

		// �k�̗̑͂�K�p����
		m_pShell->SetLife(nLife);

		if (nLife <= 0)
		{ // �k�̗̑͂��Ȃ��Ȃ����ꍇ

			// �k����������
			m_pShell->Uninit();
			m_pShell = nullptr;
		}
	}
	else
	{ // ��L�ȊO

		// �m�b�N�o�b�N������
		m_fMoveX = sinf(GetRot().y + (D3DX_PI)) * fKnockback;
		SetGravity(KNOCKBACK_HEIGHT);
		m_fMoveZ = cosf(GetRot().y + (D3DX_PI)) * fKnockback;

		// �̗͂��擾����
		nLife = GetLife();

		// �̗͂����炷
		nLife -= nDamage;

		// �̗͂�K�p����
		SetLife(nLife);

		// �_���[�W��Ԃɂ���
		SetState(STATE_DAMAGE);
	}

	// ���S����
	Death();
}

//===========================================
// �ǐՏ���
//===========================================
void CIwakari::Chase(void)
{
	D3DXVECTOR3 pos;							// �G�̈ʒu
	D3DXVECTOR3 posPlayer;						// �v���C���[�̈ʒu
	int motiontype = GetMotion()->GetType();	// ���[�V�����̎��
	float fRot = 0.0f;							// ����
	float fLength = 0.0f;						// ����

	if (CGame::GetPlayer() != nullptr)
	{ // �v���C���[�����݂����ꍇ

		pos = GetPos();									// �ʒu���擾����
		posPlayer = CGame::GetPlayer()->GetPos();		// �v���C���[�̈ʒu���擾����

		// ������ݒ肷��
		fLength = sqrtf((posPlayer.x - pos.x) * (posPlayer.x - pos.x) + (posPlayer.z - pos.z) * (posPlayer.z - pos.z));

		if (fLength <= MOVE_LENGTH)
		{ // ���������ȉ��ɂȂ����ꍇ

			// �������Z�o����
			fRot = atan2f((posPlayer.x - pos.x), (posPlayer.z - pos.z));

			// �ړ��ʂ�ݒ肷��
			m_fMoveX = sinf(fRot) * SPEED;
			m_fMoveZ = cosf(fRot) * SPEED;

			// �ڕW�̌�����ݒ肷��
			m_rotDest.y = fRot;

			// �ړ���Ԃɂ���
			m_action = ACTION_MOVE;

			if (motiontype != MOTIONTYPE_MOVE)
			{ // �ړ����[�V�����ȊO�̏ꍇ

				// �ړ����[�V�����ɂ���
				GetMotion()->Set(MOTIONTYPE_MOVE);
			}
		}
		else
		{ // ��L�ȊO

			// �ʏ��Ԃɂ���
			m_action = ACTION_NONE;

			if (motiontype != MOTIONTYPE_NEUTRAL)
			{ // �ҋ@���[�V�����ȊO�̏ꍇ

				// �ҋ@���[�V�����ɂ���
				GetMotion()->Set(MOTIONTYPE_NEUTRAL);
			}
		}
	}
	else
	{ // ��L�ȊO

		// �ʏ��Ԃɂ���
		m_action = ACTION_NONE;

		if (motiontype != MOTIONTYPE_NEUTRAL)
		{ // �ҋ@���[�V�����ȊO�̏ꍇ

			// �ҋ@���[�V�����ɂ���
			GetMotion()->Set(MOTIONTYPE_NEUTRAL);
		}
	}
}

//===========================================
// �ړ�����
//===========================================
void CIwakari::Move(void)
{
	// �ʒu�Əd�͂��擾����
	D3DXVECTOR3 pos = GetPos();

	// �ړ�����
	pos.x += m_fMoveX;
	pos.z += m_fMoveZ;

	// �ʒu�Əd�͂�K�p����
	SetPos(pos);
}

//===========================================
// �����̈ړ�����
//===========================================
void CIwakari::RotMove(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// �����̕␳����
	useful::RotCorrect(m_rotDest.y, &rot.y, 0.1f);

	// ������K�p����
	SetRot(rot);
}