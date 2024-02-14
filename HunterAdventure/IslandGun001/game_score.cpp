//============================================
//
// �Q�[���̃X�R�A����[game_score.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "game_score.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

//--------------------------------------------
// �萔��`
//--------------------------------------------
namespace
{
	// �����֌W
	const char* NUMBER_TEXTURE = "data\\TEXTURE\\Number.png";			// �����̃e�N�X�`��
	const D3DXVECTOR3 SCORE_POS = D3DXVECTOR3(1050.0f, 25.0f, 0.0f);	// �ʒu
	const D3DXVECTOR3 SCORE_SIZE = D3DXVECTOR3(15.0f, 20.0f, 0.0f);		// �T�C�Y
	const float NUMBER_SHIFT = 30.0f;		// �����̂��炷��
	const int ADD_DISP = 20;				// �\���X�R�A�̉��Z��

	// �����֌W
	const char* WORD_TEXTURE = "data\\TEXTURE\\ScoreWord.png";			// �����̃e�N�X�`��
	const D3DXVECTOR3 WORD_POS = D3DXVECTOR3(950.0f, 25.0f, 0.0f);		// �����̈ʒu
	const D3DXVECTOR3 WORD_SIZE = D3DXVECTOR3(80.0f, 20.0f, 0.0f);		// �����̃T�C�Y
}

//========================
// �R���X�g���N�^
//========================
CGameScore::CGameScore() : CScore()
{
	// �S�Ă̒l���N���A����
	m_pScoreWord = nullptr;		// �����̏��
}

//========================
// �f�X�g���N�^
//========================
CGameScore::~CGameScore()
{

}

//========================
// ����������
//========================
HRESULT CGameScore::Init(void)
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
void CGameScore::Uninit(void)
{
	if (m_pScoreWord != nullptr)
	{ // �����̏�� NULL ����Ȃ��ꍇ

		// �X�R�A�̏I������
		m_pScoreWord->Uninit();
		m_pScoreWord = nullptr;
	}

	// �I������
	CScore::Uninit();
}

//========================
// �X�V����
//========================
void CGameScore::Update(void)
{
	// �X�V����
	CScore::Update();
}

//========================
// �`�揈��
//========================
void CGameScore::Draw(void)
{
	if (m_pScoreWord != nullptr)
	{ // ������ NULL ����Ȃ��ꍇ

		// �����̕`�揈��
		m_pScoreWord->Draw();
	}

	// �`�揈��
	CScore::Draw();
}

//========================
// ���̐ݒ菈��
//========================
void CGameScore::SetData(void)
{
	// ����ݒ菈��
	CScore::SetData(SCORE_POS, NONE_D3DXVECTOR3, SCORE_SIZE, NUMBER_SHIFT, NUMBER_TEXTURE, ADD_DISP);

	// �S�Ă̒l��ݒ肷��
	if (m_pScoreWord == nullptr)
	{ // ������ NULL �̏ꍇ

		// �����𐶐�
		m_pScoreWord = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		
		// ���̐ݒ�
		m_pScoreWord->SetPos(WORD_POS);				// �ʒu
		m_pScoreWord->SetPosOld(WORD_POS);			// �O��̈ʒu
		m_pScoreWord->SetRot(NONE_D3DXVECTOR3);		// ����
		m_pScoreWord->SetSize(WORD_SIZE);			// �T�C�Y
	
		// �e�N�X�`���̊��蓖�ď���
		m_pScoreWord->BindTexture(CManager::Get()->GetTexture()->Regist(WORD_TEXTURE));

		// �T�C�Y�̐ݒ菈��
		m_pScoreWord->SetVertex();
	}

}

//========================
// ��������
//========================
CGameScore* CGameScore::Create(void)
{
	// �X�R�A�̃|�C���^��錾
	CGameScore* pScore = nullptr;

	if (pScore == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ���������m�ۂ���
		pScore = new CGameScore;
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