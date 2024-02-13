//============================================
//
// �����̃��C������[explosion.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define EXPLOSION_TEXTURE		"data/TEXTURE/explosion000.png"	// �����̃e�N�X�`��

//===========================================
// �R���X�g���N�^
//===========================================
CExplosion::CExplosion() : CBillboardAnim(CObject::TYPE_EXPLOSION,CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_nLife = 0;				// ����
}

//===========================================
// �f�X�g���N�^
//===========================================
CExplosion::~CExplosion()
{

}

//===========================================
// ����������
//===========================================
HRESULT CExplosion::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_nLife = 0;				// ����

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CExplosion::Uninit(void)
{
	// �I��
	CBillboardAnim::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CExplosion::Update(void)
{
	// ���������炷
	m_nLife--;

	// �X�V����
	CBillboardAnim::Update();

	if (m_nLife <= 0)
	{ // �p�^�[�������ő吔�𒴂����ꍇ

		// �I������
		Uninit();
	}
}

//===========================================
// �`�揈��
//===========================================
void CExplosion::Draw(void)
{
	// �`�揈��
	CBillboardAnim::DrawLightOff();
}

//===========================================
// �ݒ菈��
//===========================================
void CExplosion::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);				// �ʒu�ݒ�
	SetSize(size);				// �T�C�Y�ݒ�

	// �A�j���[�V�����̐ݒ菈��
	SetAnim(3, 8);

	// �S�Ă̒l������������
	m_nLife = 3 * 8;			// ����

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`�����̐ݒ菈��
	SetVtxAnim(GetTexPattern(), GetPattern());

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::GetTexture()->Regist(EXPLOSION_TEXTURE));
}

//===========================================
// ��������
//===========================================
CExplosion* CExplosion::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CExplosion* pExplosion = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pExplosion == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pExplosion = new CExplosion;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pExplosion != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

	  // ����������
		if (FAILED(pExplosion->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�����̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pExplosion->SetData(pos, size);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pExplosion;
}