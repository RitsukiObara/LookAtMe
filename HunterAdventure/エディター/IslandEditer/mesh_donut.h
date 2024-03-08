//=====================================================
//
// ���b�V���h�[�i�c�w�b�_�[ [mesh_donut.h]
// Author�F��������
//
//=====================================================
#ifndef _MESH_DONUT_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MESH_DONUT_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************************
// �C���N���[�h�t�@�C��
//*****************************************************
#include "Objectmesh.h"

//-----------------------------------------------------
// �N���X��`(���b�V���h�[�i�c)
//-----------------------------------------------------
class CMeshDonut : public CMesh
{
public:		// �N�ł��A�N�Z�X�ł���

	CMeshDonut(CObject::PRIORITY priority = PRIORITY_BG);			// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CMeshDonut();		// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fCircum, const SGrid& Divi);			// ���̐ݒ菈��
	void SetVertex(void);	// ���_�̐ݒ菈��

	// �ÓI�����o�֐�
	static CMeshDonut* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fCircum, const SGrid& Divi, const PRIORITY priority);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	float m_fCircumSize;		// �~���̃T�C�Y
	float m_fWidthSize;			// ���̃T�C�Y
	float m_fWidthDiviSize;		// 1������������̕��̃T�C�Y
};


#endif