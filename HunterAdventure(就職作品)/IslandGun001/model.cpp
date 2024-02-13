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
#include "model.h"
#include "texture.h"
#include "useful.h"

//=====================================================================��������CModel�̏���=====================================================================

//========================
// �I�[�o�[���[�h�R���X�g���N�^
//========================
CModel::CModel(CObject::TYPE type, PRIORITY priority) : CObject(type, DIM_3D, priority)
{
	// �S�Ă̒l���N���A����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �O��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
	m_scale = NONE_SCALE;								// �g�嗦
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));		// ���[���h�}�g���b�N�X
	ZeroMemory(&m_XFileData, sizeof(CXFile::SXFile));	// X�t�@�C���̃f�[�^
}

//========================
// �f�X�g���N�^
//========================
CModel::~CModel()
{

}

//========================
// ����������
//========================
HRESULT CModel::Init(void)
{
	// �S�Ă̒l������������
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu
	m_posOld = m_pos;			// �O��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
	m_scale = NONE_SCALE;		// �g�嗦

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CModel::Uninit(void)
{
	// �j������
	Release();
}

//========================
// �X�V����
//========================
void CModel::Update(void)
{

}

//========================
// �`�揈��
//========================
void CModel::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
// �`�揈��(�F����)
//========================
void CModel::Draw(D3DXCOLOR col)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL     colMat;					// �`��p�}�e���A��

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �\���̗̂v�f���N���A
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// �}�e���A���f�[�^�̃R�s�[�ɑ������
		colMat = pMat[nCntMat];

		// �����x��������
		colMat.MatD3D.Diffuse = col;
		colMat.MatD3D.Ambient = col;
		colMat.MatD3D.Emissive = col;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&colMat.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
// ���f���e�̕`�揈��
//========================
void CModel::DrawShadow(void)
{
	// �ϐ���錾
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     colMat;					// �`��p�}�e���A��

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �\���̗̂v�f���N���A
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// �����x��������
		colMat.MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		colMat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&colMat.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		if (m_scale != NONE_SCALE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
// �G�f�B�b�g�̕`�揈��
//========================
void CModel::DrawBlock(const D3DXMATERIAL* pMat)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
// ���̃}�g���b�N�X�Ƃ̊|�����킹�`�揈��
//========================
void CModel::DrawMatrix(D3DXMATRIX mtxParent)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL* pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	D3DXMATRIX mtxRotModel, mtxTransModel;		// �v�Z�p�}�g���b�N�X

	// �p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �Z�o�����u�p�[�c�̃��[���h�}�g���b�N�X�v�Ɓu�e�̃}�g���b�N�X�v���|�����킹��
	D3DXMatrixMultiply
	(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent
	);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
// �`�揈��(�����F)
//========================
void CModel::Draw(D3DXCOLOR* col)
{
	// �ϐ���錾
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     colMat;					// �`��p�}�e���A��

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �\���̗̂v�f���N���A
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// �F��������
		colMat.MatD3D.Diffuse = col[nCntMat];

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&colMat.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
// �`�揈��(�����x����)
//========================
void CModel::Draw(const float fAlpha)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL     colMat;					// �`��p�}�e���A��

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �\���̗̂v�f���N���A
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// �}�e���A���f�[�^�̃R�s�[�ɑ������
		colMat = pMat[nCntMat];

		// �����x��������
		colMat.MatD3D.Diffuse.a = fAlpha;
		colMat.MatD3D.Ambient.a = fAlpha;
		colMat.MatD3D.Emissive.a = fAlpha;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&colMat.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
// ���f���̓����蔻��
//========================
bool CModel::ModelCollision(D3DXVECTOR3* pos)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 nor, vec1, vec2;	// �@���A�ʒu
	int nNumVtx;					// ���f���̒��_��
	DWORD dwSizeFVF;				// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;					// ���f���̒��_�o�b�t�@�ւ̃|�C���^
	bool bLand = false;				// ���n����
	D3DXVECTOR3* aVtxNo;			// ���_���W

	if (pos->x <= m_pos.x + m_XFileData.vtxMax.x &&
		pos->x >= m_pos.x + m_XFileData.vtxMin.x &&
		pos->z <= m_pos.z + m_XFileData.vtxMax.z &&
		pos->z >= m_pos.z + m_XFileData.vtxMin.z)
	{ // ���f���͈̔͂ɂ���ꍇ

		// ���f���̒��_�����擾
		nNumVtx = m_XFileData.pMesh->GetNumVertices();

		// ���_���W�̃��������m�ۂ���
		aVtxNo = new D3DXVECTOR3[nNumVtx];

		// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(m_XFileData.pMesh->GetFVF());

		// ���f���̒��_�o�b�t�@�����b�N
		m_XFileData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // ���f���̒��_�����J��Ԃ�

			// ���f���̒��_���W����
			aVtxNo[nCntVtx] = *(D3DXVECTOR3*)pVtxBuff;

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += dwSizeFVF;
		}

		// ���f���̒��_�o�b�t�@���A�����b�N
		m_XFileData.pMesh->UnlockVertexBuffer();

		D3DXVECTOR3 vtxPos[3];		// ���_���W
		WORD dwIdx[3];				// ���_�̃C���f�b�N�X
		WORD* pIndex;				// �C���f�b�N�X�ւ̃|�C���^

		// �C���f�b�N�X�o�b�t�@�����b�N����
		m_XFileData.pMesh->LockIndexBuffer(0, (void**)&pIndex);

		for (int nCnt = 0; nCnt < m_XFileData.nNumFace; nCnt++)
		{
			for (int nCntIndex = 0; nCntIndex < 3; nCntIndex++)
			{
				// �C���f�b�N�X���擾����
				dwIdx[nCntIndex] = pIndex[(nCnt * 3) + nCntIndex];

				// ���_���W��ݒ肷��
				vtxPos[nCntIndex] = m_XFileData.vtxPos[dwIdx[nCntIndex]];

				if (useful::LineOuterProductXZ(m_pos + vtxPos[0], m_pos + vtxPos[1], *pos) >= 0 &&
					useful::LineOuterProductXZ(m_pos + vtxPos[1], m_pos + vtxPos[2], *pos) >= 0 &&
					useful::LineOuterProductXZ(m_pos + vtxPos[2], m_pos + vtxPos[0], *pos) >= 0)
				{ // �^�񒆂̋��E����荶���ɋ����ꍇ

					// �@���̌v�Z(���K��)
					//NormalizeVector(nor, pVtx[nVtxRightDown].pos, pVtx[nVtxLeftUp].pos, pVtx[nVtxLeftDown].pos);

					vec1 = (m_pos + vtxPos[1]) - (m_pos + vtxPos[0]);
					vec2 = (m_pos + vtxPos[2]) - (m_pos + vtxPos[0]);

					D3DXVec3Cross(&nor, &vec1, &vec2);

					D3DXVec3Normalize(&nor, &nor);

					if (nor.y != 0.0f)
					{ // �@����Y��0.0f�ȊO�̏ꍇ

						// ������ݒ肷��
						pos->y = (((pos->x - (m_pos.x + vtxPos[0].x)) * nor.x + (-(m_pos.y + vtxPos[0].y)) * nor.y + (pos->z - (m_pos.z + vtxPos[0].z)) * nor.z) * -1.0f) / nor.y;

						// ���n����� true �ɂ���
						bLand = true;
					}
				}
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_XFileData.pMesh->UnlockVertexBuffer();

		// ���_���W�̃��������������
		delete[] aVtxNo;
	}

	// ���n�����Ԃ�
	return bLand;
}

//========================
// X�t�@�C���̃f�[�^�̎擾����
//========================
CXFile::SXFile CModel::GetFileData(void)
{
	// X�t�@�C���̃f�[�^��Ԃ�
	return m_XFileData;
}

//========================
// �}�e���A�����̎擾����
//========================
D3DXMATERIAL CModel::GetMaterial(const int nCnt)
{
	// �|�C���^��錾
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	if (nCnt < (int)(m_XFileData.dwNumMat))
	{ // �ԍ����ő吔�����̏ꍇ

		// �}�e���A����Ԃ�
		return pMat[nCnt];
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);

		// �\���̃}�e���A����Ԃ�
		return pMat[0];
	}
}

//========================
// �e�N�X�`���̊��蓖�ď���
//========================
void CModel::BindTexture(const int nIdx, const int nCnt)
{
	if (nCnt < (int)(m_XFileData.dwNumMat))
	{ // �ԍ����ő吔�����̏ꍇ

		// �e�N�X�`���̔ԍ���ݒ肷��
		m_XFileData.m_nTexIdx[nCnt] = nIdx;
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//========================
// �ʒu�ݒ菈��
//========================
void CModel::SetPos(const D3DXVECTOR3& pos)
{
	// �ʒu��ݒ肷��
	m_pos = pos;
}

//========================
// �ʒu�擾����
//========================
D3DXVECTOR3 CModel::GetPos(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//========================
// �O��̈ʒu�ݒ菈��
//========================
void CModel::SetPosOld(const D3DXVECTOR3& posOld)
{
	// �O��̈ʒu��ݒ肷��
	m_posOld = posOld;
}

//========================
// �O��̈ʒu�擾����
//========================
D3DXVECTOR3 CModel::GetPosOld(void) const
{
	// �O��̈ʒu��Ԃ�
	return m_posOld;
}

//========================
// �����ݒ菈��
//========================
void CModel::SetRot(const D3DXVECTOR3& rot)
{
	// ������ݒ肷��
	m_rot = rot;
}

//========================
// �����擾����
//========================
D3DXVECTOR3 CModel::GetRot(void) const
{
	// ������ݒ肷��
	return m_rot;
}

//========================
// �T�C�Y�ݒ菈��
//========================
void CModel::SetScale(const D3DXVECTOR3& scale)
{
	// �g�嗦��ݒ肷��
	m_scale = scale;
}

//========================
// �T�C�Y�擾����
//========================
D3DXVECTOR3 CModel::GetScale(void) const
{
	// �g�嗦��Ԃ�
	return m_scale;
}

//========================
// �}�g���b�N�X�̎擾����
//========================
D3DXMATRIX CModel::GetMatrix(void) const
{
	// �}�g���b�N�X�̏���Ԃ�
	return m_mtxWorld;
}

//========================
// �}�g���b�N�X�|�C���^�̎擾����
//========================
D3DXMATRIX* CModel::GetMatrixPoint(void)
{
	// �}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}

//========================
// �f�[�^�̐ݒ菈��(�t�@�C���f�[�^�̐ݒ��)
//========================
void CModel::SetFileData(const CXFile::SXFile filaData)
{
	// �t�@�C���̃f�[�^��������
	m_XFileData = filaData;
}

//========================
// ��������
//========================
CModel* CModel::Create(const TYPE type, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CModel* pModel = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pModel == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pModel = new CModel(type, priority);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pModel != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pModel->Init()))
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

	// ���f���̃|�C���^��Ԃ�
	return pModel;
}