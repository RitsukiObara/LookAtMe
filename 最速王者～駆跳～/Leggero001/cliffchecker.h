//============================================
//
// �R�߂܂�w�b�_�[[cliffchecker.h]
// Author�F��������
//
//============================================
#ifndef _CLIFFCHECKER_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _CLIFFCHECKER_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object3D.h"

//--------------------------------------------
// �N���X(�w�i�N���X)
//--------------------------------------------
class CCliffChecker : public CObject3D
{
public:			// �N�ł��A�N�Z�X�ł���

	CCliffChecker();		// �R���X�g���N�^
	~CCliffChecker();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);			// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CCliffChecker* Create(const D3DXVECTOR3& pos);	// ��������

private:		// ���������A�N�Z�X�ł���

};

#endif