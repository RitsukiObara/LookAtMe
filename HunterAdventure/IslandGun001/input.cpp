//=========================================
//
//���͂̃��C������[input.cpp]
//Author ��������
//
//=========================================
#include "manager.h"
#include "debugproc.h"
#include "input.h"

//-----------------------------------------
// �ÓI�����o�ϐ��錾
//-----------------------------------------
LPDIRECTINPUT8 CInput::m_pInput = nullptr;				// DirectInput�I�u�W�F�N�g�ւ̃|�C���^

//======================================================��������CInput�̏���======================================================

//==================================
// �R���X�g���N�^
//==================================
CInput::CInput()
{
	// �S�Ă̒l���N���A����
	m_pDevice = nullptr;								// �f�o�C�X�ւ̃|�C���^
}

//==================================
// �f�X�g���N�^
//==================================
CInput::~CInput()
{

}

//==================================
// ����������
//==================================
HRESULT CInput::Init(HINSTANCE hInstance)
{
	if (m_pInput == nullptr)
	{ // �I�u�W�F�N�g�ւ̃|�C���^�� NULL �̏ꍇ

		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{ // �I�u�W�F�N�g�̐����Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "DirectInput�I�u�W�F�N�g�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// ��~
			assert(false);

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//==================================
// �I������
//==================================
void CInput::Uninit(void)
{
	//���̓f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();					//�L�[�{�[�h�ւ̃A�N�Z�X�������

		m_pDevice->Release();

		m_pDevice = nullptr;
	}

	//DirectInput�I�u�W�F�N�g�̕���
	if (m_pInput != nullptr)
	{
		m_pInput->Release();

		m_pInput = nullptr;
	}
}

//======================================================��������CInputKeyboard�̏���======================================================

//==================================
// �R���X�g���N�^
//==================================
CInputKeyboard::CInputKeyboard()
{
	// �S�Ă̒l���N���A����
	ZeroMemory(m_aKeyState, sizeof(m_aKeyState));		// �L�[���
}

//==================================
// �f�X�g���N�^
//==================================
CInputKeyboard::~CInputKeyboard()
{

}

//==================================
// ����������
//==================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// CInput�̏���������
	CInput::Init(hInstance);

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{ // �����Ɏ��s�����ꍇ

		// �x����
		MessageBox(NULL, "�L�[�{�[�h�̓��̓f�o�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// �x����
		MessageBox(NULL, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// �x����
		MessageBox(NULL, "�L�[�{�[�h�̋������[�h�ݒ�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	// ������Ԃ�
	return S_OK;
}

//==================================
// �I������
//==================================
void CInputKeyboard::Uninit(void)
{
	// CInput�̏I������
	CInput::Uninit();

	// ���������J������
	delete this;
}

//==================================
// �X�V����
//==================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];						//�L�[�{�[�h�̓��͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{ // �擾�ɐ��������ꍇ

		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//�L�[�{�[�h�̃g���K�[����ۑ�

			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];	//�L�[�{�[�h�̃����[�X����ۑ�

			m_aKeyState[nCntKey] = aKeyState[nCntKey];			//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{ // �擾�Ɏ��s�����ꍇ

		//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}
}

//======================================
// �v���X�����l��
//======================================
bool CInputKeyboard::GetPress(int nKey)
{
	// �v���X�󋵂�Ԃ�
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//======================================
// �g���K�[����
//======================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	// �g���K�[�󋵂�Ԃ�
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//======================================
// �����[�X����
//======================================
bool CInputKeyboard::GetRelease(int nKey)
{
	// �����[�X�󋵂�Ԃ�
	return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//======================================
// ���s�[�g����
//======================================
bool CInputKeyboard::GetRepeat(int nKey, int nCount)
{
	if (CInputKeyboard::GetPress(nKey) == true)
	{ // �L�[�������Ă���ꍇ

		// �J�E���g�����Z����
		m_nRepeatCount[nKey]++;
	}
	else
	{ // �L�[�������Ă��Ȃ��ꍇ

		// �J�E���g�����Z����
		m_nRepeatCount[nKey] = 0;
	}

	if (m_nRepeatCount[nKey] >= nCount)
	{ // ���s�[�g�J�E���g���J�E���g���𒴂����ꍇ

		// �J�E���g��0�ɂ���
		m_nRepeatCount[nKey] = 0;

		// true ��Ԃ�
		return true;
	}
	else
	{ // ��L�ȊO

		// false ��Ԃ�
		return false;
	}
}

//======================================
// ��������
//======================================
CInputKeyboard* CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CInputKeyboard* pInput = nullptr;		// ���͂̃C���X�^���X�𐶐�

	if (pInput == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pInput = new CInputKeyboard;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pInput != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pInput->Init(hInstance, hWnd)))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���͂̃|�C���^��Ԃ�
	return pInput;
}

//======================================================��������CInputGamePad�̏���======================================================

//==================================
// �R���X�g���N�^
//==================================
CInputGamePad::CInputGamePad()
{
	// �S�Ă̒l���N���A����
	ZeroMemory(m_aPadState, sizeof(m_aPadState));					// �v���X���
	ZeroMemory(m_aPadStateTrigger, sizeof(m_aPadStateTrigger));		// �g���K�[���
	ZeroMemory(m_aPadStateRelease, sizeof(m_aPadStateRelease));		// �����[�X���
	ZeroMemory(m_nRepeatCount, sizeof(m_nRepeatCount));				// ���s�[�g�J�E���g
	ZeroMemory(m_aVibration, sizeof(m_aVibration));					// �o�C�u���[�V�������
	m_bConnect = false;												// �ڑ�����
}

//==================================
// �f�X�g���N�^
//==================================
CInputGamePad::~CInputGamePad()
{

}

//==================================
// ����������
//==================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance)
{
	// CInput�̏���������
	CInput::Init(hInstance);

	// �Q�[���p�b�h�ݒ��L���ɂ���
	XInputEnable(true);

	// �Q�[���p�b�h�̃v���X����p�ӂ���
	memset(&m_aPadState[0], 0, sizeof(m_aPadState));

	// �Q�[���p�b�h�̃g���K�[����p�ӂ���
	memset(&m_aPadStateTrigger[0], 0, sizeof(m_aPadStateTrigger));

	// �Q�[���p�b�h�̃����[�X����p�ӂ���
	memset(&m_aPadStateRelease[0], 0, sizeof(m_aPadStateRelease));

	// ���s�[�g�J�E���g��p�ӂ���
	memset(m_nRepeatCount, 0, sizeof(m_nRepeatCount));

	// �o�C�u���[�V��������p�ӂ���
	memset(&m_aVibration[0], 0, sizeof(m_aVibration));

	// �ڑ����������������
	m_bConnect = false;

	// ���s��Ԃ�
	return S_OK;
}

//==================================
// �I������
//==================================
void CInputGamePad::Uninit(void)
{
	// CInput�̏I������
	CInput::Uninit();

	// �ڑ������ false �ɂ���
	m_bConnect = false;

	//�Q�[���p�b�h�ݒ�𖳌��ɂ���
	XInputEnable(false);

	// ���������J������
	delete this;
}

//==================================
// �X�V����
//==================================
void CInputGamePad::Update(void)
{
	XINPUT_STATE aGamePadState[MAX_PLAYER];			// �Q�[���p�b�h�̓��͏��
	bool bConnect = false;							// �ڑ�����Ă��邩�ǂ���

	for (int nCntGP = 0; nCntGP < MAX_PLAYER; nCntGP++)
	{//���ꂼ��̃Q�[���p�b�h���ݒ肷��

		//���̓f�o�C�X����f�[�^���擾
		if (XInputGetState(nCntGP, &aGamePadState[nCntGP]) == ERROR_SUCCESS)
		{
			//�Q�[���p�b�h�̃g���K�[����ۑ�
			m_aPadStateTrigger[nCntGP].Gamepad.wButtons = ~m_aPadState[nCntGP].Gamepad.wButtons & aGamePadState[nCntGP].Gamepad.wButtons;

			//�Q�[���p�b�h�̃����[�X����ۑ�
			m_aPadStateRelease[nCntGP].Gamepad.wButtons = (m_aPadState[nCntGP].Gamepad.wButtons ^ aGamePadState[nCntGP].Gamepad.wButtons) & ~aGamePadState[nCntGP].Gamepad.wButtons;

			//�Q�[���p�b�h�̃v���X����ۑ�
			m_aPadState[nCntGP] = aGamePadState[nCntGP];

			// �ڑ�����Ă���
			bConnect = true;

			if (m_aVibration[nCntGP].nCountRight > 0)
			{ // �E�̃J�E���g��0���߂̏ꍇ

				// �J�E���g�����Z����
				m_aVibration[nCntGP].nCountRight--;
			}
			else
			{ // ��L�ȊO
				
				// �J�E���g��0�ɂ���
				m_aVibration[nCntGP].nCountRight = 0;

				// �E�̃o�C�u���[�V�����̋�����0�ɂ���
				m_aVibration[nCntGP].vibration.wRightMotorSpeed = 0;
			}

			if (m_aVibration[nCntGP].nCountLeft > 0)
			{ // ���̃J�E���g��0���߂̏ꍇ

				// �J�E���g�����Z����
				m_aVibration[nCntGP].nCountLeft--;
			}
			else
			{ // ��L�ȊO
				
				// �J�E���g��0�ɂ���
				m_aVibration[nCntGP].nCountLeft = 0;

				// ���̃o�C�u���[�V�����̋�����0�ɂ���
				m_aVibration[nCntGP].vibration.wLeftMotorSpeed = 0;
			}

			// �o�C�u���[�V�����̏���n��
			XInputSetState(nCntGP, &m_aVibration[nCntGP].vibration);
		}
	}

	// �ڑ������������
	m_bConnect = bConnect;
}

//======================================
// �v���X�����l��
//======================================
bool CInputGamePad::GetPress(JOYKEY nKey, int nPlayer)
{
	return(m_aPadState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
// �g���K�[��񏈗�
//======================================
bool CInputGamePad::GetTrigger(JOYKEY nKey, int nPlayer)
{
	return(m_aPadStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
// �����[�X��񏈗�
//======================================
bool CInputGamePad::GetRelease(JOYKEY nKey, int nPlayer)
{
	return(m_aPadStateRelease[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
// ���s�[�g����
//======================================
bool CInputGamePad::GetRepeat(JOYKEY nKey, int nPlayer, int nCount)
{
	if (CInputGamePad::GetPress(nKey, nPlayer) == true)
	{ // �L�[�������Ă���ꍇ

		// �J�E���g�����Z����
		m_nRepeatCount[nPlayer][nKey]++;
	}
	else
	{ // �L�[�������Ă��Ȃ��ꍇ

		// �J�E���g�����Z����
		m_nRepeatCount[nPlayer][nKey] = 0;
	}

	if (m_nRepeatCount[nPlayer][nKey] >= nCount)
	{ // ���s�[�g�J�E���g���J�E���g���𒴂����ꍇ

		// �J�E���g��0�ɂ���
		m_nRepeatCount[nPlayer][nKey] = 0;

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

//======================================
// �E�̃o�C�u���[�V��������
//======================================
void CInputGamePad::GetRightVibration(int nPlayer, const WORD strength, const int nCount)
{
	// �E�̃o�C�u���[�V������ ON �ɂ���
	m_aVibration[nPlayer].vibration.wRightMotorSpeed = strength;

	// �J�E���g��ݒ肷��
	m_aVibration[nPlayer].nCountRight = nCount;

	// �o�C�u���[�V�����̏���n��
	XInputSetState(nPlayer, &m_aVibration[nPlayer].vibration);
}

//======================================
// ���̃o�C�u���[�V��������
//======================================
void CInputGamePad::GetLeftVibration(int nPlayer, const WORD strength, const int nCount)
{
	// �E�̃o�C�u���[�V������ ON �ɂ���
	m_aVibration[nPlayer].vibration.wLeftMotorSpeed = strength;

	// �J�E���g��ݒ肷��
	m_aVibration[nPlayer].nCountLeft = nCount;

	// �o�C�u���[�V�����̏���n��
	XInputSetState(nPlayer, &m_aVibration[nPlayer].vibration);
}

//======================================
// �ڑ�����Ă���ꍇ
//======================================
bool CInputGamePad::GetConnect(void)
{
	// �ڑ������Ԃ�
	return m_bConnect;
}

//======================================
//L�X�e�B�b�N(X��)���͂̃v���X��񏈗�
//======================================
SHORT CInputGamePad::GetGameStickLXPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbLX;
}

//======================================
//L�X�e�B�b�N(Y��)���͂̃v���X��񏈗�
//======================================
SHORT CInputGamePad::GetGameStickLYPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbLY;
}

//======================================
//R�X�e�B�b�N(X��)���͂̃v���X��񏈗�
//======================================
SHORT CInputGamePad::GetGameStickRXPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbRX;
}

//======================================
//R�X�e�B�b�N(Y��)���͂̃v���X��񏈗�
//======================================
SHORT CInputGamePad::GetGameStickRYPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbRY;
}

//======================================
// ��������
//======================================
CInputGamePad* CInputGamePad::Create(HINSTANCE hInstance)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CInputGamePad* pInput = nullptr;		// ���͂̃C���X�^���X�𐶐�

	if (pInput == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pInput = new CInputGamePad;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pInput != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pInput->Init(hInstance)))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���͂̃|�C���^��Ԃ�
	return pInput;
}

//======================================================��������CInputMouse�̏���======================================================

//======================================
// �R���X�g���N�^
//======================================
CInputMouse::CInputMouse()
{
	// �S�Ă̒l���N���A����
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
	ZeroMemory(&m_MouseStateTrigger, sizeof(m_MouseStateTrigger));
	ZeroMemory(&m_MouseStateRelease, sizeof(m_MouseStateRelease));
	ZeroMemory(&m_MousePos, sizeof(m_MousePos));
	ZeroMemory(&m_MouseMove, sizeof(m_MouseMove));
}

//======================================
// �f�X�g���N�^
//======================================
CInputMouse::~CInputMouse()
{

}

//======================================
// ����������
//======================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// CInput�̏���������
	CInput::Init(hInstance);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{ // �����Ɏ��s�����ꍇ

		// �x����
		MessageBox(NULL, "�}�E�X�̓��̓f�o�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// �x����
		MessageBox(NULL, "�}�E�X�̃f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// �x����
		MessageBox(NULL, "�}�E�X�̋������[�h�ݒ�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��
	if (FAILED(m_pDevice->Acquire()))
	{ // �ݒ�Ɏ��s�����ꍇ

		// �x����
		MessageBox(NULL, "�}�E�X�̃A�N�Z�X���̎擾�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �|�[�����O�J�n
	if (FAILED(m_pDevice->Poll()))
	{ // �ݒ�Ɏ��s�����ꍇ

		// �x����
		MessageBox(NULL, "�}�E�X�̃|�[�����O�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));					// �}�E�X�̃v���X���
	ZeroMemory(&m_MouseStateTrigger, sizeof(m_MouseStateTrigger));		// �}�E�X�̃g���K�[���
	ZeroMemory(&m_MouseStateRelease, sizeof(m_MouseStateRelease));		// �}�E�X�̃����[�X���
	ZeroMemory(&m_MousePos, sizeof(m_MousePos));						// �}�E�X�̈ʒu
	ZeroMemory(&m_MouseMove, sizeof(m_MouseMove));						// �}�E�X�̈ړ���
	
	// ���s��Ԃ�
	return S_OK;
}

//======================================
// �I������
//======================================
void CInputMouse::Uninit(void)
{
	// �I������
	CInput::Uninit();

	// ���������J������
	delete this;
}

//======================================
// �X�V����
//======================================
void CInputMouse::Update(void)
{
	// ���[�J���ϐ��錾
	DIMOUSESTATE MouseState;			// �}�E�X�̓��͏��

	// �}�E�X�̏�Ԃ��擾����
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState)))
	{ // �擾�ɐ��������ꍇ

		for (int nCntButton = 0; nCntButton < MOUSE_MAX; nCntButton++)
		{
			// �}�E�X�̃g���K�[����ۑ�
			m_MouseStateTrigger.rgbButtons[nCntButton] = ~m_MouseState.rgbButtons[nCntButton] & MouseState.rgbButtons[nCntButton];

			//�Q�[���p�b�h�̃����[�X����ۑ�
			m_MouseStateRelease.rgbButtons[nCntButton] = (m_MouseState.rgbButtons[nCntButton] ^ MouseState.rgbButtons[nCntButton]) & ~MouseState.rgbButtons[nCntButton];

			// �}�E�X�̃v���X����ۑ�����
			m_MouseState.rgbButtons[nCntButton] = MouseState.rgbButtons[nCntButton];
		}

		// �}�E�X�J�[�\���̈ʒu�擾����
		GetCursorPos(&m_MousePos);

		// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
		ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &m_MousePos);

		// �}�E�X�̈ړ��ʂ�ݒ肷��
		m_MouseMove = D3DXVECTOR3((float)MouseState.lX, (float)MouseState.lY, (float)MouseState.lZ);

		//// �J�[�\����\�����Ȃ�
		//ShowCursor(FALSE);
	}
	else
	{ // ��L�ȊO

		// �A�N�Z�X�����擾����
		m_pDevice->Acquire();
	}
}

//======================================
// �N���b�N�̃v���X����
//======================================
bool CInputMouse::GetPress(const MOUSE_BUTTON button)
{
	return (m_MouseState.rgbButtons[button] & 0x80) ? true : false;
}

//======================================
// �N���b�N�̃g���K�[����
//======================================
bool CInputMouse::GetTrigger(const MOUSE_BUTTON button)
{
	return (m_MouseStateTrigger.rgbButtons[button] & 0x80) ? true : false;
}

//======================================
// �N���b�N�̃����[�X��񏈗�
//======================================
bool CInputMouse::GetRelease(const MOUSE_BUTTON button)
{
	return (m_MouseStateRelease.rgbButtons[button] & 0x80) ? true : false;
}

//======================================
// �}�E�X�J�[�\���̈ړ��ʎ擾����
//======================================
D3DXVECTOR3 CInputMouse::GetMove(void)
{
	// �ړ��ʂ�Ԃ�
	return m_MouseMove;
}

//======================================
// �}�E�X�J�[�\���̈ʒu�ݒ菈��
//======================================
POINT CInputMouse::GetPos(void)
{
	// �ʒu��Ԃ�
	return m_MousePos;
}

//======================================
// ��������
//======================================
CInputMouse* CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CInputMouse* pInput = nullptr;		// ���͂̃C���X�^���X�𐶐�

	if (pInput == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pInput = new CInputMouse;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pInput != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pInput->Init(hInstance, hWnd)))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���͂̃|�C���^��Ԃ�
	return pInput;
}