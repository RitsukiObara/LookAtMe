//========================================================================
//
//	�{�X�̎��S�g�䏈�� [boss_ripple.cpp]
//	Author�F��������
//
//========================================================================
//************************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************************
#include "manager.h"
#include "boss_ripple.h"
#include "texture.h"
#include "useful.h"

//------------------------------------------------------------------------
// �萔��`
//------------------------------------------------------------------------
namespace
{
	const D3DXVECTOR3 ROT = D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f);		// ����
	const float WIDTH = 100.0f;					// ���̕�
	const float CIRCUM = 100.0f;				// �~���̕�
	const CMesh::SGrid GRID = { 1,16 };			// ������
	const char* TEXTURE = "data\\TEXTURE\\BossRipple.png";		// �e�N�X�`��
	const float INIT_ADD_CIRCUM = 70.0f;		// �~���̏����ǉ���
	const float INIT_ADD_WIDTH = 80.0f;			// ���̏����ǉ���
	const float CIRCUM_CORRECT = 0.8f;			// �~���̕␳��
	const float WIDTH_CORRECT = 0.8f;			// ���̕␳��
	const float INIT_ALPHA = 1.0f;				// �����x�̏����l
	const float SUB_ALPHA = 0.01f;				// �����x�̌��Z��
}

//================================
// �I�[�o�[���[�h�R���X�g���N�^
//================================
CBossRipple::CBossRipple() : CMeshDonut(PRIORITY_EFFECT)
{
	// �S�Ă̒l���N���A����
	m_fAddCircum = INIT_ADD_CIRCUM;		// �~���̒ǉ���
	m_fAddWidth = INIT_ADD_WIDTH;		// ���̒ǉ���
	m_fAlpha = INIT_ALPHA;				// �����x
	CObject::SetType(TYPE_BOSSRIPPLE);
}

//================================
// �f�X�g���N�^
//================================
CBossRipple::~CBossRipple()
{

}

//================================
// ����������
//================================
HRESULT CBossRipple::Init(void)
{
	if (FAILED(CMeshDonut::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���_���̐ݒ菈��
	SetVertex();

	// �C���f�b�N�X�̐ݒ菈��
	SetIndex(GetVtx().nWidth, GetVtx().nHeight);

	// ������Ԃ�
	return S_OK;
}

//================================
// �I������
//================================
void CBossRipple::Uninit(void)
{
	// �I������
	CMesh::Uninit();
}

//================================
// �X�V����
//================================
void CBossRipple::Update(void)
{
	// �L������
	Spread();

	if (m_fAlpha > 0.0f)
	{ // �����x��0���߂̏ꍇ

		// �����x�����Z����
		m_fAlpha -= SUB_ALPHA;
	}
	else
	{ // ��L�ȊO

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// ���_���W�̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//================================
// �`�揈��
//================================
void CBossRipple::Draw(void)
{
	// �`�揈��
	CMesh::Draw();
}

//================================
// ���̐ݒ菈��
//================================
void CBossRipple::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	CMeshDonut::SetData
	(
		pos,
		ROT,
		WIDTH,
		CIRCUM,
		GRID
	);

	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));	// �e�N�X�`���̊��蓖�ď���
	SetCulling(true);		// �J�����O
	SetLighting(true);		// ���C�e�B���O

	// �S�Ă̒l��ݒ肷��
	m_fAddCircum = INIT_ADD_CIRCUM;		// �~���̒ǉ���
	m_fAddWidth = INIT_ADD_WIDTH;		// ���̒ǉ���
	m_fAlpha = INIT_ALPHA;				// �����x
}

//================================
// ��������
//================================
CBossRipple* CBossRipple::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBossRipple* pRipple = nullptr;		// �g��̃C���X�^���X�𐶐�

	if (pRipple == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pRipple = new CBossRipple;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRipple != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		pRipple->SetData(pos);

		// ����������
		if (FAILED(pRipple->Init()))
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

	// �g��̃|�C���^��Ԃ�
	return pRipple;
}

//================================
// �L������
//================================
void CBossRipple::Spread(void)
{
	// �~���ƕ����擾
	float fCircum = GetCircum();
	float fWidth = GetWidth();

	// �~���L����
	fCircum += m_fAddCircum;
	fWidth += m_fAddWidth;

	// �~���ƕ���K�p
	SetCircum(fCircum);
	SetWidth(fWidth);

	// �ǉ��ʂ�0�ɋ߂Â���
	useful::FrameCorrect(0.0f, &m_fAddCircum, CIRCUM_CORRECT);
	useful::FrameCorrect(0.0f, &m_fAddWidth, WIDTH_CORRECT);
}