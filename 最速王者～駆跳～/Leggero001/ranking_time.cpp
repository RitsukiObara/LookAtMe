//============================================
//
// �����L���O�^�C�}�[����[ranking_time.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "ranking_time.h"
#include "manager.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define RANKINGTIME_WIDTH_SHIFT		(100.0f)								// �����L���O�^�C�}�[�̉��̂��炷��
#define RANKINGTIME_SIZE			(D3DXVECTOR3(45.0f, 60.0f, 0.0f))		// �����L���O�^�C�}�[�̃T�C�Y
#define RANKINGTIME_POS				(D3DXVECTOR3(700.0f, 80.0f, 0.0f))		// �����L���O�^�C�}�[�̈ʒu
#define RANKINGTIME_DOT_SIZE		(D3DXVECTOR3(10.0f, 10.0f, 0.0f))		// �����L���O�^�C�}�[�̓_�̃T�C�Y

//========================
// �R���X�g���N�^
//========================
CRankingTime::CRankingTime() : CTime(CObject::TYPE_TIME, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
}

//========================
// �f�X�g���N�^
//========================
CRankingTime::~CRankingTime()
{

}

//========================
// ����������
//========================
HRESULT CRankingTime::Init(void)
{
	if (FAILED(CTime::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	SetTotalTime(CManager::GetClearTime());		// �N���A�^�C����������
	m_pos = RANKINGTIME_POS;					// �ʒu��ݒ肷��

	// �v�Z����
	Calculate();

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CRankingTime::Uninit(void)
{
	// �I������
	CTime::Uninit();

	// �N���A�^�C��������������
	CManager::SetClearTime(0);
}

//========================
// �X�V����
//========================
void CRankingTime::Update(void)
{
	// �v�Z����
	Calculate();
}

//========================
// �`�揈��
//========================
void CRankingTime::Draw(void)
{
	// �`�揈��
	CTime::Draw();
}

//========================
// �v�Z����
//========================
void CRankingTime::Calculate(void)
{
	// ���l�̐ݒ菈��
	SetNumber();

	// 1�����Ƃ̃e�N�X�`���̐ݒ菈��
	SetTexture();
}

//========================
// ��������
//========================
CRankingTime* CRankingTime::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CRankingTime* pTime = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pTime == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

	  // �I�u�W�F�N�g�𐶐�
		pTime = new CRankingTime;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

	  // NULL ��Ԃ�
		return nullptr;
	}

	if (pTime != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

	  // ����������
		if (FAILED(pTime->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���Ԃ̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pTime->SetData(RANKINGTIME_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), RANKINGTIME_SIZE, RANKINGTIME_DOT_SIZE, RANKINGTIME_WIDTH_SHIFT);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

	  // NULL ��Ԃ�
		return nullptr;
	}

	// ���Ԃ̃|�C���^��Ԃ�
	return pTime;
}