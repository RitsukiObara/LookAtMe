//============================================
//
// ���U���g�Q�[���̃X�R�A����[result_score.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "result_score.h"
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
	const D3DXVECTOR3 SCORE_SIZE = D3DXVECTOR3(60.0f, 80.0f, 0.0f);		// �T�C�Y
	const D3DXVECTOR3 SCORE_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SCORE_SIZE.x * 7, SCREEN_HEIGHT * 0.5f, 0.0f);	// �ʒu
	const float NUMBER_SHIFT = 120.0f;		// �����̂��炷��
	const int ADD_DISP = 100;				// �\���X�R�A�̉��Z��
}

//========================
// �R���X�g���N�^
//========================
CResultScore::CResultScore() : CScore()
{

}

//========================
// �f�X�g���N�^
//========================
CResultScore::~CResultScore()
{

}

//========================
// ����������
//========================
HRESULT CResultScore::Init(void)
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
void CResultScore::Uninit(void)
{
	// �I������
	CScore::Uninit();
}

//========================
// �X�V����
//========================
void CResultScore::Update(void)
{
	// �X�V����
	CScore::Update();
}

//========================
// �`�揈��
//========================
void CResultScore::Draw(void)
{
	// �`�揈��
	CScore::Draw();
}

//========================
// ���̐ݒ菈��
//========================
void CResultScore::SetData(void)
{
	// ����ݒ菈��
	CScore::SetData(SCORE_POS, NONE_D3DXVECTOR3, SCORE_SIZE, NUMBER_SHIFT, NUMBER_TEXTURE, ADD_DISP);
}

//========================
// ��������
//========================
CResultScore* CResultScore::Create(void)
{
	// �X�R�A�̃|�C���^��錾
	CResultScore* pScore = nullptr;

	if (pScore == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ���������m�ۂ���
		pScore = new CResultScore;
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
		pScore->SetData();
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