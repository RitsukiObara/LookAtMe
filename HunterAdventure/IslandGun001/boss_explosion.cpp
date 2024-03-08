//===========================================
//
// �{�X�̔�������[boss_explosion.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "boss_explosion.h"
#include "texture.h"
#include "useful.h"

#include "mesh_sphere.h"
#include "mesh_donut.h"

//-------------------------------------------
// �萔��`
//-------------------------------------------
namespace
{
	const float EXPLOSION_CIRCUM = 300.0f;			// �����̉~��
	const float EXPLOSION_HEIGHT = 300.0f;			// �����̍���
	const CMesh::SGrid EXPLOSION_DIVI = { 16,16 };	// �����̕�����
	const char* EXPLOSION_TEXTURE = "data\\TEXTURE\\BombExplosion.png";		// �����̃e�N�X�`��

	const D3DXVECTOR3 SHOCK_ROT[CBossExplosion::NUM_SHOCK] =	// �Ռ��̌���
	{
		D3DXVECTOR3(D3DX_PI * 0.25f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(D3DX_PI * -0.25f, D3DX_PI * 0.5f, 0.0f),
	};
	const float SHOCK_CIRCUM = 400.0f;				// �Ռ��̉~��
	const float SHOCK_WIDTH = 100.0f;				// �Ռ��̕�
	const CMesh::SGrid SHOCK_DIVI = { 1,16 };		// �Ռ��̕�����
	const char* SHOCK_TEXTURE = "data\\TEXTURE\\BossRipple.png";			// �Ռ��̃e�N�X�`��

	const float INIT_ADD_EXPLOSION = 30.0f;			// �����̏����g�嗦
	const float INIT_ADD_SHOCK = 30.0f;				// �Ռ��̏����g�嗦
	const float EXPLOSION_CORRECT = 0.2f;			// �����̕␳��
	const float SHOCK_CORRECT = 0.1f;				// �Ռ��̕␳��
	const float INIT_ALPHA = 1.0f;					// �����x�̏����l
	const float SUB_ALPHA = 0.01f;					// �����x�̌��Z��
}

//==============================
// �R���X�g���N�^
//==============================
CBossExplosion::CBossExplosion() : CObject(CObject::TYPE_EXPLOSION, DIM_3D, CObject::PRIORITY_EFFECT)
{
	// �S�Ă̒l���N���A����
	m_pExplosion = nullptr;						// �����̏��
	memset(m_apShock, 0, sizeof(m_apShock));	// �Ռ��̏��
	m_fAddExpl = INIT_ADD_EXPLOSION;			// �����̒ǉ���
	m_fAddShock = INIT_ADD_SHOCK;				// �Ռ��̒ǉ���
	m_fAlpha = INIT_ALPHA;						// �����x
}

//==============================
// �f�X�g���N�^
//==============================
CBossExplosion::~CBossExplosion()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CBossExplosion::Init(void)
{
	// �l��Ԃ�
	return S_OK;
}

//========================================
// �j�Ђ̏I������
//========================================
void CBossExplosion::Uninit(void)
{
	if (m_pExplosion != nullptr)
	{ // ������ NULL ����Ȃ��ꍇ

		// �����̏I������
		m_pExplosion->Uninit();
		m_pExplosion = nullptr;
	}

	for (int nCnt = 0; nCnt < NUM_SHOCK; nCnt++)
	{
		if (m_apShock[nCnt] != nullptr)
		{ // �Ռ��� NULL ����Ȃ��ꍇ

			// �Ռ��̏I������
			m_apShock[nCnt]->Uninit();
			m_apShock[nCnt] = nullptr;
		}
	}

	// �{�̂̏�������
	Release();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CBossExplosion::Update(void)
{
	if (m_pExplosion !=  nullptr)
	{ // ������ NULL ����Ȃ��ꍇ

		// �~���ƍ������擾
		float fCircum = m_pExplosion->GetCircum();
		float fHeight = m_pExplosion->GetHeight();

		// �~���L����
		fCircum += m_fAddExpl;
		fHeight += m_fAddExpl;

		// �~���ƍ�����K�p
		m_pExplosion->SetCircum(fCircum);
		m_pExplosion->SetHeight(fHeight);

		// ���_���̐ݒ菈��
		m_pExplosion->SetVertex();
		m_pExplosion->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
	}

	for (int nCnt = 0; nCnt < NUM_SHOCK; nCnt++)
	{
		if (m_apShock[nCnt] != nullptr)
		{ // �Ռ��� NULL ����Ȃ��ꍇ

			// �~���ƕ����擾
			float fCircum = m_apShock[nCnt]->GetCircum();

			// �~���L����
			fCircum += m_fAddShock;

			// �~���ƍ�����K�p
			m_apShock[nCnt]->SetCircum(fCircum);

			// ���_���̐ݒ菈��
			m_apShock[nCnt]->SetVertex();
		}
	}

	// �ϓ��ȕ␳����
	useful::FrameCorrect(0.0f, &m_fAddExpl, EXPLOSION_CORRECT);
	useful::FrameCorrect(0.0f, &m_fAddShock, SHOCK_CORRECT);

	if (m_fAlpha > 0.0f)
	{ // �����x��0���߂̏ꍇ

		// �����x�����Z����
		m_fAlpha -= SUB_ALPHA;
	}
	else
	{ // ��L�ȊO

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CBossExplosion::Draw(void)
{
	if (m_pExplosion != nullptr)
	{ // ������ NULL ����Ȃ��ꍇ

		// �����̕`�揈��
		m_pExplosion->Draw();
	}

	for (int nCnt = 0; nCnt < NUM_SHOCK; nCnt++)
	{
		if (m_apShock[nCnt] != nullptr)
		{ // �Ռ��� NULL ����Ȃ��ꍇ

			// �Ռ��̕`�揈��
			m_apShock[nCnt]->Draw();
		}
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CBossExplosion::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// �S�Ă̒l��ݒ肷��
	if (m_pExplosion == nullptr)
	{ // ������ NULL �̏ꍇ

		// �����𐶐�
		m_pExplosion = CMeshSphere::Create(pos, NONE_D3DXVECTOR3, EXPLOSION_CIRCUM, EXPLOSION_HEIGHT, EXPLOSION_DIVI, PRIORITY_EFFECT);

		// ���̐ݒ菈��
		m_pExplosion->CObject::SetType(TYPE_NONE);	// ���
		m_pExplosion->BindTexture(CManager::Get()->GetTexture()->Regist(EXPLOSION_TEXTURE));		// �e�N�X�`���̊��蓖��
		m_pExplosion->SetCulling(true);				// �J�����O��
		m_pExplosion->SetLighting(true);			// ���C�e�B���O��
	}

	for (int nCnt = 0; nCnt < NUM_SHOCK; nCnt++)
	{
		if (m_apShock[nCnt] == nullptr)
		{ // �Ռ��� NULL �̏ꍇ

			// �Ռ��𐶐�
			m_apShock[nCnt] = CMeshDonut::Create(pos, rot + SHOCK_ROT[nCnt], SHOCK_CIRCUM, SHOCK_WIDTH, SHOCK_DIVI, PRIORITY_EFFECT);

			// ���̐ݒ菈��
			m_apShock[nCnt]->CObject::SetType(TYPE_NONE);	// ���
			m_apShock[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(SHOCK_TEXTURE));		// �e�N�X�`���̊��蓖��
			m_apShock[nCnt]->SetCulling(true);				// �J�����O��
			m_apShock[nCnt]->SetLighting(true);				// ���C�e�B���O��
		}
	}

	m_fAddExpl = INIT_ADD_EXPLOSION;		// �����̒ǉ���
	m_fAddShock = INIT_ADD_SHOCK;			// �Ռ��̒ǉ���
	m_fAlpha = INIT_ALPHA;					// �����x
}

//=======================================
// ��������
//=======================================
CBossExplosion* CBossExplosion::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBossExplosion* pExpl = nullptr;	// �C���X�^���X�𐶐�

	if (pExpl == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pExpl = new CBossExplosion;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pExpl != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pExpl->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pExpl->SetData(pos, rot);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �����̃|�C���^��Ԃ�
	return pExpl;
}