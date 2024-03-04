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
#include "file.h"
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
	const float CHASE_DISTANCE = 2000.0f;	// �ǐՏ�Ԃɓ��鋗��
}

//================================
// �R���X�g���N�^
//================================
CTordle::CTordle() : CEnemy()
{
	// �S�Ă̒l���N���A����
	m_action = ACTION_WALK;				// �s��
	m_rotDest = NONE_D3DXVECTOR3;		// �ڕW�̌���
	m_fMoveX = 0.0f;					// �ړ���(X��)
	m_fMoveZ = 0.0f;					// �ړ���(Z��)
	m_walking.pPosDest = nullptr;		// �p�j�o�H�֌W�̏��
	m_walking.nNowPosIdx = 0;			// ���݂̈ʒu�̔ԍ�
	m_walking.nNumPos = 0;				// �ʒu�̑���
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
	if (m_walking.pPosDest != nullptr)
	{ // �ړI�̈ʒu�� NULL ����Ȃ��ꍇ

		// �ړI�̈ʒu�� NULL �ɂ���
		delete[] m_walking.pPosDest;
		m_walking.pPosDest = nullptr;
	}

	// �I������
	CEnemy::Uninit();
}

//================================
// �X�V����
//================================
void CTordle::Update(void)
{
	if (CGame::GetState() == CGame::STATE_PLAY)
	{ // �v���C��ԈȊO

		// �O��̈ʒu��ݒ肷��
		SetPosOld(GetPos());

		switch (GetState())
		{
		case STATE_NONE:

			switch (m_action)
			{
			case CTordle::ACTION_WALK:

				// �p�j����
				Walking();

				// ��������
				Arrival();

				break;

			case CTordle::ACTION_CHASE:

				// �ǐՏ���
				Chase();

				// �ړ�����
				Move();

				break;

			default:

				// ��~
				assert(false);

				break;
			}

			// ��Ԕ��f����
			StateJudge();

			// �����̈ړ�����
			RotMove();

			break;

		case STATE_DAMAGE:

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
	}
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
	m_action = ACTION_WALK;				// �s��
	m_rotDest = NONE_D3DXVECTOR3;		// �ڕW�̌���
	m_fMoveX = 0.0f;					// �ړ���(X��)
	m_fMoveZ = 0.0f;					// �ړ���(Z��)
	SetRoute();							// �p�j�o�H
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
// �o�H�̐ݒ菈��
//===========================================
void CTordle::SetRoute(void)
{
	// �p�j�o�H��I��
	int nRoute = rand() % CManager::Get()->GetFile()->GetEnemyRouteNum();

	// �ʒu�̑������擾
	m_walking.nNumPos = CManager::Get()->GetFile()->GetEnemyRouteNumPos(nRoute);

	// �o�H�̃��������m�ۂ���
	m_walking.pPosDest = new D3DXVECTOR3[m_walking.nNumPos];

	for (int nCnt = 0; nCnt < m_walking.nNumPos; nCnt++)
	{
		// �p�j�o�H���擾
		m_walking.pPosDest[nCnt] = CManager::Get()->GetFile()->GetEnemyRoute(nRoute, nCnt);
	}
}

//===========================================
// �p�j����
//===========================================
void CTordle::Walking(void)
{
	D3DXVECTOR3 pos = GetPos();

	// ������ݒ肷��
	m_rotDest.y = atan2f(m_walking.pPosDest[m_walking.nNowPosIdx].x - pos.x, m_walking.pPosDest[m_walking.nNowPosIdx].z - pos.z);

	// �ړ��ʂ�ݒ肷��
	m_fMoveX = fabsf(sinf(m_rotDest.y) * SPEED);
	m_fMoveZ = fabsf(cosf(m_rotDest.y) * SPEED);
}

//===========================================
// ��������
//===========================================
void CTordle::Arrival(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	if (useful::FrameCorrect(m_walking.pPosDest[m_walking.nNowPosIdx].x, &pos.x, m_fMoveX) == true ||
		useful::FrameCorrect(m_walking.pPosDest[m_walking.nNowPosIdx].z, &pos.z, m_fMoveZ) == true)
	{ // �ړI�̈ʒu�ɒ������ꍇ

		// �ʒu��␳����
		pos.x = m_walking.pPosDest[m_walking.nNowPosIdx].x;
		pos.z = m_walking.pPosDest[m_walking.nNowPosIdx].z;

		// �ԍ���i�߂�
		m_walking.nNowPosIdx = (m_walking.nNowPosIdx + 1) % m_walking.nNumPos;
	}

	// �ʒu��K�p
	SetPos(pos);
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

//===========================================
// ��Ԕ��f����
//===========================================
void CTordle::StateJudge(void)
{
	if (CGame::GetPlayer() != nullptr)
	{ // �v���C���[�����݂����ꍇ

		D3DXVECTOR3 pos = GetPos();			// �ʒu
		D3DXVECTOR3 posPlayer = CGame::GetPlayer()->GetPos();		// �v���C���[�̈ʒu���擾����
		float fDist = sqrtf((posPlayer.x - pos.x) * (posPlayer.x - pos.x) + (posPlayer.z - pos.z) * (posPlayer.z - pos.z));		// ����

		if (fDist <= CHASE_DISTANCE)
		{ // ���������ȉ��̏ꍇ

			// �ǐՏ��
			m_action = ACTION_CHASE;
		}
		else
		{ // ��L�ȊO

			// �p�j���
			m_action = ACTION_WALK;
		}
	}
}