//===================================
//
// ��u���b�N�w�b�_�[[rockblock.h]
// Author ��������
//
//===================================
#ifndef _ROCKBLOCK_H_
#define _ROCKBLOCK_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "block.h"

//-----------------------------------
// �N���X��`(�n�ʃu���b�N)
//-----------------------------------
class CRockBlock : public CBlock
{
public:			// �N�ł��A�N�Z�X�ł���

	CRockBlock();				// �R���X�g���N�^
	~CRockBlock();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

private:		// ���������A�N�Z�X�ł���

};

#endif