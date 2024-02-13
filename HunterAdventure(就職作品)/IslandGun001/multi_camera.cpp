//===========================================
//
// �}���`�J�����̃��C������[multi_camera.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "multi_camera.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const float VIEWPORT_LEFTUP_X[MAX_CAMERA] =			// �J�����̍���X���W
	{
		0.0f,
		SCREEN_WIDTH * 0.5f,
		0.0f,
		SCREEN_WIDTH * 0.5f
	};
	const float VIEWPORT_LEFTUP_Y[MAX_CAMERA] =			// �J�����̍���Y���W
	{
		0.0f,
		0.0f,
		SCREEN_HEIGHT * 0.5f,
		SCREEN_HEIGHT * 0.5f
	};
	const int CAMERA_WIDTH = (int)(SCREEN_WIDTH * 0.5f);		// �J�����̕�
	const int CAMERA_HEIGHT = (int)(SCREEN_HEIGHT * 0.5f);		// �J�����̍���
}

//=======================
// �R���X�g���N�^
//=======================
CMultiCamera::CMultiCamera()
{
	// �S�Ă̏����N���A����
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		m_aCamera[nCnt] = nullptr;		// �J�����̏��
	}
}

//=======================
// �f�X�g���N�^
//=======================
CMultiCamera::~CMultiCamera()
{

}

//=======================
// �J�����̏���������
//=======================
HRESULT CMultiCamera::Init(void)
{
	// �r���[�|�[�g�̕ϐ���錾
	D3DVIEWPORT9 viewport;

	// �S�Ă̏����N���A����
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_aCamera[nCnt] == nullptr)
		{ // �J������ NULL ����Ȃ��ꍇ

			// �J�����𐶐�����
			m_aCamera[nCnt] = new CCamera;

			// ����������
			m_aCamera[nCnt]->Init();

			// �r���[�|�[�g�̐ݒ菈��
			viewport.X = (DWORD)VIEWPORT_LEFTUP_X[nCnt];	// �`�悷���ʂ̍���X���W
			viewport.Y = (DWORD)VIEWPORT_LEFTUP_Y[nCnt];	// �`�悷���ʂ̍���Y���W
			viewport.Width = CAMERA_WIDTH;					// �`�悷���ʂ̕�
			viewport.Height = CAMERA_HEIGHT;				// �`�悷���ʂ̍���
			viewport.MinZ = 0.0f;							// �N���b�v�{�����[���̍ŏ��l
			viewport.MaxZ = 1.0f;							// �N���b�v�{�����[���̍ő�l

			// �r���[�|�[�g�̏���ݒ肷��
			m_aCamera[nCnt]->SetViewport(viewport);
		}
	}

	// ������Ԃ�
	return S_OK;
}

//=======================
// �J�����̏I������
//=======================
void CMultiCamera::Uninit(void)
{
	// �S�Ă̏����N���A����
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_aCamera[nCnt] != nullptr)
		{ // �J������ NULL ����Ȃ��ꍇ

			// �I������
			m_aCamera[nCnt]->Uninit();

			// �J�����̃��������������
			delete m_aCamera[nCnt];
			m_aCamera[nCnt] = nullptr;
		}
	}
}

//=======================
// �J�����̍X�V����
//=======================
void CMultiCamera::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_aCamera[nCnt] != nullptr)
		{ // �J������ NULL ����Ȃ��ꍇ

			// �J�����̍X�V����
			m_aCamera[nCnt]->Update();
		}
	}
}

//=======================
// �ݒ菈��
//=======================
void CMultiCamera::Set(const int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DVIEWPORT9 viewport = m_aCamera[nIdx]->GetViewport();			// �r���[�|�[�g�����擾

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&viewport);

	// �ݒ菈��
	m_aCamera[nIdx]->Set();
}

//=======================
// �J�����̎擾����
//=======================
CCamera* CMultiCamera::GetCamera(const int nIdx)
{
	// �J�����̏���Ԃ�
	return m_aCamera[nIdx];
}