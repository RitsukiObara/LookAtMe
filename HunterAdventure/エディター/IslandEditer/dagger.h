//===================================
//
// �_�K�[�w�b�_�[[dagger.h]
// Author ��������
//
//===================================
#ifndef _DAGGER_H_
#define _DAGGER_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class COrbit;		// �O��

//-----------------------------------
// �N���X��`(�_�K�[)
//-----------------------------------
class CDagger : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CDagger();			// �R���X�g���N�^
	~CDagger();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(D3DXMATRIX* mtx);				// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetEnableDisp(const bool bDisp);		// �`��󋵂̐ݒ菈��
	bool IsDisp(void) const;					// �`��󋵂̎擾����

	// �ÓI�����o�֐�
	static CDagger* Create(D3DXMATRIX* mtx);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	COrbit* m_pOrbit;				// �O�Ղ̏��
	D3DXMATRIX* m_pMtxParent;		// �e�̃}�g���b�N�X
	bool m_bDisp;					// �`���
	bool m_bDispOld;				// �O��̕`���
};

#endif