//======================================================================================================================
//
//	���b�V���V�����_�[���� [mesh_cylinder.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "manager.h"
#include "texture.h"
#include "mesh_cylinder.h"
#include "useful.h"

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CMeshCylinder::CMeshCylinder(CObject::PRIORITY priority) : CMesh(CObject::TYPE_MESH, priority)
{
	// �S�Ă̒l���N���A����
	m_fCircumSize = 0.0f;		// ���̃T�C�Y
	m_fHeightSize = 0.0f;		// �c�̃T�C�Y
	m_fHeightSizeDivi = 0.0f;	// �c1�ʂ��̃T�C�Y
}

//================================
// �f�X�g���N�^
//================================
CMeshCylinder::~CMeshCylinder()
{

}

//================================
// ����������
//================================
HRESULT CMeshCylinder::Init(void)
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
void CMeshCylinder::Uninit(void)
{
	// �I������
	CMesh::Uninit();
}

//================================
// �X�V����
//================================
void CMeshCylinder::Update(void)
{

}

//================================
// �`�揈��
//================================
void CMeshCylinder::Draw(void)
{
	// �`�揈��
	CMesh::Draw();
}

//================================
// ���̐ݒ菈��
//================================
void CMeshCylinder::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const SGrid& Divi)
{
	// �������ƒ��_����錾����
	SGrid vtx;

	// ��������ݒ肷��
	vtx.nWidth = Divi.nWidth + 1;
	vtx.nHeight = Divi.nHeight + 1;

	// �S�Ă̒l������������
	SetPos(pos);				// �ʒu
	SetRot(rot);				// ����
	SetDivi(Divi);				// ������
	SetVtx(vtx);				// ���_��
	SetType(TYPE_CYLINDER);		// ���
	SetNumVtx();				// �����_��
	SetNumIdx();				// ���C���f�b�N�X��

	// �S�Ă̒l���N���A����
	m_fCircumSize = fWidth;		// �~���̃T�C�Y
	m_fHeightSize = fHeight;	// �c�̃T�C�Y
	m_fHeightSizeDivi = fHeight / (float)Divi.nHeight;	// �c1�ʂ��̃T�C�Y
}

//================================
// ���_�̐ݒ菈��
//================================
void CMeshCylinder::SetVertex(void)
{
	VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// ���_�o�b�t�@
	SGrid Divi = GetDivi();	// ������
	SGrid vtx = GetVtx();	// ���_��
	float fAngle;			// �p�x�Z�o�p�ϐ�

	if (pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�� NULL ����Ȃ��ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < vtx.nHeight; nCntHeight++)
		{
			for (int nCntCircum = 0; nCntCircum < vtx.nWidth; nCntCircum++)
			{
				// �p�x���Z�o����
				fAngle = D3DX_PI * (nCntCircum * (float)(1.0f / (Divi.nWidth / 2)));

				// �p�x�̐��K��
				useful::RotNormalize(&fAngle);

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				(
					(-sinf(fAngle) * m_fCircumSize),
					m_fHeightSize - (m_fHeightSizeDivi * nCntHeight),
					(-cosf(fAngle) * m_fCircumSize)
				);

				//�@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(((float)(1.0f / Divi.nWidth) * nCntCircum), ((float)(1.0f / Divi.nHeight) * nCntHeight));

				pVtx++;			// ���_�f�[�^��i�߂�
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//================================
// ��������
//================================
CMeshCylinder* CMeshCylinder::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const SGrid& Divi, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CMeshCylinder* pMesh = nullptr;	// ���b�V���̃C���X�^���X�𐶐�

	if (pMesh == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pMesh = new CMeshCylinder(priority);
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
		pMesh->SetData(pos, rot, fWidth, fHeight, Divi);

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

	// ���b�V���̃|�C���^��Ԃ�
	return pMesh;
}