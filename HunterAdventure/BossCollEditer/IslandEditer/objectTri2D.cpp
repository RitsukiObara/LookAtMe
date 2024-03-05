//============================================
//
// �I�u�W�F�N�g�O�p�`2D�̃��C������[objectTri2D.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "objectTri2D.h"
#include "renderer.h"
#include "texture.h"

//=========================================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================================
CObjectTri2D::CObjectTri2D(CObject::TYPE type, PRIORITY priority) : CObject(type, DIM_2D, priority)
{
	// �S�Ă̒l���N���A����
	m_pVtxBuff = nullptr;			// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = NONE_D3DXVECTOR3;		// �ʒu
	m_posOld = m_pos;				// �O��̈ʒu
	m_rot = NONE_D3DXVECTOR3;		// ����
	m_size = NONE_D3DXVECTOR3;		// �T�C�Y
	m_fLength = 0.0f;				// ����
	m_nTexIdx = NONE_TEXIDX;		// �e�N�X�`���̃C���f�b�N�X
}

//=========================================
// �f�X�g���N�^
//=========================================
CObjectTri2D::~CObjectTri2D()
{

}

//===========================================
// �O�p�|���S���̏���������
//===========================================
HRESULT CObjectTri2D::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
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
	m_pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		// �ʒu
	m_posOld = m_pos;				// �O��̈ʒu
	m_rot = NONE_D3DXVECTOR3;		// ����
	m_size = D3DXVECTOR3(30.0f, 50.0f, 0.0f);	// �T�C�Y
	m_fLength = (m_size.x + m_size.y) * 0.5f;	// ����
	m_nTexIdx = NONE_TEXIDX;		// �e�N�X�`���̃C���f�b�N�X

	// ���_���̐ݒ菈��
	SetVertexAll();

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
// �O�p�|���S���̏I������
//===========================================
void CObjectTri2D::Uninit(void)
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
// �O�p�|���S���̍X�V����
//===========================================
void CObjectTri2D::Update(void)
{
	// ���_�ݒ菈��
	SetVertexRot();

	// �e�N�X�`���̐ݒ菈��
	SetVtxTexture();
}

//===========================================
// �O�p�|���S���̕`�揈��
//===========================================
void CObjectTri2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		m_pVtxBuff,								// ���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D));						// ���_���\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,	// �v���~�e�B�u�̎��
		0,										// �`�悷��ŏ��̒��_�C���f�b�N�X
		1);										// �`�悷��v���~�e�B�u��
}

//===========================================
// �e�N�X�`���̊��蓖�ď���
//===========================================
void CObjectTri2D::BindTexture(const int nIdx)
{
	// �e�N�X�`�������蓖�Ă�
	m_nTexIdx = nIdx;
}

//===========================================
// �ʒu�ݒ菈��
//===========================================
void CObjectTri2D::SetPos(const D3DXVECTOR3& pos)
{
	// �ʒu��ݒ肷��
	m_pos = pos;
}

//===========================================
// �ʒu�擾����
//===========================================
D3DXVECTOR3 CObjectTri2D::GetPos(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//===========================================
// �O��̈ʒu�ݒ菈��
//===========================================
void CObjectTri2D::SetPosOld(const D3DXVECTOR3& posOld)
{
	// �O��̈ʒu��ݒ肷��
	m_posOld = posOld;
}

//===========================================
// �O��̈ʒu�擾����
//===========================================
D3DXVECTOR3 CObjectTri2D::GetPosOld(void) const
{
	// �O��̈ʒu��Ԃ�
	return m_posOld;
}

//===========================================
// �����ݒ菈��
//===========================================
void CObjectTri2D::SetRot(const D3DXVECTOR3& rot)
{
	// ������ݒ肷��
	m_rot = rot;
}

//===========================================
// �����擾����
//===========================================
D3DXVECTOR3 CObjectTri2D::GetRot(void) const
{
	// ������Ԃ�
	return m_rot;
}

//===========================================
// �T�C�Y�ݒ菈��
//===========================================
void CObjectTri2D::SetSize(const D3DXVECTOR3& size)
{
	// �T�C�Y��ݒ肷��
	m_size = size;
}

//===========================================
// �T�C�Y�擾����
//===========================================
D3DXVECTOR3 CObjectTri2D::GetSize(void) const
{
	// �T�C�Y��Ԃ�
	return m_size;
}

//===========================================
// �����ݒ菈��
//===========================================
void CObjectTri2D::SetLength(void)
{
	// ������ݒ肷��
	m_fLength = (m_size.x + m_size.y) * 0.5f;
}

//===========================================
// �����擾����
//===========================================
float CObjectTri2D::GetLength(void) const
{
	// ������Ԃ�
	return m_fLength;
}

//===========================================
// �e�N�X�`���̃C���f�b�N�X�̎擾����
//===========================================
int CObjectTri2D::GetTexIdx(void) const
{
	// �e�N�X�`���̃C���f�b�N�X��Ԃ�
	return m_nTexIdx;
}

//===========================================
// ���_���̐ݒ�
//===========================================
void CObjectTri2D::SetVertex(void)
{
	VERTEX_2D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_���̐ݒ�(�����L��)
//===========================================
void CObjectTri2D::SetVertexRot(void)
{
	VERTEX_2D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI * 0.25f)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI * 0.25f)) * m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI) * m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI * 0.25f)) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI * 0.25f)) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_���̐ݒ�(�x�^�ł�)
//===========================================
void CObjectTri2D::SetVtxHardCoding(const D3DXVECTOR3& Left, const D3DXVECTOR3& Up, const D3DXVECTOR3& Right)
{
	VERTEX_2D* pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + Left.x, m_pos.y + Left.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + Up.x, m_pos.y + Up.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + Right.x, m_pos.y + Right.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_�̐F�ݒ菈��
//===========================================
void CObjectTri2D::SetVtxColor(const D3DXCOLOR& col)
{
	VERTEX_2D* pVtx;											//���_���ւ̃|�C���^

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
// �e�N�X�`���̐ݒ�
//===========================================
void CObjectTri2D::SetVtxTexture(void)
{
	VERTEX_2D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// �S�Ă̒��_���̐ݒ�
//===========================================
void CObjectTri2D::SetVertexAll(void)
{
	VERTEX_2D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ��������
//===========================================
CObjectTri2D* CObjectTri2D::Create(const CObject::TYPE type, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CObjectTri2D* pObject2D = nullptr;		// �O�p�|���S���̃C���X�^���X�𐶐�

	if (pObject2D == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pObject2D = new CObjectTri2D(type, priority);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pObject2D != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pObject2D->Init()))
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

	// �O�p�|���S���̃|�C���^��Ԃ�
	return pObject2D;
}