//=============================================================================
//
// �T�E���h���� [sound.h]
// Author ��������
//
//=============================================================================
#ifndef _SOUND_H_					//���̃}�N����`������Ă��Ȃ�������
#define _SOUND_H_					//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

// �N���X��`(CSound)
class CSound
{
public:			// �N�ł��A�N�Z�X�o����

	//-------------------------------------------------------------------------
	// �T�E���h�ꗗ
	//-------------------------------------------------------------------------
	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM_TITLE = 0,		// �^�C�g��BGM
		SOUND_LABEL_BGM_TUTORIAL,		// �`���[�g���A��BGM
		SOUND_LABEL_BGM_GAME,			// �Q�[��BGM
		SOUND_LABEL_BGM_RESULT,			// ���U���gBGM
		SOUND_LABEL_BGM_RANKING,		// �����L���OBGM
		SOUND_LABEL_SE_SELECT,			// �I����
		SOUND_LABEL_SE_DECIDE,			// ���艹
		SOUND_LABEL_SE_GUNSHOT,			// �e�̔��ˉ�
		SOUND_LABEL_SE_GUNRELOAD,		// �e�̃����[�h��
		SOUND_LABEL_SE_DAGGER,			// �_�K�[��
		SOUND_LABEL_SE_WALK,			// ���s��
		SOUND_LABEL_SE_AVOID,			// �����
		SOUND_LABEL_SE_BOSSHOWLING,		// �{�X�̗Y�����щ�
		SOUND_LABEL_SE_WINDSHOT,		// ���U��
		SOUND_LABEL_SE_FIRESHOT,		// ���U��
		SOUND_LABEL_SE_EXPLOSION,		// ����
		SOUND_LABEL_SE_COINGET,			// �R�C���Q�b�g
		SOUND_LABEL_SE_ROCKBREAK,		// ��̔j��
		SOUND_LABEL_SE_SIGNBOARD,		// �Ŕ�
		SOUND_LABEL_SE_DAMAGE,			// �_���[�W��
		SOUND_LABEL_SE_BOSS_EXPLOSION,	// �{�X�̔�����
		SOUND_LABEL_SE_LANDING,			// ���n��
		SOUND_LABEL_SE_JUMP,			// �W�����v��
		SOUND_LABEL_SE_BOSSCHARGE,		// �{�X�̃`���[�W��
		SOUND_LABEL_SE_PLAYERDAMAGE,	// �v���C���[�̃_���[�W��
		SOUND_LABEL_SE_WEAKDAMAGE,		// ��_�̃_���[�W��
		SOUND_LABEL_SE_WEAKBREAK,		// ��_�̔j��
		SOUND_LABEL_MAX					// ���̗񋓌^�̑���
	};

	//-----------------------------------------------------------------------------
	// �T�E���h���̍\���̒�`
	//-----------------------------------------------------------------------------
	struct SOUNDINFO
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	};

	CSound();			// �R���X�g���N�^
	~CSound();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(HWND hWnd);					// ����������
	void Uninit(void);							// �I������
	HRESULT Play(const SOUND_LABEL label);		// �Đ�����
	void Pause(const SOUND_LABEL label);		// �|�[�Y����
	void Replay(const SOUND_LABEL label);		// ���v���C����
	void Stop(const SOUND_LABEL label);			// ��~(���x���w��)
	void Stop(void);							// ��~(�S��~)

	// �Z�b�g�E�Q�b�g�֌W
	void SetVolume(const SOUND_LABEL label, const float fVolume);		// �{�����[���̐ݒ菈��
	float GetVolume(const SOUND_LABEL label) const;						// �{�����[���̎擾����
	void SetFrequency(const SOUND_LABEL label, const float fFrequency);	// ���g���̐ݒ菈��
	float GetFrequency(const SOUND_LABEL label) const;					// ���g���̎擾����

	// �ÓI�����o�֐�
	static CSound* Create(HWND hWnd);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// �����o�ϐ�
	IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
	static SOUNDINFO m_aSoundInfo[CSound::SOUND_LABEL_MAX];	// �T�E���h�̏��
};

#endif
