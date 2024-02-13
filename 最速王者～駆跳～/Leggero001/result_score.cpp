//============================================
//
// ���U���g�X�R�A����[result_score.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "result.h"
#include "manager.h"
#include "result_score.h"
#include "result_rank.h"
#include "file.h"
#include "renderer.h"
#include "texture.h"
#include "sound.h"
#include "useful.h"

// �}�N����`
#define RESULTSCORE_TEXTURE			"data/TEXTURE/Number.png"				// ���U���g�X�R�A�̃e�N�X�`��
#define RESULTSCORE_WIDTH_SHIFT		(120.0f)								// ���U���g�X�R�A�̉��̂��炷��
#define RESULTSCORE_SIZE			(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// ���U���g�X�R�A�̃T�C�Y
#define RESULTSCORE_INIT_POS		(D3DXVECTOR3(-1000.0f, 500.0f, 0.0f))	// ���U���g�X�R�A�̏����ʒu
#define RESULTSCORE_DEST_POS		(D3DXVECTOR3(100.0f, 500.0f, 0.0f))		// ���U���g�X�R�A�̖ړI�̈ʒu
#define RESULTSCORE_RAND			(10)									// ���U���g�X�R�A�̃����_���Ō��o���镝
#define RESULTSCORE_MOVE			(70.0f)									// ���U���g�X�R�A�̈ړ���
#define RESULTSCORE_RANK_POS		(D3DXVECTOR3(320.0f, -150.0f, 100.0f))	// ���U���g�X�R�A�̃����N�̈ʒu

//========================
// �R���X�g���N�^
//========================
CResultScore::CResultScore() : CObject(TYPE_SCORE, PRIORITY_UI)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		// �����̏��
		m_apNumber[nCnt] = nullptr;
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_nScore = 0;		// �X�R�A
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
	//���[�J���ϐ��錾
	int nTexIdx = NONE_TEXIDX;		// �e�N�X�`���̃C���f�b�N�X

	// �e�N�X�`���̓ǂݍ��ݏ���
	nTexIdx = CManager::GetTexture()->Regist(RESULTSCORE_TEXTURE);

	// ���������m�ۂ���
	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] == nullptr)
		{ // �|�C���^�� NULL �̏ꍇ

			// ���������m�ۂ���
			m_apNumber[nCnt] = new CNumber(CObject::TYPE_NONE, CObject::PRIORITY_UI);
		}
		else
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �x����
			MessageBox(NULL, "�X�R�A�̃����������Ɏg���Ă���I", "�x���I", MB_ICONWARNING);

			// ���s��Ԃ�
			return E_FAIL;
		}

		if (m_apNumber[nCnt] != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// ����������
			if (FAILED(m_apNumber[nCnt]->Init()))
			{ // �����������Ɏ��s�����ꍇ

				// �x����
				MessageBox(NULL, "�X�R�A�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

				// ���s��Ԃ�
				return E_FAIL;
			}

			// �e�N�X�`���̊��蓖�ď���
			m_apNumber[nCnt]->BindTexture(nTexIdx);
		}
		else
		{ // �|�C���^���@NULL �̏ꍇ

			// �x����
			MessageBox(NULL, "�X�R�A�̃������̊m�ۂɎ��s�I", "�x���I", MB_ICONWARNING);

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	m_pos = RESULTSCORE_INIT_POS;		// �ʒu
	m_nScore = CManager::GetScore();	// �X�R�A

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CResultScore::Uninit(void)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		// �I������
		m_apNumber[nCnt]->Uninit();
	}

	// �����̏I������
	Release();
}

//========================
// �X�V����
//========================
void CResultScore::Update(void)
{
	if (CResult::GetState() == CResult::STATE_SCOREDISP)
	{ // �X�R�A�\����Ԃ̏ꍇ

		// �ړ�����
		MoveProcess();

		// �v�Z����
		Calculate();
	}
}

//========================
// �`�揈��
//========================
void CResultScore::Draw(void)
{
	// �A���t�@�e�X�g��L���ɂ���
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		// �A���t�@�e�X�g�̐ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);	// �A���t�@�e�X�g�̎Q�ƒl�ݒ�

	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		// �`�揈��
		m_apNumber[nCnt]->Draw();
	}

	// �A���t�@�e�X�g�𖳌��ɂ���
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// �A���t�@�e�X�g�̐ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0);					// �A���t�@�e�X�g�̎Q�ƒl�ݒ�
}

//========================
// �v�Z����
//========================
void CResultScore::Calculate(void)
{
	// ���[�J���ϐ��錾
	int aNum[MAX_RESULTSCORE_DIGIT];		// ���l

	// 10�i���̕ϊ�����
	DecimalCalculation(MAX_RESULTSCORE_DIGIT, m_nScore, &aNum[0]);

	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		// �����̐ݒ菈��
		m_apNumber[nCnt]->SetNumber(aNum[nCnt]);

		// �e�N�X�`�����W�̐ݒ菈��
		m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_apNumber[nCnt]->GetNumber());
	}
}

//========================
// �ړ�����
//========================
void CResultScore::MoveProcess(void)
{
	// �ʒu���X�V����
	m_pos.x += RESULTSCORE_MOVE;

	if (m_pos.x >= RESULTSCORE_DEST_POS.x)
	{ // �ʒu����萔�ȏ�ɂȂ����ꍇ

		// �ʒu��␳����
		m_pos.x = RESULTSCORE_DEST_POS.x;

		// ��~��Ԃɂ���
		CResult::SetState(CResult::STATE_STOP);

		// �����N��ݒ肷��
		CResultRank::Create(RESULTSCORE_RANK_POS, DecisionRank());

		// �����N�̏o������炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RANKAPPEAR);
	}

	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �ݒ菈��
			m_apNumber[nCnt]->SetPos(D3DXVECTOR3(m_pos.x + (nCnt * RESULTSCORE_WIDTH_SHIFT), m_pos.y, 0.0f));				// �ʒu�ݒ�

			// ���_���̐ݒ菈��
			m_apNumber[nCnt]->SetVertex();
		}
	}
}

//========================
// �����N�̌��菈��
//========================
CResult::RANK CResultScore::DecisionRank(void)
{
	// ���[�J���ϐ��錾
	CFile::SResultFile result = CManager::GetFile()->GetResultInfo();		// ���U���g�̏����擾����
	CResult::RANK rank = CResult::RANK_C;		// �����N

	if (CManager::GetScore() >= result.aRankScore[CResult::RANK_SS])
	{ // SS�����N�̊�������������ꍇ

		// SS�����N��ݒ肷��
		rank = CResult::RANK_SS;
	}
	else if (CManager::GetScore() >= result.aRankScore[CResult::RANK_S])
	{ // S�����N�̊�������������ꍇ

		// S�����N��ݒ肷��
		rank = CResult::RANK_S;
	}
	else if (CManager::GetScore() >= result.aRankScore[CResult::RANK_A])
	{ // A�����N�̊�������������ꍇ

		// A�����N��ݒ肷��
		rank = CResult::RANK_A;
	}
	else if (CManager::GetScore() >= result.aRankScore[CResult::RANK_B])
	{ // B�����N�̊�������������ꍇ

		// B�����N��ݒ肷��
		rank = CResult::RANK_B;
	}
	else if (CManager::GetScore() >= result.aRankScore[CResult::RANK_C])
	{ // C�����N�̊�������������ꍇ

		// C�����N��ݒ肷��
		rank = CResult::RANK_C;
	}

	// �����N��Ԃ�
	return rank;
}

//========================
// ��������
//========================
CResultScore* CResultScore::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CResultScore* pScore = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pScore == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pScore = new CResultScore;
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
			MessageBox(NULL, "�X�R�A�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
		{
			// �ݒ菈��
			pScore->m_apNumber[nCnt]->SetPos(D3DXVECTOR3(pScore->m_pos.x + (nCnt * RESULTSCORE_WIDTH_SHIFT), pScore->m_pos.y, 0.0f));				// �ʒu�ݒ�
			pScore->m_apNumber[nCnt]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �����ݒ�
			pScore->m_apNumber[nCnt]->SetSize(RESULTSCORE_SIZE);					// �T�C�Y�ݒ�
			pScore->m_apNumber[nCnt]->SetLength();									// �����ݒ�
			pScore->m_apNumber[nCnt]->SetAngle();									// �����ݒ�

			pScore->m_apNumber[nCnt]->SetType(CNumber::TYPE_DECIMAL);	// �ԍ��̎��
			pScore->m_apNumber[nCnt]->SetNumber(0);						// �ԍ���ݒ肷��

			// ���_���̐ݒ菈��
			pScore->m_apNumber[nCnt]->SetVertex();
			
			// �e�N�X�`���̐ݒ菈��(�A�j���[�V�����o�[�W����)
			pScore->m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, 0);
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pScore;
}