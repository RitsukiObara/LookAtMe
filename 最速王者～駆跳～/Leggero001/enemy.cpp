//============================================
//
// �G�̃��C������[enemy.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "main.h"
#include "enemy.h"
#include "game.h"
#include "motion.h"
#include "renderer.h"
#include "useful.h"

#include "shot_enemy.h"
#include "kick_enemy.h"

#include "score.h"

//===========================================
// �R���X�g���N�^
//===========================================
CEnemy::CEnemy() : CCharacter(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// �R���X�g���N�^�̔�
	Box();
}

//===========================================
// �R���X�g���N�^(�I�[�o�[���[�h)
//===========================================
CEnemy::CEnemy(CObject::TYPE type, PRIORITY priority) : CCharacter(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//===========================================
// �f�X�g���N�^
//===========================================
CEnemy::~CEnemy()
{

}

//===========================================
// �R���X�g���N�^�̔�
//===========================================
void CEnemy::Box(void)
{
	// �S�Ă̒l���N���A����
	m_type = TYPE_SHOT;			// ���
	m_state = STATE_NONE;		// ���
}

#if 0

//===========================================
// ����������
//===========================================
HRESULT CEnemy::Init(const D3DXVECTOR3& pos)
{
	// ������Ԃ�
	return S_OK;
}

#endif

//===========================================
// �I������
//===========================================
void CEnemy::Uninit(void)
{
	// ���[�V�����̃��������J������
	delete m_pMotion;
	m_pMotion = nullptr;

	// �I��
	CCharacter::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CEnemy::Update(void)
{

}

//===========================================
// �`�揈��
//===========================================
void CEnemy::Draw(void)
{
	// �`�揈��
	CCharacter::Draw();
}

//===========================================
// �q�b�g����
//===========================================
void CEnemy::Hit(void)
{
	// ���S����
	m_state = STATE_DEATH;

	// ��ނ̐ݒ菈��
	SetType(TYPE_DEATHENEMY);
}

//===========================================
// ��Ԃ̐ݒ菈��
//===========================================
void CEnemy::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//===========================================
// ��Ԃ̎擾����
//===========================================
CEnemy::STATE CEnemy::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//===========================================
// ���[�V�����̐ݒ�(����)����
//===========================================
void CEnemy::SetMotion(void)
{
	// ���[�V�����̐�������
	m_pMotion = CMotion::Create();
}

//===========================================
// ���[�V�����̎擾����
//===========================================
CMotion* CEnemy::GetMotion(void)
{
	// ���[�V�����̏���Ԃ�
	return m_pMotion;
}

//===========================================
// ��������
//===========================================
CEnemy* CEnemy::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CEnemy* pEnemy = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pEnemy == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		switch (type)
		{
		case TYPE_SHOT:			// �e�����G

			// �e�����G�𐶐�
			pEnemy = new CShotEnemy;

			break;

		case TYPE_KICK:			// ��Q��

			// ��Q���L�b�N�G�𐶐�
			pEnemy = new CKickEnemy;

			break;

		default:

			// �x����
			MessageBox(NULL, "�G�̐����ŋK��O�̐��l�����o����܂����I", "�x���I", MB_ICONWARNING);

			break;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEnemy != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEnemy->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�G�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ��������������
		pEnemy->m_type = type;			// ���
		pEnemy->m_state = STATE_NONE;	// ���

		// ���̐ݒ菈��
		pEnemy->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pEnemy;
}