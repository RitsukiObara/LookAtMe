//===================================
//
// ���C�g�w�b�_�[[light.h]
// Author ��������
//
//===================================
#ifndef _LIGHT_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _LIGHT_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"

//-----------------------------------
// �}�N����`
//-----------------------------------
#define NUM_LIGHT			(3)			// ���C�g�̐�

//-----------------------------------
// �N���X��`
//-----------------------------------
class CLight
{
public:			// �N�ł��A�N�Z�X�ł���

	CLight();			// �R���X�g���N�^
	~CLight();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����

	D3DLIGHT9 GetLightInfo(const int nIdx) const;		// ���C�g�̎擾����

	D3DLIGHT9 GetCameraLight(void) const;				// �J�������C�g�̎擾����
	void SetCameraLight(const D3DLIGHT9& light);		// �J�������C�g�̐ݒ菈��

	// �ÓI�����o�֐�
	static CLight* Create(void);		// ��������

private:		// �N�ł��A�N�Z�X�ł���

	// �����o�ϐ�
	D3DLIGHT9 m_aLight[NUM_LIGHT];		// ���C�g�̏��
	D3DLIGHT9 m_lightCamera;			// �J�������C�g�̏��
};

#endif