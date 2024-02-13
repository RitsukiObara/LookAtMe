//============================================
//
// ���d�w�i�̃��C������[bgmulti.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "bgmulti.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char *CBgMulti::m_apTextureName[TYPE_MAX] =			// �e�N�X�`���̖��O
{
	"data/TEXTURE/bg100.png",
	"data/TEXTURE/bg101.png",
	"data/TEXTURE/bg102.png",
};

const D3DXVECTOR2 CBgMulti::m_aSpeedInfo[TYPE_MAX] =		// �X�s�[�h�̏��
{
	D3DXVECTOR2(0.0f, 0.001f),
	D3DXVECTOR2(0.0f, 0.0005f),
	D3DXVECTOR2(0.0f, 0.005f)
};

//=========================================
// �R���X�g���N�^
//=========================================
CBgMulti::CBgMulti() : CObject(CObject::TYPE_BG, CObject::PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �w�i�̃|�C���^
		m_apBgMulti[nCnt] = nullptr;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CBgMulti::~CBgMulti()
{

}

//===========================================
// �I�u�W�F�N�g2D�̏���������
//===========================================
HRESULT CBgMulti::Init(void)
{
	// ���[�J���ϐ��錾
	HRESULT hr = S_OK;			// ���ʂ̕ϐ�

	for (int nCntBg = 0; nCntBg < TYPE_MAX; nCntBg++)
	{
		if (m_apBgMulti[nCntBg] == nullptr)
		{ // �|�C���^�� NULL �̏ꍇ

			// ���������m�ۂ���
			m_apBgMulti[nCntBg] = new CScroll;
		}
		else
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �x����
			MessageBox(NULL, "���d�w�i�̃������̊m�ۂɎ��s�I", "�x���I", MB_ICONWARNING);

			// ���s��Ԃ�
			return E_FAIL;
		}

		if (m_apBgMulti[nCntBg] != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �w�i�̏���
			hr = m_apBgMulti[nCntBg]->Init();
		}
		else
		{ // �|�C���^���@NULL �̏ꍇ

			// �x����
			MessageBox(NULL, "���d�w�i�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// ���s��Ԃ�
			return E_FAIL;
		}

		// �e�N�X�`���̊��蓖�ď���
		m_apBgMulti[nCntBg]->BindTexture(CManager::GetTexture()->Regist(m_apTextureName[nCntBg]));

		// �X�N���[���̐ݒ菈��
		m_apBgMulti[nCntBg]->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));	// �ʒu�ݒ�
		m_apBgMulti[nCntBg]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �����ݒ�
		m_apBgMulti[nCntBg]->SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));		// �T�C�Y�ݒ�
		m_apBgMulti[nCntBg]->SetLength();			// �����ݒ�
		m_apBgMulti[nCntBg]->SetAngle();			// �����ݒ�

		// ���_���̐ݒ菈��
		m_apBgMulti[nCntBg]->SetVertex();

		{ // �e�N�X�`���ݒ�

			D3DXVECTOR2 tex = m_apBgMulti[nCntBg]->GetTex();

			// �e�N�X�`���ݒ�
			m_apBgMulti[nCntBg]->SetVtxTextureScroll(tex);
		}

		// �X�s�[�h��ݒ肷��
		m_apBgMulti[nCntBg]->SetSpeed(m_aSpeedInfo[nCntBg]);		// ���x�̐ݒ菈��
	}

	// ������Ԃ�
	return hr;
}

//===========================================
// �I�u�W�F�N�g2D�̏I������
//===========================================
void CBgMulti::Uninit(void)
{
	for (int nCntBg = 0; nCntBg < TYPE_MAX; nCntBg++)
	{
		// �w�i�̏I������
		m_apBgMulti[nCntBg]->Uninit();
	}

	// ���d�w�i�̉�̔j������
	CObject::Release();
}

//===========================================
// �I�u�W�F�N�g2D�̍X�V����
//===========================================
void CBgMulti::Update(void)
{
	for (int nCntBg = 0; nCntBg < TYPE_MAX; nCntBg++)
	{
		// �w�i�̍X�V����
		m_apBgMulti[nCntBg]->Update();
	}
}

//===========================================
// �I�u�W�F�N�g2D�̕`�揈��
//===========================================
void CBgMulti::Draw(void)
{
	for (int nCntBg = 0; nCntBg < TYPE_MAX; nCntBg++)
	{
		// �w�i�̕`�揈��
		m_apBgMulti[nCntBg]->Draw();
	}
}

//===========================================
// ���d�w�i�̎擾����
//===========================================
CScroll* CBgMulti::GetBgMulti(int nIdx) const
{
	// �w�i�̏���Ԃ�
	return m_apBgMulti[nIdx];
}

//===========================================
// �ʒu�̐ݒ菈��
//===========================================
void CBgMulti::SetPos(const D3DXVECTOR3& pos)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// ����ݒ肷��
		m_apBgMulti[nCnt]->SetPos(pos);
	}
}

//===========================================
// �ʒu�̎擾����
//===========================================
D3DXVECTOR3 CBgMulti::GetPos(void) const
{
	// �x����
	MessageBox(NULL, "���d�w�i�̈ʒu�擾�͖����ł��I", "�x���I", MB_ICONWARNING);

	// �ʒu��Ԃ�
	return m_apBgMulti[0]->GetPos();
}

//===========================================
// ��������
//===========================================
CBgMulti* CBgMulti::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBgMulti* pBgMulti = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pBgMulti == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBgMulti = new CBgMulti;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBgMulti != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBgMulti->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���d�w�i�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pBgMulti;
}