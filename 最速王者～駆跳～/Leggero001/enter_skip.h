//============================================
//
// ENTER�X�L�b�v�w�b�_�[[enter_skip.h]
// Author�F��������
//
//============================================
#ifndef _ENTER_SKIP_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _ENTER_SKIP_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object2D.h"

//--------------------------------------------
// �N���X��`(�X�L�b�v�\��)
//--------------------------------------------
class CEnterSkip : public CObject2D
{
public:				// �N�ł��A�N�Z�X�ł���

	CEnterSkip();			// �R���X�g���N�^
	~CEnterSkip();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CEnterSkip* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

};

#endif