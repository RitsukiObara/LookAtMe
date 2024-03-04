//============================================
//
// �����L���O�X�R�A����[ranking_score.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "ranking_table.h"
#include "file.h"
#include "game.h"
#include "sound.h"

#include "rank_score.h"

//--------------------------------------------
// �萔��`
//--------------------------------------------
namespace
{
	const float SCORE_HEIGHT_SHIFT = 120.0f;		// �X�R�A�̏c�̂��炷��
	const int NO_NEW_RECORD = 0;					// �V�L�^����Ȃ��Ƃ��̐��l
	const int SOUND_COUNT = 70;						// ������J�E���g��
	const D3DXVECTOR3 SCORE_POS = D3DXVECTOR3(350.0f, 140.0f, 0.0f);			// �X�R�A�̈ʒu
	const D3DXVECTOR3 SCORE_TRANS_POS = D3DXVECTOR3(500.0f, 220.0f, 0.0f);		// �����J�ڌ�̃X�R�A�̈ʒu
}


//========================
// �R���X�g���N�^
//========================
CRankingTable::CRankingTable() : CObject(CObject::TYPE_RANKINGTABLE, DIM_2D, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++)
	{
		m_apScore[nCnt] = nullptr;				// ���Ԃ̃|�C���^
	}

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_nNewRecordRank = NO_NEW_RECORD;			// �V�L�^�̏���
	m_nSoundCount = 0;							// ����炷�J�E���g
}

//========================
// �f�X�g���N�^
//========================
CRankingTable::~CRankingTable()
{

}

//========================
// ����������
//========================
HRESULT CRankingTable::Init(void)
{
	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CRankingTable::Uninit(void)
{
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++)
	{
		if (m_apScore[nCnt] != nullptr)
		{ // �^�C�}�[�̏�� NULL ����Ȃ��ꍇ

			// �I������
			m_apScore[nCnt]->Uninit();
			m_apScore[nCnt] = nullptr;
		}
	}

	// �j������
	Release();
}

//========================
// �X�V����
//========================
void CRankingTable::Update(void)
{
	if (m_nNewRecordRank > NO_NEW_RECORD && 
		m_nNewRecordRank <= CRanking::MAX_RANKING &&
		m_apScore[m_nNewRecordRank - 1] != nullptr)
	{ // �V�L�^����������A�����̃|�C���^���������ꍇ

		for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
		{
			// �F��ݒ肷��
			m_apScore[m_nNewRecordRank - 1]->GetNumber(nCnt)->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	if (m_nNewRecordRank != NO_NEW_RECORD)
	{ // �V�L�^������Ă����ꍇ

		// ����炷�J�E���g�����Z����
		m_nSoundCount++;

		if (m_nSoundCount % SOUND_COUNT == 0)
		{ // �J�E���g������萔�ɒB�����ꍇ

			// �V�L�^�̉���炷
			//CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_NEWRECORD);
		}
	}
}

//========================
// �`�揈��
//========================
void CRankingTable::Draw(void)
{
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++)
	{
		// �`�揈��
		m_apScore[nCnt]->Draw();
	}
}

//========================
// ���̐ݒ菈��
//========================
void CRankingTable::SetData(void)
{
	// ���_�ƈʒu
	D3DXVECTOR3 pos;
	int aScore[CRanking::MAX_RANKING];

	// �����L���O�̃��[�h����
	CManager::Get()->GetFile()->Load(CFile::TYPE_RANKING);

	// �����L���O�̓���ւ�����
	RankSwap(CGame::GetScore(), aScore);

	// ���ʂ̃Z�[�u����
	CManager::Get()->GetFile()->Save(CFile::TYPE_RANKING);

	for (int nCntRank = 0; nCntRank < CRanking::MAX_RANKING; nCntRank++)
	{
		// �ʒu��ݒ肷��
		pos = D3DXVECTOR3(SCORE_POS.x, SCORE_POS.y + (SCORE_HEIGHT_SHIFT * nCntRank), 0.0f);

		if (m_apScore[nCntRank] == nullptr)
		{ // �X�R�A�� NULL �̏ꍇ

			// �X�R�A�𐶐�����
			m_apScore[nCntRank] = CRankScore::Create(pos, aScore[nCntRank]);
		}

		//if (nCntRank == m_nNewRecordRank - 1)
		//{ // �V�L�^�������ꍇ

		//	// �V�L�^�\���̐���
		//	CNewRecord::Create(m_apScore[nCntRank]->GetNumber(0)->GetPos());
		//}
	}
}

//========================================
// �����L���O�̓���ւ�����
//========================================
void CRankingTable::RankSwap(int nScore, int* aSave)
{
	// ���[�J���ϐ��錾
	int nTemp;							// �ꎞ�I�ȕϐ�
	bool bUpdate = false;				// �V�L�^���X�V���ꂽ���ǂ���

	// �����L���O���擾����
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++)
	{
		aSave[nCnt] = CManager::Get()->GetFile()->GetRankingInfo().aRank[nCnt];
	}

	for (int nCount = 0; nCount < CRanking::MAX_RANKING; nCount++)
	{ // �����L���O���r����

		if (aSave[nCount] <= nScore)
		{ // ����̃X�R�A�̐��l�̕����傫�������ꍇ

			// �ꎞ�I�ȕϐ��Ƀ����L���O�̐��l��������
			nTemp = aSave[nCount];

			// ���ʂ������L���O�̒��ɑ������
			aSave[nCount] = nScore;

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
	CManager::Get()->GetFile()->SetRankingInfo(aSave);
}

//========================
// ��������
//========================
CRankingTable* CRankingTable::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CRankingTable* pTime = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pTime == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

	  // �I�u�W�F�N�g�𐶐�
		pTime = new CRankingTable;
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

			// ��~
			assert(false);

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