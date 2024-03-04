//===========================================
//
// ���̍��̌��̃��C������[gold_bone_light.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "gold_bone_light.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* FIRE_TEXTURE = "data\\TEXTURE\\BoneLight.png";		// �e�N�X�`��
	const float FIRE_CIRCUM = 50.0f;				// ���a
	const float FIRE_HEIGHT = 10.0f;				// ����
	const CMesh::SGrid FIRE_DIVI = { 8,1 };			// ������

	const float ADD_HEIGHT = 1000.0f;				// �����̉��Z��
	const float MAX_HEIGHT = 20000.0f;				// �����̍ő�l
	const float ADD_EXTEND_ROT = 0.01f;				// �g���Ԓ��̌����̉��Z��
	const float SUB_CIRCUM = 2.0f;					// �~���̌��Z��
	const float INIT_POS_Y = -500.0f;				// �����ʒu(Y��)
	const float TEX_V_MEMORY = 1.0f / MAX_HEIGHT;	// �e�N�X�`��(V�l)��1���������̒l
}

//==============================
// �R���X�g���N�^
//==============================
CGoldBoneLight::CGoldBoneLight() : CMeshCylinder(CObject::PRIORITY_ENTITY)
{
	// ��ނ�ݒ肷��
	CObject::SetType(TYPE_NONE);

	// �S�Ă̒l���N���A����
	m_state = STATE_EXTEND;		// ���
}

//==============================
// �f�X�g���N�^
//==============================
CGoldBoneLight::~CGoldBoneLight()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CGoldBoneLight::Init(void)
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
void CGoldBoneLight::Uninit(void)
{
	// �I������
	CMeshCylinder::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CGoldBoneLight::Update(void)
{
	switch (m_state)
	{
	case CGoldBoneLight::STATE_EXTEND:

		// �L�я�ԏ���
		Extend();

		// �e�N�X�`�����W�̐ݒ菈��
		SetTex();

		break;

	case CGoldBoneLight::STATE_SHRINK:

		// �k������
		Shrink();

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ���_���W�̐ݒ菈��
	SetVertex();
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CGoldBoneLight::Draw(void)
{
	// �`�揈��
	CMeshCylinder::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CGoldBoneLight::SetData(const D3DXVECTOR3& pos)
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
	m_state = STATE_EXTEND;		// ���
}

//=======================================
// ��������
//=======================================
CGoldBoneLight* CGoldBoneLight::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CGoldBoneLight* pFire = nullptr;		// �C���X�^���X�𐶐�

	if (pFire == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pFire = new CGoldBoneLight;
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
// ��Ԃ̐ݒ菈��
//=======================================
void CGoldBoneLight::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CGoldBoneLight::STATE CGoldBoneLight::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// �L�я�ԏ���
//=======================================
void CGoldBoneLight::Extend(void)
{
	// �����ƍ������擾
	D3DXVECTOR3 rot = GetRot();
	float fHeight = GetHeight();

	// ���������Z
	fHeight += ADD_HEIGHT;

	// ���������Z����
	rot.y += ADD_EXTEND_ROT;

	if (fHeight >= MAX_HEIGHT)
	{ // �������ő�l�ȏ�ɂȂ����ꍇ

		// ������␳����
		fHeight = MAX_HEIGHT;
	}

	// �����̐��K��
	useful::RotNormalize(&rot.y);

	// ������K�p
	SetRot(rot);
	SetHeight(fHeight);
}

//=======================================
// �k�ݏ�ԏ���
//=======================================
void CGoldBoneLight::Shrink(void)
{
	// �~�����擾����
	float fCircum = GetCircum();

	// �~�������Z����
	fCircum -= SUB_CIRCUM;

	if (fCircum <= 0.0f)
	{ // �~����0�ȉ��ɂȂ����ꍇ

		// �~����0�ɂ���
		fCircum = 0.0f;
	}

	// �~����K�p����
	SetCircum(fCircum);
}

//=======================================
// �e�N�X�`�����W�̐ݒ菈��
//=======================================
void CGoldBoneLight::SetTex(void)
{
	// �e�N�X�`���̍����ݒ菈��
	float fHeight = GetHeight();

	// �e�N�X�`�����W�̐ݒ菈��
	SetVtxTexture(TEX_V_MEMORY * fHeight);
}