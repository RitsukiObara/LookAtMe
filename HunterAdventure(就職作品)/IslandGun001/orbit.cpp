//=================================================================================
//
//	�O�Տ��� [orbit.cpp]
//	Author�F��������
//
//=================================================================================
//*********************************************************************************
//	�C���N���[�h�t�@�C��
//*********************************************************************************
#include "main.h"
#include "manager.h"
#include "orbit.h"
#include "renderer.h"
#include "texture.h"

//---------------------------------------------------------------------------------
// �������O���
//---------------------------------------------------------------------------------
namespace
{
	const int ORBIT_HEIGHT_VTX = 2;			// �O�Ղ̏c��
	const float ORBIT_ALPHA_SUB = 0.02f;	// �����x�̌��Z��
}

//=================================
// �R���X�g���N�^
//=================================
COrbit::COrbit() : CObject(CObject::TYPE_ORBIT, DIM_3D, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_pVtxBuff = nullptr;				// ���_�o�b�t�@�ւ̃|�C���^
	m_pMtxParent = nullptr;				// �e�̃}�g���b�N�X�ւ̃|�C���^
	m_nTexIdx = NONE_TEXIDX;			// �e�N�X�`���̃C���f�b�N�X

	for (int nCnt = 0; nCnt < MATRIXPLACE_MAX; nCnt++)
	{
		m_aOffset[nCnt] = NONE_D3DXVECTOR3;			// ���[�̃I�t�Z�b�g
		m_aCol[nCnt] = NONE_D3DXCOLOR;				// ���[�̊�̐F
	}

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		m_aPosPoint[nCntVtx] = NONE_D3DXVECTOR3;	// �v�Z��̊e���_���W
		m_aColPoint[nCntVtx] = NONE_D3DXCOLOR;		// �v�Z��̊e���_�J���[
	}
}

//=================================
// �f�X�g���N�^
//=================================
COrbit::~COrbit()
{

}

//=================================
//	�O�Ղ̏���������
//=================================
HRESULT COrbit::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// �S�Ă̒l������������
	m_pVtxBuff = nullptr;				// ���_�o�b�t�@�ւ̃|�C���^
	m_pMtxParent = nullptr;				// �e�̃}�g���b�N�X�ւ̃|�C���^
	m_nTexIdx = NONE_TEXIDX;			// �e�N�X�`���̃C���f�b�N�X

	for (int nCnt = 0; nCnt < MATRIXPLACE_MAX; nCnt++)
	{
		m_aOffset[nCnt] = NONE_D3DXVECTOR3;			// ���[�̃I�t�Z�b�g
		m_aCol[nCnt] = NONE_D3DXCOLOR;				// ���[�̊�̐F
	}

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		m_aPosPoint[nCntVtx] = NONE_D3DXVECTOR3;	// �v�Z��̊e���_���W
		m_aColPoint[nCntVtx] = NONE_D3DXCOLOR;		// �v�Z��̊e���_�J���[
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * MAX_ORBIT_VTX,	// �K�v���_��
		D3DUSAGE_WRITEONLY,					// �g�p���@
		FVF_VERTEX_3D,						// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// �������̎w��
		&m_pVtxBuff,						// ���_�o�b�t�@�ւ̃|�C���^
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWid = 0; nCntWid < (int)(MAX_ORBIT_VTX * 0.5f); nCntWid++)
	{
		for (int nCntDep = 1; nCntDep >= 0; nCntDep--)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = NONE_D3DXVECTOR3;

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = NONE_D3DXCOLOR;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			(
				(float)(nCntWid * 0.05f),
				(float)((nCntDep) % 2)
			);

			pVtx++;				// ���_�f�[�^��i�߂�
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ������Ԃ�
	return S_OK;
}

//=================================
//	�O�Ղ̏I������
//=================================
void COrbit::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{ // �ϐ� (g_pVtxBuffOrbit) ��NULL�ł͂Ȃ��ꍇ

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �j������
	Release();
}

//=================================
//	�O�Ղ̍X�V����
//=================================
void COrbit::Update(void)
{

}

//=================================
//	�O�Ղ̕`�揈��
//=================================
void COrbit::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();
	VERTEX_3D *pVtx;				// ���_���ւ̃|�C���^

	D3DXMATRIX mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxUnit;				// ���_�̃}�g���b�N�X

	// ���C�e�B���O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �J�����O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �}�g���b�N�X���쐬��	�e�̃}�g���b�N�X�Ɗ|�����킹��
	for (int nCntmtx = 0; nCntmtx < MATRIXPLACE_MAX; nCntmtx++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_aMtxWorldPoint[nCntmtx]);

		switch (nCntmtx)
		{
		case MATRIXPLACE_BOTTOM:		// ���̃I�t�Z�b�g

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntmtx].x, m_aOffset[nCntmtx].y, m_aOffset[nCntmtx].z);
			D3DXMatrixMultiply(&m_aMtxWorldPoint[nCntmtx], &m_aMtxWorldPoint[nCntmtx], &mtxTrans);

			break;							// �����o��

		case MATRIXPLACE_TOP:			// ��̃I�t�Z�b�g

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntmtx].x, m_aOffset[nCntmtx].y, m_aOffset[nCntmtx].z);
			D3DXMatrixMultiply(&m_aMtxWorldPoint[nCntmtx], &m_aMtxWorldPoint[nCntmtx], &mtxTrans);

			break;							// �����o��
		}

		// �e�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply
		(
			&m_aMtxWorldPoint[nCntmtx],
			&m_aMtxWorldPoint[nCntmtx],
			m_pMtxParent
		);
	}

	// ���_���W�ƒ��_�J���[�����炷
	for (int nCnt = MAX_ORBIT_VTX - (ORBIT_HEIGHT_VTX + 1); nCnt >= 0; nCnt--)
	{ // ���_�������炷

		// ���_���W�̃f�[�^��2�����炷
		m_aPosPoint[nCnt + ORBIT_HEIGHT_VTX] = m_aPosPoint[nCnt];

		// ���_�J���[�̃f�[�^��2�����炷
		m_aColPoint[nCnt + ORBIT_HEIGHT_VTX] = m_aColPoint[nCnt];
	}

	// �ŏ��̒��_���W�ƒ��_�J���[��������
	for (int nCntNew = 0; nCntNew < MATRIXPLACE_MAX; nCntNew++)
	{ // �}�g���b�N�X�̐��J��Ԃ�

		// ���_���W��������
		m_aPosPoint[nCntNew].x = m_aMtxWorldPoint[nCntNew]._41;
		m_aPosPoint[nCntNew].y = m_aMtxWorldPoint[nCntNew]._42;
		m_aPosPoint[nCntNew].z = m_aMtxWorldPoint[nCntNew]._43;

		// �F��������
		m_aColPoint[nCntNew] = m_aCol[nCntNew];
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		// ���_���W�̐ݒ�
		pVtx[nCntVtx].pos = m_aPosPoint[nCntVtx];
		pVtx[nCntVtx].col = m_aColPoint[nCntVtx];
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxUnit);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxUnit);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		m_pVtxBuff,						//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D));				//���_���\���̂̃T�C�Y

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
		0,													//�`�悷��ŏ��̒��_�C���f�b�N�X
		MAX_ORBIT_VTX - 2);									//�`�悷��v���~�e�B�u��

	// �J�����O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//================================
// �e�N�X�`���̊��蓖�ď���
//================================
void COrbit::BindTexture(int nIdx)
{
	// �e�N�X�`�������蓖�Ă�
	m_nTexIdx = nIdx;
}

//================================
// ���̐ݒ菈��
//================================
void COrbit::SetData(D3DXMATRIX *mtx, const D3DXVECTOR3 &pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const int nTexIdx, const TYPE type)
{
	// �S�Ă̒l��ݒ肷��
	m_pMtxParent = mtx;			// �e�̃}�g���b�N�X�ւ̃|�C���^
	m_nTexIdx = nTexIdx;		// �e�N�X�`���̃C���f�b�N�X

	m_aOffset[MATRIXPLACE_BOTTOM] = OffSet1;	// ���[�̃I�t�Z�b�g
	m_aOffset[MATRIXPLACE_TOP] = OffSet2;		// ���[�̃I�t�Z�b�g

	for (int nCntSet = 0; nCntSet < MATRIXPLACE_MAX; nCntSet++)
	{
		// ���̏�����
		m_aCol[nCntSet] = NONE_D3DXCOLOR;								// ���[�̕W���̐F
		ZeroMemory(&m_aMtxWorldPoint[nCntSet], sizeof(D3DXMATRIX));		// ���[�̃��[���h�}�g���b�N�X
	}

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		// ���̐ݒ�
		m_aColPoint[nCntVtx] = NONE_D3DXCOLOR;		// �v�Z��̊e���_�J���[
		m_aPosPoint[nCntVtx] = pos;					// �v�Z��̊e���_���W
	}

	// ��ނ�ݒ肷��
	SetType(type);
}

//================================
// ���̐ݒ菈��
//================================
void COrbit::SetData(D3DXMATRIX *mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const char* pTextureName, const TYPE type)
{
	// �S�Ă̒l��ݒ肷��
	m_pMtxParent = mtx;			// �e�̃}�g���b�N�X�ւ̃|�C���^
	m_nTexIdx = CManager::Get()->GetTexture()->Regist(pTextureName);	// �e�N�X�`���̃C���f�b�N�X

	m_aOffset[MATRIXPLACE_BOTTOM] = OffSet1;	// ���[�̃I�t�Z�b�g
	m_aOffset[MATRIXPLACE_TOP] = OffSet2;		// ���[�̃I�t�Z�b�g

	for (int nCntSet = 0; nCntSet < MATRIXPLACE_MAX; nCntSet++)
	{
		// ���̏�����
		m_aCol[nCntSet] = NONE_D3DXCOLOR;								// ���[�̕W���̐F
		ZeroMemory(&m_aMtxWorldPoint[nCntSet], sizeof(D3DXMATRIX));		// ���[�̃��[���h�}�g���b�N�X
	}

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		// ���̐ݒ�
		m_aColPoint[nCntVtx] = NONE_D3DXCOLOR;		// �v�Z��̊e���_�J���[
		m_aPosPoint[nCntVtx] = pos;					// �v�Z��̊e���_���W
	}

	// ��ނ�ݒ肷��
	SetType(type);
}

//=================================
// ��������(�e�N�X�`���ԍ�)
//=================================
COrbit* COrbit::Create(D3DXMATRIX *mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const int nTexIdx, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	COrbit* pOrbit = nullptr;	// �O�Ղ̃C���X�^���X�𐶐�

	if (pOrbit == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pOrbit = new COrbit;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pOrbit != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pOrbit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// �S���_�̈ʒu�̐ݒ菈��
		pOrbit->SetData(mtx, pos, OffSet1, OffSet2, nTexIdx, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �O�Ղ̃|�C���^��Ԃ�
	return pOrbit;
}

//=================================
// ��������(�e�N�X�`���̖��O)
//=================================
COrbit* COrbit::Create(D3DXMATRIX *mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const char* pTextureName, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	COrbit* pOrbit = nullptr;	// �O�Ղ̃C���X�^���X�𐶐�

	if (pOrbit == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pOrbit = new COrbit;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pOrbit != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pOrbit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// �S���_�̈ʒu�̐ݒ菈��
		pOrbit->SetData(mtx, pos, OffSet1, OffSet2, pTextureName, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �O�Ղ̃|�C���^��Ԃ�
	return pOrbit;
}

//=================================
// �ʒu�̃��Z�b�g����
//=================================
void COrbit::PosReset(void)
{
	VERTEX_3D* pVtx;				// ���_���ւ̃|�C���^

	D3DXMATRIX mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxUnit;				// ���_�̃}�g���b�N�X

	// �}�g���b�N�X���쐬��	�e�̃}�g���b�N�X�Ɗ|�����킹��
	for (int nCntmtx = 0; nCntmtx < MATRIXPLACE_MAX; nCntmtx++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_aMtxWorldPoint[nCntmtx]);

		switch (nCntmtx)
		{
		case MATRIXPLACE_BOTTOM:		// ���̃I�t�Z�b�g

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntmtx].x, m_aOffset[nCntmtx].y, m_aOffset[nCntmtx].z);
			D3DXMatrixMultiply(&m_aMtxWorldPoint[nCntmtx], &m_aMtxWorldPoint[nCntmtx], &mtxTrans);

			break;							// �����o��

		case MATRIXPLACE_TOP:			// ��̃I�t�Z�b�g

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntmtx].x, m_aOffset[nCntmtx].y, m_aOffset[nCntmtx].z);
			D3DXMatrixMultiply(&m_aMtxWorldPoint[nCntmtx], &m_aMtxWorldPoint[nCntmtx], &mtxTrans);

			break;							// �����o��
		}

		// �e�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply
		(
			&m_aMtxWorldPoint[nCntmtx],
			&m_aMtxWorldPoint[nCntmtx],
			m_pMtxParent
		);
	}

	// �ŏ��̒��_���W�ƒ��_�J���[��������
	for (int nCntNew = 0; nCntNew < MAX_ORBIT_VTX; nCntNew++)
	{ // �}�g���b�N�X�̐��J��Ԃ�

		// ���_���W��������
		m_aPosPoint[nCntNew].x = m_aMtxWorldPoint[nCntNew % 2]._41;
		m_aPosPoint[nCntNew].y = m_aMtxWorldPoint[nCntNew % 2]._42;
		m_aPosPoint[nCntNew].z = m_aMtxWorldPoint[nCntNew % 2]._43;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		// ���_���W�̐ݒ�
		pVtx[nCntVtx].pos = m_aPosPoint[nCntVtx];
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}