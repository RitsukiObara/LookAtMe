//======================================================================================================================
//
//	���b�V���t�B�[���h���� [mesh_field.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "manager.h"
#include "texture.h"
#include "mesh_field.h"

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CMeshField::CMeshField(CObject::PRIORITY priority) : CMesh(CObject::TYPE_MESH, priority)
{
	// �S�Ă̒l���N���A����
	m_fWidthSize = 0.0f;		// ���̃T�C�Y
	m_fDepthSize = 0.0f;		// ���s�̃T�C�Y
	m_fWidthSizeDivi = 0.0f;	// ��1�ʂ��̃T�C�Y
	m_fDepthSizeDivi = 0.0f;	// ���s1�ʂ��̃T�C�Y
}

//================================
// �f�X�g���N�^
//================================
CMeshField::~CMeshField()
{

}

//================================
// ����������
//================================
HRESULT CMeshField::Init(void)
{
	if (FAILED(CMesh::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���_���̐ݒ菈��
	SetVertex();

	// �C���f�b�N�X�̐ݒ菈��
	SetIndex(GetVtx().nHeight, GetVtx().nWidth);

	// ������Ԃ�
	return S_OK;
}

//================================
// �I������
//================================
void CMeshField::Uninit(void)
{
	// �I������
	CMesh::Uninit();
}

//================================
// �X�V����
//================================
void CMeshField::Update(void)
{

}

//================================
// �`�揈��
//================================
void CMeshField::Draw(void)
{
	// �`�揈��
	CMesh::Draw();
}

//================================
// ���̐ݒ菈��
//================================
void CMeshField::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi)
{
	// ���_����錾����
	SGrid vtx;

	// ��������ݒ肷��
	vtx.nWidth = Divi.nWidth + 1;
	vtx.nHeight = Divi.nHeight + 1;

	// �S�Ă̒l������������
	SetPos(pos);				// �ʒu
	SetRot(rot);				// ����
	SetDivi(Divi);				// ������
	SetVtx(vtx);				// ���_��
	SetType(TYPE_FIELD);		// ���
	SetNumVtx();				// �����_��
	SetNumIdx();				// ���C���f�b�N�X��

	// �S�Ă̒l���N���A����
	m_fWidthSize = fWidth;		// ���̃T�C�Y
	m_fDepthSize = fDepth;		// ���s�̃T�C�Y
	m_fWidthSizeDivi = fWidth / (float)Divi.nWidth;		// ��1�ʂ��̃T�C�Y
	m_fDepthSizeDivi = fDepth / (float)Divi.nHeight;	// ���s1�ʂ��̃T�C�Y
}

//================================
// ���_�̐ݒ菈��
//================================
void CMeshField::SetVertex(void)
{
	VERTEX_3D *pVtx;									// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// ���_�o�b�t�@
	SGrid Divi = GetDivi();			// ������
	SGrid vtx = GetVtx();			// ���_��

	if (pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�̃|�C���^�� NULL ����Ȃ��ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDep = 0; nCntDep < vtx.nHeight; nCntDep++)
		{
			for (int nCntWid = 0; nCntWid < vtx.nWidth; nCntWid++)
			{
				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				( // ����
					(nCntWid * m_fWidthSizeDivi) - (m_fWidthSize / 2.0f),	// X���W
					0.0f,													// Y���W
					(m_fDepthSize / 2.0f) - (nCntDep * m_fDepthSizeDivi)	// Z���W
				);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(nCntWid * (1.0f / (float)(Divi.nWidth)), nCntDep * (1.0f / (float)(Divi.nHeight)));

				pVtx++;				// ���_�f�[�^��i�߂�
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//================================
// ��������
//================================
CMeshField* CMeshField::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CMeshField* pMesh = nullptr;	// �t�B�[���h�̃C���X�^���X�𐶐�

	if (pMesh == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pMesh = new CMeshField(priority);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pMesh != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		pMesh->SetData(pos, rot, fWidth, fDepth, Divi);

		// ����������
		if (FAILED(pMesh->Init()))
		{ // �����������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �t�B�[���h�̃|�C���^��Ԃ�
	return pMesh;
}