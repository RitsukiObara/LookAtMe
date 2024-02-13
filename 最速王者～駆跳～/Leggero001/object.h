//============================================
//
// �I�u�W�F�N�g�w�b�_�[[object.h]
// Author�F��������
//
//============================================
#ifndef _OBJECT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _OBJECT_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "main.h"

//--------------------------------------------
// �N���X(�I�u�W�F�N�g�N���X)
//--------------------------------------------
class CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	// ���
	typedef enum
	{
		TYPE_NONE = 0,			// ����
		TYPE_PLAYER,			// �v���C���[
		TYPE_BG,				// �w�i
		TYPE_BULLET,			// �e
		TYPE_ENEMY,				// �G
		TYPE_DEATHENEMY,		// ���񂾓G
		TYPE_EXPLOSION,			// ����
		TYPE_EFFECT,			// �G�t�F�N�g
		TYPE_PARTICLE,			// �p�[�e�B�N��
		TYPE_TIME,				// ����
		TYPE_SCORE,				// �X�R�A
		TYPE_BLOCK,				// �u���b�N
		TYPE_ITEM,				// �A�C�e��
		TYPE_EDIT,				// �G�f�B�b�g
		TYPE_3DPOLIGON,			// 3D�|���S��
		TYPE_BILLBOARD,			// �r���{�[�h
		TYPE_FIELD,				// �t�B�[���h
		TYPE_SHADOW,			// �e
		TYPE_BGOBJECT,			// �w�i�̃I�u�W�F�N�g
		TYPE_STAMINAUI,			// �X�^�~�iUI
		TYPE_LANDUI,			// ���nUI
		TYPE_OBSTACLE,			// ��Q��
		TYPE_COLLPOLY,			// �����蔻��|���S��
		TYPE_FRACTION,			// �j��
		TYPE_GOAL,				// �S�[��
		TYPE_GOALUI,			// �S�[��UI
		TYPE_CLIFFCHECKER,		// �R�߂܂�`�F�b�J�[
		TYPE_LANDRIPPLE,		// ���n�̔g��
		TYPE_ORBIT,				// �O��
		TYPE_ACTIONUI,			// �A�N�V����UI
		TYPE_LOGOMARK,			// ���S�}�[�N
		TYPE_LOCUS,				// �c��
		TYPE_SPEEDUI,			// ���xUI
		TYPE_RESULTRANK,		// ���U���g�̃����N
		TYPE_PAUSE,				// �|�[�Y���j���[
		TYPE_ITEMGENERATOR,		// �A�C�e��������
		TYPE_RANKINGSCORE,		// �����L���O�̋L�^
		TYPE_RANKINGPRECE,		// �����L���O�̏��ʕ\��
		TYPE_RANKINGSCOREDISP,	// �����L���O��UI
		TYPE_NEWRECORD,			// �V�L�^
		TYPE_CAMERARANGE,		// �J�����͈̔�
		TYPE_RANGEMOVE,			// �͈͈ړ�
		TYPE_AURA,				// �I�[��
		TYPE_ANNOUNCE,			// �c�苗���̍��m
		TYPE_PRAISEUI,			// �J�ߌ��t
		TYPE_COUNTDOWN,			// �J�E���g�_�E��
		TYPE_BALLOON,			// �����o��
		TYPE_SKIP,				// �X�L�b�v�̉~
		TYPE_BALLOONUI,			// �����o����UI
		TYPE_PUSHTIMING,		// �^�C�~���O�\��
		TYPE_MESH,				// ���b�V��
		TYPE_SKYBOX,			// �X�J�C�{�b�N�X
		TYPE_SILHOUETTE,		// �V���G�b�g
		TYPE_ARROWSIGN,			// ���Ŕ�
		TYPE_ENTERSKIP,			// ENTER�L�[�ŃX�L�b�v����\��
		TYPE_RUBSMOKE,			// �C�艌
		TYPE_MAX				// ���̗񋓌^�̑���
	}TYPE;

	// �D�揇��
	typedef enum
	{
		PRIORITY_BG = 0,			// �w�i�֌W
		PRIORITY_BLOCK,				// �u���b�N�E��Q���n
		PRIORITY_EFFECT,			// �G�t�F�N�g�n
		PRIORITY_SHADOW,			// �e�֌W
		PRIORITY_ENTITY,			// �G�E�A�C�e���֌W
		PRIORITY_PLAYER,			// �v���C���[�֌W
		PRIORITY_UI,				// UI�֌W
		PRIORITY_PAUSE,				// �|�[�Y(��ΑO�ɏo�����)
		PRIORITY_MAX				// ���̗񋓌^�̑���
	}PRIORITY;

	CObject();													// �R���X�g���N�^
	CObject(TYPE type, PRIORITY priority = PRIORITY_ENTITY);	// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CObject();											// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) = 0;	// ����������
	virtual void Uninit(void) = 0;	// �I������
	virtual void Update(void) = 0;	// �X�V����
	virtual void Draw(void) = 0;	// �`�揈��

	// �Z�b�g�E�Q�b�g�֌W
	static CObject* GetTop(PRIORITY priority);		// �I�u�W�F�N�g�̎擾����

	CObject* GetNext(void);				// ���̃|�C���^�̎擾����

	void SetType(const TYPE type);		// ��ނ̐ݒ菈��
	TYPE GetType(void) const;			// ��ނ̎擾����

	// �ÓI�����o�֐�
	static void ReleaseAll(void);		// �S�Ă̔j������
	static void UpdateAll(void);		// �S�Ă̍X�V����
	static void DrawAll(void);			// �S�Ă̕`�揈��

	static void AnyAllClear(const TYPE type);		// �ėp�I�ȑS��������
	static void AnyAllClear(const PRIORITY priority, const TYPE type);		// �ėp�I�ȑS��������
	static void DeathDecision(const int nCnt);		// ���S���ʏ���

	static void UpdateNormal(void);		// �ʏ�X�V����
	static void UpdateGame(void);		// �Q�[���̍X�V����

	static void DrawNormal(void);		// �ʏ�`�揈��
	static void DrawGame(void);			// �Q�[���̕`�揈��

protected:		// �����Ɣh���N���X�������A�N�Z�X�ł���

	// �����o�֐�
	void Release(void);					// �j������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Death(void);					// ���S����

	// �����o�ϐ�
	CObject* m_pPrev;					// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNext;					// ���̃I�u�W�F�N�g�ւ̃|�C���^
	TYPE m_type;						// ���
	PRIORITY m_priority;				// �I�u�W�F�N�g�̗D�揇��
	bool m_bDeath;						// ���S�t���O
	
	// �ÓI�����o�ϐ�
	static CObject* m_apTop[PRIORITY_MAX];				// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject* m_apCur[PRIORITY_MAX];				// �Ō���I�u�W�F�N�g�ւ̃|�C���^
	static int m_nNumAll;								// �I�u�W�F�N�g�̑���
};

#endif