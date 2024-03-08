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
class CGameScore;		// �Q�[���X�R�A
class CAlter;			// �Ւd

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
		STATE_BOSSMOVIE,	// �{�X�o�����
		STATE_GAMEOVER,		// �Q�[���I�[�o�[���
		STATE_CONTINUE,		// �R���e�j���[���
		STATE_FINISH,		// �I�����
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

	static CPlayer* GetPlayer(void);			// �v���C���[�̎擾����

	static CGameScore* GetGameScore(void);		// �Q�[���X�R�A�̎擾����

	static CAlter* GetAlter(void);				// �Ւd�̎擾����

	static int GetScore(void);					// �����X�R�A�̎擾����

	// NULL������
	static void DeletePause(void);		// �|�[�Y��NULL������
	static void DeletePlayer(void);		// �v���C���[��NULL������
	static void DeleteGameScore(void);	// �X�R�A��NULL������
	static void DeleteAlter(void);		// �Ւd��NULL������

private:					// ���������A�N�Z�X�ł���

	// �����o�֐�
	void GameOver(void);		// �Q�[���I�[�o�[����
	void Pause(void);			// �|�[�Y����
	void Transition(void);		// �J�ڏ���

	// �����o�ϐ�
	int m_nStateCount;			// �I���J�E���g

	// �ÓI�����o�ϐ�
	static CPause* m_pPause;			// �|�[�Y�̏��
	static CPlayer* m_pPlayer;			// �v���C���[�̏��
	static CGameScore* m_pGameScore;	// �Q�[���X�R�A�̏��
	static CAlter* m_pAlter;			// �Ւd�̏��
	static STATE m_state;				// �Q�[���̐i�s���
	static int m_nScore;				// �X�R�A
	static bool m_bPause;				// �|�[�Y��
};

#endif