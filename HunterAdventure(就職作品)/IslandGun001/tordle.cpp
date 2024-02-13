//============================================================
//
// �^�[�h������ [tordle.cpp]
// Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "tordle.h"
#include "motion.h"
#include "manager.h"

#include "game.h"
#include "player.h"
#include "useful.h"

//------------------------------------------------------------
// �������O���
//------------------------------------------------------------
namespace
{
	const float SPEED = 5.0f;				// �ړ���
	const float KNOCKBACK_HEIGHT = 5.0f;	// �m�b�N�o�b�N�l�̍���
}

//================================
// �R���X�g���N�^
//================================
CTordle::CTordle() : CEnemy()
{
	// �S�Ă̒l���N���A����
	m_rotDest = NONE_D3DXVECTOR3;		// �ڕW�̌���
	m_fMoveX = 0.0f;					// �ړ���(X��)
	m_fMoveZ = 0.0f;					// �ړ���(Z��)
}

//================================
// �f�X�g���N�^
//================================
CTordle::~CTordle()
{

}

//================================
// ����������
//================================
HRESULT CTordle::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���f���̑�����ݒ�
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_TORDLE));

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
		GetMotion()->SetInfo(CMotion::STYLE_TORDLE, GetHierarchy(), GetNumModel());
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
void CTordle::Uninit(void)
{
	// �I������
	CEnemy::Uninit();
}

//================================
// �X�V����
//================================
void CTordle::Update(void)
{
	// �O��̈ʒu��ݒ肷��
	SetPosOld(GetPos());

	if (GetState() == STATE_NONE)
	{ // �ʏ��Ԃ̏ꍇ

		// �ǐՏ���
		Chase();

		// �����̈ړ�����
		RotMove();
	}

	// �ړ�����
	Move();

	// �X�V����
	CEnemy::Update();
}

//================================
// �`�揈��
//================================
void CTordle::Draw(void)
{
	// �`�揈��
	CEnemy::Draw();
}

//================================
// ���̐ݒ菈��
//================================
void CTordle::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ���̐ݒ菈��
	CEnemy::SetData(pos, rot, type);

	// ���[�V�����̃��Z�b�g����
	GetMotion()->ResetMotion(MOTIONTYPE_MOVE);

	// �S�Ă̒l��ݒ肷��
	m_rotDest = NONE_D3DXVECTOR3;		// �ڕW�̌���
	m_fMoveX = 0.0f;					// �ړ���(X��)
	m_fMoveZ = 0.0f;					// �ړ���(Z��)
}

//===========================================
// �q�b�g����
//===========================================
void CTordle::Hit(const int nDamage, const float fKnockback)
{
	// �m�b�N�o�b�N������
	m_fMoveX = sinf(GetRot().y + D3DX_PI) * fKnockback;
	SetGravity(KNOCKBACK_HEIGHT);
	m_fMoveZ = cosf(GetRot().y + D3DX_PI) * fKnockback;

	// �̗͂��擾����
	int nLife = GetLife();

	// �̗͂����炷
	nLife -= nDamage;

	// �̗͂�K�p����
	SetLife(nLife);

	// �_���[�W��Ԃɂ���
	SetState(STATE_DAMAGE);

	// ���S����
	Death();
}

//===========================================
// �ǐՏ���
//===========================================
void CTordle::Chase(void)
{
	D3DXVECTOR3 pos;						// �G�̈ʒu
	D3DXVECTOR3 posPlayer;					// �v���C���[�̈ʒu
	float fRot = 0.0f;						// ����

	if (CGame::GetPlayer() != nullptr)
	{ // �v���C���[�����݂����ꍇ

		pos = GetPos();									// �ʒu���擾����
		posPlayer = CGame::GetPlayer()->GetPos();		// �v���C���[�̈ʒu���擾����

		// �������Z�o����
		fRot = atan2f((posPlayer.x - pos.x), (posPlayer.z - pos.z));

		// �ړ��ʂ�ݒ肷��
		m_fMoveX = sinf(fRot) * SPEED;
		m_fMoveZ = cosf(fRot) * SPEED;

		// �ڕW�̌�����ݒ肷��
		m_rotDest.y = fRot;
	}
}

//===========================================
// �ړ�����
//===========================================
void CTordle::Move(void)
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
void CTordle::RotMove(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// �����̕␳����
	useful::RotCorrect(m_rotDest.y, &rot.y, 0.1f);

	// ������K�p����
	SetRot(rot);
}