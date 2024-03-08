//================================================================================================================
//
// �{�X�̉��U����ԏ��� [boss_firestate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_firestate.h"
#include "motion.h"

#include "game.h"
#include "player.h"
#include "fraction.h"
#include "fire_warning.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const float CHASE_CORRECT = 0.01f;		// �ǐՂ̕␳��
	const int FIRE_CREATE_COUNT = 30;		// �����o���J�E���g��
	const int FIRE_CREATE_RANGE = 180;		// �����o���Ă���Ԃ̃J�E���g��
	const int FINISH_COUNT = 240;			// �I���J�E���g

	const int FRACTION_COUNT = 3;			// �j�Ђ��o����
	const int FRACTION_TIMING[FRACTION_COUNT] =		// �j�Ђ��o���^�C�~���O
	{
		17,
		104,
		190
	};
	const int NUM_FRACTION = 15;			// �o���j�Ђ̑���
	const int RAND_FRACTION_LIFE = 30;		// �j�Ђ̎����̃����_���l
	const int MIN_FRACTION_LIFE = 15;		// �j�Ђ̎����̍ŏ��l
	const int FRACTION_WIDTH = 4;			// �j�Ђ̕�
	const int FRACTION_HEIGHT = 45;			// �j�Ђ̍���
}

//==========================
// �R���X�g���N�^
//==========================
CBossFireState::CBossFireState()
{
	// �S�Ă̒l���N���A����
	m_nCount = 0;					// �o�߃J�E���g
}

//==========================
// �f�X�g���N�^
//==========================
CBossFireState::~CBossFireState()
{

}

//==========================
// ��ԏ���
//==========================
void CBossFireState::Process(CBoss* pBoss)
{
	// �J�E���g�����Z����
	m_nCount++;

	// ����������
	FireCreate(pBoss);
	
	if (m_nCount == FRACTION_TIMING[0] ||
		m_nCount == FRACTION_TIMING[1] ||
		m_nCount == FRACTION_TIMING[2])
	{ // �K��̃J�E���g���ɂȂ����ꍇ

		// �j�Ђ��o������
		Fraction(pBoss);
	}

	if (m_nCount == FIRE_CREATE_RANGE)
	{ // �J�E���g����萔�̏ꍇ

		if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_NEUTRAL)
		{ // �ʏ탂�[�V�����ȊO�̏ꍇ

			// �ʏ탂�[�V�����ɂ���
			pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_NEUTRAL);
		}
	}

	if (m_nCount >= FINISH_COUNT)
	{ // �I���J�E���g�o�߂����ꍇ

		// ��Ԑ؂�ւ�����
		pBoss->ChangeState(new CBossNoneState);
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossFireState::SetData(CBoss* pBoss)
{
	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_STOMP)
	{ // �����݃��[�V�����ȊO�̏ꍇ

		// �����݃��[�V�����ɂ���
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_STOMP);
	}
}

//==========================
// �����o������
//==========================
void CBossFireState::FireCreate(CBoss* pBoss)
{
	CPlayer* pPlayer = CGame::GetPlayer();

	if (m_nCount % FIRE_CREATE_COUNT == 0 &&
		m_nCount <= FIRE_CREATE_RANGE &&
		pPlayer != nullptr)
	{ // �o�߃J�E���g����萔�o�߂����ꍇ

		// �����ӂ̐���
		CFireWarning::Create(pPlayer->GetPos());
	}
}

//==========================
// �j�Џo������
//==========================
void CBossFireState::Fraction(CBoss* pBoss)
{
	D3DXMATRIX mtx;
	D3DXVECTOR3 pos[2] = {};
	int nLife = 0;

	for (int nCntFrac = 0; nCntFrac < 2; nCntFrac++)
	{
		// �ʒu��ݒ�
		pBoss->GetHierarchy(nCntFrac + 7)->MatrixCalc(&mtx, pBoss->GetMatrix());
		pos[nCntFrac] = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

		for (int nCnt = 0; nCnt < NUM_FRACTION; nCnt++)
		{
			// �j�Ђ𐶐�
			nLife = rand() % RAND_FRACTION_LIFE + MIN_FRACTION_LIFE;
			CFraction::Create(pos[nCntFrac], CFraction::TYPE::TYPE_DIRT, nLife, FRACTION_WIDTH, FRACTION_HEIGHT);
		}
	}
}