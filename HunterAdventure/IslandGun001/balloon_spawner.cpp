//===========================================
//
// ���D�̃��C������[balloon.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "balloon_spawner.h"
#include "renderer.h"
#include "useful.h"

#include "balloon.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const int INTERVAL_COUNT = 80;						// ���D���ďo������J�E���g
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CBalloonSpawner*> CBalloonSpawner::m_list = {};			// ���X�g���

//==============================
// �R���X�g���N�^
//==============================
CBalloonSpawner::CBalloonSpawner() : CObject(CObject::TYPE_TARGET, DIM_3D, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pBalloon = nullptr;		// ���D�̃��f��
	m_pos = NONE_D3DXVECTOR3;	// �ʒu
	m_nIntervalCount = 0;		// ���D�̊Ԋu�J�E���g

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CBalloonSpawner::~CBalloonSpawner()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CBalloonSpawner::Init(void)
{
	// �l��Ԃ�
	return S_OK;
}

//========================================
// �j�Ђ̏I������
//========================================
void CBalloonSpawner::Uninit(void)
{
	if (m_pBalloon != nullptr)
	{ // ���D�� NULL ����Ȃ��ꍇ

		// ���D�̏I������
		m_pBalloon->Uninit();
		m_pBalloon = nullptr;
	}

	// �{�̂̏I������
	Release();

	// ������������
	m_list.Pull(this);
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CBalloonSpawner::Update(void)
{
	if (m_pBalloon != nullptr)
	{ // ���D�� NULL ����Ȃ��ꍇ

		// ���D�̍X�V����
		m_pBalloon->Update();
	}
	else
	{ // ��L�ȊO

		// �Ԋu�J�E���g�����Z����
		m_nIntervalCount++;

		if (m_nIntervalCount >= INTERVAL_COUNT)
		{ // �Ԋu�J�E���g����萔�𒴂����ꍇ

			// ���D�𐶐�
			m_pBalloon = CBalloon::Create(m_pos);

			// �Ԋu�J�E���g��0�ɂ���
			m_nIntervalCount = 0;
		}
	}
}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CBalloonSpawner::Draw(void)
{
	if (m_pBalloon != nullptr)
	{ // ���D�� NULL ����Ȃ��ꍇ

		// ���D�̕`�揈��
		m_pBalloon->Draw();
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CBalloonSpawner::SetData(const D3DXVECTOR3& pos)
{
	// �S�Ă̒l��ݒ肷��
	m_pos = pos;			// �ʒu
	m_nIntervalCount = 0;	// �Ԋu�J�E���g

	// ���D�𐶐�
	m_pBalloon = CBalloon::Create(m_pos);
}

//=======================================
// �q�b�g����
//=======================================
void CBalloonSpawner::Hit(void)
{
	if (m_pBalloon != nullptr)
	{ // ���D�� NULL ����Ȃ��ꍇ

		// �I������
		m_pBalloon->Uninit();
		m_pBalloon = nullptr;
	}
}

//=======================================
// ��������
//=======================================
CBalloonSpawner* CBalloonSpawner::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBalloonSpawner* pTarget = nullptr;	// �C���X�^���X�𐶐�

	if (pTarget == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pTarget = new CBalloonSpawner;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pTarget != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pTarget->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pTarget->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�̃|�C���^��Ԃ�
	return pTarget;
}

//=======================================
// ���D�̎擾����
//=======================================
CBalloon* CBalloonSpawner::GetBalloon(void) const
{
	// ���D�̏���Ԃ�
	return m_pBalloon;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CBalloonSpawner*> CBalloonSpawner::GetList(void)
{
	// ���X�g�\���̏���Ԃ�
	return m_list;
}