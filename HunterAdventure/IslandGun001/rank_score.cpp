//============================================
//
// �����N�X�R�A�̃X�R�A����[rank_score.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "rank_score.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// �萔��`
//--------------------------------------------
namespace
{
	// �����֌W
	const char* NUMBER_TEXTURE = "data\\TEXTURE\\Number.png";			// �����̃e�N�X�`��
	const D3DXVECTOR3 SCORE_SIZE = D3DXVECTOR3(30.0f, 40.0f, 0.0f);		// �T�C�Y
	const float NUMBER_SHIFT = 60.0f;		// �����̂��炷��
}

//========================
// �R���X�g���N�^
//========================
CRankScore::CRankScore() : CScore()
{

}

//========================
// �f�X�g���N�^
//========================
CRankScore::~CRankScore()
{

}

//========================
// ����������
//========================
HRESULT CRankScore::Init(void)
{
	if (FAILED(CScore::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CRankScore::Uninit(void)
{
	// �I������
	CScore::Uninit();
}

//========================
// �X�V����
//========================
void CRankScore::Update(void)
{
	// �X�V����
	CScore::Update();
}

//========================
// �`�揈��
//========================
void CRankScore::Draw(void)
{
	// �`�揈��
	CScore::Draw();
}

//========================
// ���̐ݒ菈��
//========================
void CRankScore::SetData(const D3DXVECTOR3& pos, const int nScore)
{
	// ����ݒ菈��
	CScore::SetData(pos, NONE_D3DXVECTOR3, SCORE_SIZE, NUMBER_SHIFT, NUMBER_TEXTURE, 0);

	// �X�R�A��ݒ肷��
	SetScore(nScore);		// �X�R�A
	SetScoreDisp(nScore);	// �`��X�R�A
}

//========================
// ��������
//========================
CRankScore* CRankScore::Create(const D3DXVECTOR3& pos, const int nScore)
{
	// �X�R�A�̃|�C���^��錾
	CRankScore* pScore = nullptr;

	if (pScore == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ���������m�ۂ���
		pScore = new CRankScore;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pScore != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pScore->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pScore->SetData(pos, nScore);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �X�R�A�̃|�C���^��Ԃ�
	return pScore;
}