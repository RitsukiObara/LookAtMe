//============================================
//
// �`���[�g���A���̃X�^�~�iUI�w�b�_�[[tutorial_staminaUI.h]
// Author�F��������
//
//============================================
#ifndef _TUTORIAL_STAMINAUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _TUTORIAL_STAMINAUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "staminaUI.h"

//--------------------------------------------
// �N���X��`(�Q�[���̃X�^�~�iUI)
//--------------------------------------------
class CTutorialStaminaUI : public CStaminaUI
{
public:				// �N�ł��A�N�Z�X�ł���

	CTutorialStaminaUI();		// �R���X�g���N�^
	~CTutorialStaminaUI();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);			// ���̐ݒ菈��

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void PosCorrect(void);		// �ʒu�̕␳����

	// �����o�ϐ�
	float m_posDest2D[TYPE_MAX];		// �ړI�̈ʒu
	float m_posDestAnim;				// �ړI�̈ʒu
};

#endif