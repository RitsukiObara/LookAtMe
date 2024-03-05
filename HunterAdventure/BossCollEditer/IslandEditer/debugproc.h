//==========================================================
//
// �f�o�b�O�\������ [debugproc.h]
// Author Ibuki Okusada
//
//==========================================================
#ifndef _DEBUGPROC_H_		//���̃}�N������`����Ă��Ȃ��ꍇ
#define _DEBUGPROC_H_

#include "main.h"

//**********************************************************
// �}�N����`
//**********************************************************
#define MAX_DEBUGSTRING	(2048)	//�f�o�b�O�\���̍ő啶����

//**********************************************************
// �N���X�̒�`
//**********************************************************
class CDebugProc
{
public:		// �N�ł��A�N�Z�X�o����

	CDebugProc();	// �R���X�g���N�^
	~CDebugProc();	// �f�X�g���N�^

	// �����o�֐�
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void Print(const char *fmt, ...);	// �ݒ菈��

	// �ÓI�����o�֐�
	static CDebugProc* Create(void);	// ��������

private:	// �����̂݃A�N�Z�X�ł���

	// �����o�ϐ�
	char m_aStr[MAX_DEBUGSTRING];	//�f�o�b�O�\���p�̕�����
	bool m_bDisp;		//�f�o�b�O�\����ON/OFF

	// �ÓI�����o�ϐ�
	static LPD3DXFONT m_pFont;	//�t�H���g�ւ̃|�C���^
};

#endif