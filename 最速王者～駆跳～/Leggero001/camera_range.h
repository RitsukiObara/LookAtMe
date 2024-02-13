//============================================
//
// �J�����͈̔̓w�b�_�[[camera_range.h]
// Author�F��������
//
//============================================
#ifndef _CAMERA_RANGE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_RANGE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object3D.h"
#include "camera.h"

//--------------------------------------------
// �N���X(�J�����͈̓N���X)
//--------------------------------------------
class CCameraRange : public CObject3D
{
public:			// �N�ł��A�N�Z�X�ł���

	CCameraRange();				// �R���X�g���N�^
	CCameraRange(CObject::TYPE type, PRIORITY priority = PRIORITY_BG);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CCameraRange();	// �f�X�g���N�^
	void Box(void);				// �R���X�g���N�^�̔�

	// �����o�֐�
	virtual HRESULT Init(void);		// ����������
	virtual void Uninit(void);		// �I������
	virtual void Update(void);		// �X�V����
	virtual void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֐�
	void SetType(const CCamera::TYPE type);	// �J�����̎�ނ̐ݒ菈��
	CCamera::TYPE GetType(void) const;		// �J�����̎�ނ̎擾����

	// �ÓI�����o�֐�
	static CCameraRange* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CCamera::TYPE m_type;		// ���
};

#endif