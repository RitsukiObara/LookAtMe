//============================================
//
// �Q�[���w�b�_�[[game.h]
// Author�F��������
//
//============================================
#ifndef _GAME_H_			//���̃}�N����`������Ă��Ȃ�������
#define _GAME_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "scene.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CPause;			// �|�[�Y
class CPlayer;			// �v���C���[
class CEdit;			// �G�f�B�b�g

//--------------------------------------------
// �N���X(�Q�[���N���X)
//--------------------------------------------
class CGame : public CScene
{
public:						// �N�ł��A�N�Z�X�ł���

	// �Q�[���̐i�s���
	enum STATE
	{
		STATE_START = 0,	// �J�n���
		STATE_PLAY,			// �v���C���
		STATE_GOAL,			// �S�[�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CGame();				// �R���X�g���N�^
	~CGame();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const MODE mode);		// ���̐ݒ菈��

	// �ÓI�����o�֐�(�Z�b�g�E�Q�b�g)
	static void SetEnablePause(const bool bPause);	// �|�[�Y�󋵂̐ݒ菈��
	static bool IsPause(void);						// �|�[�Y�󋵂̎擾����

	static void SetState(const STATE state);		// �Q�[���̐i�s��Ԃ̐ݒ菈��
	static STATE GetState(void);					// �Q�[���̐i�s��Ԃ̎擾����

	static CPlayer* GetPlayer(void);	// �v���C���[�̎擾����

	static CEdit* GetEdit(void);		// �G�f�B�b�g�̎擾����

	// NULL������
	static void DeletePause(void);		// �|�[�Y��NULL������
	static void DeletePlayer(void);		// �v���C���[��NULL������
	static void DeleteEdit(void);		// �G�f�B�b�g��NULL������

private:					// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Pause(void);			// �|�[�Y����
	void Transition(void);		// �J�ڏ���

	// �ÓI�����o�ϐ�(�ŏ�����K�v�ȕ���z�u���Ă���)
	static CPause* m_pPause;			// �|�[�Y�̏��
	static CPlayer* m_pPlayer;			// �v���C���[�̏��
	static CEdit* m_pEdit;				// �G�f�B�b�g�̏��
	static STATE m_GameState;			// �Q�[���̐i�s���
	static int m_nFinishCount;			// �I���J�E���g
	static bool m_bPause;				// �|�[�Y��
};

#endif