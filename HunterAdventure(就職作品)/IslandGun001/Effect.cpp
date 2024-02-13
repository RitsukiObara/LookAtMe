//=======================================
//
//�G�t�F�N�g�̃��C������[Effect.cpp]
//Author ��������
//
//=======================================
#include "Effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// �������O���
//=======================================
namespace
{
	const char* TEXTURE[CEffect::TYPE_MAX] =
	{
		"data\\TEXTURE\\Platform\\effect000.jpg",		// �ʏ�
		"data\\TEXTURE\\Platform\\Smoke.tga",			// ���n
		"data\\TEXTURE\\Platform\\Dust.tga",			// ��
		"data\\TEXTURE\\Platform\\Rupture.tga",			// �j��
		"data\\TEXTURE\\Platform\\Smoke.tga",			// ��
		"data\\TEXTURE\\Platform\\PopSmoke.tga",		// ��(�A�j����)
	};
	const float DUST_GRAVITY = 0.4f;		// ���̏d��
}

//=======================================
// �}�N����`
//=======================================


//=========================
// �R���X�g���N�^
//=========================
CEffect::CEffect() : CBillboard(CObject::TYPE_EFFECT, CObject::PRIORITY_EFFECT)
{
	// �S�Ă̒l���N���A����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_nLife = 0;									// ����
	m_nLifeOrigin = 0;								// ���X�̎���
	m_fSub = 0.0f;									// �����ɂȂ�Ԋu
	m_fContra = 0.0f;								// ���a�̏k�ފԊu
	m_type = TYPE_NONE;								// ���
	m_bAdd = false;									// ���Z������
	m_bZTest = false;								// Z�e�X�g�̗L��
}

//=========================
// �f�X�g���N�^
//=========================
CEffect::~CEffect()
{

}

//=========================
// ����������
//=========================
HRESULT CEffect::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_nLife = 0;									// ����
	m_nLifeOrigin = 0;								// ���X�̎���
	m_fSub = 0.0f;									// �����ɂȂ�Ԋu
	m_fContra = 0.0f;								// ���a�̏k�ފԊu
	m_type = TYPE_NONE;								// ���
	m_bAdd = false;									// ���Z������
	m_bZTest = false;								// Z�e�X�g�̗L��

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CEffect::Uninit(void)
{
	// �I��
	CBillboard::Uninit();
}

//=========================
// �X�V����
//=========================
void CEffect::Update(void)
{
	switch (m_type)
	{
	case TYPE_NONE:			// ����

		// �ړ�����
		Move();

		// �����x�̌��Z����
		SubAlpha();

		// �T�C�W���O����
		Sizing();

		break;

	case TYPE_FIRE:			// ��

		// ���̍X�V����
		FireProcess();

		// �ړ�����
		Move();

		// �����x�̌��Z����
		SubAlpha();

		// �T�C�W���O����
		Sizing();

		break;

	case TYPE_DUST:			// ��

		// �d�͂𑫂�
		m_move.y -= DUST_GRAVITY;

		// �ړ�����
		Move();

		// �����x�̌��Z����
		SubAlpha();

		// �T�C�W���O����
		Sizing();

		break;

	case TYPE_RUPTURE:		// �j��

		// �j��̍X�V����
		RuptureProcess();

		// �ړ�����
		Move();

		// �����x�̌��Z����
		SubAlpha();

		// �T�C�W���O����
		Sizing();

		break;

	case TYPE_SMOKE:		// ��

		// �ړ�����
		Move();

		// �����x�̌��Z����
		SubAlpha();

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ���������炵�Ă���
	m_nLife--;

	if (m_nLife <= 0)
	{ // �G�t�F�N�g�̎������s������

		// �j������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// ���_���W�̐ݒ菈��
	SetVertex();

	// �F�̐ݒ菈��
	SetVtxColor(m_col);
}

//=========================
// �`�揈��
//=========================
void CEffect::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	if (m_bAdd == true)
	{ // ���Z��������ꍇ

		//���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// �`�揈��
		CBillboard::DrawLightOff(m_bZTest);

		//���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else
	{ // ��L�ȊO

		// �`�揈��
		CBillboard::DrawLightOff(m_bZTest);
	}
}

//=========================
// ���̐ݒ菈��
//=========================
void CEffect::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type, const D3DXCOLOR& col, const bool bAdd, const bool bZTest)
{
	// ���̐ݒ菈��
	SetPos(pos);			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetSize(D3DXVECTOR3(fRadius, fRadius, 0.0f));	// �T�C�Y
	SetEnableLookY(true);	// Y����

	// �S�Ă̒l������������
	m_type = type;			// ���
	m_nLife = nLife;		// ����
	m_nLifeOrigin = nLife;	// ���X�̎���
	m_move = move;			// �ړ���
	m_col = col;			// �F
	m_bAdd = bAdd;			// ���Z������
	m_bZTest = bZTest;		// Z�e�X�g�̗L��

	m_fSub = m_col.a * (1.0f / m_nLife);			// �����ɂȂ�Ԋu
	m_fContra = fRadius * (1.0f / m_nLife);			// ���a�̏k�ފԊu

	// ���_���̏�����
	SetVertex();

	// �F�̐ݒ菈��
	SetVtxColor(m_col);

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[m_type]));
}

//=========================
// ��������
//=========================
CEffect* CEffect::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type, const D3DXCOLOR& col, const bool bAdd, const bool bZTest)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CEffect* pEffect = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pEffect == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pEffect = new CEffect;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEffect != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEffect->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEffect->SetData(pos, move, nLife, fRadius, type, col, bAdd, bZTest);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pEffect;
}

//=========================
// �ړ�����
//=========================
void CEffect::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// �ړ�����
	pos += m_move;

	// �ʒu���X�V����
	SetPos(pos);
}

//=========================
// �����x���Z����
//=========================
void CEffect::SubAlpha(void)
{
	// �����x�����Z������
	m_col.a -= m_fSub;

	if (m_col.a <= 0.0f)
	{ // �����x��0.0f�ȉ��̏ꍇ

		// �����x��␳����
		m_col.a = 0.0f;
	}
}

//=========================
// �T�C�W���O����
//=========================
void CEffect::Sizing(void)
{
	// ���[�J���ϐ���錾
	D3DXVECTOR3 size = GetSize();	// �T�C�Y���擾����

	// �T�C�Y���k�߂�
	size.x -= m_fContra;
	size.y -= m_fContra;

	if (size.x <= 0.0f)
	{ // �T�C�Y��0.0f�ȉ��̏ꍇ

		// �T�C�Y��␳����
		size.x = 0.0f;
	}

	if (size.y <= 0.0f)
	{ // �T�C�Y��0.0f�ȉ��̏ꍇ

		// �T�C�Y��␳����
		size.y = 0.0f;
	}

	// �T�C�Y���X�V����
	SetSize(size);
}

//=========================
// �����̏���
//=========================
void CEffect::FireProcess(void)
{
	// G�l�ɉ��Z����
	m_col.g += m_fSub;

	if (m_col.g >= 1.0f)
	{ // �F��1.0f�ȏ�ɂȂ����ꍇ

		// G�l���Œ肷��
		m_col.g = 1.0f;
	}
}

//=========================
// �j��̏���
//=========================
void CEffect::RuptureProcess(void)
{
	if (m_nLife >= m_nLifeOrigin / 2)
	{ // ���������ȊO�̏ꍇ

		float f;

		f = m_move.x / ((m_nLifeOrigin / 2) - (m_nLifeOrigin - m_nLife));

		m_move.x -= f;

		f = m_move.y / ((m_nLifeOrigin / 2) - (m_nLifeOrigin - m_nLife));

		m_move.y -= f;

		f = m_move.z / ((m_nLifeOrigin / 2) - (m_nLifeOrigin - m_nLife));

		m_move.z -= f;
	}
}