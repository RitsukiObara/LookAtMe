//===================================
//
// �X�R�A�̕����\���w�b�_�[[ranking_scoredisp.h]
// Author ��������
//
//===================================
#ifndef _RANKING_SCOREDISP_H_			//���̃}�N����`������Ă��Ȃ�������
#define _RANKING_SCOREDISP_H_			//2�d�C���N���[�h�h�~�̃}�N����ݒ肷��

//-----------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------
#include "object2D.h"

//-----------------------------------
// �N���X��`(�X�R�A�̕���)
//-----------------------------------
class CRankingScoreDisp : public CObject2D
{
public:			// �N�ł��A�N�Z�X�ł���

	CRankingScoreDisp();		// �R���X�g���N�^
	~CRankingScoreDisp();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// �X�R�A�̕����̏���������
	void Uninit(void);			// �X�R�A�̕����̏I������
	void Update(void);			// �X�R�A�̕����̍X�V
	void Draw(void);			// �X�R�A�̕����̕`�揈��

	// �ÓI�����o�֐�
	static CRankingScoreDisp* Create(void);	// ��������

private:		// ���������A�N�Z�X�ł���

};

#endif