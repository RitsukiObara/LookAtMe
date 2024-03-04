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
#include "area.h"
#include "sound.h"
#include "useful.h"

#include "motion.h"
#include "game.h"
#include "game_score.h"
#include "addscoreUI.h"
#include "rock.h"
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
		D3DXVECTOR3(120.0f,230.0f,120.0f),
	};
	const int LIFE[CEnemy::TYPE_MAX] =					// �̗�
	{
		50,
		5,
	};
	const int SCORE[CEnemy::TYPE_MAX] =					// �X�R�A
	{
		500,
		1000,
	};
	const CAddScoreUI::TYPE ADDSCORE_TYPE[CEnemy::TYPE_MAX] =		// �ǉ��X�R�A�̎��
	{
		CAddScoreUI::TYPE_TORDLE,
		CAddScoreUI::TYPE_IWAKARI,
	};
	const D3DXVECTOR3 DEATH_EXPLOSION = D3DXVECTOR3(200.0f, 200.0f, 0.0f);		// ���S���̔���
	const D3DXCOLOR DAMAGE_COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);				// �_���[�W��Ԃ̐F
	const int DAMAGE_COUNT = 12;						// �_���[�W��Ԃ̃J�E���g
	const float LAND_GRAVITY = -50.0f;					// �N���n�ʂɒ��n���Ă��鎞�̏d��
	const int SLASHRIPPLE_DAMAGE = 2;					// �a���g��ɓ����������̃_���[�W
	const float SLASHRIPPLE_KNOCKBACK = 20.0f;			// �a���g��ɓ����������̐������
	const int WIND_SHOT_DAMAGE = 10;					// ���U���ɓ����������̃_���[�W
	const float WIND_SHOT_KNOCKBACK = 50.0f;			// ���U���ɓ����������̐������
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
	m_nFieldIdx = 0;				// �敪�̔ԍ�

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

	// �敪�̔ԍ��ݒ菈��
	m_nFieldIdx = area::SetFieldIdx(GetPos());

	// ���[�V�����̍X�V����
	m_pMotion->Update();

	// �a���g��Ƃ̓����蔻��
	SlashRippleHit();

	// �N���n�ʂƂ̓����蔻��
	ElevationCollision();

	// �����蔻�菈��
	Collision();

	// �Ւd�Ƃ̓����蔻��
	AlterCollision();

	// �G���m�̓����蔻��
	collision::EnemyHitToEnemy(this);

	switch (m_state)
	{
	case CEnemy::STATE_NONE:

		if (collision::WindShotHit(GetPos(), m_collSize.x, m_collSize.y) == true ||
			collision::FireShotHit(GetPos(), m_collSize.x, m_collSize.y) == true)
		{ // ���������ꍇ

			// �q�b�g����
			Hit(WIND_SHOT_DAMAGE, WIND_SHOT_KNOCKBACK);
		}

		break;

	case CEnemy::STATE_DAMAGE:

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount % DAMAGE_COUNT == 0)
		{ // �J�E���g����萔�ɒB�����ꍇ

			// ��ԃJ�E���g��0�ɂ���
			m_nStateCount = 0;

			// �ʏ�J�E���g�ɂ���
			m_state = STATE_NONE;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// �X�e�[�W�̓����蔻��
	StageCollision();
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

	// �敪�̔ԍ��ݒ菈��
	m_nFieldIdx = area::SetFieldIdx(GetPos());
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

		// �_���[�W����炷
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);

		if (CGame::GetGameScore() != nullptr)
		{ // �Q�[���X�R�A�� NULL ����Ȃ��ꍇ

			// �X�R�A�����Z����
			CGame::GetGameScore()->AddScore(SCORE[m_type]);

			// �ǉ��X�R�AUI�𐶐�
			CAddScoreUI::Create(pos + m_collSize, ADDSCORE_TYPE[m_type]);
		}

		// �I������
		Uninit();
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
// ��ނ̎擾����
//===========================================
CEnemy::TYPE CEnemy::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
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
// �����蔻�菈��
//===========================================
void CEnemy::Collision(void)
{
	D3DXVECTOR3 pos = GetPos();								// �ʒu
	D3DXVECTOR3 posOld = GetPosOld();						// �O��̈ʒu
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(m_collSize);	// ���_�̍ŏ��l
	int nIdx = 0;

	for (int nCnt = 0; nCnt < area::NUM_COLL; nCnt++)
	{
		nIdx = m_nFieldIdx + area::COLL_ADD_IDX[nCnt];

		if (area::IndexCheck(nIdx) == true)
		{ // �敪���̏ꍇ

			// �؂Ƃ̓����蔻��
			collision::TreeCollision(&pos, m_collSize.x, nIdx);

			// �u���b�N�Ƃ̓����蔻��
			BlockCollision(&pos, posOld, m_collSize, vtxMin, nIdx);

			// ��Ƃ̓����蔻��
			collision::RockCollision(&pos, posOld, m_collSize.x, m_collSize.y, nIdx, &m_fGravity);

			// �ǂƂ̓����蔻��
			collision::WallCollision(&pos, posOld, m_collSize, vtxMin, nIdx);
		}
	}

	// �ʒu��K�p����
	SetPos(pos);
}

//===========================================
// �u���b�N�Ƃ̓����蔻��
//===========================================
void CEnemy::BlockCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx)
{
	// ���[�J���ϐ��錾
	collision::SCollision coll = { false,false,false,false,false,false };		// �����蔻��̕ϐ�
	CListManager<CBlock*> list = CBlock::GetList(nAreaIdx);
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
				pos,
				pBlock->GetPos(),
				posOld,
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
}

//===========================================
// �Ւd�Ƃ̓����蔻��
//===========================================
void CEnemy::AlterCollision(void)
{
	D3DXVECTOR3 pos = GetPos();			// �ʒu
	D3DXVECTOR3 posOld = GetPosOld();	// �O��̈ʒu
	D3DXVECTOR3 vtxMax = m_collSize;	// �ő�l
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(m_collSize);	// �ŏ��l

	// �ǂƂ̓����蔻��
	collision::AlterCollision(&pos, posOld, vtxMax, vtxMin);

	// �ʒu��K�p����
	SetPos(pos);
}

//===========================================
// �a���̔g��Ƃ̓����蔻��
//===========================================
void CEnemy::SlashRippleHit(void)
{
	// �ϐ���ݒ�
	D3DXVECTOR3 pos = GetPos();
	float fRadius = m_collSize.x;
	float fHeight = m_collSize.y;

	if (m_state == STATE_NONE &&
		collision::RippleHit(pos, fRadius, fHeight) == true)
	{ // �a���g��ɓ��������ꍇ

		// �q�b�g����
		Hit(SLASHRIPPLE_DAMAGE, SLASHRIPPLE_KNOCKBACK);
	}
}

//===========================================
// �X�e�[�W�̓����蔻��
//===========================================
void CEnemy::StageCollision(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	// �X�e�[�W�̓����蔻��
	collision::StageCollision(&pos, m_collSize.x);

	// �ʒu��K�p
	SetPos(pos);
}