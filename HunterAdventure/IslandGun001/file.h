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
#include "ranking.h"
#include "boss_collision.h"
#include "motion.h"
#include "signboard.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define MAX_FILE_DATA		(2048)		// �t�@�C���̃f�[�^�̍ő吔
#define MAX_ENEMY_ROUTE		(10)		// �G�̖ړI�n�̐�

//--------------------------------------------
// �N���X(�t�@�C���ǂݍ��݃N���X)
//--------------------------------------------
class CFile
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_RANKING = 0,	// �����L���O
		TYPE_ENEMY,			// �G
		TYPE_COIN,			// �R�C��
		TYPE_GOLDBONE,		// ���̍�
		TYPE_TREE,			// ��
		TYPE_ROCK,			// ��
		TYPE_BLOCK,			// �u���b�N
		TYPE_BANGFLOWER,	// ���e��
		TYPE_WALL,			// ��
		TYPE_BOSSCOLL,		// �{�X�̓����蔻��
		TYPE_SIGNBOARD,		// �Ŕ�
		TYPE_BALLOON,		// ���D
		TYPE_ENEMYROUTE,	// �G�̜p�j�o�H
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �����L���O�̏��
	struct SRankingInfo
	{
		int aRank[CRanking::MAX_RANKING];				// �����L���O�̃f�[�^
		bool bSuccess;						// ������
	};

	// �G�̏��
	struct SEnemyInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
		int nType;				// ���
	};

	// �R�C���̏��
	struct SCoinInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
	};

	// ���̍��̏��
	struct SGoldBoneInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
	};

	// �؂̏��
	struct STreeInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
		int nType;				// ���
	};

	// ��̏��
	struct SRockInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
		D3DXVECTOR3 scale;		// �g�嗦
		int nType;				// ���
	};

	// �u���b�N�̏��
	struct SBlockInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 scale;		// �g�嗦
	};

	// ���e�Ԃ̏��
	struct SBangFlowerInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
	};

	// �ǂ̏��
	struct SWallInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 scale;		// �g�嗦
		int nType;				// ���
		int nRotType;			// �����̎��
	};

	// �{�X�̓����蔻��̊��
	struct SBossCollBase
	{
		D3DXVECTOR3 offset;		// �I�t�Z�b�g���W
		float fRadius;			// ���a
		bool bWeakness;			// ��_��
	};

	// �{�X�̓����蔻��̏��
	struct SBossCollinfo
	{
		SBossCollBase aBase[CBossCollision::MAX_COLLISION];		// �����蔻��
		int nNum;				// ����
	};

	// �Ŕ̏��
	struct SSignboardinfo
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
		CSignboard::TYPE type;	// ���
	};

	// �G�̜p�j�o�H�̏��
	struct SEnemyRouteinfo
	{
		D3DXVECTOR3 pos[MAX_ENEMY_ROUTE];		// �ʒu
		int nNum;				// �ʒu�̑���
	};

	//************************************************************************************************************************************************
	// ���\����
	//************************************************************************************************************************************************
	// �G�̃t�@�C��
	struct SEnemyFile
	{
		SEnemyInfo aFile[MAX_FILE_DATA];	// ���
		int nNumData;						// ����
		bool bSuccess;						// ������
	};

	// �R�C���̃t�@�C��
	struct SCoinFile
	{
		SCoinInfo aFile[MAX_FILE_DATA];		// ���
		int nNumData;						// ����
		bool bSuccess;						// ������
	};

	// ���̍��̃t�@�C��
	struct SGoldBoneFile
	{
		SGoldBoneInfo aFile[3];				// ���
		int nNumData;						// ����
		bool bSuccess;						// ������
	};

	// �؂̃t�@�C��
	struct STreeFile
	{
		STreeInfo aFile[MAX_FILE_DATA];		// ���
		int nNumData;						// ����
		bool bSuccess;						// ������
	};

	// ��̃t�@�C��
	struct SRockFile
	{
		SRockInfo aFile[MAX_FILE_DATA];		// ���
		int nNumData;						// ����
		bool bSuccess;						// ������
	};

	// �u���b�N�̃t�@�C��
	struct SBlockFile
	{
		SBlockInfo aFile[MAX_FILE_DATA];	// ���
		int nNumData;						// ����
		bool bSuccess;						// ������
	};

	// ���e�Ԃ̃t�@�C��
	struct SBangFlowerFile
	{
		SBangFlowerInfo aFile[MAX_FILE_DATA];	// ���
		int nNumData;							// ����
		bool bSuccess;							// ������
	};

	// �ǂ̃t�@�C��
	struct SWallFile
	{
		SWallInfo aFile[MAX_FILE_DATA];			// ���
		int nNumData;							// ����
		bool bSuccess;							// ������
	};

	// �{�X�̓����蔻��̃t�@�C��
	struct SBossCollFile
	{
		SBossCollinfo aFile[MAX_PARTS];			// ���
		int nNumData;							// ���f���̑���
		bool bSuccess;							// ������
	};

	// �Ŕ̃t�@�C��
	struct SSignboardFile
	{
		SSignboardinfo aFile[MAX_FILE_DATA];	// ���
		int nNumData;							// ����
		bool bSuccess;							// ������
	};

	// ���D�̃t�@�C��
	struct SBalloonFile
	{
		D3DXVECTOR3 pos[MAX_FILE_DATA];			// �ʒu
		int nNumData;							// ����
		bool bSuccess;							// ������
	};

	// �G�̜p�j�o�H�̃t�@�C��
	struct SEnemyRouteFile
	{
		SEnemyRouteinfo aFile[MAX_FILE_DATA];	// ���
		int nNumData;							// ����
		bool bSuccess;							// ������
	};

	CFile();					// �R���X�g���N�^
	~CFile();					// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������

	HRESULT Save(const TYPE type);	// �Z�[�u����
	HRESULT Load(const TYPE type);	// ���[�h����

	void SetRankingInfo(int* pRank);		// �����L���O�̐ݒ菈��
	SRankingInfo GetRankingInfo(void);		// �����L���O�̎擾����

	void SetEnemy(void);		// �G�̐ݒ菈��
	void SetCoin(void);			// �R�C���̐ݒ菈��
	void SetGoldBone(void);		// ���̍��̐ݒ菈��
	void SetTree(void);			// �؂̐ݒ菈��
	void SetRock(void);			// ��̐ݒ菈��
	void SetBlock(void);		// �u���b�N�̐ݒ菈��
	void SetBangFlower(void);	// ���e�Ԃ̐ݒ菈��
	void SetWall(void);			// �ǂ̐ݒ菈��
	void SetBossColl(CBossCollision** pColl);		// �{�X�̓����蔻��̐ݒ菈��
	void SetSignboard(void);	// �Ŕ̐ݒ菈��
	void SetBalloon(void);		// ���D�̐ݒ菈��
	D3DXVECTOR3 GetEnemyRoute(const int nRoute, const int nNum);		// �G�̜p�j�o�H�̎擾����
	int GetEnemyRouteNumPos(const int nRoute);			// �G�̜p�j�o�H�̈ʒu�̑����̎擾����
	int GetEnemyRouteNum(void);	// �G�̜p�j�o�H�̑����̎擾����

	// �ÓI�����o�֐�
	static CFile* Create(void);		// ��������

private:		// �����̂݃A�N�Z�X�ł���

	// �����o�֐�(�Z�[�u�֌W)
	HRESULT SaveRanking(void);		// �����L���O�̃Z�[�u����

	// �����o�֐�(���[�h�֌W)
	HRESULT LoadRanking(void);		// �����L���O�̃��[�h����
	HRESULT LoadEnemy(void);		// �G�̃��[�h����
	HRESULT LoadCoin(void);			// �R�C���̃��[�h����
	HRESULT LoadGoldBone(void);		// ���̍��̃��[�h����
	HRESULT LoadTree(void);			// �؂̃��[�h����
	HRESULT LoadRock(void);			// ��̃��[�h����
	HRESULT LoadBlock(void);		// �u���b�N�̃��[�h����
	HRESULT LoadBangFlower(void);	// ���e�Ԃ̃��[�h����
	HRESULT LoadWall(void);			// �ǂ̃��[�h����
	HRESULT LoadBossColl(void);		// �{�X�̓����蔻��̃��[�h����
	HRESULT LoadSignboard(void);	// �Ŕ̃��[�h����
	HRESULT LoadBalloon(void);		// ���D�̃��[�h����
	HRESULT LoadEnemyRoute(void);	// �G�̌o�H�̃��[�h����

	// �����o�ϐ�
	SRankingInfo m_RankingInfo;			// �����L���O�̏��
	SEnemyFile m_EnemyFile;				// �G�̏��
	SCoinFile m_CoinFile;				// �R�C���̏��
	SGoldBoneFile m_GoldBoneFile;		// ���̍��̏��
	STreeFile m_TreeFile;				// �؂̏��
	SRockFile m_RockFile;				// ��̏��
	SBlockFile m_BlockFile;				// �u���b�N�̏��
	SBangFlowerFile m_BangFlowerFile;	// ���e�Ԃ̏��
	SWallFile m_WallFile;				// �ǂ̏��
	SBossCollFile m_BossCollFile;		// �{�X�̓����蔻��̏��
	SSignboardFile m_SignboardFile;		// �Ŕ̏��
	SBalloonFile m_BalloonFile;			// ���D�̏��
	SEnemyRouteFile m_EnemyRouteFile;	// �G�̌o�H�̏��

	// �ÓI�����o�ϐ�
	static const char* c_apBooleanDisp[2];			// bool�^�̕\��
};

#endif