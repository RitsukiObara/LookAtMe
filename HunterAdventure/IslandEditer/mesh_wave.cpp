//====================================================
//
// ���b�V���̔g���� [mesh_wave.cpp]
// Author�F��������
//
//====================================================
//****************************************************
//	�C���N���[�h�t�@�C��
//****************************************************
#include "manager.h"
#include "mesh_wave.h"
#include "texture.h"
#include "useful.h"

// �������O���
namespace
{
	const int DIVI_WIDTH = 50;				// ���̕�����
	const int DIVI_HEIGHT = 50;				// �c(���s)�̕�����
	const float SIZE_WIDTH = 1000.0f;		// ���̃T�C�Y
	const float SIZE_DEPTH = 1000.0f;		// ���s�̃T�C�Y
}

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CMeshWave::CMeshWave(CObject::PRIORITY priority) : CMeshField(priority)
{
	// �S�Ă̒l���N���A����
	m_fWaveRot = 0.0f;		// �g�̌���
	m_fWaveHeight = 0.0f;	// �g�̍���
	m_fCycle = 0.0f;		// �g�̎���
	m_fSpeed = 0.0f;		// �g�̑��x
}

//================================
// �f�X�g���N�^
//================================
CMeshWave::~CMeshWave()
{

}

//================================
// ����������
//================================
HRESULT CMeshWave::Init(void)
{
	if (FAILED(CMeshField::Init()))
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
void CMeshWave::Uninit(void)
{
	// �I������
	CMeshField::Uninit();
}

//================================
// �X�V����
//================================
void CMeshWave::Update(void)
{
	// �g�̐ݒ菈��
	SetWave();

	// �g�̌��������Z����
	m_fWaveRot += m_fSpeed;

	// �g�̌����𐳋K������
	useful::RotNormalize(&m_fWaveRot);
}

//================================
// �`�揈��
//================================
void CMeshWave::Draw(void)
{
	// �`�揈��
	CMeshField::Draw();
}

//================================
// �f�[�^�̐ݒ菈��
//================================
void CMeshWave::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const float fWaveHeight, const float fCycle, const float fSpeed)
{
	// ���̐ݒ菈��
	CMeshField::SetData
	(
		pos,
		rot,
		fWidth,
		fDepth,
		Divi
	);

	// �S�Ă̒l������������
	m_fWaveRot = 0.0f;		// �g�̌���
	m_fWaveHeight = fWaveHeight;	// �g�̍���
	m_fCycle = fCycle;		// �g�̎���
	m_fSpeed = fSpeed;		// �g�̑��x
}

//================================
// ��������
//================================
CMeshWave* CMeshWave::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const float fWaveHeight, const float fCycle, const float fSpeed, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CMeshWave* pWave = nullptr;			// ���b�V���̔g�̃C���X�^���X�𐶐�

	if (pWave == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pWave = new CMeshWave(priority);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pWave != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		pWave->SetData(pos, rot, fWidth, fDepth, Divi, fWaveHeight, fCycle, fSpeed);

		// ����������
		if (FAILED(pWave->Init()))
		{ // �����������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���b�V���̔g�̃|�C���^��Ԃ�
	return pWave;
}

//================================
// �g�̐ݒ菈��
//================================
void CMeshWave::SetWave(void)
{
	VERTEX_3D *pVtx;									// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// ���_�o�b�t�@
	SGrid vtx = GetVtx();	// ���_��
	float fRot;				// �g�̌���

	if (pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�̃|�C���^�� NULL ����Ȃ��ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDep = 0; nCntDep < vtx.nHeight; nCntDep++)
		{
			// �g�̌�����������
			fRot = m_fWaveRot;

			for (int nCntWid = 0; nCntWid < vtx.nWidth; nCntWid++)
			{
				// ���������Z����
				fRot += m_fCycle;

				// �����̐��K��
				useful::RotNormalize(&fRot);

				// ������ݒ肷��
				pVtx[0].pos.y = cosf(fRot) * m_fWaveHeight;

				pVtx++;				// ���_�f�[�^��i�߂�
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}