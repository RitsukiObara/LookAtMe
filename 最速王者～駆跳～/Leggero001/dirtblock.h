//===================================
//
// �n�ʃu���b�N�w�b�_�[[dirtblock.h]
// Author ��������
//
//===================================
#ifndef _DIRTBLOCK_H_
#define _DIRTBLOCK_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "block.h"

//-----------------------------------
// �N���X��`(�n�ʃu���b�N)
//-----------------------------------
class CDirtBlock : public CBlock
{
public:			// �N�ł��A�N�Z�X�ł���

	CDirtBlock();				// �R���X�g���N�^
	~CDirtBlock();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

private:		// ���������A�N�Z�X�ł���

};

#endif