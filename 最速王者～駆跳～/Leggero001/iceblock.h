//===================================
//
// �X�u���b�N�w�b�_�[[iceblock.h]
// Author ��������
//
//===================================
#ifndef _ICEBLOCK_H_
#define _ICEBLOCK_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "block.h"

//-----------------------------------
// �N���X��`(�n�ʃu���b�N)
//-----------------------------------
class CIceBlock : public CBlock
{
public:			// �N�ł��A�N�Z�X�ł���

	CIceBlock();				// �R���X�g���N�^
	~CIceBlock();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

private:		// ���������A�N�Z�X�ł���

};

#endif