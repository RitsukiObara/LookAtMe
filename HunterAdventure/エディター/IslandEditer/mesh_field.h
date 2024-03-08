//=====================================================
//
// ���b�V���t�B�[���h�w�b�_�[ [mesh_field.h]
// Author�F��������
//
//=====================================================
#ifndef _MESH_FIELD_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MESH_FIELD_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************************
// �C���N���[�h�t�@�C��
//*****************************************************
#include "Objectmesh.h"

//-----------------------------------------------------
// �N���X��`(���b�V���t�B�[���h)
//-----------------------------------------------------
class CMeshField : public CMesh
{
public:		// �N�ł��A�N�Z�X�ł���

	CMeshField(CObject::PRIORITY priority = PRIORITY_BG);			// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CMeshField();	// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi);			// ���̐ݒ菈��
	void SetVertex(void);	// ���_�̐ݒ菈��

	// �ÓI�����o�֐�
	static CMeshField* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const PRIORITY priority);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	float m_fWidthSize;			// ���̃T�C�Y
	float m_fDepthSize;			// ���s�̃T�C�Y
	float m_fWidthSizeDivi;		// ��1�ʂ��̃T�C�Y
	float m_fDepthSizeDivi;		// ���s1�ʂ��̃T�C�Y
};


#endif