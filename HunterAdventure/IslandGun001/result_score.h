//============================================
//
// ���U���g�X�R�A�w�b�_�[[result_score.h]
// Author�F��������
//
//============================================
#ifndef _RESULT_SCORE_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _RESULT_SCORE_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "score.h"

//--------------------------------------------
// �N���X��`(���U���g�X�R�A)
//--------------------------------------------
class CResultScore : public CScore
{
public:				// �N�ł��A�N�Z�X�ł���

	CResultScore();			// �R���X�g���N�^
	~CResultScore();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CResultScore* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

};

#endif