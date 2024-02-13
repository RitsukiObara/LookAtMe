//============================================
//
// ���U���g�\���p�^�C�}�[�w�b�_�[[result_time.h]
// Author�F��������
//
//============================================
#ifndef _RESULT_TIME_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _RESULT_TIME_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "result.h"
#include "time.h"

//--------------------------------------------
// �N���X��`(�^�C��)
//--------------------------------------------
class CResultTime : public CTime
{
public:				// �N�ł��A�N�Z�X�ł���

	CResultTime();				// �R���X�g���N�^
	~CResultTime();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �ÓI�����o�ϐ�
	static CResultTime* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Calculate(void);		// �v�Z����
	void MoveProcess(void);		// �ړ�����
	void DotMove(void);			// �_�̈ړ�����
	CResult::RANK DecisionRank(void);		// �����N�̌��菈��

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;							// �ʒu
};

#endif