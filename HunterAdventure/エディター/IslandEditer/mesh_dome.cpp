//======================================================================================================================
//
//	���b�V���h�[������ [mesh_dome.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "manager.h"
#include "texture.h"
#include "mesh_dome.h"
#include "useful.h"

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CMeshDome::CMeshDome(CObject::PRIORITY priority) : CMesh(TYPE_MESH, priority)
{
	// �S�Ă̒l���N���A����
	m_fCircumSize = 0.0f;		// ���̃T�C�Y
	m_fHeightSize = 0.0f;		// �c�̃T�C�Y
}

//================================
// �f�X�g���N�^
//================================
CMeshDome::~CMeshDome()
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

//================================
// ���̐ݒ菈��
//================================
void CMeshDome::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircum, const float fHeight, const SGrid& Divi)
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
void CMeshDome::SetVertex(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// ���_�o�b�t�@
	SGrid Divi = GetDivi();	// ������
	SGrid vtx = GetVtx();	// ���_��
	float fAngle;			// ����
	float fHeiAngle;		// �����̕���

	if (pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�� NULL ����Ȃ��ꍇ

		// ���_�o�b�t�@�����b�N
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < vtx.nHeight; nCntHeight++)
		{
			for (int nCntCircum = 0; nCntCircum < vtx.nWidth; nCntCircum++)
			{
				// �p�x���Z�o����
				fAngle = D3DX_PI * (nCntCircum * (float)(1.0f / (Divi.nWidth / 2)));
				fHeiAngle = D3DX_PI - ((D3DX_PI * 0.5f) / Divi.nHeight) * nCntHeight;

				// �p�x�̐��K��
				useful::RotNormalize(&fAngle);
				useful::RotNormalize(&fHeiAngle);

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				(
					(sinf(fAngle) * ((float)(m_fCircumSize / Divi.nHeight) * nCntHeight)),
					cosf(fHeiAngle) * -m_fHeightSize,
					(cosf(fAngle) * ((float)(m_fCircumSize / Divi.nHeight) * nCntHeight))
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
// ��������
//================================
CMeshDome* CMeshDome::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircum, const float fHeight, const SGrid& Divi, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CMeshDome* pMesh = nullptr;		// ���b�V���̃C���X�^���X�𐶐�

	if (pMesh == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pMesh = new CMeshDome(priority);
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