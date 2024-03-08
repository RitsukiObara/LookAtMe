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

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define RANKING_BIN			"data\\BIN\\Ranking.bin"		// �����L���O�̃e�L�X�g
#define ENEMY_TXT			"data\\TXT\\Enemy.txt"			// �G�̃e�L�X�g
#define COIN_TXT			"data\\TXT\\Coin.txt"			// �R�C���̃e�L�X�g
#define GOLDBONE_TXT		"data\\TXT\\GoldBone.txt"		// ���̍��̃e�L�X�g
#define TREE_TXT			"data\\TXT\\Tree.txt"			// �؂̃e�L�X�g
#define ROCK_TXT			"data\\TXT\\Rock.txt"			// ��̃e�L�X�g
#define BLOCK_TXT			"data\\TXT\\Block.txt"			// �u���b�N�̃e�L�X�g
#define BANGFLOWER_TXT		"data\\TXT\\BangFlower.txt"		// ���e�Ԃ̃e�L�X�g
#define WALL_TXT			"data\\TXT\\Wall.txt"			// �ǂ̃e�L�X�g

#define GOLDBONE_NUM		(3)		// ���̍��̐�

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
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		m_RankingInfo.aRank[nCntRank] = 0;				// �����L���O�̒l
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

	case CFile::TYPE_ENEMY:

		// �G�̃Z�[�u����
		if (FAILED(SaveEnemy()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_COIN:

		// �R�C���̃Z�[�u����
		if (FAILED(SaveCoin()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_GOLDBONE:

		// ���̍��̃Z�[�u����
		if (FAILED(SaveGoldBone()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_TREE:

		// �؂̃Z�[�u����
		if (FAILED(SaveTree()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_ROCK:

		// ��̃Z�[�u����
		if (FAILED(SaveRock()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BLOCK:

		// �u���b�N�̃Z�[�u����
		if (FAILED(SaveBlock()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_BANGFLOWER:

		// �����Ԃ̃Z�[�u����
		if (FAILED(SaveBangFlower()))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case CFile::TYPE_WALL:

		// �ǂ̃Z�[�u����
		if (FAILED(SaveWall()))
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
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++, pRank++)
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
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		m_RankingInfo.aRank[nCntRank] = 0;				// �����L���O�̒l
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

	// �����󋵂��N���A����
	m_RankingInfo.bSuccess = false;		// �����L���O
	m_EnemyFile.bSuccess = false;		// �G�̏��
	m_CoinFile.bSuccess = false;		// �R�C���̏��
	m_GoldBoneFile.bSuccess = false;	// ���̍��̏��
	m_TreeFile.bSuccess = false;		// �؂̏��
	m_RockFile.bSuccess = false;		// ��̏��
	m_BlockFile.bSuccess = false;		// �u���b�N�̏��
	m_BangFlowerFile.bSuccess = false;	// ���e�Ԃ̏��

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
	FILE *pFile;												// �t�@�C���|�C���^��錾
	m_RankingInfo.bSuccess = false;								// ������

	// �t�@�C�����J��
	pFile = fopen(RANKING_BIN, "wb");			// �o�C�i���t�@�C���ɏ������ނ��߂ɊJ��

	// �t�@�C�����r����
	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		// �t�@�C�����琔�l�������o��
		fwrite(&m_RankingInfo.aRank[0], sizeof(int), MAX_RANKING, pFile);

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
// �G�̃Z�[�u����
//===========================================
HRESULT CFile::SaveEnemy(void)
{
	// ���[�J���ϐ��錾
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pEnemy = nullptr;		// �擪�̓G
	CEnemy* pEnemyEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// �|�C���^��錾
	FILE* pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(ENEMY_TXT, "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// while������
		if (list.IsEmpty() == false)
		{ // �󔒂���Ȃ��ꍇ

			// �擪�̒l���擾����
			pEnemy = list.GetTop();

			// �����̒l���擾����
			pEnemyEnd = list.GetEnd();

			while (true)
			{ // �������[�v

				// ���������������
				fprintf(pFile, "SET_ENEMY\n");		// �G�̐ݒ����������

				fprintf(pFile, "\tPOS = ");			// �ʒu�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pEnemy->GetPos().x, pEnemy->GetPos().y, pEnemy->GetPos().z);			// �ʒu����������

				fprintf(pFile, "\tROT = ");			// �����̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pEnemy->GetRot().x, pEnemy->GetRot().y, pEnemy->GetRot().z);			// ��������������

				fprintf(pFile, "\tTYPE = ");		// ��ނ̐ݒ����������
				fprintf(pFile, "%d\n", pEnemy->GetType());			// ��ނ���������

				// ���������������
				fprintf(pFile, "END_SET_ENEMY\n\n");		// �G�̐ݒ�̏I������������

				if (pEnemy == pEnemyEnd)
				{ // �����ɒB�����ꍇ

					// while���𔲂��o��
					break;
				}

				// ���̃I�u�W�F�N�g��������
				pEnemy = list.GetData(nIdx + 1);

				// �C���f�b�N�X�����Z����
				nIdx++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
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
// �R�C���̃Z�[�u����
//===========================================
HRESULT CFile::SaveCoin(void)
{
	// ���[�J���ϐ��錾
	CListManager<CCoin*> list = CCoin::GetList();
	CCoin* pCoin = nullptr;		// �擪�̓G
	CCoin* pCoinEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// �|�C���^��錾
	FILE* pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(COIN_TXT, "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// while������
		if (list.IsEmpty() == false)
		{ // �󔒂���Ȃ��ꍇ

			// �擪�̒l���擾����
			pCoin = list.GetTop();

			// �����̒l���擾����
			pCoinEnd = list.GetEnd();

			while (true)
			{ // �������[�v

				// ���������������
				fprintf(pFile, "SET_COIN\n");		// �R�C���̐ݒ����������

				fprintf(pFile, "\tPOS = ");			// �ʒu�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pCoin->GetPos().x, pCoin->GetPos().y, pCoin->GetPos().z);			// �ʒu����������

				// ���������������
				fprintf(pFile, "END_SET_COIN\n\n");	// �R�C���̐ݒ�̏I������������

				if (pCoin == pCoinEnd)
				{ // �����ɒB�����ꍇ

					// while���𔲂��o��
					break;
				}

				// ���̃I�u�W�F�N�g��������
				pCoin = list.GetData(nIdx + 1);

				// �C���f�b�N�X�����Z����
				nIdx++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
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
// ���̍��̃Z�[�u����
//===========================================
HRESULT CFile::SaveGoldBone(void)
{
	// ���[�J���ϐ��錾
	CListManager<CGoldBone*> list = CGoldBone::GetList();
	CGoldBone* pGoldBone = nullptr;		// �擪�̓G
	CGoldBone* pGoldBoneEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// �|�C���^��錾
	FILE* pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(GOLDBONE_TXT, "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// while������
		if (list.IsEmpty() == false)
		{ // �󔒂���Ȃ��ꍇ

			// �擪�̒l���擾����
			pGoldBone = list.GetTop();

			// �����̒l���擾����
			pGoldBoneEnd = list.GetEnd();

			while (true)
			{ // �������[�v

				// ���������������
				fprintf(pFile, "SET_GOLDBONE\n");		// ���̍��̐ݒ����������

				fprintf(pFile, "\tPOS = ");				// �ʒu�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pGoldBone->GetPos().x, pGoldBone->GetPos().y, pGoldBone->GetPos().z);			// �ʒu����������

				// ���������������
				fprintf(pFile, "END_SET_GOLDBONE\n\n");	// ���̍��̐ݒ�̏I������������

				if (pGoldBone == pGoldBoneEnd)
				{ // �����ɒB�����ꍇ

					// while���𔲂��o��
					break;
				}

				// ���̃I�u�W�F�N�g��������
				pGoldBone = list.GetData(nIdx + 1);

				// �C���f�b�N�X�����Z����
				nIdx++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
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
// �؂̃Z�[�u����
//===========================================
HRESULT CFile::SaveTree(void)
{
	// ���[�J���ϐ��錾
	CListManager<CTree*> list = CTree::GetList();
	CTree* pTree = nullptr;		// �擪�̓G
	CTree* pTreeEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// �|�C���^��錾
	FILE* pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(TREE_TXT, "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// while������
		if (list.IsEmpty() == false)
		{ // �󔒂���Ȃ��ꍇ

			// �擪�̒l���擾����
			pTree = list.GetTop();

			// �����̒l���擾����
			pTreeEnd = list.GetEnd();

			while (true)
			{ // �������[�v

				// ���������������
				fprintf(pFile, "SET_TREE\n");		// �؂̐ݒ����������

				fprintf(pFile, "\tPOS = ");				// �ʒu�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pTree->GetPos().x, pTree->GetPos().y, pTree->GetPos().z);			// �ʒu����������

				fprintf(pFile, "\tROT = ");				// �����̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pTree->GetRot().x, pTree->GetRot().y, pTree->GetRot().z);			// ��������������

				fprintf(pFile, "\tTYPE = ");		// ��ނ̐ݒ����������
				fprintf(pFile, "%d\n", pTree->GetType());			// ��ނ���������

				// ���������������
				fprintf(pFile, "END_SET_TREE\n\n");	// �؂̐ݒ�̏I������������

				if (pTree == pTreeEnd)
				{ // �����ɒB�����ꍇ

					// while���𔲂��o��
					break;
				}

				// ���̃I�u�W�F�N�g��������
				pTree = list.GetData(nIdx + 1);

				// �C���f�b�N�X�����Z����
				nIdx++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
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
// ��̃Z�[�u����
//===========================================
HRESULT CFile::SaveRock(void)
{
	// ���[�J���ϐ��錾
	CListManager<CRock*> list = CRock::GetList();
	CRock* pRock = nullptr;		// �擪�̓G
	CRock* pRockEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// �|�C���^��錾
	FILE* pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(ROCK_TXT, "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// while������
		if (list.IsEmpty() == false)
		{ // �󔒂���Ȃ��ꍇ

			// �擪�̒l���擾����
			pRock = list.GetTop();

			// �����̒l���擾����
			pRockEnd = list.GetEnd();

			while (true)
			{ // �������[�v

				// ���������������
				fprintf(pFile, "SET_ROCK\n");		// �؂̐ݒ����������

				fprintf(pFile, "\tPOS = ");				// �ʒu�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pRock->GetPos().x, pRock->GetPos().y, pRock->GetPos().z);			// �ʒu����������

				fprintf(pFile, "\tROT = ");				// �����̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pRock->GetRot().x, pRock->GetRot().y, pRock->GetRot().z);			// ��������������

				fprintf(pFile, "\tSCALE = ");			// �g�嗦�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pRock->GetScale().x, pRock->GetScale().y, pRock->GetScale().z);		// �g�嗦����������

				fprintf(pFile, "\tTYPE = ");		// ��ނ̐ݒ����������
				fprintf(pFile, "%d\n", pRock->GetType());			// ��ނ���������

				// ���������������
				fprintf(pFile, "END_SET_ROCK\n\n");	// �؂̐ݒ�̏I������������

				if (pRock == pRockEnd)
				{ // �����ɒB�����ꍇ

					// while���𔲂��o��
					break;
				}

				// ���̃I�u�W�F�N�g��������
				pRock = list.GetData(nIdx + 1);

				// �C���f�b�N�X�����Z����
				nIdx++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
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
// �u���b�N�̃Z�[�u����
//===========================================
HRESULT CFile::SaveBlock(void)
{
	// ���[�J���ϐ��錾
	CListManager<CBlock*> list = CBlock::GetList();
	CBlock* pBlock = nullptr;		// �擪�̓G
	CBlock* pBlockEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// �|�C���^��錾
	FILE* pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(BLOCK_TXT, "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// while������
		if (list.IsEmpty() == false)
		{ // �󔒂���Ȃ��ꍇ

			// �擪�̒l���擾����
			pBlock = list.GetTop();

			// �����̒l���擾����
			pBlockEnd = list.GetEnd();

			while (true)
			{ // �������[�v

				// ���������������
				fprintf(pFile, "SET_BLOCK\n");		// �؂̐ݒ����������

				fprintf(pFile, "\tPOS = ");				// �ʒu�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetPos().x, pBlock->GetPos().y, pBlock->GetPos().z);			// �ʒu����������

				fprintf(pFile, "\tSCALE = ");			// �g�嗦�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetScale().x, pBlock->GetScale().y, pBlock->GetScale().z);		// �g�嗦����������

				// ���������������
				fprintf(pFile, "END_SET_BLOCK\n\n");	// �؂̐ݒ�̏I������������

				if (pBlock == pBlockEnd)
				{ // �����ɒB�����ꍇ

					// while���𔲂��o��
					break;
				}

				// ���̃I�u�W�F�N�g��������
				pBlock = list.GetData(nIdx + 1);

				// �C���f�b�N�X�����Z����
				nIdx++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
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
// ���e�Ԃ̃Z�[�u����
//===========================================
HRESULT CFile::SaveBangFlower(void)
{
	// ���[�J���ϐ��錾
	CListManager<CBangFlower*> list = CBangFlower::GetList();
	CBangFlower* pBangFlower = nullptr;		// �擪�̓G
	CBangFlower* pBangFlowerEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// �|�C���^��錾
	FILE* pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(BANGFLOWER_TXT, "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// while������
		if (list.IsEmpty() == false)
		{ // �󔒂���Ȃ��ꍇ

			// �擪�̒l���擾����
			pBangFlower = list.GetTop();

			// �����̒l���擾����
			pBangFlowerEnd = list.GetEnd();

			while (true)
			{ // �������[�v

				// ���������������
				fprintf(pFile, "SET_BANGFLOWER\n");		// �؂̐ݒ����������

				fprintf(pFile, "\tPOS = ");				// �ʒu�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pBangFlower->GetPos().x, pBangFlower->GetPos().y, pBangFlower->GetPos().z);			// �ʒu����������

				fprintf(pFile, "\tROT = ");				// �����̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pBangFlower->GetRot().x, pBangFlower->GetRot().y, pBangFlower->GetRot().z);			// ��������������

				// ���������������
				fprintf(pFile, "END_SET_BANGFLOWER\n\n");	// �؂̐ݒ�̏I������������

				if (pBangFlower == pBangFlowerEnd)
				{ // �����ɒB�����ꍇ

					// while���𔲂��o��
					break;
				}

				// ���̃I�u�W�F�N�g��������
				pBangFlower = list.GetData(nIdx + 1);

				// �C���f�b�N�X�����Z����
				nIdx++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
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
// �ǂ̃Z�[�u����
//===========================================
HRESULT CFile::SaveWall(void)
{
	// ���[�J���ϐ��錾
	CListManager<CWall*> list = CWall::GetList();
	CWall* pWall = nullptr;		// �擪�̓G
	CWall* pWallEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// �|�C���^��錾
	FILE* pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(WALL_TXT, "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// while������
		if (list.IsEmpty() == false)
		{ // �󔒂���Ȃ��ꍇ

			// �擪�̒l���擾����
			pWall = list.GetTop();

			// �����̒l���擾����
			pWallEnd = list.GetEnd();

			while (true)
			{ // �������[�v

				// ���������������
				fprintf(pFile, "SET_WALL\n");		// �؂̐ݒ����������

				fprintf(pFile, "\tPOS = ");				// �ʒu�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pWall->GetPos().x, pWall->GetPos().y, pWall->GetPos().z);			// �ʒu����������

				fprintf(pFile, "\tSCALE = ");			// �g�嗦�̐ݒ����������
				fprintf(pFile, "%.1f %.1f %.1f\n", pWall->GetScale().x, pWall->GetScale().y, pWall->GetScale().z);		// �g�嗦����������

				fprintf(pFile, "\tTYPE = ");			// ��ނ̐ݒ����������
				fprintf(pFile, "%d\n", pWall->GetType());		// ��ނ���������

				fprintf(pFile, "\tROTTYPE = ");			// �����̎�ނ̐ݒ����������
				fprintf(pFile, "%d\n", pWall->GetRotType());	// �����̎�ނ���������

				// ���������������
				fprintf(pFile, "END_SET_WALL\n\n");	// �؂̐ݒ�̏I������������

				if (pWall == pWallEnd)
				{ // �����ɒB�����ꍇ

					// while���𔲂��o��
					break;
				}

				// ���̃I�u�W�F�N�g��������
				pWall = list.GetData(nIdx + 1);

				// �C���f�b�N�X�����Z����
				nIdx++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
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
// �����L���O�̃��[�h����
//===========================================
HRESULT CFile::LoadRanking(void)
{
	FILE *pFile;						// �t�@�C���|�C���^��錾
	m_RankingInfo.bSuccess = false;		// ������

	// �t�@�C�����J��
	pFile = fopen(RANKING_BIN, "rb");			// �o�C�i���t�@�C������ǂݍ��ނ��߂ɊJ��

	// �t�@�C�����r����
	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		// �t�@�C�����琔�l��ǂݍ���
		fread(&m_RankingInfo.aRank[0], sizeof(int), MAX_RANKING, pFile);

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

		for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
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
	FILE *pFile;						// �t�@�C���|�C���^

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