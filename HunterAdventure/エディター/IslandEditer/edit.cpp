//===========================================
//
// �G�f�B�b�g�̃��C������[edit.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "edit.h"
#include "renderer.h"
#include "useful.h"
#include "file.h"

#include "model.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "enemy.h"
#include "coin.h"
#include "gold_bone.h"
#include "tree.h"
#include "rock.h"
#include "block.h"
#include "bang_flower.h"
#include "wall.h"

// �������O���
namespace
{
	// �S��
	const char* STYLE_NAME[CEdit::STYLE_MAX] =		// �X�^�C���̕\����
	{
		"Enemy",
		"Coin",
		"GoldBone",
		"Tree",
		"Rock",
		"Block",
		"BangFlower",
		"Wall",
		"Grass",
		"Lake",
	};

	// �G�֌W
	const char* ENEMY_NAME[CEnemy::TYPE_MAX] =		// �G�̕\����
	{
		"Tordle",
		"Iwakari",
	};	
	const char* ENEMY_MODEL[CEnemy::TYPE_MAX] =		// �G�̃��f��
	{
		"data\\MODEL\\Tordle.x",
		"data\\MODEL\\Iwakari.x",
	};

	// �R�C���֌W
	const char* COIN_MODEL = "data\\MODEL\\Coin.x";	// �R�C���̃��f��

	// ���̍��֌W
	const char* GOLDBONE_MODEL = "data\\MODEL\\GoldBone.x";		// ���̍��̃��f��

	// �؊֌W
	const char* TREE_NAME[CTree::TYPE_MAX] =		// �؂̕\����
	{
		"PalmTree",
	};
	const char* TREE_MODEL[CTree::TYPE_MAX] =		// �؂̃��f��
	{
		"data\\MODEL\\PalmTree.x",
	};

	// ��֌W
	const char* ROCK_NAME[CRock::TYPE_MAX] =		// ��̕\����
	{
		"Brown",
		"Gray",
		"Break",
	};
	const char* ROCK_MODEL[CRock::TYPE_MAX] =		// ��̃��f��
	{
		"data\\MODEL\\Rock001.x",
		"data\\MODEL\\Rock002.x",
		"data\\MODEL\\Rock001.x",
	};

	// �u���b�N�֌W
	const char* BLOCK_MODEL = "data\\MODEL\\IronBlock.x";		// �u���b�N�̃��f��

	// ���e�Ԋ֌W
	const char* BANGFLOWER_MODEL = "data\\MODEL\\BombFlower.x";		// ���e�Ԃ̃��f��

	// �Ǌ֌W
	const char* WALL_MODEL[CWall::TYPE_MAX] =		// �ǂ̃��f��
	{
		"data\\MODEL\\Wall001.x",
		"data\\MODEL\\Wall002.x",
	};

	const char* WALL_NAME[CWall::TYPE_MAX] =		// �ǂ̖��O
	{
		"Normal",
		"Narrow"
	};

	const char* ROTTYPE_NAME[CWall::ROTTYPE_MAX] =	// �����̖��O
	{
		"Front",
		"Right",
		"Back",
		"Left",
	};

	const float MOVE = 4.0f;			// �ړ���
	const float POS_MAX = 10000.0f;		// �ʒu�̍ő�l
	const float POS_MIN = -10000.0f;	// �ʒu�̍ŏ��l
	const float ROT_MOVE = 0.01f;		// �ړ���
	const float SCALE_MOVE = 0.1f;		// �ړ���
	const float SCALE_MAX = 99000.0f;	// �ړ���
	const float SCALE_MIN = 99000.0f;	// �ړ���
}

//==============================
// �R���X�g���N�^
//==============================
CEdit::CEdit() : CObject(CObject::TYPE_EDIT, DIM_3D, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_pModel = nullptr;			// ���f���̏��
	m_pProcess = nullptr;		// �����̏��
	m_style = STYLE_ENEMY;		// �X�^�C��
}

//==============================
// �f�X�g���N�^
//==============================
CEdit::~CEdit()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CEdit::Init(void)
{
	// ���f���𐶐�����
	m_pModel = CModel::Create(TYPE_NONE, PRIORITY_UI);

	// �����̏��𐶐�����
	m_pProcess = new CEnemyProcess();

	// �l��Ԃ�
	return S_OK;
}

//========================================
//�u���b�N�̏I������
//========================================
void CEdit::Uninit(void)
{
	if (m_pProcess != nullptr)
	{ // ������ NULL ����Ȃ��ꍇ

		// ���������J������
		delete m_pProcess;
		m_pProcess = nullptr;
	}

	if (m_pModel != nullptr)
	{ // ���f���� NULL ����Ȃ��ꍇ

		// �I������
		m_pModel->Uninit();
		m_pModel = nullptr;
	}

	// �I������
	Release();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CEdit::Update(void)
{
	D3DXVECTOR3 pos = m_pModel->GetPos();

	// Window�̃^�C�g��
	ImGui::Begin("Editer");

	// ���X�g��\������
	if (ImGui::ListBox("STYLE", (int*)&m_style, STYLE_NAME, STYLE_MAX))
	{ // �X�^�C����ς����Ƃ�

		if (m_pProcess != nullptr)
		{ // ������ NULL ����Ȃ��ꍇ

			// �����̏��̏���
			delete m_pProcess;
			m_pProcess = nullptr;
		}

		// �����Ɗg�嗦��ݒ肷��
		m_pModel->SetRot(NONE_D3DXVECTOR3);
		m_pModel->SetScale(NONE_SCALE);

		switch (m_style)
		{
		case CEdit::STYLE_ENEMY:

			// �G����
			m_pProcess = new CEnemyProcess();

			// ���f���̏��ݒ菈��
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ENEMY_MODEL[0]));

			break;

		case CEdit::STYLE_COIN:

			// �R�C������
			m_pProcess = new CCoinProcess();

			// ���f���̏��ݒ菈��
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(COIN_MODEL));

			break;

		case CEdit::STYLE_GOLDBONE:

			// ���̍�����
			m_pProcess = new CGoldBoneProcess();

			// ���f���̏��ݒ菈��
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(GOLDBONE_MODEL));

			break;

		case CEdit::STYLE_TREE:

			// �؏���
			m_pProcess = new CTreeProcess();

			// ���f���̏��ݒ菈��
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(TREE_MODEL[0]));

			break;

		case CEdit::STYLE_ROCK:

			// �⏈��
			m_pProcess = new CRockProcess();

			// ���f���̏��ݒ菈��
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ROCK_MODEL[0]));

			break;

		case CEdit::STYLE_BLOCK:

			// �u���b�N����
			m_pProcess = new CBlockProcess();

			// ���f���̏��ݒ菈��
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(BLOCK_MODEL));
			
			break;

		case CEdit::STYLE_BANGFLOWER:

			// �u���b�N����
			m_pProcess = new CBangFlowerProcess();

			// ���f���̏��ݒ菈��
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(BANGFLOWER_MODEL));

			break;

		case CEdit::STYLE_WALL:

			// �ǂ̏���
			m_pProcess = new CWallProcess();

			// ���f���̏��ݒ菈��
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(WALL_MODEL[0]));

			break;

		case CEdit::STYLE_GRASS:
			break;

		case CEdit::STYLE_LAKE:
			break;

		default:

			// ��~
			assert(false);

			break;
		}
	}

	// �ʒu�̐ݒ�
	if (ImGui::DragFloat3("Position", (float*)&pos, MOVE, POS_MIN, POS_MAX))

	// �ʒu��K�p����
	m_pModel->SetPos(pos);

	// ����
	m_pProcess->Process(m_pModel);

	//// �e�L�X�g�̕\��
	//ImGui::Text("Obara is Stupid");

	//// �`�F�b�N�{�b�N�X�̕\��
	//ImGui::Checkbox("Y/N", &bObaraNew);

	//// �e�L�X�g�̕\��
	//ImGui::Text("DonkuraiKusa?");

	//// INT�^�̃X���C�_�[
	//if (ImGui::SliderInt("Level", &nGlassLevel, 0, 6000) == true)
	//{ // ���������ꍇ

	//}

	//// float�^3�̃X���C�_�[
	//if (ImGui::SliderFloat3("position", (float*)&f, -4000.0f, 4000.0f))
	//{
	//	int n = 0;
	//}

	//// �{�^���̕\��
	//if (ImGui::Button("KusaHayashi") == true)
	//{ // �{�^�����������ꍇ

	//	nGlassLevel++;
	//}
	//// ��ɑ����e�L�X�g�𓯂��s�ɕ\������
	//ImGui::SameLine();

	//// �e�L�X�g��\������
	//ImGui::Text("KusaLevel = %d", nGlassLevel);

	// ImGui�̕\���̏I��
	ImGui::End();
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CEdit::Draw(void)
{
	if (m_pModel != nullptr)
	{ // ���f���̏�� NULL ����Ȃ��ꍇ

		// �`�揈��
		m_pModel->Draw();
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CEdit::SetData(void)
{
	if (m_pModel != nullptr)
	{ // ���f���̏�� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		m_pModel->SetPos(NONE_D3DXVECTOR3);		// �ʒu
		m_pModel->SetPosOld(NONE_D3DXVECTOR3);	// �O��̈ʒu
		m_pModel->SetRot(NONE_D3DXVECTOR3);		// ����
		m_pModel->SetScale(NONE_SCALE);			// �g�嗦
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ENEMY_MODEL[0]));		// ���f���̏��
	}

	// �S�Ă̒l��ݒ肷��
	m_style = STYLE_ENEMY;		// �X�^�C��
}

//=======================================
// ��������
//=======================================
CEdit* CEdit::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CEdit* pEdit = nullptr;		// �G�f�B�b�g�𐶐�

	if (pEdit == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pEdit = new CEdit;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEdit != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEdit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEdit->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �G�f�B�b�g�̃|�C���^��Ԃ�
	return pEdit;
}

// �����̐e�N���X
CProcess::CProcess()
{

}

CProcess::~CProcess()
{

}

// �G�̏���
CEnemyProcess::CEnemyProcess() : CProcess()
{
	// �S�Ă̒l���N���A����
	m_type = CEnemy::TYPE_TORDLE;		// �G�̎��
}

CEnemyProcess::~CEnemyProcess()
{

}

void CEnemyProcess::Process(CModel* m_pModel)
{
	// �������擾����
	D3DXVECTOR3 rot = m_pModel->GetRot();

	// �����̐ݒ�
	ImGui::DragFloat("Rotation", (float*)&rot.y, ROT_MOVE, -D3DX_PI, D3DX_PI);

	// ������K�p����
	m_pModel->SetRot(rot);

	// �G�̎�ޑI��
	if (ImGui::ListBox("ENEMY TYPE", (int*)&m_type, ENEMY_NAME, CEnemy::TYPE_MAX))
	{ // �G�̎�ނ��ς�����u��

		// ���f����ς���
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ENEMY_MODEL[m_type]));
	}

	// �{�^���̕\��
	if (ImGui::Button("EnemySet") == true)
	{ // �{�^�����������ꍇ

		// �G�̐�������
		CEnemy::Create(m_pModel->GetPos(), m_pModel->GetRot(), m_type);
	}

	if (ImGui::Button("Save") == true)
	{ // �{�^�����������ꍇ

		// �G�̃Z�[�u����
		CManager::Get()->GetFile()->Save(CFile::TYPE_ENEMY);
	}

	// ��������
	Delete(m_pModel);
}

void CEnemyProcess::Delete(CModel* m_pModel)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = m_pModel->GetPos();		// �ʒu
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// �ő�l
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// �ŏ��l
	D3DXVECTOR3 posObject;		// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 MaxObject;		// �I�u�W�F�N�g�̍ő�l
	D3DXVECTOR3 MinObject;		// �I�u�W�F�N�g�̍ŏ��l
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pObject = nullptr;		// �擪�̓G
	CEnemy* pObjectEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pObject = list.GetTop();

		// �����̒l���擾����
		pObjectEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ�
			posObject = pObject->GetPos();
			MaxObject = pObject->GetCollSize();
			MinObject = D3DXVECTOR3
			(
				-pObject->GetCollSize().x,
				0.0f,
				-pObject->GetCollSize().z
			);

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // �G�Əd�Ȃ����ꍇ

				if (ImGui::Button("Delete") == true)
				{ // �{�^�����������ꍇ

					// �q�b�g����
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pObject = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

// �R�C���̏���
CCoinProcess::CCoinProcess()
{

}

CCoinProcess::~CCoinProcess()
{

}

void CCoinProcess::Process(CModel* m_pModel)
{
	// �{�^���̕\��
	if (ImGui::Button("CoinSet") == true)
	{ // �{�^�����������ꍇ

		// �R�C���̐�������
		CCoin::Create(m_pModel->GetPos());
	}

	if (ImGui::Button("Save") == true)
	{ // �{�^�����������ꍇ

		// �R�C���̃Z�[�u����
		CManager::Get()->GetFile()->Save(CFile::TYPE_COIN);
	}

	// ��������
	Delete(m_pModel);
}

void CCoinProcess::Delete(CModel* m_pModel)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = m_pModel->GetPos();		// �ʒu
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// �ő�l
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// �ŏ��l
	D3DXVECTOR3 posObject;		// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 MaxObject;		// �I�u�W�F�N�g�̍ő�l
	D3DXVECTOR3 MinObject;		// �I�u�W�F�N�g�̍ŏ��l
	CListManager<CCoin*> list = CCoin::GetList();
	CCoin* pObject = nullptr;		// �擪�̓G
	CCoin* pObjectEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pObject = list.GetTop();

		// �����̒l���擾����
		pObjectEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ�
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // �G�Əd�Ȃ����ꍇ

				if (ImGui::Button("Delete") == true)
				{ // �{�^�����������ꍇ

					// �q�b�g����
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pObject = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

// ���̍��̏���
CGoldBoneProcess::CGoldBoneProcess()
{

}

CGoldBoneProcess::~CGoldBoneProcess()
{

}

void CGoldBoneProcess::Process(CModel* m_pModel)
{
	// �{�^���̕\��
	if (ImGui::Button("GoldBoneSet") == true)
	{ // �{�^�����������ꍇ

		// �R�C���̐�������
		CGoldBone::Create(m_pModel->GetPos());
	}

	if (ImGui::Button("Save") == true)
	{ // �{�^�����������ꍇ

		// ���̍��̃Z�[�u����
		CManager::Get()->GetFile()->Save(CFile::TYPE_GOLDBONE);
	}

	// ��������
	Delete(m_pModel);
}

void CGoldBoneProcess::Delete(CModel* m_pModel)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = m_pModel->GetPos();		// �ʒu
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// �ő�l
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// �ŏ��l
	D3DXVECTOR3 posObject;		// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 MaxObject;		// �I�u�W�F�N�g�̍ő�l
	D3DXVECTOR3 MinObject;		// �I�u�W�F�N�g�̍ŏ��l
	CListManager<CGoldBone*> list = CGoldBone::GetList();
	CGoldBone* pObject = nullptr;		// �擪�̓G
	CGoldBone* pObjectEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pObject = list.GetTop();

		// �����̒l���擾����
		pObjectEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ�
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // �G�Əd�Ȃ����ꍇ

				if (ImGui::Button("Delete") == true)
				{ // �{�^�����������ꍇ

					// �q�b�g����
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pObject = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

// �؂̏���
CTreeProcess::CTreeProcess()
{
	// �S�Ă̒l���N���A����
	m_type = CTree::TYPE_PALM;		// �؂̎��
}

CTreeProcess::~CTreeProcess()
{

}

void CTreeProcess::Process(CModel* m_pModel)
{
	// �������擾����
	D3DXVECTOR3 rot = m_pModel->GetRot();

	// �����̐ݒ�
	ImGui::DragFloat("Rotation", (float*)&rot.y, ROT_MOVE, -D3DX_PI, D3DX_PI);

	// ������K�p����
	m_pModel->SetRot(rot);

	// �؂̎�ޑI��
	if (ImGui::ListBox("TREE TYPE", (int*)&m_type, TREE_NAME, CTree::TYPE_MAX))
	{ // �؂̎�ނ��ς�����u��

		// ���f����ς���
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(TREE_MODEL[m_type]));
	}

	// �{�^���̕\��
	if (ImGui::Button("TreeSet") == true)
	{ // �{�^�����������ꍇ

		// �؂̐�������
		CTree::Create(m_pModel->GetPos(), m_pModel->GetRot(), m_type);
	}

	if (ImGui::Button("Save") == true)
	{ // �{�^�����������ꍇ

		// �؂̃Z�[�u����
		CManager::Get()->GetFile()->Save(CFile::TYPE_TREE);
	}

	// ��������
	Delete(m_pModel);
}

void CTreeProcess::Delete(CModel* m_pModel)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = m_pModel->GetPos();		// �ʒu
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// �ő�l
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// �ŏ��l
	D3DXVECTOR3 posObject;		// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 MaxObject;		// �I�u�W�F�N�g�̍ő�l
	D3DXVECTOR3 MinObject;		// �I�u�W�F�N�g�̍ŏ��l
	CListManager<CTree*> list = CTree::GetList();
	CTree* pObject = nullptr;		// �擪�̓G
	CTree* pObjectEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pObject = list.GetTop();

		// �����̒l���擾����
		pObjectEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ�
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // �G�Əd�Ȃ����ꍇ

				if (ImGui::Button("Delete") == true)
				{ // �{�^�����������ꍇ

					// �q�b�g����
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pObject = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

// ��̏���
CRockProcess::CRockProcess()
{
	// �S�Ă̒l���N���A����
	m_type = CRock::TYPE_BROWN;		// ��̎��
}

CRockProcess::~CRockProcess()
{

}

void CRockProcess::Process(CModel* m_pModel)
{
	{ // �����̐ݒ�

		D3DXVECTOR3 rot = m_pModel->GetRot();

		ImGui::DragFloat("Rotation", (float*)&rot.y, ROT_MOVE, -D3DX_PI, D3DX_PI);

		m_pModel->SetRot(rot);
	}

	{ // �g�嗦�̐ݒ�

		D3DXVECTOR3 scale = m_pModel->GetScale();

		ImGui::DragFloat3("Scaling", (float*)&scale, SCALE_MOVE, SCALE_MIN, SCALE_MAX);

		m_pModel->SetScale(scale);
	}

	// ��̎�ޑI��
	if (ImGui::ListBox("ROCK TYPE", (int*)&m_type, ROCK_NAME, CRock::TYPE_MAX))
	{ // ��̎�ނ��ς�����u��

		// ���f����ς���
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ROCK_MODEL[m_type]));
	}

	// �{�^���̕\��
	if (ImGui::Button("RockSet") == true)
	{ // �{�^�����������ꍇ

		// ��̐�������
		CRock::Create(m_pModel->GetPos(), m_pModel->GetRot(), m_pModel->GetScale(), m_type);
	}

	if (ImGui::Button("Save") == true)
	{ // �{�^�����������ꍇ

		// ��̃Z�[�u����
		CManager::Get()->GetFile()->Save(CFile::TYPE_ROCK);
	}

	// ��������
	Delete(m_pModel);
}

void CRockProcess::Delete(CModel* m_pModel)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = m_pModel->GetPos();		// �ʒu
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// �ő�l
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// �ŏ��l
	D3DXVECTOR3 posObject;		// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 MaxObject;		// �I�u�W�F�N�g�̍ő�l
	D3DXVECTOR3 MinObject;		// �I�u�W�F�N�g�̍ŏ��l
	CListManager<CRock*> list = CRock::GetList();
	CRock* pObject = nullptr;		// �擪�̓G
	CRock* pObjectEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pObject = list.GetTop();

		// �����̒l���擾����
		pObjectEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ�
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // �G�Əd�Ȃ����ꍇ

				if (ImGui::Button("Delete") == true)
				{ // �{�^�����������ꍇ

					// �q�b�g����
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pObject = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

// �u���b�N�̏���
CBlockProcess::CBlockProcess()
{

}

CBlockProcess::~CBlockProcess()
{

}

void CBlockProcess::Process(CModel* m_pModel)
{
	{ // �g�嗦�̐ݒ�

		D3DXVECTOR3 scale = m_pModel->GetScale();

		ImGui::DragFloat3("Scaling", (float*)&scale, SCALE_MOVE, SCALE_MIN, SCALE_MAX);

		m_pModel->SetScale(scale);
	}

	// �{�^���̕\��
	if (ImGui::Button("BlockSet") == true)
	{ // �{�^�����������ꍇ

		// �u���b�N�̐�������
		CBlock::Create(m_pModel->GetPos(), m_pModel->GetScale());
	}

	if (ImGui::Button("Save") == true)
	{ // �{�^�����������ꍇ

		// �u���b�N�̃Z�[�u����
		CManager::Get()->GetFile()->Save(CFile::TYPE_BLOCK);
	}

	// ��������
	Delete(m_pModel);
}

void CBlockProcess::Delete(CModel* m_pModel)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = m_pModel->GetPos();		// �ʒu
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// �ő�l
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// �ŏ��l
	D3DXVECTOR3 posObject;		// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 MaxObject;		// �I�u�W�F�N�g�̍ő�l
	D3DXVECTOR3 MinObject;		// �I�u�W�F�N�g�̍ŏ��l
	CListManager<CBlock*> list = CBlock::GetList();
	CBlock* pObject = nullptr;		// �擪�̓G
	CBlock* pObjectEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pObject = list.GetTop();

		// �����̒l���擾����
		pObjectEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ�
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // �G�Əd�Ȃ����ꍇ

				if (ImGui::Button("Delete") == true)
				{ // �{�^�����������ꍇ

					// �q�b�g����
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pObject = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

// ���e�Ԃ̏���
CBangFlowerProcess::CBangFlowerProcess()
{

}

CBangFlowerProcess::~CBangFlowerProcess()
{

}

void CBangFlowerProcess::Process(CModel* m_pModel)
{
	{ // �����̐ݒ�

		D3DXVECTOR3 rot = m_pModel->GetRot();

		ImGui::DragFloat("Rotation", (float*)&rot.y, ROT_MOVE, -D3DX_PI, D3DX_PI);

		m_pModel->SetRot(rot);
	}

	// �{�^���̕\��
	if (ImGui::Button("BangFlowerSet") == true)
	{ // �{�^�����������ꍇ

		// ���e�Ԃ̐�������
		CBangFlower::Create(m_pModel->GetPos(), m_pModel->GetRot());
	}

	if (ImGui::Button("Save") == true)
	{ // �{�^�����������ꍇ

		// ���e�Ԃ̃Z�[�u����
		CManager::Get()->GetFile()->Save(CFile::TYPE_BANGFLOWER);
	}

	// ��������
	Delete(m_pModel);
}

void CBangFlowerProcess::Delete(CModel* m_pModel)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = m_pModel->GetPos();		// �ʒu
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// �ő�l
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// �ŏ��l
	D3DXVECTOR3 posObject;		// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 MaxObject;		// �I�u�W�F�N�g�̍ő�l
	D3DXVECTOR3 MinObject;		// �I�u�W�F�N�g�̍ŏ��l
	CListManager<CBangFlower*> list = CBangFlower::GetList();
	CBangFlower* pObject = nullptr;		// �擪�̓G
	CBangFlower* pObjectEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pObject = list.GetTop();

		// �����̒l���擾����
		pObjectEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ�
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // �G�Əd�Ȃ����ꍇ

				if (ImGui::Button("Delete") == true)
				{ // �{�^�����������ꍇ

					// �q�b�g����
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pObject = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

// �ǂ̏���
CWallProcess::CWallProcess()
{
	// �S�Ă̒l���N���A����
	m_type = CWall::TYPE::TYPE_NORMAL;
	m_rottype = CWall::ROTTYPE_FRONT;
}

CWallProcess::~CWallProcess()
{
	
}

void CWallProcess::Process(CModel* m_pModel)
{
	{ // �g�嗦�̐ݒ�

		D3DXVECTOR3 scale = m_pModel->GetScale();

		ImGui::DragFloat3("Scaling", (float*)&scale, SCALE_MOVE, SCALE_MIN, SCALE_MAX);

		m_pModel->SetScale(scale);
	}

	// ��̎�ޑI��
	if (ImGui::ListBox("WALL TYPE", (int*)&m_type, WALL_NAME, CWall::TYPE_MAX))
	{ // ��̎�ނ��ς�����u��

		// ���f����ς���
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(WALL_MODEL[m_type]));
	}

	// ��̎�ޑI��
	if (ImGui::ListBox("ROT TYPE", (int*)&m_rottype, ROTTYPE_NAME, CWall::ROTTYPE_MAX))
	{
		switch (m_rottype)
		{
		case CWall::ROTTYPE_FRONT:		// �O����

			// ������ݒ肷��
			m_pModel->SetRot(NONE_D3DXVECTOR3);

			break;

		case CWall::ROTTYPE_RIGHT:		// �E����

			// ������ݒ肷��
			m_pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

			break;

		case CWall::ROTTYPE_BACK:		// ������

			// ������ݒ肷��
			m_pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

			break;

		case CWall::ROTTYPE_LEFT:		// ������

			// ������ݒ肷��
			m_pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

			break;

		default:

			// ��~
			assert(false);

			break;
		}
	}

	// �{�^���̕\��
	if (ImGui::Button("WallSet") == true)
	{ // �{�^�����������ꍇ

		// �ǂ̐�������
		CWall::Create(m_pModel->GetPos(), m_pModel->GetScale(), m_type, m_rottype);
	}

	if (ImGui::Button("Save") == true)
	{ // �{�^�����������ꍇ

		// �ǂ̃Z�[�u����
		CManager::Get()->GetFile()->Save(CFile::TYPE_WALL);
	}

	// ��������
	Delete(m_pModel);
}

void CWallProcess::Delete(CModel* m_pModel)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = m_pModel->GetPos();		// �ʒu
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// �ő�l
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// �ŏ��l
	D3DXVECTOR3 posObject;		// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 MaxObject;		// �I�u�W�F�N�g�̍ő�l
	D3DXVECTOR3 MinObject;		// �I�u�W�F�N�g�̍ŏ��l
	CListManager<CWall*> list = CWall::GetList();
	CWall* pObject = nullptr;		// �擪�̓G
	CWall* pObjectEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pObject = list.GetTop();

		// �����̒l���擾����
		pObjectEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�֌W�̕ϐ���ݒ�
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // �G�Əd�Ȃ����ꍇ

				if (ImGui::Button("Delete") == true)
				{ // �{�^�����������ꍇ

					// �q�b�g����
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pObject = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}