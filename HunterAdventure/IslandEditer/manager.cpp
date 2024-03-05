//============================================
//
// �}�l�[�W���[�̃��C������[manager.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "file.h"
#include "debugproc.h"
#include "texture.h"
#include "objectX.h"
#include "camera.h"
#include "light.h"
#include "fade.h"

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
CManager* CManager::m_pManager = nullptr;			// �}�l�[�W���[�̏��

//=========================================
// �R���X�g���N�^
//=========================================
CManager::CManager()
{

}

//=========================================
// �f�X�g���N�^
//=========================================
CManager::~CManager()
{

}

//=========================================
//����������
//=========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer == nullptr)
	{ // �����_���[�ւ̃|�C���^�� NULL �̏ꍇ

		// �����_���[�̐�������
		m_pRenderer = CRenderer::Create(hWnd, bWindow);
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pFile == nullptr)
	{ // �t�@�C���ւ̃|�C���^�� nullptr �̏ꍇ

		// �t�@�C���̐�������
		m_pFile = CFile::Create();
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pDebugProc == nullptr)
	{ // �f�o�b�O�\���ւ̃|�C���^�� nullptr �̏ꍇ

		// �f�o�b�O�\���̐�������
		m_pDebugProc = CDebugProc::Create();
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pInputKeyboard == nullptr)
	{ // �L�[�{�[�h�ւ̃|�C���^�� NULL �̏ꍇ

		// �L�[�{�[�h�̐�������
		m_pInputKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pInputGamePad == nullptr)
	{ // �Q�[���p�b�h�ւ̃|�C���^�� NULL �̏ꍇ

		// �Q�[���p�b�h�̐�������
		m_pInputGamePad = CInputGamePad::Create(hInstance);
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pInputMouse == nullptr)
	{ // �}�E�X�ւ̃|�C���^�� NULL �̏ꍇ

		// �}�E�X�̐�������
		m_pInputMouse = CInputMouse::Create(hInstance, hWnd);
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pSound == nullptr)
	{ // �T�E���h�ւ̃|�C���^�� NULL �̏ꍇ

		// �T�E���h�̐�������
		m_pSound = CSound::Create(hWnd);
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pCamera == nullptr)
	{ // �J�����ւ̃|�C���^�� NULL �̏ꍇ

		// �J�����̐�������
		m_pCamera = CCamera::Create();
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pLight == nullptr)
	{ // ���C�g�ւ̃|�C���^�� NULL �̏ꍇ

		// ���C�g�̐�������
		m_pLight = CLight::Create();
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pTexture == nullptr)
	{ // �e�N�X�`���ւ̃|�C���^�� NULL �̏ꍇ

		// �e�N�X�`���̐�������
		m_pTexture = CTexture::Create();
	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	if (m_pXFile == nullptr)
	{ // X�t�@�C���ւ̃|�C���^�� NULL �̏ꍇ

		// X�t�@�C���̐�������
		m_pXFile = CXFile::Create();
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}

	// �e�N�X�`���̓ǂݍ��ݏ���
	if (FAILED(TexLoad()))
	{ // �ǂݍ��݂Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	if (m_pFade == nullptr)
	{ // �t�F�[�h�ւ̃|�C���^�� nullptr �̏ꍇ

#ifdef _DEBUG

		// �t�F�[�h�̃��������m�ۂ���
		m_pFade = m_pFade->Create(CScene::MODE_GAME);
#else

		// �V�[���̃��������m�ۂ���
		m_pFade = m_pFade->Create(CScene::MODE_GAME);
#endif 

	}
	else
	{ // �|�C���^���g���Ă����ꍇ

		// ��~
		assert(false);
	}

	// ������Ԃ�
	return S_OK;
}

//=============================================
//�I������
//=============================================
void CManager::Uninit(void)
{
	if (m_pSound != nullptr)
	{ // �T�E���h�̃��������m�ۂ���Ă����ꍇ

		// �T�E���h�̒�~
		m_pSound->Stop();
	}

	// �e�N�X�`���̔j������
	TexUnload();

	if (m_pRenderer != nullptr)
	{ // �����_���[�� NULL ����Ȃ��ꍇ

		// �I������
		m_pRenderer->Uninit();
		m_pRenderer = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{ // �L�[�{�[�h�� NULL ����Ȃ��ꍇ

		// �I������
		m_pInputKeyboard->Uninit();
		m_pInputKeyboard = nullptr;
	}

	if (m_pInputGamePad != nullptr)
	{ // �Q�[���p�b�h�� NULL ����Ȃ��ꍇ

		// �I������
		m_pInputGamePad->Uninit();
		m_pInputGamePad = nullptr;
	}

	if (m_pInputMouse != nullptr)
	{ // �}�E�X�� NULL ����Ȃ��ꍇ

		// �I������
		m_pInputMouse->Uninit();
		m_pInputMouse = nullptr;
	}

	if (m_pSound != nullptr)
	{ // �T�E���h�� NULL ����Ȃ��ꍇ

		// �I������
		m_pSound->Uninit();
		m_pSound = nullptr;
	}

	if (m_pFile != nullptr)
	{ // �t�@�C���� NULL ����Ȃ��ꍇ

		// �I������
		m_pFile->Uninit();
		m_pFile = nullptr;
	}

	if (m_pDebugProc != nullptr)
	{ // �f�o�b�O�\���� NULL ����Ȃ��ꍇ

		// �I������
		m_pDebugProc->Uninit();
		m_pDebugProc = nullptr;
	}

	if (m_pCamera != nullptr)
	{ // �J������ NULL ����Ȃ��ꍇ

		// �I������
		m_pCamera->Uninit();
		m_pCamera = nullptr;
	}

	if (m_pLight != nullptr)
	{ // ���C�g�� NULL ����Ȃ��ꍇ

		// �I������
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	if (m_pTexture != nullptr)
	{ // �e�N�X�`���� NULL ����Ȃ��ꍇ

		// �e�N�X�`���̔j������
		m_pTexture->Unload();
		m_pTexture = nullptr;
	}

	if (m_pXFile != nullptr)
	{ // X�t�@�C���� NULL ����Ȃ��ꍇ

		// X�t�@�C���̔j������
		m_pXFile->Unload();
		m_pXFile = nullptr;
	}

	if (m_pFade != nullptr)
	{ // �t�F�[�h�� NULL ����Ȃ��ꍇ

		// �t�F�[�h�̔j������
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	if (m_pScene != nullptr)
	{ // �V�[���� NULL ����Ȃ��ꍇ

		// �V�[����j������
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	// �}�l�[�W���[�̃��������������
	delete m_pManager;
	m_pManager = nullptr;
}

//======================================
//�X�V����
//======================================
void CManager::Update(void)
{
	if (m_pFade != nullptr)
	{ // �t�F�[�h�� NULL ����Ȃ��ꍇ

		// �t�F�[�h�̍X�V
		m_pFade->Update();
	}

	if (m_pInputKeyboard != nullptr)
	{ // �L�[�{�[�h�� NULL ����Ȃ��ꍇ

		// �L�[�{�[�h�̍X�V
		m_pInputKeyboard->Update();
	}

	if (m_pInputGamePad != nullptr)
	{ // �Q�[���p�b�h�� NULL ����Ȃ��ꍇ

		// �Q�[���p�b�h�̍X�V
		m_pInputGamePad->Update();
	}

	if (m_pInputMouse != nullptr)
	{ // �}�E�X�� NULL ����Ȃ��ꍇ

		// �}�E�X�̍X�V
		m_pInputMouse->Update();
	}

	if (m_pCamera != nullptr)
	{ // �J������ NULL ����Ȃ��ꍇ

		// �J�����̍X�V
		m_pCamera->Update();
	}

	if (m_pLight != nullptr)
	{ // ���C�g�� NULL ����Ȃ��ꍇ

		// ���C�g�̍X�V
		m_pLight->Update();
	}

#ifdef _DEBUG		// �f�o�b�O�p

	if (m_pDebugProc != nullptr)
	{ // �f�o�b�O�\���� NULL ����Ȃ��ꍇ

		// �f�o�b�O�\���̍X�V
		m_pDebugProc->Update();
	}

	if (m_pInputKeyboard->GetTrigger(DIK_F6) == true)
	{ // F6�L�[���������ꍇ

		// ���C���[�t���[����ON/OFF
		m_bWire = m_bWire ? false : true;

		if (m_bWire == true)
		{ // ���C���[���[�hON�̏ꍇ

			// ���C���[�t���[����ON�ɂ���
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{ // ���C���[���[�hOFF�̏ꍇ

			// ���C���[�t���[����OFF�ɂ���
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}

#endif

	if (m_pScene != nullptr)
	{ // �V�[���� NULL ����Ȃ��ꍇ

		// �V�[���̍X�V����
		m_pScene->Update();
	}

	//if (m_pRenderer != nullptr)
	//{ // �����_���[�� NULL ����Ȃ��ꍇ

	//	// �X�V����
	//	m_pRenderer->Update();
	//}

	m_pDebugProc->Print("���[�h�F%d", m_pScene->GetMode());
}

//======================================
//�`�揈��
//======================================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{ // �����_���[�� NULL ����Ȃ��ꍇ

		// �`�揈��
		m_pRenderer->Draw();
	}
}

//======================================
// �e�N�X�`���̓ǂݍ��ݏ���
//======================================
HRESULT CManager::TexLoad(void)
{
	// X�t�@�C���̃��[�h����
	m_pXFile->Load();

	// �e�N�X�`���̃��[�h����
	m_pTexture->Load();

	// ������Ԃ�
	return S_OK;
}

//======================================
// �e�N�X�`���̔j������
//======================================
void CManager::TexUnload(void)
{
	// �S�ẴI�u�W�F�N�g�̔j������
	CObject::ReleaseAll();
}

//======================================
// �����_���[�̎擾����
//======================================
CRenderer* CManager::GetRenderer(void)
{
	// �����_���[�̏���Ԃ�
	return m_pRenderer;
}

//======================================
// �L�[�{�[�h�̎擾����
//======================================
CInputKeyboard* CManager::GetInputKeyboard(void)
{
	// �L�[�{�[�h�̏���Ԃ�
	return m_pInputKeyboard;
}

//======================================
// �Q�[���p�b�h�̎擾����
//======================================
CInputGamePad* CManager::GetInputGamePad(void)
{
	// �Q�[���p�b�h�̏���Ԃ�
	return m_pInputGamePad;
}

//======================================
// �}�E�X�̎擾����
//======================================
CInputMouse* CManager::GetInputMouse(void)
{
	// �}�E�X�̏���Ԃ�
	return m_pInputMouse;
}

//======================================
// �T�E���h�̎擾����
//======================================
CSound* CManager::GetSound(void)
{
	// �T�E���h�̏���Ԃ�
	return m_pSound;
}

//======================================
// �t�@�C���̎擾����
//======================================
CFile* CManager::GetFile(void)
{
	// �t�@�C���̏���Ԃ�
	return m_pFile;
}

//======================================
// �f�o�b�O�\���̎擾����
//======================================
CDebugProc* CManager::GetDebugProc(void)
{
	// �f�o�b�O�\���̏���Ԃ�
	return m_pDebugProc;
}

//======================================
// �J�����\���̎擾����
//======================================
CCamera* CManager::GetCamera(void)
{
	// �J�����̏���Ԃ�
	return m_pCamera;
}

//======================================
// ���C�g�̎擾����
//======================================
CLight* CManager::GetLight(void)
{
	// ���C�g�̏���Ԃ�
	return m_pLight;
}

//======================================
// �e�N�X�`���̎擾����
//======================================
CTexture* CManager::GetTexture(void)
{
	// �e�N�X�`���̏���Ԃ�
	return m_pTexture;
}

//======================================
// �V�[���̎擾����
//======================================
CScene* CManager::GetScene(void)
{
	// �V�[���̏���Ԃ�
	return m_pScene;
}

//======================================
// �t�F�[�h�̎擾����
//======================================
CFade* CManager::GetFade(void)
{
	// �t�F�[�h�̏���Ԃ�
	return m_pFade;
}

//======================================
// ���[�h�̐ݒ菈��
//======================================
void CManager::SetMode(const CScene::MODE mode)
{
	if (m_pSound != nullptr)
	{ // �T�E���h�� NULL ����Ȃ��ꍇ

		// �T�E���h�̒�~����
		m_pSound->Stop();
	}

	// �S�ẴI�u�W�F�N�g�̔j������
	CObject::ReleaseAll();

	if (m_pScene != nullptr)
	{ // �V�[���� NULL ����Ȃ��ꍇ

		// �V�[������������
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	if (m_pScene == nullptr)
	{ // �V�[���� NULL ����Ȃ��ꍇ

		// ��������
		m_pScene = m_pScene->Create(mode);
	}
}

//======================================
// ���[�h�̎擾����
//======================================
CScene::MODE CManager::GetMode(void)
{
	// ���[�h��Ԃ�
	return m_pScene->GetMode();
}

//======================================
// X�t�@�C���̎擾����
//======================================
CXFile* CManager::GetXFile(void)
{
	// X�t�@�C���̏���Ԃ�
	return m_pXFile;
}

//======================================
// �}�l�[�W���[�̐�������
//======================================
CManager* CManager::Create(void)
{
	if (m_pManager == nullptr)
	{ // �}�l�[�W���[�� NULL �̏ꍇ

		// �}�l�[�W���[�𐶐�����
		m_pManager = new CManager;

		// �}�l�[�W���[�̏���Ԃ�
		return m_pManager;
	}
	else
	{ // ��L�ȊO

		// �}�l�[�W���[�̏���Ԃ�
		return m_pManager;
	}
}

//======================================
// �}�l�[�W���[�̎擾����
//======================================
CManager* CManager::Get(void)
{
	if (m_pManager != nullptr)
	{ // �}�l�[�W���[�� NULL ����Ȃ��ꍇ

		// �}�l�[�W���[�̏���Ԃ�
		return m_pManager;
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);

		// �}�l�[�W���[�𐶐�����
		return CManager::Create();
	}
}