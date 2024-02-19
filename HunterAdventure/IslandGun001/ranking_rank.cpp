//============================================
//
// ����UI����[ranking_rank.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "ranking_rank.h"
#include "texture.h"

#include "object2D.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Rank.png";				// �e�N�X�`��
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(200.0f, 140.0f, 0.0f);	// �����ʒu
	const D3DXVECTOR3 UI_SIZE = D3DXVECTOR3(80.0f, 40.0f, 0.0f);	// �T�C�Y
	const float SHIFT = 120.0f;					// ���炷����
	const float TEXTURE_PATTERN = 0.2f;			// �e�N�X�`���̃p�^�[��
}

//========================
// �R���X�g���N�^
//========================
CRankUI::CRankUI() : CObject(TYPE_RANKUI, DIM_2D, PRIORITY_UI)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_apRankUI[nCnt] = nullptr;
	}
}

//========================
// �f�X�g���N�^
//========================
CRankUI::~CRankUI()
{

}

//========================
// ����������
//========================
HRESULT CRankUI::Init(void)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_apRankUI[nCnt] == nullptr)
		{ // ����UI�� NULL �̏ꍇ

			// ����UI�𐶐�
			m_apRankUI[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		}
	}

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CRankUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_apRankUI[nCnt] != nullptr)
		{ // ����UI�� NULL ����Ȃ��ꍇ

			// �I������
			m_apRankUI[nCnt]->Uninit();
			m_apRankUI[nCnt] = nullptr;
		}
	}

	// �����̏I������
	Release();
}

//========================
// �X�V����
//========================
void CRankUI::Update(void)
{

}

//========================
// �`�揈��
//========================
void CRankUI::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_apRankUI[nCnt] != nullptr)
		{ // ����UI�� NULL ����Ȃ��ꍇ
			 
			// �`�揈��
			m_apRankUI[nCnt]->Draw();
		}
	}
}

//========================
// ���̐ݒ菈��
//========================
void CRankUI::SetData(void)
{
	// UI�̈ʒu
	D3DXVECTOR3 posUI = NONE_D3DXVECTOR3;

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_apRankUI[nCnt] != nullptr)
		{ // ����UI�� NULL ����Ȃ��ꍇ

			// �ʒu��ݒ肷��
			posUI.x = INIT_POS.x;
			posUI.y = INIT_POS.y + (SHIFT * nCnt);
			posUI.z = INIT_POS.z;

			// �ݒ菈��
			m_apRankUI[nCnt]->SetPos(posUI);				// �ʒu
			m_apRankUI[nCnt]->SetPosOld(posUI);				// �O��̈ʒu
			m_apRankUI[nCnt]->SetRot(NONE_D3DXVECTOR3);		// ����
			m_apRankUI[nCnt]->SetSize(UI_SIZE);				// �T�C�Y
			m_apRankUI[nCnt]->SetAngle();					// ����
			m_apRankUI[nCnt]->SetLength();					// ����

			// �e�N�X�`���̊��蓖�ď���
			m_apRankUI[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

			// ���_���W�̐ݒ菈��
			m_apRankUI[nCnt]->SetVertex();

			// �e�N�X�`�����W�̐ݒ菈��
			m_apRankUI[nCnt]->SetVtxTextureAnim(TEXTURE_PATTERN, nCnt);
		}
	}
}

//========================
// ��������
//========================
CRankUI* CRankUI::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CRankUI* pRank = nullptr;	// UI�̃C���X�^���X�𐶐�

	if (pRank == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pRank = new CRankUI;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRank != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pRank->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pRank->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// UI�̃|�C���^��Ԃ�
	return pRank;
}