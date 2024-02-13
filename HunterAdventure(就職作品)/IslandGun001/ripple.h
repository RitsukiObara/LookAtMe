//===================================
//
// �g��w�b�_�[[ripple.h]
// Author�F��������
//
//===================================
#ifndef _RIPPLE_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _RIPPLE_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �N���X��`(���n�̔g��)
//-----------------------------------
class CRipple : public CModel
{
public:		// �N�ł��A�N�Z�X�ł���

	CRipple(const CObject::TYPE type = TYPE_RIPPLE, const CObject::PRIORITY priority = PRIORITY_PLAYER);	// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CRipple();		// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	virtual void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);			// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CRipple* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);		// ��������

private:	// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	float m_fAlpha;								// �����x
};

#endif