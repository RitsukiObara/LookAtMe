//===========================================
//
// �`���[�g���A���}�b�v�̃��C������[tutorial_map.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "tutorial_map.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "mesh_wall.h"
#include "mesh_field.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const D3DXVECTOR3 WALL_POS[CTutorialMap::NUM_WALL] =		// �ǂ̈ʒu
	{
		D3DXVECTOR3(2500.0f,0.0f,0.0f),
		D3DXVECTOR3(-2500.0f,0.0f,0.0f),
		D3DXVECTOR3(0.0f,0.0f,-2500.0f),
	};
	const D3DXVECTOR3 WALL_ROT[CTutorialMap::NUM_WALL] =		// �ǂ̌���
	{
		D3DXVECTOR3(0.0f,D3DX_PI * 0.5f,0.0f),
		D3DXVECTOR3(0.0f,D3DX_PI * -0.5f,0.0f),
		D3DXVECTOR3(0.0f,D3DX_PI,0.0f),
	};
	const float WALL_WIDTH = 5000.0f;			// �ǂ̕�
	const float WALL_HEIGHT = 2000.0f;			// �ǂ̍���
	const CMesh::SGrid WALL_DIVI = { 1,1 };		// �ǂ̕�����
	const char* WALL_TEXTURE = "data\\TEXTURE\\TutorialWall.png";	// �ǂ̃e�N�X�`��

	const D3DXVECTOR3 CEIL_POS = D3DXVECTOR3(0.0f, 2000.0f, 0.0f);	// �V��̈ʒu
}

//==============================
// �R���X�g���N�^
//==============================
CTutorialMap::CTutorialMap() : CObject(CObject::TYPE_TUTORIALMAP, DIM_3D, CObject::PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < NUM_WALL; nCnt++)
	{
		m_apWall[nCnt] = nullptr;	// �ǂ̏��
	}
}

//==============================
// �f�X�g���N�^
//==============================
CTutorialMap::~CTutorialMap()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CTutorialMap::Init(void)
{
	// �l��Ԃ�
	return S_OK;
}

//========================================
// �j�Ђ̏I������
//========================================
void CTutorialMap::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_WALL; nCnt++)
	{
		if (m_apWall[nCnt] != nullptr)
		{ // �ǂ� NULL ����Ȃ��ꍇ

			// �ǂ̏I������
			m_apWall[nCnt]->Uninit();
			m_apWall[nCnt] = nullptr;
		}
	}

	// �{�̂̏I������
	Release();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CTutorialMap::Update(void)
{

}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CTutorialMap::Draw(void)
{
	for (int nCnt = 0; nCnt < NUM_WALL; nCnt++)
	{
		if (m_apWall[nCnt] != nullptr)
		{ // �ǂ� NULL ����Ȃ��ꍇ

			// �ǂ̕`�揈��
			m_apWall[nCnt]->Draw();
		}
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CTutorialMap::SetData(void)
{
	for (int nCnt = 0; nCnt < NUM_WALL; nCnt++)
	{
		if (m_apWall[nCnt] == nullptr)
		{ // �ǂ� NULL �̏ꍇ

			// �ǂ𐶐�
			m_apWall[nCnt] = CMeshWall::Create
			(
				WALL_POS[nCnt], 
				WALL_ROT[nCnt], 
				WALL_WIDTH, 
				WALL_HEIGHT, 
				WALL_DIVI, 
				PRIORITY_BG
			);
			m_apWall[nCnt]->CObject::SetType(TYPE_NONE);		// ����Ԃɂ���
			m_apWall[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(WALL_TEXTURE));
			m_apWall[nCnt]->SetLighting(true);					// ���C�e�B���O��
			m_apWall[nCnt]->SetCulling(false);					// �J�����O��
		}
		else
		{ // ��L�ȊO

			// ��~
			assert(false);
		}
	}
}

//=======================================
// ��������
//=======================================
CTutorialMap* CTutorialMap::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CTutorialMap* pMap = nullptr;	// �C���X�^���X�𐶐�

	if (pMap == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pMap = new CTutorialMap;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pMap != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pMap->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pMap->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �}�b�v�̃|�C���^��Ԃ�
	return pMap;
}