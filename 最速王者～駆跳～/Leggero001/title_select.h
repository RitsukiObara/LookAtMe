//============================================
//
// �^�C�g���̑I�����w�b�_�[[title_select.h]
// Author�F��������
//
//============================================
#ifndef _TITLE_SELECT_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _TITLE_SELECT_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object2D.h"

//--------------------------------------------
// �N���X��`(�^�C�g��)
//--------------------------------------------
class CTitleSelect : public CObject2D
{
public:				// �N�ł��A�N�Z�X�ł���

	CTitleSelect();			// �R���X�g���N�^
	~CTitleSelect();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �ÓI�����o�֐�
	static CTitleSelect* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	float m_fAlpha;		// �����x
	float m_fAlphaAdd;	// �����x�̉��Z��
	float m_fAlphaDest;	// �ړI�̓����x
};

#endif