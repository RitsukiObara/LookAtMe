//===================================
//
// �������o�w�b�_�[[sonic.h]
// Author ��������
//
//===================================
#ifndef _SONIC_H_
#define _SONIC_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "scrollobject.h"

//-----------------------------------
// �N���X��`(CSonic)
//-----------------------------------
class CSonic : public CScroll
{
public:			// �N�ł��A�N�Z�X�ł���

	CSonic();				// �R���X�g���N�^
	~CSonic();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �ÓI�����o�֐�
	static CSonic* Create(void);		// ��������

private:		// ���������A�N�Z�X�ł���

};

#endif