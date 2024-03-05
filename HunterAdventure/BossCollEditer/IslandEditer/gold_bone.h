//===================================
//
// ���̍��w�b�_�[[gold_bone.h]
// Author ��������
//
//===================================
#ifndef _GOLD_BONE_H_
#define _GOLD_BONE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(���̍�)
//-----------------------------------
class CGoldBone : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CGoldBone();					// �R���X�g���N�^
	~CGoldBone();					// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);				// ���̐ݒ菈��
	void Hit(void);					// �q�b�g����

	// �ÓI�����o�֐�
	static CGoldBone* Create(const D3DXVECTOR3& pos);	// ��������

	static CListManager<CGoldBone*> GetList(void);		// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Get(void);			// �擾��ԏ���

	// �ÓI�����o�ϐ�
	static CListManager<CGoldBone*> m_list;		// ���X�g
};

#endif