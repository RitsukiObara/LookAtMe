//=====================================================
//
// ���b�V���E�H�[���w�b�_�[ [mesh_wall.h]
// Author�F��������
//
//=====================================================
#ifndef _MESH_WALL_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MESH_WALL_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************************
// �C���N���[�h�t�@�C��
//*****************************************************
#include "Objectmesh.h"

//-----------------------------------------------------
// �N���X��`(���b�V���E�H�[��)
//-----------------------------------------------------
class CMeshWall : public CMesh
{
public:		// �N�ł��A�N�Z�X�ł���

	CMeshWall(CObject::PRIORITY priority = PRIORITY_BG);			// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CMeshWall();			// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const SGrid& Divi);			// ���̐ݒ菈��
	void SetVertexAll(void);	// ���_�̑S�ݒ菈��
	void SetVertex(void);		// ���_�̐ݒ菈��

	// �ÓI�����o�֐�
	static CMeshWall* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const SGrid& Divi, const PRIORITY priority);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	float m_fWidthSize;			// ���̃T�C�Y
	float m_fHeightSize;		// �c�̃T�C�Y
	float m_fWidthSizeDivi;		// ��1�ʂ��̃T�C�Y
	float m_fHeightSizeDivi;	// �c1�ʂ��̃T�C�Y
};


#endif