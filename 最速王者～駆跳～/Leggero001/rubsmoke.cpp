//============================================
//
// �C�艌�̃��C������[rubsmoke.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "rubsmoke.h"
#include "player.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define RUBSMOKE_TEXTURE		"data/TEXTURE/explosion000.png"		// �����̃e�N�X�`��
#define RUBSMOKE_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))	// �C�艌�̃T�C�Y

//===========================================
// �R���X�g���N�^
//===========================================
CRubSmoke::CRubSmoke() : CBillboardAnim(CObject::TYPE_RUBSMOKE,CObject::PRIORITY_PLAYER)
{
	// �S�Ă̒l���N���A����
	m_nLife = 0;				// ����
}

//===========================================
// �f�X�g���N�^
//===========================================
CRubSmoke::~CRubSmoke()
{

}

//===========================================
// ����������
//===========================================
HRESULT CRubSmoke::Init(void)
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
void CRubSmoke::Uninit(void)
{
	// �I��
	CBillboardAnim::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CRubSmoke::Update(void)
{
	if (CScene::GetPlayer() != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 pos = GetPos();

		// �ʒu�����炷
		pos.y += CScene::GetPlayer()->GetMove().y;

		// �ʒu��ݒ肷��
		SetPos(pos);
	}

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
void CRubSmoke::Draw(void)
{
	// �`�揈��
	CBillboardAnim::DrawLightOff();
}

//===========================================
// �ݒ菈��
//===========================================
void CRubSmoke::SetData(const D3DXVECTOR3& pos)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);				// �ʒu�ݒ�
	SetSize(RUBSMOKE_SIZE);		// �T�C�Y�ݒ�

	// �A�j���[�V�����̐ݒ菈��
	SetAnim(3, 8);

	// �S�Ă̒l������������
	m_nLife = 3 * 8;			// ����

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`�����̐ݒ菈��
	SetVtxAnim(GetTexPattern(), GetPattern());

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::GetTexture()->Regist(RUBSMOKE_TEXTURE));
}

//===========================================
// ��������
//===========================================
CRubSmoke* CRubSmoke::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CRubSmoke* pRubSmoke = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pRubSmoke == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pRubSmoke = new CRubSmoke;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRubSmoke != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

	  // ����������
		if (FAILED(pRubSmoke->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�C�艌�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pRubSmoke->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pRubSmoke;
}