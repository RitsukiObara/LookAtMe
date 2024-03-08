//===================================
//
// ���e�ԃw�b�_�[[bang_flower.h]
// Author ��������
//
//===================================
#ifndef _BANG_FLOWER_H_
#define _BANG_FLOWER_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CBomb;		// ���e

//-----------------------------------
// �N���X��`(���e��)
//-----------------------------------
class CBangFlower : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CBangFlower();			// �R���X�g���N�^
	~CBangFlower();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// ���̐ݒ菈��
	void Hit(void);					// �q�b�g����

	CBomb* GetBomb(void) const;		// ���e�̎擾����

	// �ÓI�����o�֐�
	static CBangFlower* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);	// ��������

	static CListManager<CBangFlower*> GetList(void);		// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CBomb* m_pBomb;		// ���e�̏��

	// �ÓI�����o�ϐ�
	static CListManager<CBangFlower*> m_list;		// ���X�g
};

#endif