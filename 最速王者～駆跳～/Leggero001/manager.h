//============================================
//
// �}�l�[�W���[�̃w�b�_�[[manager.h]
// Author�F��������
//
//============================================
#ifndef _MANAGER_H_			//���̃}�N����`������Ă��Ȃ�������
#define _MANAGER_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "main.h"
#include "scene.h"
#include "camera.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CRenderer;		// �����_���[
class CInputKeyboard;	// �L�[�{�[�h����
class CInputGamePad;	// �Q�[���p�b�h
class CSound;			// �T�E���h
class CFile;			// �t�@�C��
class CDebugProc;		// �f�o�b�O�\��
class CCamera;			// �J����
class CLight;			// ���C�g
class CEditAll;			// �G�f�B�b�g�I�[��
class CTexture;			// �e�N�X�`��
class CFade;			// �t�F�[�h

//--------------------------------------------
// �N���X(�}�l�[�W���[�N���X)
//--------------------------------------------
class CManager
{
public:						// �N�ł��A�N�Z�X�ł���

	CManager();				// �R���X�g���N�^
	~CManager();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	HRESULT TexLoad(void);						// �e�N�X�`���̓ǂݍ��ݏ���
	void TexUnload(void);						// �e�N�X�`���̔j������

	// �ÓI�����o�֐�(�Z�b�g�E�Q�b�g)
	static CRenderer* GetRenderer(void);			// �����_���[�̎擾����
	static CInputKeyboard* GetInputKeyboard(void);	// �L�[�{�[�h�̎擾����
	static CInputGamePad* GetInputGamePad(void);	// �Q�[���p�b�h�̎擾����
	static CSound* GetSound(void);					// �T�E���h�̎擾����
	static CFile* GetFile(void);					// �t�@�C���̎擾����
	static CDebugProc* GetDebugProc(void);			// �f�o�b�O�\���̎擾����
	static CCamera* GetCamera(void);				// �J�����\���̎擾����
	static CLight* GetLight(void);					// ���C�g�̎擾����
	static CTexture* GetTexture(void);				// �e�N�X�`���̎擾����
	static CFade* GetFade(void);					// �t�F�[�h�̎擾����

	static void SetClearTime(const DWORD nTime);	// �N���A�^�C���̐ݒ菈��
	static DWORD GetClearTime(void);				// �N���A�^�C���̎擾����

	static void SetMode(const CScene::MODE mode);	// ���[�h�̐ݒ菈��
	static CScene::MODE GetMode(void);				// ���[�h�̎擾����

	static void SetScore(const int nScore);			// �X�R�A�̐ݒ菈��
	static int GetScore(void);						// �X�R�A�̎擾����

	static void SetClear(const bool bClear);		// �N���A�󋵂̐ݒ菈��
	static bool GetClear(void);						// �N���A�󋵂̎擾����

private:					// ���������A�N�Z�X�ł���

	// �ÓI�����o�ϐ�(�ŏ�����K�v�ȕ���z�u���Ă���)
	static CRenderer* m_pRenderer;					// �����_���[�̏��
	static CInputKeyboard* m_pInputKeyboard;		// �L�[�{�[�h�̏��
	static CInputGamePad* m_pInputGamePad;			// �Q�[���p�b�h�̏��
	static CSound* m_pSound;						// �T�E���h�̏��
	static CFile* m_pFile;							// �t�@�C���̏��
	static CDebugProc* m_pDebugProc;				// �f�o�b�O�\���̏��
	static CCamera* m_pCamera;						// �J�����̏��
	static CLight* m_pLight;						// ���C�g�̏��
	static CTexture* m_pTexture;					// �e�N�X�`���̏��
	static CScene* m_pScene;						// �V�[���̏��
	static CFade* m_pFade;							// �t�F�[�h�̏��
	static DWORD m_nClearTime;						// �N���A�^�C��
	static int m_nActionScore;						// �A�N�V�����X�R�A
	static bool m_bClear;							// �N���A��

#ifdef _DEBUG		// �f�o�b�O���̂�

	static bool m_bWire;						// ���C���[�t���[����

#endif
};

#endif