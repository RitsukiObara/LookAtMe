//======================================================================================================================
//
// ���b�V���g���l�[�h���� [mesh_tornado.cpp]
// Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "manager.h"
#include "mesh_tornado.h"
#include "texture.h"
#include "useful.h"

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CMeshTornado::CMeshTornado(CObject::PRIORITY priority) : CMesh(CObject::TYPE_MESH, priority)
{
	// �S�Ă̒l���N���A����
	m_nVortex = 0;			// �Q�̐�
	m_fWidth = 0.0f;		// ��
	m_fHeight = 0.0f;		// ����
	m_fShift = 0.0f;		// ���炷��
	m_fCircum = 0.0f;		// �~��
}

//================================
// �f�X�g���N�^
//================================
CMeshTornado::~CMeshTornado()
{

}

//================================
// ����������
//================================
HRESULT CMeshTornado::Init(void)
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
void CMeshTornado::Uninit(void)
{
	// �I������
	CMesh::Uninit();
}

//================================
// �X�V����
//================================
void CMeshTornado::Update(void)
{

}

//================================
// �`�揈��
//================================
void CMeshTornado::Draw(void)
{
	// �`�揈��
	CMesh::Draw();
}

//================================
// ���̐ݒ菈��
//================================
void CMeshTornado::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const int nVortex, const float fShift, const float fCircum, const SGrid& Divi)
{
	// ���_����錾����
	SGrid vtx;

	// ���_����ݒ肷��
	vtx.nWidth = Divi.nWidth * nVortex;
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
	m_nVortex = nVortex;		// �Q�̐�
	m_fWidth = fWidth;			// ��
	m_fHeight = fHeight;		// ����
	m_fShift = fShift;			// ���炷��
	m_fCircum = fCircum;		// �~��
}

//================================
// ���_�̐ݒ菈��
//================================
void CMeshTornado::SetVertex(void)
{
	VERTEX_3D *pVtx;									// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// ���_�o�b�t�@
	SGrid Divi = GetDivi();			// ������
	SGrid vtx = GetVtx();			// ���_��
	float fAngle;					// ����
	float fLength;					// ����
	float fHeight;					// ����
	int nCount = 0;					// �J�E���g

	if (pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�̃|�C���^�� NULL ����Ȃ��ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWid = 0; nCntWid < vtx.nHeight; nCntWid++)
		{
			for (int nCntHei = 0; nCntHei < m_nVortex; nCntHei++)
			{
				fLength = ((m_fWidth / Divi.nHeight) * (Divi.nHeight - nCntWid)) + (m_fShift * nCntHei) + m_fCircum;

				for (int nCntCir = 0; nCntCir < Divi.nWidth; nCntCir++)
				{
					fHeight = (nCntHei * m_fHeight) + ((m_fHeight / Divi.nWidth) * nCntCir);

					fAngle = ((D3DX_PI * 2) / Divi.nWidth) * nCntCir;

					// �����𐳋K������
					useful::RotNormalize(&fAngle);

					// ���_���W�̐ݒ�
					pVtx[nCount].pos = D3DXVECTOR3
					( // ����
						sinf(fAngle) * fLength,		// X���W
						fHeight,					// Y���W
						cosf(fAngle) * fLength		// Z���W
					);

					// �@���x�N�g���̐ݒ�
					pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					// ���_�J���[�̐ݒ�
					pVtx[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �e�N�X�`�����W�̐ݒ�
					pVtx[nCount].tex = D3DXVECTOR2(((nCntHei * Divi.nWidth) + nCntCir) * (1.0f / (float)(Divi.nWidth)), nCntWid * (1.0f / (float)(Divi.nHeight)));

					// �J�E���g�����Z����
					nCount++;
				}
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//================================
// ��������
//================================
CMeshTornado* CMeshTornado::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const int nVortex, const float fShift, const float fCircum, const SGrid& Divi, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CMeshTornado* pMesh = nullptr;	// �t�B�[���h�̃C���X�^���X�𐶐�

	if (pMesh == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pMesh = new CMeshTornado(priority);
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
		pMesh->SetData(pos, rot, fWidth, fHeight, nVortex, fShift, fCircum, Divi);

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

//================================
// �~���̐ݒ菈��
//================================
void CMeshTornado::SetWidth(const float fWidth)
{
	// ����ݒ肷��
	m_fWidth = fWidth;
}

//================================
// �~���̎擾����
//================================
float CMeshTornado::GetWidth(void) const
{
	// ����Ԃ�
	return m_fWidth;
}

//================================
// �����̐ݒ菈��
//================================
void CMeshTornado::SetHeight(const float fHeight)
{
	// ������ݒ肷��
	m_fHeight = fHeight;
}

//================================
// �����̎擾����
//================================
float CMeshTornado::GetHeight(void) const
{
	// ������Ԃ�
	return m_fHeight;
}