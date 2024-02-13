//============================================
//
// �c�eUI����[bulletUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "bulletUI.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const D3DXVECTOR3 INIT_BULLET_POS = D3DXVECTOR3(80.0f, 17.0f, 0.0f);		// �ŏ��̒e�ۂ̈ʒu
	const D3DXVECTOR3 BULLET_SIZE = D3DXVECTOR3(7.0f, 14.0f, 0.0f);				// �e�ۂ̃T�C�Y
	const float BULLET_SHIFT_X = 15.0f;											// �e�ۂ̂��炷��(X���W)
	const char* BULLET_TEXTURE = "data\\TEXTURE\\RestBullet.png";				// �e�ۂ̃e�N�X�`��
}

//========================
// �R���X�g���N�^
//========================
CBulletUI::CBulletUI() : CObject(TYPE_BULLETUI, DIM_2D, PRIORITY_UI)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < MAX_REMAINING_BULLET; nCnt++)
	{
		m_aBullet[nCnt].pBullet = nullptr;		// �c�e�̏��
		m_aBullet[nCnt].bDisp = true;			// �`���
	}

	m_pReload = nullptr;						// �����[�h�̕���
	m_nNumBullet = MAX_REMAINING_BULLET;		// �c�e��
}

//========================
// �f�X�g���N�^
//========================
CBulletUI::~CBulletUI()
{

}

//========================
// ����������
//========================
HRESULT CBulletUI::Init(void)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < MAX_REMAINING_BULLET; nCnt++)
	{
		if (m_aBullet[nCnt].pBullet == nullptr)
		{ // �c�e�� NULL �̏ꍇ

			// �e�ۂ̏��𐶐�����
			m_aBullet[nCnt].pBullet = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		}

		m_aBullet[nCnt].bDisp = true;			// �`���
	}

	// �����[�h�̕����𐶐�����
	m_pReload = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CBulletUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_REMAINING_BULLET; nCnt++)
	{
		if (m_aBullet[nCnt].pBullet != nullptr)
		{ // �c�e�̏�� NULL ����Ȃ��ꍇ

			// �c�e�̏I������
			m_aBullet[nCnt].pBullet->Uninit();
			m_aBullet[nCnt].pBullet = nullptr;
		}
	}

	if (m_pReload != nullptr)
	{ // �����[�h�̕����� NULL ����Ȃ��ꍇ

		// �����[�h�̕����̏I������
		m_pReload->Uninit();
		m_pReload = nullptr;
	}

	// �����̏I������
	Release();
}

//========================
// �X�V����
//========================
void CBulletUI::Update(void)
{

}

//========================
// �`�揈��
//========================
void CBulletUI::Draw(void)
{
	for (int nCnt = 0; nCnt < m_nNumBullet; nCnt++)
	{
		if (m_aBullet[nCnt].pBullet != nullptr &&
			m_aBullet[nCnt].bDisp == true)
		{ // �c�e���`��ł����Ԃ̏ꍇ

			// �`�揈��
			m_aBullet[nCnt].pBullet->Draw();
		}
	}

	if (m_pReload != nullptr)
	{ // �����[�h�̕����� NULL ����Ȃ��ꍇ

		// �`�揈��
		//m_pReload->Draw();
	}
}

//========================
// ���̐ݒ菈��
//========================
void CBulletUI::SetData(void)
{
	D3DXVECTOR3 posBullet;			// �e�ۂ̈ʒu

	for (int nCnt = 0; nCnt < MAX_REMAINING_BULLET; nCnt++)
	{
		if (m_aBullet[nCnt].pBullet != nullptr)
		{
			// �e�ۂ̈ʒu��ݒ肷��
			posBullet = D3DXVECTOR3(INIT_BULLET_POS.x + (BULLET_SHIFT_X * nCnt), INIT_BULLET_POS.y, 0.0f);

			// �ݒ菈��
			m_aBullet[nCnt].pBullet->SetPos(posBullet);			// �ʒu
			m_aBullet[nCnt].pBullet->SetPosOld(posBullet);		// �O��̈ʒu
			m_aBullet[nCnt].pBullet->SetRot(NONE_D3DXVECTOR3);	// ����
			m_aBullet[nCnt].pBullet->SetSize(BULLET_SIZE);		// �T�C�Y
			m_aBullet[nCnt].pBullet->SetAngle();				// ����
			m_aBullet[nCnt].pBullet->SetLength();				// ����

			// �e�N�X�`���̊��蓖�ď���
			m_aBullet[nCnt].pBullet->BindTexture(CManager::Get()->GetTexture()->Regist(BULLET_TEXTURE));

			// ���_���W�̐ݒ菈��
			m_aBullet[nCnt].pBullet->SetVertex();
		}
	}
}

//========================
// ��������
//========================
CBulletUI* CBulletUI::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBulletUI* pUI = nullptr;	// UI�̃C���X�^���X�𐶐�

	if (pUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pUI = new CBulletUI;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pUI != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pUI->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// UI�̃|�C���^��Ԃ�
	return pUI;
}

//========================
// �c�e���̐ݒ菈��
//========================
void CBulletUI::SetNumBullet(const int nNum)
{
	// �c�e����ݒ肷��
	m_nNumBullet = nNum;

	if (m_nNumBullet < 0)
	{ // �c�e����0�����ɂȂ����ꍇ

		// �c�e����0�ɂ���
		m_nNumBullet = 0;
	}
}

//========================
// �c�e���̎擾����
//========================
int CBulletUI::GetNumBullet(void) const
{
	// �c�e����Ԃ�
	return m_nNumBullet;
}