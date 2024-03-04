//===================================
//
// ���D�w�b�_�[[balloon.h]
// Author ��������
//
//===================================
#ifndef _BALLOON_H_
#define _BALLOON_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CBalloonRope;		// ���D�̕R

//-----------------------------------
// �N���X��`(���D)
//-----------------------------------
class CBalloon : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CBalloon();			// �R���X�g���N�^
	~CBalloon();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBalloon* Create(const D3DXVECTOR3& pos);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CBalloonRope* m_pRope;	// �R�̏��
	float m_fPosYInit;		// �����̍���
	float m_fHeightRot;		// ���������߂����
};

#endif