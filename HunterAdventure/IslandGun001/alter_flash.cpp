//=======================================
//
// �Ւd�M���̃��C������[alter_flash.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "alter_flash.h"
#include "renderer.h"
#include "useful.h"

//---------------------------------------
// �萔��`
//---------------------------------------
namespace
{
	const D3DXVECTOR3 FLASH_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		// �ʒu
	const D3DXVECTOR3 FLASH_SIZE = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);	// �ʒu
	const float DEST_ALPHA = 0.0f;			// �ړI�̓����x
	const float ALPHA_CORRECT = 0.1f;		// �����x�̕␳�W��
	const float DEATH_ALPHA = 0.0001f;		// ���ʂƂ��̓����x
}

//=========================
// �R���X�g���N�^
//=========================
CAlterFlash::CAlterFlash() : CObject2D(CObject2D::TYPE_ALTERFLASH, CObject::PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	m_fAlpha = 1.0f;			// �����x
}

//=========================
// �f�X�g���N�^
//=========================
CAlterFlash::~CAlterFlash()
{

}

//=========================
// ����������
//=========================
HRESULT CAlterFlash::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CAlterFlash::Uninit(void)
{
	// �I��
	CObject2D::Uninit();
}

//=========================
// �X�V����
//=========================
void CAlterFlash::Update(void)
{
	// �����ɋ߂Â��Ă���
	useful::Correct(DEST_ALPHA, &m_fAlpha, ALPHA_CORRECT);

	if (m_fAlpha <= DEATH_ALPHA)
	{ // �����x���ق�0�ɂȂ����ꍇ

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// �`�揈��
//=========================
void CAlterFlash::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}
//=========================
// ���̐ݒ菈��
//=========================
void CAlterFlash::SetData(void)
{
	// �X�N���[���̐ݒ菈��
	SetPos(FLASH_POS);			// �ʒu�ݒ�
	SetRot(NONE_D3DXVECTOR3);	// �����ݒ�
	SetSize(FLASH_SIZE);		// �T�C�Y�ݒ�
	SetLength();				// �����ݒ�
	SetAngle();					// �����ݒ�

	// ���_���̏�����
	SetVertex();

	// �S�Ă̒l��ݒ肷��
	m_fAlpha = 1.0f;			// �����x

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// ��������
//=========================
CAlterFlash* CAlterFlash::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAlterFlash* pFlash = nullptr;	// �M���̃C���X�^���X�𐶐�

	if (pFlash == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pFlash = new CAlterFlash;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pFlash != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pFlash->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pFlash->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pFlash;
}