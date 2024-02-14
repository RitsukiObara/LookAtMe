//============================================
//
// �Q�[���X�R�A�w�b�_�[[game_score.h]
// Author�F��������
//
//============================================
#ifndef _GAME_SCORE_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _GAME_SCORE_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "score.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;		// �I�u�W�F�N�g2D

//--------------------------------------------
// �N���X��`(�X�R�A)
//--------------------------------------------
class CGameScore : public CScore
{
public:				// �N�ł��A�N�Z�X�ł���

	CGameScore();			// �R���X�g���N�^
	~CGameScore();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CGameScore* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CObject2D* m_pScoreWord;		// �����̏��
};

#endif