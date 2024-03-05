//=======================================
//
//���̓w�b�_�[[input.h]
//Author ��������
//
//=======================================
#ifndef _IMPUT_H_			//���̃}�N����`������Ă��Ȃ�������
#define _INPUT_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include<Xinput.h>

#pragma comment(lib,"xinput.lib")

//---------------------------------------
// �}�N����`
//---------------------------------------
#define NUM_KEY_MAX			(256)			// �L�[�̍ő吔
#define MAX_PLAYER			(4)				// �v���C�o����l��

//---------------------------------------
// �N���X��`(CInput)
//---------------------------------------
class CInput
{
public:			// �N�ł��A�N�Z�X�o����

	CInput();				// �R���X�g���N�^
	~CInput();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance);				// ����������
	virtual void Uninit(void);						// �I������
	virtual void Update(void) = 0;					// �X�V����

protected:		// �����Ɣh���N���X�̂݃A�N�Z�X�ł���

	// �����o�ϐ�
	LPDIRECTINPUTDEVICE8 m_pDevice;				// ���̓f�o�C�X�ւ̃|�C���^

	// �ÓI�����o�ϐ�
	static LPDIRECTINPUT8 m_pInput;				// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
};

//---------------------------------------
// �L�[�{�[�h�N���X
//---------------------------------------
class CInputKeyboard : public CInput
{
public:			// �N�ł��A�N�Z�X�ł���

	CInputKeyboard();		// �R���X�g���N�^
	~CInputKeyboard();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// ����������
	void Uninit(void) override;							// �I������
	void Update(void) override;							// �X�V����
	bool GetPress(int nKey);							// �v���X����
	bool GetTrigger(int nKey);							// �g���K�[����
	bool GetRelease(int nKey);							// �����[�X����
	bool GetRepeat(int nKey, int nCount);				// ���s�[�g����

	// �ÓI�����o�֐�
	static CInputKeyboard* Create(HINSTANCE hInstance, HWND hWnd);		// ��������

private:		// �����̂݃A�N�Z�X�ł���

	// �����o�ϐ�
	BYTE m_aKeyState[NUM_KEY_MAX];						// �v���X���
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];				// �g���K�[���
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];				// �����[�X���
	int m_nRepeatCount[NUM_KEY_MAX];					// ���s�[�g�J�E���g
};

//---------------------------------------
// �W���C�p�b�h�N���X
//---------------------------------------
class CInputGamePad : public CInput
{
public:			// �N�ł��A�N�Z�X�ł���

	//�Q�[���p�b�h�̃{�^��
	enum JOYKEY
	{
		JOYKEY_UP = 0,			//��{�^��
		JOYKEY_DOWN,			//���{�^��
		JOYKEY_LEFT,			//���{�^��
		JOYKEY_RIGHT,			//�E�{�^��
		JOYKEY_START,			//START�{�^��
		JOYKEY_BACK,			//BACK�{�^��
		JOYKEY_LEFTSTICK,		//���̃X�e�B�b�N��������
		JOYKEY_RIGHTSTICK,		//�E�̃X�e�B�b�N��������
		JOYKEY_LB,				//LB�{�^��
		JOYKEY_RB,				//RB�{�^��
		JOYKEY_PPP1,			//�s��
		JOYKEY_PPP2,			//�s��
		JOYKEY_A,				//A�{�^��
		JOYKEY_B,				//B�{�^��
		JOYKEY_X,				//X�{�^��
		JOYKEY_Y,				//Y�{�^��
		JOYKEY_STICKLX,			//L�X�e�B�b�N(X��)(�E�ɍs���قǒl�͑傫��)
		JOYKEY_STICKLY,			//L�X�e�B�b�N(Y��)(��ɍs���قǒl�͑傫��)
		JOYKEY_STICKRX,			//L�X�e�B�b�N(X��)(�E�ɍs���قǒl�͑傫��)
		JOYKEY_STICKRY,			//L�X�e�B�b�N(Y��)(��ɍs���قǒl�͑傫��)
		JOYKEY_MAX
	};

	// �\���̒�`(�o�C�u���[�V�����֌W)
	struct SVibrate
	{
		XINPUT_VIBRATION vibration;		// �o�C�u���[�V�����̏��
		int nCountRight;				// �E�̐k����J�E���g
		int nCountLeft;					// ���̐k����J�E���g
	};

	CInputGamePad();		// �R���X�g���N�^
	~CInputGamePad();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance);				// ����������
	void Uninit(void) override;						// �I������
	void Update(void) override;						// �X�V����
	bool GetPress(JOYKEY nKey, int nPlayer);		// �v���X����
	bool GetTrigger(JOYKEY nKey, int nPlayer);		// �g���K�[��񏈗�
	bool GetRelease(JOYKEY nKey, int nPlayer);		// �����[�X��񏈗�
	bool GetRepeat(JOYKEY nKey, int nPlayer, int nCount);	// ���s�[�g����
	void GetRightVibration(int nPlayer, const WORD strength, const int nCount);		// �E�̃o�C�u���[�V��������
	void GetLeftVibration(int nPlayer, const WORD strength, const int nCount);		// ���̃o�C�u���[�V��������
	bool GetConnect(void);							// �ڑ�����̎擾����

	// ���X�e�B�b�N�֌W
	SHORT GetGameStickLXPress(int nPlayer);			// L�X�e�B�b�N(X��)���͂̃v���X��񏈗�
	SHORT GetGameStickLYPress(int nPlayer);			// L�X�e�B�b�N(Y��)���͂̃v���X��񏈗�

	// �E�X�e�B�b�N�֌W
	SHORT GetGameStickRXPress(int nPlayer);			// R�X�e�B�b�N(X��)���͂̃v���X��񏈗�
	SHORT GetGameStickRYPress(int nPlayer);			// R�X�e�B�b�N(Y��)���͂̃v���X��񏈗�

	// �ÓI�����o�֐�
	static CInputGamePad* Create(HINSTANCE hInstance);		// ��������

private:		// �N�ł��A�N�Z�X�ł���

	// �����o�ϐ�
	XINPUT_STATE m_aPadState[MAX_PLAYER];			// �v���X���
	XINPUT_STATE m_aPadStateTrigger[MAX_PLAYER];	// �g���K�[���
	XINPUT_STATE m_aPadStateRelease[MAX_PLAYER];	// �����[�X���
	int m_nRepeatCount[MAX_PLAYER][JOYKEY_MAX];		// ���s�[�g�J�E���g
	SVibrate m_aVibration[MAX_PLAYER];				// �o�C�u���[�V�����̏��
	bool m_bConnect;								// �ڑ�����
};

//---------------------------------------
// �}�E�X�N���X
//---------------------------------------
class CInputMouse : public CInput
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`
	typedef enum
	{
		MOUSE_L = 0,	// ���N���b�N
		MOUSE_R,		// �E�N���b�N
		MOUSE_WHEEL,	// �}�E�X�z�C�[��
		MOUSE_CYCLE,	// �}�E�X�z�C�[���̉�]
		MOUSE_MAX		// ���̗񋓌^�̑���
	}MOUSE_BUTTON;

	CInputMouse();		// �R���X�g���N�^
	~CInputMouse();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ����������
	void Uninit(void) override;						// �I������
	void Update(void) override;						// �X�V����

	// ���͊֌W
	bool GetPress(const MOUSE_BUTTON button);		// �N���b�N�̃v���X����
	bool GetTrigger(const MOUSE_BUTTON button);		// �N���b�N�̃g���K�[����
	bool GetRelease(const MOUSE_BUTTON button);		// �N���b�N�̃����[�X��񏈗�
	D3DXVECTOR3 GetMove(void);						// �}�E�X�J�[�\���̈ړ��ʎ擾����
	POINT GetPos(void);								// �}�E�X�J�[�\���̈ʒu�ݒ菈��

	// �ÓI�����o�֐�
	static CInputMouse* Create(HINSTANCE hInstance, HWND hWnd);		// ��������

private:		// �N�ł��A�N�Z�X�ł���

	// �����o�ϐ�
	DIMOUSESTATE m_MouseState;			// �v���X(�h���b�O)�̏��
	DIMOUSESTATE m_MouseStateTrigger;	// �g���K�[�̏��
	DIMOUSESTATE m_MouseStateRelease;	// �����[�X�̏��
	POINT m_MousePos;					// �}�E�X�̈ʒu
	D3DXVECTOR3 m_MouseMove;			// �}�E�X�̈ړ���
};

#endif