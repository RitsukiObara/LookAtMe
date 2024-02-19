//=====================================================
//
// ���b�V���g���l�[�h�w�b�_�[ [mesh_tornado.h]
// Author�F��������
//
//=====================================================
#ifndef _MESH_TORNADO_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MESH_TORNADO_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************************
// �C���N���[�h�t�@�C��
//*****************************************************
#include "Objectmesh.h"

//-----------------------------------------------------
// �N���X��`(���b�V���g���l�[�h)
//-----------------------------------------------------
class CMeshTornado : public CMesh
{
public:		// �N�ł��A�N�Z�X�ł���

	CMeshTornado(CObject::PRIORITY priority = PRIORITY_BG);			// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CMeshTornado();		// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const int nVortex, const float fShift, const float fCircum, const SGrid& Divi);			// ���̐ݒ菈��
	void SetVertexAll(void);	// �S�Ă̒��_�̐ݒ菈��
	void SetVertex(void);		// ���_�̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetVortex(const int nVortex);		// �Q�̐ݒ菈��
	int GetVortex(void) const;				// �Q�̎擾����
	void SetWidth(const float fWidth);		// ���̐ݒ菈��
	float GetWidth(void) const;				// ���̎擾����
	void SetHeight(const float fHeight);	// �����̐ݒ菈��
	float GetHeight(void) const;			// �����̎擾����
	void SetShift(const float fShift);		// ���炷���̎擾����
	float GetShift(void) const;				// ���炷���̎擾����
	void SetCircum(const float fCircum);	// �~���̐ݒ菈��
	float GetCircum(void) const;			// �~���̎擾����

	// �ÓI�����o�֐�
	static CMeshTornado* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const int nVortex, const float fShift, const float fCircum, const SGrid& Divi, const PRIORITY priority);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	int m_nVortex;		// �Q�̐�
	float m_fWidth;		// ��
	float m_fHeight;	// ����
	float m_fShift;		// ���炷��
	float m_fCircum;	// �~��
};


#endif