//============================================
//
// �����N�X�R�A�w�b�_�[[rank_score.h]
// Author�F��������
//
//============================================
#ifndef _RANK_SCORE_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _RANK_SCORE_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "score.h"

//--------------------------------------------
// �N���X��`(�����N�X�R�A)
//--------------------------------------------
class CRankScore : public CScore
{
public:				// �N�ł��A�N�Z�X�ł���

	CRankScore();			// �R���X�g���N�^
	~CRankScore();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const int nScore);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CRankScore* Create(const D3DXVECTOR3& pos, const int nScore);		// ��������

private:			// ���������A�N�Z�X�ł���

};

#endif