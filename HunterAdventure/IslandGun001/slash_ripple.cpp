//=======================================
//
// �a���̔g��̃��C������[slash_ripple.cpp]
// Author�F��������
//
//=======================================
#include "manager.h"
#include "slash_ripple.h"
#include "renderer.h"
#include "collision.h"
#include "texture.h"
#include "useful.h"

//---------------------------------------
// �������O���
namespace
{
	const int LIFE = 140;				// ����
	const float ADD_SCALE = 0.4f;		// �g�嗦�̉��Z��
	const float SUB_ALPHA = 0.05f;		// �����x�̌��Z��
}

//==========================================
// �ÓI�����o�ϐ�
//==========================================
CListManager<CSlashRipple*> CSlashRipple::m_list = {};		// ���X�g

//==========================================
// �R���X�g���N�^
//==========================================
CSlashRipple::CSlashRipple() : CRipple(TYPE_SLASHRIPPLE, PRIORITY_PLAYER)
{
	// �S�Ă̒l���N���A����
	m_nLife = LIFE;		// ����

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==========================================
// �f�X�g���N�^
//==========================================
CSlashRipple::~CSlashRipple()
{

}

//==========================================
// �g��̏���������
//==========================================
HRESULT CSlashRipple::Init(void)
{
	if (FAILED(CRipple::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//========================================
// �g��̏I������
//========================================
void CSlashRipple::Uninit(void)
{
	// �I������
	CRipple::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
// �g��̍X�V����
//========================================
void CSlashRipple::Update(void)
{
	// �g�嗦���擾
	D3DXVECTOR3 scale = GetScale();

	// �g�嗦��ω�������
	scale.x += ADD_SCALE;
	scale.z += ADD_SCALE;

	// �����X�V����
	SetScale(scale);			// �g�嗦

	// ���e�Ƃ̓����蔻��
	collision::BombHitToSlashRipple(GetPos(), GetFileData().vtxMax.x * scale.x, GetFileData().vtxMax.y);

	// ���e�ԂƂ̓����蔻��
	collision::BangFlowerHit(GetPos(), GetFileData().vtxMax.x * scale.x, GetFileData().vtxMax.y);

	// �I(���D)�Ƃ̓����蔻��
	collision::TargetHit(GetPos(), GetFileData().vtxMax.x * scale.x);

	if (m_nLife < 0)
	{ // ������ 0 �����̏ꍇ

		// �����x���擾
		float fAlpha = GetAlpha();

		// �����x�����炷
		fAlpha -= SUB_ALPHA;

		// �����x��K�p����
		SetAlpha(fAlpha);

		if (fAlpha <= 0.0f)
		{ // �����x�� 0.0f �ȉ��̏ꍇ

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}
	}
	else
	{ // ��L�ȊO

		// ���������炷
		m_nLife--;
	}
}

//===========================================
// �g��̕`�揈��
//===========================================
void CSlashRipple::Draw(void)
{
	// �`�揈��
	CRipple::Draw();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CSlashRipple::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���̐ݒ菈��
	CRipple::SetData(pos, rot, NONE_SCALE);

	// �S�Ă̒l��ݒ肷��
	m_nLife = LIFE;		// ����
}

//===========================================
// ��������
//===========================================
CSlashRipple* CSlashRipple::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CSlashRipple* pRipple = nullptr;	// �g��̃C���X�^���X�𐶐�

	if (pRipple == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pRipple = new CSlashRipple;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRipple != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pRipple->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pRipple->SetData(pos, rot);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �g��̃|�C���^��Ԃ�
	return pRipple;
}

//===========================================
// ���X�g�̎擾����
//===========================================
CListManager<CSlashRipple*> CSlashRipple::GetList(void)
{
	// ���X�g�̏���Ԃ�
	return m_list;
}