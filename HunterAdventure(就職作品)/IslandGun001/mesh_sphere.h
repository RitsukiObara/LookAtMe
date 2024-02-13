//=====================================================
//
// ���b�V���X�t�B�A�w�b�_�[ [mesh_sphere.h]
// Author�F��������
//
//=====================================================
#ifndef _MESH_SPHERE_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MESH_SPHERE_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************************
// �C���N���[�h�t�@�C��
//*****************************************************
#include "Objectmesh.h"

//-----------------------------------------------------
// �N���X��`(���b�V���X�t�B�A)
//-----------------------------------------------------
class CMeshSphere : public CMesh
{
public:		// �N�ł��A�N�Z�X�ł���

	CMeshSphere(CObject::PRIORITY priority = PRIORITY_BG);			// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CMeshSphere();		// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircum, const float fHeight, const SGrid& Divi);			// ���̐ݒ菈��
	void SetVertexAll(void);	// ���_�̐ݒ菈��
	void SetVertex(void);		// ���_�̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetCircum(const float fCircum);		// ���̃T�C�Y�̐ݒ菈��
	float GetCircum(void) const;				// ���̃T�C�Y�̎擾����
	void SetHeight(const float fHeight);		// �c�̃T�C�Y�̐ݒ菈��
	float GetHeight(void) const;				// �c�̃T�C�Y�̎擾����

	// �ÓI�����o�֐�
	static CMeshSphere* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircum, const float fHeight, const SGrid& Divi, const PRIORITY priority);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	float m_fCircumSize;		// ���̃T�C�Y
	float m_fHeightSize;		// �c�̃T�C�Y
};


#endif