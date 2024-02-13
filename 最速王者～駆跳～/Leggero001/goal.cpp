//=======================================
//
// �S�[���̃��C������[Goal.cpp]
// Author�F��������
//
//=======================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "goal.h"
#include "useful.h"

#include "game_goal.h"
#include "tutorial_goal.h"

//---------------------------------------
// �}�N����`
//---------------------------------------
#define GOAL_TEXTURE		"data/TEXTURE/Goal.png"					// �S�[���̃e�N�X�`��
#define GOAL_SIZE			(D3DXVECTOR3(30.0f, 5000.0f, 0.0f))		// �S�[���̃T�C�Y
#define GOAL_INIT_COLOR		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))		// �S�[���̏����F
#define GOAL_ADD_ALPHA		(0.01f)									// �S�[���̓����x�̒ǉ���
#define GOAL_ALPHA_MAX		(0.7f)									// �S�[���̓����x�̍ő�l
#define GOAL_ALPHA_MIN		(0.3f)									// �S�[���̓����x�̍ŏ��l

//==========================================
// �R���X�g���N�^
//==========================================
CGoal::CGoal() : CObject3D(CObject::TYPE_GOAL, CObject::PRIORITY_PLAYER)
{
	// �R���X�g���N�^�̔�
	Box();
}

//==========================================
// �I�[�o�[���[�h�R���X�g���N�^
//==========================================
CGoal::CGoal(CObject::TYPE type, CObject::PRIORITY priority) : CObject3D(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//==========================================
// �f�X�g���N�^
//==========================================
CGoal::~CGoal()
{

}

//==========================================
// �R���X�g���N�^�̔�
//==========================================
void CGoal::Box(void)
{
	// �S�Ă̒l���N���A����
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// �F
	m_fSubCol = 0.0f;								// �����x�̌��Z��
}

//==========================================
// �S�[���̏���������
//==========================================
HRESULT CGoal::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// �F
	m_fSubCol = 0.0f;								// �����x�̌��Z��

	// ������Ԃ�
	return S_OK;
}

//========================================
// �S�[���̏I������
//========================================
void CGoal::Uninit(void)
{
	// �I������
	CObject3D::Uninit();
}

//===========================================
// �S�[���̕`�揈��
//===========================================
void CGoal::Draw(void)
{
	// �`�揈��
	CObject3D::DrawLightOff();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CGoal::SetData(const D3DXVECTOR3& pos)
{
	// �S�Ă̒l������������
	m_col = GOAL_INIT_COLOR;					// �F
	m_fSubCol = GOAL_ADD_ALPHA;					// �����x�̒ǉ���

	// �X�N���[���̐ݒ菈��
	SetPos(pos);								// �ʒu�ݒ�
	SetPosOld(pos);								// �O��̈ʒu�ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �����ݒ�
	SetSize(GOAL_SIZE);							// �T�C�Y�ݒ�

	// ���_���̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(m_col);

	// �e�N�X�`�����W�̐ݒ菈��(�c�̈��Ԋu�e�N�X�`��)
	SetVtxTextureHeight(D3DXVECTOR2(GetSize().x, GetSize().y));

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::GetTexture()->Regist(GOAL_TEXTURE));
}

//===========================================
// �����x�̕ω�
//===========================================
void CGoal::AlphaChange(void)
{
	// �����x�����Z����
	m_col.a -= m_fSubCol;

	if (m_col.a >= GOAL_ALPHA_MAX)
	{ // �ő�l����������ꍇ

		// �����x��␳����
		m_col.a = GOAL_ALPHA_MAX;

		// �����x�̌��Z�ʂ� -1 ��������
		m_fSubCol *= -1;
	}
	else if (m_col.a <= GOAL_ALPHA_MIN)
	{ // �ŏ��l����������ꍇ

		// �����x��␳����
		m_col.a = GOAL_ALPHA_MIN;

		// �����x�̌��Z�ʂ� -1 ��������
		m_fSubCol *= -1;
	}

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(m_col);
}

//===========================================
// ��������
//===========================================
CGoal* CGoal::Create(const D3DXVECTOR3& pos, const CScene::MODE mode)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CGoal* pGoal = nullptr;	// �t�B�[���h�̃C���X�^���X�𐶐�

	if (pGoal == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		switch (mode)
		{
		case CScene::MODE_TUTORIAL:	// �`���[�g���A���̏ꍇ

			// �I�u�W�F�N�g�𐶐�
			pGoal = new CTutorialGoal;

			break;

		case CScene::MODE_GAME:		// �Q�[�����[�h�̏ꍇ

			// �I�u�W�F�N�g�𐶐�
			pGoal = new CGameGoal;

			break;

		default:					// ��L�ȊO

			// NULL ��Ԃ�
			return nullptr;

			break;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pGoal != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pGoal->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�S�[���̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pGoal->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �S�[���̃|�C���^��Ԃ�
	return pGoal;
}