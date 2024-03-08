//============================================
//
// �A�j�����A�N�V�����̃��C������[explosion.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "anim_reaction.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const char* TEXTURE[CAnimReaction::TYPE_MAX] =		// �e�N�X�`��
	{
		"data\\TEXTURE\\Platform\\Explosion.png",		// ����
		"data\\TEXTURE\\Platform\\GunExplosion.png",	// ���򔚔�
		"data\\TEXTURE\\Platform\\Splash.png",			// ��
		"data\\TEXTURE\\Platform\\WaterExpl.png",		// ������
	};
	const int ANIM_PATTERN[CAnimReaction::TYPE_MAX] =	// �p�^�[��
	{
		8,			// ����
		8,			// ���򔚔�
		6,			// ��
		8,			// ������
	};
}

//===========================================
// �R���X�g���N�^
//===========================================
CAnimReaction::CAnimReaction() : CBillboardAnim(CObject::TYPE_ANIMREACTION, CObject::PRIORITY_EFFECT)
{
	// �S�Ă̒l���N���A����
	m_type = TYPE_EXPLOSION;	// ���
	m_nLife = 0;				// ����
}

//===========================================
// �f�X�g���N�^
//===========================================
CAnimReaction::~CAnimReaction()
{

}

//===========================================
// ����������
//===========================================
HRESULT CAnimReaction::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_type = TYPE_EXPLOSION;	// ���
	m_nLife = 0;				// ����

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CAnimReaction::Uninit(void)
{
	// �I��
	CBillboardAnim::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CAnimReaction::Update(void)
{
	// ���������Z����
	m_nLife--;

	if (m_nLife <= 0)
	{ // ������0�ɂȂ����ꍇ

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// �X�V����
	CBillboardAnim::Update();
}

//===========================================
// �`�揈��
//===========================================
void CAnimReaction::Draw(void)
{
	// �`�揈��
	CBillboardAnim::DrawLightOff(false);
}

//===========================================
// �ݒ菈��
//===========================================
void CAnimReaction::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nCount, const int nLife)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);				// �ʒu�ݒ�
	SetSize(size);				// �T�C�Y�ݒ�
	SetEnableLookY(true);		// Y����
	SetAnim(nCount, ANIM_PATTERN[type]);		// �A�j���[�V�������

	// �S�Ă̒l��ݒ肷��
	m_type = type;				// ���
	m_nLife = nCount * ANIM_PATTERN[m_type] * nLife;		// ����

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`�����̐ݒ菈��
	SetVtxAnim(GetTexPattern(), GetPattern());

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(col);

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[m_type]));
}

//===========================================
// ��������
//===========================================
CAnimReaction* CAnimReaction::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nCount, const int nLife)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAnimReaction* pReaction = nullptr;		// ���A�N�V�����̃C���X�^���X�𐶐�

	if (pReaction == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pReaction = new CAnimReaction;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pReaction != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

	  // ����������
		if (FAILED(pReaction->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pReaction->SetData(pos, size, col, type, nCount, nLife);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���A�N�V�����̃|�C���^��Ԃ�
	return pReaction;
}