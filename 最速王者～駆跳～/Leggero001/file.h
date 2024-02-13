//============================================
//
// �t�@�C���w�b�_�[[file.h]
// Author�F��������
//
//============================================
#ifndef _FILE_H_			//���̃}�N����`������Ă��Ȃ�������
#define _FILE_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "main.h"
#include "block.h"
#include "obstacle.h"
#include "camera.h"
#include "result.h"
#include "ranking.h"
#include "balloon.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define MAX_FILE_DATA		(512)		// �t�@�C���̃f�[�^�̍ő吔

//--------------------------------------------
// �N���X(�t�@�C���ǂݍ��݃N���X)
//--------------------------------------------
class CFile
{
public:			// �N�ł��A�N�Z�X�ł���

	// �t�@�C���̎��
	typedef enum
	{
		TYPE_ENEMY = 0,			// �G�̏��
		TYPE_BLOCK,				// �u���b�N�̏��
		TYPE_BGOBJECT,			// �w�i�I�u�W�F�N�g
		TYPE_OBSTACLE,			// ��Q��
		TYPE_ITEM,				// �A�C�e��
		TYPE_CAMERA,			// �J����
		TYPE_RESULT,			// ���U���g
		TYPE_RANKING,			// �����L���O
		TYPE_GAME,				// �Q�[��
		TYPE_TUTORIAL,			// �`���[�g���A��
		TYPE_MAX				// ���̗񋓌^�̑���
	}TYPE;

	// �t�@�C���̎��
	typedef enum
	{
		SCENE_TYPE_GAME = 0,	// �Q�[�����
		SCENE_TYPE_TUTORIAL,	// �`���[�g���A�����
		SCENE_TYPE_MAX			// ���̗񋓌^�̑���
	}SCENE_TYPE;

	// �t�@�C���œǂݍ��ފ�{���
	typedef struct
	{
		D3DXVECTOR3 pos;					// �ʒu
		D3DXVECTOR3 rot;					// ����
		D3DXVECTOR3 scale;					// �g�嗦
	}SFileInfo;

	// �u���b�N�t�@�C���œǂݍ��ޏ��
	typedef struct
	{
		SFileInfo info;						// ��{���
		CBlock::TYPE type;					// ���
		D3DXMATERIAL aMat[MAX_MATERIAL];	// �}�e���A���f�[�^
		bool bCliffRight;					// �E�R�̔���
		bool bCliffLeft;					// ���R�̔���
	}SBlockInfo;

	// �u���b�N�t�@�C���̑������
	typedef struct
	{
		SBlockInfo info[MAX_FILE_DATA];		// ���ւ̃|�C���^
		int nNumData;						// �f�[�^�̐�
		bool bSuccess;						// ������
	}SBlockFile;

	// �w�i�t�@�C���œǂݍ��ޏ��
	typedef struct
	{
		SFileInfo info;						// ��{���
		CXFile::TYPE type;					// ���
		D3DXMATERIAL aMat[MAX_MATERIAL];	// �}�e���A���f�[�^
	}SBgObjectInfo;

	// �w�i�t�@�C���̑������
	typedef struct
	{
		SBgObjectInfo info[MAX_FILE_DATA];	// ���ւ̃|�C���^
		int nNumData;							// �f�[�^�̐�
		bool bSuccess;						// ������
	}SBgObjectFile;

	// ��Q���t�@�C���œǂݍ��ޏ��
	typedef struct
	{
		D3DXVECTOR3 pos;					// �ʒu
		CObstacle::TYPE type;				// ���
	}SObstacleInfo;

	// ��Q���t�@�C���̑������
	typedef struct
	{
		SObstacleInfo info[MAX_FILE_DATA];	// ���ւ̃|�C���^
		int nNumData;						// �f�[�^�̐�
		bool bSuccess;						// ������
	}SObstacleFile;

	// �A�C�e���t�@�C���̑������
	typedef struct
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];		// �ʒu
		int nNumItemData;					// �A�C�e���̃f�[�^�̐�
		int nNumItemGeneData;				// �A�C�e��������̃f�[�^�̐�
		bool bSuccess;						// ������
	}SItemFile;

	// �J�����t�@�C���œǂݍ��ޏ��
	typedef struct
	{
		D3DXVECTOR3 pos;					// �ʒu
		D3DXVECTOR3 size;					// �T�C�Y
		CCamera::TYPE type;					// ���
	}SCameraInfo;

	// �J�����t�@�C���̑������
	typedef struct
	{
		SCameraInfo info[MAX_FILE_DATA];	// �ʒu
		int nNumData;						// �f�[�^�̐�
		bool bSuccess;						// ������
	}SCameraFile;

	// ���U���g�̏��
	typedef struct
	{
		int aRankTime[CResult::RANK_MAX];	// ���Ԃ̃����N�̊
		int aRankScore[CResult::RANK_MAX];	// �X�R�A�̃����N�̊
		bool bSuccess;						// ������
	}SResultFile;

	// �����L���O�̏��
	typedef struct
	{
		int aRank[MAX_RANKING];				// �����L���O�̃f�[�^
		bool bSuccess;						// ������
	}SRankingFile;

	// �Q�[�����[�h�̏��
	typedef struct
	{
		D3DXVECTOR3 PlayerPos;				// �v���C���[�̏����ʒu
		D3DXVECTOR3 StartPos;				// �v���C���[�̃X�^�[�g�ʒu
		D3DXVECTOR3 GoalPos;				// �S�[���̈ʒu
		D3DXVECTOR3 StageLeftPos;			// �X�e�[�W�̍��[�̈ʒu
		D3DXVECTOR3 ScorePos;				// �X�R�A�̈ʒu
		D3DXVECTOR3 ScoreSize;				// �X�R�A�̃T�C�Y
		D3DXVECTOR3 TimePos;				// �^�C���̈ʒu
		D3DXVECTOR3 TimeSize;				// �^�C���̃T�C�Y
		bool bSuccess;						// ������
	}SGameFile;

	// �����o���̎�ނ��Ƃ̃f�[�^
	typedef struct
	{
		D3DXVECTOR3 size;					// �T�C�Y
		char aTextureName[MAX_STRING];		// �e�N�X�`����
	}SBalloonData;

	// �����o���̏��
	typedef struct
	{
		D3DXVECTOR3 pos;					// �ʒu
		CBalloon::TYPE type;				// ���
	}SBalloonInfo;

	// �����o���̑������
	typedef struct
	{
		SBalloonInfo info[MAX_FILE_DATA];	// ���ւ̃|�C���^
		int nNumData;						// �f�[�^�̐�
	}SBalloonFile;

	// �`���[�g���A���̏��
	typedef struct
	{
		SBalloonFile BalloonInfo;						// �����o���̏��
		SBlockFile BlockInfo;							// �u���b�N�̏��
		SBgObjectFile BgObjInfo;						// �w�i�̃I�u�W�F�N�g�̏��
		SObstacleFile ObstaInfo;						// ��Q���̏��
		D3DXVECTOR3 PlayerPos;							// �v���C���[�̏����ʒu
		D3DXVECTOR3 GoalPos;							// �S�[���̈ʒu
		bool bSuccess;									// ������
	}STutorialFile;

	CFile();					// �R���X�g���N�^
	~CFile();					// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	HRESULT Save(TYPE type);	// �Z�[�u����
	HRESULT Load(TYPE type);	// ���[�h����

	// �g�p����ۂɎg���֐�
	void SetBlock(void);					// �u���b�N�̐ݒu����
	void SetObstacle(void);					// ��Q���̐ݒu����
	void SetItem(void);						// �A�C�e���̐ݒu����
	void SetCamera(void);					// �J�����͈̔͂̐ݒu����
	SResultFile GetResultInfo(void);		// ���U���g���̐ݒu����
	void SetRankingInfo(int* pRank);		// �����L���O�̐ݒ菈��
	SRankingFile GetRankingInfo(void);		// �����L���O�̎擾����
	SGameFile GetGameInfo(void);			// �Q�[����ʂ̎擾����
	void SetTutorial(void);					// �`���[�g���A���̐ݒ菈��
	SBalloonData GetBalloon(const CBalloon::TYPE type);			// �����o���̏��̎擾����

private:

	// �����o�֐�
	HRESULT LoadEnemy(void);		// �G�̃��[�h����

	HRESULT SaveBlock(void);		// �u���b�N�̃Z�[�u����
	HRESULT LoadBlock(void);		// �u���b�N�̃��[�h����

	void LoadBlockProcess(FILE *pFile, SBlockFile *pInfo);			// �u���b�N�̃��[�h�����̒��g

	HRESULT SaveBgObject(void);		// �w�i�I�u�W�F�N�g�̃Z�[�u����
	HRESULT LoadBgObject(void);		// �w�i�I�u�W�F�N�g�̃��[�h����

	HRESULT SaveObstacle(void);		// ��Q���̃Z�[�u����
	HRESULT LoadObstacle(void);		// ��Q���̃��[�h����

	void LoadObstacleProcess(FILE *pFile, SObstacleFile *pInfo);	// ��Q���̃��[�h�����̒��g

	HRESULT SaveItem(void);			// �A�C�e���̃Z�[�u����
	HRESULT LoadItem(void);			// �A�C�e���̃��[�h����

	HRESULT SaveCameraRange(void);	// �J�����͈̔͂̃Z�[�u����
	HRESULT LoadCameraRange(void);	// �J�����͈̔͂̃��[�h����

	HRESULT LoadResultFile(void);	// ���U���g�̃��[�h����

	HRESULT SaveRankingFile(void);	// �����L���O�̃Z�[�u����
	HRESULT LoadRankingFile(void);	// �����L���O�̃��[�h����

	HRESULT LoadGameFile(void);		// �Q�[���̃��[�h����

	HRESULT LoadTutorialFile(void);	// �`���[�g���A���̃��[�h����

	void LoadBalloonProcess(FILE *pFile, SBalloonFile *pInfo);			// �����o���̃��[�h�����̒��g

	// �����o�ϐ�
	SBlockFile m_BlockInfo;			// �u���b�N�̑������
	SObstacleFile m_ObstacleInfo;	// ��Q���̑������
	SItemFile m_ItemInfo;			// �A�C�e���̑������
	SCameraFile m_CameraInfo;		// �J�����̑������
	SResultFile m_ResultInfo;		// ���U���g�̑������
	SRankingFile m_RankingInfo;		// �����L���O�̑������
	SGameFile m_GameInfo;			// �Q�[���̑������
	SBalloonData m_aBalloonData[CBalloon::TYPE_MAX];		// �����o���̎�ނ��Ƃ̏��
	STutorialFile m_TutorialInfo;	// �`���[�g���A���̑������

	// �ÓI�����o�ϐ�
	static const char* c_apFileName[TYPE_MAX];		// �t�@�C���̖��O
	static const char* c_apBooleanDisp[2];			// bool�^�̕\��
};

#endif