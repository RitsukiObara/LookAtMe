//============================================
//
// �����L���O�X�R�A����[ranking_score.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "ranking_score.h"
#include "manager.h"
#include "time.h"
#include "sound.h"

#include "file.h"
#include "new_record.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define SCORE_WIDTH_SHIFT		(70.0f)									// �X�R�A�̉��̂��炷��
#define SCORE_HEIGHT_SHIFT		(110.0f)								// �X�R�A�̏c�̂��炷��
#define SCORE_SIZE				(D3DXVECTOR3(30.0f, 40.0f, 0.0f))		// �X�R�A�̃T�C�Y
#define SCORE_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 150.0f, 220.0f, 0.0f))		// �X�R�A�̈ʒu
#define SCORE_TRANS_POS			(D3DXVECTOR3(500.0f, 220.0f, 0.0f))		// �����J�ڌ�̃X�R�A�̈ʒu
#define NO_NEW_RECORD			(0)										// �V�L�^����Ȃ��Ƃ��̐��l
#define SCOREDOT_SIZE			(D3DXVECTOR3(8.0f, 8.0f, 0.0f))			// �X�R�A�̓_�̃T�C�Y
#define SOUND_COUNT				(70)									// ������J�E���g��

//========================
// �R���X�g���N�^
//========================
CRankingScore::CRankingScore() : CObject(CObject::TYPE_RANKINGSCORE, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_apTime[nCnt] = nullptr;				// ���Ԃ̃|�C���^
	}

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_nNewRecordRank = NO_NEW_RECORD;			// �V�L�^�̏���
	m_nSoundCount = 0;							// ����炷�J�E���g
}

//========================
// �f�X�g���N�^
//========================
CRankingScore::~CRankingScore()
{

}

//========================
// ����������
//========================
HRESULT CRankingScore::Init(void)
{
	// ���������m�ۂ���
	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		if (m_apTime[nCnt] == nullptr)
		{ // �|�C���^�� NULL �̏ꍇ

			// ���������m�ۂ���
			m_apTime[nCnt] = new CTime(CObject::TYPE_NONE, CObject::PRIORITY_UI);
		}
		else
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �x����
			MessageBox(NULL, "���Ԃ̃����������Ɏg���Ă���I", "�x���I", MB_ICONWARNING);

			// ���s��Ԃ�
			return E_FAIL;
		}

		if (m_apTime[nCnt] != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// ����������
			if (FAILED(m_apTime[nCnt]->Init()))
			{ // �����������Ɏ��s�����ꍇ

				// �x����
				MessageBox(NULL, "���Ԃ̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

				// ���s��Ԃ�
				return E_FAIL;
			}
		}
		else
		{ // �|�C���^���@NULL �̏ꍇ

			// �x����
			MessageBox(NULL, "���Ԃ̃������̊m�ۂɎ��s�I", "�x���I", MB_ICONWARNING);

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	if (CManager::GetClear() == true)
	{ // �N���A��̏ꍇ

		// �ʒu
		m_pos = SCORE_POS;
	}
	else
	{ // ��L�ȊO

		// �ʒu
		m_pos = SCORE_TRANS_POS;
	}
	// �S�Ă̒l������������
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_nNewRecordRank = NO_NEW_RECORD;			// �V�L�^�̏���
	m_nSoundCount = 0;							// ����炷�J�E���g

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CRankingScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (m_apTime[nCnt] != nullptr)
		{ // �^�C�}�[�̏�� NULL ����Ȃ��ꍇ

			// �I������
			m_apTime[nCnt]->Uninit();
			m_apTime[nCnt] = nullptr;
		}
	}

	// �j������
	Release();
}

//========================
// �X�V����
//========================
void CRankingScore::Update(void)
{
	if (m_nNewRecordRank > NO_NEW_RECORD && 
		m_nNewRecordRank <= MAX_RANKING &&
		m_apTime[m_nNewRecordRank - 1] != nullptr)
	{ // �V�L�^����������A�����̃|�C���^���������ꍇ

		for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
		{
			// �F��ݒ肷��
			m_apTime[m_nNewRecordRank - 1]->GetNumber(nCnt)->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// 1�����Ƃ̐����ݒ菈��
		m_apTime[nCnt]->SetNumber();

		// 1�����Ƃ̃e�N�X�`���ݒ菈��
		m_apTime[nCnt]->SetTexture();
	}

	if (m_nNewRecordRank != NO_NEW_RECORD)
	{ // �V�L�^������Ă����ꍇ

		// ����炷�J�E���g�����Z����
		m_nSoundCount++;

		if (m_nSoundCount % SOUND_COUNT == 0)
		{ // �J�E���g������萔�ɒB�����ꍇ

			// �V�L�^�̉���炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_NEWRECORD);
		}
	}
}

//========================
// �`�揈��
//========================
void CRankingScore::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// �`�揈��
		m_apTime[nCnt]->Draw();
	}
}

//========================
// ���̐ݒ菈��
//========================
void CRankingScore::SetData(void)
{
	// �����L���O�̃��[�h����
	CManager::GetFile()->Load(CFile::TYPE_RANKING);

	if (CManager::GetClear() == true)
	{ // �N���A�󋵂� true �������ꍇ

		// �����L���O�̓���ւ�����
		RankSwap(CManager::GetClearTime());

		// ���ʂ̃Z�[�u����
		CManager::GetFile()->Save(CFile::TYPE_RANKING);
	}

	// �ʒu��ݒ肷��
	D3DXVECTOR3 pos;

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// �ʒu��ݒ肷��
		pos.x = m_pos.x;
		pos.y = m_pos.y + (nCntRank * SCORE_HEIGHT_SHIFT);
		pos.z = 0.0f;

		m_apTime[nCntRank]->SetData(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCORE_SIZE, SCOREDOT_SIZE, SCORE_WIDTH_SHIFT);

		// ���ʂ�������
		m_apTime[nCntRank]->SetTotalTime(CManager::GetFile()->GetRankingInfo().aRank[nCntRank]);

		// ���l�̐ݒ菈��
		m_apTime[nCntRank]->SetNumber();

		// 1�����Ƃ̃e�N�X�`���̐ݒ菈��
		m_apTime[nCntRank]->SetTexture();

		if (nCntRank == m_nNewRecordRank - 1)
		{ // �V�L�^�������ꍇ

			// �V�L�^�\���̐���
			CNewRecord::Create(m_apTime[nCntRank]->GetNumber(0)->GetPos());
		}
	}
}

//========================================
// �����L���O�̓���ւ�����
//========================================
void CRankingScore::RankSwap(int nScore)
{
	// ���[�J���ϐ��錾
	int aScore[MAX_RANKING];		// �X�R�A
	int nTemp;						// �ꎞ�I�ȕϐ�
	bool bUpdate = false;			// �V�L�^���X�V���ꂽ���ǂ���

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		aScore[nCnt] = CManager::GetFile()->GetRankingInfo().aRank[nCnt];
	}

	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{ // �����L���O���r����

		if (aScore[nCount] >= nScore)
		{ // �N���A�^�C���̐��l�̕��������������ꍇ

			// �ꎞ�I�ȕϐ��Ƀ����L���O�̐��l��������
			nTemp = aScore[nCount];

			// ���ʂ������L���O�̒��ɑ������
			aScore[nCount] = nScore;

			// ���ʂ��ꎞ�I�ȕϐ��ɕς���
			nScore = nTemp;

			if (bUpdate == false)
			{ // �V�L�^���X�V����Ă��Ȃ������ꍇ

				// ���̃J�E���g��V�L�^�̔ԍ��Ƃ���
				m_nNewRecordRank = nCount + 1;

				// �V�L�^�X�V
				bUpdate = true;
			}
		}
	}

	// �����L���O�̐ݒ菈��
	CManager::GetFile()->SetRankingInfo(aScore);
}

//========================
// ��������
//========================
CRankingScore* CRankingScore::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CRankingScore* pTime = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pTime == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

	  // �I�u�W�F�N�g�𐶐�
		pTime = new CRankingScore;
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
		pTime->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

	  // NULL ��Ԃ�
		return nullptr;
	}

	// ���Ԃ̃|�C���^��Ԃ�
	return pTime;
}