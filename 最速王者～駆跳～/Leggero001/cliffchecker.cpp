//============================================
//
// �R�߂܂�̃��C������[cliffchecker.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "cliffchecker.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define CLIFFCHECKER_TEXTURE		"data/TEXTURE/CliffChecker.png"		// �R�߂܂�̃e�N�X�`��
#define CLIFFCHECKER_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))	// �R�߂܂�`�F�b�J�[�̃T�C�Y

//===========================================
// �R���X�g���N�^
//===========================================
CCliffChecker::CCliffChecker() : CObject3D(CObject::TYPE_CLIFFCHECKER, CObject::PRIORITY_SHADOW)
{

}

//===========================================
// �f�X�g���N�^
//===========================================
CCliffChecker::~CCliffChecker()
{

}

//===========================================
// ����������
//===========================================
HRESULT CCliffChecker::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CCliffChecker::Uninit(void)
{
	// �I��
	CObject3D::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CCliffChecker::Update(void)
{
	// �X�V
	CObject3D::Update();
}

//===========================================
// �`�揈��
//===========================================
void CCliffChecker::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);		//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// �`�揈��
	CObject3D::DrawLightOff();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// �A���t�@�e�X�g�̎Q�ƒl�ݒ�
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CCliffChecker::SetData(const D3DXVECTOR3& pos)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);								// �ʒu�ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �����ݒ�
	SetSize(CLIFFCHECKER_SIZE);					// �T�C�Y�ݒ�

	// ���_���̐ݒ菈��
	SetVertex();

	// �S�Ă̒l������������
	BindTexture(CManager::GetTexture()->Regist(CLIFFCHECKER_TEXTURE));			// �e�N�X�`���̓ǂݍ���
}

//===========================================
// ��������
//===========================================
CCliffChecker* CCliffChecker::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CCliffChecker* pCliff = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pCliff == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pCliff = new CCliffChecker;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pCliff != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pCliff->Init()))
		{ // �R�߂܂�̏������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�R�߂܂�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pCliff->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �R�߂܂�`�F�b�J�[�̃|�C���^��Ԃ�
	return pCliff;
}