//=======================================
//
// ��ʃG�t�F�N�g�̃��C������[screen_effect.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "screen_effect.h"
#include "texture.h"

//---------------------------------------
// �������O���
//---------------------------------------
namespace
{
	const char* TEXTURE[CScreenEffect::TYPE_MAX] =		// �e�N�X�`��
	{
		"data\\TEXTURE\\Platform\\Dmg_Screen.png",		// ���Ӑ�
		"data\\TEXTURE\\Platform\\Blood_Screen.png",	// ��
		"data\\TEXTURE\\Platform\\Concent_Screen.png",	// �W����
		"data\\TEXTURE\\Platform\\Red_Screen.png",		// �S������
		"data\\TEXTURE\\Platform\\Anxiety.png",			// ��
	};
}

//=========================
// �R���X�g���N�^
//=========================
CScreenEffect::CScreenEffect() : CObject2D(CObject::TYPE_SCREENEFFECT, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_type = TYPE_RED_AROUND;		// ���
	m_fAlpha = 1.0f;				// �����x
}

//=========================
// �f�X�g���N�^
//=========================
CScreenEffect::~CScreenEffect()
{

}

//=========================
// ����������
//=========================
HRESULT CScreenEffect::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_type = TYPE_RED_AROUND;		// ���
	m_fAlpha = 1.0f;				// �����x

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CScreenEffect::Uninit(void)
{
	// �I��
	CObject2D::Uninit();
}

//=========================
// �X�V����
//=========================
void CScreenEffect::Update(void)
{
	// ���_���̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// �`�揈��
//=========================
void CScreenEffect::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//=========================
// ���̐ݒ菈��
//=========================
void CScreenEffect::SetData(const TYPE type)
{
	// �X�N���[���̐ݒ菈��
	SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));		// �ʒu�ݒ�
	SetRot(NONE_D3DXVECTOR3);			// �����ݒ�
	SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));		// �T�C�Y�ݒ�
	SetLength();			// �����ݒ�
	SetAngle();				// �����ݒ�

	// �S�Ă̒l������������
	m_type = type;				// ���
	m_fAlpha = 1.0f;			// �����x

	// ���_���̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[m_type]));
}

//=========================
// ��������
//=========================
CScreenEffect* CScreenEffect::Create(const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CScreenEffect* pEffect = nullptr;	// �G�t�F�N�g�̃C���X�^���X�𐶐�

	if (pEffect == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pEffect = new CScreenEffect;
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
		pEffect->SetData(type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �G�t�F�N�g�̃|�C���^��Ԃ�
	return pEffect;
}

//=========================
// �����x�̐ݒ菈��
//=========================
void CScreenEffect::SetAlpha(const float fAlpha)
{
	// �����x��ݒ肷��
	m_fAlpha = fAlpha;
}

//=========================
// �����x�̎擾����
//=========================
float CScreenEffect::GetAlpha(void) const
{
	// �����x��Ԃ�
	return m_fAlpha;
}