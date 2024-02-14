//===========================================
//
// ���e�̃��C������[bomb.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "bomb.h"
#include "texture.h"
#include "collision.h"
#include "useful.h"

#include "bomb_fuse.h"
#include "objectElevation.h"
#include "bomb_explosion.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\BombBody.x";		// ���f���̖��O
	const float GROWTH_SCALE_DEST = 1.0f;				// �������̖ڕW�̊g�嗦
	const float GROWTH_SCALE_ADD = 0.01f;				// �������̒ǉ��̊g�嗦
	const float GRAVITY = 0.5f;							// �d��
	const float BOUND_GRAVITY_MAGNI = -0.7f;			// �o�E���h�̏d�͂̔{��
	const int BOUND_COUNT = 3;							// �o�E���h�̉�
	const float ROT_DEST = 0.0f;						// �ړI�̌���
	const float ROT_CORRECT = 0.1f;						// �����̕␳�{��
	const float FUSE_EXPLOSION_POS_Y = 81.0f;			// ���ΐ��̔�������ʒu
	const float EXPLO_SCALE_MAGNI = 0.003f;				// �������̊g�嗦�̔{��
	const int EXPLO_COUNT = 240;						// ������Ԃ̃J�E���g
	const float EXPLO_SCALE_MAX = 3.0f;					// �������̊g�嗦�̍ő�l
	const float EXPLO_SCALE_MIN = 0.1f;					// �������̊g�嗦�̍ŏ��l
	const D3DXCOLOR COL_ADD =							// �F�̉��Z��
	{
		0.834f / EXPLO_COUNT,
		-0.166f / EXPLO_COUNT,
		-0.166f / EXPLO_COUNT,
		0.0f
	};
	const float SMASH_HEIGHT = 8.0f;			// ������ԍ���
	const float SMASH_FUSE_SUB_MABNI = 0.1f;	// ������ю��̓��ΐ��̌��Z��
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CBomb*> CBomb::m_list = {};			// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CBomb::CBomb() : CModel(TYPE_NONE, PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pFuse = nullptr;			// ���ΐ��̏��
	m_col = NONE_D3DXCOLOR;		// �F
	m_move = NONE_D3DXVECTOR3;	// �ړ���
	m_state = STATE_GROWTH;		// ���
	m_nBoundCount = 0;			// �o�E���h�J�E���g
	m_nExplosionCount = 0;		// �����J�E���g
	m_bAdd = false;				// ���Z��

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CBomb::~CBomb()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CBomb::Init(void)
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
void CBomb::Uninit(void)
{
	if (m_pFuse != nullptr)
	{ // ���ΐ��� NULL ����Ȃ��ꍇ

		// �I������
		m_pFuse->Uninit();
		m_pFuse = nullptr;
	}

	// �I������
	CModel::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CBomb::Update(void)
{
	// �O��̈ʒu��ݒ肷��
	SetPosOld(GetPos());

	switch (m_state)
	{
	case CBomb::STATE_GROWTH:		// �������

		// ��������
		Growth();

		break;

	case CBomb::STATE_RIPEN:		// ������

		break;

	case CBomb::STATE_BOUND:		// �o�E���h���

		// �o�E���h����
		Bound();

		break;

	case CBomb::STATE_DETONATION:	// �N�����

		// �N����ԏ���
		Detonation();

		// �ړ�����
		Move();

		// �d�͏���
		Gravity();

		// �����蔻��
		Collision();

		// �N���n�ʂƂ̓����蔻��
		ElevationCollision();

		break;

	case CBomb::STATE_EXPLOSION:	// �������

		// ������ԏ���
		if (Explosion() == true)
		{
			// �����̐���
			CBombExplosion::Create(GetPos());
				
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
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CBomb::Draw(void)
{
	switch (m_state)
	{
	case CBomb::STATE_EXPLOSION:

		// �`�揈��
		CModel::Draw(m_col);

		break;

	default:

		// �`�揈��
		CModel::Draw();

		break;
	}

	if (m_pFuse != nullptr)
	{ // ���ΐ��� NULL ����Ȃ��ꍇ

		// ���ΐ��̕`�揈��
		m_pFuse->Draw();
	}
}

//=====================================
// �q�b�g����
//=====================================
void CBomb::Hit(const float fRot, const float fSpeed)
{
	// �ړ��ʂ�ݒ肷��
	m_move.x = sinf(fRot) * fSpeed;
	m_move.y = SMASH_HEIGHT;
	m_move.z = cosf(fRot) * fSpeed;

	// �ʒu���擾����
	D3DXVECTOR3 pos = m_pFuse->GetPos();

	// �ʒu��������
	pos.y -= fSpeed * SMASH_FUSE_SUB_MABNI;

	// �ʒu��K�p����
	m_pFuse->SetPos(pos);
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CBomb::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(rot);					// ����
	SetScale(NONE_D3DXVECTOR3);		// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_pFuse = CBombFuse::Create(GetMatrixPoint());		// ���ΐ��̏��
	m_col = NONE_D3DXCOLOR;			// �F
	m_move = NONE_D3DXVECTOR3;		// �ړ���
	m_state = STATE_GROWTH;			// ���
	m_nBoundCount = 0;				// �o�E���h�J�E���g
	m_nExplosionCount = 0;			// �����J�E���g
	m_bAdd = false;					// ���Z��
}

//=======================================
// ��������
//=======================================
CBomb* CBomb::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBomb* pBomb = nullptr;			// ���e�𐶐�

	if (pBomb == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �u���b�N�𐶐�����
		pBomb = new CBomb;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBomb != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBomb->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBomb->SetData(pos, rot);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���e�̃|�C���^��Ԃ�
	return pBomb;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CBomb*> CBomb::GetList(void)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_list;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CBomb::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CBomb::STATE CBomb::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// �����̈ړ�����
//=======================================
void CBomb::RotMove(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// �␳����
	useful::FrameCorrect(ROT_DEST, &rot.x, ROT_CORRECT);

	// �����̐��K��
	useful::RotNormalize(&rot.x);

	// ������K�p����
	SetRot(rot);
}

//=======================================
// �ړ�����
//=======================================
void CBomb::Move(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �ړ�������
	pos.x += m_move.x;
	pos.z += m_move.z;

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// �d�͏���
//=======================================
void CBomb::Gravity(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �d�͏���
	useful::Gravity(&m_move.y, &pos.y, GRAVITY);

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// �N���n�ʂ̓����蔻�菈��
//=======================================
bool CBomb::ElevationCollision(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	float fHeight = 0.0f;			// ����

	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;		// �擪�̏���
	CElevation* pElevEnd = nullptr;		// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pElev = list.GetTop();

		// �����̒l���擾����
		pElevEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �����蔻������
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // �����蔻��̈ʒu�����������ꍇ

				// ������ݒ肷��
				pos.y = fHeight;

				// �ʒu���X�V����
				SetPos(pos);

				// �ړ��ʂ�0�ɂ���
				m_move = NONE_D3DXVECTOR3;

				// true ��Ԃ�
				return true;
			}

			if (pElev == pElevEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pElev = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �ʒu���X�V����
	SetPos(pos);

	// false ��Ԃ�
	return false;
}

//=======================================
// �����蔻�菈��
//=======================================
void CBomb::Collision(void)
{
	D3DXVECTOR3 pos = GetPos();					// �ʒu
	D3DXVECTOR3 posOld = GetPosOld();			// �O��̈ʒu
	D3DXVECTOR3 vtxMax = GetFileData().vtxMax;	// �ő�l
	D3DXVECTOR3 vtxMin = GetFileData().vtxMin;	// �ŏ��l

	// �����蔻��
	collision::RockCollision(&pos, vtxMax.x, vtxMax.y);
	collision::TreeCollision(&pos, vtxMax.x);
	collision::WallCollision(&pos, posOld, vtxMax, vtxMin);
	collision::BlockHit(&pos, posOld, vtxMax, vtxMin);

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// ������ԏ���
//=======================================
void CBomb::Growth(void)
{
	// �g�嗦���擾����
	D3DXVECTOR3 scale = GetScale();

	// �g�傷��
	if (useful::FrameCorrect(GROWTH_SCALE_DEST, &scale.x, GROWTH_SCALE_ADD) == true ||
		useful::FrameCorrect(GROWTH_SCALE_DEST, &scale.y, GROWTH_SCALE_ADD) == true ||
		useful::FrameCorrect(GROWTH_SCALE_DEST, &scale.z, GROWTH_SCALE_ADD) == true)
	{ // �g�傪�I������ꍇ

		// �����Ԃɂ���
		m_state = STATE_RIPEN;

		// �g�嗦��ݒ肷��
		scale = NONE_SCALE;
	}

	// �g�嗦��ݒ肷��
	SetScale(scale);
}

//=======================================
// �o�E���h��ԏ���
//=======================================
void CBomb::Bound(void)
{
	// �����̈ړ�����
	RotMove();

	// �d�͏���
	Gravity();

	if (ElevationCollision() == true)
	{ // �N���n�ʂɓ��������ꍇ

		// �o�E���h�񐔂����Z����
		m_nBoundCount++;

		// �o�E���h������
		m_move.y *= BOUND_GRAVITY_MAGNI;
	}

	if (m_nBoundCount >= BOUND_COUNT)
	{ // �o�E���h�J�E���g����萔�ȏ�ɂȂ����ꍇ

		// �o�E���h�J�E���g�� 0 �ɂ���
		m_nBoundCount = 0;

		// �d�͂� 0 �ɂ���
		m_move.y = 0.0f;

		// �N����Ԃɂ���
		m_state = STATE_DETONATION;
	}
}

//=======================================
// �N����ԏ���
//=======================================
void CBomb::Detonation(void)
{
	if (m_pFuse != nullptr)
	{ // ���ΐ��� NULL ����Ȃ��ꍇ

		// ���ΐ��̍X�V����
		m_pFuse->Update();

		if (m_pFuse->GetPos().y <= FUSE_EXPLOSION_POS_Y)
		{ // ���ΐ������̈ʒu�܂ŉ��������ꍇ

			// �I������
			m_pFuse->Uninit();
			m_pFuse = nullptr;

			// �N����Ԃɂ���
			m_state = STATE_EXPLOSION;

			// �F���擾����
			m_col = GetMaterial(0).MatD3D.Diffuse;
		}
	}
}

//=======================================
// ������ԏ���
//=======================================
bool CBomb::Explosion(void)
{
	// �g�嗦���擾����
	D3DXVECTOR3 scale = GetScale();

	// �����J�E���g�����Z����
	m_nExplosionCount++;

	if (m_bAdd == true)
	{ // ���Z�󋵂� true �̏ꍇ
		
		// �g�傷��
		scale.x += EXPLO_SCALE_MAGNI * m_nExplosionCount;
		scale.y += EXPLO_SCALE_MAGNI * m_nExplosionCount;
		scale.z += EXPLO_SCALE_MAGNI * m_nExplosionCount;

		if (scale.x >= EXPLO_SCALE_MAX ||
			scale.y >= EXPLO_SCALE_MAX ||
			scale.z >= EXPLO_SCALE_MAX)
		{ // �g�嗦���������ꍇ

			// ���Z�󋵂ɂ���
			m_bAdd = false;

			// �g�嗦��␳����
			scale = NONE_SCALE;
		}
	}
	else
	{ // ��L�ȊO

		// �k������
		scale.x -= EXPLO_SCALE_MAGNI * m_nExplosionCount;
		scale.y -= EXPLO_SCALE_MAGNI * m_nExplosionCount;
		scale.z -= EXPLO_SCALE_MAGNI * m_nExplosionCount;

		if (scale.x <= EXPLO_SCALE_MIN ||
			scale.y <= EXPLO_SCALE_MIN ||
			scale.z <= EXPLO_SCALE_MIN)
		{ // �g�嗦���������ꍇ

			// ���Z�󋵂ɂ���
			m_bAdd = true;

			// �g�嗦��␳����
			scale = NONE_D3DXVECTOR3;
		}
	}

	// �g�嗦��K�p����
	SetScale(scale);

	// �F��ݒ肷��
	m_col += COL_ADD;

	if (m_nExplosionCount >= EXPLO_COUNT)
	{ // �����J�E���g����萔�ɂȂ����ꍇ

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}