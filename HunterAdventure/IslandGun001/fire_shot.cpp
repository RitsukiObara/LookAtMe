//===========================================
//
// ���U���̃��C������[fire_shot.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "fire_shot.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* FIRE_TEXTURE = "data\\TEXTURE\\BombExplosion.png";		// �e�N�X�`��
	const float FIRE_CIRCUM = 200.0f;				// ���a
	const float FIRE_HEIGHT = 100.0f;				// ����
	const CMesh::SGrid FIRE_DIVI = { 8,1 };			// ������

	const float ADD_HEIGHT = 100.0f;				// �����̉��Z��
	const float MAX_HEIGHT = 6000.0f;				// �����̍ő�l
	const float ADD_EXTEND_ROT = 0.01f;				// �g���Ԓ��̌����̉��Z��
	const float ADD_SHRINK_ROT = 0.001f;			// �k����Ԓ��̌����̉��Z��
	const float SUB_CIRCUM = 20.0f;					// �~���̌��Z��
	const float INIT_POS_Y = -500.0f;				// �����ʒu(Y��)
	const float TEX_V_MEMORY = 1.0f / MAX_HEIGHT;	// �e�N�X�`��(V�l)��1���������̒l
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CFireShot*> CFireShot::m_list = {};		// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CFireShot::CFireShot() : CMeshCylinder(CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_state = STATE_EXTEND;			// ���

	// ��ނ�ݒ肷��
	CObject::SetType(TYPE_FIRESHOT);

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CFireShot::~CFireShot()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CFireShot::Init(void)
{
	if (FAILED(CMeshCylinder::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//========================================
//�u���b�N�̏I������
//========================================
void CFireShot::Uninit(void)
{
	// �I������
	CMeshCylinder::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CFireShot::Update(void)
{
	switch (m_state)
	{
	case CFireShot::STATE_EXTEND:

		// �L�я�ԏ���
		Extend();

		break;

	case CFireShot::STATE_SHRINK:

		// �k�ݏ�ԏ���
		if (Shrink() == true)
		{ // �k�����������ꍇ

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ���_���W�̐ݒ菈��
	SetVertex();

	// �e�N�X�`�����W�̐ݒ菈��
	SetTex();
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CFireShot::Draw(void)
{
	// �`�揈��
	CMeshCylinder::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CFireShot::SetData(const D3DXVECTOR3& pos)
{
	// ���̈ʒu
	D3DXVECTOR3 posFire = D3DXVECTOR3(pos.x, INIT_POS_Y, pos.z);

	// ���̐ݒ菈��
	CMeshCylinder::SetData
	(
		posFire,
		NONE_D3DXVECTOR3,
		FIRE_CIRCUM,
		FIRE_HEIGHT,
		FIRE_DIVI
	);

	BindTexture(CManager::Get()->GetTexture()->Regist(FIRE_TEXTURE));		// �e�N�X�`���̊��蓖�ď���
	SetLighting(true);		// ���C�e�B���O�̐ݒ菈��
	SetCulling(true);		// �J�����O�̐ݒ菈��

	// ���_���W�̐ݒ菈��
	SetVertex();

	// �S�Ă̒l��ݒ肷��
	m_state = STATE_EXTEND;			// ���
}

//=======================================
// ��������
//=======================================
CFireShot* CFireShot::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CFireShot* pFire = nullptr;		// �C���X�^���X�𐶐�

	if (pFire == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pFire = new CFireShot;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pFire != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		pFire->SetData(pos);

		// ����������
		if (FAILED(pFire->Init()))
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

	// ���̃|�C���^��Ԃ�
	return pFire;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CFireShot*> CFireShot::GetList(void)
{
	// ���X�g�̏���Ԃ�
	return m_list;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CFireShot::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CFireShot::STATE CFireShot::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// �L�я�ԏ���
//=======================================
void CFireShot::Extend(void)
{
	// �����ƍ������擾
	D3DXVECTOR3 rot = GetRot();
	float fHeight = GetHeight();

	// ���������Z����
	fHeight += ADD_HEIGHT;

	if (fHeight >= MAX_HEIGHT)
	{ // �������ő�l�ȏ�ɂȂ����ꍇ

		// ������␳����
		fHeight = MAX_HEIGHT;

		// �k����Ԃɂ���
		m_state = STATE_SHRINK;
	}

	// ���������Z����
	rot.y += ADD_EXTEND_ROT;

	// �����̐��K��
	useful::RotNormalize(&rot.y);

	// �����ƍ�����K�p����
	SetRot(rot);
	SetHeight(fHeight);
}

//=======================================
// �k�ݏ�ԏ���
//=======================================
bool CFireShot::Shrink(void)
{
	// �����Ɖ~�����擾����
	D3DXVECTOR3 rot = GetRot();
	float fCircum = GetCircum();
	bool bFinish = false;

	// �~�������Z����
	fCircum -= SUB_CIRCUM;

	if (fCircum <= 0.0f)
	{ // �~����0�ȉ��ɂȂ����ꍇ

		// �~����0�ɂ���
		fCircum = 0.0f;

		// �I���̍��}���o��
		bFinish = true;
	}

	// ���������Z����
	rot.y += fCircum * ADD_SHRINK_ROT;

	// �����̐��K��
	useful::RotNormalize(&rot.y);

	// �����Ɖ~����K�p����
	SetRot(rot);
	SetCircum(fCircum);

	// �I���󋵂�Ԃ�
	return bFinish;
}

//=======================================
// �e�N�X�`�����W�̐ݒ菈��
//=======================================
void CFireShot::SetTex(void)
{
	// �e�N�X�`���̍����ݒ菈��
	float fHeight = GetHeight();

	// �e�N�X�`�����W�̐ݒ菈��
	SetVtxTexture(TEX_V_MEMORY * fHeight);
}