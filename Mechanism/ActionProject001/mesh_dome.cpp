//======================================================================================================================
//
//	���b�V���h�[������ [mesh_dome.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "mesh_dome.h"

//================================
// �R���X�g���N�^
//================================
CMeshDome::CMeshDome() : CMesh(CObject::TYPE_MESH, CObject::PRIORITY_BG)
{
	
}

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CMeshDome::CMeshDome(CObject::TYPE type, CObject::PRIORITY priority) : CMesh(type, priority)
{

}

//================================
// �f�X�g���N�^
//================================
CMeshDome::~CMeshDome()
{

}

//================================
// �R���X�g���N�^�̔�
//================================
void CMeshDome::Box(void)
{

}

//================================
// ����������
//================================
HRESULT CMeshDome::Init(void)
{
	if (FAILED(CMesh::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//================================
// �I������
//================================
void CMeshDome::Uninit(void)
{
	// �I������
	CMesh::Uninit();
}

//================================
// �X�V����
//================================
void CMeshDome::Update(void)
{

}

//================================
// �`�揈��
//================================
void CMeshDome::Draw(void)
{
	// �`�揈��
	CMesh::Draw();
}