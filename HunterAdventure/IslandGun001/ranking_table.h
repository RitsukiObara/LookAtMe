//===================================
//
// �����L���O�w�b�_�[[ranking_score.h]
// Author ��������
//
//===================================
#ifndef _RANKING_SCORE_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _RANKING_SCORE_H_			// 2�d�C���N���[�h�h�~�̃}�N����ݒ肷��

//-----------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------
#include "object.h"
#include "ranking.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CRankScore;		// �����N�X�R�A

//-----------------------------------
// �}�N����`
//-----------------------------------

// �N���X��`(�����L���O)
class CRankingTable : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	CRankingTable();			// �R���X�g���N�^
	~CRankingTable();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V
	void Draw(void);			// �`�揈��

	void SetData(void);			// ���̐ݒ菈��

	// �ÓI�����o�ϐ�
	static CRankingTable* Create(void);	// �����L���O��ʂ̐�������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void RankSwap(int nScore, int* aSave);	// �����L���O�̓���ւ�����

	// �����o�ϐ�
	D3DXCOLOR m_col;					// �V�L�^�̐F
	CRankScore* m_apScore[CRanking::MAX_RANKING];	// �����L���O
	int m_nNewRecordRank;				// �V�L�^�̏���
	int m_nSoundCount;					// ����炷�J�E���g
};

#endif