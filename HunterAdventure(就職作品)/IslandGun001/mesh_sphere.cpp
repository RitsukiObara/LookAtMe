//======================================================================================================================
//
// ���b�V���X�t�B�A���� [mesh_sphere.cpp]
// Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "manager.h"
#include "mesh_sphere.h"
#include "texture.h"
#include "useful.h"

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CMeshSphere::CMeshSphere(CObject::PRIORITY priority) : CMesh(TYPE_MESH, priority)
{
	// �S�Ă̒l���N���A����
	m_fCircumSize = 0.0f;		// ���̃T�C�Y
	m_fHeightSize = 0.0f;		// �c�̃T�C�Y
}

//================================
// �f�X�g���N�^
//================================
CMeshSphere::~CMeshSphere()
{

}

//================================
// ����������
//================================
HRESULT CMeshSphere::Init(void)
{
	if (FAILED(CMesh::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���_���̐ݒ菈��
	SetVertexAll();

	// �C���f�b�N�X�̐ݒ菈��
	SetIndex(GetVtx().nHeight, GetVtx().nWidth);

	// ������Ԃ�
	return S_OK;
}

//================================
// �I������
//================================
void CMeshSphere::Uninit(void)
{
	// �I������
	CMesh::Uninit();
}

//================================
// �X�V����
//================================
void CMeshSphere::Update(void)
{

}

//================================
// �`�揈��
//================================
void CMeshSphere::Draw(void)
{
	// �`�揈��
	CMesh::Draw();
}

//================================
// ���̐ݒ菈��
//================================
void CMeshSphere::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircum, const float fHeight, const SGrid& Divi)
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
	SetType(TYPE_DOME);			// ���
	SetNumVtx();				// �����_��
	SetNumIdx();				// ���C���f�b�N�X��

	// �S�Ă̒l���N���A����
	m_fCircumSize = fCircum;		// �~���̃T�C�Y
	m_fHeightSize = fHeight;	// �c�̃T�C�Y
}

//================================
// ���_�̐ݒ菈��
//================================
void CMeshSphere::SetVertexAll(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D* pVtx;		// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// ���_�o�b�t�@
	SGrid Divi = GetDivi();	// ������
	SGrid vtx = GetVtx();	// ���_��
	float fAngle;			// ����
	float fHeiAngle;		// �����̕���
	float fLength;			// ����

	if (pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�� NULL ����Ȃ��ꍇ

		// ���_�o�b�t�@�����b�N
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < vtx.nHeight; nCntHeight++)
		{
			for (int nCntCircum = 0; nCntCircum < vtx.nWidth; nCntCircum++)
			{
				// �p�x���Z�o����
				fAngle = (((D3DX_PI * 2) / Divi.nWidth) * nCntCircum) - D3DX_PI;
				fHeiAngle = ((D3DX_PI / Divi.nHeight) * nCntHeight);

				// �p�x�̐��K��
				useful::RotNormalize(&fAngle);
				useful::RotNormalize(&fHeiAngle);

				// ������ݒ肷��
				fLength = cosf(fHeiAngle - (D3DX_PI * 0.5f)) * m_fCircumSize;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				(
					sinf(fAngle) * fLength,
					cosf(fHeiAngle) * m_fHeightSize,
					cosf(fAngle) * fLength
				);

				//�@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((float)((1.0f / Divi.nWidth) * nCntCircum), (float)((1.0f / Divi.nHeight) * nCntHeight));

				pVtx++;			// ���_�f�[�^��i�߂�
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//================================
// ���_�̐ݒ菈��
//================================
void CMeshSphere::SetVertex(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// ���_�o�b�t�@
	SGrid Divi = GetDivi();	// ������
	SGrid vtx = GetVtx();	// ���_��
	float fAngle;			// ����
	float fHeiAngle;		// �����̕���
	float fLength;			// ����

	if (pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�� NULL ����Ȃ��ꍇ

		// ���_�o�b�t�@�����b�N
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < vtx.nHeight; nCntHeight++)
		{
			for (int nCntCircum = 0; nCntCircum < vtx.nWidth; nCntCircum++)
			{
				// �p�x���Z�o����
				fAngle = (((D3DX_PI * 2) / Divi.nWidth) * nCntCircum) - D3DX_PI;
				fHeiAngle = ((D3DX_PI / Divi.nHeight) * nCntHeight);

				// �p�x�̐��K��
				useful::RotNormalize(&fAngle);
				useful::RotNormalize(&fHeiAngle);

				// ������ݒ肷��
				fLength = cosf(fHeiAngle - (D3DX_PI * 0.5f)) * m_fCircumSize;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				(
					sinf(fAngle) * fLength,
					cosf(fHeiAngle) * m_fHeightSize,
					cosf(fAngle) * fLength
				);

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
CMeshSphere* CMeshSphere::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircum, const float fHeight, const SGrid& Divi, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CMeshSphere* pMesh = nullptr;		// ���b�V���̃C���X�^���X�𐶐�

	if (pMesh == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pMesh = new CMeshSphere(priority);
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
		pMesh->SetData(pos, rot, fCircum, fHeight, Divi);

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

//================================
// ���̃T�C�Y�̐ݒ菈��
//================================
void CMeshSphere::SetCircum(const float fCircum)
{
	// ���̃T�C�Y��ݒ肷��
	m_fCircumSize = fCircum;
}

//================================
// ���̃T�C�Y�̎擾����
//================================
float CMeshSphere::GetCircum(void) const
{
	// ���̃T�C�Y��Ԃ�
	return m_fCircumSize;
}

//================================
// �c�̃T�C�Y�̐ݒ菈��
//================================
void CMeshSphere::SetHeight(const float fHeight)
{
	// �c�̃T�C�Y��ݒ肷��
	m_fHeightSize = fHeight;
}

//================================
// �c�̃T�C�Y�̎擾����
//================================
float CMeshSphere::GetHeight(void) const
{
	// �c�̃T�C�Y��Ԃ�
	return m_fHeightSize;
}