//======================================================================================================================
//
//	���b�V������ [mesh.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Objectmesh.h"
#include "useful.h"
#include "texture.h"

#include "mesh_field.h"
#include "mesh_wall.h"
#include "mesh_cylinder.h"
#include "mesh_dome.h"
#include "mesh_donut.h"
#include "mesh_tornado.h"
#include "mesh_sphere.h"

//----------------------------------------------------------------------------------------------------------------------
// �������O���
//----------------------------------------------------------------------------------------------------------------------
namespace
{
	const char* MESH_TXT = "data/TXT/Mesh.txt";			// ���b�V���̃e�L�X�g
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CMesh*> CMesh::m_list = {};		// ���X�g

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CMesh::CMesh(CObject::TYPE type, CObject::PRIORITY priority) : CObject(type, DIM_3D, priority)
{
	// �S�Ă̒l���N���A����
	m_pos = NONE_D3DXVECTOR3;	// �ʒu
	m_rot = NONE_D3DXVECTOR3;	// ����
	m_pVtxBuff = nullptr;		// ���_�o�b�t�@�̃|�C���^
	m_pIdxBuff = nullptr;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_type = TYPE_FIELD;		// ���
	m_vtx.nWidth = 0;			// �����̒��_��
	m_vtx.nHeight = 0;			// �c���̒��_��
	m_divi.nWidth = 0;			// �����̕�����
	m_divi.nHeight = 0;			// �c���̕�����
	m_nNumVtx = 0;				// �����_��
	m_nNumIdx = 0;				// ���C���f�b�N�X��
	m_nTexIdx = NONE_TEXIDX;	// �e�N�X�`���̃C���f�b�N�X
	m_bLightOff = false;		// ���C�e�B���O��
	m_bCullOff = false;			// �J�����O��
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ���[���h�}�g���b�N�X

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//================================
// �f�X�g���N�^
//================================
CMesh::~CMesh()
{

}

//================================
// ����������
//================================
HRESULT CMesh::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * m_nNumVtx,	// �K�v���_��
		D3DUSAGE_WRITEONLY,				// �g�p���@
		FVF_VERTEX_3D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,				// �������̎w��
		&m_pVtxBuff,					// ���_�o�b�t�@�ւ̃|�C���^
		NULL
	)))
	{ // ���_�̐����Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(pDevice->CreateIndexBuffer
	( // ����
		sizeof(WORD) * m_nNumIdx,		// �K�v�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,				// �g�p���@
		D3DFMT_INDEX16,					// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,				// �������̎w��
		&m_pIdxBuff,					// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		NULL
	)))
	{ // �C���f�b�N�X�̐����Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//================================
// �I������
//================================
void CMesh::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{ // �ϐ� (g_pVtxBuffMeshField) ��NULL�ł͂Ȃ��ꍇ

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != nullptr)
	{ // �ϐ� (g_pIdxBuffMeshField) ��NULL�ł͂Ȃ��ꍇ

		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	// �j������
	Release();

	// ������������
	m_list.Pull(this);
}

//================================
// �X�V����
//================================
void CMesh::Update(void)
{

}

//================================
// �`�揈��
//================================
void CMesh::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	if (m_bLightOff == true)
	{ // ���C�e�B���OOFF�̏ꍇ

		// ���C�e�B���O��OFF�ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	if (m_bCullOff == true)
	{ // �J�����OOFF�̏ꍇ

		// �J�����O�̐ݒ��OFF�ɂ���
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	// �`�揈���̒��g
	DrawProcess();

	if (m_bCullOff == true)
	{ // �J�����OOFF�̏ꍇ

		// �J�����O�̐ݒ��ON�ɂ���
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	if (m_bLightOff == true)
	{ // ���C�e�B���OOFF�̏ꍇ

		// ���C�e�B���O��ON�ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//================================
// �`�揈���̒��g
//================================
void CMesh::DrawProcess(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		m_pVtxBuff,					// ���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D));			// ���_���\���̂̃T�C�Y

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,													// �`�悷��ŏ��̒��_�C���f�b�N�X
		0,
		m_nNumVtx,											// �p�ӂ������_�̐�
		0,
		m_nNumIdx - 2);										// �`�悷��v���~�e�B�u��
}

//================================
// �e�N�X�`���̊��蓖�ď���
//================================
void CMesh::BindTexture(int nIdx)
{
	// �e�N�X�`�������蓖�Ă�
	m_nTexIdx = nIdx;
}

//================================
// �ʒu�̐ݒ菈��
//================================
void CMesh::SetPos(const D3DXVECTOR3& pos)
{
	// �ʒu��ݒ肷��
	m_pos = pos;
}

//================================
// �ʒu�̎擾����
//================================
D3DXVECTOR3 CMesh::GetPos(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//================================
// �����̐ݒ菈��
//================================
void CMesh::SetRot(const D3DXVECTOR3& rot)
{
	// ������ݒ肷��
	m_rot = rot;
}

//================================
// �����̎擾����
//================================
D3DXVECTOR3 CMesh::GetRot(void) const
{
	// ������Ԃ�
	return m_rot;
}

//================================
// ���_�o�b�t�@�̃|�C���^�̎擾����
//================================
LPDIRECT3DVERTEXBUFFER9 CMesh::GetVtxBuff(void) const
{
	// ���_�o�b�t�@�̃|�C���^��Ԃ�
	return m_pVtxBuff;
}

//================================
// �C���f�b�N�X�o�b�t�@�̃|�C���^�̎擾����
//================================
LPDIRECT3DINDEXBUFFER9 CMesh::GetIdxBuff(void) const
{
	// �C���f�b�N�X�o�b�t�@�̃|�C���^��Ԃ�
	return m_pIdxBuff;
}

//================================
// ��ނ̐ݒ菈��
//================================
void CMesh::SetType(const TYPE type)
{
	// ��ނ�ݒ肷��
	m_type = type;
}

//================================
// ��ނ̎擾����
//================================
CMesh::TYPE CMesh::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//================================
// ���_���̐ݒ菈��
//================================
void CMesh::SetVtx(const SGrid vtx)
{
	// ���_����ݒ肷��
	m_vtx = vtx;
}

//================================
// ���_���̎擾����
//================================
CMesh::SGrid CMesh::GetVtx(void) const
{
	// ���_����Ԃ�
	return m_vtx;
}

//================================
// �������̐ݒ菈��
//================================
void CMesh::SetDivi(const SGrid divi)
{
	// ��������ݒ肷��
	m_divi = divi;
}

//================================
// �������̎擾����
//================================
CMesh::SGrid CMesh::GetDivi(void) const
{
	// ��������Ԃ�
	return m_divi;
}

//================================
// �����_���̐ݒ菈��
//================================
void CMesh::SetNumVtx(void)
{
	// �����_����ݒ肷��
	m_nNumVtx = m_vtx.nWidth * m_vtx.nHeight;
}

//================================
// �����_���̎擾����
//================================
int CMesh::GetNumVtx(void)
{
	// �����_����Ԃ�
	return m_nNumVtx;
}

//================================
// ���C���f�b�N�X���̐ݒ菈��
//================================
void CMesh::SetNumIdx(void)
{	
	// ���C���f�b�N�X����ݒ肷��
	m_nNumIdx = (2 * (m_vtx.nWidth * m_divi.nHeight)) + ((m_vtx.nHeight - 2) * 2);			
}

//================================
// ���C���f�b�N�X���̎擾����
//================================
int CMesh::GetNumIdx(void)
{
	// ���C���f�b�N�X����Ԃ�
	return m_nNumIdx;
}

//================================
// ���C�e�B���O�̐ݒ菈��
//================================
void CMesh::SetLighting(const bool bLighting)
{
	// ���C�e�B���O�󋵂�ݒ肷��
	m_bLightOff = bLighting;
}

//================================
// �J�����O�̐ݒ菈��
//================================
void CMesh::SetCulling(const bool bCulling)
{
	// �J�����O�󋵂�ݒ肷��
	m_bCullOff = bCulling;
}

/*
* @brief �C���f�b�N�X�̐ݒ菈��
* @param nNumVtx1 [in] ����̒��_��
* @param nNumVtx2 [in] 1�w���Ƃ̒��_��
*/
void CMesh::SetIndex(const int nNumVtx1, const int nNumVtx2)
{
	// ���[�J���ϐ��錾
	WORD *pIdx;			// �C���f�b�N�X���ւ̃|�C���^
	int nIdx = 0;		// �C���f�b�N�X��

	//------------------------------------------------------------------------------------------------------------------
	//	�C���f�b�N�X���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntDep = 1; nCntDep < nNumVtx1; nCntDep++)
	{
		if (nCntDep != 1)
		{ // �ŏ��ȊO

			// �C���f�b�N�X����ݒ肷��
			pIdx[0] = (WORD)(nNumVtx2 * nCntDep);

			// �C���f�b�N�X�f�[�^�����Z����
			pIdx++;
			nIdx++;
		}

		for (int nCntWid = 0; nCntWid < nNumVtx2; nCntWid++)
		{ // 1�w���Ƃɐݒ肷��

			// �C���f�b�N�X����ݒ肷��
			pIdx[0] = (WORD)((nNumVtx2 * nCntDep) + nCntWid);
			pIdx[1] = (WORD)((nNumVtx2 * nCntDep) - (nNumVtx2 - nCntWid));

			// �C���f�b�N�X�f�[�^��2�i�߂�
			pIdx += 2;
			nIdx += 2;

			if (nCntWid == (nNumVtx2 - 1) && nCntDep != (nNumVtx1 - 1))
			{ // �܂�Ԃ��ɓ������ꍇ

				// �C���f�b�N�X����ݒ肷��
				pIdx[0] = (WORD)((nNumVtx2 * nCntDep) - (nNumVtx2 - nCntWid));

				// �C���f�b�N�X�f�[�^�����Z����
				pIdx++;
				nIdx++;
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//================================
// ���_�J���[�̐ݒ菈��
//================================
void CMesh::SetColor(const D3DXCOLOR& col)
{
	VERTEX_3D* pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCol = 0; nCntCol < m_nNumVtx; nCntCol++)
	{
		//���_���W�̐ݒ�
		pVtx[0].col = col;

		pVtx++;			// ���_�f�[�^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//================================
// �e�L�X�g�ǂݍ��ݏ���
//================================
void CMesh::TxtSet(void)
{
	// �ϐ���錾
	int nEnd;						// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE  *pFile;							// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(MESH_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (nEnd == EOF)
			{ // �I����������v�����ꍇ

				// while���𔲂��o��
				break;
			}

			if (strcmp(&aString[0], "SET_MESHFIELD") == 0)
			{ // �ǂݍ��񂾕����� SET_MESHFIELD �̏ꍇ

				// �t�B�[���h�̓ǂݍ��ݏ���
				nEnd = TxtField(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHWALL") == 0)
			{ // �ǂݍ��񂾕����� SET_MESHWALL �̏ꍇ

				// �E�H�[���̓ǂݍ��ݏ���
				nEnd = TxtWall(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHCYLINDER") == 0)
			{ // �ǂݍ��񂾕����� SET_MESHCYLINDER �̏ꍇ

				// �V�����_�[�̓ǂݍ��ݏ���
				nEnd = TxtCylinder(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHDOME") == 0)
			{ // �ǂݍ��񂾕����� SET_MESHDOME �̏ꍇ

				// �h�[���̓ǂݍ��ݏ���
				nEnd = TxtDome(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHDONUT") == 0)
			{ // �ǂݍ��񂾕����� SET_MESH_DONUT �̏ꍇ

				// �h�[�i�c�̓ǂݍ��ݏ���
				nEnd = TxtDonut(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHTORNADO") == 0)
			{ // �ǂݍ��񂾕����� SET_MESHTORNADO �̏ꍇ

				// �g���l�[�h�̓ǂݍ��ݏ���
				nEnd = TxtTornado(pFile);
			}
			else if (strcmp(&aString[0], "SET_MESHSPHERE") == 0)
			{ // �ǂݍ��񂾕����� SET_MESHSPHERE �̏ꍇ

				// �X�t�B�A�̓ǂݍ��ݏ���
				nEnd = TxtSphere(pFile);
			}

		} while (nEnd != EOF);														// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);
	}
}

//================================
// �t�B�[���h�̓ǂݍ��ݏ���
//================================
int CMesh::TxtField(FILE* pFile)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// �ʒu
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// ����
	float fWidth = 0.0f;				// ��
	float fDepth = 0.0f;				// ���s
	SGrid divi = { 0,0 };				// ������
	bool bTex = false;					// �e�N�X�`����
	bool bLighting = false;				// ���C�e�B���O��
	bool bCulling = false;				// �J�����O��
	CMeshField* pField = nullptr;		// �t�B�[���h�̃|�C���^

	// �ϐ��z���錾
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	char aTextureName[MAX_STRING];	// �e�N�X�`���̃p�X��

	do
	{ // �ǂݍ��񂾕����� END_SET_MESHFIELD �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // �ǂݍ��񂾕����� ROT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // �ǂݍ��񂾕����� SIZE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f", &fWidth, &fDepth);			// �傫����ǂݍ���
		}
		else if (strcmp(&aString[0], "WIDTH") == 0)
		{ // �ǂݍ��񂾕����� WIDTH �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nWidth);					// �����̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "DEPTH") == 0)
		{ // �ǂݍ��񂾕����� DEPTH �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nHeight);					// �����̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // �ǂݍ��񂾕����� TEXTURE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aTextureName[0]);			// �p�X��ǂݍ���

			// �e�N�X�`���󋵂� true �ɂ���
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // �ǂݍ��񂾕����� LIGHTING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// ���C�e�B���O�󋵂�ǂݍ���

			// ���C�e�B���O�󋵂�ݒ肷��
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // �ǂݍ��񂾕����� CULLING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// �J�����O�󋵂�ǂݍ���

			// �J�����O�󋵂�ݒ肷��
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHFIELD") != 0);	// �ǂݍ��񂾕����� END_SET_MESHFIELD �ł͂Ȃ��ꍇ���[�v

	// �t�B�[���h�̐�������
	pField = CMeshField::Create(pos, rot, fWidth, fDepth, divi, PRIORITY_BG);

	if (bTex == true)
	{ // �e�N�X�`�����g���ꍇ

		// �e�N�X�`���̊��蓖�ď���
		pField->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pField->SetLighting(bLighting);		// ���C�e�B���O��
	pField->SetCulling(bCulling);		// �J�����O��

	// �I���󋵂�Ԃ�
	return nEnd;
}

//================================
// �E�H�[���̓ǂݍ��ݏ���
//================================
int CMesh::TxtWall(FILE* pFile)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// �ʒu
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// ����
	float fWidth = 0.0f;				// ��
	float fHeight = 0.0f;				// ����
	SGrid divi = { 0,0 };				// ������
	bool bTex = false;					// �e�N�X�`����
	bool bLighting = false;				// ���C�e�B���O��
	bool bCulling = false;				// �J�����O��
	CMeshWall* pWall = nullptr;			// �E�H�[���̃|�C���^

	// �ϐ��z���錾
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	char aTextureName[MAX_STRING];	// �e�N�X�`���̃p�X��

	do
	{ // �ǂݍ��񂾕����� END_SET_MESHWALL �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // �ǂݍ��񂾕����� ROT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // �ǂݍ��񂾕����� SIZE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f", &fWidth, &fHeight);			// �傫����ǂݍ���
		}
		else if (strcmp(&aString[0], "WIDTH") == 0)
		{ // �ǂݍ��񂾕����� WIDTH �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nWidth);					// �����̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nHeight);					// �c���̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // �ǂݍ��񂾕����� TEXTURE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aTextureName[0]);			// �p�X��ǂݍ���

			// �e�N�X�`���󋵂� true �ɂ���
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // �ǂݍ��񂾕����� LIGHTING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// OFF��ǂݍ���

			// ���C�e�B���O�󋵂�ݒ肷��
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // �ǂݍ��񂾕����� CULLING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// �J�����O�󋵂�ǂݍ���

			// �J�����O�󋵂�ݒ肷��
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHWALL") != 0);	// �ǂݍ��񂾕����� END_SET_MESHWALL �ł͂Ȃ��ꍇ���[�v

	// ���b�V���E�H�[���̐�������
	pWall = CMeshWall::Create(pos, rot, fWidth, fHeight, divi, PRIORITY_BG);

	if (bTex == true)
	{ // �e�N�X�`�����g���ꍇ

		// �e�N�X�`���̊��蓖�ď���
		pWall->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pWall->SetLighting(bLighting);		// ���C�e�B���O��
	pWall->SetCulling(bCulling);		// �J�����O��

	// �I���󋵂�Ԃ�
	return nEnd;
}

//================================
// �V�����_�[�̓ǂݍ��ݏ���
//================================
int CMesh::TxtCylinder(FILE* pFile)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// �ʒu
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// ����
	float fCircum = 0.0f;				// �~��
	float fHeight = 0.0f;				// ����
	SGrid divi = { 0,0 };				// ������
	bool bTex = false;					// �e�N�X�`����
	bool bLighting = false;				// ���C�e�B���O��
	bool bCulling = false;				// �J�����O��
	CMeshCylinder* pCylinder = nullptr;	// �V�����_�[�̃|�C���^

	// �ϐ��z���錾
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	char aTextureName[MAX_STRING];	// �e�N�X�`���̃p�X��

	do
	{ // �ǂݍ��񂾕����� END_SET_MESHCYLINDER �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // �ǂݍ��񂾕����� ROT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // �ǂݍ��񂾕����� SIZE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f", &fCircum, &fHeight);			// �T�C�Y��ǂݍ���
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // �ǂݍ��񂾕����� CIRCUM �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nWidth);					// �~���̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nHeight);					// �����̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // �ǂݍ��񂾕����� TEXTURE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aTextureName[0]);			// �p�X��ǂݍ���

			// �e�N�X�`���󋵂� true �ɂ���
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // �ǂݍ��񂾕����� LIGHTING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// OFF��ǂݍ���

			// ���C�e�B���O�󋵂�ݒ肷��
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // �ǂݍ��񂾕����� CULLING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// �J�����O�󋵂�ǂݍ���

			// �J�����O�󋵂�ݒ肷��
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHCYLINDER") != 0);	// �ǂݍ��񂾕����� END_SET_MESHCYLINDER �ł͂Ȃ��ꍇ���[�v

	// ���̐�������
	pCylinder = CMeshCylinder::Create(pos, rot, fCircum, fHeight, divi, PRIORITY_BG);

	if (bTex == true)
	{ // �e�N�X�`�����g���ꍇ

		// �e�N�X�`���̊��蓖�ď���
		pCylinder->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pCylinder->SetLighting(bLighting);		// ���C�e�B���O��
	pCylinder->SetCulling(bCulling);		// �J�����O��

	// �I���󋵂�Ԃ�
	return nEnd;
}

//================================
// �h�[���̓ǂݍ��ݏ���
//================================
int CMesh::TxtDome(FILE* pFile)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// �ʒu
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// ����
	float fCircum = 0.0f;				// �~��
	float fHeight = 0.0f;				// ����
	SGrid divi = { 0,0 };				// ������
	bool bTex = false;					// �e�N�X�`����
	bool bLighting = false;				// ���C�e�B���O��
	bool bCulling = false;				// �J�����O��
	CMeshDome* pDome = nullptr;			// �h�[���̃|�C���^

	// �ϐ��z���錾
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	char aTextureName[MAX_STRING];	// �e�N�X�`���̃p�X��

	do
	{ // �ǂݍ��񂾕����� END_SET_MESHDOME �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // �ǂݍ��񂾕����� ROT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // �ǂݍ��񂾕����� SIZE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f", &fCircum, &fHeight);			// �T�C�Y��ǂݍ���
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // �ǂݍ��񂾕����� CIRCUM �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nWidth);					// �~���̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ0
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nHeight);					// �����̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // �ǂݍ��񂾕����� TEXTURE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aTextureName[0]);			// �p�X��ǂݍ���

			// �e�N�X�`���󋵂� true �ɂ���
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // �ǂݍ��񂾕����� LIGHTING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// OFF��ǂݍ���

			// ���C�e�B���O�󋵂�ݒ肷��
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // �ǂݍ��񂾕����� CULLING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// �J�����O�󋵂�ǂݍ���

			// �J�����O�󋵂�ݒ肷��
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHDOME") != 0);	// �ǂݍ��񂾕����� END_SET_MESHDOME �ł͂Ȃ��ꍇ���[�v

	// �h�[���̐�������
	pDome = CMeshDome::Create(pos, rot, fCircum, fHeight, divi, PRIORITY_BG);

	if (bTex == true)
	{ // �e�N�X�`�����g���ꍇ

		// �e�N�X�`���̊��蓖�ď���
		pDome->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pDome->SetLighting(bLighting);		// ���C�e�B���O��
	pDome->SetCulling(bCulling);		// �J�����O��

	// �I���󋵂�Ԃ�
	return nEnd;
}

//================================
// �h�[�i�c�̓ǂݍ��ݏ���
//================================
int CMesh::TxtDonut(FILE* pFile)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// �ʒu
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// ����
	float fWidth = 0.0f;				// ��
	float fCircum = 0.0f;				// �~��
	SGrid divi = { 0,0 };				// ������
	bool bTex = false;					// �e�N�X�`����
	bool bLighting = false;				// ���C�e�B���O��
	bool bCulling = false;				// �J�����O��
	CMeshDonut* pDonut = nullptr;		// �h�[�i�c�̃|�C���^

	// �ϐ��z���錾
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	char aTextureName[MAX_STRING];	// �e�N�X�`���̃p�X��

	do
	{ // �ǂݍ��񂾕����� END_SET_MESHDONUT �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // �ǂݍ��񂾕����� ROT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // �ǂݍ��񂾕����� SIZE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f", &fWidth, &fCircum);			// �T�C�Y��ǂݍ���
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // �ǂݍ��񂾕����� CIRCUM �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nHeight);					// �~���̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "WIDTH") == 0)
		{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nWidth);					// �����̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // �ǂݍ��񂾕����� TEXTURE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aTextureName[0]);			// �p�X��ǂݍ���

			// �e�N�X�`���󋵂� true �ɂ���
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // �ǂݍ��񂾕����� LIGHTING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// OFF��ǂݍ���

			// ���C�e�B���O�󋵂�ݒ肷��
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // �ǂݍ��񂾕����� CULLING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// �J�����O�󋵂�ǂݍ���

			// �J�����O�󋵂�ݒ肷��
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHDONUT") != 0);	// �ǂݍ��񂾕����� END_SET_MESHDONUT �ł͂Ȃ��ꍇ���[�v

	// �h�[�i�c�̐�������
	pDonut = CMeshDonut::Create(pos, rot, fWidth, fCircum, divi, PRIORITY_BG);

	if (bTex == true)
	{ // �e�N�X�`�����g���ꍇ

		// �e�N�X�`���̊��蓖�ď���
		pDonut->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pDonut->SetLighting(bLighting);		// ���C�e�B���O��
	pDonut->SetCulling(bCulling);		// �J�����O��

	// �I���󋵂�Ԃ�
	return nEnd;
}

//================================
// �g���l�[�h�̓ǂݍ��ݏ���
//================================
int CMesh::TxtTornado(FILE* pFile)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// �ʒu
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// ����
	float fWidth = 0.0f;				// ��
	float fHeight = 0.0f;				// ����
	float fShift = 0.0f;				// ���炷��
	float fCircum = 0.0f;				// �~��
	int nVortex = 0;					// �Q
	SGrid divi = { 0,0 };				// ������
	bool bTex = false;					// �e�N�X�`����
	bool bLighting = false;				// ���C�e�B���O��
	bool bCulling = false;				// �J�����O��
	CMeshTornado* pTornado = nullptr;	// �g���l�[�h�̃|�C���^

	// �ϐ��z���錾
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	char aTextureName[MAX_STRING];	// �e�N�X�`���̃p�X��

	do
	{ // �ǂݍ��񂾕����� END_SET_MESHTORNADO �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // �ǂݍ��񂾕����� ROT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // �ǂݍ��񂾕����� SIZE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f", &fWidth, &fHeight);			// �T�C�Y��ǂݍ���
		}
		else if (strcmp(&aString[0], "SHIFT") == 0)
		{ // �ǂݍ��񂾕����� SHIFT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f", &fShift);						// ���炷����ǂݍ���
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // �ǂݍ��񂾕����� CIRCUM �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f", &fCircum);						// �~����ǂݍ���
		}
		else if (strcmp(&aString[0], "WIDTH") == 0)
		{ // �ǂݍ��񂾕����� WIDTH �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nWidth);					// ���̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nHeight);					// �����̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "VORTEX") == 0)
		{ // �ǂݍ��񂾕����� VORTEX �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &nVortex);						// �Q�̐���ǂݍ���
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // �ǂݍ��񂾕����� TEXTURE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aTextureName[0]);			// �p�X��ǂݍ���

			// �e�N�X�`���󋵂� true �ɂ���
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // �ǂݍ��񂾕����� LIGHTING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// OFF��ǂݍ���

			// ���C�e�B���O�󋵂�ݒ肷��
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // �ǂݍ��񂾕����� CULLING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// �J�����O�󋵂�ǂݍ���

			// �J�����O�󋵂�ݒ肷��
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHTORNADO") != 0);	// �ǂݍ��񂾕����� END_SET_MESHTORNADO �ł͂Ȃ��ꍇ���[�v

	// �g���l�[�h�̐�������
	pTornado = CMeshTornado::Create(pos, rot, fWidth, fHeight, nVortex, fShift, fCircum, divi, PRIORITY_BG);

	if (bTex == true)
	{ // �e�N�X�`�����g���ꍇ

		// �e�N�X�`���̊��蓖�ď���
		pTornado->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pTornado->SetLighting(bLighting);		// ���C�e�B���O��
	pTornado->SetCulling(bCulling);		// �J�����O��

	// �I���󋵂�Ԃ�
	return nEnd;
}

//================================
// �X�t�B�A�̓ǂݍ��ݏ���
//================================
int CMesh::TxtSphere(FILE* pFile)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;	// �ʒu
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;	// ����
	float fCircum = 0.0f;				// �~��
	float fHeight = 0.0f;				// ����
	SGrid divi = { 0,0 };				// ������
	bool bTex = false;					// �e�N�X�`����
	bool bLighting = false;				// ���C�e�B���O��
	bool bCulling = false;				// �J�����O��
	CMeshSphere* pSphere = nullptr;		// �X�t�B�A�̃|�C���^

	// �ϐ��z���錾
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	char aTextureName[MAX_STRING];	// �e�N�X�`���̃p�X��

	do
	{ // �ǂݍ��񂾕����� END_SET_MESHSPHERE �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		nEnd = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // �ǂݍ��񂾕����� ROT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
		}
		else if (strcmp(&aString[0], "SIZE") == 0)
		{ // �ǂݍ��񂾕����� SIZE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f", &fCircum, &fHeight);			// �T�C�Y��ǂݍ���
		}
		else if (strcmp(&aString[0], "CIRCUM") == 0)
		{ // �ǂݍ��񂾕����� CIRCUM �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nWidth);					// �~���̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "HEIGHT") == 0)
		{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d", &divi.nHeight);					// �����̕�������ǂݍ���
		}
		else if (strcmp(&aString[0], "TEXTURE") == 0)
		{ // �ǂݍ��񂾕����� TEXTURE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aTextureName[0]);			// �p�X��ǂݍ���

			// �e�N�X�`���󋵂� true �ɂ���
			bTex = true;
		}
		else if (strcmp(&aString[0], "LIGHTING") == 0)
		{ // �ǂݍ��񂾕����� LIGHTING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// OFF��ǂݍ���

			// ���C�e�B���O�󋵂�ݒ肷��
			bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}
		else if (strcmp(&aString[0], "CULLING") == 0)
		{ // �ǂݍ��񂾕����� CULLING �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);				// �J�����O�󋵂�ǂݍ���

			// �J�����O�󋵂�ݒ肷��
			bCulling = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
		}

	} while (strcmp(&aString[0], "END_SET_MESHSPHERE") != 0);	// �ǂݍ��񂾕����� END_SET_MESHSPHERE �ł͂Ȃ��ꍇ���[�v

	// �X�t�B�A�̐�������
	pSphere = CMeshSphere::Create(pos, rot, fCircum, fHeight, divi, PRIORITY_BG);

	if (bTex == true)
	{ // �e�N�X�`�����g���ꍇ

		// �e�N�X�`���̊��蓖�ď���
		pSphere->BindTexture(CManager::Get()->GetTexture()->Regist(&aTextureName[0]));
	}
	pSphere->SetLighting(bLighting);		// ���C�e�B���O��
	pSphere->SetCulling(bCulling);		// �J�����O��

	// �I���󋵂�Ԃ�
	return nEnd;
}

//================================
// ���X�g�̎擾����
//================================
CListManager<CMesh*> CMesh::GetList(void)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_list;
}