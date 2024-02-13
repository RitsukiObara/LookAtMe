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
#include "obstacle.h"
#include "block.h"
#include "camera_range.h"
#include "Item.h"
#include "item_generator.h"
#include "enemy.h"
#include "time.h"
#include "player.h"
#include "balloon.h"
#include "goal.h"

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CFile::c_apFileName[TYPE_MAX] =				// �t�@�C���̖��O
{
	"data/TXT/Enemy.txt",				// �G�̃e�L�X�g
	"data/TXT/Block.txt",				// �u���b�N�̃e�L�X�g
	"data/TXT/BgObject.txt",			// �w�i�I�u�W�F�N�g�̃e�L�X�g
	"data/TXT/Obstacle.txt",			// ��Q���̃e�L�X�g
	"data/TXT/Item.txt",				// �A�C�e���̃e�L�X�g
	"data/TXT/CameraRange.txt",			// �J�����͈̔͂̃e�L�X�g
	"data/TXT/Result.txt",				// ���U���g��ʂ̃e�L�X�g
	"data/BIN/Ranking.bin",				// �����L���O��ʂ̃o�C�i��
	"data/TXT/Game.txt",				// �Q�[����ʂ̃e�L�X�g
	"data/TXT/Tutorial.txt",			// �`���[�g���A����ʂ̃e�L�X�g
};

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
	for (int nCnt = 0; nCnt < MAX_FILE_DATA; nCnt++)
	{
		// �u���b�N���
		m_BlockInfo.info[nCnt] = {};

		// ��Q�����
		m_ObstacleInfo.info[nCnt] = {};

		// �A�C�e�����
		m_ItemInfo.pos[nCnt] = {};

		// �J�����͈̔͏��
		m_CameraInfo.info[nCnt] = {};

		// �`���[�g���A���̏��
		m_TutorialInfo.BlockInfo.info[nCnt] = {};	// �u���b�N�̏��
		m_TutorialInfo.BgObjInfo.info[nCnt] = {};	// �w�i�I�u�W�F�N�g�̏��
		m_TutorialInfo.ObstaInfo.info[nCnt] = {};	// ��Q���̏��
		m_TutorialInfo.BalloonInfo.info[nCnt] = {};	// �����o���̏��
	}

	for (int nCntRsl = 0; nCntRsl < CResult::RANK_MAX; nCntRsl++)
	{
		// ���U���g�̃����N
		m_ResultInfo.aRankTime[nCntRsl] = 0;		// ���Ԃ̃����N�̊
		m_ResultInfo.aRankScore[nCntRsl] = 0;		// �X�R�A�̃����N�̊
	}

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// �����L���O�̋L�^
		m_RankingInfo.aRank[nCntRank] = 0;
	}

	for (int nCntTuto = 0; nCntTuto < CBalloon::TYPE_MAX; nCntTuto++)
	{
		// �����o���̎�ނ��Ƃ̏��
		m_aBalloonData[nCntTuto] = {};
	}

	// �Q�[���̏��
	m_GameInfo = {};

	// �`���[�g���A���̏��
	m_TutorialInfo.GoalPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �S�[���̈ʒu
	m_TutorialInfo.PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �v���C���[�̈ʒu

	// �f�[�^�̑���
	m_BlockInfo.nNumData = 0;						// �u���b�N�̑���
	m_BlockInfo.bSuccess = false;					// ������
	m_ObstacleInfo.nNumData = 0;					// ��Q���̑���
	m_ObstacleInfo.bSuccess = false;				// ������
	m_ItemInfo.nNumItemData = 0;					// �A�C�e���̑���
	m_ItemInfo.nNumItemGeneData = 0;				// �A�C�e��������̑���
	m_ItemInfo.bSuccess = false;					// ������
	m_CameraInfo.nNumData = 0;						// �J�����͈̔͂̑���
	m_CameraInfo.bSuccess = false;					// ������
	m_ResultInfo.bSuccess = false;					// ������
	m_RankingInfo.bSuccess = false;					// ������
	m_GameInfo.bSuccess = false;					// ������
	m_TutorialInfo.BlockInfo.nNumData = 0;			// �u���b�N�̑���
	m_TutorialInfo.BgObjInfo.nNumData = 0;			// �w�i�I�u�W�F�N�g�̑���
	m_TutorialInfo.ObstaInfo.nNumData = 0;			// ��Q���̑���
	m_TutorialInfo.BalloonInfo.nNumData = 0;		// �����o���̑���
	m_TutorialInfo.bSuccess = false;				// ������
}

//===========================================
// �f�X�g���N�^
//===========================================
CFile::~CFile()
{

}

//===========================================
// ���[�h����
//===========================================
HRESULT CFile::Load(TYPE type)
{
	switch (type)
	{
	case TYPE_ENEMY:	// �G�̃t�@�C��

		// �G�̃��[�h����
		LoadEnemy();

		break;

	case TYPE_BLOCK:	// �u���b�N�̃t�@�C��

		// �u���b�N�̃��[�h����
		if (FAILED(LoadBlock()))
		{ // �u���b�N�̃��[�h�����s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_OBSTACLE:			// ��Q��

		// ��Q���̃��[�h����
		if (FAILED(LoadObstacle()))
		{ // ��Q���̃��[�h�����s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_ITEM:				// �A�C�e��

		// �A�C�e���̃��[�h����
		if (FAILED(LoadItem()))
		{ // �A�C�e���̃��[�h���������s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_CAMERA:

		// �J�����͈̔͂̃��[�h����
		if (FAILED(LoadCameraRange()))
		{ // �J�����͈̔͂̃��[�h���������s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_RESULT:			// ���U���g

		// ���U���g�̃��[�h����
		if (FAILED(LoadResultFile()))
		{ // ���U���g�̃��[�h�����s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_RANKING:			// �����L���O

		// �����L���O�̃��[�h����
		if (FAILED(LoadRankingFile()))
		{ // �����L���O�̃��[�h�����s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_GAME:				// �Q�[��

		if (FAILED(LoadGameFile()))
		{ // �Q�[���̃��[�h�����s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_TUTORIAL:

		if (FAILED(LoadTutorialFile()))
		{ // �`���[�g���A���̃��[�h�����s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;
	}

	// ���ʂ�Ԃ�
	return S_OK;
}

//===========================================
// �Z�[�u����
//===========================================
HRESULT CFile::Save(TYPE type)
{
	switch (type)
	{
	case TYPE_ENEMY:	// �G�̃t�@�C��

		break;

	case TYPE_BLOCK:	// �u���b�N�̃t�@�C��

		// �u���b�N�̃Z�[�u����
		if (FAILED(SaveBlock()))
		{ // �Z�[�u�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_OBSTACLE:			// ��Q��

		// ��Q���̃Z�[�u����
		if (FAILED(SaveObstacle()))
		{ // �Z�[�u�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_ITEM:				// �A�C�e��

		// �A�C�e���̃Z�[�u����
		if (FAILED(SaveItem()))
		{ // �Z�[�u�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_CAMERA:

		// �J�����͈̔͂̃Z�[�u����
		if (FAILED(SaveCameraRange()))
		{ // �Z�[�u�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;

	case TYPE_RANKING:			// �����L���O

		// �����L���O�̃Z�[�u����
		if (FAILED(SaveRankingFile()))
		{ // �Z�[�u�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}

		break;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �u���b�N�̐ݒu����
//===========================================
void CFile::SetBlock(void)
{
	if (m_BlockInfo.bSuccess == true)
	{ // ���[�h�ɐ������Ă����ꍇ

		for (int nCnt = 0; nCnt < m_BlockInfo.nNumData; nCnt++)
		{
			// �u���b�N�̐�������
			CBlock::Create
			(
				m_BlockInfo.info[nCnt].info.pos,
				m_BlockInfo.info[nCnt].info.rot,
				m_BlockInfo.info[nCnt].info.scale,
				&m_BlockInfo.info[nCnt].aMat[0],
				m_BlockInfo.info[nCnt].type,
				m_BlockInfo.info[nCnt].bCliffRight,
				m_BlockInfo.info[nCnt].bCliffLeft
			);
		}
	}
}

//===========================================
// ��Q���̐ݒu����
//===========================================
void CFile::SetObstacle(void)
{
	if (m_ObstacleInfo.bSuccess == true)
	{ // ���[�h�ɐ������Ă����ꍇ

		for (int nCnt = 0; nCnt < m_ObstacleInfo.nNumData; nCnt++)
		{
			// �u���b�N�̐�������
			CObstacle::Create
			(
				m_ObstacleInfo.info[nCnt].pos,
				m_ObstacleInfo.info[nCnt].type
			);
		}
	}
}

//===========================================
// �A�C�e���̐ݒu����
//===========================================
void CFile::SetItem(void)
{
	if (m_ItemInfo.bSuccess == true)
	{ // ���[�h�ɐ������Ă����ꍇ

		for (int nCntItem = 0; nCntItem < m_ItemInfo.nNumItemData; nCntItem++)
		{
			// �A�C�e���̐�������
			CItem::Create
			(
				m_ItemInfo.pos[nCntItem],
				CItem::STATE_NONE
			);
		}

		for (int nCntGene = 0; nCntGene < m_ItemInfo.nNumItemGeneData; nCntGene++)
		{
			// �A�C�e��������̐�������
			CItemGenerator::Create
			(
				m_ItemInfo.pos[nCntGene + m_ItemInfo.nNumItemData]
			);
		}
	}
}

//===========================================
// �J�����͈̔͂̐ݒu����
//===========================================
void CFile::SetCamera(void)
{
	if (m_CameraInfo.bSuccess == true)
	{ // ���[�h�ɐ������Ă����ꍇ

		for (int nCnt = 0; nCnt < m_CameraInfo.nNumData; nCnt++)
		{
			// �u���b�N�̐�������
			CCameraRange::Create
			(
				m_CameraInfo.info[nCnt].pos,
				m_CameraInfo.info[nCnt].size,
				m_CameraInfo.info[nCnt].type
			);
		}
	}
}

//===========================================
// ���U���g���̐ݒu����
//===========================================
CFile::SResultFile CFile::GetResultInfo(void)
{
	if (m_ResultInfo.bSuccess == false)
	{ // �������Ă��Ȃ��ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "���U���g�̏�񂪓ǂݍ��߂Ă܂���I", "�x���I", MB_ICONWARNING);
	}

	// ���U���g�̏���Ԃ�
	return m_ResultInfo;
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
CFile::SRankingFile CFile::GetRankingInfo(void)
{
	// �����L���O�̏���Ԃ�
	return m_RankingInfo;
}

//===========================================
// �Q�[����ʂ̐ݒ菈��
//===========================================
CFile::SGameFile CFile::GetGameInfo(void)
{
	if (m_GameInfo.bSuccess == false)
	{ // �������Ă��Ȃ��ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�Q�[���̏�񂪓ǂݍ��߂Ă܂���I", "�x���I", MB_ICONWARNING);
	}

	// �Q�[���̏���Ԃ�
	return m_GameInfo;
}

//===========================================
// �`���[�g���A���̐ݒ菈��
//===========================================
void CFile::SetTutorial(void)
{
	if (m_TutorialInfo.bSuccess == true)
	{ // �`���[�g���A���̓ǂݍ��݂ɐ������Ă����ꍇ

		for (int nCntBal = 0; nCntBal < m_TutorialInfo.BalloonInfo.nNumData; nCntBal++)
		{
			// �����o���̐�������
			CBalloon::Create(m_TutorialInfo.BalloonInfo.info[nCntBal].pos, m_TutorialInfo.BalloonInfo.info[nCntBal].type);
		}

		for (int nCntBlock = 0; nCntBlock < m_TutorialInfo.BlockInfo.nNumData; nCntBlock++)
		{
			// �u���b�N�̐�������
			CBlock::Create
			(
				m_TutorialInfo.BlockInfo.info[nCntBlock].info.pos,
				m_TutorialInfo.BlockInfo.info[nCntBlock].info.rot,
				m_TutorialInfo.BlockInfo.info[nCntBlock].info.scale,
				m_TutorialInfo.BlockInfo.info[nCntBlock].aMat,
				m_TutorialInfo.BlockInfo.info[nCntBlock].type,
				m_TutorialInfo.BlockInfo.info[nCntBlock].bCliffRight,
				m_TutorialInfo.BlockInfo.info[nCntBlock].bCliffLeft
			);
		}

		for (int nCntObsta = 0; nCntObsta < m_TutorialInfo.ObstaInfo.nNumData; nCntObsta++)
		{
			// ��Q���̐�������
			CObstacle::Create
			(
				m_TutorialInfo.ObstaInfo.info[nCntObsta].pos,
				m_TutorialInfo.ObstaInfo.info[nCntObsta].type
			);
		}

		if (CScene::GetPlayer() != nullptr)
		{ // �v���C���[�� NULL ����Ȃ��ꍇ

			// �v���C���[�̈ʒu�ݒ菈��
			CScene::GetPlayer()->SetPos(m_TutorialInfo.PlayerPos);
		}

		// �S�[����ݒu����
		CGoal::Create(m_TutorialInfo.GoalPos, CScene::MODE_TUTORIAL);
	}
	else
	{ // ��L�ȊO

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�`���[�g���A���̏�񂪓ǂݍ��߂Ă܂���I", "�x���I", MB_ICONWARNING);
	}
}

//===========================================
// �����o���̏��̎擾����
//===========================================
CFile::SBalloonData CFile::GetBalloon(const CBalloon::TYPE type)
{
	// �����o���̏���Ԃ�
	return m_aBalloonData[type];
}

//===========================================
// ����������
//===========================================
HRESULT CFile::Init(void)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < MAX_FILE_DATA; nCnt++)
	{
		// �u���b�N���
		m_BlockInfo.info[nCnt] = {};

		// ��Q�����
		m_ObstacleInfo.info[nCnt] = {};

		// �A�C�e�����
		m_ItemInfo.pos[nCnt] = {};

		// �J�����͈̔͏��
		m_CameraInfo.info[nCnt] = {};
	}

	for (int nCntRsl = 0; nCntRsl < CResult::RANK_MAX; nCntRsl++)
	{
		// ���U���g�̃����N
		m_ResultInfo.aRankTime[nCntRsl] = 0;		// ���Ԃ̃����N�̊
		m_ResultInfo.aRankScore[nCntRsl] = 0;		// �X�R�A�̃����N�̊
	}

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// �����L���O�̋L�^
		m_RankingInfo.aRank[nCntRank] = 0;
	}

	// �Q�[���̏��
	m_GameInfo = {};

	// �f�[�^�̑���
	m_BlockInfo.nNumData = 0;			// �u���b�N�̑���
	m_BlockInfo.bSuccess = false;		// ������
	m_ObstacleInfo.nNumData = 0;		// ��Q���̑���
	m_ObstacleInfo.bSuccess = false;	// ������
	m_ItemInfo.nNumItemData = 0;		// �A�C�e���̑���
	m_ItemInfo.nNumItemGeneData = 0;	// �A�C�e��������̑���
	m_CameraInfo.nNumData = 0;			// �J�����͈̔͂̑���
	m_CameraInfo.bSuccess = false;		// ������
	m_ItemInfo.bSuccess = false;		// ������
	m_ResultInfo.bSuccess = false;		// ������
	m_RankingInfo.bSuccess = false;		// ������
	m_GameInfo.bSuccess = false;		// ������

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CFile::Uninit(void)
{
	// �S�Ă̒l�̔j��
	for (int nCnt = 0; nCnt < MAX_FILE_DATA; nCnt++)
	{
		// ���̏I��
		m_BlockInfo.info[nCnt].info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		m_BlockInfo.info[nCnt].info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		m_BlockInfo.info[nCnt].info.scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �g�嗦
		m_BlockInfo.info[nCnt].type = CBlock::TYPE_DIRT;						// �u���b�N�̎��

		for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
		{
			ZeroMemory(&m_BlockInfo.info[nCnt].aMat[nCntMat], sizeof(D3DXMATERIAL));		// �}�e���A���f�[�^
		}

		// ���̏I��
		m_ObstacleInfo.info[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		m_ObstacleInfo.info[nCnt].type = CObstacle::TYPE_HYDRANT;			// �u���b�N�̎��

		// ���̏I��
		m_ItemInfo.pos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu

		m_CameraInfo.info[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		m_CameraInfo.info[nCnt].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y
		m_CameraInfo.info[nCnt].type = CCamera::TYPE_NONE;					// ���
	}

	for (int nCntRsl = 0; nCntRsl < CResult::RANK_MAX; nCntRsl++)
	{
		// ���U���g�̃����N
		m_ResultInfo.aRankTime[nCntRsl] = 0;		// ���Ԃ̃����N�̊
		m_ResultInfo.aRankScore[nCntRsl] = 0;		// �X�R�A�̃����N�̊
	}

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// �����L���O�̋L�^
		m_RankingInfo.aRank[nCntRank] = 0;
	}

	// �f�[�^�̑���
	m_BlockInfo.nNumData = 0;			// �u���b�N�̑���
	m_BlockInfo.bSuccess = false;		// ������
	m_ObstacleInfo.nNumData = 0;		// ��Q���̑���
	m_ObstacleInfo.bSuccess = false;	// ������
	m_ItemInfo.nNumItemData = 0;		// �A�C�e���̑���
	m_ItemInfo.nNumItemGeneData = 0;	// �A�C�e��������̑���
	m_ItemInfo.bSuccess = false;		// ������
	m_CameraInfo.nNumData = 0;			// �J�����͈̔͂̑���
	m_CameraInfo.bSuccess = false;		// ������
	m_ResultInfo.bSuccess = false;		// ������
	m_RankingInfo.bSuccess = false;		// ������
}

//===========================================
// �X�V����
//===========================================
void CFile::Update(void)
{
	
}

//===========================================
// �`�揈��
//===========================================
void CFile::Draw(void)
{
	
}

//===========================================
// �G�̃��[�h����
//===========================================
HRESULT CFile::LoadEnemy(void)
{
	//// �ϐ���錾
	//int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	//char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	//m_BlockInfo[TYPE_ENEMY].nNumData = 0;	// ����������������

	//// �|�C���^��錾
	//FILE *pFile;						// �t�@�C���|�C���^

	//// �t�@�C����ǂݍ��݌`���ŊJ��
	//pFile = fopen(c_apFileName[TYPE_ENEMY], "r");

	//if (pFile != NULL)
	//{ // �t�@�C�����J�����ꍇ

	//	do
	//	{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

	//		// �t�@�C�����當�����ǂݍ���
	//		nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

	//		if (strcmp(&aString[0], "SET_ENEMY") == 0)
	//		{ // �ǂݍ��񂾕����� SET_ENEMY �̏ꍇ

	//			if (m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData] == NULL)
	//			{ // �|�C���^�� NULL �̏ꍇ

	//				// ���������m�ۂ���
	//				m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData] = new SInfo;
	//			}

	//			// ���̏�����
	//			m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	//			m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	//			m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y
	//			m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->type = 0;									// ���

	//			do
	//			{ // �ǂݍ��񂾕����� END_SET_ENEMY �ł͂Ȃ��ꍇ���[�v

	//				// �t�@�C�����當�����ǂݍ���
	//				fscanf(pFile, "%s", &aString[0]);

	//				if (strcmp(&aString[0], "POS") == 0)
	//				{ // �ǂݍ��񂾕����� POS �̏ꍇ

	//					fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
	//					fscanf(pFile, "%f%f%f", 
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->pos.x, 
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->pos.y, 
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->pos.z);		// �ʒu��ǂݍ���
	//				}
	//				else if (strcmp(&aString[0], "ROT") == 0)
	//				{ // �ǂݍ��񂾕����� ROT �̏ꍇ

	//					fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
	//					fscanf(pFile, "%f%f%f",
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->rot.x,
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->rot.y,
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->rot.z);		// ������ǂݍ���
	//				}
	//				else if (strcmp(&aString[0], "SIZE") == 0)
	//				{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

	//					fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
	//					fscanf(pFile, "%f%f%f",
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->size.x,
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->size.y,
	//						&m_BlockInfo[TYPE_ENEMY].info[m_BlockInfo[TYPE_ENEMY].nNumData]->size.z);		// �T�C�Y��ǂݍ���
	//				}
	//			} while (strcmp(&aString[0], "END_SET_ENEMY") != 0);		// �ǂݍ��񂾕����� END_SET_ENEMY �ł͂Ȃ��ꍇ���[�v

	//			// �f�[�^�̑����𑝂₷
	//			m_BlockInfo[TYPE_ENEMY].nNumData++;
	//		}
	//	} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

	//	// �t�@�C�������
	//	fclose(pFile);
	//}
	//else
	//{ // �t�@�C�����J���Ȃ������ꍇ

	//	// �G���[���b�Z�[�W�{�b�N�X
	//	MessageBox(NULL, "�G�̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

	//	// ���s��Ԃ�
	//	return E_FAIL;
	//}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �u���b�N�̃Z�[�u����
//===========================================
HRESULT CFile::SaveBlock(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBlock* pBlock = nullptr;		// �u���b�N�ւ̃|�C���^

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_BLOCK], "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// �擪�̃I�u�W�F�N�g�̏����擾����
		pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�ւ̃|�C���^�� NULL ����Ȃ������ꍇ

			if (pObj->GetType() == CObject::TYPE_BLOCK)
			{ // �u���b�N�������ꍇ

				// �e�̏����擾����
				pBlock = dynamic_cast<CBlock*>(pObj);

				if (pBlock != nullptr)
				{ // �u���b�N�� NULL ����Ȃ��ꍇ

					// ���������������
					fprintf(pFile, "SET_BLOCK\n");		// �u���b�N�̐ݒ����������

					fprintf(pFile, "\tPOS = ");			// �ʒu�̐ݒ����������
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetPos().x, pBlock->GetPos().y, pBlock->GetPos().z);			// �ʒu����������

					fprintf(pFile, "\tROT = ");			// �����̐ݒ����������
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetRot().x, pBlock->GetRot().y, pBlock->GetRot().z);			// ��������������

					fprintf(pFile, "\tSCALE = ");		// �T�C�Y�̐ݒ����������
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetScale().x, pBlock->GetScale().y, pBlock->GetScale().z);	// �g�嗦����������

					fprintf(pFile, "\tTYPE = ");		// ��ނ̐ݒ����������
					fprintf(pFile, "%d\n", pBlock->GetType());		// ��ނ���������

					fprintf(pFile, "\tMAT\n");			// �}�e���A������������

					for (int nCntMat = 0; nCntMat < (int)pBlock->GetFileData().dwNumMat; nCntMat++)
					{
						fprintf(pFile, "\tMAT%d = ", nCntMat + 1);	// �}�e���A���̔ԍ�����������
						fprintf(pFile, "%.1f %.1f %.1f %.1f\n",		// �}�e���A������������
							pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.r,
							pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.g,
							pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.b,
							pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.a
						);
					}

					fprintf(pFile, "\tRCLIFF = ");		// �E�R�̔���̐ݒ����������
					fprintf(pFile, "%s\n", c_apBooleanDisp[(int)(pBlock->GetCliffRight())]);		// �E�R�̔������������

					fprintf(pFile, "\tLCLIFF = ");		// ���R�̔���̐ݒ����������
					fprintf(pFile, "%s\n", c_apBooleanDisp[(int)(pBlock->GetCliffLeft())]);			// ���R�̔������������

					// ���������������
					fprintf(pFile, "END_SET_BLOCK\n\n");			// �u���b�N�̐ݒ�̏I������������
				}
			}

			// ���̃I�u�W�F�N�g��������
			pObj = pObj->GetNext();
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�u���b�N�̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

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
	int nEnd = 0;					// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	m_BlockInfo.nNumData = 0;		// ����������������
	m_BlockInfo.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE *pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_BLOCK], "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_BLOCK") == 0 &&
				nEnd != EOF)
			{ // �ǂݍ��񂾕����� SET_BLOCK �̏ꍇ

				// �u���b�N�̃��[�h�����̒��g
				LoadBlockProcess(pFile, &m_BlockInfo);
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_BlockInfo.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�u���b�N�̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �u���b�N�̃��[�h�����̒��g
//===========================================
void CFile::LoadBlockProcess(FILE *pFile, SBlockFile *pInfo)
{
	// ���[�J���ϐ��錾
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p

	do
	{ // �ǂݍ��񂾕����� END_SET_BLOCK �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ

			fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f",
				&pInfo->info[pInfo->nNumData].info.pos.x,
				&pInfo->info[pInfo->nNumData].info.pos.y,
				&pInfo->info[pInfo->nNumData].info.pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{ // �ǂݍ��񂾕����� ROT �̏ꍇ

			fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f",
				&pInfo->info[pInfo->nNumData].info.rot.x,
				&pInfo->info[pInfo->nNumData].info.rot.y,
				&pInfo->info[pInfo->nNumData].info.rot.z);	// ������ǂݍ���
		}
		else if (strcmp(&aString[0], "SCALE") == 0)
		{ // �ǂݍ��񂾕����� SCALE �̏ꍇ

			fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f",
				&pInfo->info[pInfo->nNumData].info.scale.x,
				&pInfo->info[pInfo->nNumData].info.scale.y,
				&pInfo->info[pInfo->nNumData].info.scale.z);	// �T�C�Y��ǂݍ���
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
			fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d",
				&pInfo->info[pInfo->nNumData].type);	// ��ނ�ǂݍ���
		}
		else if (strcmp(&aString[0], "MAT") == 0)
		{ // �ǂݍ��񂾕����� MAT �̏ꍇ

			for (int nCntMat = 0; nCntMat < (int)CXFile::GetXFile((CXFile::TYPE)(INIT_BLOCK + pInfo->info[pInfo->nNumData].type)).dwNumMat; nCntMat++)
			{
				fscanf(pFile, "%s", &aString[0]);						// �}�e���A���̔ԍ� ��ǂݍ��� (�s�v)
				fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f %f",							// ��ނ�ǂݍ���
					&pInfo->info[pInfo->nNumData].aMat[nCntMat].MatD3D.Diffuse.r,
					&pInfo->info[pInfo->nNumData].aMat[nCntMat].MatD3D.Diffuse.g,
					&pInfo->info[pInfo->nNumData].aMat[nCntMat].MatD3D.Diffuse.b,
					&pInfo->info[pInfo->nNumData].aMat[nCntMat].MatD3D.Diffuse.a);
			}
		}
		else if (strcmp(&aString[0], "RCLIFF") == 0)
		{ // �ǂݍ��񂾕����� RCLIFF �̏ꍇ

			fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);						// �����ǂݍ���

			if (strcmp(&aString[0], c_apBooleanDisp[0]) == 0)
			{ // FALSE �������ꍇ

				// false ��ݒ肷��
				pInfo->info[pInfo->nNumData].bCliffRight = false;
			}
			else
			{ // TRUE �������ꍇ

				// true ��ݒ肷��
				pInfo->info[pInfo->nNumData].bCliffRight = true;
			}
		}
		else if (strcmp(&aString[0], "LCLIFF") == 0)
		{ // �ǂݍ��񂾕����� LCLIFF �̏ꍇ

			fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%s", &aString[0]);						// �����ǂݍ���

			if (strcmp(&aString[0], c_apBooleanDisp[0]) == 0)
			{ // FALSE �������ꍇ

				// false ��ݒ肷��
				pInfo->info[pInfo->nNumData].bCliffLeft = false;
			}
			else
			{ // TRUE �������ꍇ

				// true ��ݒ肷��
				pInfo->info[pInfo->nNumData].bCliffLeft = true;
			}
		}

	} while (strcmp(&aString[0], "END_SET_BLOCK") != 0);		// �ǂݍ��񂾕����� END_SET_BLOCK �ł͂Ȃ��ꍇ���[�v

	// �f�[�^�̑����𑝂₷
	pInfo->nNumData++;
}

//===========================================
// �w�i�I�u�W�F�N�g�̃Z�[�u����
//===========================================
HRESULT CFile::SaveBgObject(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBlock* pBlock = nullptr;		// �u���b�N�ւ̃|�C���^

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_BLOCK], "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// �擪�̃I�u�W�F�N�g�̏����擾����
		pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�� NULL ����Ȃ������

			if (pObj->GetType() == CObject::TYPE_BLOCK)
			{ // �u���b�N�������ꍇ

				// �e�̏����擾����
				pBlock = dynamic_cast<CBlock*>(pObj);

				if (pBlock != nullptr)
				{ // �u���b�N�� NULL ����Ȃ��ꍇ

					// ���������������
					fprintf(pFile, "SET_BLOCK\n");		// �u���b�N�̐ݒ����������

					fprintf(pFile, "\tPOS = ");			// �ʒu�̐ݒ����������
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetPos().x, pBlock->GetPos().y, pBlock->GetPos().z);			// �ʒu����������

					fprintf(pFile, "\tROT = ");			// �����̐ݒ����������
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetRot().x, pBlock->GetRot().y, pBlock->GetRot().z);			// ��������������

					fprintf(pFile, "\tSCALE = ");		// �T�C�Y�̐ݒ����������
					fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetScale().x, pBlock->GetScale().y, pBlock->GetScale().z);	// �g�嗦����������

					fprintf(pFile, "\tTYPE = ");		// ��ނ̐ݒ����������
					fprintf(pFile, "%d\n", pBlock->GetType());		// ��ނ���������

					fprintf(pFile, "\tMAT\n");			// �}�e���A������������

					for (int nCntMat = 0; nCntMat < (int)pBlock->GetFileData().dwNumMat; nCntMat++)
					{
						fprintf(pFile, "\tMAT%d = ", nCntMat + 1);	// �}�e���A���̔ԍ�����������
						fprintf(pFile, "%.1f %.1f %.1f\n", pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.r, pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.g, pBlock->GetMaterial(nCntMat).MatD3D.Diffuse.b);	// �}�e���A������������
					}

					// ���������������
					fprintf(pFile, "END_SET_BLOCK\n\n");			// �u���b�N�̐ݒ�̏I������������
				}
			}

			// ���̃I�u�W�F�N�g��������
			pObj = pObj->GetNext();
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�u���b�N�̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �w�i�I�u�W�F�N�g�̃��[�h����
//===========================================
HRESULT CFile::LoadBgObject(void)
{
	return E_FAIL;
}

//===========================================
// ��Q���̃Z�[�u����
//===========================================
HRESULT CFile::SaveObstacle(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CObstacle* pObstacle = nullptr;		// �u���b�N�ւ̃|�C���^

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_OBSTACLE], "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// �擪�̃I�u�W�F�N�g�̏����擾����
		pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�� NULL ����Ȃ������

			if (pObj->GetType() == CObject::TYPE_OBSTACLE)
			{ // �u���b�N�������ꍇ

				// �e�̏����擾����
				pObstacle = dynamic_cast<CObstacle*>(pObj);

				if (pObstacle != nullptr)
				{ // �u���b�N�� NULL ����Ȃ��ꍇ

					// ���������������
					fprintf(pFile, "SET_OBSTACLE\n");		// �u���b�N�̐ݒ����������

					fprintf(pFile, "\tPOS = ");			// �ʒu�̐ݒ����������
					fprintf(pFile, "%.1f %.1f %.1f\n", pObstacle->GetPos().x, pObstacle->GetPos().y, pObstacle->GetPos().z);			// �ʒu����������

					fprintf(pFile, "\tTYPE = ");		// ��ނ̐ݒ����������
					fprintf(pFile, "%d\n", pObstacle->GetType());		// ��ނ���������

					// ���������������
					fprintf(pFile, "END_SET_OBSTACLE\n\n");			// �u���b�N�̐ݒ�̏I������������
				}
			}

			// ���̃I�u�W�F�N�g��������
			pObj = pObj->GetNext();
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "��Q���̏��̏������݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// ��Q���̃��[�h����
//===========================================
HRESULT CFile::LoadObstacle(void)
{
	// �ϐ���錾
	int nEnd = 0;						// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_ObstacleInfo.nNumData = 0;		// ����������������
	m_ObstacleInfo.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE *pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_OBSTACLE], "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_OBSTACLE") == 0 &&
				nEnd != EOF)
			{ // �ǂݍ��񂾕����� SET_OBSTACLE �̏ꍇ

				// ��Q���̃��[�h�����̒��g
				LoadObstacleProcess(pFile, &m_ObstacleInfo);
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_ObstacleInfo.bSuccess = true;	
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "��Q���̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// ��Q���̃��[�h�����̒��g
//===========================================
void CFile::LoadObstacleProcess(FILE *pFile, SObstacleFile *pInfo)
{
	// ���[�J���ϐ��錾
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p

	do
	{ // �ǂݍ��񂾕����� END_SET_BLOCK �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ

			fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f%f%f",
				&pInfo->info[pInfo->nNumData].pos.x,
				&pInfo->info[pInfo->nNumData].pos.y,
				&pInfo->info[pInfo->nNumData].pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

			fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d",
				&pInfo->info[pInfo->nNumData].type);	// ��ނ�ǂݍ���
		}

	} while (strcmp(&aString[0], "END_SET_OBSTACLE") != 0);		// �ǂݍ��񂾕����� END_SET_BLOCK �ł͂Ȃ��ꍇ���[�v

	// �f�[�^�̑����𑝂₷
	pInfo->nNumData++;
}

//===========================================
// �A�C�e���̃Z�[�u����
//===========================================
HRESULT CFile::SaveItem(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;				// �I�u�W�F�N�g�ւ̃|�C���^

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_ITEM], "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		{ // �A�C�e���̏�������

			// �A�C�e���ւ̃|�C���^
			CItem* pItem = nullptr;

			// ���������������
			fprintf(pFile, "ITEM\n\n");		// �A�C�e���̈�

			// �擪�̃I�u�W�F�N�g�̏����擾����
			pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

			while (pObj != nullptr)
			{ // �I�u�W�F�N�g�� NULL ����Ȃ������

				if (pObj->GetType() == CObject::TYPE_ITEM)
				{ // �A�C�e���������ꍇ

					// �A�C�e���̏����擾����
					pItem = dynamic_cast<CItem*>(pObj);

					if (pItem != nullptr)
					{ // �u���b�N�� NULL ����Ȃ��ꍇ

						// ���������������
						fprintf(pFile, "\tSET_ITEM\n");			// �A�C�e���̐ݒ����������

						// �ʒu
						fprintf(pFile, "\t\tPOS = ");
						fprintf(pFile, "%.1f %.1f %.1f\n", pItem->GetPos().x, pItem->GetPos().y, pItem->GetPos().z);			// �ʒu����������

						// ���������������
						fprintf(pFile, "\tEND_SET_ITEM\n\n");	// �A�C�e���̐ݒ�̏I������������
					}
				}

				// ���̃I�u�W�F�N�g��������
				pObj = pObj->GetNext();
			}

			// ���������������
			fprintf(pFile, "END_ITEM\n\n");		// �A�C�e���̈�
		}

		{ // �A�C�e��������̏�������

			// �A�C�e��������ւ̃|�C���^
			CItemGenerator* pItemGene = nullptr;	

			// ���������������
			fprintf(pFile, "ITEMGENERATOR\n\n");		// �A�C�e��������̈�

			// �擪�̃I�u�W�F�N�g�̏����擾����
			pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

			while (pObj != nullptr)
			{ // �I�u�W�F�N�g�� NULL ����Ȃ������

				if (pObj->GetType() == CObject::TYPE_ITEMGENERATOR)
				{ // �A�C�e�������킾�����ꍇ

					// �A�C�e��������̏����擾����
					pItemGene = dynamic_cast<CItemGenerator*>(pObj);

					if (pItemGene != nullptr)
					{ // �u���b�N�� NULL ����Ȃ��ꍇ

						// ���������������
						fprintf(pFile, "\tSET_ITEMGENE\n");			// �A�C�e��������̐ݒ����������

						// �ʒu
						fprintf(pFile, "\t\tPOS = ");
						fprintf(pFile, "%.1f %.1f %.1f\n", pItemGene->GetPos().x, pItemGene->GetPos().y, pItemGene->GetPos().z);			// �ʒu����������

						// ���������������
						fprintf(pFile, "\tEND_SET_ITEMGENE\n\n");	// �A�C�e��������̐ݒ�̏I������������
					}
				}

				// ���̃I�u�W�F�N�g��������
				pObj = pObj->GetNext();
			}

			// ���������������
			fprintf(pFile, "END_ITEMGENERATOR\n");		// �A�C�e��������̈�
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�A�C�e���̏��̏������݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �A�C�e���̃��[�h����
//===========================================
HRESULT CFile::LoadItem(void)
{
	// �ϐ���錾
	int nEnd = 0;						// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_ItemInfo.nNumItemData = 0;		// �A�C�e���̑���������������
	m_ItemInfo.nNumItemGeneData = 0;	// �A�C�e��������̑���������������
	m_ItemInfo.bSuccess = false;		// ������

	// �|�C���^��錾
	FILE *pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_ITEM], "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "ITEM") == 0 &&
				nEnd != EOF)
			{ // �A�C�e���������ꍇ

				do
				{ // �ǂݍ��񂾕����� END_ITEM �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					nEnd = fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SET_ITEM") == 0 &&
						nEnd != EOF)
					{ // �ǂݍ��񂾕����� SET_ITEM �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_SET_ITEM �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f%f%f",
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].x,
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].y,
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].z);		// �ʒu��ǂݍ���
							}

						} while (strcmp(&aString[0], "END_SET_ITEM") != 0);		// �ǂݍ��񂾕����� END_SET_ITEM �ł͂Ȃ��ꍇ���[�v

						// �A�C�e���̃f�[�^�̑����𑝂₷
						m_ItemInfo.nNumItemData++;
					}
				} while (strcmp(&aString[0], "END_ITEM") != 0);		// �ǂݍ��񂾕����� SET_ITEM �ł͂Ȃ��ꍇ���[�v
			}						// �A�C�e���������ꍇ
			else if (strcmp(&aString[0], "ITEMGENERATOR") == 0 &&
				nEnd != EOF)
			{ // �A�C�e�������킾�����ꍇ

				do
				{ // �ǂݍ��񂾕����� END_ITEMGENE �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					nEnd = fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SET_ITEMGENE") == 0 &&
						nEnd != EOF)
					{ // �ǂݍ��񂾕����� SET_ITEM �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_SET_ITEM �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f%f%f",
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].x,
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].y,
									&m_ItemInfo.pos[m_ItemInfo.nNumItemData + m_ItemInfo.nNumItemGeneData].z);		// �ʒu��ǂݍ���
							}

						} while (strcmp(&aString[0], "END_SET_ITEMGENE") != 0);		// �ǂݍ��񂾕����� END_SET_ITEM �ł͂Ȃ��ꍇ���[�v

						// �A�C�e���̃f�[�^�̑����𑝂₷
						m_ItemInfo.nNumItemGeneData++;
					}
				} while (strcmp(&aString[0], "END_ITEMGENERATOR") != 0);		// �ǂݍ��񂾕����� SET_ITEMGENE �ł͂Ȃ��ꍇ���[�v
			}							// �A�C�e�������킾�����ꍇ
		} while (nEnd != EOF);			// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_ItemInfo.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�A�C�e���̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �J�����͈̔͂̃Z�[�u����
//===========================================
HRESULT CFile::SaveCameraRange(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CCameraRange* pCameraRange = nullptr;		// �u���b�N�ւ̃|�C���^

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_CAMERA], "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// �擪�̃I�u�W�F�N�g�̏����擾����
		pObj = CObject::GetTop(CObject::PRIORITY_BG);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�� NULL ����Ȃ������

			if (pObj->GetType() == CObject::TYPE_CAMERARANGE)
			{ // �u���b�N�������ꍇ

				// �J�����͈̔͂̏����擾����
				pCameraRange = dynamic_cast<CCameraRange*>(pObj);

				if (pCameraRange != nullptr)
				{ // �u���b�N�� NULL ����Ȃ��ꍇ

					// ���������������
					fprintf(pFile, "SET_CAMERA\n");		// �u���b�N�̐ݒ����������

					// �ʒu
					fprintf(pFile, "\tPOS = ");
					fprintf(pFile, "%.1f %.1f %.1f\n", pCameraRange->GetPos().x, pCameraRange->GetPos().y, pCameraRange->GetPos().z);

					// �T�C�Y
					fprintf(pFile, "\tSIZE = ");
					fprintf(pFile, "%.1f %.1f %.1f\n", pCameraRange->GetSize().x, pCameraRange->GetSize().y, pCameraRange->GetSize().z);

					// ���
					fprintf(pFile, "\tTYPE = ");
					fprintf(pFile, "%d\n", pCameraRange->GetType());

					// ���������������
					fprintf(pFile, "END_SET_CAMERA\n\n");			// �u���b�N�̐ݒ�̏I������������
				}
			}

			// ���̃I�u�W�F�N�g��������
			pObj = pObj->GetNext();
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�J�����͈̔͂̏��̏������݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �J�����͈̔͂̃��[�h����
//===========================================
HRESULT CFile::LoadCameraRange(void)
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_CameraInfo.nNumData = 0;		// ����������������
	m_CameraInfo.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE *pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_CAMERA], "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "SET_CAMERA") == 0)
			{ // �ǂݍ��񂾕����� SET_CAMERA �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_CAMERA �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_CameraInfo.info[m_CameraInfo.nNumData].pos.x,
							&m_CameraInfo.info[m_CameraInfo.nNumData].pos.y,
							&m_CameraInfo.info[m_CameraInfo.nNumData].pos.z);	// �ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f%f%f",
							&m_CameraInfo.info[m_CameraInfo.nNumData].size.x,
							&m_CameraInfo.info[m_CameraInfo.nNumData].size.y,
							&m_CameraInfo.info[m_CameraInfo.nNumData].size.z);	// �ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d",
							&m_CameraInfo.info[m_CameraInfo.nNumData].type);	// ��ނ�ǂݍ���
					}

				} while (strcmp(&aString[0], "END_SET_CAMERA") != 0);		// �ǂݍ��񂾕����� END_SET_CAMERA �ł͂Ȃ��ꍇ���[�v

				// �f�[�^�̑����𑝂₷
				m_CameraInfo.nNumData++;
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_CameraInfo.bSuccess = true;	
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�J�����͈̔͂̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// ���U���g�̃��[�h����
//===========================================
HRESULT CFile::LoadResultFile(void)
{
	// ���[�J���ϐ��錾
	int nEnd;						// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	m_ResultInfo.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE *pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_RESULT], "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "RANK_TIME") == 0)
			{ // �ǂݍ��񂾕����� RANK_TIME �̏ꍇ

				for (int nCntTime = 0; nCntTime < CResult::RANK_MAX; nCntTime++)
				{
					fscanf(pFile, "%s", &aString[0]);							// �����N ��ǂݍ��� (�s�v)
					fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
					fscanf(pFile, "%d", &m_ResultInfo.aRankTime[nCntTime]);		// �^�C����ǂݍ���
				}
			}
			if (strcmp(&aString[0], "RANK_SCORE") == 0)
			{ // �ǂݍ��񂾕����� RANK_SCORE �̏ꍇ

				for (int nCntTime = 0; nCntTime < CResult::RANK_MAX; nCntTime++)
				{
					fscanf(pFile, "%s", &aString[0]);							// �����N ��ǂݍ��� (�s�v)
					fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
					fscanf(pFile, "%d", &m_ResultInfo.aRankScore[nCntTime]);	// �X�R�A��ǂݍ���
				}
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_ResultInfo.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "���U���g�̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �����L���O�̃Z�[�u����
//===========================================
HRESULT CFile::SaveRankingFile(void)
{
	FILE *pFile;												// �t�@�C���|�C���^��錾
	m_RankingInfo.bSuccess = false;								// ������

	// �t�@�C�����J��
	pFile = fopen(c_apFileName[TYPE_RANKING], "wb");			// �o�C�i���t�@�C���ɏ������ނ��߂ɊJ��

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

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�����L���O�̏��̏������݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//===========================================
// �����L���O�̃��[�h����
//===========================================
HRESULT CFile::LoadRankingFile(void)
{
	FILE *pFile;						// �t�@�C���|�C���^��錾
	m_RankingInfo.bSuccess = false;		// ������

	// �t�@�C�����J��
	pFile = fopen(c_apFileName[TYPE_RANKING], "rb");			// �o�C�i���t�@�C������ǂݍ��ނ��߂ɊJ��

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

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�����L���O�̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���l��ݒ肷��
		m_RankingInfo.aRank[0] = MAX_TIME;
		m_RankingInfo.aRank[1] = MAX_TIME;
		m_RankingInfo.aRank[2] = MAX_TIME;
		m_RankingInfo.aRank[3] = MAX_TIME;
		m_RankingInfo.aRank[4] = MAX_TIME;

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//===========================================
// �Q�[���̃��[�h����
//===========================================
HRESULT CFile::LoadGameFile(void)
{
	// ���[�J���ϐ��錾
	int nEnd;						// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];		// �e�L�X�g�̕�����̑���p
	m_GameInfo.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE *pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_GAME], "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "PLAYER_POS") == 0)
			{ // �ǂݍ��񂾕����� PLAYER_POS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �v���C���[�̏����ʒu��ǂݍ���
					&m_GameInfo.PlayerPos.x, &m_GameInfo.PlayerPos.y, &m_GameInfo.PlayerPos.z);
			}
			else if (strcmp(&aString[0], "START_POS") == 0)
			{ // �ǂݍ��񂾕����� START_POS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �v���C���[�̃X�^�[�g�ʒu��ǂݍ���
					&m_GameInfo.StartPos.x, &m_GameInfo.StartPos.y, &m_GameInfo.StartPos.z);
			}
			else if (strcmp(&aString[0], "GOAL_POS") == 0)
			{ // �ǂݍ��񂾕����� GOAL_POS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �S�[���̈ʒu��ǂݍ���
					&m_GameInfo.GoalPos.x, &m_GameInfo.GoalPos.y, &m_GameInfo.GoalPos.z);
			}
			else if (strcmp(&aString[0], "STAGE_LEFT") == 0)
			{ //  �ǂݍ��񂾕����� STAGE_LEFT �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �X�e�[�W�̍��[��ǂݍ���
					&m_GameInfo.StageLeftPos.x, &m_GameInfo.StageLeftPos.y, &m_GameInfo.StageLeftPos.z);
			}
			else if (strcmp(&aString[0], "SCORE_POS") == 0)
			{ //  �ǂݍ��񂾕����� SCORE_POS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �X�R�A�̈ʒu��ǂݍ���
					&m_GameInfo.ScorePos.x, &m_GameInfo.ScorePos.y, &m_GameInfo.ScorePos.z);
			}
			else if (strcmp(&aString[0], "SCORE_SIZE") == 0)
			{ //  �ǂݍ��񂾕����� SCORE_SIZE �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �X�R�A�̃T�C�Y��ǂݍ���
					&m_GameInfo.ScoreSize.x, &m_GameInfo.ScoreSize.y, &m_GameInfo.ScoreSize.z);
			}
			else if (strcmp(&aString[0], "TIME_POS") == 0)
			{ //  �ǂݍ��񂾕����� TIME_POS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �^�C���̈ʒu��ǂݍ���
					&m_GameInfo.TimePos.x, &m_GameInfo.TimePos.y, &m_GameInfo.TimePos.z);
			}
			else if (strcmp(&aString[0], "TIME_SIZE") == 0)
			{ //  �ǂݍ��񂾕����� TIME_SIZE �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �^�C���̃T�C�Y��ǂݍ���
					&m_GameInfo.TimeSize.x, &m_GameInfo.TimeSize.y, &m_GameInfo.TimeSize.z);
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_GameInfo.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�Q�[���̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �`���[�g���A���̃��[�h����
//===========================================
HRESULT CFile::LoadTutorialFile(void)
{
	// ���[�J���ϐ��錾
	int nEnd = 0;						// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p
	m_TutorialInfo.bSuccess = false;	// ������

	// �|�C���^��錾
	FILE *pFile;						// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(c_apFileName[TYPE_TUTORIAL], "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "BALLOON_DATA") == 0 &&
				nEnd != EOF)
			{ // �ǂݍ��񂾕����� BALLOON_DATA �̏ꍇ

				for (int nCnt = 0; nCnt < CBalloon::TYPE_MAX; nCnt++)
				{
					fscanf(pFile, "%s", &aString[0]);		// TYPE ��ǂݍ��� (�s�v)
					fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
					fscanf(pFile, "%f %f %f",				// �����o���̃T�C�Y��ǂݍ���
						&m_aBalloonData[nCnt].size.x, 
						&m_aBalloonData[nCnt].size.y, 
						&m_aBalloonData[nCnt].size.z);
					fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
					fscanf(pFile, "%s", &m_aBalloonData[nCnt].aTextureName[0]);	// �����o���̃e�N�X�`����ǂݍ���
				}
			}
			else if (strcmp(&aString[0], "SET_BALLOON") == 0 &&
				nEnd != EOF)
			{ // �ǂݍ��񂾕����� SET_BALLOON �̏ꍇ

				// �����o���̃��[�h�����̒��g
				LoadBalloonProcess(pFile, &m_TutorialInfo.BalloonInfo);
			}
			else if (strcmp(&aString[0], "SET_BLOCK") == 0 &&
				nEnd != EOF)
			{ // �ǂݍ��񂾕����� SET_BLOCK �̏ꍇ

				// �u���b�N�̃��[�h�����̒��g
				LoadBlockProcess(pFile, &m_TutorialInfo.BlockInfo);
			}
			else if (strcmp(&aString[0], "SET_BGOBJECT") == 0 &&
				nEnd != EOF)
			{ // �ǂݍ��񂾕����� SET_BGOBJECT �̏ꍇ

			}
			else if (strcmp(&aString[0], "SET_OBSTACLE") == 0 &&
				nEnd != EOF)
			{ // �ǂݍ��񂾕����� SET_OBSTACLE �̏ꍇ

				// ��Q���̃��[�h�����̒��g
				LoadObstacleProcess(pFile, &m_TutorialInfo.ObstaInfo);
			}
			else if (strcmp(&aString[0], "PLAYER_POS") == 0)
			{ //  �ǂݍ��񂾕����� PLAYER_POS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �v���C���[�̈ʒu��ǂݍ���
					&m_TutorialInfo.PlayerPos.x, &m_TutorialInfo.PlayerPos.y, &m_TutorialInfo.PlayerPos.z);
			}
			else if (strcmp(&aString[0], "GOAL_POS") == 0)
			{ //  �ǂݍ��񂾕����� GOAL_POS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f %f %f",									// �S�[���̈ʒu��ǂݍ���
					&m_TutorialInfo.GoalPos.x, &m_TutorialInfo.GoalPos.y, &m_TutorialInfo.GoalPos.z);
			}
		} while (nEnd != EOF);				// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);

		// �����󋵂� true �ɂ���
		m_TutorialInfo.bSuccess = true;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�`���[�g���A���̏��̓ǂݍ��݂Ɏ��s!", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �����o���̃��[�h�����̒��g
//===========================================
void CFile::LoadBalloonProcess(FILE *pFile, SBalloonFile *pInfo)
{
	// ���[�J���ϐ��錾
	char aString[MAX_STRING];			// �e�L�X�g�̕�����̑���p

	do
	{ // �ǂݍ��񂾕����� END_SET_BALLOON �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�����當�����ǂݍ���
		fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "POS") == 0)
		{ // �ǂݍ��񂾕����� POS �̏ꍇ

			fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%f %f %f",
				&pInfo->info[pInfo->nNumData].pos.x,
				&pInfo->info[pInfo->nNumData].pos.y,
				&pInfo->info[pInfo->nNumData].pos.z);	// �ʒu��ǂݍ���
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

			fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
			fscanf(pFile, "%d",
				&pInfo->info[pInfo->nNumData].type);	// ��ނ�ǂݍ���
		}
	} while (strcmp(&aString[0], "END_SET_BALLOON") != 0);		// �ǂݍ��񂾕����� END_SET_BALLOON �ł͂Ȃ��ꍇ���[�v

	// �����o���̑��������Z����
	pInfo->nNumData++;
}