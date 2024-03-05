//============================================================
//
// X�t�@�C������ [xfile.cpp]
// Author�F��������
//
//============================================================
//************************************************************
// �C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "texture.h"

//------------------------------------------------------------
// �������O���
//------------------------------------------------------------
namespace
{
	const char* LOAD_TXT = "data/TXT/Model.txt";		// ���f���̏��e�L�X�g
}

//============================================================
// �R���X�g���N�^
//============================================================
CXFile::CXFile()
{
	for (int nCnt = 0; nCnt < MAX_XFILE; nCnt++)
	{
		// ���f���̏�������������
		m_apModel[nCnt].pMesh = nullptr;			// ���b�V�� (���_���) �ւ̃|�C���^
		m_apModel[nCnt].pBuffMat = nullptr;			// �}�e���A���ւ̃|�C���^
		m_apModel[nCnt].dwNumMat = 0;				// �}�e���A���̐�
		m_apModel[nCnt].vtxMin = INIT_VTX_MIN;		// �ŏ��̒��_���W
		m_apModel[nCnt].vtxMax = INIT_VTX_MAX;		// �ő�̒��_���W
		m_apModel[nCnt].collsize = INIT_SIZE;		// �����蔻��̃T�C�Y
		m_apModel[nCnt].vtxPos = nullptr;			// ���_�̈ʒu
		m_apModel[nCnt].fRadius = 0.0f;				// ���a
		m_apModel[nCnt].nNumFace = 0;				// �ʂ̐�
		m_apModel[nCnt].bEmpty = true;				// �󔒏�

		for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
		{
			m_apModel[nCnt].m_nTexIdx[nCntMat] = NONE_TEXIDX;		// �e�N�X�`���̃C���f�b�N�X
		}

		*m_apModelName[nCnt] = {};					// ���f���̖��O
	}
}

//============================================================
// �f�X�g���N�^
//============================================================
CXFile::~CXFile()
{

}

//============================================================
// ���f���̏���������
//============================================================
HRESULT CXFile::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_XFILE; nCnt++)
	{
		// ���f���̏�������������
		m_apModel[nCnt].pMesh = nullptr;			// ���b�V�� (���_���) �ւ̃|�C���^
		m_apModel[nCnt].pBuffMat = nullptr;			// �}�e���A���ւ̃|�C���^
		m_apModel[nCnt].dwNumMat = 0;				// �}�e���A���̐�
		m_apModel[nCnt].vtxMin = INIT_VTX_MIN;		// �ŏ��̒��_���W
		m_apModel[nCnt].vtxMax = INIT_VTX_MAX;		// �ő�̒��_���W
		m_apModel[nCnt].collsize = INIT_SIZE;		// �����蔻��̃T�C�Y
		m_apModel[nCnt].vtxPos = nullptr;			// ���_�̈ʒu
		m_apModel[nCnt].fRadius = 0.0f;				// ���a
		m_apModel[nCnt].nNumFace = 0;				// �ʂ̐�
		m_apModel[nCnt].bEmpty = true;				// �󔒏�

		for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
		{
			m_apModel[nCnt].m_nTexIdx[nCntMat] = NONE_TEXIDX;		// �e�N�X�`���̃C���f�b�N�X
		}

		*m_apModelName[nCnt] = {};					// ���f���̖��O
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
// ���f���̏I������
//============================================================
void CXFile::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntModel = 0; nCntModel < MAX_XFILE; nCntModel++)
	{ // ���f���̍ő吔���J��Ԃ�

		if (m_apModel[nCntModel].pMesh != nullptr)
		{ // �ϐ� (m_apModel[nCntModel].pMesh) �� NULL ����Ȃ��ꍇ

			m_apModel[nCntModel].pMesh->Release();
			m_apModel[nCntModel].pMesh = nullptr;
		}

		if (m_apModel[nCntModel].pBuffMat != nullptr)
		{ // �ϐ� (m_apModel[nCntModel].pBuffMat) �� NULL ����Ȃ��ꍇ

			m_apModel[nCntModel].pBuffMat->Release();
			m_apModel[nCntModel].pBuffMat = nullptr;
		}

		if (m_apModel[nCntModel].vtxPos != nullptr)
		{ // ���_���W (m_apModel[nCntModel].vtxPos) �� NULL ����Ȃ��ꍇ

			delete[] m_apModel[nCntModel].vtxPos;
			m_apModel[nCntModel].vtxPos = nullptr;
		}
	}

	// ���������J������
	delete this;
}

//============================================================
// X�t�@�C���̓o�^����
//============================================================
CXFile::SXFile CXFile::Regist(const char* filename)
{
	// ���[�J���ϐ��錾
	SXFile XFile;		// �Ԃ�l�p�ϐ�

	for (int nCntModel = 0; nCntModel < MAX_TEXTURE; nCntModel++)
	{
		if (m_apModel[nCntModel].bEmpty == true)
		{ // ���ɏ�񂪓����Ă��Ȃ��ꍇ

			// �e�N�X�`���̓ǂݍ���
			if (FAILED(LoadXFile(filename,&m_apModel[nCntModel])))
			{ // �e�N�X�`���̐����Ɏ��s�����ꍇ

				// ��~
				assert(false);
			}

			// �����蔻��̍쐬
			SetCollision(&m_apModel[nCntModel]);

			// �e�N�X�`���̓ǂݍ���
			if (FAILED(LoadTexture(&m_apModel[nCntModel])))
			{ // �e�N�X�`���̓ǂݍ��݂Ɏ��s�����ꍇ

				// ��~
				assert(false);
			}

			// �p�X��o�^����
			strcpy(&m_apModelName[nCntModel][0], filename);

			// �󔒏󋵂� false �ɂ���
			m_apModel[nCntModel].bEmpty = false;

			// ���f����������
			XFile = m_apModel[nCntModel];

			// �����o��
			break;
		}
		else
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			if (strcmp(&m_apModelName[nCntModel][0], filename) == 0)
			{ // �ߋ��ɓǂݍ���ł����ꍇ

				// ���f����������
				XFile = m_apModel[nCntModel];

				// �����o��
				break;
			}
		}
	}

	// ���ʂ̃��f����Ԃ�
	return XFile;
}

//============================================================
// ���[�h����
//============================================================
HRESULT CXFile::Load(void)
{
	// ���[�J���ϐ��錾
	int nEnd;						// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	char aModelName[MAX_STRING];	// �e�N�X�`���̖��O(���΃p�X)

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(LOAD_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{ // �ǂݍ��񂾕����� MODEL_FILENAME �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%s", &aModelName[0]);		// ���f���̑��΃p�X��ǂݍ���

				// �e�N�X�`���̓o�^����
				Regist(&aModelName[0]);
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	x�t�@�C���̓ǂݍ���
//============================================================
HRESULT CXFile::LoadXFile(const char* filename, SXFile* pXFile)
{
	// �ϐ���錾
	HRESULT hr;		// �ُ�I���̊m�F�p

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// x�t�@�C���̓ǂݍ���
	hr = D3DXLoadMeshFromX
	( // ����
		filename,				// ���f���̑��΃p�X
		D3DXMESH_SYSTEMMEM,		// ���b�V���쐬�p�I�v�V����
		pDevice,				// �f�o�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^
		&pXFile->pBuffMat,		// �}�e���A���ւ̃|�C���^
		NULL,					// �G�t�F�N�g�f�[�^
		&pXFile->dwNumMat,		// �}�e���A���̐�
		&pXFile->pMesh			// ���b�V�� (���_���) �ւ̃|�C���^
	);

	if (FAILED(hr))
	{ // x�t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�����蔻��̍쐬
//============================================================
void CXFile::SetCollision(SXFile* pXFile)
{
	// �ϐ���錾
	int         nNumVtx;		// ���f���̒��_��
	DWORD       dwSizeFVF;		// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;				// ���f���̒��_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 vtx;			// ���f���̒��_���W

	// ���f���̒��_�����擾
	nNumVtx = pXFile->pMesh->GetNumVertices();

	// ���_�ʒu�̃��������m�ۂ���
	pXFile->vtxPos = new D3DXVECTOR3[nNumVtx];

	// ���f���̖ʂ̐����擾����
	pXFile->nNumFace = pXFile->pMesh->GetNumFaces();

	// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(pXFile->pMesh->GetFVF());

	// ���f���̒��_�o�b�t�@�����b�N
	pXFile->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{ // ���f���̒��_�����J��Ԃ�

		// ���f���̒��_���W����
		vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���W (x) �̐ݒ�
		if (vtx.x < pXFile->vtxMin.x)
		{ // ����̒��_���W (x) ���A����̒��_���W (x) �����������ꍇ

			// ����̒��_��� (x) ����
			pXFile->vtxMin.x = vtx.x;
		}
		else if (vtx.x > pXFile->vtxMax.x)
		{ // ����̒��_���W (x) ���A����̒��_���W (x) �����傫���ꍇ

			// ����̒��_��� (x) ����
			pXFile->vtxMax.x = vtx.x;
		}

		// ���_���W (y) �̐ݒ�
		if (vtx.y < pXFile->vtxMin.y)
		{ // ����̒��_���W (y) ���A����̒��_���W (y) �����������ꍇ

			// ����̒��_��� (y) ����
			pXFile->vtxMin.y = vtx.y;
		}
		else if (vtx.y > pXFile->vtxMax.y)
		{ // ����̒��_���W (y) ���A����̒��_���W (y) �����傫���ꍇ

			// ����̒��_��� (y) ����
			pXFile->vtxMax.y = vtx.y;
		}

		// ���_���W (z) �̐ݒ�
		if (vtx.z < pXFile->vtxMin.z)
		{ // ����̒��_���W (z) ���A����̒��_���W (z) �����������ꍇ

			// ����̒��_��� (z) ����
			pXFile->vtxMin.z = vtx.z;
		}
		else if (vtx.z > pXFile->vtxMax.z)
		{ // ����̒��_���W (z) ���A����̒��_���W (z) �����傫���ꍇ

			// ����̒��_��� (z) ����
			pXFile->vtxMax.z = vtx.z;
		}

		// ���_�̈ʒu��ݒ肷��
		pXFile->vtxPos[nCntVtx] = vtx;

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;
	}

	// ���f���̒��_�o�b�t�@���A�����b�N
	pXFile->pMesh->UnlockVertexBuffer();

	// ���f���T�C�Y�����߂�
	pXFile->collsize = pXFile->vtxMax - pXFile->vtxMin;

	// ���f���̉~�̓����蔻����쐬
	pXFile->fRadius = ((pXFile->collsize.x * 0.5f) + (pXFile->collsize.z * 0.5f)) * 0.5f;
}

//============================================================
//	�e�N�X�`���̓ǂݍ���
//============================================================
HRESULT CXFile::LoadTexture(SXFile* pXFile)
{
	// �f�o�C�X���擾����
	D3DXMATERIAL* pMat;						// �}�e���A���ւ̃|�C���^

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)pXFile->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pXFile->dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{ // �e�N�X�`���t�@�C�������݂���ꍇ

			// �e�N�X�`���̓ǂݍ��ݏ���
			pXFile->m_nTexIdx[nCntMat] = CManager::Get()->GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
// ��������
//============================================================
CXFile* CXFile::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CXFile* pXFile = nullptr;		// X�t�@�C���̃C���X�^���X�𐶐�

	if (pXFile == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pXFile = new CXFile;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pXFile != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pXFile->Init()))
		{ // �������Ɏ��s�����ꍇ

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

	// X�t�@�C���̃|�C���^��Ԃ�
	return pXFile;
}