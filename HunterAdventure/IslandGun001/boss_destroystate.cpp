//================================================================================================================
//
// �{�X�̌��j��ԏ��� [boss_destroystate.cpp]
// Author�F��������
//
//================================================================================================================
//****************************************************************************************************************
//	�C���N���[�h�t�@�C��
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_destroystate.h"
#include "manager.h"
#include "motion.h"

#include "camera.h"
#include "game.h"
#include "anim_reaction.h"

//----------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int FINISH_COUNT = 180;			// �I���܂ł̃J�E���g��
	const float INIT_ADD_HEIGHT = 30.0f;	// �����̉��Z���̏����l
	const float DEC_HEIGHT = 0.1f;			// �����̉��Z���̌����W��
}

//==========================
// �R���X�g���N�^
//==========================
CBossDestroyState::CBossDestroyState()
{
	// �S�Ă̒l���N���A����
	m_nCount = 0;						// �o�߃J�E���g
	m_fAddHeight = INIT_ADD_HEIGHT;		// �����̉��Z��
}

//==========================
// �f�X�g���N�^
//==========================
CBossDestroyState::~CBossDestroyState()
{

}

//==========================
// ��Ԃ̐ݒ菈��
//==========================
void CBossDestroyState::Process(CBoss* pBoss)
{
	// �o�߃J�E���g�����Z����
	m_nCount++;

	// �ړ�����
	Move(pBoss);

	if (m_nCount >= FINISH_COUNT)
	{ // ��莞�Ԍo�߂̏ꍇ

		// �I����Ԃɂ���
		CGame::SetState(CGame::STATE_FINISH);
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossDestroyState::SetData(CBoss* pBoss)
{
	if (CManager::Get()->GetCamera()->GetType() != CCamera::TYPE_BOSSDESTROY)
	{ // �{�X�̎��S��ԈȊO�̏ꍇ

		// �{�X�̎��S�J������Ԃɂ���
		CManager::Get()->GetCamera()->SetType(CCamera::TYPE_BOSSDESTROY);
	}
}

//==========================
// �ړ�����
//==========================
void CBossDestroyState::Move(CBoss* pBoss)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = pBoss->GetPos();

	// ������ݒ肷��
	pos.y += m_fAddHeight;

	// �����̉��Z������������
	m_fAddHeight -= DEC_HEIGHT;

	// �ʒu��K�p����
	pBoss->SetPos(pos);
}