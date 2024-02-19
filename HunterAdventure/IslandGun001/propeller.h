//===================================
//
// �v���y���w�b�_�[[propeller.h]
// Author ��������
//
//===================================
#ifndef _PROPELLER_H_
#define _PROPELLER_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �N���X��`(�v���y��)
//-----------------------------------
class CPropeller : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CPropeller();			// �R���X�g���N�^
	~CPropeller();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(D3DXMATRIX* pMtx);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CPropeller* Create(D3DXMATRIX* pMtx);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXMATRIX* m_pMtxParent;		// �e�̃}�g���b�N�X
};

#endif