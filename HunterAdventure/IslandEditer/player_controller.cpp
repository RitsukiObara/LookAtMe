//===========================================
//
// �v���C���[�̃R���g���[���[�̃��C������[player_controller.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "player_controller.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "useful.h"

#include "player.h"
#include "motion.h"
#include "player_action.h"
#include "aim.h"
#include "bullet.h"
#include "bulletUI.h"
#include "handgun.h"
#include "dagger.h"
#include "objectElevation.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const float CAMERA_ELEVATION_HEIGHT = 30.0f;	// �J�����̋N���n�ʂ̍���
	const float JUMP = 24.0f;						// �W�����v��
	const float CAMERA_ROT_CORRECT = 0.000003f;		// �J�����̌����̕␳�{��
	const float CAMERA_HEIGHT = 0.000002f;			// �J�����̍����̔{��
	const float SHOT_ADD_HEIGHT = 160.0f;			// �ˌ����̍����̒ǉ���
	const float AIM_SHIFT = 1000.0f;				// �G�C����\�����镝
	const float CAMERA_MOUSE_MAGNI = 5000.0f;		// �}�E�X�ł̃J��������̔{��
	const float SHOT_SHIFT_ROT[NUM_HANDGUN] =		// �ˌ����̂��炷����
	{
		(-D3DX_PI * 0.06f),
		(D3DX_PI * 0.06f),
	};
	const float SHOT_SHIFT_LENGTH = 95.0f;			// �ˌ����̂��炷��
	const float SHOTGUN_GRAVITY = 15.0f;			// �U�e��Ԃ̎��̏d��
	const float SHOTGUN_RECOIL = 7.0f;				// �U�e��Ԃ̔���
	const int LAST_SHOTCOUNT = 2;					// ���X�g�t�@�C�A�̗P�\�t���[��
	const int LAST_SHOT_BULLET = 20;				// ���X�g�t�@�C�A�̒e�̐�
	const int SHOTGUN_RAND_ROT = 60;				// �U�e�̃����_���Ŕ�΂�����
	const int LAST_RAND_ROT = 80;					// ���X�g�t�@�C�A�̃����_���Ŕ�΂�����
	const int NUM_SHOTGUN_BULLET = 8;				// �U�e�Ŕ�΂��e�̐�
	const int DODGE_INTERVAL = 90;					// ����C���^�[�o��
	const int SHOT_INTERVAL = 10;					// ���C���^�[�o��
	const float SPEED = 14.0f;						// ���x
}

//=========================
// �R���X�g���N�^
//=========================
CPlayerController::CPlayerController()
{
	// �S�Ă̒l���N���A����
	m_nShotCount = 0;			// �ˌ��J�E���g
	m_fStickRot = 0.0f;			// �X�e�B�b�N�̌���
	m_fSpeed = SPEED;			// ���x
	m_bRightShot = true;		// �E�Ō����ǂ���
}

//=========================
// �f�X�g���N�^
//=========================
CPlayerController::~CPlayerController()
{

}

//=========================
// �s���̏���������
//=========================
HRESULT CPlayerController::Init(void)
{
	// ������Ԃ�
	return S_OK;
}

//=========================
// �s���̏I������
//=========================
void CPlayerController::Uninit(void)
{
	// ���g����������
	delete this;
}

//=======================================
// ���쏈��
//=======================================
void CPlayerController::Control(CPlayer* pPlayer)
{
	if (CManager::Get()->GetInputGamePad()->GetConnect() == true)
	{ // �Q�[���p�b�h���ڑ�����Ă���ꍇ

		// �J�����̑��쏈��
		CameraControl(pPlayer);
	}
	else
	{ // ��L�ȊO

		// �}�E�X�ł̃J�����̑��쏈��
		CameraMouse(pPlayer);
	}

	if (pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_DODGE &&
		pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_DAGGER &&
		pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SHOTGUN &&
		pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SWOOP)
	{ // ����ԈȊO�̏ꍇ

		if (CManager::Get()->GetInputGamePad()->GetConnect() == true)
		{ // �Q�[���p�b�h���h�����Ă����ꍇ

			// �����̈ړ�����
			RotMove(pPlayer);
		}
		else
		{ // ��L�ȊO

			// �L�[�{�[�h�ł̏���
			KeyboardMove(pPlayer);
		}

		if (pPlayer->GetAction() != nullptr &&
			pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_RELOAD)
		{ // �����[�h��ԈȊO

			// �W�����v����
			Jump(pPlayer);

			// �U������
			Shot(pPlayer);

			// �_�K�[����
			Dagger(pPlayer);

			// �������
			Avoid(pPlayer);
		}
	}
}

//=======================================
// ��������
//=======================================
CPlayerController* CPlayerController::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CPlayerController* pControl = nullptr;		// �R���g���[���|�̃C���X�^���X�𐶐�

	if (pControl == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pControl = new CPlayerController;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pControl != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pControl->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �R���g���[���[��Ԃ�
	return pControl;
}

//=======================================
// �N���n�ʂƃJ�����̓����蔻��
//=======================================
void CPlayerController::ElevationCamera(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posV = CManager::Get()->GetCamera()->GetPosV();	// �ʒu���擾����
	float fHeight = 0.0f;			// ����
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// �擪�̏���
	CElevation* pElevEnd = nullptr;		// �����̒l
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
			fHeight = pElev->ElevationCollision(posV) + CAMERA_ELEVATION_HEIGHT;

			if (posV.y < fHeight)
			{ // �����蔻��̈ʒu�����������ꍇ

				// ������ݒ肷��
				posV.y = fHeight;
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
	CManager::Get()->GetCamera()->SetPosV(posV);
}

//=======================================
// �����̐ݒ菈��
//=======================================
void CPlayerController::RotMove(CPlayer* pPlayer)
{
	// ���[�J���ϐ���錾����
	float fStickRotX = 0.0f;	// �X�e�B�b�N��X���W
	float fStickRotY = 0.0f;	// �X�e�B�b�N��Y���W
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();		// �ړI�̌���
	D3DXVECTOR3 move = pPlayer->GetMove();				// �ړ���
	D3DXVECTOR3 CameraRot = CManager::Get()->GetCamera()->GetRot();	// �J�����̌���

	// �X�e�B�b�N�̌��������
	fStickRotY = (float)(CManager::Get()->GetInputGamePad()->GetGameStickLYPress(0));
	fStickRotX = (float)(CManager::Get()->GetInputGamePad()->GetGameStickLXPress(0));

	if (fStickRotY != 0 ||
		fStickRotX != 0)
	{ // �E�X�e�B�b�N���ǂ������ɓ|�����ꍇ

		// �X�e�B�b�N�̌�����ݒ肷��
		m_fStickRot = atan2f(fStickRotX, fStickRotY);

		// �����̐��K��
		useful::RotNormalize(&m_fStickRot);

		// �����ɃJ�����̌��������Z����
		m_fStickRot += CameraRot.y;

		if (pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SHOT)
		{ // �ˌ���ԈȊO�̏ꍇ

			// �����̐��K��
			useful::RotNormalize(&m_fStickRot);

			// ������ݒ肷��
			rotDest.y = m_fStickRot;

			// �ړI�̌�����ݒ肷��
			pPlayer->SetRotDest(rotDest);
		}

		// �ړ��ʂ�ݒ肷��
		move.x = sinf(m_fStickRot) * m_fSpeed;
		move.z = cosf(m_fStickRot) * m_fSpeed;

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_MOVE)
		{ // �ړ����[�V��������Ȃ������ꍇ

			// �ړ����[�V������ݒ肷��
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_MOVE);
		}
	}
	else
	{ // ��L�ȊO

		// �ړ��ʂ�ݒ肷��
		move.x = 0.0f;
		move.z = 0.0f;

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_NEUTRAL)
		{ // �ړ����[�V��������Ȃ������ꍇ

			// �ړ����[�V������ݒ肷��
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_NEUTRAL);
		}
	}

	// �ړ��ʂ�ݒ肷��
	pPlayer->SetMove(move);
}

//=======================================
// �L�[�{�[�h�ł̐ݒ菈��
//=======================================
void CPlayerController::KeyboardMove(CPlayer* pPlayer)
{
	// ���[�J���ϐ���錾����
	float fMoveX = 0.0f;	// X���̈ړ���
	float fMoveZ = 0.0f;	// Z���̈ړ���
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();		// �ړI�̌���
	D3DXVECTOR3 move = pPlayer->GetMove();				// �ړ���
	D3DXVECTOR3 CameraRot = CManager::Get()->GetCamera()->GetRot();	// �J�����̌���

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_W) == true)
	{ // W�L�[���������ꍇ

		// Z���̈ړ��ʂ�ݒ肷��
		fMoveZ = 1.0f;
	}

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_S) == true)
	{ // S�L�[���������ꍇ

		// Z���̈ړ��ʂ�ݒ肷��
		fMoveZ = -1.0f;
	}

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_A) == true)
	{ // A�L�[���������ꍇ

		// X���̈ړ��ʂ�ݒ肷��
		fMoveX = -1.0f;
	}

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_D) == true)
	{ // D�L�[���������ꍇ

		// X���̈ړ��ʂ�ݒ肷��
		fMoveX = 1.0f;
	}

	if (fMoveX != 0 ||
		fMoveZ != 0)
	{ // �E�X�e�B�b�N���ǂ������ɓ|�����ꍇ

		// �X�e�B�b�N�̌�����ݒ肷��
		m_fStickRot = atan2f(fMoveX, fMoveZ);

		// �����̐��K��
		useful::RotNormalize(&m_fStickRot);

		// �����ɃJ�����̌��������Z����
		m_fStickRot += CameraRot.y;

		if (pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SHOT)
		{ // �ˌ���ԈȊO�̏ꍇ

			// �����̐��K��
			useful::RotNormalize(&m_fStickRot);

			// ������ݒ肷��
			rotDest.y = m_fStickRot;
		}

		// �ړ��ʂ�ݒ肷��
		move.x = sinf(m_fStickRot) * m_fSpeed;
		move.z = cosf(m_fStickRot) * m_fSpeed;

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_MOVE)
		{ // �ړ����[�V��������Ȃ������ꍇ

			// �ړ����[�V������ݒ肷��
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_MOVE);
		}
	}
	else
	{ // ��L�ȊO

		// �ړ��ʂ�ݒ肷��
		move.x = 0.0f;
		move.z = 0.0f;

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_NEUTRAL)
		{ // �ړ����[�V��������Ȃ������ꍇ

			// �ړ����[�V������ݒ肷��
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_NEUTRAL);
		}
	}

	// �ړ��ʂƖڕW�̌�����ݒ肷��
	pPlayer->SetMove(move);
	pPlayer->SetRotDest(rotDest);
}

//=======================================
// �W�����v����
//=======================================
void CPlayerController::Jump(CPlayer* pPlayer)
{
	// �ړ��ʂ��擾����
	D3DXVECTOR3 move = pPlayer->GetMove();

	if ((CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
		CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true) &&
		pPlayer->IsJump() == false)
	{ // A�{�^�����������ꍇ

		// �ړ��ʂ�ݒ肷��
		move.y = JUMP;

		// �W�����v���Ă���
		pPlayer->SetEnableJump(true);
	}

	// �ړ��ʂ�K�p����
	pPlayer->SetMove(move);
}

//=======================================
// �J�����̑��쏈��
//=======================================
void CPlayerController::CameraControl(CPlayer* pPlayer)
{
	D3DXVECTOR3 CameraRot = CManager::Get()->GetCamera()->GetRot();		// �J�����̌������擾����
	float fStickRotX, fStickRotY;		// �X�e�B�b�N�̌���

	// �E�X�e�B�b�N�̌X���x���擾����
	fStickRotX = CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0);
	fStickRotY = CManager::Get()->GetInputGamePad()->GetGameStickRYPress(0);

	// �J�����̌��������Z����
	CameraRot.y += (fStickRotX * CAMERA_ROT_CORRECT);

	// �����̐��K��
	useful::RotNormalize(&CameraRot.y);

	// �J�����̌��������Z����
	CameraRot.x -= (fStickRotY * CAMERA_HEIGHT);

	if (CameraRot.x >= D3DX_PI - 0.02f)
	{ // ���������𒴂����ꍇ

		CameraRot.x = D3DX_PI - 0.02f;
	}
	else if (CameraRot.x <= 0.02f)
	{ // ���������𒴂����ꍇ

		CameraRot.x = 0.02f;
	}

	if (pPlayer->GetAim() != nullptr)
	{ // �G�C���� NULL ����Ȃ��ꍇ

		D3DXVECTOR3 posShot;		// �e���o���ʒu

		// �ʒu��ݒ肷��
		posShot.x = pPlayer->GetPos().x;
		posShot.y = pPlayer->GetPos().y + SHOT_ADD_HEIGHT;
		posShot.z = pPlayer->GetPos().z;

		// �ˌ��̈ʒu��ݒ肷��
		pPlayer->GetAim()->SetPosPlayer(posShot);
	}

	//// �N���n�ʂƃJ�����̓����蔻��
	//ElevationCamera();

	// ������K�p����
	CManager::Get()->GetCamera()->SetRot(CameraRot);
}

//=======================================
// �}�E�X�ł̃J�����̑��쏈��
//=======================================
void CPlayerController::CameraMouse(CPlayer* pPlayer)
{
	D3DXVECTOR3 CameraRot = CManager::Get()->GetCamera()->GetRot();		// �J�����̌������擾����
	float fMoveX = 0.0f;
	float fMoveZ = 0.0f;		// �X�e�B�b�N�̌���

	fMoveX = CManager::Get()->GetInputMouse()->GetMove().x * CAMERA_MOUSE_MAGNI;
	fMoveZ = -CManager::Get()->GetInputMouse()->GetMove().y * CAMERA_MOUSE_MAGNI;

	// �J�����̌��������Z����
	CameraRot.y += (fMoveX * CAMERA_ROT_CORRECT);

	// �����̐��K��
	useful::RotNormalize(&CameraRot.y);

	// �J�����̌��������Z����
	CameraRot.x -= (fMoveZ * CAMERA_HEIGHT);

	if (CameraRot.x >= D3DX_PI - 0.01f)
	{ // ���������𒴂����ꍇ

		CameraRot.x = D3DX_PI - 0.01f;
	}
	else if (CameraRot.x <= 0.0f + 0.01f)
	{ // ���������𒴂����ꍇ

		CameraRot.x = 0.0f + 0.01f;
	}

	if (pPlayer->GetAim() != nullptr)
	{ // �G�C���� NULL ����Ȃ��ꍇ

		D3DXVECTOR3 posShot;		// �e���o���ʒu

		// �ʒu��ݒ肷��
		posShot.x = pPlayer->GetPos().x;
		posShot.y = pPlayer->GetPos().y + SHOT_ADD_HEIGHT;
		posShot.z = pPlayer->GetPos().z;

		// �ˌ��̈ʒu��ݒ肷��
		pPlayer->GetAim()->SetPosPlayer(posShot);
	}

	//// �N���n�ʂƃJ�����̓����蔻��
	//ElevationCamera();

	// ������K�p����
	CManager::Get()->GetCamera()->SetRot(CameraRot);
}

//=======================================
// �ˌ��n����
//=======================================
void CPlayerController::Shot(CPlayer* pPlayer)
{
	if ((pPlayer->GetAction()->GetAction() == CPlayerAction::ACTION_NONE ||
		pPlayer->GetAction()->GetAction() == CPlayerAction::ACTION_SHOT) &&
		(CManager::Get()->GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_RB, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_LB, 0) == true ||
		CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_L) == true) &&
		pPlayer->GetBulletUI()->GetNumBullet() > 0)
	{ // �c�e������ARB�ELB�L�[���������ꍇ

		if (pPlayer->IsJump() == true)
		{ // �W�����v���Ă����ꍇ

			// �U�e����
			ShotGun(pPlayer);
		}
		else
		{ // ��L�ȊO

			// ���e����
			HandGun(pPlayer);
		}

		// �ˌ��J�E���g�����Z����
		m_nShotCount++;
	}

	if (pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SHOTGUN &&
		(CManager::Get()->GetInputGamePad()->GetRelease(CInputGamePad::JOYKEY_RB, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetRelease(CInputGamePad::JOYKEY_LB, 0) == true ||
		CManager::Get()->GetInputMouse()->GetRelease(CInputMouse::MOUSE_L) == true))
	{ // RB�ELB�{�^���𗣂����ꍇ

		// �ˌ��J�E���g��0�ɂ���
		m_nShotCount = 0;

		if (pPlayer->GetAction() != nullptr &&
			pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_RELOAD)
		{ // �����[�h��ԈȊO�̏ꍇ

			// �ʏ��Ԃɂ���
			pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_NONE);
		}
	}
}

//=======================================
// ���e����
//=======================================
void CPlayerController::HandGun(CPlayer* pPlayer)
{
	// �c�e�����擾����
	int nNumBullet = pPlayer->GetBulletUI()->GetNumBullet();

	// �ˌ���Ԃɂ���
	pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_SHOT);

	if (m_nShotCount % SHOT_INTERVAL == 0)
	{ // ���J�E���g���Ƃ�

		D3DXVECTOR3 pos;		// �e�̏o��ʒu��錾
		D3DXVECTOR3 rot;		// �e�̏o�������錾

		// ������ݒ肷��
		rot = CManager::Get()->GetCamera()->GetRot();

		// �����̐��K��
		useful::RotNormalize(&rot.x);

		// �ʒu��ݒ肷��
		pos.x = pPlayer->GetPos().x + sinf(pPlayer->GetRot().y + SHOT_SHIFT_ROT[(int)(m_bRightShot)]) * SHOT_SHIFT_LENGTH;
		pos.y = pPlayer->GetPos().y + SHOT_ADD_HEIGHT;
		pos.z = pPlayer->GetPos().z + cosf(pPlayer->GetRot().y + SHOT_SHIFT_ROT[(int)(m_bRightShot)]) * SHOT_SHIFT_LENGTH;

		// �e������
		CBullet::Create(pos, rot, CBullet::TYPE::TYPE_HANDGUN);

		// �E�Ō����ǂ�����ς���
		m_bRightShot = !m_bRightShot;

		// �c�e�������炷
		nNumBullet--;

		// �c�e����K�p����
		pPlayer->GetBulletUI()->SetNumBullet(nNumBullet);

		if (nNumBullet <= 0)
		{ // �c�e��0�ɂȂ����ꍇ

			// �����[�h��Ԃɂ���
			pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_RELOAD);

			for (int nCnt = 0; nCnt < NUM_HANDGUN; nCnt++)
			{
				if (pPlayer->GetHandGun(nCnt) != nullptr)
				{ // ���e�� NULL ����Ȃ��ꍇ

					// �����[�h��Ԃɐݒ肷��
					pPlayer->GetHandGun(nCnt)->SetState(CHandgun::STATE_RELOAD);
				}
			}
		}
	}
}

//=======================================
// �U�e����
//=======================================
void CPlayerController::ShotGun(CPlayer* pPlayer)
{
	// �����̈ړ��ʂƃJ�����̌�����錾
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotCamera = CManager::Get()->GetCamera()->GetRot();
	int nNumBullet = pPlayer->GetBulletUI()->GetNumBullet();

	// �ړ��ʂ�ݒ肷��
	move.x = sinf(rotCamera.y) * -SHOTGUN_RECOIL;
	move.y = SHOTGUN_GRAVITY;
	move.z = cosf(rotCamera.y) * -SHOTGUN_RECOIL;

	// �����̈ړ��ʂ�ݒ肷��
	pPlayer->GetAction()->SetMoveRecoil(move);

	// �ړ��ʂ�����������
	pPlayer->SetMove(NONE_D3DXVECTOR3);

	D3DXVECTOR3 pos = pPlayer->GetPos();	// �v���C���[�̈ʒu��錾
	D3DXVECTOR3 rot;						// �v���C���[�̌�����錾
	D3DXVECTOR3 rotBullet;					// �e�̏o�������錾

	// ������ݒ肷��
	rot = CManager::Get()->GetCamera()->GetRot();

	// �����̐��K��
	useful::RotNormalize(&rot.x);

	// �ʒu��ݒ肷��
	pos.x = pPlayer->GetPos().x;
	pos.y = pPlayer->GetPos().y + SHOT_ADD_HEIGHT;
	pos.z = pPlayer->GetPos().z;

	if (nNumBullet == 1 &&
		m_nShotCount < LAST_SHOTCOUNT)
	{ // �Ō�̒e�������ꍇ

		for (int nCnt = 0; nCnt < LAST_SHOT_BULLET; nCnt++)
		{
			// �e�̏o�������ݒ肷��
			rotBullet.x = rot.x + (float)((rand() % LAST_RAND_ROT - (LAST_RAND_ROT / 2)) * 0.01f);
			rotBullet.y = rot.y + (float)((rand() % LAST_RAND_ROT - (LAST_RAND_ROT / 2)) * 0.01f);
			rotBullet.z = rot.z + (float)((rand() % LAST_RAND_ROT - (LAST_RAND_ROT / 2)) * 0.01f);

			// �e������
			CBullet::Create(pos, rotBullet, CBullet::TYPE::TYPE_SHOTGUN);
		}
	}
	else
	{ // ��L�ȊO

		for (int nCnt = 0; nCnt < NUM_SHOTGUN_BULLET; nCnt++)
		{
			// �e�̏o�������ݒ肷��
			rotBullet.x = rot.x + (float)((rand() % SHOTGUN_RAND_ROT - (SHOTGUN_RAND_ROT / 2)) * 0.01f);
			rotBullet.y = rot.y + (float)((rand() % SHOTGUN_RAND_ROT - (SHOTGUN_RAND_ROT / 2)) * 0.01f);
			rotBullet.z = rot.z + (float)((rand() % SHOTGUN_RAND_ROT - (SHOTGUN_RAND_ROT / 2)) * 0.01f);

			// �e������
			CBullet::Create(pos, rotBullet, CBullet::TYPE::TYPE_SHOTGUN);
		}
	}

	// �c�e�������炷
	nNumBullet--;

	// �c�e����K�p����
	pPlayer->GetBulletUI()->SetNumBullet(nNumBullet);

	if (nNumBullet <= 0)
	{ // �c�e��0�ɂȂ����ꍇ

		// �����[�h��Ԃɂ���
		pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_RELOAD);

		for (int nCnt = 0; nCnt < NUM_HANDGUN; nCnt++)
		{
			if (pPlayer->GetHandGun(nCnt) != nullptr)
			{ // ���e�� NULL ����Ȃ��ꍇ

				// ���e�������[�h��Ԃɐݒ肷��
				pPlayer->GetHandGun(nCnt)->SetState(CHandgun::STATE_RELOAD);
			}
		}
	}
	else
	{ // ��L�ȊO

		// �U�e��Ԃɂ���
		pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_SHOTGUN);
	}

	// �����󋵂� true �ɂ���
	pPlayer->GetAction()->SetEnableRecoil(true);
}

//=======================================
// �������
//=======================================
void CPlayerController::Avoid(CPlayer* pPlayer)
{
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();

	if (pPlayer->IsJump() == false &&
		pPlayer->GetAction()->IsDodgeUse() == true &&
		(CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_B, 0) == true ||
			CManager::Get()->GetInputMouse()->GetTrigger(CInputMouse::MOUSE_R) == true))
	{ // �n���B�L�[���������ꍇ

		// �����Ԃɂ���
		pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_DODGE);

		// ������������ݒ肷��
		pPlayer->GetAction()->SetDodgeRot(m_fStickRot);

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_DODGE)
		{ // ������[�V��������Ȃ��ꍇ

			// ������[�V�����ɂ���
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_DODGE);
		}

		// �ڕW�̌�����ݒ肷��
		rotDest.y = m_fStickRot;

		// ����C���^�[�o����ݒ肷��
		pPlayer->GetAction()->SetDodgeInterval(DODGE_INTERVAL);

		// �g�p�\�󋵂�false�ɂ���
		pPlayer->GetAction()->SetEnableDodgeUse(false);
	}

	// �ړI�̌�����K�p����
	pPlayer->SetRotDest(rotDest);
}

//=======================================
// �_�K�[����
//=======================================
void CPlayerController::Dagger(CPlayer* pPlayer)
{
	if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_X, 0) == true ||
		CManager::Get()->GetInputMouse()->GetTrigger(CInputMouse::MOUSE_WHEEL) == true)
	{ // X�L�[���������ꍇ

		if (pPlayer->IsJump() == true)
		{ // �W�����v�󋵂� true �̏ꍇ

			// �}�~����Ԃɂ���
			pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_SWOOP);
		}
		else
		{ // ��L�ȊO

			// �_�K�[��Ԃɂ���
			pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_DAGGER);
		}

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_DAGGER)
		{ // �_�K�[���[�V�����ȊO�̏ꍇ

			// �_�K�[���[�V������ݒ肷��
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_DAGGER);
		}

		// �_�K�[��\������
		pPlayer->GetDagger()->SetEnableDisp(true);

		// ���e��`�悵�Ȃ�
		pPlayer->GetHandGun(0)->SetEnableDisp(false);
		pPlayer->GetHandGun(1)->SetEnableDisp(false);
	}
}