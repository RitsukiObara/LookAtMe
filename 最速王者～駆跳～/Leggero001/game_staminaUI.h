//============================================
//
// �Q�[���̃X�^�~�iUI�w�b�_�[[game_staminaUI.h]
// Author�F��������
//
//============================================
#ifndef _GAME_STAMINAUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _GAME_STAMINAUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "staminaUI.h"

//--------------------------------------------
// �N���X��`(�Q�[���̃X�^�~�iUI)
//--------------------------------------------
class CGameStaminaUI : public CStaminaUI
{
public:				// �N�ł��A�N�Z�X�ł���

	CGameStaminaUI();		// �R���X�g���N�^
	~CGameStaminaUI();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);			// ���̐ݒ菈��

private:			// ���������A�N�Z�X�ł���

};

#endif