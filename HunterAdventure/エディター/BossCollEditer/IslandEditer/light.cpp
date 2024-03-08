//===========================================
//
// ���C�g�̃��C������[light.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"

#include "input.h"
#include "debugproc.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const D3DXVECTOR3 LIGHT_VEC[3] =		// ���C�g�̕���
	{
		D3DXVECTOR3(0.22f, -0.87f, 0.22f),
		D3DXVECTOR3(-0.9f, -0.2f, 0.2f),
		D3DXVECTOR3(0.55f, 0.3f, 0.88f)
	};
	const D3DXCOLOR LIGHT_DIFFUSE[3] =		// ���C�g�̐F
	{
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f),
		D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f)
	};
}

//=========================
// �R���X�g���N�^
//=========================
CLight::CLight()
{
	// �S�Ă̒l���N���A����
	for (int nCntSet = 0; nCntSet < NUM_LIGHT; nCntSet++)
	{
		// ���C�g�̏��
		ZeroMemory(&m_aLight[nCntSet], sizeof(D3DLIGHT9));
	}
}

//=========================
// �f�X�g���N�^
//=========================
CLight::~CLight()
{

}

//=========================
// ���C�g�̏���������
//=========================
HRESULT CLight::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;				// �ݒ�p�����x�N�g��

	for (int nCntSet = 0; nCntSet < NUM_LIGHT; nCntSet++)
	{
		// ���C�g�̏����N���A����
		ZeroMemory(&m_aLight[nCntSet], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_aLight[nCntSet].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		m_aLight[nCntSet].Diffuse = LIGHT_DIFFUSE[nCntSet];

		// ���C�g�̕�����ݒ�
		vecDir = LIGHT_VEC[nCntSet];

		D3DXVec3Normalize(&vecDir, &vecDir);				// �x�N�g���𐳋K������

		m_aLight[nCntSet].Direction = vecDir;

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntSet, &m_aLight[nCntSet]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntSet, TRUE);
	}

	// ������Ԃ�
	return S_OK;
}

//=========================
// ���C�g�̏I������
//=========================
void CLight::Uninit(void)
{
	// ���������J������
	delete this;
}

//=========================
// ���C�g�̍X�V����
//=========================
void CLight::Update(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;				// �ݒ�p�����x�N�g��

	for (int nCntSet = 0; nCntSet < NUM_LIGHT; nCntSet++)
	{
		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntSet, &m_aLight[nCntSet]);
	}
}

//=========================
// ���C�g�̎擾����
//=========================
D3DLIGHT9 CLight::GetLightInfo(const int nIdx) const
{
	if (nIdx >= NUM_LIGHT)
	{ // ���C�g�̏���𒴂��Ă����ꍇ

		// ��~
		assert(false);

		// �����0�Ԗڂ̃��C�g��n��
		return m_aLight[0];
	}

	// ���C�g�̏���Ԃ�
	return m_aLight[nIdx];
}

//=========================
// ��������
//=========================
CLight* CLight::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CLight* pLight = nullptr;		// ���C�g�̃C���X�^���X�𐶐�

	if (pLight == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pLight = new CLight;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pLight != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pLight->Init()))
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

	// ���C�g�̃|�C���^��Ԃ�
	return pLight;
}