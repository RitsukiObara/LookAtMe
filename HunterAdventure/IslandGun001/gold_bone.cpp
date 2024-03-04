//===========================================
//
// ���̍��̃��C������[gold_bone.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "gold_bone.h"
#include "renderer.h"
#include "useful.h"

#include "game.h"
#include "alter.h"
#include "alter_pole.h"
#include "blur_org.h"
#include "gold_bone_light.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\GoldBone.x";		// ���f���̖��O
	const float ROT_MOVE = 0.01f;			// ���f���̌����̈ړ���
	const float GET_HEIGHT = 300.0f;		// �擾���̍���
	const float ADD_HEIGHT = 30.0f;			// �����̉��Z��
	const float ADD_ROT_MOVE = 0.008f;		// �����̈ړ��ʂ̉��Z��

	const float UP_ROT_MOVE = 0.8f;			// �㏸��ԂɑJ�ڂ��鎞�̌����̈ړ���
	const float UP_DEST_HEIGHT = 5000.0f;	// �㏸��Ԃ̖ڕW�̍���
	const float UP_MOVE_Y = 100.0f;			// �㏸��Ԃ̈ړ���
	const float UP_BLUR_ALPHA = 0.6f;		// �㏸��Ԃ̎c���̓����x
	const int UP_BLUR_LIFE = 7;				// �㏸��Ԃ̎c���̎���

	const float ALTER_SET_MOVE_Y = 30.0f;	// �Ւd�ݒ��Ԃ̈ړ���
	const float ALTER_SET_ROT_MOVE = 0.0f;	// �Ւd�ݒ��Ԃ̌����̈ړ���
	const float ALTER_ROT_CORRECT = 0.02f;	// �Ւd�ݒ��Ԃ̌����̕␳��
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CGoldBone*> CGoldBone::m_list = {};		// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CGoldBone::CGoldBone() : CModel(CObject::TYPE_GOLDBONE, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pLight = nullptr;			// ���C�g�̏��
	m_state = STATE_NONE;		// ���
	m_fDestHeight = 0.0f;		// �ړI�̍���
	m_fRotMove = ROT_MOVE;		// �����̈ړ���

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CGoldBone::~CGoldBone()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CGoldBone::Init(void)
{
	if (FAILED(CModel::Init()))
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
void CGoldBone::Uninit(void)
{
	if (m_pLight != nullptr)
	{ // ���C�g�� NULL ����Ȃ��ꍇ

		// �I������
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	// �I������
	CModel::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CGoldBone::Update(void)
{
	switch (m_state)
	{
	case CGoldBone::STATE_NONE:

		break;

	case CGoldBone::STATE_GET:

		// �擾����
		Get();

		break;

	case CGoldBone::STATE_UP:

		// �㏸����
		Up();

		break;

	case CGoldBone::STATE_ALTERSET:

		// �Ւd�ݒ菈��
		AlterSet();

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ��]����
	Cycle();

	if (m_pLight != nullptr)
	{ // ���C�g�� NULL ����Ȃ��ꍇ

		// �`�揈��
		m_pLight->Update();

		if (m_pLight->GetCircum() <= 0.0f)
		{ // �~����0�ȉ��̏ꍇ

			// �I������
			m_pLight->Uninit();
			m_pLight = nullptr;
		}
	}
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CGoldBone::Draw(void)
{
	// �`�揈��
	CModel::Draw();

	if (m_pLight != nullptr)
	{ // ���C�g�� NULL ����Ȃ��ꍇ

		// �`�揈��
		m_pLight->Draw();
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CGoldBone::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos);							// �ʒu
	SetPosOld(pos);							// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);				// ����
	SetScale(NONE_SCALE);					// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));		// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_state = STATE_NONE;		// ���
	m_fDestHeight = 0.0f;		// �ړI�̍���
	m_fRotMove = ROT_MOVE;		// �����̈ړ���
}

//=======================================
// ��������
//=======================================
CGoldBone* CGoldBone::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CGoldBone* pBone = nullptr;	// �C���X�^���X�𐶐�

	if (pBone == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pBone = new CGoldBone;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBone != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBone->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBone->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

// ���̍��̃|�C���^��Ԃ�
return pBone;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CGoldBone*> CGoldBone::GetList(void)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_list;
}

//=======================================
// �q�b�g����
//=======================================
void CGoldBone::Hit(void)
{
	// �擾��Ԃɂ���
	m_state = STATE_GET;

	// �ړI�̍�����ݒ肷��
	m_fDestHeight = GetPos().y + GET_HEIGHT;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CGoldBone::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CGoldBone::STATE CGoldBone::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// ���̐ݒ菈��
//=======================================
void CGoldBone::SetLight(void)
{
	if (m_pLight == nullptr)
	{ // ���� NULL �̏ꍇ

		// ���𐶐�
		m_pLight = CGoldBoneLight::Create(GetPos());
	}
}

//=======================================
// ���̎擾����
//=======================================
CGoldBoneLight* CGoldBone::GetLight(void)
{
	// ���̏���Ԃ�
	return m_pLight;
}

//=======================================
// ��]����
//=======================================
void CGoldBone::Cycle(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// ��
	rot.y += m_fRotMove;

	// �����̐��K��
	useful::RotNormalize(&rot.y);

	// ������K�p����
	SetRot(rot);
}

//=======================================
// �擾����
//=======================================
void CGoldBone::Get(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �ʒu�����̏ꏊ�܂ŏグ��
	useful::FrameCorrect(m_fDestHeight, &pos.y, ADD_HEIGHT);

	// �ʒu��K�p����
	SetPos(pos);

	// �����̈ړ��ʂ����Z����
	m_fRotMove += ADD_ROT_MOVE;

	if (m_fRotMove >= UP_ROT_MOVE)
	{ // �����̈ړ��ʂ���萔�ȏ�ɂȂ����ꍇ

		// �㏸��Ԃɂ���
		m_state = STATE_UP;

		// �ڕW�̍�����ݒ肷��
		m_fDestHeight = UP_DEST_HEIGHT;
	}
}

//=======================================
// �㏸����
//=======================================
void CGoldBone::Up(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �ʒu�����̏ꏊ�܂ŏグ��
	if (useful::FrameCorrect(m_fDestHeight, &pos.y, UP_MOVE_Y) == true)
	{
		// �Ւd�̏����擾
		CAlter* pAlter = CGame::GetAlter();
		CAlterPole* pPole = nullptr;

		if (pAlter != nullptr)
		{ // �Ւd�� NULL ����Ȃ��ꍇ

			for (int nCnt = 0; nCnt < CAlter::NUM_POLE; nCnt++)
			{
				// �Β��̏����擾
				pPole = pAlter->GetPole(nCnt);

				if (pPole != nullptr &&
					pPole->IsEmpty() == true)
				{ // �Β��ɋ��̍��������ꍇ

					// �ʒu��ݒ肷��
					pos.x = pPole->GetPos().x;
					pos.y = pPole->GetPos().y + m_fDestHeight;
					pos.z = pPole->GetPos().z;

					// �ړI�̍�����ݒ肷��
					m_fDestHeight = pPole->GetPos().y + pPole->GetFileData().vtxMax.y;

					// �Β��̃C���f�b�N�X��ݒ肷��
					m_nPoleIdx = nCnt;

					// ���̍���������
					pPole->SetEnableEmpty(false);
					pPole->SetGoldBone(this);

					//�Ւd�ݒ��Ԃɂ���
					m_state = STATE_ALTERSET;

					// for���𔲂���
					break;
				}
			}
		}
	}

	// �ʒu��K�p����
	SetPos(pos);

	// �����̈ړ��ʂ����Z����
	m_fRotMove += ADD_ROT_MOVE;

	// �c���̐�������
	CBlurOrg::Create(pos, GetRot(), GetScale(), UP_BLUR_ALPHA, GetFileData(), UP_BLUR_LIFE, PRIORITY_EFFECT);
}

//=======================================
// �Ւd�ݒ菈��
//=======================================
void CGoldBone::AlterSet(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	if (useful::FrameCorrect(m_fDestHeight, &pos.y, ALTER_SET_MOVE_Y) == true)
	{ // �Β��ɓ��������Ƃ�

		// �Ւd�̏����擾
		CAlter* pAlter = CGame::GetAlter();
		CAlterPole* pPole = nullptr;

		if (pAlter != nullptr)
		{ // �Ւd�� NULL ����Ȃ��ꍇ

			// �Β��̏����擾
			pPole = pAlter->GetPole(m_nPoleIdx);

			if (pPole != nullptr)
			{ // �|�[���� NULL ����Ȃ��ꍇ

				// �����󋵂� true �ɂ���
				pPole->SetEnableArrival(true);
			}
		}
	}

	// �ʒu��K�p����
	SetPos(pos);

	// �����̈ړ��ʂ�0�ɂ��Ă���
	useful::RotCorrect(ALTER_SET_ROT_MOVE, &m_fRotMove, ALTER_ROT_CORRECT);
}