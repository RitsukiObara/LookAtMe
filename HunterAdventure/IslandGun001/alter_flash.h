//===================================
//
// �Ւd�M���w�b�_�[[alter_flash.h]
// Author ��������
//
//===================================
#ifndef _ALTER_FLASH_H_
#define _ALTER_FLASH_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object2D.h"

//-----------------------------------
// �N���X��`(CAlterFlash)
//-----------------------------------
class CAlterFlash : public CObject2D
{
public:			// �N�ł��A�N�Z�X�ł���

	CAlterFlash();				// �R���X�g���N�^
	~CAlterFlash();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(void);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CAlterFlash* Create(void);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	float m_fAlpha;			// �����x
};

#endif