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
#include "camera.h"
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "useful.h"

#include "player.h"
#include "light.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------
// �J�����S��
#define ASPECT_RATIO		(80.0f)				// ����p
#define MIN_DISTANCE		(50.0f)				// �����̍ŏ��l
#define MAX_DISTANCE		(8000.0f)			// �����̍ő�l
#define DRAW_MIN_Z			(10.0f)				// Z���̍ŏ��l
#define DRAW_MAX_Z			(50000.0f)			// Z���̍ő�l

// �����֌W
#define ROT_Y_SPEED			(0.04f)				// Y���̉�]�̑��x
#define ROTATION_SPEED		(0.05f)				// ��荞�ݏ������s����̃��f���̑��x
#define ROTATION_ROT		(0.02f)				// �J�����̊p�x�̕␳�{��

// �ʒu�E�����֌W
#define POS_SPEED			(30.0f)				// �ړ����x
#define DIS_SPEED			(16.0f)				// �����̈ړ���
#define CAMERA_DISTANCE		(600.0f)			// �J�����̋���
#define CORRECT_POSR		(0.22f)				// �����_�̕␳�{��
#define CORRECT_POSV		(0.20f)				// ���_�̕␳�{��

#define POSR_SHIFT_Y		(320.0f)			// �����_�̂��炷��(Y��)
#define POSR_SHIFT			(100.0f)			// �����_�̂��炷��

//=======================
// �R���X�g���N�^
//=======================
CCamera::CCamera()
{
	// �S�Ă̏����N���A����
	m_posV = NONE_D3DXVECTOR3;						// ���_
	m_posVDest = m_posV;							// �ړI�̎��_
	m_posR = NONE_D3DXVECTOR3;						// �����_
	m_posRDest = m_posR;							// �ړI�̒����_
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// ������x�N�g��
	m_rot = NONE_D3DXVECTOR3;						// ����
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));	// �r���[�|�[�g
	m_type = TYPE_NONE;								// ���
	m_Dis = 0.0f;									// ����
	m_nSwingCount = 0;								// �h��J�����̃J�E���g
	m_bControl = false;								// �����
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
	m_viewport.X = (DWORD)0.0f;			// �`�悷���ʂ̍���X���W
	m_viewport.Y = (DWORD)0.0f;			// �`�悷���ʂ̍���Y���W
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
	// ���������J������
	delete this;
}

//=======================
// �J�����̍X�V����
//=======================
void CCamera::Update(void)
{
	D3DLIGHT9 light = CManager::Get()->GetLight()->GetCameraLight();
	D3DXVECTOR3 vecDir = NONE_D3DXVECTOR3;

	// ���C�g�̕�����ݒ�
	vecDir = m_posR - m_posV;

	// �x�N�g���𐳋K������
	D3DXVec3Normalize(&vecDir, &vecDir);

	// ���C�g�̕�����K�p����
	light.Direction = vecDir;

	// ���C�g�̏���K�p����
	CManager::Get()->GetLight()->SetCameraLight(light);

	switch (CManager::Get()->GetMode())
	{
	case CScene::MODE_GAME:		// �Q�[�����[�h

		if (CGame::IsPause() == false)
		{ // �|�[�Y���ȊO�̏ꍇ

			if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
			{ // ��SHIFT�L�[���������ꍇ

				// �}�E�X�R���g���[��
				MouseControl();
			}

			// ��ނ��Ƃ̏���
			//TypeProcess();
		}
		else
		{ // ��L�ȊO

// �f�o�b�O���[�h
#ifdef _DEBUG

// �|�[�Y���̃J��������
#if 0
			if (m_bControl == true)
			{ // �{���󋵂� true �̏ꍇ

				// ���쏈��
				Control();
			}
#endif

#endif
		}

		break;

	case CScene::MODE_RANKING:	// �����L���O

		break;

	default:					// ��L�ȊO

		// ���̃��Z�b�g����
		Reset();

		break;
	}

#if 0

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // �X�y�[�X�L�[���������ꍇ

		// �J�����̏��̃��Z�b�g����
		Reset();
	}

#endif

	//// �ړI�̒����_��ݒ肷��
	//m_posRDest.x = pos.x;
	//m_posRDest.z = pos.z;

	//// �ړI�̎��_��ݒ肷��
	//m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
	//m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

	//// �ړI�̌�����ݒ肷��
	//m_rotDest = 0.0f;
}

//=======================
// �J�����̐ݒ菈��
//=======================
void CCamera::Set(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

// �f�o�b�O�p
#ifdef _DEBUG

		// �v���W�F�N�V�����}�g���b�N�X���쐬(�������e)
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,								// �v���W�F�N�V�����}�g���b�N�X
		D3DXToRadian(ASPECT_RATIO),						// ����p
		(float)m_viewport.Width / (float)m_viewport.Height,	// ��ʂ̃A�X�y�N�g��
		DRAW_MIN_Z,										// Z�l�̍ŏ��l
		DRAW_MAX_Z										// Z�l�̍ő�l
	);

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
// �r���[�|�[�g�̐ݒ菈��
//=======================
void CCamera::SetViewport(const D3DVIEWPORT9& viewport)
{
	// �r���[�|�[�g����ݒ肷��
	m_viewport = viewport;
}

//=======================
// �r���[�|�[�g�̐ݒ菈��
//=======================
D3DVIEWPORT9 CCamera::GetViewport(void) const
{
	// �r���[�|�[�g����Ԃ�
	return m_viewport;
}

//=======================
// ��ނ̐ݒ菈��
//=======================
void CCamera::SetType(const TYPE type)
{
	// ��ނ�ݒ肷��
	m_type = type;
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
// ��������
//=======================
CCamera* CCamera::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CCamera* pCamera = nullptr;		// �J�����̃C���X�^���X�𐶐�

	if (pCamera == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pCamera = new CCamera;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pCamera != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pCamera->Init()))
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

	// �J�����̃|�C���^��Ԃ�
	return pCamera;
}

//=======================
// �J�����̏��̃��Z�b�g����
//=======================
void CCamera::Reset(void)
{
	// �S�Ă̒l�����Z�b�g����
	m_Dis = CAMERA_DISTANCE;					// ���_�ƒ����_�̋���
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -m_Dis);	// ���_�̈ʒu
	m_posVDest = m_posV;						// �ړI�̎��_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_�̈ʒu
	m_posRDest = m_posR;						// �ړI�̒����_
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_type = TYPE_NONE;							// ���
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
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_UP) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRYPress(0) > 0)
	{ // W�L�[���������ꍇ

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // D�L�[���������ꍇ

			m_posR.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			
			m_posR.z += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // A�L�[���������ꍇ

			m_posR.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;

			m_posR.z += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else
		{ // W�L�[�݂̂��������ꍇ

			m_posR.x += sinf(m_rot.y) * POS_SPEED;
			m_posV.x += sinf(m_rot.y) * POS_SPEED;

			m_posR.z += cosf(m_rot.y) * POS_SPEED;
			m_posV.z += cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_DOWN) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRYPress(0) < 0)
	{ // S�L�[���������ꍇ

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // D�L�[���������ꍇ

			m_posR.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.z += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // A�L�[���������ꍇ

			m_posR.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.z += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else
		{ // S�L�[�݂̂��������ꍇ

			m_posR.x -= sinf(m_rot.y) * POS_SPEED;
			m_posV.x -= sinf(m_rot.y) * POS_SPEED;

			m_posR.z -= cosf(m_rot.y) * POS_SPEED;
			m_posV.z -= cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
	{ // D�L�[���������ꍇ

		m_posR.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.z += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.z += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
	{ // A�L�[���������ꍇ

		m_posR.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.z += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
		m_posV.z += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
	}
}

//=======================
// �J�����̎��_���쏈��
//=======================
void CCamera::MovePosV(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_Y) == true)
	{ // Y�L�[���������ꍇ
		
		// ���_����ɓ�����
		m_posV.y += POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_N) == true)
	{ // N�L�[���������ꍇ

		// ���_�����ɓ�����
		m_posV.y -= POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_Z) == true)
	{ // Z�L�[���������ꍇ

		// ���_���E�ɉ�
		m_rot.y -= ROT_Y_SPEED;

		// �J�����̎��_���X�V����
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_C) == true)
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
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_T) == true)
	{ // T�L�[���������ꍇ

		// �����_����ɓ�����
		m_posR.y += POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_B) == true)
	{ // B�L�[���������ꍇ

		// �����_�����ɓ�����
		m_posR.y -= POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_Q) == true)
	{ // Q�L�[���������ꍇ

		// �����_���E�ɉ�
		m_rot.y -= ROT_Y_SPEED;

		// �����_���X�V����
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_E) == true)
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
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // ��SHIFT�L�[�������Ă���ꍇ

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_U) == true)
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
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_M) == true)
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
// �}�E�X�ł̃J�������쏈��
//=======================
void CCamera::MouseControl(void)
{
	// �}�E�X�ł̒����_���쏈��
	MousePosR();

	// �}�E�X�ł̎��_���쏈��
	MousePosV();

	// �}�E�X�ł̎��_�E�����_���쏈��
	MousePosMove();

	CManager::Get()->GetDebugProc()->Print("�����F%f %f %f\n", m_rot.x, m_rot.y, m_rot.z);
}

//=======================
// ���_���쏈��
//=======================
void CCamera::MousePosV(void)
{
	// �ړ��ʂ��擾����
	D3DXVECTOR3 move = CManager::Get()->GetInputMouse()->GetMove();

	if (CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_L) == true &&
		CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_R) == false)
	{ // ���N���b�N���Ă����ꍇ

		// ���_��Y�������Z����
		m_posV.y += move.y * 5.0f;

		// ���������Z����
		m_rot.y += move.x * 0.01f;

		// �����̐��K��
		useful::RotNormalize(&m_rot.y);

		// �J�����̎��_���X�V����
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
}

//=======================
// �����_���쏈��
//=======================
void CCamera::MousePosR(void)
{
	// �ړ��ʂ��擾����
	D3DXVECTOR3 move = CManager::Get()->GetInputMouse()->GetMove();

	if (CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_R) == true &&
		CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_L) == false)
	{ // �E�N���b�N���Ă����ꍇ

		// �����_��Y����ݒ肷��
		m_posR.y += move.y * -5.0f;

		// ���������Z����
		m_rot.y += move.x * 0.01f;

		// �����̐��K��
		useful::RotNormalize(&m_rot.y);

		// �J�����̒����_���X�V����
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
}

//=======================
// ���_�E���n�_���쏈��
//=======================
void CCamera::MousePosMove(void)
{
	// �ړ��ʂ��擾����
	D3DXVECTOR3 move = CManager::Get()->GetInputMouse()->GetMove();

	if (CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_L) == true &&
		CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_R) == true)
	{ // ���N���b�N���Ă����ꍇ

		// �J�����̎��_���X�V����
		m_posV.x += sinf(m_rot.y - (D3DX_PI * 0.5f)) * (move.x * 5.0f);
		m_posV.z += cosf(m_rot.y) * (move.y * 5.0f);

		// �J�����̒����_���X�V����
		m_posR.x += sinf(m_rot.y - (D3DX_PI * 0.5f)) * (move.x * 5.0f);
		m_posR.z += cosf(m_rot.y) * (move.y * 5.0f);
	}
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
// �J�����̎�ނ��Ƃ̏���
//=======================
void CCamera::TypeProcess(void)
{
	switch (m_type)
	{
	case CCamera::TYPE_NONE:		// �ʏ�

		// �ǐՏ���
		Chase();

		break;

	case CCamera::TYPE_VIBRATE:		// �U��

		// �U���J��������
		Vibrate();

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//=======================
// �J�����̒ǐՏ���
//=======================
void CCamera::Chase(void)
{
	// ���[�J���ϐ��錾
	CPlayer* pPlayer = CGame::GetPlayer();	// �v���C���[�̃|�C���^

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �v���C���[�̏����擾����
		D3DXVECTOR3 pos = pPlayer->GetPos();			// �ʒu
		D3DXVECTOR3 rot = pPlayer->GetRot();			// ����

		// �ړI�̒����_��ݒ肷��
		m_posRDest.x = pos.x + sinf(m_rot.y) * POSR_SHIFT;
		m_posRDest.y = pos.y + POSR_SHIFT_Y;
		m_posRDest.z = pos.z + cosf(m_rot.y) * POSR_SHIFT;

		// �ړI�̎��_��ݒ肷��
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * sinf(m_rot.x) * -m_Dis;
		m_posVDest.y = m_posRDest.y + cosf(m_rot.x) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * sinf(m_rot.x) * -m_Dis;

		// �����_��␳
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y += (m_posRDest.y - m_posR.y) * CORRECT_POSR;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// ���_��␳
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSV;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	}
}

//=======================
// �J�����̐U������
//=======================
void CCamera::Vibrate(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	//CPlayer* pPlayer = CPlayer::Get();	// �v���C���[�̃|�C���^

	//if (pPlayer != nullptr)
	//{ // �v���C���[�� NULL ����Ȃ��ꍇ

	//	// �v���C���[�̏����擾����
	//	pos = pPlayer->GetPos();		// �ʒu
	//	rot = pPlayer->GetRot();		// ����

	//	if (m_nSwingCount % 5 == 0)
	//	{ // �h��J�E���g����萔���Ƃ�

	//		float f = (float)(rand() % 8 + 6);

	//		if (m_nSwingCount % 2 == 0)
	//		{ // �J�E���g�������̏ꍇ

	//			// �ړI�̒����_��ݒ肷��
	//			m_posRDest.y = pos.y + POSR_SHIFT_Y - f;
	//		}
	//		else
	//		{ // �J�E���g����̏ꍇ

	//			// �ړI�̒����_��ݒ肷��
	//			m_posRDest.y = pos.y + POSR_SHIFT_Y + f;
	//		}

	//		// �ړI�̒����_��ݒ肷��
	//		m_posRDest.x = pos.x + CHASE_SHIFT_X;
	//		m_posRDest.z = pos.z;

	//		// �ړI�̎��_��ݒ肷��
	//		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
	//		m_posVDest.y = pos.y + POSV_SHIFT_Y;
	//		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
	//	}

	//	// �����_��␳
	//	m_posR.x += (m_posRDest.x - m_posR.x) * 0.3f;
	//	m_posR.y += (m_posRDest.y - m_posR.y) * 0.3f;
	//	m_posR.z += (m_posRDest.z - m_posR.z) * 0.3f;

	//	// ���_��␳
	//	m_posV.x += (m_posVDest.x - m_posV.x) * 0.3f;
	//	m_posV.y += (m_posVDest.y - m_posV.y) * 0.3f;
	//	m_posV.z += (m_posVDest.z - m_posV.z) * 0.3f;

	//	if (pPlayer->GetAbility()->GetAbility() != CAbility::ABILITY_GROUNDQUAKE)
	//	{ // �J�E���g�������ȏ�ɂȂ����ꍇ

	//		// ��ނ�ݒ肷��
	//		SetType(TYPE_NONE);

	//		// �h��J�E���g������������
	//		m_nSwingCount = 0;
	//	}
	//}

	// �h��J�E���g�����Z����
	m_nSwingCount++;
}