//============================================
//
// �t�@�C���̃��C������[file.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "scene.h"
#include "file.h"
#include "object.h"

#include "enemy.h"
#include "coin.h"
#include "gold_bone.h"
#include "tree.h"
#include "rock.h"
#include "block.h"
#include "bang_flower.h"
#include "wall.h"
#include "boss.h"
#include "balloon_spawner.h"

//--------------------------------------------
// �萔��`
//--------------------------------------------
namespace
{
	const char* RANKING_BIN = "data\\BIN\\Ranking.bin";			// �����L���O�̃e�L�X�g
	const char* ENEMY_TXT = "data\\TXT\\Enemy.txt";				// �G�̃e�L�X�g
	const char* COIN_TXT = "data\\TXT\\Coin.txt";				// �R�C���̃e�L�X�g
	const char* GOLDBONE_TXT = "data\\TXT\\GoldBone.txt";		// ���̍��̃e�L�X�g
	const char* TREE_TXT = "data\\TXT\\Tree.txt";				// �؂̃e�L�X�g
	const char* ROCK_TXT = "data\\TXT\\Rock.txt";				// ��̃e�L�X�g
	const char* BLOCK_TXT = "data\\TXT\\Block.txt";				// �u���b�N�̃e�L�X�g
	const char* BANGFLOWER_TXT = "data\\TXT\\BangFlower.txt";	// ���e�Ԃ̃e�L�X�g
	const char* WALL_TXT = "data\\TXT\\Wall.txt";				// �ǂ̃e�L�X�g
	const char* BOSSCOLL_TXT = "data\\TXT\\BossColl.txt";		// �{�X�̓����蔻��̃e�L�X�g
	const char* SIGNBOARD_TXT = "data\\TXT\\Signboard.txt";		// �Ŕ̃e�L�X�g
	const char* BALLOON_TXT = "data\\TXT\\Balloon.txt";			// ���D�̃e�L�X�g
	const char* ENEMYROUTE_TXT = "data\\TXT\\EnemyRoute.txt";	// �G�̜p�j�o�H�̃e�L�X�g

	const int GOLDBONE_NUM = 3;		// ���̍��̐�
}

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CFile::c_apBooleanDisp[2] =					// bool�^�̕\��
{
	"FALSE",							// TRUE
	"TRUE",								// FALSE
};

//===========================================
// �R���X�g���N�^
//===========================================
CFile::CFile()
{
	// �S�Ă̒l���N���A����
	for (int nCntRank = 0; nCntRank < CRanking::MAX_RANKING; nCntRank++)
	{
		m_RankingInfo.aRank[nCntRank] = 0;				// �����L���O�̒l
	}

	for (int nCntBoss = 0; nCntBoss < MAX_PARTS; nCntBoss++)
	{
		for (int nCntColl = 0; nCntColl < CBossCollision::MAX_COLLISION; nCntColl++)
		{
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].offset = NONE_D3DXVECTOR3;
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].fRadius = 0.0f;
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].bWeakness = false;
		}

		m_BossCollFile.aFile[nCntBoss].nNum = 0;
	}

	for (int nCntFile = 0; nCntFile < MAX_FILE_DATA; nCntFile++)
	{
		m_EnemyFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_EnemyFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_EnemyFile.aFile[nCntFile].nType = 0;

		m_CoinFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;

		m_TreeFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_TreeFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_TreeFile.aFile[nCntFile].nType = 0;

		m_RockFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].nType = 0;

		m_BlockFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_BlockFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;

		m_BangFlowerFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_BangFlowerFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;

		m_WallFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_WallFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;
		m_WallFile.aFile[nCntFile].nType = 0;
		m_WallFile.aFile[nCntFile].nRotType = 0;

		m_SignboardFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_SignboardFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_SignboardFile.aFile[nCntFile].type = CSignboard::TYPE::TYPE_JUMP;

		m_BalloonFile.pos[nCntFile] = NONE_D3DXVECTOR3;

		for (int nCntRoute = 0; nCntRoute < MAX_ENEMY_ROUTE; nCntRoute++)
		{
			m_EnemyRouteFile.aFile[nCntFile].pos[nCntRoute] = NONE_D3DXVECTOR3;		// �G�̌o�H�̏��
			m_EnemyRouteFile.aFile[nCntFile].nNum = 0;		// ����
		}
	}

	for (int nCntBone = 0; nCntBone < GOLDBONE_NUM; nCntBone++)
	{
		m_GoldBoneFile.aFile[nCntBone].pos = NONE_D3DXVECTOR3;
	}

	// �������N���A����
	m_EnemyFile.nNumData = 0;			// �G�̏��
	m_CoinFile.nNumData = 0;			// �R�C���̏��
	m_GoldBoneFile.nNumData = 0;		// ���̍��̏��
	m_TreeFile.nNumData = 0;			// �؂̏��
	m_RockFile.nNumData = 0;			// ��̏��
	m_BlockFile.nNumData = 0;			// �u���b�N�̏��
	m_BangFlowerFile.nNumData = 0;		// ���e�Ԃ̏��
	m_WallFile.nNumData = 0;			// �ǂ̏��
	m_BossCollFile.nNumData = 0;		// �{�X�̓����蔻��̏��
	m_SignboardFile.nNumData = 0;		// �Ŕ̏��
	m_BalloonFile.nNumData = 0;			// ���D�̏��
	m_EnemyRouteFile.nNumData = 0;		// �G�̜p�j�o�H�̏��

	// �����󋵂��N���A����
	m_RankingInfo.bSuccess = false;		// �����L���O
	m_EnemyFile.bSuccess = false;		// �G�̏��
	m_CoinFile.bSuccess = false;		// �R�C���̏��
	m_GoldBoneFile.bSuccess = false;	// ���̍��̏��
	m_TreeFile.bSuccess = false;		// �؂̏��
	m_RockFile.bSuccess = false;		// ��̏��
	m_BlockFile.bSuccess = false;		// �u���b�N�̏��
	m_BangFlowerFile.bSuccess = false;	// ���e�Ԃ̏��
	m_WallFile.bSuccess = false;		// �ǂ̏��
	m_BossCollFile.bSuccess = false;	// �{�X�̓����蔻��̏��
	m_SignboardFile.bSuccess = false;	// �Ŕ̏��
	m_BalloonFile.bSuccess = false;		// ���D�̏��
	m_EnemyRouteFile.bSuccess = false;	// �G�̜p�j�o�H�̏��
}

//===========================================
// �f�X�g���N�^
//===========================================
CFile::~CFile()
{

}

//===========================================
// �Z�[�u����
//===========================================
HRESULT CFile::Save(const TYPE type)
{
	switch (type)
	{
	case CFile::TYPE_RANKING:

		// �����L���O�̃Z�[�u����
		if (FAILED(SaveRanking()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// ���[�h����
//===========================================
HRESULT CFile::Load(const TYPE type)
{
	switch (type)
	{
	case CFile::TYPE_RANKING:

		// �����L���O�̃��[�h����
		if (FAILED(LoadRanking()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_ENEMY:

		// �G�̃��[�h����
		if (FAILED(LoadEnemy()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_COIN:

		// �R�C���̃��[�h����
		if (FAILED(LoadCoin()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_GOLDBONE:

		// ���̍��̃��[�h����
		if (FAILED(LoadGoldBone()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_TREE:

		// �؂̃��[�h����
		if (FAILED(LoadTree()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_ROCK:

		// ��̃��[�h����
		if (FAILED(LoadRock()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BLOCK:

		// �u���b�N�̃��[�h����
		if (FAILED(LoadBlock()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BANGFLOWER:

		// ���e�Ԃ̃��[�h����
		if (FAILED(LoadBangFlower()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_WALL:

		// �ǂ̃��[�h����
		if (FAILED(LoadWall()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BOSSCOLL:

		if (FAILED(LoadBossColl()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_SIGNBOARD:

		if (FAILED(LoadSignboard()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BALLOON:

		if (FAILED(LoadBalloon()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_ENEMYROUTE:

		if (FAILED(LoadEnemyRoute()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ���ʂ�Ԃ�
	return S_OK;
}

//===========================================
// �����L���O�̐ݒ菈��
//===========================================
void CFile::SetRankingInfo(int* pRank)
{
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++, pRank++)
	{
		// �����L���O�̏���ݒ肷��
		m_RankingInfo.aRank[nCnt] = *pRank;
	}
}

//===========================================
// �����L���O�̎擾����
//===========================================
CFile::SRankingInfo CFile::GetRankingInfo(void)
{
	// �����L���O�̏���Ԃ�
	return m_RankingInfo;
}

//===========================================
// �G�̐ݒ菈��
//===========================================
void CFile::SetEnemy(void)
{
	if (m_EnemyFile.bSuccess == true)
	{ // �G���ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_EnemyFile.nNumData; nCnt++)
		{
			// �G�𐶐�����
			CEnemy::Create
			(
				m_EnemyFile.aFile[nCnt].pos,
				m_EnemyFile.aFile[nCnt].rot,
				(CEnemy::TYPE)m_EnemyFile.aFile[nCnt].nType
			);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// �R�C���̐ݒ菈��
//===========================================
void CFile::SetCoin(void)
{
	if (m_CoinFile.bSuccess == true)
	{ // �R�C�����ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_CoinFile.nNumData; nCnt++)
		{
			// �R�C���𐶐�����
			CCoin::Create
			(
				m_CoinFile.aFile[nCnt].pos
			);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// ���̍��̐ݒ菈��
//===========================================
void CFile::SetGoldBone(void)
{
	if (m_GoldBoneFile.bSuccess == true)
	{ // ���̍����ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_GoldBoneFile.nNumData; nCnt++)
		{
			// ���̍��𐶐�����
			CGoldBone::Create
			(
				m_GoldBoneFile.aFile[nCnt].pos
			);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// �؂̐ݒ菈��
//===========================================
void CFile::SetTree(void)
{
	if (m_TreeFile.bSuccess == true)
	{ // �؂��ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_TreeFile.nNumData; nCnt++)
		{
			// �؂𐶐�����
			CTree::Create
			(
				m_TreeFile.aFile[nCnt].pos,
				m_TreeFile.aFile[nCnt].rot,
				(CTree::TYPE)m_TreeFile.aFile[nCnt].nType
			);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// ��̐ݒ菈��
//===========================================
void CFile::SetRock(void)
{
	if (m_RockFile.bSuccess == true)
	{ // �₪�ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_RockFile.nNumData; nCnt++)
		{
			// ��𐶐�����
			CRock::Create
			(
				m_RockFile.aFile[nCnt].pos,
				m_RockFile.aFile[nCnt].rot,
				m_RockFile.aFile[nCnt].scale,
				(CRock::TYPE)m_RockFile.aFile[nCnt].nType
			);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// �u���b�N�̐ݒ菈��
//===========================================
void CFile::SetBlock(void)
{
	if (m_BlockFile.bSuccess == true)
	{ // �u���b�N���ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_BlockFile.nNumData; nCnt++)
		{
			// �u���b�N�𐶐�����
			CBlock::Create
			(
				m_BlockFile.aFile[nCnt].pos,
				m_BlockFile.aFile[nCnt].scale
			);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// ���e�Ԃ̐ݒ菈��
//===========================================
void CFile::SetBangFlower(void)
{
	if (m_BangFlowerFile.bSuccess == true)
	{ // ���e�Ԃ��ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_BangFlowerFile.nNumData; nCnt++)
		{
			// ���e�Ԃ𐶐�����
			CBangFlower::Create
			(
				m_BangFlowerFile.aFile[nCnt].pos,
				m_BangFlowerFile.aFile[nCnt].rot
			);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// �ǂ̐ݒ菈��
//===========================================
void CFile::SetWall(void)
{
	if (m_WallFile.bSuccess == true)
	{ // �ǂ��ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_WallFile.nNumData; nCnt++)
		{
			// �ǂ𐶐�����
			CWall::Create
			(
				m_WallFile.aFile[nCnt].pos,
				m_WallFile.aFile[nCnt].scale,
				(CWall::TYPE)(m_WallFile.aFile[nCnt].nType),
				(CWall::ROTTYPE)(m_WallFile.aFile[nCnt].nRotType)
			);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// �{�X�̓����蔻��̐ݒ菈��
//===========================================
void CFile::SetBossColl(CBossCollision** pColl)
{
	if (m_BossCollFile.bSuccess == true)
	{ // �{�X�̓����蔻�肪�ǂݍ��߂��ꍇ

		for (int nCntPart = 0; nCntPart < m_BossCollFile.nNumData; nCntPart++)
		{
			// �����蔻��𐶐�
			pColl[nCntPart] = CBossCollision::Create(m_BossCollFile.aFile[nCntPart].nNum);

			for (int nCntColl = 0; nCntColl < m_BossCollFile.aFile[nCntPart].nNum; nCntColl++)
			{
				pColl[nCntPart]->SetCollOffset(m_BossCollFile.aFile[nCntPart].aBase[nCntColl].offset, nCntColl);
				pColl[nCntPart]->SetRadius(m_BossCollFile.aFile[nCntPart].aBase[nCntColl].fRadius, nCntColl);
				pColl[nCntPart]->SetEnableWeakness(m_BossCollFile.aFile[nCntPart].aBase[nCntColl].bWeakness, nCntColl);
			}
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// �Ŕ̐ݒ菈��
//===========================================
void CFile::SetSignboard(void)
{
	if (m_SignboardFile.bSuccess == true)
	{ // �Ŕ��ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_SignboardFile.nNumData; nCnt++)
		{
			// �Ŕ𐶐�����
			CSignboard::Create
			(
				m_SignboardFile.aFile[nCnt].pos,
				m_SignboardFile.aFile[nCnt].rot,
				m_SignboardFile.aFile[nCnt].type
			);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// ���D�̐ݒ菈��
//===========================================
void CFile::SetBalloon(void)
{
	if (m_BalloonFile.bSuccess == true)
	{ // �Ŕ��ǂݍ��߂��ꍇ

		for (int nCnt = 0; nCnt < m_BalloonFile.nNumData; nCnt++)
		{
			// �Ŕ𐶐�����
			CBalloonSpawner::Create(m_BalloonFile.pos[nCnt]);
		}
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);
	}
}

//===========================================
// �G�̜p�j�o�H�̐ݒ菈��
//===========================================
D3DXVECTOR3 CFile::GetEnemyRoute(const int nRoute, const int nNum)
{
	if (m_EnemyRouteFile.bSuccess == true && 
		m_EnemyRouteFile.nNumData > nRoute &&
		m_EnemyRouteFile.aFile[nRoute].nNum > nNum)
	{ // ����ǂݍ���ł����ꍇ

		// �p�j�o�H��Ԃ�
		return m_EnemyRouteFile.aFile[nRoute].pos[nNum];
	}
	else
	{ // ��L�ȊO

		// ��~
		assert(false);

		// �ً}���W��Ԃ�
		return NONE_D3DXVECTOR3;
	}
}

//===========================================
// �G�̜p�j�o�H�̑����̎擾����
//===========================================
int CFile::GetEnemyRouteNumPos(const int nRoute)
{
	// ������Ԃ�
	return m_EnemyRouteFile.aFile[nRoute].nNum;
}

//===========================================
// �G�̜p�j�o�H�̑����̎擾����
//===========================================
int CFile::GetEnemyRouteNum(void)
{
	// ������Ԃ�
	return m_EnemyRouteFile.nNumData;
}

//===========================================
// �t�@�C���̐�������
//===========================================
CFile* CFile::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CFile* pFile = nullptr;		// �t�@�C���̃C���X�^���X�𐶐�

	if (pFile == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pFile = new CFile;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pFile != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pFile->Init()))
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

	// �t�@�C���̃|�C���^��Ԃ�
	return pFile;
}

//===========================================
// ����������
//===========================================
HRESULT CFile::Init(void)
{
	// �S�Ă̒l���N���A����
	for (int nCntRank = 0; nCntRank < CRanking::MAX_RANKING; nCntRank++)
	{
		m_RankingInfo.aRank[nCntRank] = 0;				// �����L���O�̒l
	}

	for (int nCntBoss = 0; nCntBoss < MAX_PARTS; nCntBoss++)
	{
		for (int nCntColl = 0; nCntColl < CBossCollision::MAX_COLLISION; nCntColl++)
		{
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].offset = NONE_D3DXVECTOR3;
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].fRadius = 0.0f;
			m_BossCollFile.aFile[nCntBoss].aBase[nCntColl].bWeakness = false;
		}

		m_BossCollFile.aFile[nCntBoss].nNum = 0;
	}

	for (int nCntFile = 0; nCntFile < MAX_FILE_DATA; nCntFile++)
	{
		m_EnemyFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_EnemyFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_EnemyFile.aFile[nCntFile].nType = 0;

		m_CoinFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;

		m_TreeFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_TreeFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_TreeFile.aFile[nCntFile].nType = 0;

		m_RockFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;
		m_RockFile.aFile[nCntFile].nType = 0;

		m_BlockFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_BlockFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;

		m_BangFlowerFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_BangFlowerFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;

		m_WallFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_WallFile.aFile[nCntFile].scale = NONE_D3DXVECTOR3;
		m_WallFile.aFile[nCntFile].nType = 0;
		m_WallFile.aFile[nCntFile].nRotType = 0;

		m_SignboardFile.aFile[nCntFile].pos = NONE_D3DXVECTOR3;
		m_SignboardFile.aFile[nCntFile].rot = NONE_D3DXVECTOR3;
		m_SignboardFile.aFile[nCntFile].type = CSignboard::TYPE::TYPE_JUMP;

		m_BalloonFile.pos[nCntFile] = NONE_D3DXVECTOR3;

		for (int nCntRoute = 0; nCntRoute < MAX_ENEMY_ROUTE; nCntRoute++)
		{
			m_EnemyRouteFile.aFile[nCntFile].pos[nCntRoute] = NONE_D3DXVECTOR3;		// �G�̌o�H�̏��
			m_EnemyRouteFile.aFile[nCntFile].nNum = 0;		// ����
		}
	}

	for (int nCntBone = 0; nCntBone < GOLDBONE_NUM; nCntBone++)
	{
		m_GoldBoneFile.aFile[nCntBone].pos = NONE_D3DXVECTOR3;
	}

	// �������N���A����
	m_EnemyFile.nNumData = 0;			// �G�̏��
	m_CoinFile.nNumData = 0;			// �R�C���̏��
	m_GoldBoneFile.nNumData = 0;		// ���̍��̏��
	m_TreeFile.nNumData = 0;			// �؂̏��
	m_RockFile.nNumData = 0;			// ��̏��
	m_BlockFile.nNumData = 0;			// �u���b�N�̏��
	m_BangFlowerFile.nNumData = 0;		// ���e�Ԃ̏��
	m_WallFile.nNumData = 0;			// �ǂ̏��
	m_BossCollFile.nNumData = 0;		// �{�X�̓����蔻��̏��
	m_SignboardFile.nNumData = 0;		// �Ŕ̏��
	m_BalloonFile.nNumData = 0;			// ���D�̏��
	m_EnemyRouteFile.nNumData = 0;		// �G�̜p�j�o�H�̏��

	// �����󋵂��N���A����
	m_RankingInfo.bSuccess = false;		// �����L���O
	m_EnemyFile.bSuccess = false;		// �G�̏��
	m_CoinFile.bSuccess = false;		// �R�C���̏��
	m_GoldBoneFile.bSuccess = false;	// ���̍��̏��
	m_TreeFile.bSuccess = false;		// �؂̏��
	m_RockFile.bSuccess = false;		// ��̏��
	m_BlockFile.bSuccess = false;		// �u���b�N�̏��
	m_BangFlowerFile.bSuccess = false;	// ���e�Ԃ̏��
	m_WallFile.bSuccess = false;		// �ǂ̏��
	m_BossCollFile.bSuccess = false;	// �{�X�̓����蔻��̏��
	m_SignboardFile.bSuccess = false;	// �Ŕ̏��
	m_BalloonFile.bSuccess = false;		// ���D�̏��
	m_EnemyRouteFile.bSuccess = false;	// �G�̜p�j�o�H�̏��

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CFile::Uninit(void)
{
	// ���������J������
	delete this;
}

//===========================================
// �����L���O�̃Z�[�u����
//===========================================
HRESULT CFile::SaveRanking(void)
{
	FILE* pFile;												// �t�@�C���|�C���^��錾
	m_RankingInfo.bSuccess = false;								// ������

	unsigned char aData[128];		// �f�[�^

	int nSize = 0;		// �f�[�^�̑��T�C�Y
	int nCheckSum = 0;	// �`�F�b�N�T��

	// �[���N���A
	memset(&aData[0], 0, sizeof(aData));

	// aNum�̐��l���R�s�[����
	memcpy(&aData[nSize], &m_RankingInfo.aRank, sizeof(m_RankingInfo.aRank));

	// �T�C�Y�ϐ��Ƀo�C�g�������Z����
	nSize += sizeof(m_RankingInfo.aRank);

	// �`�F�b�N�T���𐶐�����
	for (int nCnt = 0; nCnt < nSize; nCnt++)
	{
		// �`�F�b�N�T���ɐ��l�����Z���Ă���
		nCheckSum += aData[nCnt];
	}

	// �t�@�C�����J��
	pFile = fopen(RANKING_BIN, "wb");			// �o�C�i���t�@�C���ɏ������ނ��߂ɊJ��

	// �t�@�C�����r����
	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		// ���o�C�g������������
		fwrite(&nSize, sizeof(int), 1, pFile);

		// �`�F�b�N�T������������
		fwrite(&nCheckSum, sizeof(int), 1, pFile);

		// �t�@�C�����琔�l�������o��
		fwrite(&m_RankingInfo.aRank[0], sizeof(int), CRanking::MAX_RANKING, pFile);

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_RankingInfo.bSuccess = true;

		// ������Ԃ�
		return S_OK;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//===========================================
// �����L���O�̃��[�h����
//===========================================
HRESULT CFile::LoadRanking(void)
{
	FILE* pFile;						// �t�@�C���|�C���^��錾
	m_RankingInfo.bSuccess = false;		// ������

	int nSize = 0;		// �f�[�^�̑��T�C�Y
	int nCheckSum = 0;	// �`�F�b�N�T��
	int nCheckCnt = 0;	// �J�E���g

	unsigned char aData[128];		// �f�[�^

	// �[���N���A
	memset(&aData[0], 0, sizeof(aData));

	// �t�@�C�����J��
	pFile = fopen(RANKING_BIN, "rb");			// �o�C�i���t�@�C������ǂݍ��ނ��߂ɊJ��

	// �t�@�C�����r����
	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		//�t�@�C������T�C�Y����ǂݍ���
		fread(&nSize, sizeof(int), 1, pFile);

		//�t�@�C������T���`�F�b�N�̒l��ǂݍ���
		fread(&nCheckSum, sizeof(int), 1, pFile);

		// �t�@�C������S�Ẵf�[�^��ǂݍ���
		fread(&aData[0], sizeof(unsigned char), nSize, pFile);

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_RankingInfo.bSuccess = true;

		for (int nCnt = 0; nCnt < nSize; nCnt++)
		{
			// �J�E���g�����Z����
			nCheckCnt += aData[nCnt];
		}

		if (nCheckSum == nCheckCnt)
		{ // �T���`�F�b�N�p�ϐ��Ɠǂݍ��񂾃f�[�^����v�����ꍇ

			// aNum�̐��l���R�s�[����
			memcpy(&m_RankingInfo.aRank, &aData[0], sizeof(m_RankingInfo.aRank));
		}
		else
		{ // �T���`�F�b�N�p�ϐ��Ɠǂݍ��񂾃f�[�^����v�����ꍇ

			// ��~
			assert(false);
		}

		// ������Ԃ�
		return S_OK;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		for (int nCntRank = 0; nCntRank < CRanking::MAX_RANKING; nCntRank++)
		{
			// ���l��ݒ肷��
			m_RankingInfo.aRank[0] = 0;
		}

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//===========================================
// �G�̃��[�h����
//===========================================
HRESULT CFile::LoadEnemy(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_EnemyFile.nNumData = 0;			// ����
	m_EnemyFile.bSuccess = false;		// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(ENEMY_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_ENEMY") == 0)
			{ // �ǂݍ��񂾕����� SET_ENEMY �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_ENEMY �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].pos.x,
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].pos.y,
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].pos.z);		// �ʒu��ǂݍ���
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].rot.x,
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].rot.y,
							&m_EnemyFile.aFile[m_EnemyFile.nNumData].rot.z);		// ������ǂݍ���
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_EnemyFile.aFile[m_EnemyFile.nNumData].nType);		// ��ނ�ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_ENEMY") != 0);		// �ǂݍ��񂾕����� END_SET_ENEMY �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_EnemyFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_EnemyFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �R�C���̃��[�h����
//===========================================
HRESULT CFile::LoadCoin(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_CoinFile.nNumData = 0;			// ����
	m_CoinFile.bSuccess = false;		// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(COIN_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_COIN") == 0)
			{ // �ǂݍ��񂾕����� SET_COIN �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_COIN �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_CoinFile.aFile[m_CoinFile.nNumData].pos.x,
							&m_CoinFile.aFile[m_CoinFile.nNumData].pos.y,
							&m_CoinFile.aFile[m_CoinFile.nNumData].pos.z);		// �ʒu��ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_COIN") != 0);		// �ǂݍ��񂾕����� END_SET_COIN �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_CoinFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_CoinFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// ���̍��̃��[�h����
//===========================================
HRESULT CFile::LoadGoldBone(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_GoldBoneFile.nNumData = 0;		// ����
	m_GoldBoneFile.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(GOLDBONE_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_GOLDBONE") == 0)
			{ // �ǂݍ��񂾕����� SET_GOLDBONE �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_GOLDBONE �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_GoldBoneFile.aFile[m_GoldBoneFile.nNumData].pos.x,
							&m_GoldBoneFile.aFile[m_GoldBoneFile.nNumData].pos.y,
							&m_GoldBoneFile.aFile[m_GoldBoneFile.nNumData].pos.z);		// �ʒu��ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_GOLDBONE") != 0);		// �ǂݍ��񂾕����� END_SET_GOLDBONE �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_GoldBoneFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_GoldBoneFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �؂̃��[�h����
//===========================================
HRESULT CFile::LoadTree(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_TreeFile.nNumData = 0;		// ����
	m_TreeFile.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(TREE_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_TREE") == 0)
			{ // �ǂݍ��񂾕����� SET_TREE �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_TREE �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_TreeFile.aFile[m_TreeFile.nNumData].pos.x,
							&m_TreeFile.aFile[m_TreeFile.nNumData].pos.y,
							&m_TreeFile.aFile[m_TreeFile.nNumData].pos.z);		// �ʒu��ǂݍ���
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_TreeFile.aFile[m_TreeFile.nNumData].rot.x,
							&m_TreeFile.aFile[m_TreeFile.nNumData].rot.y,
							&m_TreeFile.aFile[m_TreeFile.nNumData].rot.z);		// �ʒu��ǂݍ���
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_TreeFile.aFile[m_TreeFile.nNumData].nType);		// ��ނ�ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_TREE") != 0);		// �ǂݍ��񂾕����� END_SET_TREE �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_TreeFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_TreeFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// ��̃��[�h����
//===========================================
HRESULT CFile::LoadRock(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_RockFile.nNumData = 0;			// ����
	m_RockFile.bSuccess = false;		// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(ROCK_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_ROCK") == 0)
			{ // �ǂݍ��񂾕����� SET_ROCK �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_ROCK �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_RockFile.aFile[m_RockFile.nNumData].pos.x,
							&m_RockFile.aFile[m_RockFile.nNumData].pos.y,
							&m_RockFile.aFile[m_RockFile.nNumData].pos.z);		// �ʒu��ǂݍ���
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_RockFile.aFile[m_RockFile.nNumData].rot.x,
							&m_RockFile.aFile[m_RockFile.nNumData].rot.y,
							&m_RockFile.aFile[m_RockFile.nNumData].rot.z);		// ������ǂݍ���
					}

					if (strcmp(&aString[0], "SCALE") == 0)
					{ // �ǂݍ��񂾕����� SCALE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_RockFile.aFile[m_RockFile.nNumData].scale.x,
							&m_RockFile.aFile[m_RockFile.nNumData].scale.y,
							&m_RockFile.aFile[m_RockFile.nNumData].scale.z);		// �g�嗦��ǂݍ���
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_RockFile.aFile[m_RockFile.nNumData].nType);		// ��ނ�ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_ROCK") != 0);		// �ǂݍ��񂾕����� END_SET_ROCK �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_RockFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_RockFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �u���b�N�̃��[�h����
//===========================================
HRESULT CFile::LoadBlock(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_BlockFile.nNumData = 0;			// ����
	m_BlockFile.bSuccess = false;		// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(BLOCK_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_BLOCK") == 0)
			{ // �ǂݍ��񂾕����� SET_BLOCK �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_BLOCK �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_BlockFile.aFile[m_BlockFile.nNumData].pos.x,
							&m_BlockFile.aFile[m_BlockFile.nNumData].pos.y,
							&m_BlockFile.aFile[m_BlockFile.nNumData].pos.z);		// �ʒu��ǂݍ���
					}

					if (strcmp(&aString[0], "SCALE") == 0)
					{ // �ǂݍ��񂾕����� SCALE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_BlockFile.aFile[m_BlockFile.nNumData].scale.x,
							&m_BlockFile.aFile[m_BlockFile.nNumData].scale.y,
							&m_BlockFile.aFile[m_BlockFile.nNumData].scale.z);		// �g�嗦��ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_BLOCK") != 0);		// �ǂݍ��񂾕����� END_SET_BLOCK �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_BlockFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_BlockFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// ���e�Ԃ̃��[�h����
//===========================================
HRESULT CFile::LoadBangFlower(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_BangFlowerFile.nNumData = 0;			// ����
	m_BangFlowerFile.bSuccess = false;		// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(BANGFLOWER_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_BANGFLOWER") == 0)
			{ // �ǂݍ��񂾕����� SET_BANGFLOWER �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_BANGFLOWER �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].pos.x,
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].pos.y,
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].pos.z);		// �ʒu��ǂݍ���
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].rot.x,
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].rot.y,
							&m_BangFlowerFile.aFile[m_BangFlowerFile.nNumData].rot.z);		// ������ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_BANGFLOWER") != 0);		// �ǂݍ��񂾕����� END_SET_BANGFLOWER �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_BangFlowerFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_BangFlowerFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �ǂ̃��[�h����
//===========================================
HRESULT CFile::LoadWall(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_WallFile.nNumData = 0;			// ����
	m_WallFile.bSuccess = false;		// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(WALL_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_WALL") == 0)
			{ // �ǂݍ��񂾕����� SET_WALL �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_WALL �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_WallFile.aFile[m_WallFile.nNumData].pos.x,
							&m_WallFile.aFile[m_WallFile.nNumData].pos.y,
							&m_WallFile.aFile[m_WallFile.nNumData].pos.z);		// �ʒu��ǂݍ���
					}

					if (strcmp(&aString[0], "SCALE") == 0)
					{ // �ǂݍ��񂾕����� SCALE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_WallFile.aFile[m_WallFile.nNumData].scale.x,
							&m_WallFile.aFile[m_WallFile.nNumData].scale.y,
							&m_WallFile.aFile[m_WallFile.nNumData].scale.z);	// �g�嗦��ǂݍ���
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_WallFile.aFile[m_WallFile.nNumData].nType);		// ��ނ�ǂݍ���
					}

					if (strcmp(&aString[0], "ROTTYPE") == 0)
					{ // �ǂݍ��񂾕����� ROTTYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_WallFile.aFile[m_WallFile.nNumData].nRotType);	// �����̎�ނ�ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_WALL") != 0);		// �ǂݍ��񂾕����� END_SET_WALL �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_WallFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_WallFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �{�X�̓����蔻��̃��[�h����
//===========================================
HRESULT CFile::LoadBossColl(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_BossCollFile.nNumData = 0;		// ����
	m_BossCollFile.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(BOSSCOLL_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "NUM_MODEL") == 0)
			{ // �ǂݍ��񂾕����� NUM_MODEL �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%d", &m_BossCollFile.nNumData);	// �ʒu��ǂݍ���

				for (int nCnt = 0; nCnt < m_BossCollFile.nNumData; nCnt++)
				{
					// �t�@�C�����當�����ǂݍ���
					nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

					if (strcmp(&aString[0], "SET_COLL") == 0)
					{ // �ǂݍ��񂾕����� SET_COLL �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_SET_WALL �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_COLL") == 0)
							{ // �ǂݍ��񂾕����� NUM_COLL �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_BossCollFile.aFile[nCnt].nNum);		// ���̑�����ǂݍ���

								for (int nCntColl = 0; nCntColl < m_BossCollFile.aFile[nCnt].nNum; nCntColl++)
								{
									fscanf(pFile, "%s", &aString[0]);				// POS ��ǂݍ��� (�s�v)
									fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%f%f%f",
										&m_BossCollFile.aFile[nCnt].aBase[nCntColl].offset.x,
										&m_BossCollFile.aFile[nCnt].aBase[nCntColl].offset.y,
										&m_BossCollFile.aFile[nCnt].aBase[nCntColl].offset.z);		// �ʒu��ǂݍ���

									fscanf(pFile, "%s", &aString[0]);				// RADIUS ��ǂݍ��� (�s�v)
									fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%f", &m_BossCollFile.aFile[nCnt].aBase[nCntColl].fRadius);		// ���a��ǂݍ���

									fscanf(pFile, "%s", &aString[0]);				// WEAK ��ǂݍ��� (�s�v)
									fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%s", &aString[0]);				// ��_�󋵂�ǂݍ���

									if (strcmp(&aString[0], "TRUE") == 0)
									{ // ��_�󋵂� true �̏ꍇ

										m_BossCollFile.aFile[nCnt].aBase[nCntColl].bWeakness = true;
									}
									else
									{ // ��L�ȊO

										m_BossCollFile.aFile[nCnt].aBase[nCntColl].bWeakness = false;
									}
								}
							}
						} while (strcmp(&aString[0], "END_SET_COLL") != 0);		// �ǂݍ��񂾕����� END_SET_WALL �ł͂Ȃ��ꍇ���[�v
					}
				}
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_BossCollFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �Ŕ̃��[�h����
//===========================================
HRESULT CFile::LoadSignboard(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_SignboardFile.nNumData = 0;		// ����
	m_SignboardFile.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SIGNBOARD_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_SIGNBOARD") == 0)
			{ // �ǂݍ��񂾕����� SET_SIGNBOARD �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_SIGNBOARD �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].pos.x,
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].pos.y,
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].pos.z);		// �ʒu��ǂݍ���
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].rot.x,
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].rot.y,
							&m_SignboardFile.aFile[m_SignboardFile.nNumData].rot.z);	// �g�嗦��ǂݍ���
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_SignboardFile.aFile[m_SignboardFile.nNumData].type);		// ��ނ�ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_SIGNBOARD") != 0);		// �ǂݍ��񂾕����� END_SET_SIGNBOARD �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_SignboardFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_SignboardFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// ���D�̃��[�h����
//===========================================
HRESULT CFile::LoadBalloon(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_BalloonFile.nNumData = 0;			// ����
	m_BalloonFile.bSuccess = false;		// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(BALLOON_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_BALLOON") == 0)
			{ // �ǂݍ��񂾕����� SET_BALLOON �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_BALLOON �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_BalloonFile.pos[m_BalloonFile.nNumData].x,
							&m_BalloonFile.pos[m_BalloonFile.nNumData].y,
							&m_BalloonFile.pos[m_BalloonFile.nNumData].z);		// �ʒu��ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_BALLOON") != 0);		// �ǂݍ��񂾕����� END_SET_BALLOON �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_BalloonFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_BalloonFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �G�̌o�H�̃��[�h����
//===========================================
HRESULT CFile::LoadEnemyRoute(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_EnemyRouteFile.nNumData = 0;		// ����
	m_EnemyRouteFile.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE* pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(ENEMYROUTE_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_ENEMYROUTE") == 0)
			{ // �ǂݍ��񂾕����� SET_ENEMYROUTE �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_ENEMYROUTE �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM") == 0)
					{ // �ǂݍ��񂾕����� NUM �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].nNum);	// �ʒu�̑�����ǂݍ���

						for (int nCntRoute = 0; nCntRoute < m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].nNum; nCntRoute++)
						{
							fscanf(pFile, "%s", &aString[0]);				// POS ��ǂݍ��� (�s�v)
							fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
							fscanf(pFile, "%f%f%f",
								&m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].pos[nCntRoute].x,
								&m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].pos[nCntRoute].y,
								&m_EnemyRouteFile.aFile[m_EnemyRouteFile.nNumData].pos[nCntRoute].z);	// �ʒu��ǂݍ���
						}
					}

				} while (strcmp(&aString[0], "END_SET_ENEMYROUTE") != 0);		// �ǂݍ��񂾕����� END_SET_ENEMYROUTE �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_EnemyRouteFile.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_EnemyRouteFile.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}