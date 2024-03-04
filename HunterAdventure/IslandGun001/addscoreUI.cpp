//=======================================
//
// �ǉ��X�R�AUI�̃��C������[addscoreUI.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "addscoreUI.h"
#include "renderer.h"
#include "texture.h"

//---------------------------------------
// �萔��`
//---------------------------------------
namespace
{
	const char* TEXTURE[CAddScoreUI::TYPE_MAX] =
	{
		"data\\TEXTURE\\ScoreCoin.png",		// �R�C��
		"data\\TEXTURE\\ScoreEnm001.png",	// �^�[�h��
		"data\\TEXTURE\\ScoreEnm002.png",	// �C���J��
	};
	const D3DXVECTOR3 UI_SIZE = D3DXVECTOR3(90.0f, 30.0f, 0.0f);		// �T�C�Y
	const float ADD_HEIGHT = 2.0f;			// �����̒ǉ���
	const int LIFE = 60;					// ����
	const float SUB_ALPHA = 0.1f;			// �����x�̌��Z��
	const float SUB_ALPHA_LIFE = 10;		// �����x�����炷����
}

//=========================
// �R���X�g���N�^
//=========================
CAddScoreUI::CAddScoreUI() : CBillboard(TYPE_ADDSCOREUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_type = TYPE_COIN;			// ���
	m_nLife = LIFE;				// ����
	m_fAlpha = 1.0f;			// �����x
}

//=========================
// �f�X�g���N�^
//=========================
CAddScoreUI::~CAddScoreUI()
{

}

//=========================
// ����������
//=========================
HRESULT CAddScoreUI::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CAddScoreUI::Uninit(void)
{
	// �I��
	CBillboard::Uninit();
}

//=========================
// �X�V����
//=========================
void CAddScoreUI::Update(void)
{
	// ���������Z����
	m_nLife--;

	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	// ��Ɉړ�����
	pos.y += ADD_HEIGHT;

	// �ʒu��K�p
	SetPos(pos);

	if (m_nLife <= SUB_ALPHA_LIFE)
	{ // �����x�����炷�������ɂȂ����ꍇ

		// �����x�����Z����
		m_fAlpha -= SUB_ALPHA;
	}

	if (m_nLife <= 0)
	{ // ������0�ɂȂ����ꍇ

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// ���_���̏�����
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// �`�揈��
//=========================
void CAddScoreUI::Draw(void)
{
	// �`�揈��
	CBillboard::DrawLightOff(false);
}

//=========================
// ���̐ݒ菈��
//=========================
void CAddScoreUI::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);			// �ʒu�ݒ�
	SetRot(0.0f);			// �����ݒ�
	SetSize(UI_SIZE);		// �T�C�Y�ݒ�
	SetLength();			// �����ݒ�
	SetAngle();				// �����ݒ�
	SetEnableLookY(true);	// Y���ݒ�

	// �S�Ă̒l������������
	m_type = type;			// ���
	m_nLife = LIFE;			// ����
	m_fAlpha = 1.0f;			// �����x

	// ���_���̏�����
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[m_type]));
}

//=========================
// ��������
//=========================
CAddScoreUI* CAddScoreUI::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAddScoreUI* pLocus3D = nullptr;	// �c���̃C���X�^���X�𐶐�

	if (pLocus3D == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pLocus3D = new CAddScoreUI;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pLocus3D != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pLocus3D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pLocus3D->SetData(pos, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �c���̃|�C���^��Ԃ�
	return pLocus3D;
}