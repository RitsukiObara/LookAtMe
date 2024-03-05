//============================================================
//
// �G���� [enemy.cpp]
// Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "enemy.h"
#include "manager.h"
#include "useful.h"

#include "motion.h"
#include "block.h"
#include "collision.h"
#include "anim_reaction.h"
#include "objectElevation.h"

#include "tordle.h"
#include "iwakari.h"

//------------------------------------------------------------
// �������O���
//------------------------------------------------------------
namespace
{
	const D3DXVECTOR3 COLLSIZE[CEnemy::TYPE_MAX] =		// �����蔻��̃T�C�Y
	{
		D3DXVECTOR3(85.0f,85.0f,85.0f),
		D3DXVECTOR3(80.0f,50.0f,80.0f),
	};
	const int LIFE[CEnemy::TYPE_MAX] =					// �̗�
	{
		30,
		5,
	};
	const D3DXVECTOR3 DEATH_EXPLOSION = D3DXVECTOR3(200.0f, 200.0f, 0.0f);		// ���S���̔���
	const D3DXCOLOR DAMAGE_COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);				// �_���[�W��Ԃ̐F
	const int DAMAGE_HITSTOP = 5;						// �_���[�W���̃q�b�g�X�g�b�v
	const int DEATH_HITSTOP = 10;						// ���S���̃q�b�g�X�g�b�v
	const int DAMAGE_COUNT = 8;							// �_���[�W��Ԃ̃J�E���g
	const float LAND_GRAVITY = -50.0f;					// �N���n�ʂɒ��n���Ă��鎞�̏d��
}

// �ÓI�����o�ϐ�
CListManager<CEnemy*> CEnemy::m_list = {};				// ���X�g

//================================
// �R���X�g���N�^
//================================
CEnemy::CEnemy() : CCharacter(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pMotion = nullptr;			// ���[�V����

	m_type = TYPE_TORDLE;			// ���
	m_state = STATE_NONE;			// ���
	m_collSize = NONE_D3DXVECTOR3;	// �����蔻��̃T�C�Y
	m_nLife = 0;					// �̗�
	m_nStateCount = 0;				// ��ԃJ�E���g
	m_fGravity = 0.0f;				// �d��

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//================================
// �f�X�g���N�^
//================================
CEnemy::~CEnemy()
{

}

//================================
// �I������
//================================
void CEnemy::Uninit(void)
{
	// ���[�V�����̏I������
	delete m_pMotion;
	m_pMotion = nullptr;

	// �I������
	CCharacter::Uninit();

	// ������������
	m_list.Pull(this);
}

//================================
// �X�V����
//================================
void CEnemy::Update(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �d�͏���
	useful::Gravity(&m_fGravity, &pos.y, 0.5f);

	// �ʒu��K�p����
	SetPos(pos);

	// ���[�V�����̍X�V����
	m_pMotion->Update();

	// �N���n�ʂƂ̓����蔻��
	ElevationCollision();

	// �؂Ƃ̓����蔻��
	TreeCollision();

	// ��Ƃ̓����蔻��
	RockCollision();

	// �u���b�N�Ƃ̓����蔻��
	BlockCollision();

	if (m_state == STATE_DAMAGE)
	{ // �_���[�W��Ԃ̏ꍇ

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount % DAMAGE_COUNT == 0)
		{ // �J�E���g����萔�ɒB�����ꍇ

			// ��ԃJ�E���g��0�ɂ���
			m_nStateCount = 0;

			// �ʏ�J�E���g�ɂ���
			m_state = STATE_NONE;
		}
	}
}

//================================
// �`�揈��
//================================
void CEnemy::Draw(void)
{
	switch (m_state)
	{
	case CEnemy::STATE_NONE:

		// �`�揈��
		CCharacter::Draw();

		break;

	case CEnemy::STATE_DAMAGE:

		// �`�揈��
		CCharacter::Draw(DAMAGE_COL);
		
		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//================================
// ���̐ݒ菈��
//================================
void CEnemy::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
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
	m_pMotion->ResetMotion(MOTIONTYPE_NEUTRAL);

	// �S�Ă̒l��ݒ肷��
	m_type = type;					// ���
	m_state = STATE_NONE;			// ���
	m_collSize = COLLSIZE[m_type];	// �����蔻��̃T�C�Y
	m_nLife = LIFE[m_type];			// �̗�
	m_nStateCount = 0;				// ��ԃJ�E���g
	m_fGravity = 0.0f;				// �d��
}

//===========================================
// ���S���̏���
//===========================================
void CEnemy::Death(void)
{
	if (m_nLife <= 0)
	{ // �̗͂�0�ȉ��ɂȂ����ꍇ

		// �ʒu���擾����
		D3DXVECTOR3 pos = GetPos();

		// �ʒu��^�񒆂ɂ���
		pos.y += (m_collSize.y * 0.5f);

		// �A�j���[�V�������A�N�V�����𐶐�
		CAnimReaction::Create(pos, DEATH_EXPLOSION, NONE_D3DXCOLOR, CAnimReaction::TYPE::TYPE_GUNEXPLOSION, 4, 1);

		// �q�b�g�X�g�b�v
		Sleep(DEATH_HITSTOP);

		// �I������
		Uninit();
	}
	else
	{ // ��L�ȊO

		// �q�b�g�X�g�b�v
		Sleep(DAMAGE_HITSTOP);
	}
}

//===========================================
// ��������
//===========================================
CEnemy* CEnemy::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// �G�̃C���X�^���X�𐶐�
	CEnemy* pEnemy = nullptr;

	if (pEnemy == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		switch (type)
		{
		case CEnemy::TYPE_TORDLE:		// �^�[�h��

			// �^�[�h���𐶐�����
			pEnemy = new CTordle;

			break;

		case CEnemy::TYPE_IWAKARI:		// �C���J��

			// �C���J���𐶐�����
			pEnemy = new CIwakari;

			break;

		default:

			// ��~
			assert(false);

			break;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return pEnemy;
	}

	if (pEnemy != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEnemy->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEnemy->SetData(pos, rot, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �G�̃|�C���^��Ԃ�
	return pEnemy;
}

//===========================================
// ���[�V�����̎擾����
//===========================================
CMotion* CEnemy::GetMotion(void) const
{
	// ���[�V�����̏���Ԃ�
	return m_pMotion;
}

//===========================================
// ���[�V�����̐�������
//===========================================
void CEnemy::CreateMotion(void)
{
	if (m_pMotion == nullptr)
	{ // ���[�V������ NULL �̏ꍇ

		// ���[�V�����𐶐�����
		m_pMotion = CMotion::Create();
	}
}

//===========================================
// ��ނ̐ݒ菈��
//===========================================
void CEnemy::SetType(const TYPE type)
{
	// ��ނ�ݒ肷��
	m_type = type;
}

//===========================================
// ��ނ̎擾����
//===========================================
CEnemy::TYPE CEnemy::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//===========================================
// ��Ԃ̐ݒ菈��
//===========================================
void CEnemy::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//===========================================
// ��Ԃ̎擾����
//===========================================
CEnemy::STATE CEnemy::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//===========================================
// �����蔻��̃T�C�Y�̐ݒ菈��
//===========================================
void CEnemy::SetCollSize(const D3DXVECTOR3& size)
{
	// �����蔻��̃T�C�Y��ݒ肷��
	m_collSize = size;
}

//===========================================
// �����蔻��̃T�C�Y�̎擾����
//===========================================
D3DXVECTOR3 CEnemy::GetCollSize(void) const
{
	// �����蔻��̃T�C�Y���擾����
	return m_collSize;
}

//===========================================
// �̗͂̐ݒ菈��
//===========================================
void CEnemy::SetLife(const int nLife)
{
	// �̗͂�ݒ肷��
	m_nLife = nLife;
}

//===========================================
// �̗͂̎擾����
//===========================================
int CEnemy::GetLife(void) const
{
	// �̗͂�Ԃ�
	return m_nLife;
}

//===========================================
// �d�͂̐ݒ菈��
//===========================================
void CEnemy::SetGravity(const float fGravity)
{
	// �d�͂�ݒ肷��
	m_fGravity = fGravity;
}

//===========================================
// �d�͂̎擾����
//===========================================
float CEnemy::GetGravity(void) const
{
	// �d�͂�Ԃ�
	return m_fGravity;
}

//===========================================
// ���X�g�̎擾����
//===========================================
CListManager<CEnemy*> CEnemy::GetList(void)
{
	// ���X�g�̏���Ԃ�
	return m_list;
}

//===========================================
// �N���n�ʂƂ̓����蔻��
//===========================================
void CEnemy::ElevationCollision(void)
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

				// �d�͂�ݒ肷��
				m_fGravity = LAND_GRAVITY;
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
}

//===========================================
// �؂Ƃ̓����蔻��
//===========================================
void CEnemy::TreeCollision(void)
{
	// �ʒu�Ɣ��a���擾����
	D3DXVECTOR3 pos = GetPos();
	float fRadius = m_collSize.x;

	// �؂Ƃ̓����蔻��
	collision::TreeCollision(&pos, fRadius);

	// �ʒu��K�p����
	SetPos(pos);
}

//===========================================
// ��Ƃ̓����蔻��
//===========================================
void CEnemy::RockCollision(void)
{

}

//===========================================
// �u���b�N�Ƃ̓����蔻��
//===========================================
void CEnemy::BlockCollision(void)
{
	// ���[�J���ϐ��錾
	collision::SCollision coll = { false,false,false,false,false,false };		// �����蔻��̕ϐ�
	D3DXVECTOR3 pos = GetPos();							// �ʒu���擾����
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-m_collSize.x, 0.0f, -m_collSize.z);		// �ŏ��l���擾����
	D3DXVECTOR3 vtxMax = m_collSize;					// �ő�l���擾����
	CListManager<CBlock*> list = CBlock::GetList();
	CBlock* pBlock = nullptr;		// �擪�̒l
	CBlock* pBlockEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBlock = list.GetTop();

		// �����̒l���擾����
		pBlockEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �Z�ʑ̂̓����蔻��
			coll = collision::HexahedronClush
			(
				&pos,
				pBlock->GetPos(),
				GetPosOld(),
				pBlock->GetPosOld(),
				vtxMin,
				pBlock->GetVtxMin(),
				vtxMax,
				pBlock->GetVtxMax()
			);

			if (pBlock == pBlockEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBlock = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �ʒu��K�p����
	SetPos(pos);
}