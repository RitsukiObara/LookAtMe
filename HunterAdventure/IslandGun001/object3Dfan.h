//===================================
//
// �I�u�W�F�N�g3D�t�@���w�b�_�[[object3Dfan.h]
// Author ��������
//
//===================================
#ifndef _OBJECT3DFAN_H_
#define _OBJECT3DFAN_H_

#include "object.h"

//--------------------------------------------
// �N���X(�I�u�W�F�N�g3D�N���X)
//--------------------------------------------
class CObject3DFan : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	CObject3DFan(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CObject3DFan();		// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	//void BindTexture(int nIdx);				// �e�N�X�`���̊��蓖�ď���

	// ���_����n
	void SetVertex(void);						// ���_���̐ݒ�
	void SetVtxColor(const D3DXCOLOR& col);		// ���_�J���[�̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetPos(const D3DXVECTOR3& pos);		// �ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPos(void) const;				// �ʒu�̎擾����
	void SetRot(const D3DXVECTOR3& rot);		// �����̐ݒ菈��
	D3DXVECTOR3 GetRot(void) const;				// �����̎擾����
	void SetNumAngle(const int nNum);			// �p�x�̐��̐ݒ菈��
	int GetNumAngle(void) const;				// �p�x�̐��̎擾����
	void SetRadius(const float fRadius);		// ���a�̐ݒ菈��
	float GetRadius(void) const;				// ���a�̎擾����

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_rot;							// ����
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�̃|�C���^
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	int m_nNumAngle;							// �p�x�̐�
	float m_fRadius;							// ���a
};

#endif