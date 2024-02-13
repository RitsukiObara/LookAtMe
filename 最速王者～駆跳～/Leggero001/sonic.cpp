//============================================
//
// �������o�̃��C������[sonic.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "renderer.h"
#include "sonic.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define SONIC_TEXTURE			"data/TEXTURE/SonicEffect.png"		// ���x���o�̃e�N�X�`��

//=========================================
// �R���X�g���N�^
//=========================================
CSonic::CSonic() : CScroll(CObject::TYPE_BG, CObject::PRIORITY_BG)
{

}

//=========================================
// �f�X�g���N�^
//=========================================
CSonic::~CSonic()
{

}

//===========================================
// �������o�̏���������
//===========================================
HRESULT CSonic::Init(void)
{
	if (FAILED(CScroll::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(SONIC_TEXTURE));

	// �X�N���[���̐ݒ菈��
	SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));	// �ʒu�ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �����ݒ�
	SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));		// �T�C�Y�ݒ�
	SetLength();			// �����ݒ�
	SetAngle();				// �����ݒ�
	SetTex(D3DXVECTOR2(0.0f, 0.0f));			// �e�N�X�`�����W�̐ݒ菈��
	SetSpeed(D3DXVECTOR2(0.005f, 0.0f));		// ���x�̐ݒ菈��

	// ���_���̐ݒ菈��
	SetVertex();

	{
		// �e�N�X�`���̐ݒ菈��
		D3DXVECTOR2 tex = GetTex();

		// �e�N�X�`���ݒ�
		SetVtxTextureScroll(tex);
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �������o�̏I������
//===========================================
void CSonic::Uninit(void)
{
	// �I������
	CScroll::Uninit();
}

//===========================================
// �������o�̍X�V����
//===========================================
void CSonic::Update(void)
{
	// �X�V����
	CScroll::Update();
}

//===========================================
// �������o�̕`�揈��
//===========================================
void CSonic::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);		//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// �`�揈��
	CScroll::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// �A���t�@�e�X�g�̎Q�ƒl�ݒ�
}

//===========================================
// ��������
//===========================================
CSonic* CSonic::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CSonic* pSonic = nullptr;			// �������o�̃C���X�^���X�𐶐�

	if (pSonic == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pSonic = new CSonic;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pSonic != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pSonic->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�������o�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �������o�̃|�C���^��Ԃ�
	return pSonic;
}