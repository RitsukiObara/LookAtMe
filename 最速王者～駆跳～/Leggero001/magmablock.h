//===================================
//
// �}�O�}�u���b�N�w�b�_�[[magmablock.h]
// Author ��������
//
//===================================
#ifndef _MAGMABLOCK_H_
#define _MAGMABLOCK_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "block.h"

//-----------------------------------
// �N���X��`(�n�ʃu���b�N)
//-----------------------------------
class CMagmaBlock : public CBlock
{
public:			// �N�ł��A�N�Z�X�ł���

	CMagmaBlock();				// �R���X�g���N�^
	~CMagmaBlock();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

private:		// ���������A�N�Z�X�ł���

};

#endif