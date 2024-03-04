//===================================
//
// ���D�̕R�w�b�_�[[balloon_rope.h]
// Author ��������
//
//===================================
#ifndef _BALLOON_ROPE_H_
#define _BALLOON_ROPE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �N���X��`(���D�̕R)
//-----------------------------------
class CBalloonRope : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CBalloonRope();			// �R���X�g���N�^
	~CBalloonRope();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(D3DXMATRIX* mtx);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBalloonRope* Create(D3DXMATRIX* mtx);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXMATRIX* m_pMtxParent;		// �e�̃}�g���b�N�X
};

#endif