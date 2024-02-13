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
class CScore;			// �X�R�A
class CEditAll;			// �G�f�B�b�g�I�[��
class CGameTime;		// �Q�[���^�C��
class CPause;			// �|�[�Y

//--------------------------------------------
// �N���X(�Q�[���N���X)
//--------------------------------------------
class CGame : public CScene
{
public:						// �N�ł��A�N�Z�X�ł���

	// �Q�[���̐i�s���
	typedef enum
	{
		STATE_START = 0,	// �J�n���
		STATE_COUNTDOWN,	// �J�E���g�_�E��
		STATE_PLAY,			// �v���C���
		STATE_GOAL,			// �S�[�����
		STATE_FINISH,		// �I�����
		STATE_HITSTOP,		// �q�b�g�X�g�b�v���
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	CGame();				// �R���X�g���N�^
	~CGame();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const MODE mode);		// ���̐ݒ菈��

	// �ÓI�����o�֐�(�Z�b�g�E�Q�b�g)
	static CScore* GetScore(void);					// �X�R�A�̎擾����
	static CGameTime* GetTime(void);				// �Q�[���^�C���̎擾����
	static CPause* GetPause(void);					// �|�[�Y�̎擾����

	static void SetState(const STATE state);		// �Q�[���̐i�s��Ԃ̐ݒ菈��
	static STATE GetState(void);					// �Q�[���̐i�s��Ԃ̎擾����

	static void SetHitStop(const int nCount);		// �Q�[���̃q�b�g�X�g�b�v�̐ݒ菈��

	// NULL������
	static void DeleteScore(void);					// �X�R�A��NULL������
	static void DeleteTime(void);					// �^�C����NULL������
	static void DeletePause(void);					// �|�[�Y��NULL������

#ifdef _DEBUG		// �f�o�b�O���̂�

	static bool GetEdit(void);						// �G�f�B�b�g���[�h�̎擾����
	static CEditAll* GetEditAll(void);				// �G�f�B�b�g�I�[���̎擾����
#endif

private:					// ���������A�N�Z�X�ł���

	// �����o�֐�
	void GameUpdate(void);		// �Q�[�����̍X�V����
	void HitStop(void);			// �q�b�g�X�g�b�v����
	void Pause(void);			// �|�[�Y����
	void Transition(void);		// �J�ڏ���

	// �ÓI�����o�ϐ�(�ŏ�����K�v�ȕ���z�u���Ă���)
	static CScore* m_pScore;					// �X�R�A�̏��
	static CGameTime* m_pTime;					// �Q�[���^�C���̏��
	static CPause* m_pPause;					// �|�[�Y�̏��
	static STATE m_GameState;					// �Q�[���̐i�s���
	static int m_nHitStopCount;					// �q�b�g�X�g�b�v�J�E���g
	static int m_nFinishCount;					// �I���J�E���g
	static bool m_bStartSkip;					// �X�^�[�g���o�̃X�L�b�v

#ifdef _DEBUG		// �f�o�b�O���̂�

	// �����o�֐�
	void EditChange(void);						// �G�f�B�b�g�̐؂�ւ�����
	void StageSave(void);						// �X�e�[�W�Z�[�u����
	void EditUpdate(void);						// �G�f�B�b�g�̃A�b�v�f�[�g����

	// �ÓI�����o�ϐ�
	static bool m_bEdit;						// �G�f�B�b�g�̏��
	static CEditAll* m_pEditAll;				// �G�f�B�b�g�I�[���̏��

#endif
};

#endif