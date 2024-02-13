//============================================
//
// �`���[�g���A���̑��xUI�w�b�_�[[tutorial_speedUI.h]
// Author�F��������
//
//============================================
#ifndef _TUTORIALSPEEDUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _TUTORIALSPEEDUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "speedUI.h"

//--------------------------------------------
// �N���X��`(���xUI)
//--------------------------------------------
class CTutorialSpeedUI : public CSpeedUI
{
public:				// �N�ł��A�N�Z�X�ł���

	CTutorialSpeedUI();		// �R���X�g���N�^
	~CTutorialSpeedUI();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);		// ���̐ݒ菈��

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void PosCorrect(void);	// �␳����

	// �����o�ϐ�
	float m_posDest[TYPE_MAX];		// �ړI�̈ʒu
};

#endif