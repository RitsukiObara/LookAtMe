//============================================
//
// �����_���[�̃��C������[renderer.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "object.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "fade.h"

//=========================================
// �R���X�g���N�^
//=========================================
CRenderer::CRenderer()
{
	// �S�Ă̒l���N���A����
	m_pD3D = nullptr;			// �I�u�W�F�N�g�ւ̃|�C���^
	m_pD3DDevice = nullptr;	// �f�o�C�X�ւ̃|�C���^
}

//=========================================
// �f�X�g���N�^
//=========================================
CRenderer::~CRenderer()
{

}

//=========================================
//����������
//=========================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;										//�f�B�X�v���C���[�h

	D3DPRESENT_PARAMETERS d3dpp;								//�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{ // Direct3D�I�u�W�F�N�g�̐����Ɏ��s�����ꍇ

		// �x����
		MessageBox(NULL, "Direct3D�I�u�W�F�N�g�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		// �x����
		MessageBox(NULL, "�f�B�X�v���C���[�h�̎擾�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			//�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		//�`�揈�����n�[�h�E�F�A�ōs���A���_������CPU�ōs��
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,	//�`�揈���ƒ��_������CPU�ōs��
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				// �x����
				MessageBox(NULL, "Direct3D�f�o�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// �J�����O�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ� (�e�N�X�`���̊g�k��Ԃ̐ݒ�)
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ� (�e�N�X�`���̃A���t�@�u�����h�̐ݒ�)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ������Ԃ�
	return S_OK;
}

//=============================================
//�I������
//=============================================
void CRenderer::Uninit(void)
{
	//Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();

		m_pD3DDevice = nullptr;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();

		m_pD3D = nullptr;
	}

	// ���������J������
	delete this;
}

//======================================
//�X�V����
//======================================
void CRenderer::Update(void)
{
	// FPS�\��
	CManager::Get()->GetDebugProc()->Print("FPS:%d\n", GetFPS());

	// �S�Ă̍X�V����
	CObject::UpdateAll();

	// Rendering
	ImGui::EndFrame();
}

//======================================
//�`�揈��
//======================================
void CRenderer::Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		// ���̐ݒ菈��
		SetFog(false, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), 4000.0f, 50000.0f);

		// �S�Ă̕`�揈��
		CObject::DrawAll();

		if (CManager::Get()->GetScene() != nullptr)
		{ // �V�[���� NULL ����Ȃ��ꍇ

			// �V�[���̕`�揈��
			CManager::Get()->GetScene()->Draw();
		}

		if (CManager::Get()->GetFade() != nullptr)
		{ // �t�F�[�h�� NULL ����Ȃ��ꍇ

			// �t�F�[�h�̕`��
			CManager::Get()->GetFade()->Draw();
		}

		if (CManager::Get()->GetDebugProc() != nullptr)
		{ // �f�o�b�O�\���� NULL ����Ȃ��ꍇ

			// �f�o�b�O�\���̕`��
			CManager::Get()->GetDebugProc()->Draw();
		}

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		//�`��I��
		m_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//===========================================
// ��������
//===========================================
CRenderer* CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CRenderer* pRenderer = nullptr;		// �����_���[�̃C���X�^���X�𐶐�

	if (pRenderer == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pRenderer = new CRenderer;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRenderer != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pRenderer->Init(hWnd, bWindow)))
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

	// �����_���[�̃|�C���^��Ԃ�
	return pRenderer;
}

//===========================================
//�f�o�C�X�̎擾
//===========================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CRenderer::SetFog(const bool bFog, const D3DXCOLOR& col, const float fStartPos, const float fEndPos)
{
	//�t�H�O�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, bFog);	//�t�H�O�FON
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, col);		//���F�ŕs����
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);			//���_���[�h
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);		//�e�[�u�����[�h
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fStartPos));		//�J�n�ʒu
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fEndPos));			//�I���ʒu
}