//============================================
//
// �e�̃��C������[bullet.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "useful.h"

#include "collision.h"
#include "Effect.h"
#include "enemy.h"
#include "explosion.h"
#include "Particle.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define BULLET_TEXTURE		"data/TEXTURE/Bullet.png"	// �e�̃e�N�X�`��
#define BULLET_SPEED		(10.0f)						// �e�̑��x
#define BULLET_LIFE			(60)						// �e�̎���

//===========================================
// �R���X�g���N�^
//===========================================
CBullet::CBullet() : CBillboard(TYPE_BULLET, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l������������
	m_nLife = 0;								// ����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_fMoveRot = 0.0f;							// �ړ��������
	m_type = TYPE_PLAYER;						// �e�̎��
}

//===========================================
// �f�X�g���N�^
//===========================================
CBullet::~CBullet()
{

}

//===========================================
// ����������
//===========================================
HRESULT CBullet::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_nLife = BULLET_LIFE;						// ����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_fMoveRot = 0.0f;							// �ړ��������
	m_type = TYPE_PLAYER;						// �e�̎��

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CBullet::Uninit(void)
{
	// �I��
	CBillboard::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CBullet::Update(void)
{
	// ���[�J���ϐ���錾
	D3DXVECTOR3 pos = GetPos();			// �e�̈ʒu
	D3DXVECTOR3 size = GetSize();		// �e�̃T�C�Y

	// �O��̈ʒu�̐ݒ菈��
	SetPosOld(pos);

	// �ʒu�ݒ菈��
	Move();

	// ���������Z����
	m_nLife--;

	if (m_type == TYPE_PLAYER)
	{ // ��ނ��v���C���[�̒e�������ꍇ

		// �G�Ƃ̓����蔻��
		if (collision::BulletEnemyCollision(*this) == true)
		{ // �����蔻��œ��������ꍇ

			// ���e����
			Impact(pos);

			// ���̐�̏������s��Ȃ�
			return;
		}
	}

	if (m_type == TYPE_ENEMY)
	{ // ��ނ��G�̒e�������ꍇ

		// �v���C���[�Ƃ̓����蔻��
		if (collision::BulletPlayerCollision(*this) == true)
		{ // �����蔻��œ��������ꍇ

			// ���e����
			Impact(pos);

			// ���̐�̏������s��Ȃ�
			return;
		}
	}

	// �u���b�N�̓����蔻�菈��
	if (collision::BulletBlockCollision(*this) == true)
	{ // �u���b�N�ɓ��������ꍇ

		// ���e����
		Impact(pos);

		// ���̐�̏������s��Ȃ�
		return;
	}

	if (m_nLife <= 0)
	{ // �������s�����ꍇ

		// ���e����
		Impact(pos);

		// ���̐�̏������s��Ȃ�
		return;
	}

	// �X�V
	CBillboard::Update();
}

//===========================================
// �`�揈��
//===========================================
void CBullet::Draw(void)
{
	// �`�揈��
	CBillboard::DrawLightOff();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CBullet::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fMoveRot, TYPE type)
{
	// �S�Ă̒l������������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_type = type;								// ���
	m_nLife = BULLET_LIFE;						// ����
	m_fMoveRot = fMoveRot;						// �ړ�����

	// ���̐ݒ菈��
	SetPos(pos);		// �ʒu
	SetSize(size);		// �T�C�Y

	// ���_���̐ݒ�
	SetVertex();

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::GetTexture()->Regist(BULLET_TEXTURE));
}

//===========================================
// �ړ�����
//===========================================
void CBullet::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu

	// �ړ��ʂ�ݒ肷��
	m_move.x = sinf(m_fMoveRot) * BULLET_SPEED;
	m_move.z = cosf(m_fMoveRot) * BULLET_SPEED;

	// �ʒu���X�V����
	pos += m_move;

	// �ʒu��ݒ肷��
	SetPos(pos);

	// �G�t�F�N�g�̐ݒ菈��
	CEffect::Create
	(
		pos,								// �ʒu
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// �ړ���
		30,									// ����
		40.0f,								// ���a
		CEffect::TYPE_NONE					// ���
	);
}

//===========================================
// ���e����
//===========================================
void CBullet::Impact(const D3DXVECTOR3& pos)
{
	// �����̐�������
	CExplosion::Create(pos, EXPLOSION_SIZE);

	// �e�̔j������
	Uninit();

	// �p�[�e�B�N���̐�������
	CParticle::Create(pos, CEffect::TYPE_FIRE);
}

//===========================================
// �ړ������̐ݒ菈��
//===========================================
void CBullet::SetMoveRot(const float fMoveRot)
{
	// �ړ�������ݒ肷��
	m_fMoveRot = fMoveRot;
}

//===========================================
// �ړ������̎擾����
//===========================================
float CBullet::GetMoveRot(void) const
{
	// �ړ�������Ԃ�
	return m_fMoveRot;
}

//===========================================
// �ړ��ʂ̐ݒ菈��
//===========================================
void CBullet::SetMove(const D3DXVECTOR3& move)
{
	// �ړ��ʂ�ݒ肷��
	m_move = move;
}

//===========================================
// �ړ��ʂ̎擾����
//===========================================
D3DXVECTOR3 CBullet::GetMove(void) const
{
	// �ړ��ʂ�Ԃ�
	return m_move;
}

//===========================================
// ��������
//===========================================
CBullet* CBullet::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fMoveRot,TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBullet* pBullet = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pBullet == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBullet = new CBullet;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBullet != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBullet->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�e�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBullet->SetData(pos, size, fMoveRot, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pBullet;
}