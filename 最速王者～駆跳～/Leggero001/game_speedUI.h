//============================================
//
// �Q�[���̑��xUI�w�b�_�[[game_speedUI.h]
// Author�F��������
//
//============================================
#ifndef _GAMESPEEDUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _GAMESPEEDUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "speedUI.h"

//--------------------------------------------
// �N���X��`(���xUI)
//--------------------------------------------
class CGameSpeedUI : public CSpeedUI
{
public:				// �N�ł��A�N�Z�X�ł���

	CGameSpeedUI();			// �R���X�g���N�^
	~CGameSpeedUI();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);		// ���̐ݒ菈��

private:			// ���������A�N�Z�X�ł���

};

#endif