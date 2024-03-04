//============================================
//
// �`���[�g���A���v���C���[�̃��C������[player_tutorial.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "player_tutorial.h"
#include "tutorial.h"
#include "collision.h"
#include "renderer.h"
#include "area.h"
#include "useful.h"

#include "player_controller.h"
#include "motion.h"
#include "player_action.h"
#include "handgun.h"
#include "aim.h"
#include "lifeUI.h"
#include "airplane.h"
#include "door.h"

// �萔��`
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(40.0f, 200.0f, 40.0f);		// �����蔻�莞�̃T�C�Y
	const float TRANS_DEPTH = -180.0f;				// �J�ڂ̎��̖ړI��Z���W
	const float TRANS_DEST_ROT = 0.0f;				// �J�ڂ̎��̖ړI�̌���
	const float TRANS_DEST_CAMERAROT_X = 1.3f;		// �J�ڂ̎��̖ړI�̌���
	const float TRANS_DEST_CAMERAROT_Y = 0.0f;		// �J�ڂ̎��̖ړI�̌���
	const float TRANS_DEST_CAMERA_DIST = 800.0f;	// �J�ڂ̎��̖ړI�̌���
	const float TRANS_CORRECT = 0.05f;				// �J�ڂ̎��̕␳�W��
	const int MOVE_COUNT = 65;						// �ړ�����J�E���g
	const float MOVE_DEPTH = 180.0f;				// �ړ�����ړI��Z���W
	const float TRANS_GRAVITY = 1.0f;				// �J�ڏ�Ԓ��̏d��
	const D3DXVECTOR3 STAGE_VTXMAX = D3DXVECTOR3(2500.0f, 0.0f, 1500.0f);		// �X�e�[�W�̍ő�l
	const D3DXVECTOR3 STAGE_VTXMIN = D3DXVECTOR3(-2500.0f, 0.0f, -2500.0f);		// �X�e�[�W�̍ŏ��l
}

//=========================================
// �R���X�g���N�^
//=========================================
CTutorialPlayer::CTutorialPlayer() : CPlayer()
{
	// �S�Ă̒l���N���A����
	m_nTransCount = 0;		// �J�ڃJ�E���g
}

//=========================================
// �f�X�g���N�^
//=========================================
CTutorialPlayer::~CTutorialPlayer()
{

}

//===========================================
// �v���C���[�̏���������
//===========================================
HRESULT CTutorialPlayer::Init(void)
{
	if (FAILED(CPlayer::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �v���C���[�̏I������
//===========================================
void CTutorialPlayer::Uninit(void)
{
	// �I������
	CPlayer::Uninit();

	// �v���C���[��NULL������
	CTutorial::DeletePlayer();
}

//===========================================
// �X�V����
//===========================================
void CTutorialPlayer::Update(void)
{
	// �O��̈ʒu�̐ݒ菈��
	SetPosOld(GetPos());

	switch (CTutorial::GetState())
	{
	case CTutorial::STATE_NONE:

		if (collision::SignboardCollision(GetPos(), COLLISION_SIZE.x) == true)
		{ // �Ŕɋ߂Â����ꍇ

			// ���̐�̏������s��Ȃ�
			return;
		}

		if (collision::DoorHit(GetPos(), COLLISION_SIZE.x) == true)
		{ // �h�A���J�����ꍇ

			// ���̐�̏������s��Ȃ�
			return;
		}

		// ���쏈��
		GetController()->Control(this);

		// �ړ�����
		Move();

		if (GetMotion() != nullptr)
		{ // ���[�V������ NULL ����Ȃ��ꍇ

			// ���[�V�����̍X�V����
			GetMotion()->Update();
		}

		if (GetAction() != nullptr)
		{ // �s���� NULL ����Ȃ��ꍇ

			// �s���̍X�V����
			GetAction()->Update(this);
		}

		for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
		{
			if (GetHandGun(nCntGun) != nullptr)
			{ // ���e�� NULL ����Ȃ��ꍇ

				// �X�V����
				GetHandGun(nCntGun)->Update();
			}
		}

		// �ً}�̃����[�h����
		EmergentReload();

		if (GetAim() != nullptr)
		{ // �G�C���� NULL ����Ȃ��ꍇ

			// �G�C���̍X�V����
			GetAim()->Update();
		}

		if (GetLifeUI() != nullptr)
		{ // ������ NULL ����Ȃ��ꍇ

			// ������ݒ肷��
			GetLifeUI()->SetLife(GetLife());
		}

		break;

	case CTutorial::STATE_EXPL:

		break;

	case CTutorial::STATE_TRANS:

		// �J�ڏ�ԏ���
		Trans();

		if (GetMotion() != nullptr)
		{ // ���[�V������ NULL ����Ȃ��ꍇ

			// ���[�V�����̍X�V����
			GetMotion()->Update();
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// �敪�̐ݒ菈��
	SetAreaIdx(area::SetFieldIdx(GetPos()));

	// ���V�̎��Ƃ̓����蔻��
	collision::PalmFruitHit(this, COLLISION_SIZE.x, COLLISION_SIZE.y);

	// ���̍��Ƃ̓����蔻��
	collision::GoldBoneCollision(*this, COLLISION_SIZE);

	// �N���n�ʂƂ̓����蔻�菈��
	ElevationCollision();

	// �����蔻�菈��
	Collision();

	if (CTutorial::GetState() == CTutorial::STATE_NONE)
	{ // �ʏ��Ԃ̏ꍇ

		// �h�A�Ƃ̓����蔻��
		DoorCollision();

		// �X�e�[�W�Ƃ̓����蔻��
		StageCollision();
	}
}

//===========================================
// �v���C���[�̕`�揈��
//===========================================
void CTutorialPlayer::Draw(void)
{
	// �`�揈��
	CPlayer::Draw();
}

//=======================================
// ���̐ݒ菈��
//=======================================
void CTutorialPlayer::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	CPlayer::SetData(pos);

	// ��s�@����������
	GetAirplane()->Uninit();
	DeleteAirplane();

	// �ʒu���Đݒ肷��
	SetPos(pos);

	// �S�Ă̒l���N���A����
	m_nTransCount = 0;		// �J�ڃJ�E���g
}

//===========================================
// ��������
//===========================================
CTutorialPlayer* CTutorialPlayer::Create(const D3DXVECTOR3& pos)
{
	// �v���C���[�̃C���X�^���X�𐶐�
	CTutorialPlayer* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ���������m�ۂ���
		pPlayer = new CTutorialPlayer;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return pPlayer;
	}

	if (pPlayer != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pPlayer->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �v���C���[�̃|�C���^��Ԃ�
	return pPlayer;
}

//===========================================
// �h�A�Ƃ̓����蔻��
//===========================================
void CTutorialPlayer::DoorCollision(void)
{
	// �ʒu�ƈړ��ʂ��擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	// �h�A�Ƃ̓����蔻��
	if (collision::DoorCollision(&pos, GetPosOld(), COLLISION_SIZE) == true)
	{ // ��ɏ�����ꍇ

		// �W�����v�󋵂� false �ɂ���
		SetEnableJump(false);

		// �d�͂�0�ɂ���
		move.y = 0.0f;
	}

	// �ʒu�ƈړ��ʂ�K�p
	SetPos(pos);
	SetMove(move);
}

//===========================================
// �J�ڏ�ԏ���
//===========================================
void CTutorialPlayer::Trans(void)
{
	// �J�ڃJ�E���g�����Z����
	m_nTransCount++;

	CDoor* pDoor = CTutorial::GetDoor();	// �h�A�̏��
	D3DXVECTOR3 pos = GetPos();				// �ʒu
	D3DXVECTOR3 rot = GetRot();				// ����
	D3DXVECTOR3 move = GetMove();			// �ړ���
	D3DXVECTOR3 rotCamera = CManager::Get()->GetCamera()->GetRot();
	float fDistCamera = CManager::Get()->GetCamera()->GetDistance();

	if (pDoor != nullptr)
	{ // �h�A�� NULL ����Ȃ��ꍇ

		if (m_nTransCount >= MOVE_COUNT)
		{ // �J�ڃJ�E���g����萔�ȏ�̏ꍇ

			if (pDoor->GetState() == CDoor::STATE_OPEN &&
				GetMotion()->GetType() != MOTIONTYPE_MOVE)
			{ // �h�A���J����Ԃ̏ꍇ

				// �ړ����[�V�����ɂ���
				GetMotion()->Set(MOTIONTYPE_MOVE);
			}

			if (useful::FrameCorrect(pDoor->GetPos().z + MOVE_DEPTH, &pos.z, GetController()->GetSpeedInit()) == true)
			{ // �ړI�̈ʒu�ɒ������ꍇ

				// ����Ԃɂ���
				pDoor->SetState(CDoor::STATE_CLOSE);

				// �ʏ탂�[�V�����ɂ���
				GetMotion()->Set(MOTIONTYPE_NEUTRAL);
			}
		}
		else
		{ // ��L�ȊO

			// �ړ�����
			useful::Correct(pDoor->GetPos().x, &pos.x, TRANS_CORRECT);
			useful::Correct(pDoor->GetPos().z + TRANS_DEPTH, &pos.z, TRANS_CORRECT);

			// ���ʂ�����
			useful::RotCorrect(TRANS_DEST_ROT, &rot.y, TRANS_CORRECT);
			useful::RotCorrect(TRANS_DEST_CAMERAROT_X, &rotCamera.x, TRANS_CORRECT);
			useful::RotCorrect(TRANS_DEST_CAMERAROT_Y, &rotCamera.y, TRANS_CORRECT);
			useful::Correct(TRANS_DEST_CAMERA_DIST, &fDistCamera, TRANS_CORRECT);
		}
	}

	// �d�͏���
	useful::Gravity(&move.y, &pos.y, TRANS_GRAVITY);

	// ����K�p
	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	CManager::Get()->GetCamera()->SetRot(rotCamera);
	CManager::Get()->GetCamera()->SetDistance(fDistCamera);
}

//===========================================
// �X�e�[�W�Ƃ̓����蔻��
//===========================================
void CTutorialPlayer::StageCollision(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	if (pos.x + COLLISION_SIZE.x >= STAGE_VTXMAX.x)
	{ // ������͂ݏo���ꍇ

		// �ʒu��␳����
		pos.x = STAGE_VTXMAX.x - COLLISION_SIZE.x;
	}

	if (pos.x - COLLISION_SIZE.x <= STAGE_VTXMIN.x)
	{ // �E����͂ݏo���ꍇ

		// �ʒu��␳����
		pos.x = STAGE_VTXMIN.x + COLLISION_SIZE.x;
	}

	if (pos.z + COLLISION_SIZE.z >= STAGE_VTXMAX.z)
	{ // ������͂ݏo���ꍇ

		// �ʒu��␳����
		pos.z = STAGE_VTXMAX.z - COLLISION_SIZE.z;
	}

	if (pos.z - COLLISION_SIZE.z <= STAGE_VTXMIN.z)
	{ // ��O����͂ݏo���ꍇ

		// �ʒu��␳����
		pos.z = STAGE_VTXMIN.z + COLLISION_SIZE.z;
	}

	// �ʒu��K�p
	SetPos(pos);
}