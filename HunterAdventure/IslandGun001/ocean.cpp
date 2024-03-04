//====================================================
//
// �C�̏��� [ocean.cpp]
// Author�F��������
//
//====================================================
//****************************************************
//	�C���N���[�h�t�@�C��
//****************************************************
#include "manager.h"
#include "ocean.h"
#include "texture.h"

//----------------------------------------------------
// �}�N����`
//----------------------------------------------------
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Lake002.png";			// �e�N�X�`���̃p�X
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, -50.0f, 0.0f);	// �ʒu
	const int OCEAN_DIVI_WIDTH = 80;		// �C�̏c�̕�����
	const int OCEAN_DIVI_DEPTH = 80;		// �C�̉��s�̕�����
	const float OCEAN_WIDTH = 80000.0f;		// �C�̕�
	const float OCEAN_DEPTH = 80000.0f;		// �C�̉��s
	const float OCEAN_WAVE_HEIGHT = 30.0f;	// �C�̍���
	const float OCEAN_CYCLE = 1.0f;			// �C�̉�]��
	const float OCEAN_SPEED = 0.01f;		// �C�̗���̑��x
}

//================================
// �R���X�g���N�^
//================================
COcean::COcean() : CMeshWave(CObject::PRIORITY_BG)
{
	// �^�C�v��ύX����
	CObject::SetType(TYPE_OCEAN);
}

//================================
// �f�X�g���N�^
//================================
COcean::~COcean()
{

}

//================================
// ����������
//================================
HRESULT COcean::Init(void)
{
	if (FAILED(CMeshWave::Init()))
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
void COcean::Uninit(void)
{
	// �I������
	CMeshWave::Uninit();
}

//================================
// �X�V����
//================================
void COcean::Update(void)
{
	// �X�V����
	CMeshWave::Update();
}

//================================
// �`�揈��
//================================
void COcean::Draw(void)
{
	// �`�揈��
	CMeshWave::Draw();
}

//================================
// �f�[�^�̐ݒ菈��
//================================
void COcean::SetData(void)
{
	SGrid grid;

	// �������ݒ肷��
	grid.nWidth = OCEAN_DIVI_WIDTH;
	grid.nHeight = OCEAN_DIVI_DEPTH;

	// ���̐ݒ菈��
	CMeshWave::SetData(POS, NONE_D3DXVECTOR3, OCEAN_WIDTH, OCEAN_DEPTH, grid, OCEAN_WAVE_HEIGHT, OCEAN_CYCLE, OCEAN_SPEED);

	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
	SetLighting(true);
	SetCulling(false);
}

//================================
// ��������
//================================
COcean* COcean::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	COcean* pOcean = nullptr;			// �C�̃C���X�^���X�𐶐�

	if (pOcean == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pOcean = new COcean;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pOcean != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		pOcean->SetData();

		// ����������
		if (FAILED(pOcean->Init()))
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

	// �C�̃|�C���^��Ԃ�
	return pOcean;
}