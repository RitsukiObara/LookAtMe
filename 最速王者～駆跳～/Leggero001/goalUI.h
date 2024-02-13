//============================================
//
// �S�[��UI�w�b�_�[[GoalUI.h]
// Author�F��������
//
//============================================
#ifndef _GOALUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _GOALUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "model.h"

//--------------------------------------------
// �N���X��`(���l)
//--------------------------------------------
class CGoalUI : public CModel
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_APPEAR = 0,		// �o�����
		STATE_STOP,				// ��~���
		STATE_CLEAR,			// ��������
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

	CGoalUI();		// �R���X�g���N�^
	~CGoalUI();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);			// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CGoalUI* Create(const D3DXVECTOR3& pos);	// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	D3DXVECTOR3 InitPosCalc(const D3DXVECTOR3& pos);		// �����ʒu�ݒ菈��

	void Move(void);			// �ړ�����
	void Cycle(void);			// ��]����
	void AppearChange(void);	// �o����ԕω�����
	void Brake(void);			// �u���[�L����
	void StopChange(void);		// ��~��ԕω�����
	bool Clear(void);			// ��������

	// �����o�ϐ�
	D3DXVECTOR3 m_posDest;		// �ڕW�̈ʒu
	D3DXVECTOR3 m_move;			// �ړ���
	D3DXVECTOR3 m_rotmove;		// �����̈ړ���
	STATE m_state;				// ���
	int m_nStateCount;			// ��ԃJ�E���g
};

#endif