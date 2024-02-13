//============================================
//
// �J�����͈̔̓w�b�_�[[camera_range.h]
// Author�F��������
//
//============================================
#ifndef _CAMERAEDIT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _CAMERAEDIT_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "camera_range.h"
#include "camera.h"

//--------------------------------------------
// �N���X(�J�����͈̓N���X)
//--------------------------------------------
class CCameraEdit : public CCameraRange
{
public:			// �N�ł��A�N�Z�X�ł���

	CCameraEdit();				// �R���X�g���N�^
	CCameraEdit(CObject::TYPE type, PRIORITY priority = PRIORITY_BG);		// �I�[�o�[���[�h�R���X�g���N�^
	~CCameraEdit();	// �f�X�g���N�^
	void Box(void);				// �R���X�g���N�^�̔�

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �ÓI�����o�֐�
	static CCameraEdit* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void TypeChange(void);		// ��ޕύX����
	void Move(void);			// �ړ�����
	void ScaleX(void);			// �g��k������(X��)
	void ScaleY(void);			// �g��k������(Y��)
	void Scale(void);			// �g��k������
	void Set(void);				// �ݒ菈��
	void Delete(void);			// ��������
	void PlayerPosSet(void);	// �v���C���[�ʒu�ړ�����

	// �ÓI�����o�ϐ�
	static const char* c_apTypeName[CCamera::TYPE_STARTSTAGE];		// ��ނ̖��O
};

#endif