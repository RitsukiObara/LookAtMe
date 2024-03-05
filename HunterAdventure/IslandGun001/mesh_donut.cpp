//======================================================================================================================
//
//	���b�V���h�[�i�c���� [mesh_donut.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "manager.h"
#include "texture.h"
#include "mesh_donut.h"
#include "useful.h"

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CMeshDonut::CMeshDonut(CObject::PRIORITY priority) : CMesh(TYPE_MESH, priority)
{
	// �S�Ă̒l���N���A����
	m_fCircumSize = 0.0f;		// �~���̃T�C�Y
	m_fWidthSize = 0.0f;		// ���̃T�C�Y
	m_fWidthDiviSize = 0.0f;	// ��1�ʂ��̃T�C�Y
}

//================================
// �f�X�g���N�^
//================================
CMeshDonut::~CMeshDonut()
{

}

//================================
// ����������
//================================
HRESULT CMeshDonut::Init(void)
{
	if (FAILED(CMesh::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S���_���̐ݒ菈��
	SetVertexAll();

	// �C���f�b�N�X�̐ݒ菈��
	SetIndex(GetVtx().nWidth, GetVtx().nHeight);

	// ������Ԃ�
	return S_OK;
}

//================================
// �I������
//================================
void CMeshDonut::Uninit(void)
{
	// �I������
	CMesh::Uninit();
}

//================================
// �X�V����
//================================
void CMeshDonut::Update(void)
{

}

//================================
// �`�揈��
//================================
void CMeshDonut::Draw(void)
{
	// �`�揈��
	CMesh::Draw();
}

//================================
// ���̐ݒ菈��
//================================
void CMeshDonut::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fCircum, const SGrid& Divi)
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

	// �S�Ă̒l��ݒ肷��
	m_fCircumSize = fCircum;		// �~���̃T�C�Y
	m_fWidthSize = fWidth;			// ���̃T�C�Y
	m_fWidthDiviSize = fWidth / (float)Divi.nWidth;	// ��1�ʂ��̃T�C�Y
}

//================================
// �S���_�̐ݒ菈��
//================================
void CMeshDonut::SetVertexAll(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D* pVtx;		// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// ���_�o�b�t�@
	SGrid Divi = GetDivi();	// ������
	SGrid vtx = GetVtx();	// ���_��
	float fAngle;			// ����
	float fLength;			// �����̕���

	if (pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�� NULL ����Ȃ��ꍇ

		// ���_�o�b�t�@�����b�N
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWidth = 0; nCntWidth < vtx.nWidth; nCntWidth++)
		{
			for (int nCntCircum = 0; nCntCircum < vtx.nHeight; nCntCircum++)
			{
				// �p�x���Z�o����
				fAngle = D3DX_PI * (nCntCircum * (float)(1.0f / (Divi.nHeight / 2)));

				// �����Z�o����
				fLength = m_fCircumSize + (m_fWidthDiviSize * (Divi.nWidth - nCntWidth));

				// �p�x�̐��K��
				useful::RotNormalize(&fAngle);

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				(
					(sinf(fAngle) * fLength),
					(cosf(fAngle) * fLength),
					0.0f
				);

				//�@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(((float)(1.0f / Divi.nHeight) * nCntCircum), (nCntWidth * 1.0f));

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
void CMeshDonut::SetVertex(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// ���_�o�b�t�@
	SGrid Divi = GetDivi();	// ������
	SGrid vtx = GetVtx();	// ���_��
	float fAngle;			// ����
	float fLength;			// �����̕���

	if (pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�� NULL ����Ȃ��ꍇ

		// ���_�o�b�t�@�����b�N
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWidth = 0; nCntWidth < vtx.nWidth; nCntWidth++)
		{
			for (int nCntCircum = 0; nCntCircum < vtx.nHeight; nCntCircum++)
			{
				// �p�x���Z�o����
				fAngle = D3DX_PI * (nCntCircum * (float)(1.0f / (Divi.nHeight / 2)));

				// �����Z�o����
				fLength = m_fCircumSize + (m_fWidthDiviSize * (Divi.nWidth - nCntWidth));

				// �p�x�̐��K��
				useful::RotNormalize(&fAngle);

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				(
					(sinf(fAngle) * fLength),
					(cosf(fAngle) * fLength),
					0.0f
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
CMeshDonut* CMeshDonut::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fCircum, const SGrid& Divi, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CMeshDonut* pMesh = nullptr;		// ���b�V���̃C���X�^���X�𐶐�

	if (pMesh == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pMesh = new CMeshDonut(priority);
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
		pMesh->SetData(pos, rot, fCircum, fWidth, Divi);

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
// �~���̐ݒ菈��
//================================
void CMeshDonut::SetCircum(const float fCircum)
{
	// �~����ݒ肷��
	m_fCircumSize = fCircum;
}

//================================
// �~���̎擾����
//================================
float CMeshDonut::GetCircum(void) const
{
	// �~����Ԃ�
	return m_fCircumSize;
}

//================================
// ���̐ݒ菈��
//================================
void CMeshDonut::SetWidth(const float fWidth)
{
	// ����ݒ肷��
	m_fWidthSize = fWidth;
	m_fWidthDiviSize = fWidth / (float)GetDivi().nWidth;
}

//================================
// ���̎擾����
//================================
float CMeshDonut::GetWidth(void) const
{
	// ����Ԃ�
	return m_fWidthSize;
}