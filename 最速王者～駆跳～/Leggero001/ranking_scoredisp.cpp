//============================================
//
// �����L���O�̃X�R�A�\������[ranking_scoredisp.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "ranking_scoredisp.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define SCOREDISP_POS				(D3DXVECTOR3(500.0f, 80.0f, 0.0f))		// �X�R�A�\���̈ʒu
#define SCOREDISP_SIZE				(D3DXVECTOR3(160.0f, 60.0f, 0.0f))		// �X�R�A�\���̃T�C�Y
#define SCOREDISP_TEXTURE			"data/TEXTURE/YourScore.png"			// �X�R�A�\���̃e�N�X�`��

//========================
// �R���X�g���N�^
//========================
CRankingScoreDisp::CRankingScoreDisp() : CObject2D(CObject::TYPE_RANKINGSCOREDISP, CObject::PRIORITY_UI)
{

}

//========================
// �f�X�g���N�^
//========================
CRankingScoreDisp::~CRankingScoreDisp()
{

}

//========================
// ����������
//========================
HRESULT CRankingScoreDisp::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���̐ݒ�
	SetPos(SCOREDISP_POS);					// �ʒu
	SetPosOld(SCOREDISP_POS);				// �O��̈ʒu
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ����
	SetSize(SCOREDISP_SIZE);				// �T�C�Y
	SetAngle();								// ����
	SetLength();							// ����

	SetVertex();							// ���_���W�̐ݒ菈��

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(SCOREDISP_TEXTURE));

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CRankingScoreDisp::Uninit(void)
{
	// �j������
	CObject2D::Uninit();
}

//========================
// �X�V����
//========================
void CRankingScoreDisp::Update(void)
{

}

//========================
// �`�揈��
//========================
void CRankingScoreDisp::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//========================
// ��������
//========================
CRankingScoreDisp* CRankingScoreDisp::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CRankingScoreDisp* pScore = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pScore == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pScore = new CRankingScoreDisp;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pScore != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pScore->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�X�R�A�\���̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �X�R�A�\���̃|�C���^��Ԃ�
	return pScore;
}