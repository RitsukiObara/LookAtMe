//=======================================
//
// �e�̃��C������[bullet.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "bullet.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "locus3D.h"
#include "collision.h"

//=======================================
// �������O���
//=======================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Bullet.png";		// �e�̃e�N�X�`��
	const D3DXVECTOR3 RADIUS[CBullet::TYPE_MAX] =
	{
		D3DXVECTOR3(20.0f,20.0f,0.0f),		// �񒚌��e
		D3DXVECTOR3(10.0f,10.0f,0.0f),		// �U�e�e
	};
	const float SPEED = 45.0f;		// ���x
	const int LIFE = 90;			// ����
	const int DAMAGE[CBullet::TYPE_MAX] =	// �_���[�W
	{
		8,
		2,
	};
	const float KNOCKBACK[CBullet::TYPE_MAX] =	// �m�b�N�o�b�N�̒l
	{
		2.0f,
		15.0f,
	};
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CBullet*> CBullet::m_list = {};		// ���X�g

//=========================
// �R���X�g���N�^
//=========================
CBullet::CBullet() : CBillboard(CObject::TYPE_BULLET, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_move = NONE_D3DXVECTOR3;	// �ړ���
	m_type = TYPE_HANDGUN;		// ���
	m_nLife = LIFE;				// ����
	m_nDamage = 0;				// �_���[�W
	m_fKnockBack = 0.0f;		// �m�b�N�o�b�N�l

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//=========================
// �f�X�g���N�^
//=========================
CBullet::~CBullet()
{

}

//=========================
// ����������
//=========================
HRESULT CBullet::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_move = NONE_D3DXVECTOR3;	// �ړ���
	m_type = TYPE_HANDGUN;		// ���
	m_nLife = LIFE;				// ����
	m_nDamage = 0;				// �_���[�W
	m_fKnockBack = 0.0f;		// �m�b�N�o�b�N�l

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CBullet::Uninit(void)
{
	// �I��
	CBillboard::Uninit();

	// ������������
	m_list.Pull(this);
}

//=========================
// �X�V����
//=========================
void CBullet::Update(void)
{
	// �O��̈ʒu��ۑ�����
	SetPosOld(GetPos());

	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �ړ��ʂ�ݒ肷��
	pos += m_move;

	// �ʒu��K�p����
	SetPos(pos);

	// ���������Z����
	m_nLife--;

	if (m_nLife % 2 == 0)
	{ // ��莞�Ԃ��Ƃ�

		// �c���̐���
		CLocus3D::Create(GetPos(), GetSize(), 0.5f, 20, GetTexIdx(), true, true);
	}

	if (m_nLife <= 0)
	{ // ������0�ɂȂ����ꍇ

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	if (Hit() == true)
	{ // �����ɓ��������ꍇ

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// ���_���W�̐ݒ菈��
	SetVertex();
}

//=========================
// �`�揈��
//=========================
void CBullet::Draw(void)
{
	// �`�揈��
	CBillboard::DrawLightOff(true);
}

//=========================
// ���̐ݒ菈��
//=========================
void CBullet::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ���̐ݒ菈��
	SetPos(pos);			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetRot(0.0f);			// ����
	SetSize(RADIUS[type]);	// �T�C�Y
	SetAngle();				// ����
	SetLength();			// ����
	SetEnableLookY(true);	// Y����

	// �S�Ă̒l��ݒ肷��
	m_type = type;				// ���
	m_nLife = LIFE;				// ����
	m_nDamage = DAMAGE[m_type];	// �_���[�W
	m_fKnockBack = KNOCKBACK[m_type];		// �m�b�N�o�b�N�l

	// �ړ��ʂ�ݒ肷��
	m_move.x = sinf(rot.y) * SPEED;	// X��
	m_move.y = cosf(rot.x) * SPEED;	// Y��
	m_move.z = cosf(rot.y) * SPEED;	// Z��

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
}

//=========================
// ��������
//=========================
CBullet* CBullet::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBullet* pBullet = nullptr;	// �e�̃C���X�^���X�𐶐�

	if (pBullet == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBullet = new CBullet;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBullet != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBullet->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBullet->SetData(pos, rot, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �e�̃|�C���^��Ԃ�
	return pBullet;
}

//=========================
// �_���[�W�̎擾����
//=========================
int CBullet::GetDamage(void) const
{
	// �_���[�W��Ԃ�
	return m_nDamage;
}

//=========================
// �m�b�N�o�b�N�l�̎擾����
//=========================
float CBullet::GetKnockBack(void) const
{
	// �m�b�N�o�b�N�l��Ԃ�
	return m_fKnockBack;
}

//=========================
// ���X�g�̎擾����
//=========================
CListManager<CBullet*> CBullet::GetList(void)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_list;
}

//=========================
// �q�b�g����
//=========================
bool CBullet::Hit(void)
{
	D3DXVECTOR3 pos = GetPos();			// �ʒu
	D3DXVECTOR3 posOld = GetPosOld();	// �O��̈ʒu
	D3DXVECTOR3 size = GetSize();		// �T�C�Y
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(size.x, size.y, size.x);		// ���_�̍ő�l
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-size.x, -size.y, -size.x);	// ���_�̍ŏ��l

	// �G�Əe�̓����蔻��
	if (collision::EnemyHitToGun(*this) == true ||
		collision::BangFlowerHit(pos, size.x, size.y) == true ||
		collision::BombHitToGun(pos, posOld, size.x) == true ||
		collision::BlockHit(&pos, posOld, vtxMax, vtxMin) == true ||
		collision::RockCollision(&pos, vtxMax.x, vtxMax.y) == true ||
		collision::TreeCollision(&pos, vtxMax.x) == true ||
		collision::WallCollision(&pos, posOld, vtxMax, vtxMin) == true)
	{ // �G�ɓ��������ꍇ

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}