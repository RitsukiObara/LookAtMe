//============================================
//
// �I�u�W�F�N�g�O�p3D�̃��C������[objectTri3D.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "objectTri3D.h"
#include "texture.h"
#include "renderer.h"
#include "useful.h"

//=========================================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================================
CObjectTri3D::CObjectTri3D(CObject::TYPE type, PRIORITY priority) : CObject(type, DIM_3D, priority)
{
	// �S�Ă̒l���N���A����
	m_pVtxBuff = nullptr;							// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_posOld = m_pos;								// �O��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �T�C�Y
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ���[���h�}�g���b�N�X
	m_nTexIdx = NONE_TEXIDX;						// �e�N�X�`���̃C���f�b�N�X
}

//=========================================
// �f�X�g���N�^
//=========================================
CObjectTri3D::~CObjectTri3D()
{

}

//===========================================
// �I�u�W�F�N�g3D�̏���������
//===========================================
HRESULT CObjectTri3D::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{ // �o�b�t�@�̐����Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posOld = m_pos;							// �O��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y

	VERTEX_3D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[1].pos = D3DXVECTOR3(0.0f, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(+m_size.x, -m_size.y, +m_size.z);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	if (m_pVtxBuff == nullptr)
	{ // ���_�o�b�t�@�̃|�C���^��NULL�̏ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �I�u�W�F�N�g3D�̏I������
//===========================================
void CObjectTri3D::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�̔j��

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �j������
	Release();
}

//===========================================
// �I�u�W�F�N�g3D�̍X�V����
//===========================================
void CObjectTri3D::Update(void)
{
	// ���_�̐ݒ菈��
	SetVertex();
}

//===========================================
// �I�u�W�F�N�g3D�̕`�揈��
//===========================================
void CObjectTri3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;						// �r���[�}�g���b�N�X�擾�p

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

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
		m_pVtxBuff,									// ���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D));							// ���_���\���̂̃T�C�Y

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		1);											// �`�悷��v���~�e�B�u��
}

//===========================================
// �`�揈��(���C�e�B���O����)
//===========================================
void CObjectTri3D::DrawLightOff(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;						// �r���[�}�g���b�N�X�擾�p

	// ���C�e�B���O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

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
		m_pVtxBuff,									// ���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D));							// ���_���\���̂̃T�C�Y

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		1);											// �`�悷��v���~�e�B�u��

	// ���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===========================================
// �e�N�X�`���̊��蓖�ď���
//===========================================
void CObjectTri3D::BindTexture(int nIdx)
{
	// �ԍ������蓖�Ă�
	m_nTexIdx = nIdx;
}

//===========================================
// �ʒu�ݒ菈��
//===========================================
void CObjectTri3D::SetPos(const D3DXVECTOR3& pos)
{
	// �ʒu��ݒ肷��
	m_pos = pos;
}

//===========================================
// �ʒu�擾����
//===========================================
D3DXVECTOR3 CObjectTri3D::GetPos(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//===========================================
// �O��̈ʒu�ݒ菈��
//===========================================
void CObjectTri3D::SetPosOld(const D3DXVECTOR3& posOld)
{
	// �O��̈ʒu��ݒ肷��
	m_posOld = posOld;
}

//===========================================
// �O��̈ʒu�擾����
//===========================================
D3DXVECTOR3 CObjectTri3D::GetPosOld(void) const
{
	// �O��̈ʒu��Ԃ�
	return m_posOld;
}

//===========================================
// �����ݒ菈��
//===========================================
void CObjectTri3D::SetRot(const D3DXVECTOR3& rot)
{
	// ������ݒ肷��
	m_rot = rot;
}

//===========================================
// �����擾����
//===========================================
D3DXVECTOR3 CObjectTri3D::GetRot(void) const
{
	// ������Ԃ�
	return m_rot;
}

//===========================================
// �T�C�Y�ݒ菈��
//===========================================
void CObjectTri3D::SetSize(const D3DXVECTOR3& size)
{
	// �T�C�Y��ݒ肷��
	m_size = size;
}

//===========================================
// �T�C�Y�擾����
//===========================================
D3DXVECTOR3 CObjectTri3D::GetSize(void) const
{
	// �T�C�Y��Ԃ�
	return m_size;
}

//===========================================
// �}�g���b�N�X�̎擾����
//===========================================
D3DXMATRIX CObjectTri3D::GetMatrix(void) const
{
	// ���[���h�}�g���b�N�X�̃f�[�^��Ԃ�
	return m_mtxWorld;
}

//===========================================
// �e�N�X�`���̃C���f�b�N�X�̎擾����
//===========================================
int CObjectTri3D::GetTexIdx(void) const
{
	// �e�N�X�`���̃C���f�b�N�X��Ԃ�
	return m_nTexIdx;
}

//===========================================
// ���_���̐ݒ�
//===========================================
void CObjectTri3D::SetVertex(void)
{
	VERTEX_3D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[1].pos = D3DXVECTOR3(0.0f, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(+m_size.x, -m_size.y, +m_size.z);

	D3DXVECTOR3 nor[3] = {};

	// �@���̌v�Z(���K��)
	useful::NormalizeVector(&nor[0], pVtx[2].pos, pVtx[1].pos, pVtx[0].pos);
	useful::NormalizeVector(&nor[1], pVtx[0].pos, pVtx[2].pos, pVtx[1].pos);
	useful::NormalizeVector(&nor[2], pVtx[1].pos, pVtx[0].pos, pVtx[2].pos);

	// �@�����W�̐ݒ�
	pVtx[0].nor = nor[0];
	pVtx[1].nor = nor[1];
	pVtx[2].nor = nor[2];

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_���̐ݒ�(���ߑł�)
//===========================================
void CObjectTri3D::SetVertexHardCoding(const D3DXVECTOR3& Left, const D3DXVECTOR3& Up, const D3DXVECTOR3& Right)
{
	VERTEX_3D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-Left.x, -Left.y, -Left.z);
	pVtx[1].pos = D3DXVECTOR3(0.0f, +Up.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(+Right.x, -Right.y, +Right.z);

	D3DXVECTOR3 nor[3] = {};

	// �@���̌v�Z(���K��)
	useful::NormalizeVector(&nor[0], pVtx[2].pos, pVtx[1].pos, pVtx[0].pos);
	useful::NormalizeVector(&nor[1], pVtx[0].pos, pVtx[2].pos, pVtx[1].pos);
	useful::NormalizeVector(&nor[2], pVtx[1].pos, pVtx[0].pos, pVtx[2].pos);

	// �@�����W�̐ݒ�
	pVtx[0].nor = nor[0];
	pVtx[1].nor = nor[1];
	pVtx[2].nor = nor[2];

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_�J���[�̐ݒ菈��
//===========================================
void CObjectTri3D::SetVtxColor(const D3DXCOLOR& col)
{
	VERTEX_3D * pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_�e�N�X�`���̐ݒ菈��(�c�̈��Ԋu�̃e�N�X�`���z��)
//===========================================
void CObjectTri3D::SetVtxTexture(void)
{
	VERTEX_3D * pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ��������
//===========================================
CObjectTri3D* CObjectTri3D::Create(const TYPE type, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CObjectTri3D* pObject3D = nullptr;	// �I�u�W�F�N�g3D�̃C���X�^���X�𐶐�

	if (pObject3D == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pObject3D = new CObjectTri3D(type, priority);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pObject3D != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pObject3D->Init()))
		{ // ���s�����ꍇ

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

	// �I�u�W�F�N�g3D�̃|�C���^��Ԃ�
	return pObject3D;
}