//===================================
//
// �`���[�g���A���S�[���w�b�_�[[tutorial_goal.h]
// Author�F��������
//
//===================================
#ifndef _TUTORIAL_GOAL_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _TUTORIAL_GOAL_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "goal.h"

//-----------------------------------
// �N���X��`(�S�[��)
//-----------------------------------
class CTutorialGoal : public CGoal
{
public:		// �N�ł��A�N�Z�X�ł���

	CTutorialGoal();			// �R���X�g���N�^
	~CTutorialGoal();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

private:	// ���������A�N�Z�X�ł���

};

#endif