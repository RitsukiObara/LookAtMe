//=======================================
//
//�G�t�F�N�g�̃��C������[Effect.cpp]
//Author ��������
//
//=======================================
#include "Effect.h"
#include "game.h"
#include "manager.h"
#include "player.h"
#include "renderer.h"
#include "texture.h"

//=======================================
// �}�N����`
//=======================================
#define NORMAL_TEXTURE		"data/TEXTURE/effect000.jpg"			// �ʏ�G�t�F�N�g�̃e�N�X�`��
#define STAR_TEXTURE		"data/TEXTURE/SpeedEffect.tga"			// ���G�t�F�N�g�̃e�N�X�`��
#define DUST_TEXTURE		"data/TEXTURE/Dust.tga"					// ���G�t�F�N�g�̃e�N�X�`��
#define BIGJUMP_COL_RAND	(3)										// ���W�����v�G�t�F�N�g���̐F�̃����_����
#define BIGJUMP_COL_FIRST	(D3DXCOLOR(1.0f, 0.0f, 0.8f, 1.0f))		// ���W�����v�G�t�F�N�g�̐F(1��)
#define BIGJUMP_COL_SEC		(D3DXCOLOR(0.0f, 1.0f, 0.1f, 1.0f))		// ���W�����v�G�t�F�N�g�̐F(2��)
#define BIGJUMP_COL_THIRD	(D3DXCOLOR(0.0f, 0.7f, 1.0f, 1.0f))		// ���W�����v�G�t�F�N�g�̐F(3��)

//=========================
// �R���X�g���N�^
//=========================
CEffect::CEffect() : CBillboard(CObject::TYPE_EFFECT, CObject::PRIORITY_EFFECT)
{
	// �S�Ă̒l���N���A����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_nLife = 0;									// ����
	m_fSub = 0.0f;									// �����ɂȂ�Ԋu
	m_fContra = 0.0f;								// ���a�̏k�ފԊu
	m_type = TYPE_NONE;								// ���
	m_bAdd = false;									// ���Z������
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
	m_fSub = 0.0f;									// �����ɂȂ�Ԋu
	m_fContra = 0.0f;								// ���a�̏k�ފԊu
	m_type = TYPE_NONE;								// ���
	m_bAdd = false;									// ���Z������

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
	// ���[�J���ϐ���錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	D3DXVECTOR3 size = GetSize();	// �T�C�Y���擾����

	// ���������炵�Ă���
	m_nLife--;

	// �ړ�����
	pos += m_move;

	// �����x�����Z������
	m_col.a -= m_fSub;

	// �T�C�Y���k�߂�
	size.x -= m_fContra;
	size.y -= m_fContra;

	if (m_col.a <= 0.0f)
	{ // �����x��0.0f�ȉ��̏ꍇ

		// �����x��␳����
		m_col.a = 0.0f;
	}

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

	// �ʒu���X�V����
	SetPos(pos);

	// �T�C�Y���X�V����
	SetSize(size);

	switch (m_type)
	{
	case TYPE_NONE:			// ����

		break;

	case TYPE_FIRE:	// ����

		// ���̍X�V����
		UpdateFire();

		break;

	case TYPE_ITEMGET:		// �A�C�e���l��

		break;

	case TYPE_BIGJUMP:		// ���W�����v

		break;

	case TYPE_SPEEDSTAR:	// ���x�̐�

		break;

	case TYPE_CATCH:		// �L���b�`

		// �d�͂𑫂�
		m_move.y -= 0.4f;

		break;

	case TYPE_RUNDUST:		// ��������̚�

		// �d�͂𑫂�
		m_move.y -= 0.4f;

		break;

	case TYPE_BIGJUMPCHARGE:	// ���W�����v�̃`���[�W

		// ���W�����v�̃`���[�W�̏���
		UpdateBJCharge();

		break;

	case TYPE_LANDDUST:		// ���n��

		break;
	}

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_bAdd == true)
	{ // ���̎��(tga�t�@�C��)�̏ꍇ

		//���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// �`�揈��
		CBillboard::DrawLightOff();

		//���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else
	{ // ��L�ȊO

		// �`�揈��
		CBillboard::DrawLightOff();
	}
}

//=========================
// ���̐ݒ菈��
//=========================
void CEffect::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);			// �ʒu�ݒ�
	SetSize(D3DXVECTOR3(fRadius, fRadius, 0.0f));	// �T�C�Y�ݒ�

	// �S�Ă̒l������������
	m_type = type;			// ���
	m_nLife = nLife;		// ����
	m_move = move;			// �ړ���

	switch (m_type)
	{
	case TYPE_NONE:			// ����

		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F

		// ���Z�����󋵂� true �ɂ���
		m_bAdd = true;

		break;

	case TYPE_FIRE:			// ���n

		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		// �F

		// ���Z�����󋵂� true �ɂ���
		m_bAdd = true;

		break;

	case TYPE_ITEMGET:		// �A�C�e���l��

		m_col = D3DXCOLOR(1.0f, 0.3f, 0.7f, 1.0f);		// �F

		// ���Z�����󋵂� true �ɂ���
		m_bAdd = true;

		break;

	case TYPE_BIGJUMP:		// ���W�����v

		// ���[�J���ϐ��錾
		int nColRand;

		// �F�������_���Ő�������
		nColRand = rand() % BIGJUMP_COL_RAND;

		switch (nColRand)
		{
		case 0:
			
			// �F��ݒ肷��
			m_col = BIGJUMP_COL_FIRST;

			break;

		case 1:

			// �F��ݒ肷��
			m_col = BIGJUMP_COL_SEC;

			break;

		case 2:

			// �F��ݒ肷��
			m_col = BIGJUMP_COL_THIRD;

			break;
		}

		// ���Z�����󋵂� true �ɂ���
		m_bAdd = true;

		break;

	case TYPE_SPEEDSTAR:	// ���x�̐�

		// �F��ݒ肷��
		m_col = D3DXCOLOR(rand() % 8 * 0.1f + 0.3f, rand() % 8 * 0.1f + 0.3f, rand() % 5 * 0.1f + 0.1f, 1.0f);

		// ���Z�����󋵂� true �ɂ���
		m_bAdd = false;

		break;

	case TYPE_CATCH:		// �R�L���b�`

		// �F��ݒ肷��
		m_col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

		// ���Z�����󋵂� false �ɂ���
		m_bAdd = false;

		break;

	case TYPE_RUNDUST:		// ��������̚�

		// �F��ݒ肷��
		m_col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

		// ���Z�����󋵂� false �ɂ���
		m_bAdd = false;

		break;

	case TYPE_BIGJUMPCHARGE:	// ���W�����v�̃`���[�W

		// �F��ݒ肷��
		m_col = D3DXCOLOR(1.0f, 0.2f, 0.9f, 1.0f);

		// ���Z�����󋵂� true �ɂ���
		m_bAdd = true;

		break;

	case TYPE_LANDDUST:		// ���n��

		// �F��ݒ肷��
		m_col = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

		// ���Z�����󋵂� false �ɂ���
		m_bAdd = false;

		break;

	default:				// ��L�ȊO

		// �x����
		MessageBox(NULL, "�G�t�F�N�g�̎�ނŗ\��O�̐��l�����m���ꂽ�I", "�x���I", MB_ICONWARNING);

		break;
	}

	m_fSub = m_col.a * (1.0f / m_nLife);			// �����ɂȂ�Ԋu
	m_fContra = fRadius * (1.0f / m_nLife);			// ���a�̏k�ފԊu

	// ���_���̏�����
	SetVertex();

	// �F�̐ݒ菈��
	SetVtxColor(m_col);

	if (m_type == TYPE_SPEEDSTAR)
	{ // ���x�̐��̏ꍇ

		// �e�N�X�`���̓ǂݍ��ݏ���
		BindTexture(CManager::GetTexture()->Regist(STAR_TEXTURE));
	}
	else if (m_type == TYPE::TYPE_CATCH ||
		m_type == TYPE::TYPE_RUNDUST ||
		m_type == TYPE::TYPE_LANDDUST)
	{ // ���n�̎�ނ̏ꍇ

		// �e�N�X�`���̓ǂݍ��ݏ���
		BindTexture(CManager::GetTexture()->Regist(DUST_TEXTURE));
	}
	else
	{ // ��L�ȊO

		// �e�N�X�`���̓ǂݍ��ݏ���
		BindTexture(CManager::GetTexture()->Regist(NORMAL_TEXTURE));
	}
}

//=========================
// ��������
//=========================
CEffect* CEffect::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type)
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

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEffect != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEffect->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�G�t�F�N�g�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEffect->SetData(pos, move, nLife, fRadius, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pEffect;
}

//=========================
// �����̏���
//=========================
void CEffect::UpdateFire(void)
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
// ���W�����v�̃`���[�W�̏���
//=========================
void CEffect::UpdateBJCharge(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos, PlayerMove;	// �G�t�F�N�g�̈ʒu�A�v���C���[�̈ړ���

	if (CScene::GetPlayer() != nullptr)
	{ // �v���C���[�̏�񂪂������ꍇ

		// �G�t�F�N�g�̈ʒu���擾����
		pos = GetPos();

		// �v���C���[�̈ړ��ʂ��擾����
		PlayerMove = CScene::GetPlayer()->GetMove();

		// �ʒu���ړ�����
		pos += PlayerMove;

		// �ʒu���X�V����
		SetPos(pos);
	}
}