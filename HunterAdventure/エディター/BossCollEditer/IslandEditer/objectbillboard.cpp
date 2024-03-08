//============================================
//
// �r���{�[�h�̃��C������[billboard.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "objectbillboard.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//----------------------------------------------------------------��������CBillboard�̏���----------------------------------------------------------------

//=========================================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================================
CBillboard::CBillboard(CObject::TYPE type, PRIORITY priority) : CObject(type, DIM_3D, priority)
{
	// �S�Ă̒l���N���A����
	m_pos = NONE_D3DXVECTOR3;						// �ʒu
	m_posOld = m_pos;								// �O��̈ʒu
	m_size = NONE_D3DXVECTOR3;						// �T�C�Y
	m_pVtxBuff = nullptr;							// ���_�o�b�t�@�̃|�C���^
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ���[���h�}�g���b�N�X
	m_nTexIdx = NONE_TEXIDX;						// �e�N�X�`���̃C���f�b�N�X
	m_fRot = 0.0f;									// ����
	m_fAngle = 0.0f;								// ����
	m_fLength = 0.0f;								// ����
	m_bLookY = true;								// Y����
}

//=========================================
// �f�X�g���N�^
//=========================================
CBillboard::~CBillboard()
{

}

//===========================================
// �r���{�[�h�̏���������
//===========================================
HRESULT CBillboard::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
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
	m_pos = NONE_D3DXVECTOR3;	// �ʒu
	m_posOld = m_pos;			// �O��̈ʒu
	m_size = NONE_D3DXVECTOR3;	// �T�C�Y
	m_nTexIdx = NONE_TEXIDX;	// �e�N�X�`���̃C���f�b�N�X
	m_fRot = 0.0f;				// ����
	m_fAngle = 0.0f;			// ����
	m_fLength = 0.0f;			// ����
	m_bLookY = true;			// Y����

	VERTEX_3D * pVtx;											//���_���ւ̃|�C���^

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
// �r���{�[�h�̏I������
//===========================================
void CBillboard::Uninit(void)
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
// �r���{�[�h�̍X�V����
//===========================================
void CBillboard::Update(void)
{

}

//===========================================
// �r���{�[�h�̕`�揈��
//===========================================
void CBillboard::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����Ɍ����Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	if (m_bLookY == false)
	{ // Y���ɂ͌X���Ȃ��ꍇ

		// Y����]����ł�����
		m_mtxWorld._21 = 0.0f;
		m_mtxWorld._22 = 1.0f;
		m_mtxWorld._23 = 0.0f;
	}

	// �t�s���ł�����
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);											// �`�悷��v���~�e�B�u��

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// �A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Z�e�X�g�̗L��/�����ݒ�
}

//===========================================
// �`�揈��(���C�e�B���O����)
//===========================================
void CBillboard::DrawLightOff(const bool bZTest)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	// ���C�e�B���O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (bZTest == false)
	{ // Z�e�X�g�����Ȃ��ꍇ

		// Z�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�
	}

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����Ɍ����Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	if (m_bLookY == false)
	{ // Y���ɂ͌X���Ȃ��ꍇ

		// Y����]����ł�����
		m_mtxWorld._21 = 0.0f;
		m_mtxWorld._22 = 1.0f;
		m_mtxWorld._23 = 0.0f;
	}

	// �t�s���ł�����
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);											// �`�悷��v���~�e�B�u��

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// �A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Z�e�X�g�̗L��/�����ݒ�

	// ���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===========================================
// �`�揈��(�ʒu���炵)
//===========================================
void CBillboard::DrawShift(const D3DXVECTOR3 shift)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	// ���C�e�B���O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����Ɍ����Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	if (m_bLookY == false)
	{ // Y���ɂ͌X���Ȃ��ꍇ

		// Y����]����ł�����
		m_mtxWorld._21 = 0.0f;
		m_mtxWorld._22 = 1.0f;
		m_mtxWorld._23 = 0.0f;
	}

	// �t�s���ł�����
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + shift.x, m_pos.y + shift.y, m_pos.z + shift.z);
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
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);											// �`�悷��v���~�e�B�u��

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// �A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Z�e�X�g�̗L��/�����ݒ�

	// ���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===========================================
// ���_���̐ݒ�
//===========================================
void CBillboard::SetVertex(void)
{
	VERTEX_3D* pVtx;											// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_���̐ݒ�(�����L��)
//===========================================
void CBillboard::SetVertexRot(void)
{
	VERTEX_3D* pVtx;											// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = sinf(m_fRot - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[0].pos.y = cosf(m_fRot - D3DX_PI + m_fAngle) * -m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = sinf(m_fRot + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = cosf(m_fRot + D3DX_PI - m_fAngle) * -m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = sinf(m_fRot - m_fAngle) * m_fLength;
	pVtx[2].pos.y = cosf(m_fRot - m_fAngle) * -m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = sinf(m_fRot + m_fAngle) * m_fLength;
	pVtx[3].pos.y = cosf(m_fRot + m_fAngle) * -m_fLength;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_���̐ݒ�(�����_)
//===========================================
void CBillboard::SetVtxPosUnder(void)
{
	VERTEX_3D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, (m_size.y * 2), (m_size.z * 2));
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, (m_size.y * 2), (m_size.z * 2));
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, 0.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// �A�j���[�V�����̐ݒ�
//===========================================
void CBillboard::SetVtxAnim(const float fTex, const int nPattern)
{
	VERTEX_3D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(nPattern * fTex, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattern * fTex + fTex, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattern * fTex, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattern * fTex + fTex, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// �A�j���[�V�����̐ݒ�(���])
//===========================================
void CBillboard::SetVtxAnimRev(const float fTex, const int nPattern)
{
	VERTEX_3D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(nPattern * fTex + fTex, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattern * fTex, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattern * fTex + fTex, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattern * fTex, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// ���_�J���[�̐ݒ�
//===========================================
void CBillboard::SetVtxColor(D3DXCOLOR col)
{
	VERTEX_3D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================
// �e�N�X�`���̊��蓖�ď���
//===========================================
void CBillboard::BindTexture(int nIdx)
{
	// �e�N�X�`�������蓖�Ă�
	m_nTexIdx = nIdx;
}

//===========================================
// �ʒu�ݒ菈��
//===========================================
void CBillboard::SetPos(const D3DXVECTOR3& pos)
{
	// �ʒu��ݒ肷��
	m_pos = pos;
}

//===========================================
// �ʒu�擾����
//===========================================
D3DXVECTOR3 CBillboard::GetPos(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//===========================================
// �O��̈ʒu�ݒ菈��
//===========================================
void CBillboard::SetPosOld(const D3DXVECTOR3& posOld)
{
	// �O��̈ʒu��ݒ肷��
	m_posOld = posOld;
}

//===========================================
// �O��̈ʒu�擾����
//===========================================
D3DXVECTOR3 CBillboard::GetPosOld(void) const
{
	// �O��̈ʒu��Ԃ�
	return m_posOld;
}

//===========================================
// �����̐ݒ菈��
//===========================================
void CBillboard::SetRot(const float fRot)
{
	// ������ݒ肷��
	m_fRot = fRot;
}

//===========================================
// �����̎擾����
//===========================================
float CBillboard::GetRot(void) const
{
	// ������Ԃ�
	return m_fRot;
}

//===========================================
// �T�C�Y�ݒ菈��
//===========================================
void CBillboard::SetSize(const D3DXVECTOR3& size)
{
	// �T�C�Y��ݒ肷��
	m_size = size;
}

//===========================================
// �T�C�Y�擾����
//===========================================
D3DXVECTOR3 CBillboard::GetSize(void) const
{
	// �T�C�Y��Ԃ�
	return m_size;
}

//===========================================
// �����̐ݒ菈��
//===========================================
void CBillboard::SetAngle(void)
{
	//�Ίp���̊p�x���Z�o����
	m_fAngle = atan2f((m_size.x * 2), (m_size.y * 2));
}

//===========================================
// �����̐ݒ菈��
//===========================================
void CBillboard::SetLength(void)
{
	//�Ίp���̒������Z�o����
	m_fLength = sqrtf((m_size.x * 2) * (m_size.x * 2) + (m_size.y * 2) * (m_size.y * 2)) * 0.5f;
}

//===========================================
// �e�N�X�`���̃C���f�b�N�X�̎擾����
//===========================================
int CBillboard::GetTexIdx(void) const
{
	// �e�N�X�`���̃C���f�b�N�X��Ԃ�
	return m_nTexIdx;
}

//===========================================
// Y���󋵂̐ݒ菈��
//===========================================
void CBillboard::SetEnableLookY(const bool bLookY)
{
	// Y���󋵂�ݒ肷��
	m_bLookY = bLookY;
}

//===========================================
// Y���󋵂̎擾����
//===========================================
bool CBillboard::IsLookY(void) const
{
	// Y���󋵂�Ԃ�
	return m_bLookY;
}

//===========================================
// ��������
//===========================================
CBillboard* CBillboard::Create(const TYPE type, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBillboard* pBillboard = nullptr;	// �r���{�[�h�̃C���X�^���X�𐶐�

	if (pBillboard == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBillboard = new CBillboard(type, priority);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBillboard != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBillboard->Init()))
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

	// �r���{�[�h�̃|�C���^��Ԃ�
	return pBillboard;
}

//----------------------------------------------------------------��������CBillboardAnim�̏���----------------------------------------------------------------

//===========================================
// �I�[�o�[���[�h�R���X�g���N�^
//===========================================
CBillboardAnim::CBillboardAnim(CObject::TYPE type, PRIORITY priority) : CBillboard(type, priority)
{
	// �S�Ă̒l���N���A����
	m_nCounter = 0;					// �A�j���[�V�����J�E���^�[
	m_nReplayCount = 0;				// �Đ��J�E���g
	m_nPattern = 0;					// �A�j���[�V�����p�^�[��
	m_nReplayPattern = 0;			// �Đ��p�^�[��
	m_fTexPattern = 0.0f;			// �e�N�X�`���̃p�^�[��
}

//===========================================
// �f�X�g���N�^
//===========================================
CBillboardAnim::~CBillboardAnim()
{

}

//===========================================
// ����������
//===========================================
HRESULT CBillboardAnim::Init(void)
{
	// ������
	if (FAILED(CBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_nCounter = 0;				// �A�j���[�V�����J�E���^�[
	m_nPattern = 0;				// �A�j���[�V�����p�^�[��
	m_nReplayCount = 0;				// �Đ��J�E���g
	m_nReplayPattern = 0;			// �Đ��p�^�[��
	m_fTexPattern = 0.0f;			// �e�N�X�`���̍Đ��p�^�[��

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CBillboardAnim::Uninit(void)
{
	// �I������
	CBillboard::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CBillboardAnim::Update(void)
{
	// �J�E���g�����Z����
	m_nReplayCount++;

	if (m_nReplayCount % m_nCounter == 0)
	{ // �J�E���g���K��l�ɒB�����ꍇ

		// �Đ��J�E���g��0�ɂ���
		m_nReplayCount = 0;

		// �p�^�[�������ړ�����
		m_nReplayPattern = (m_nReplayPattern + 1) % m_nPattern;
	}

	// �e�N�X�`���̐ݒ菈��(�A�j���[�V�����o�[�W����)
	SetVtxAnim(m_fTexPattern, m_nReplayPattern);
}

//===========================================
// �`�揈��
//===========================================
void CBillboardAnim::Draw(void)
{
	// �`�揈��
	CBillboard::Draw();
}

//===========================================
// �`�揈��
//===========================================
void CBillboardAnim::DrawLightOff(const bool bZTest)
{
	// �`�揈��
	CBillboard::DrawLightOff(bZTest);
}

//===========================================
// �p�^�[���̎擾����
//===========================================
int CBillboardAnim::GetPattern(void) const
{
	// ���݂̃p�^�[����Ԃ�
	return m_nReplayPattern;
}

//===========================================
// �e�N�X�`���̃p�^�[���̎擾����
//===========================================
float CBillboardAnim::GetTexPattern(void) const
{
	// �e�N�X�`���̃p�^�[����Ԃ�
	return m_fTexPattern;
}

//===========================================
// �A�j���[�V�����̐ݒ菈��
//===========================================
void CBillboardAnim::SetAnim(const int nCount, const int nPattern)
{
	// ����ݒ肷��
	m_nCounter = nCount;			// �A�j���[�V�����J�E���^�[
	m_nPattern = nPattern;			// �A�j���[�V�����p�^�[��
	m_fTexPattern = 1.0f / nPattern;	// �e�N�X�`���̍Đ��p�^�[��
}

//===========================================
// ��������
//===========================================
CBillboardAnim* CBillboardAnim::Create(const TYPE type, const PRIORITY priority)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBillboardAnim* pBillboard = nullptr;	// �r���{�[�h�̃C���X�^���X�𐶐�

	if (pBillboard == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBillboard = new CBillboardAnim(type, priority);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBillboard != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBillboard->Init()))
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

	// �r���{�[�h�̃|�C���^��Ԃ�
	return pBillboard;
}