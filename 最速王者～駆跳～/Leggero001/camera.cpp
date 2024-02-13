//===========================================
//
// �J�����̃��C������[camera.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "tutorial.h"
#include "game.h"
#include "useful.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "object.h"
#include "model.h"
#include "scene.h"
#include "file.h"

#include "player.h"
#include "editAll.h"
#include "pause.h"
#include "game_time.h"
#include "countdown.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------

// �J�����S��
#define ASPECT_RATIO				(80.0f)				// ��ʂ̃A�X�y�N�g��
#define MIN_DISTANCE				(50.0f)				// �����̍ŏ��l
#define MAX_DISTANCE				(8000.0f)			// �����̍ő�l
#define DRAW_MIN_Z					(10.0f)				// Z���̍ŏ��l
#define DRAW_MAX_Z					(50000.0f)			// Z���̍ő�l

// �����֌W
#define ROT_Y_SPEED					(0.04f)				// Y���̉�]�̑��x
#define ROTATION_SPEED				(0.05f)				// ��荞�ݏ������s����̃��f���̑��x
#define ROTATION_ROT				(0.02f)				// �J�����̊p�x�̕␳�{��

// �ʒu�E�����֌W
#define POS_SPEED					(30.0f)				// �ړ����x
#define DIS_SPEED					(16.0f)				// �����̈ړ���
#define CAMERA_DISTANCE				(300.0f)			// �J�����̋���
#define POSR_POINT					(40.0f)				// �Ǐ]���[�h�̒����_�̈ʒu
#define POSV_POINT					(40.0f)				// �Ǐ]���[�h�̎��_�̈ʒu
#define CORRECT_POSR				(0.22f)				// �����_�̕␳�{��
#define CORRECT_POSV				(0.20f)				// ���_�̕␳�{��
#define RANKING_MOVE				(40.0f)				// �����L���O�J�����̈ړ���
#define RANKING_STOP				(25000.0f)			// �����L���O�J�����̎~�܂���W

// ����J�����֌W
#define CORRECT_UPWARD_POSV			(0.05f)				// ������J�����̎��_�̕␳�{��
#define ZOOMOUT_DISTANCE			(600.0f)			// �Y�[���A�E�g�J�����̋���
#define ZOOMIN_DISTANCE				(150.0f)			// �Y�[���C���J�����̋���
#define ZOOM_DIST_CORRECT			(0.3f)				// �Y�[���̋����̕␳
#define LANDING_PROCESS_COUNT		(3)					// ���n�J�����̋������ω�����J�E���g��
#define LANDING_CHANGE_COUNT		(9)					// ���n�J�����̃J�E���g��
#define LANDING_CORRECT_POSR		(0.3f)				// ���n�J�����̒����_�̕␳�{��
#define LANDING_CORRECT_POSV		(0.3f)				// ���n�J�����̎��_�̕␳�{��
#define STAGE_CAMERA_SHIFT_WIDTH	(500.0f)			// �X�e�[�W���e�J�����̂��炷��(X��)
#define STAGE_CAMERA_HEIGHT			(500.0f)			// �X�e�[�W���e�J�����̍���
#define STAGE_CAMERA_DISTANCE		(1000.0f)			// �X�e�[�W���e�J�����̋���
#define STAGE_CAMERA_MOVE			(200.0f)			// �X�e�[�W���e�J�����̈ړ���
#define STAGE_CORRECT_POS			(0.5f)				// �X�e�[�W���e�J�����̕␳�{��
#define RUN_CAMERA_DISTANCE			(200.0f)			// ����p�J�����̋���
#define BACK_CORRECT_MAGNI			(0.05f)				// �߂�J�����̕␳�{��
#define BACK_DECISION_DISTANCE		(CAMERA_DISTANCE - 0.001f)		// �߂�J�����̔��肪�ʂ鋗��
#define BOOST_DISTANCE				(400.0f)			// �u�[�X�g�J�����̋���
#define BOOST_DIST_CORRECT			(0.05f)				// �Y�[���̋����̕␳
#define PRECEDENCE_POS				(450.0f)			// ��s���e�J�����̋���

//=======================
// �R���X�g���N�^
//=======================
CCamera::CCamera()
{
	// �S�Ă̏����N���A����
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���_
	m_posVDest = m_posV;						// �ړI�̎��_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_
	m_posRDest = m_posR;						// �ړI�̒����_
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_type = TYPE_NONE;							// ���
	m_rotDest = m_rot.y;						// �ړI�̌���
	m_Dis = 0.0f;								// ����
	m_DisDest = 0.0f;							// �ړI�̋���
	m_nSwingCount = 0;							// �h��J�����̃J�E���g
	m_bControl = false;							// �����
}

//=======================
// �f�X�g���N�^
//=======================
CCamera::~CCamera()
{

}

//=======================
// �J�����̏���������
//=======================
HRESULT CCamera::Init(void)
{
	//�J�����̏��̃��Z�b�g����
	Reset();

	// �r���[�|�[�g�̐ݒ菈��
	m_viewport.X = (DWORD)0.0f;				// �`�悷���ʂ̍���X���W
	m_viewport.Y = (DWORD)0.0f;				// �`�悷���ʂ̍���Y���W
	m_viewport.Width = SCREEN_WIDTH;	// �`�悷���ʂ̕�
	m_viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̍���
	m_viewport.MinZ = 0.0f;				// �N���b�v�{�����[���̍ŏ��l
	m_viewport.MaxZ = 1.0f;				// �N���b�v�{�����[���̍ő�l

	// ������Ԃ�
	return S_OK;
}

//=======================
// �J�����̏I������
//=======================
void CCamera::Uninit(void)
{

}

//=======================
// �J�����̍X�V����
//=======================
void CCamera::Update(void)
{
	switch (CManager::GetMode())
	{
	case CScene::MODE_TUTORIAL:	// �`���[�g���A��

		switch (CTutorial::GetState())
		{
		case CTutorial::STATE_END:

			if (CScene::GetPlayer() != nullptr)
			{ // �v���C���[�̏�񂪂������ꍇ

				// �ړI�̒����_��ݒ肷��
				m_posRDest.y = CScene::GetPlayer()->GetPos().y + PLAYER_HEIGHT * 0.5f;

				// ���_��␳
				m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
				m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSR;
				m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;

				// �����_��ݒ肷��
				m_posR.x = CScene::GetPlayer()->GetPos().x;
				m_posR.y += (m_posRDest.y - m_posR.y) * CORRECT_POSR;
				m_posR.z = CScene::GetPlayer()->GetPos().z;
			}

			break;

		default:

#if 1
			// ��ނ��Ƃ̍X�V����
			TypeProcess();
#else
			// ���쏈��
			Control();
#endif
			break;
		}

		break;

	case CScene::MODE_GAME:		// �Q�[�����[�h

		if (CGame::GetPause() != nullptr &&
			CGame::GetPause()->GetPause() == false)
		{ // �|�[�Y���ȊO�̏ꍇ

			switch (CGame::GetState())
			{
			case CGame::STATE_START:		// �X�^�[�g��

				// ��ނ��Ƃ̍X�V����
				TypeProcess();

				break;

			case CGame::STATE_COUNTDOWN:	// �J�E���g�_�E��

				// ��ނ��Ƃ̍X�V����
				TypeProcess();

				break;

			case CGame::STATE_PLAY:			// �v���C���

// �f�o�b�O���[�h
#ifdef _DEBUG

// �|�[�Y�ȊO�̃J����
#if 1
				if (CGame::GetPause() != nullptr)
				{ // �Q�[����ʂ��A�|�[�Y�� NULL ����Ȃ��ꍇ

					if (CGame::GetEdit() == false &&
						CGame::GetPause()->GetPause() == false)
					{ // �v���C���[�h�̏ꍇ

						// ��ނ��Ƃ̍X�V����
						TypeProcess();
					}
				}
#endif

// �G�f�B�b�g��Ԃ̃J����
#if 1
				if (CGame::GetEdit() == true)
				{ // �G�f�B�b�g��Ԃ̏ꍇ

					// ���쏈��
					Control();
				}

#endif

// �����[�X���[�h
#else

				if (CGame::GetPause() != nullptr)
				{ // �Q�[����ʂ��A�|�[�Y�� NULL ����Ȃ��ꍇ

					if (CGame::GetPause()->GetPause() == false)
					{ // �v���C���[�h�̏ꍇ

						// ��ނ��Ƃ̍X�V����
						TypeProcess();
					}
				}

#endif

				break;

			case CGame::STATE_GOAL:			// �S�[�����

				// �ʏ�J�����ɂ���
				SetType(CCamera::TYPE_NONE);

				// ��ނ��Ƃ̍X�V����
				TypeProcess();

				break;

			case CGame::STATE_FINISH:		// �I�����

				// �ʏ�J�����ɂ���
				SetType(CCamera::TYPE_NONE);

				// ��ނ��Ƃ̍X�V����
				TypeProcess();

				break;

			case CGame::STATE_HITSTOP:		// �q�b�g�X�g�b�v���

				// �J�����̏�������(��ʂ��~�߂�)

				break;

			default:

				// �ړI�̒����_��ݒ肷��
				m_posR.x = 0.0f;
				m_posR.y = PLAYER_HEIGHT * 0.5f;
				m_posR.z = 0.0f;

				// �ړI�̎��_��ݒ肷��
				m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
				m_posV.y = m_posR.y;
				m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;

				break;
			}

// ����ԈȊO�̐ݒ�
#if 1
			if (m_type != TYPE_PRECEDENCE &&
				m_type != TYPE_STARTSTAGE &&
				m_type != TYPE_STARTRUN &&
				m_type != TYPE_STARTBACK)
			{ // ����ԈȊO�̏ꍇ

				// �ʒu�̐ݒ�(���K��)����
				PosSet();
			}
#endif
		}
		else
		{ // ��L�ȊO

// �f�o�b�O���[�h
#ifdef _DEBUG

// �|�[�Y���̃J��������
#if 1
			if (m_bControl == true)
			{ // �{���󋵂� true �̏ꍇ

				// ���쏈��
				Control();
			}
#endif

#endif
		}

		break;

	case CScene::MODE_RANKING:	// ���U���g

		if (m_posV.x <= RANKING_STOP ||
			m_posV.x <= RANKING_STOP)
		{ // �ʒu����萔�ȉ��������ꍇ

			// �J�������ړ�������
			m_posV.x += RANKING_MOVE;
			m_posR.x += RANKING_MOVE;
		}

		break;

	default:					// ��L�ȊO

		// ���̃��Z�b�g����
		Reset();

		break;
	}

	// �����̐��K������
	RotNormalize(&m_rot.y);

#if 0

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // �X�y�[�X�L�[���������ꍇ

		// �J�����̏��̃��Z�b�g����
		Reset();
	}

#endif
}

//=======================
// �J�����̐ݒ菈��
//=======================
void CCamera::Set(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

// �f�o�b�O�p
#ifdef _DEBUG

	if (CGame::GetEdit() == false)
	{ // �v���C���[�h�̏ꍇ

		// �v���W�F�N�V�����}�g���b�N�X���쐬(�������e)
		D3DXMatrixPerspectiveFovLH
		(
			&m_mtxProjection,									// �v���W�F�N�V�����}�g���b�N�X
			D3DXToRadian(ASPECT_RATIO),							// ����p
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// ��ʂ̃A�X�y�N�g��
			DRAW_MIN_Z,											// Z�l�̍ŏ��l
			DRAW_MAX_Z											// Z�l�̍ő�l
		);
	}
	else
	{ // �G�f�B�b�g���[�h�̏ꍇ

		if (CGame::GetEditAll() != nullptr)
		{ // �G�f�B�b�g�I�[���� NULL ����Ȃ��ꍇ

			if (CGame::GetEditAll()->GetStyle() == CEditAll::STYLE_BGOBJECT ||
				CGame::GetEditAll()->GetStyle() == CEditAll::STYLE_RANGEMOVE)
			{ // �w�i�I�u�W�F�N�g�Ɣ͈͈ړ��̏ꍇ

				// �v���W�F�N�V�����}�g���b�N�X���쐬(�������e)
				D3DXMatrixPerspectiveFovLH
				(
					&m_mtxProjection,									// �v���W�F�N�V�����}�g���b�N�X
					D3DXToRadian(ASPECT_RATIO),							// ����p
					(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// ��ʂ̃A�X�y�N�g��
					DRAW_MIN_Z,											// Z�l�̍ŏ��l
					DRAW_MAX_Z											// Z�l�̍ő�l
				);
			}
			else
			{ // ��L�ȊO

				// �v���W�F�N�V�����}�g���b�N�X�̍쐬(���s���e)
				D3DXMatrixOrthoLH
				(
					&m_mtxProjection,									// �r���[�}�g���b�N�X
					SCREEN_WIDTH,										// ��ʂ̕�
					SCREEN_HEIGHT,										// ��ʂ̍���
					DRAW_MIN_Z,											// Z���̍ŏ��l
					DRAW_MAX_Z											// Z���̍ő�l
				);
			}
		}
	}

// �����[�X�p
#else

	// �v���W�F�N�V�����}�g���b�N�X���쐬(�������e)
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,									// �v���W�F�N�V�����}�g���b�N�X
		D3DXToRadian(ASPECT_RATIO),							// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// ��ʂ̃A�X�y�N�g��
		DRAW_MIN_Z,											// Z�l�̍ŏ��l
		DRAW_MAX_Z											// Z�l�̍ő�l
	);

	//// �v���W�F�N�V�����}�g���b�N�X�̍쐬(���s���e)
	//D3DXMatrixOrthoLH
	//(
	//	&m_mtxProjection,									// �r���[�}�g���b�N�X
	//	SCREEN_WIDTH,										// ��ʂ̕�
	//	SCREEN_HEIGHT,										// ��ʂ̍���
	//	DRAW_MIN_Z,											// Z���̍ŏ��l
	//	DRAW_MAX_Z											// Z���̍ő�l
	//);

#endif

	// �v���W�F�N�g�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH
	(
		&m_mtxView,										// �r���[�}�g���b�N�X
		&m_posV,										// ���_
		&m_posR,										// �����_
		&m_VecU											// ������x�N�g��
	);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=======================
// ��ނ̐ݒ菈��
//=======================
void CCamera::SetType(const TYPE type)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	// ��ނ�ݒ肷��
	m_type = type;

	switch (m_type)
	{
	case TYPE_NONE:		// ����

		if (pPlayer != nullptr)
		{ // �v���C���[�� NULL ����Ȃ��ꍇ

			// �v���C���[�̏����擾����
			pos = pPlayer->GetPos();		// �ʒu
			rot = pPlayer->GetRot();		// ����

			// �ړI�̒����_��ݒ肷��
			m_posRDest.x = pos.x;
			m_posRDest.z = pos.z;

			// �ړI�̎��_��ݒ肷��
			m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
			m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

			// �ړI�̌�����ݒ肷��
			m_rotDest = 0.0f;
		}

		break;

	case TYPE_UPWARD:	// ������J����

		// ������J�����̐ݒ菈��
		SetUpward();

		break;

	case TYPE_PRECEDENCE:	// ��s�ǐ�

		// ��s�ǐՃJ�����̐ݒ菈��
		SetPrecedence();

		break;

	case TYPE_ZOOMOUT:	// �Y�[���A�E�g

		// �Y�[���A�E�g�̐ݒ菈��
		SetZoomOut();

		break;

	case TYPE_ZOOMIN:	// �Y�[���C��

		// �Y�[���C���̐ݒ菈��
		SetZoomIn();

		break;

	case TYPE_STARTSTAGE:	// �X�^�[�g�J����(�X�e�[�W)

		// �X�e�[�W���e�̐ݒ菈��
		SetStartStage();

		break;

	case TYPE_STARTRUN:		// �X�^�[�g�J����(����p)

		// ����p�̐ݒ菈��
		SetStartRun();

		break;

	case TYPE_STARTBACK:	// �X�^�[�g�J����(�߂�)

		// �߂�J�����̐ݒ菈��
		SetStartBack();

		break;

	case TYPE_LAND:			// ���n�J����

		// ���n�J�����̐ݒ菈��
		SetLand();

		break;

	case TYPE_WALLKICK:		// �ǃL�b�N�J����

		// �ǃL�b�N�̐ݒ菈��
		SetWallKick();

		break;

	case TYPE_BOOST:		// �u�[�X�g�J����

		// �u�[�X�g�J�����̐ݒ菈��
		SetBoost();

		break;
	}
}

//=======================
// ��ނ̎擾����
//=======================
CCamera::TYPE CCamera::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//=======================
// ���_�̈ʒu�̐ݒ菈��
//=======================
void CCamera::SetPosV(const D3DXVECTOR3 posV)
{
	// ���_�̈ʒu��ݒ肷��
	m_posV = posV;
}

//=======================
// ���_�̈ʒu�̎擾����
//=======================
D3DXVECTOR3 CCamera::GetPosV(void) const
{
	// ���_�̈ʒu��Ԃ�
	return m_posV;
}

//=======================
// �����_�̈ʒu�̐ݒ菈��
//=======================
void CCamera::SetPosR(const D3DXVECTOR3 posR)
{
	// �����_�̈ʒu��ݒ肷��
	m_posR = posR;
}

//=======================
// �����_�̈ʒu�̎擾����
//=======================
D3DXVECTOR3 CCamera::GetPosR(void) const
{
	// �����_�̈ʒu��Ԃ�
	return m_posR;
}

//=======================
// �����̐ݒ菈��
//=======================
void CCamera::SetRot(const D3DXVECTOR3& rot)
{
	// �����̏���ݒ肷��
	m_rot = rot;
}

//=======================
// �����̎擾����
//=======================
D3DXVECTOR3 CCamera::GetRot(void) const
{
	// �����̏���Ԃ�
	return m_rot;
}

//=======================
// �����̐ݒ菈��
//=======================
void CCamera::SetDistance(const float fDis)
{
	// ������ݒ肷��
	m_Dis = fDis;

	// �ړI�̎��_��ݒ肷��
	m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
}

//=======================
// �����̎擾����
//=======================
float CCamera::GetDistance(void) const
{
	// ������Ԃ�
	return m_Dis;
}

//=======================
// �J�����̑���󋵂̐؂�ւ�����
//=======================
void CCamera::ChangeControl(void)
{
	// ����󋵂�؂�ւ���
	m_bControl = m_bControl ? false : true;
}

//=======================
// �J�����̏��̃��Z�b�g����
//=======================
void CCamera::Reset(void)
{
	// �S�Ă̒l�����Z�b�g����
	m_Dis = CAMERA_DISTANCE;					// ���_�ƒ����_�̋���
	m_posV = D3DXVECTOR3(0.0f, PLAYER_HEIGHT * 0.5f, -m_Dis);	// ���_�̈ʒu
	m_posVDest = m_posV;						// �ړI�̎��_
	m_posR = D3DXVECTOR3(0.0f, PLAYER_HEIGHT * 0.5f, 0.0f);		// �����_�̈ʒu
	m_posRDest = m_posR;						// �ړI�̒����_
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_type = TYPE_NONE;							// ���
	m_rotDest = m_rot.y;						// �ړI�̌���
	m_DisDest = CAMERA_DISTANCE;				// �ړI�̋���
	m_nSwingCount = 0;							// �h��J�����̃J�E���g
	m_bControl = false;							// �����
}

//=======================
// �J�����̉�荞�ݏ���
//=======================
void CCamera::Rotation(void)
{
	////�J�����̖ړI�̌�����ݒ肷��
	//m_rotDest = (pModelrot->y);

	////�J�����̌����̍�����ݒ肷��
	//m_rotDiff = m_rotDest - m_rot.y;

	//if (m_rotDiff > D3DX_PI)
	//{//�p�x��3.14f���傫�������ꍇ
	// //�p�x����1�������炷
	//	m_rotDiff -= (2 * D3DX_PI);
	//}
	//else if (m_rotDiff < -D3DX_PI)
	//{//�p�x��-3.14f��菬���������ꍇ
	// //�p�x��1����������
	//	m_rotDiff += (2 * D3DX_PI);
	//}

	////�J�����̌�����␳����
	//m_rot.y += m_rotDiff * ROTATION_ROT;

	//if (m_rot.y > D3DX_PI)
	//{//�p�x��3.14f���傫�������ꍇ
	// //�p�x����1�������炷
	//	m_rot.y = -D3DX_PI;
	//}
	//else if (m_rot.y < -D3DX_PI)
	//{//�p�x��-3.14f��菬���������ꍇ
	// //�p�x��1����������
	//	m_rot.y = D3DX_PI;
	//}
}

//=======================
// �J�����̈ړ�����
//=======================
void CCamera::Move(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_UP) == true ||
		CManager::GetInputGamePad()->GetGameStickRYPress(0) > 0)
	{ // W�L�[���������ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // D�L�[���������ꍇ

			m_posR.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			
			m_posR.y += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.y += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // A�L�[���������ꍇ

			m_posR.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;

			m_posR.y += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.y += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else
		{ // W�L�[�݂̂��������ꍇ

			m_posR.x += sinf(m_rot.y) * POS_SPEED;
			m_posV.x += sinf(m_rot.y) * POS_SPEED;

			m_posR.y += cosf(m_rot.y) * POS_SPEED;
			m_posV.y += cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_DOWN) == true ||
		CManager::GetInputGamePad()->GetGameStickRYPress(0) < 0)
	{ // S�L�[���������ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // D�L�[���������ꍇ

			m_posR.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.y += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.y += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // A�L�[���������ꍇ

			m_posR.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.y += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.y += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else
		{ // S�L�[�݂̂��������ꍇ

			m_posR.x -= sinf(m_rot.y) * POS_SPEED;
			m_posV.x -= sinf(m_rot.y) * POS_SPEED;

			m_posR.y -= cosf(m_rot.y) * POS_SPEED;
			m_posV.y -= cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
		CManager::GetInputGamePad()->GetGameStickRXPress(0) > 0)
	{ // D�L�[���������ꍇ

		m_posR.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.y += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.y += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
		CManager::GetInputGamePad()->GetGameStickRXPress(0) < 0)
	{ // A�L�[���������ꍇ

		m_posR.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.y += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
		m_posV.y += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
	}
}

//=======================
// �J�����̎��_���쏈��
//=======================
void CCamera::MovePosV(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
	{ // Y�L�[���������ꍇ
		
		// ���_����ɓ�����
		m_posV.y += POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_N) == true)
	{ // N�L�[���������ꍇ

		// ���_�����ɓ�����
		m_posV.y -= POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_Z) == true)
	{ // Z�L�[���������ꍇ

		// ���_���E�ɉ�
		m_rot.y -= ROT_Y_SPEED;

		// �J�����̎��_���X�V����
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
	{ // C�L�[���������ꍇ

		// ���_�����ɉ�
		m_rot.y += ROT_Y_SPEED;

		// �J�����̎��_���X�V����
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
}

//=======================
// �J�����̒����_���쏈��
//=======================
void CCamera::MovePosR(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
	{ // T�L�[���������ꍇ

		// �����_����ɓ�����
		m_posR.y += POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_B) == true)
	{ // B�L�[���������ꍇ

		// �����_�����ɓ�����
		m_posR.y -= POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{ // Q�L�[���������ꍇ

		// �����_���E�ɉ�
		m_rot.y -= ROT_Y_SPEED;

		// �����_���X�V����
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{ // E�L�[���������ꍇ

		// �����_�����ɉ�
		m_rot.y += ROT_Y_SPEED;

		// �����_���X�V����
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
}

//=======================
// �J�����̋������쏈��
//=======================
void CCamera::MoveRange(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // ��SHIFT�L�[�������Ă���ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // U�L�[���������ꍇ

			// ���������Z����
			m_Dis -= DIS_SPEED;

			if (m_Dis <= MIN_DISTANCE)
			{ // 100.0f�ȉ��̏ꍇ

				// 100.0f�ɂ���
				m_Dis = MIN_DISTANCE;
			}

			// �J�����̎��_���X�V����
			m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
			m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_M) == true)
		{ // M�L�[���������ꍇ

			// ���������Z����
			m_Dis += DIS_SPEED;

			if (m_Dis >= MAX_DISTANCE)
			{ // 800.0f�ȏ�̏ꍇ

				// 800.0f�ɂ���
				m_Dis = MAX_DISTANCE;
			}

			// �J�����̎��_���X�V����
			m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
			m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
		}
	}
}

//=======================
// ���쏈��
//=======================
void CCamera::Control(void)
{
	//�J�����̈ړ�����
	Move();

	// �J�����̎��_���쏈��
	MovePosV();

	// �J�����̒����_���쏈��
	MovePosR();

	// �J�����̋������쏈��
	MoveRange();
}

//=======================
// �J�����̈ʒu�̐ݒ菈��
//=======================
void CCamera::PosSet(void)
{
	// �J�����̎��_���X�V����
	m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;

	// �J�����̒����_���X�V����
	m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
	m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
}

//=======================
// �J�����̎�ނ̍X�V����
//=======================
void CCamera::TypeProcess(void)
{
	switch (m_type)
	{
	case TYPE_NONE:		// ����

		// �ǐՏ���
		Chase();

		break;

	case TYPE_UPWARD:

		// ������J�����̍X�V
		Upward();

		break;

	case TYPE_PRECEDENCE:	// ��s�ǐՃJ����

		// ��s�ǐՃJ�����̍X�V
		Precedence();

		break;

	case TYPE_LAND:			// ���n�J����

		// ���n�J�����̍X�V
		Land();

		break;

	case TYPE_ZOOMOUT:	// �Y�[���A�E�g

		// �J�����̃Y�[���A�E�g����
		ZoomOut();

		break;

	case TYPE_ZOOMIN:	// �Y�[���C��

		// �J�����̃Y�[���C������
		ZoomIn();

		break;

	case TYPE_STARTSTAGE:	// �X�^�[�g�J����(�X�e�[�W)

		// �J�����̃X�e�[�W���e����
		StartStage();

		break;

	case TYPE_STARTRUN:		// �X�^�[�g�J����(����p)

		// �J�����̑���p����
		StartRun();

		break;

	case TYPE_STARTBACK:		// �X�^�[�g�J����(�߂�)

		// �J�����̖߂菈��
		StartBack();

		break;

	case TYPE_WALLKICK:			// �ǃL�b�N�J����

		// �J�����̕ǃL�b�N�̏���
		WallKick();

		break;

	case TYPE_BOOST:			// �u�[�X�g�J����

		// �u�[�X�g�J�����̏���
		Boost();

		break;
	}
}

//=======================
// �J�����̒ǐՏ���
//=======================
void CCamera::Chase(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
	CPlayer* pPlayer = CScene::GetPlayer();		// �v���C���[�̃|�C���^
	m_DisDest = CAMERA_DISTANCE;	// �ړI�̋���

	// �����̕␳����
	Correct(m_DisDest, &m_Dis, CORRECT_POSR);
	Correct(m_rotDest, &m_rot.y, CORRECT_POSR);

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// �����_��␳
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y += (m_posRDest.y - m_posR.y) * CORRECT_POSR;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// ���_��␳
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSR;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	}
}

//=======================
// �J�����̏��������
//=======================
void CCamera::Upward(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// �����_��␳
		m_posR.x += (pos.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (pos.z - m_posR.z) * CORRECT_POSR;

		// ���_��␳
		m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSV;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_UPWARD_POSV;

		if (pPlayer->GetJump() == false)
		{ // �n��ɒ�������

			// ��ނ�ݒ肷��
			SetType(TYPE_NONE);
		}
	}
}

//=======================
// �J�����̐�s�ǐՏ���
//=======================
void CCamera::Precedence(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ڕW�̒����_��ݒ肷��
		m_posRDest = pos;

		// �����_��␳
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// ���_��␳
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y = m_posR.y;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;

		if (pos.x >= m_posV.x)
		{ // ���_�ɓ��������Ƃ�

			// ��ނ�ݒ肷��
			SetType(TYPE_NONE);
		}
	}
}

//=======================
// �J�����̃Y�[���A�E�g����
//=======================
void CCamera::ZoomOut(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	// �␳����
	Correct(m_DisDest, &m_Dis, ZOOM_DIST_CORRECT);

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// �����_��␳
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// ���_��␳
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	}
}

//=======================
// �J�����̃Y�[���C������
//=======================
void CCamera::ZoomIn(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	// �␳����
	Correct(m_DisDest, &m_Dis, ZOOM_DIST_CORRECT);

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// �����_��␳
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// ���_��␳
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	}
}

//=======================
// �X�e�[�W���e�̏���
//=======================
void CCamera::StartStage(void)
{
	// �����̕␳����
	Correct(m_DisDest, &m_Dis, STAGE_CORRECT_POS);

	// �ړI�̒����_��ݒ肷��
	m_posRDest.x = m_posR.x + STAGE_CAMERA_MOVE;

	// �ړI�̎��_��ݒ肷��
	m_posVDest.x = m_posV.x + STAGE_CAMERA_MOVE;

	// �����_��␳
	m_posR.x += (m_posRDest.x - m_posR.x) * STAGE_CORRECT_POS;
	m_posR.y += (m_posRDest.y - m_posR.y) * STAGE_CORRECT_POS;
	m_posR.z += (m_posRDest.z - m_posR.z) * STAGE_CORRECT_POS;

	// ���_��␳
	m_posV.x += (m_posVDest.x - m_posV.x) * STAGE_CORRECT_POS;
	m_posV.y += (m_posVDest.y - m_posV.y) * STAGE_CORRECT_POS;
	m_posV.z += (m_posVDest.z - m_posV.z) * STAGE_CORRECT_POS;

	if (CManager::GetFile()->GetGameInfo().GoalPos.x <= m_posR.x)
	{ // �S�[���̈ʒu��ʂ�߂����ꍇ

		// ����p�J�����ɐݒ肷��
		SetType(TYPE_STARTRUN);
	}
}

//=======================
// ����p�̏���
//=======================
void CCamera::StartRun(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;		// �v���C���[�̈ʒu
	CPlayer* pPlayer = CScene::GetPlayer();		// �v���C���[�̏��

	if (pPlayer != nullptr)
	{ // �v���C���[�̏�񂪂���ꍇ

		// �v���C���[�̈ʒu���擾����
		pos = pPlayer->GetPos();

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = pos.x + m_Dis;

		// �����_��␳����
		m_posR.x = pos.x;

		// ���_��␳����
		m_posV.x = pos.x + m_Dis;

		if (CManager::GetFile() != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			if (pos.x >= CManager::GetFile()->GetGameInfo().StartPos.x)
			{ // �ʒu���X�^�[�g�n�_���߂����ꍇ

				// �ʒu��␳����
				pos.x = 0.0f;

				// �v���C���[�̈ʒu��ݒ肷��
				pPlayer->SetPos(pos);

				// �߂�J�����ɐݒ肷��
				SetType(TYPE_STARTBACK);
			}

		}
	}
}

//=======================
// �߂�̏���
//=======================
void CCamera::StartBack(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^
	m_DisDest = CAMERA_DISTANCE;			// �ړI�̋���

	// �����̕␳����
	RotCorrect(0.0f, &m_rot.y, 0.02f);

	// �����̕␳����
	Correct(m_DisDest, &m_Dis, BACK_CORRECT_MAGNI);

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// �����_��␳
		m_posR.x += (m_posRDest.x - m_posR.x) * BACK_CORRECT_MAGNI;
		m_posR.y += (m_posRDest.y - m_posR.y) * BACK_CORRECT_MAGNI;
		m_posR.z += (m_posRDest.z - m_posR.z) * BACK_CORRECT_MAGNI;

		// ���_��␳
		m_posV.x += (m_posVDest.x - m_posV.x) * BACK_CORRECT_MAGNI;
		m_posV.y += (m_posVDest.y - m_posV.y) * BACK_CORRECT_MAGNI;
		m_posV.z += (m_posVDest.z - m_posV.z) * BACK_CORRECT_MAGNI;
	}

	if (m_Dis >= BACK_DECISION_DISTANCE)
	{ // �ړI�̋����ɋ߂Â����ꍇ

		// �ʏ�J�����ɐݒ肷��
		SetType(TYPE_NONE);

		// �Q�[���̏�Ԃ��J�E���g�_�E���ɐݒ肷��
		CGame::SetState(CGame::STATE_COUNTDOWN);

		// �J�E���g�_�E���̐�������
		CCountdown::Create();
	}
}

//=======================
// �J�����̒��n����
//=======================
void CCamera::Land(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		if (m_nSwingCount % LANDING_PROCESS_COUNT == 0)
		{ // �h��J�E���g����萔���Ƃ�

			if (m_nSwingCount % 2 == 0)
			{ // �J�E���g�������̏ꍇ

				// �ړI�̒����_��ݒ肷��
				m_posRDest.y = pos.y + (PLAYER_HEIGHT * 0.5f) - (rand() % 10 + 25);
			}
			else
			{ // �J�E���g����̏ꍇ

				// �ړI�̒����_��ݒ肷��
				m_posRDest.y = pos.y + (PLAYER_HEIGHT * 0.5f) + (rand() % 10 + 25);
			}

			// �ړI�̒����_��ݒ肷��
			m_posRDest.x = pos.x;
			m_posRDest.z = pos.z;

			// �ړI�̎��_��ݒ肷��
			m_posVDest.x = m_posRDest.x;
			m_posVDest.y = m_posRDest.y;
			m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
		}

		// �����_��␳
		m_posR.x += (m_posRDest.x - m_posR.x) * LANDING_CORRECT_POSR;
		m_posR.y += (m_posRDest.y - m_posR.y) * LANDING_CORRECT_POSR;
		m_posR.z += (m_posRDest.z - m_posR.z) * LANDING_CORRECT_POSR;

		// ���_��␳
		m_posV.x += (m_posVDest.x - m_posV.x) * LANDING_CORRECT_POSV;
		m_posV.y += (m_posVDest.y - m_posV.y) * LANDING_CORRECT_POSV;
		m_posV.z += (m_posVDest.z - m_posV.z) * LANDING_CORRECT_POSV;
	}

	// �h��J�E���g�����Z����
	m_nSwingCount++;

	if (m_nSwingCount >= LANDING_CHANGE_COUNT)
	{ // �J�E���g�������ȏ�ɂȂ����ꍇ

		// ��ނ�ݒ肷��
		SetType(TYPE_NONE);
	}
}

//=======================
// �ǃL�b�N�̏���
//=======================
void CCamera::WallKick(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		if (m_nSwingCount % LANDING_PROCESS_COUNT == 0)
		{ // �h��J�E���g����萔���Ƃ�

			if (m_nSwingCount % 2 == 0)
			{ // �J�E���g�������̏ꍇ

				// �ړI�̋�����ݒ肷��
				m_DisDest *= 0.95f;
			}
			else
			{ // �J�E���g����̏ꍇ

				// �ړI�̋�����ݒ肷��
				m_DisDest *= 1.05f;
			}

			// �ړI�̒����_��ݒ肷��
			m_posRDest.x = pos.x;
			m_posRDest.y = pos.y + (PLAYER_HEIGHT * 0.5f);
			m_posRDest.z = pos.z;

			// �ړI�̎��_��ݒ肷��
			m_posVDest.x = m_posRDest.x;
			m_posVDest.y = m_posRDest.y;
			m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
		}

		// �����̕␳����
		Correct(m_DisDest, &m_Dis, 0.1f);

		// �����_��␳
		m_posR.x += (m_posRDest.x - m_posR.x) * LANDING_CORRECT_POSR;
		m_posR.y += (m_posRDest.y - m_posR.y) * LANDING_CORRECT_POSR;
		m_posR.z += (m_posRDest.z - m_posR.z) * LANDING_CORRECT_POSR;

		// ���_��␳
		m_posV.x += (m_posVDest.x - m_posV.x) * LANDING_CORRECT_POSV;
		m_posV.y += (m_posVDest.y - m_posV.y) * LANDING_CORRECT_POSV;
		m_posV.z += (m_posVDest.z - m_posV.z) * LANDING_CORRECT_POSV;
	}

	// �h��J�E���g�����Z����
	m_nSwingCount++;

	if (m_nSwingCount >= LANDING_CHANGE_COUNT)
	{ // �J�E���g�������ȏ�ɂȂ����ꍇ

		// ��ނ�ݒ肷��
		SetType(TYPE_NONE);
	}
}

//=======================
// �u�[�X�g�J�����̏���
//=======================
void CCamera::Boost(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	// �␳����
	Correct(m_DisDest, &m_Dis, BOOST_DIST_CORRECT);

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// �����_��␳
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// ���_��␳
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;

		if (pPlayer->GetState() != CPlayer::STATE_BOOST)
		{ // �u�[�X�g��ԈȊO

			// �ʏ��Ԃɂ���
			SetType(TYPE_NONE);
		}
	}
}

//=======================
// ������J�����̐ݒ菈��
//=======================
void CCamera::SetUpward(void)
{
	// ���[�J���|�C���^��錾
	CObject* pObj = nullptr;						// �I�u�W�F�N�g�̃|�C���^
	CModel* pModel = nullptr;						// ���f���̃|�C���^
	CPlayer* pPlayer = CScene::GetPlayer();			// �v���C���[�̃|�C���^
	float fPosComp;									// ��r�p�ϐ�
	float fPosTemp = m_posV.y + 800.0f;				// �ۑ��p�ϐ�
	bool bPassing = false;							// IF����ʂ������ǂ���

	// �擪�̃I�u�W�F�N�g�̃|�C���^���擾����
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_BLOCK ||
			pObj->GetType() == CObject::TYPE_OBSTACLE ||
			pObj->GetType() == CObject::TYPE_COLLPOLY)
		{ // �u���b�N�܂��́A��Q���܂��́A�����蔻��|���S���̏ꍇ

			// ���f���^�ɃL���X�g
			pModel = dynamic_cast<CModel*>(pObj);

			if (pModel != nullptr)
			{ // �|�C���^�� NULL ����Ȃ��ꍇ

				if (pModel->GetPos().x + pModel->GetFileData().vtxMax.x >= m_posV.x &&
					pModel->GetPos().x + pModel->GetFileData().vtxMin.x <= m_posV.x &&
					pModel->GetPos().y + pModel->GetFileData().vtxMax.y <= m_posV.y)
				{ // �J�����̎��_���u���b�N�̉����ȓ����A���݂̎��_�����Ⴂ�ꍇ

					// ��r�p�ϐ��ɑ������
					fPosComp = pModel->GetPos().y + pModel->GetFileData().vtxMax.y;

					if (bPassing == false)
					{ // �ŏ��������ꍇ

						// �ۑ��ϐ��ɑ������
						fPosTemp = fPosComp;
					}
					else
					{ // ��L�ȊO

						if (pPlayer->GetPos().y - fPosComp >= pPlayer->GetPos().y - fPosTemp)
						{ // ��r�����ʒu�̕����v���C���[�ɋ߂������ꍇ

							// �ۑ��ϐ��ɑ������
							fPosTemp = fPosComp;
						}
					}
				}
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}

	// �ʒu��ݒ肷��
	m_posVDest.y = fPosTemp;
}

//=======================
// ��s�ǐՃJ�����̐ݒ菈��
//=======================
void CCamera::SetPrecedence(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	if (pPlayer != nullptr &&
		pPlayer->GetRight() == true)
	{ // �v���C���[�� NULL ����Ȃ����A�E�����������ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��␳
		m_posVDest.x = pos.x + sinf(rot.y) * PRECEDENCE_POS;
		m_posVDest.y = m_posRDest.y;
		m_posVDest.z = pos.z - m_Dis;
	}
	else
	{ // ��L�ȊO

		// �ʏ��Ԃɂ���
		m_type = TYPE_NONE;
	}
}

//=======================
// �Y�[���A�E�g�J�����̐ݒ菈��
//=======================
void CCamera::SetZoomOut(void)
{
	// ������L�΂�
	m_DisDest = ZOOMOUT_DISTANCE;
}

//=======================
// �Y�[���C���J�����̐ݒ菈��
//=======================
void CCamera::SetZoomIn(void)
{
	// ������L�΂�
	m_DisDest = ZOOMIN_DISTANCE;
}

//=======================
// �X�e�[�W���e�J�����̐ݒ菈��
//=======================
void CCamera::SetStartStage(void)
{
	if (CManager::GetFile() != nullptr)
	{ // �Q�[�����[�h���A�t�@�C���̏�񂪂������ꍇ

		// �J�����̋�����ݒ肷��
		m_DisDest = STAGE_CAMERA_DISTANCE;

		{ // �ʒu�̐ݒ菈��

			// ���[�J���ϐ��錾
			D3DXVECTOR3 posLeft = CManager::GetFile()->GetGameInfo().StageLeftPos;		// �X�e�[�W�̍��[

			// �ړI�̎��_��ݒ肷��
			m_posV = D3DXVECTOR3(posLeft.x, STAGE_CAMERA_HEIGHT, -m_DisDest);
			m_posVDest = D3DXVECTOR3(posLeft.x, STAGE_CAMERA_HEIGHT, -m_DisDest);

			// �ړI�̒����_��ݒ肷��
			m_posR = D3DXVECTOR3(m_posV.x + STAGE_CAMERA_SHIFT_WIDTH, m_posV.y, 0.0f);
			m_posRDest = D3DXVECTOR3(m_posV.x + STAGE_CAMERA_SHIFT_WIDTH, m_posV.y, 0.0f);
		}
	}
}

//=======================
// ����p�J�����̐ݒ菈��
//=======================
void CCamera::SetStartRun(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^
	m_Dis = RUN_CAMERA_DISTANCE;	// �J�����̋���
	m_rot.y = D3DX_PI * -0.5f;		// ������ݒ肷��

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �����_��ݒ肷��
		m_posR.x = pos.x;
		m_posR.y = pos.y + 20.0f;
		m_posR.z = pos.z;

		// ���_��ݒ肷��
		m_posV.x = pos.x + m_Dis;
		m_posV.y = pos.y + PLAYER_HEIGHT;
		m_posV.z = pos.z;
	}
}

//=======================
// �߂�J�����̐ݒ菈��
//=======================
void CCamera::SetStartBack(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
	}
}

//=======================
// ���n�J�����̐ݒ菈��
//=======================
void CCamera::SetLand(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��␳
		m_posVDest.x = pos.x + sinf(rot.y) * m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT;
		m_posVDest.z = pos.z - m_Dis;
	}

	// �h��J�E���g������������
	m_nSwingCount = 0;
}

//=======================
// �ǃL�b�N�̐ݒ菈��
//=======================
void CCamera::SetWallKick(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��␳
		m_posVDest.x = pos.x + sinf(rot.y) * m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT;
		m_posVDest.z = pos.z - m_Dis;
	}

	// �h��J�E���g������������
	m_nSwingCount = 0;
}

//=======================
// �u�[�X�g�J�����̐ݒ菈��
//=======================
void CCamera::SetBoost(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
	CPlayer* pPlayer = CScene::GetPlayer();	// �v���C���[�̃|�C���^
	m_DisDest = BOOST_DISTANCE;		// ������L�΂�

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		pos = pPlayer->GetPos();		// �ʒu
		rot = pPlayer->GetRot();		// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
	}
}