//================================================================================================================
//
// �{�X�̌��������ԏ��� [boss_turnstate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_turnstate.h"
#include "motion.h"
#include "sound.h"

#include "game.h"
#include "alter.h"
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "fraction.h"

#include "boss_howlingstate.h"

//----------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const float ROT_CORRECT = 0.08f;		// �����̕␳�{��
	const int COUNT = 100;					// ���������Ԃ̃J�E���g��
	const float SUB_HEIGHT = 30.0f;			// ���x�̌��Z��
	const int FRACTION_PART = 4;			// �j�Ђ̏o��p�[�c��
	const int NUM_FRACTION = 15;			// �j�Ђ̐�
	const D3DXVECTOR3 FRACTION_SCALE = D3DXVECTOR3(2.0f, 2.0f, 2.0f);		// �j�Ђ̊g�嗦
}
//==========================
// �R���X�g���N�^
//==========================
CBossTurnState::CBossTurnState()
{
	// �S�Ă̒l���N���A����
	m_nCount = 0;		// �o�߃J�E���g
}

//==========================
// �f�X�g���N�^
//==========================
CBossTurnState::~CBossTurnState()
{

}

//==========================
// ��ԏ���
//==========================
void CBossTurnState::Process(CBoss* pBoss)
{
	// �v���C���[�̏����擾����
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �J�E���g�����Z����
		m_nCount++;

		// �������擾����
		D3DXVECTOR3 rot = pBoss->GetRot();
		D3DXVECTOR3 pos = pBoss->GetPos();
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		float fRotDest = atan2f(posPlayer.x - pos.x, posPlayer.z - pos.z);

		// �����̕␳����
		useful::RotCorrect(fRotDest, &rot.y, ROT_CORRECT);

		// �����̐��K��
		useful::RotNormalize(&rot.y);

		// ������K�p����
		pBoss->SetRot(rot);

		if (m_nCount >= COUNT)
		{ // ��萔�o�߂����ꍇ

			// �{�X�o����Ԃ�ݒ肷��
			CGame::SetState(CGame::STATE_BOSSMOVIE);

			if (CManager::Get()->GetCamera()->GetType() != CCamera::TYPE_BOSSCLOSER)
			{ // �{�X�߂Â���ԈȊO�̏ꍇ

				// �{�X�߂Â��J������Ԃɂ���
				CManager::Get()->GetCamera()->SetType(CCamera::TYPE_BOSSCLOSER);
			}

			// ���x�����Z����
			pos.y -= SUB_HEIGHT;

			// �ʒu�̐ݒ菈��
			pBoss->SetPos(pos);

			// �N���n�ʂƂ̓����蔻��
			if (pBoss->ElevationCollision() == true)
			{ // �n�ʂɒ��n�����ꍇ

				// �j�Д�������
				Fraction(pBoss);

				// �Ւd�̔j�󏈗�
				CGame::GetAlter()->Break();

				// ��������炷
				CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

				// ������␳����
				rot.y = fRotDest;

				// ������K�p����
				pBoss->SetRot(rot);

				// ���i����Ԃɂ���
				pBoss->ChangeState(new CBossHowlingState());
			}

			// ���̐�̏������s��Ȃ�
			return;
		}
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossTurnState::SetData(CBoss* pBoss)
{
	// �S�Ă̒l��ݒ肷��
	m_nCount = 0;		// �o�߃J�E���g

	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_HOVERING)
	{ // �z�o�����O���[�V�����ȊO�̏ꍇ

		// �z�o�����O���[�V������ݒ肷��
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_HOVERING);
	}
}

//==========================
// �j�Д�������
//==========================
void CBossTurnState::Fraction(CBoss* pBoss)
{
	D3DXMATRIX mtx;
	D3DXVECTOR3 aPos[FRACTION_PART];

	// �s����擾
	pBoss->GetHierarchy(7)->MatrixCalc(&mtx, pBoss->GetMatrix());

	// �ʒu���擾����
	aPos[0] = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

	// �s����擾
	pBoss->GetHierarchy(8)->MatrixCalc(&mtx, pBoss->GetMatrix());

	// �ʒu���擾����
	aPos[1] = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

	// �s����擾
	pBoss->GetHierarchy(15)->MatrixCalc(&mtx, pBoss->GetMatrix());

	// �ʒu���擾����
	aPos[2] = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

	// �s����擾
	pBoss->GetHierarchy(16)->MatrixCalc(&mtx, pBoss->GetMatrix());

	// �ʒu���擾����
	aPos[3] = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

	for (int nCntPart = 0; nCntPart < FRACTION_PART; nCntPart++)
	{
		for (int nCnt = 0; nCnt < NUM_FRACTION; nCnt++)
		{
			// �j�Ђ̐���
			CFraction::Create(aPos[nCntPart], CFraction::TYPE::TYPE_DIRT, 50, 40, 10, FRACTION_SCALE);
		}
	}
}