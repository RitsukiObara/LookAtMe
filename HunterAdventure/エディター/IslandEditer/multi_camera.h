//===================================
//
// �}���`�J�����w�b�_�[[multi_camera.h]
// Author ��������
//
//===================================
#ifndef _MULTI_CAMERA_H_			//���̃}�N����`������Ă��Ȃ�������
#define _MULTI_CAMERA_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "camera.h"

// �}�N����`
#define MAX_CAMERA		(4)		// �J�����̍ő吔

//-----------------------------------
// �N���X��`(�}���`�J����)
//-----------------------------------
class CMultiCamera
{
public:			// �N�ł��A�N�Z�X�o����

	CMultiCamera();		// �R���X�g���N�^
	~CMultiCamera();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Set(const int nIdx);	// �ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	CCamera* GetCamera(const int nIdx);	// �J�����̎擾����

private:		// ���������A�N�Z�X�o����

	// �����o�ϐ�
	CCamera* m_aCamera[MAX_CAMERA];		// �J�����̏��
};

#endif