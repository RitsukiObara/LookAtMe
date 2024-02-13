//============================================
//
// �^�C�g���̑I��������[title_select.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "renderer.h"
#include "title_select.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define TITLE_SELECT_TEXTURE		"data/TEXTURE/TitleSelect.png"						// �^�C�g���̑I�����̃e�N�X�`��
#define TITLE_SELECT_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500.0f, 0.0f))	// �^�C�g���̑I�����̈ʒu
#define TITLE_SELECT_SIZE			(D3DXVECTOR3(400.0f, 40.0f, 0.0f))					// �^�C�g���̑I�����̃T�C�Y
#define TITLE_SELECT_ALPHA_ADD		(-0.02f)											// �^�C�g���̑I�����̓����x�̉��Z��

//============================
// �R���X�g���N�^
//============================
CTitleSelect::CTitleSelect() : CObject2D(CObject2D::TYPE_LANDUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_fAlpha = 1.0f;		// �����x
	m_fAlphaAdd = 0.0f;		// �����x�̉��Z��
	m_fAlphaDest = 0.0f;	// �ړI�̓����x
}

//============================
// �f�X�g���N�^
//============================
CTitleSelect::~CTitleSelect()
{

}

//============================
// ����������
//============================
HRESULT CTitleSelect::Init(void)
{
	// �S�Ă̒l������������
	m_fAlpha = 1.0f;						// �����x
	m_fAlphaAdd = TITLE_SELECT_ALPHA_ADD;	// �����x�̉��Z��
	m_fAlphaDest = 0.0f;					// �ړI�̓����x

	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l��ݒ肷��
	SetPos(TITLE_SELECT_POS);				// �ʒu
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ����
	SetSize(TITLE_SELECT_SIZE);				// �T�C�Y�ݒ�
	SetLength();							// �����ݒ�
	SetAngle();								// �����ݒ�

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(TITLE_SELECT_TEXTURE));

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CTitleSelect::Uninit(void)
{
	// �I��
	CObject2D::Uninit();
}

//============================
// �X�V����
//============================
void CTitleSelect::Update(void)
{
	// �����x�ɉ��Z����
	m_fAlpha += m_fAlphaAdd;

	if (m_fAlpha >= 1.0f ||
		m_fAlpha <= 0.0f)
	{ // �����x����萔�ȓ��ɂȂ����ꍇ

		// �����x�̉��Z���̕����𔽓]������
		m_fAlphaAdd *= -1;
	}

	{
		// �F��ݒ肷��
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, m_fAlpha);

		// ���_�J���[��ݒ肷��
		SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
	}
}

//============================
// �`�揈��
//============================
void CTitleSelect::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//============================
//��������
//============================
CTitleSelect* CTitleSelect::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CTitleSelect* pTitleSelect = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pTitleSelect == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pTitleSelect = new CTitleSelect;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pTitleSelect != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pTitleSelect->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�^�C�g���I�����̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �^�C�g���̑I�����̃|�C���^��Ԃ�
	return pTitleSelect;
}