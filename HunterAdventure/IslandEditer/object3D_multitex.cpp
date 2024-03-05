//============================================
//
// �}���`�e�N�X�`���̃I�u�W�F�N�g3D�̃��C������[object3D_multitex.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "main.h"
#include "manager.h"
#include "object3D_multitex.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=========================================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================================
CObject3DMultiTex::CObject3DMultiTex(CObject::TYPE type, PRIORITY priority) : CObject(type, DIM_3D, priority)
{
	// �S�Ă̒l���N���A����
	m_pTexStyle = nullptr;							// �e�N�X�`���`��̏��
	m_pVtxBuff = nullptr;							// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_posOld = m_pos;								// �O��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �T�C�Y
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ���[���h�}�g���b�N�X
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_aTexIdx[nCnt] = NONE_TEXIDX;				// �e�N�X�`���̃C���f�b�N�X
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CObject3DMultiTex::~CObject3DMultiTex()
{

}

//===========================================
// �I�u�W�F�N�g3D�̏���������
//===========================================
HRESULT CObject3DMultiTex::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULTI) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_MULTI,
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
	m_pTexStyle = nullptr;						// �e�N�X�`���`��̏��
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posOld = m_pos;							// �O��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y

	VERTEX_3D_MULTI * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].texM = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].texM = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].texM = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].texM = D3DXVECTOR2(1.0f, 1.0f);

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
void CObject3DMultiTex::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{ // ���_�o�b�t�@�̔j��

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pTexStyle != nullptr)
	{ // �e�N�X�`���`�悪 NULL ����Ȃ��ꍇ

		delete m_pTexStyle;
		m_pTexStyle = nullptr;
	}

	// �j������
	Release();
}

//===========================================
// �I�u�W�F�N�g3D�̍X�V����
//===========================================
void CObject3DMultiTex::Update(void)
{
	// ���_�̐ݒ菈��
	SetVertex();
}

//===========================================
// �I�u�W�F�N�g3D�̕`�揈��
//===========================================
void CObject3DMultiTex::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;						// �r���[�}�g���b�N�X�擾�p

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

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

	if (m_pTexStyle != nullptr)
	{ // �e�N�X�`���`�悪 NULL ����Ȃ��ꍇ

		// �`��I���̏���
		m_pTexStyle->DispStart();
	}

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		m_pVtxBuff,									// ���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D_MULTI));					// ���_���\���̂̃T�C�Y

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(nCnt, CManager::Get()->GetTexture()->GetAddress(m_aTexIdx[nCnt]));
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);											// �`�悷��v���~�e�B�u��

	if (m_pTexStyle != nullptr)
	{ // �e�N�X�`���`�悪 NULL ����Ȃ��ꍇ

		// �`��I���̏���
		m_pTexStyle->DispEnd();
	}
}

//===========================================
// �`�揈��(���C�e�B���O����)
//===========================================
void CObject3DMultiTex::DrawLightOff(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;						// �r���[�}�g���b�N�X�擾�p

	// ���C�e�B���O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

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

	if (m_pTexStyle != nullptr)
	{ // �e�N�X�`���`�悪 NULL ����Ȃ��ꍇ

		// �`��I���̏���
		m_pTexStyle->DispStart();
	}

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		m_pVtxBuff,									// ���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D_MULTI));							// ���_���\���̂̃T�C�Y

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(nCnt, CManager::Get()->GetTexture()->GetAddress(m_aTexIdx[nCnt]));
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);											// �`�悷��v���~�e�B�u��

	// ���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_pTexStyle != nullptr)
	{ // �e�N�X�`���`�悪 NULL ����Ȃ��ꍇ

		// �`��I���̏���
		m_pTexStyle->DispEnd();
	}
}

//===========================================
// �e�N�X�`���̊��蓖�ď���
//===========================================
void CObject3DMultiTex::BindTexture(int nIdx, int nCnt)
{
	// �ԍ������蓖�Ă�
	m_aTexIdx[nCnt] = nIdx;
}

//===========================================
// �ʒu�ݒ菈��
//===========================================
void CObject3DMultiTex::SetPos(const D3DXVECTOR3& pos)
{
	// �ʒu��ݒ肷��
	m_pos = pos;
}

//===========================================
// �ʒu�擾����
//===========================================
D3DXVECTOR3 CObject3DMultiTex::GetPos(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//===========================================
// �O��̈ʒu�ݒ菈��
//===========================================
void CObject3DMultiTex::SetPosOld(const D3DXVECTOR3& posOld)
{
	// �O��̈ʒu��ݒ肷��
	m_posOld = posOld;
}

//===========================================
// �O��̈ʒu�擾����
//===========================================
D3DXVECTOR3 CObject3DMultiTex::GetPosOld(void) const
{
	// �O��̈ʒu��Ԃ�
	return m_posOld;
}

//===========================================
// �����ݒ菈��
//===========================================
void CObject3DMultiTex::SetRot(const D3DXVECTOR3& rot)
{
	// ������ݒ肷��
	m_rot = rot;
}

//===========================================
// �����擾����
//===========================================
D3DXVECTOR3 CObject3DMultiTex::GetRot(void) const
{
	// ������Ԃ�
	return m_rot;
}

//===========================================
// �T�C�Y�ݒ菈��
//===========================================
void CObject3DMultiTex::SetSize(const D3DXVECTOR3& size)
{
	// �T�C�Y��ݒ肷��
	m_size = size;
}

//===========================================
// �T�C�Y�擾����
//===========================================
D3DXVECTOR3 CObject3DMultiTex::GetSize(void) const
{
	// �T�C�Y��Ԃ�
	return m_size;
}

//===========================================
// �}�g���b�N�X�̎擾����
//===========================================
D3DXMATRIX CObject3DMultiTex::GetMatrix(void) const
{
	// ���[���h�}�g���b�N�X�̃f�[�^��Ԃ�
	return m_mtxWorld;
}

//===========================================
// �}�g���b�N�X�|�C���^�̎擾����
//===========================================
D3DXMATRIX* CObject3DMultiTex::GetMatrixPoint(void)
{
	// ���[���h�}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}

//===========================================
// ��ނ̐ݒ菈��
//===========================================
void CObject3DMultiTex::SetType(const TYPE type)
{
	if (m_pTexStyle != nullptr)
	{ // �e�N�X�`���`��̏�� NULL �̏ꍇ

		// ���������J������
		delete m_pTexStyle;
		m_pTexStyle = nullptr;							// �e�N�X�`���`��̏��
	}

	switch (type)
	{
	case CObject3DMultiTex::TYPE_NONE:

		// ����ԂȂ̂œ��ɂȂ�

		break;

	case CObject3DMultiTex::TYPE_PULL_OUT_WHITE:

		// �^����(�w�i��)
		m_pTexStyle = new CPulloutWhite;

		break;

	case CObject3DMultiTex::TYPE_BACKGROUND_WHITE:

		// �w�i�؂蔲��(�w�i��)
		m_pTexStyle = new CBackGroundWhite;

		break;

	case CObject3DMultiTex::TYPE_PULL_OUT_BLACK:

		// �^����(�w�i��)
		m_pTexStyle = new CPulloutBlack;

		break;

	case CObject3DMultiTex::TYPE_BACKGROUND_BLACK:

		// �w�i�؂蔲��(�w�i��)
		m_pTexStyle = new CBackGroundBlack;

		break;

	case CObject3DMultiTex::TYPE_LIGHT_BLACK:

		// �P���e�N�X�`��(���w�i)
		m_pTexStyle = new CLightBlack;

		break;

	case CObject3DMultiTex::TYPE_NEGA_BLACK:

		// �l�K�e�N�X�`��(���w�i)
		m_pTexStyle = new CNegaBlack;

		break;

	case CObject3DMultiTex::TYPE_MONO_TRANS:

		// �����w�i(������)
		m_pTexStyle = new CMonoTrans;

		break;

	case CObject3DMultiTex::TYPE_PULL_OUT_SP:

		// �^����(���S����)
		m_pTexStyle = new CPulloutSpecial;

		break;

	case CObject3DMultiTex::TYPE_LIGHT_PULL_OUT:

		// �^�������P���e�N�X�`��
		m_pTexStyle = new CLightPullout;

		break;

	case CObject3DMultiTex::TYPE_LIGHT_SP:

		// �P���e�N�X�`��(���S����)
		m_pTexStyle = new CLightSpecial;

		break;

	case CObject3DMultiTex::TYPE_BACKGROUND_SP:

		// �w�i�؂蔲��(���S����)
		m_pTexStyle = new CBackGroundSpecial;

		break;

	case CObject3DMultiTex::TYPE_NEGA_SP:

		// �l�K�e�N�X�`��(���S����)
		m_pTexStyle = new CNegaSpecial;

		break;

	case CObject3DMultiTex::TYPE_BACKGROUND_NEGA:

		// �l�K�e�N�X�`��(�w�i�؂蔲��)
		m_pTexStyle = new CBackGroundNega;

		break;

	case CObject3DMultiTex::TYPE_FUSION:

		// ����
		m_pTexStyle = new CFusion;

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//===========================================
// �e�N�X�`���̃C���f�b�N�X�̎擾����
//===========================================
int CObject3DMultiTex::GetTexIdx(const int nCnt) const
{
	// �e�N�X�`���̃C���f�b�N�X��Ԃ�
	return m_aTexIdx[nCnt];
}

//===========================================
// ���_���̐ݒ�
//===========================================
void CObject3DMultiTex::SetVertex(void)
{
	VERTEX_3D_MULTI * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

	D3DXVECTOR3 nor[4] = {};

	// �@���̌v�Z(���K��)
	useful::NormalizeVector(&nor[0], pVtx[2].pos, pVtx[1].pos, pVtx[0].pos);
	useful::NormalizeVector(&nor[1], pVtx[0].pos, pVtx[3].pos, pVtx[1].pos);
	useful::NormalizeVector(&nor[2], pVtx[3].pos, pVtx[0].pos, pVtx[2].pos);
	useful::NormalizeVector(&nor[3], pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

	// �@�����W�̐ݒ�
	pVtx[0].nor = nor[0];
	pVtx[1].nor = nor[1];
	pVtx[2].nor = nor[2];
	pVtx[3].nor = nor[3];

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_�J���[�̐ݒ菈��
//===========================================
void CObject3DMultiTex::SetVtxColor(const D3DXCOLOR& col)
{
	VERTEX_3D_MULTI * pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_�e�N�X�`���̐ݒ菈��(�c�̈��Ԋu�̃e�N�X�`���z��)
//===========================================
void CObject3DMultiTex::SetVtxTextureWidth(const D3DXVECTOR2& size, int nCnt)
{
	// ���[�J���ϐ��錾
	float fTexWidth = (float)(size.x / size.y);

	VERTEX_3D_MULTI * pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nCnt == 0)
	{ // 0�̏ꍇ

		// ���_�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(fTexWidth, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(fTexWidth, 1.0f);
	}
	else
	{ // ��L�ȊO

		// ���_�e�N�X�`���̐ݒ�
		pVtx[0].texM = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].texM = D3DXVECTOR2(fTexWidth, 0.0f);
		pVtx[2].texM = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].texM = D3DXVECTOR2(fTexWidth, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_�e�N�X�`���̐ݒ菈��(���̈��Ԋu�̃e�N�X�`���z��)
//===========================================
void CObject3DMultiTex::SetVtxTextureHeight(const D3DXVECTOR2& size, int nCnt)
{
	// ���[�J���ϐ��錾
	float fTexHeight = (float)(size.y / size.x);

	VERTEX_3D_MULTI * pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nCnt == 0)
	{ // 0�̏ꍇ

		// ���_�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, fTexHeight);
		pVtx[3].tex = D3DXVECTOR2(1.0f, fTexHeight);
	}
	else
	{ // ��L�ȊO

		// ���_�e�N�X�`���̐ݒ�
		pVtx[0].texM = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].texM = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].texM = D3DXVECTOR2(0.0f, fTexHeight);
		pVtx[3].texM = D3DXVECTOR2(1.0f, fTexHeight);
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_�e�N�X�`���̐ݒ菈��(�A�j���[�V������)
//===========================================
void CObject3DMultiTex::SetVtxTextureAnim(const D3DXVECTOR2& anim, int nCnt)
{
	VERTEX_3D_MULTI* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nCnt == 0)
	{ // 0�̏ꍇ

		// ���_�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(anim.x, anim.y);
		pVtx[1].tex = D3DXVECTOR2(anim.x + 1.0f, anim.y);
		pVtx[2].tex = D3DXVECTOR2(anim.x, anim.y + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(anim.x + 1.0f, anim.y + 1.0f);
	}
	else
	{ // ��L�ȊO

		// ���_�e�N�X�`���̐ݒ�
		pVtx[0].texM = D3DXVECTOR2(anim.x, anim.y);
		pVtx[1].texM = D3DXVECTOR2(anim.x + 1.0f, anim.y);
		pVtx[2].texM = D3DXVECTOR2(anim.x, anim.y + 1.0f);
		pVtx[3].texM = D3DXVECTOR2(anim.x + 1.0f, anim.y + 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ��������
//===========================================
CObject3DMultiTex* CObject3DMultiTex::Create(const CObject::TYPE type, const PRIORITY priority, const TYPE typeTex)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CObject3DMultiTex* pObject3D = nullptr;	// �I�u�W�F�N�g3D�̃C���X�^���X�𐶐�

	if (pObject3D == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pObject3D = new CObject3DMultiTex(type, priority);
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

		// ��ނ̐ݒ菈��
		pObject3D->SetType(typeTex);
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