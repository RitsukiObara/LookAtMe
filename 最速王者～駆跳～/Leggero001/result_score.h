//============================================
//
// �X�R�A�w�b�_�[[result_score.h]
// Author�F��������
//
//============================================
#ifndef _RESULT_SCORE_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _RESULT_SCORE_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "number.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define MAX_RESULTSCORE_DIGIT		(6)			// �X�R�A�̌���

//--------------------------------------------
// �N���X��`(�^�C��)
//--------------------------------------------
class CResultScore : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	CResultScore();			// �R���X�g���N�^
	~CResultScore();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �ÓI�����o�ϐ�
	static CResultScore* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Calculate(void);	// �v�Z����
	void MoveProcess(void);	// �ړ�����
	CResult::RANK DecisionRank(void);		// �����N�̌��菈��

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;								// �ʒu
	CNumber *m_apNumber[MAX_RESULTSCORE_DIGIT];		// �ԍ��̏��
	int m_nScore;			// �X�R�A
};

#endif