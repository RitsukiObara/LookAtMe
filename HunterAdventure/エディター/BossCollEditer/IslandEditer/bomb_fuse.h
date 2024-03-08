//===================================
//
// ���e�̓��ΐ��w�b�_�[[bomb_fuse.h]
// Author ��������
//
//===================================
#ifndef _BOMB_FUSE_H_
#define _BOMB_FUSE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �N���X��`(���e�̓��ΐ�)
//-----------------------------------
class CBombFuse : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CBombFuse();			// �R���X�g���N�^
	~CBombFuse();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(D3DXMATRIX* pMtx);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBombFuse* Create(D3DXMATRIX* pMtx);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXMATRIX* m_pMtxParent;		// �e�̃}�g���b�N�X
};

#endif