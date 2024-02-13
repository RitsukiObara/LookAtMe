//===========================================
//
// �ۉe�̃��C������[shadowCircle.cpp]
// Author ��������
//
//===========================================
#include "main.h"
#include "shadowCircle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const D3DXCOLOR INIT_SHADOW_COL = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);	// �����̉e�̐F
	const char* SHADOW_TEXTURE = "data\\TEXTURE\\shadow.jpg";				// �e�̃e�N�X�`��
}

//====================
// �R���X�g���N�^
//====================
CShadowCircle::CShadowCircle() : CObject3D(TYPE_SHADOW, PRIORITY_SHADOW)
{
	// �S�Ă̒l���N���A����
	m_posParent = nullptr;			// �e�̈ʒu
	m_col = INIT_SHADOW_COL;		// �F
	m_fRadiusInit = 0.0f;			// �����̔��a
}

//====================
// �f�X�g���N�^
//====================
CShadowCircle::~CShadowCircle()
{

}

//====================
// ����������
//====================
HRESULT CShadowCircle::Init(void)
{
	// ����������
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_posParent = nullptr;			// �e�̈ʒu
	m_col = INIT_SHADOW_COL;		// �F
	m_fRadiusInit = 0.0f;			// �����̔��a

	// ������Ԃ�
	return S_OK;
}

//====================
// �I������
//====================
void CShadowCircle::Uninit(void)
{
	// �I�u�W�F�N�g�̏I������
	CObject3D::Uninit();
}

//====================
// �X�V����
//====================
void CShadowCircle::Update(void)
{
	// �����ɂ��ݒ菈��
	Distance();

	// ���_���̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(m_col);
}

//====================
// �`�揈��
//====================
void CShadowCircle::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	//���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �`�揈��
	CObject3D::Draw();

	//�ʏ�̍����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//====================
// ���̐ݒ菈��
//====================
void CShadowCircle::SetData(const D3DXVECTOR3& pos, const float fRadius, D3DXVECTOR3* posParent)
{
	// ���̏�����
	m_posParent = posParent;		// �e�̈ʒu
	m_col = INIT_SHADOW_COL;		// �F
	m_fRadiusInit = fRadius;		// �����̔��a

	// ���̐ݒ�
	SetPos(pos);				// �ʒu
	SetPosOld(pos);				// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);	// ����
	SetSize(D3DXVECTOR3(fRadius, 0.0f, fRadius));		// �T�C�Y

	// �ݒ菈��
	Distance();

	// ���_���̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(m_col);

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(SHADOW_TEXTURE));
}

//====================
// ��������
//====================
CShadowCircle* CShadowCircle::Create(const D3DXVECTOR3& pos, const float fRadius, D3DXVECTOR3* posParent)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CShadowCircle* pShadow = nullptr;	// �e�̃C���X�^���X�𐶐�

	if (pShadow == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pShadow = new CShadowCircle;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pShadow != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pShadow->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pShadow->SetData(pos, fRadius, posParent);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �e�̃|�C���^��Ԃ�
	return pShadow;
}

//====================
// �����x�̐ݒ菈��
//====================
void CShadowCircle::Distance(void)
{
	if (m_posParent != nullptr)
	{ // �e�̈ʒu�̃|�C���^�� NULL ����Ȃ��ꍇ

		D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
		float fDis = 0.0f;				// ����

		// �����𑪂�
		fDis = m_posParent->y - pos.y;
	}
}