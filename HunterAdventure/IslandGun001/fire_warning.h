//===================================
//
// �����Ӄw�b�_�[[fire_warning.h]
// Author ��������
//
//===================================
#ifndef _FIRE_WARNING_H_
#define _FIRE_WARNING_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object3Dfan.h"

//--------------------------------------------
// �N���X(�����ӃN���X)
//--------------------------------------------
class CFireWarning : public CObject3DFan
{
public:			// �N�ł��A�N�Z�X�ł���

	CFireWarning();		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CFireWarning();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CFireWarning* Create(const D3DXVECTOR3& pos);	// ��������

private:

	// �����o�ϐ�
	D3DXCOLOR m_col;		// �F
	int m_nCount;			// �o�߃J�E���g
};

#endif