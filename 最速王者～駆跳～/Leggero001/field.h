//===================================
//
// �t�B�[���h�w�b�_�[[field.h]
// Author ��������
//
//===================================
#ifndef _FIELD_H_
#define _FIELD_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object3D.h"

//--------------------------------------------
// �N���X(�t�B�[���h�N���X)
//--------------------------------------------
class CField : public CObject3D
{
public:			// �N�ł��A�N�Z�X�ł���

	CField();				// �R���X�g���N�^
	~CField();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);			// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CField* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	// ��������

private:

};

#endif