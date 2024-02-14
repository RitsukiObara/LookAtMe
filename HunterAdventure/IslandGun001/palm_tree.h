//===================================
//
// ���V�̖؃w�b�_�[[palm_tree.h]
// Author ��������
//
//===================================
#ifndef _PALM_TREE_H_
#define _PALM_TREE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "tree.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CPalmFruit;	// ���V�̎�

//-----------------------------------
// �N���X��`(���V�̖�)
//-----------------------------------
class CPalmTree : public CTree
{
public:			// �N�ł��A�N�Z�X�ł���

	CPalmTree();				// �R���X�g���N�^
	~CPalmTree();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type) override;		// ���̐ݒ菈��
	void Hit(void) override;		// �q�b�g����

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CPalmFruit* m_pPalmFruit;		// ���V�̎��̏��
};

#endif