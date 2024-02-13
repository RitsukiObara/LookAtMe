//============================================
//
// �����L���O�^�C�}�[�w�b�_�[[ranking_time.h]
// Author�F��������
//
//============================================
#ifndef _RANKING_TIME_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _RANKING_TIME_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "time.h"

//--------------------------------------------
// �N���X��`(�^�C��)
//--------------------------------------------
class CRankingTime : public CTime
{
public:				// �N�ł��A�N�Z�X�ł���

	CRankingTime();				// �R���X�g���N�^
	~CRankingTime();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void Calculate(void);	// �v�Z����

	// �ÓI�����o�ϐ�
	static CRankingTime* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;		// �ʒu
};

#endif