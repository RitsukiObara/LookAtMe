//====================================================
//
// �X�J�C�{�b�N�X���� [skybox.cpp]
// Author�F��������
//
//====================================================
//****************************************************
//	�C���N���[�h�t�@�C��
//****************************************************
#include "manager.h"
#include "skybox.h"
#include "texture.h"

//----------------------------------------------------
// �}�N����`
//----------------------------------------------------
#define TEXTURE				"data\\TEXTURE\\Platform\\NoonSky.jpg"				// �e�N�X�`���̃p�X
#define SKYBOX_POS			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				// �ʒu
#define SKYBOX_ROT			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				// ����
#define SKYBOX_WIDTH		(30000.0f)									// ����
#define SKYBOX_HEIGHT		(30000.0f)									// �c��
#define SKYBOX_DIVI_CIRCUM	(16)										// �~���̕�����
#define SKYBOX_DIVI_HEIGHT	(16)										// �c�̕�����
#define SKYBOX_ROTATION		(0.001f)									// �񂷑��x

//================================
// �R���X�g���N�^
//================================
CSkyBox::CSkyBox() : CMeshDome(CObject::PRIORITY_BG)
{
	// �^�C�v��ύX����
	CObject::SetType(TYPE_SKYBOX);
}

//================================
// �f�X�g���N�^
//================================
CSkyBox::~CSkyBox()
{

}

//================================
// ����������
//================================
HRESULT CSkyBox::Init(void)
{
	if (FAILED(CMeshDome::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//================================
// �I������
//================================
void CSkyBox::Uninit(void)
{
	// �I������
	CMeshDome::Uninit();
}

//================================
// �X�V����
//================================
void CSkyBox::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// ����

	// ��������
	rot.y += SKYBOX_ROTATION;

	// �����̐ݒ菈��
	SetRot(rot);
}

//================================
// �`�揈��
//================================
void CSkyBox::Draw(void)
{
	// �`�揈��
	CMeshDome::Draw();
}

//================================
// ��������
//================================
CSkyBox* CSkyBox::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CSkyBox* pSkyBox = nullptr;			// �I�u�W�F�N�g3D�̃C���X�^���X�𐶐�

	if (pSkyBox == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pSkyBox = new CSkyBox;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pSkyBox != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		SGrid grid;

		// �������ݒ肷��
		grid.nWidth = SKYBOX_DIVI_CIRCUM;
		grid.nHeight = SKYBOX_DIVI_HEIGHT;

		// �h�[���̐ݒ菈��
		pSkyBox->SetData(SKYBOX_POS, SKYBOX_ROT, SKYBOX_WIDTH, SKYBOX_HEIGHT, grid);

		pSkyBox->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
		pSkyBox->SetLighting(true);
		pSkyBox->SetCulling(false);

		// ����������
		if (FAILED(pSkyBox->Init()))
		{ // �����������Ɏ��s�����ꍇ

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �X�J�C�{�b�N�X�̃|�C���^��Ԃ�
	return pSkyBox;
}