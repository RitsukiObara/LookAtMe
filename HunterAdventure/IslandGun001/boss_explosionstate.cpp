//==================================================================
//
// �{�X�̔��j��ԏ��� [boss_explosionstate.cpp]
// Author�F��������
//
//==================================================================
//******************************************************************
//	�C���N���[�h�t�@�C��
//******************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_explosionstate.h"
#include "manager.h"
#include "motion.h"
#include "sound.h"

#include "game.h"
#include "alter_flash.h"
#include "boss_ripple.h"
#include "boss_explosion.h"

//------------------------------------------------------------------
// �萔��`
//------------------------------------------------------------------
namespace
{
	const int FINISH_COUNT = 100;		// �I���J�E���g
	const float RIPPLE_HEIGHT = 600.0f;	// �g����o������
}

//==========================
// �R���X�g���N�^
//==========================
CBossExplosionState::CBossExplosionState()
{
	// �S�Ă̒l���N���A����
	m_nCount = 0;		// �J�E���g
}

//==========================
// �f�X�g���N�^
//==========================
CBossExplosionState::~CBossExplosionState()
{

}

//==========================
// ��Ԃ̐ݒ菈��
//==========================
void CBossExplosionState::Process(CBoss* pBoss)
{
	// �J�E���g�����Z����
	m_nCount++;

	if (m_nCount == FINISH_COUNT)
	{ // ��莞�Ԍo�߂̏ꍇ

		// �M���𐶐�
		CAlterFlash::Create();

		// �{�X�̔��j�g��𐶐�
		CBossRipple::Create(D3DXVECTOR3(pBoss->GetPos().x, pBoss->GetPos().y + RIPPLE_HEIGHT, pBoss->GetPos().z));

		// �{�X�̔����𐶐�
		CBossExplosion::Create(D3DXVECTOR3(pBoss->GetPos().x, pBoss->GetPos().y + RIPPLE_HEIGHT, pBoss->GetPos().z), pBoss->GetRot());

		// �{�X�̔�������炷
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_EXPLOSION);

		// �I����Ԃɂ���
		CGame::SetState(CGame::STATE_FINISH);

		pBoss->Uninit();

		return;
	}
}

//==========================
// ���̐ݒ菈��
//==========================
void CBossExplosionState::SetData(CBoss* pBoss)
{
	// �S�Ă̒l��ݒ肷��
	m_nCount = 0;		// �J�E���g
}