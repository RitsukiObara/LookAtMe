//===========================================
//
// �v���C���[�̍s���̃��C������[player_action.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "player_action.h"
#include "renderer.h"

#include "player.h"
#include "handgun.h"
#include "dagger.h"
#include "slash_ripple.h"
#include "chara_blur.h"
#include "collision.h"
#include "bulletUI.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const float DODGE_SPEED = 20.0f;	// �����Ԃ̑��x
	const int DODGE_COUNT = 27;			// �����Ԃ̃J�E���g��
	const int DAGGER_COUNT = 35;		// �_�K�[��Ԃ̃J�E���g��
	const int SWOOP_COUNT = 50;			// �}�~����Ԃ̃J�E���g��
	const int DODGE_BLUR_LIFE = 10;		// �����Ԃ̃u���[�̎���
	const D3DXCOLOR DODGE_BLUR_COL = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);		// �����Ԃ̃u���[�̐F
	const float DAGGER_HEIGHT = 80.0f;	// �_�K�[�̍���
	const float ATTACK_DAGGER_HEIGHT = 150.0f;	// �_�K�[�U�����̍���
	const float ATTACK_DAGGER_RADIUS = 180.0f;	// �_�K�[�U�����̔��a
	const int DAGGER_ATTACK_START = 8;	// �_�K�[�̍U�����肪�n�܂�J�E���g��
	const int DAGGER_ATTACK_END = 28;	// �_�K�[�̍U�����肪�I���J�E���g��
}

//=========================
// �R���X�g���N�^
//=========================
CPlayerAction::CPlayerAction()
{
	// �S�Ă̒l���N���A����
	m_moveRecoil = NONE_D3DXVECTOR3;	// �����̈ړ���
	m_action = ACTION_NONE;				// �s��
	m_nActionCount = 0;					// �s���J�E���g
	m_nDodgeInterval = 0;				// ����̃C���^�[�o���J�E���g
	m_fDodgeRot = 0.0f;					// ����������
	m_bDodgeUse = true;					// ����g�p�\��
	m_bRecoil = false;					// ������
}

//=========================
// �f�X�g���N�^
//=========================
CPlayerAction::~CPlayerAction()
{

}

//=========================
// �s���̏���������
//=========================
HRESULT CPlayerAction::Init(void)
{
	// �S�Ă̒l������������
	m_moveRecoil = NONE_D3DXVECTOR3;	// �����̈ړ���
	m_action = ACTION_NONE;				// �s��
	m_nActionCount = 0;					// �s���J�E���g
	m_nDodgeInterval = 0;				// ����̃C���^�[�o���J�E���g
	m_fDodgeRot = 0.0f;					// ����������
	m_bDodgeUse = true;					// ����g�p�\��
	m_bRecoil = false;					// ������

	// ������Ԃ�
	return S_OK;
}

//=========================
// �s���̏I������
//=========================
void CPlayerAction::Uninit(void)
{
	// ���g����������
	delete this;
}

//=========================
// �s���̍X�V����
//=========================
void CPlayerAction::Update(CPlayer* pPlayer)
{
	switch (m_action)
	{
	case CPlayerAction::ACTION_NONE:	// �ʏ���

		// �ʏ��ԏ���
		NoneProcess();

		break;

	case CPlayerAction::ACTION_SHOT:	// �ˌ����

		// �ˌ����
		ShotProcess(pPlayer);

		break;

	case CPlayerAction::ACTION_DAGGER:	// �_�K�[���

		// �_�K�[��ԏ���
		DaggerPrecess(pPlayer);

		break;

	case CPlayerAction::ACTION_DODGE:	// ������

		// �����ԏ���
		DodgeProcess(pPlayer);

		break;

	case CPlayerAction::ACTION_SHOTGUN:	// �U�e(J+�e)���

		// �U�e��ԏ���
		ShotgunProcess(pPlayer);

		break;

	case CPlayerAction::ACTION_SWOOP:	// �}�~��(J+�_�K�[)���

		// �}�~����ԏ���
		SwoopProcess(pPlayer);

		break;

	case CPlayerAction::ACTION_RELOAD:	// �����[�h���

		// �����[�h����
		ReloadProcess(pPlayer);

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	if (m_nDodgeInterval > 0)
	{ // ����C���^�[�o���J�E���g��0���߂̏ꍇ

		// ����C���^�[�o�������Z����
		m_nDodgeInterval--;

		if (m_nDodgeInterval <= 0)
		{ // ����C���^�[�o����0�ȏ�ɂȂ����ꍇ

			// ����̃C���^�[�o���J�E���g��0�ɂ���
			m_nDodgeInterval = 0;

			// ������g�p�ł���
			m_bDodgeUse = true;
		}
	}
}

//=========================
// ��������
//=========================
CPlayerAction* CPlayerAction::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CPlayerAction* pAction = nullptr;		// �s���̃C���X�^���X�𐶐�

	if (pAction == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pAction = new CPlayerAction;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pAction != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pAction->Init()))
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

	// �s���̃|�C���^��Ԃ�
	return pAction;
}

//=========================
// �ړ��ʂ̐ݒ菈��
//=========================
void CPlayerAction::SetMoveRecoil(const D3DXVECTOR3& move)
{
	// �����̈ړ��ʂ�ݒ肷��
	m_moveRecoil = move;
}

//=========================
// �ړ��ʂ̎擾����
//=========================
D3DXVECTOR3 CPlayerAction::GetMoveRecoil(void) const
{
	// �����̈ړ��ʂ�Ԃ�
	return m_moveRecoil;
}

//=========================
// �s���̐ݒ菈��
//=========================
void CPlayerAction::SetAction(const ACTION action)
{
	// �s����ݒ肷��
	m_action = action;

	// �s���J�E���g�����Z�b�g����
	m_nActionCount = 0;
}

//=========================
// �s���̎擾����
//=========================
CPlayerAction::ACTION CPlayerAction::GetAction(void) const
{
	// �s����Ԃ�
	return m_action;
}

//=========================
// �����������̐ݒ菈��
//=========================
void CPlayerAction::SetDodgeRot(const float fRot)
{
	// ������������ݒ肷��
	m_fDodgeRot = fRot;
}

//=========================
// ����C���^�[�o���̐ݒ菈��
//=========================
void CPlayerAction::SetDodgeInterval(const int nInterval)
{
	// ����g�p�\�󋵂�ݒ肷��
	m_nDodgeInterval = nInterval;
}

//=========================
// ����g�p�\��Ԃ̐ݒ菈��
//=========================
void CPlayerAction::SetEnableDodgeUse(const bool bUse)
{
	// ����g�p�\�󋵂�Ԃ�
	m_bDodgeUse = bUse;
}

//=========================
// ����g�p�\�󋵂̎擾����
//=========================
bool CPlayerAction::IsDodgeUse(void) const
{
	// ����g�p�\�󋵂�Ԃ�
	return m_bDodgeUse;
}

//=========================
// �����󋵂̐ݒ菈��
//=========================
void CPlayerAction::SetEnableRecoil(const bool bRecoil)
{
	// �����󋵂�ݒ肷��
	m_bRecoil = bRecoil;
}

//=========================
// �����󋵂̎擾����
//=========================
bool CPlayerAction::IsRecoil(void) const
{
	// �����󋵂��擾����
	return m_bRecoil;
}

//=========================
// �ʏ��ԏ���
//=========================
void CPlayerAction::NoneProcess(void)
{
	// �ً}���Ɉړ��ł���悤�ɂ��Ă���
	m_bRecoil = false;
}

//=========================
// �ˌ���ԏ���
//=========================
void CPlayerAction::ShotProcess(CPlayer* pPlayer)
{
	// �������擾����
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();
	D3DXVECTOR3 rotCamera = CManager::Get()->GetCamera()->GetRot();

	// �J�����̌����𓯂������𑵂���
	rotDest.y = rotCamera.y;

	// ������K�p����
	pPlayer->SetRotDest(rotDest);

	// �r��^�������L�΂�
	pPlayer->GetHierarchy(5)->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	pPlayer->GetHierarchy(6)->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pPlayer->GetHierarchy(7)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(8)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(9)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f,  0.0f, 0.0f));
	pPlayer->GetHierarchy(10)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
}

//=========================
// �_�K�[��ԏ���
//=========================
void CPlayerAction::DaggerPrecess(CPlayer* pPlayer)
{
	// �ړ��ʂ��擾����
	D3DXVECTOR3 move = pPlayer->GetMove();

	// �ړ��ʂ�0.0f�ɂ���
	move.x = 0.0f;
	move.z = 0.0f;

	// �ړ��ʂ�K�p����
	pPlayer->SetMove(move);

	// �s���J�E���g�����Z����
	m_nActionCount++;

	if (m_nActionCount >= DAGGER_ATTACK_START &&
		m_nActionCount <= DAGGER_ATTACK_END)
	{ // �s���J�E���g�����ȏ�̏ꍇ

		// �O�Ղ̕`��󋵂� true �ɂ���
		pPlayer->GetDagger()->SetEnableDispOrbit(true);

		// �؂ւ̍U�����菈��
		collision::TreeAttack(*pPlayer, DAGGER_HEIGHT);

		// ���V�̎��Ƃ̓����蔻��
		collision::PalmFruitHit(pPlayer, DAGGER_HEIGHT);

		// �G�ƃ_�K�[�̓����蔻��
		collision::EnemyHitToDagger(pPlayer->GetPos(), ATTACK_DAGGER_HEIGHT, ATTACK_DAGGER_RADIUS);

		// ���e�ƃ_�K�[�̓����蔻��
		collision::BombHitToDagger(pPlayer->GetPos(), ATTACK_DAGGER_HEIGHT);
	}

	if (m_nActionCount % DAGGER_COUNT == 0)
	{ // �s���J�E���g����萔�ɒB�����ꍇ

		// �s����ݒ肷��
		SetAction(ACTION_NONE);

		// �_�K�[��\�����Ȃ�
		pPlayer->GetDagger()->SetEnableDisp(false);
		pPlayer->GetDagger()->SetEnableDispOrbit(false);

		// ���e��`�悷��
		pPlayer->GetHandGun(0)->SetEnableDisp(true);
		pPlayer->GetHandGun(1)->SetEnableDisp(true);
	}
}

//=========================
// �����ԏ���
//=========================
void CPlayerAction::DodgeProcess(CPlayer* pPlayer)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = pPlayer->GetPos();

	pos.x += sinf(m_fDodgeRot) * DODGE_SPEED;
	pos.z += cosf(m_fDodgeRot) * DODGE_SPEED;

	// �ʒu��K�p����
	pPlayer->SetPos(pos);

	CCharaBlur* pBlur = CCharaBlur::Create
	(
		pPlayer->GetPos(),
		pPlayer->GetRot(),
		pPlayer->GetScale(),
		pPlayer->GetNumModel(),
		DODGE_BLUR_COL,
		DODGE_BLUR_LIFE,
		CObject::PRIORITY_PAUSE
	);

	if (pBlur != nullptr)
	{ // �u���[�� NULL ����Ȃ��ꍇ

		for (int nCnt = 0; nCnt < pPlayer->GetNumModel(); nCnt++)
		{
			// ���f���̏��ݒ菈��
			pBlur->SetModelData
			(
				nCnt,
				pPlayer->GetHierarchy(nCnt)->GetPos(),
				pPlayer->GetHierarchy(nCnt)->GetRot(),
				pPlayer->GetHierarchy(nCnt)->GetScale(),
				pPlayer->GetHierarchy(nCnt)->GetFileData(),
				pPlayer->GetHierarchy(nCnt)->GetParentIdx()
			);
		}
	}

	// �s���J�E���g�����Z����
	m_nActionCount++;

	if (m_nActionCount >= DODGE_COUNT)
	{ // �s���J�E���g����萔�ȏ�ɂȂ����ꍇ

		// �s����ݒ肷��
		SetAction(ACTION_NONE);
	}
}

//=========================
// �U�e(J+�e)���
//=========================
void CPlayerAction::ShotgunProcess(CPlayer* pPlayer)
{
	// �ʒu�ƌ������擾����
	D3DXVECTOR3 pos = pPlayer->GetPos();
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();
	D3DXVECTOR3 rotCamera = CManager::Get()->GetCamera()->GetRot();

	if (pPlayer->IsJump() == true)
	{ // �󒆂ɂ���ꍇ

		// �ʒu���ړ�����
		pos += m_moveRecoil;
	}
	else
	{ // ��L�ȊO

		// �s����ݒ肷��
		SetAction(ACTION_NONE);

		// �����󋵂� false �ɂ���
		m_bRecoil = false;
	}

	// �J�����̌����𓯂������𑵂���
	rotDest.y = rotCamera.y;

	// �ʒu�ƌ�����K�p����
	pPlayer->SetPos(pos);
	pPlayer->SetRotDest(rotDest);

	// �r��^�������L�΂�
	pPlayer->GetHierarchy(5)->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	pPlayer->GetHierarchy(6)->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pPlayer->GetHierarchy(7)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(8)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(9)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(10)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
}

//=========================
// �}�~����ԏ���
//=========================
void CPlayerAction::SwoopProcess(CPlayer* pPlayer)
{
	// �ړ��ʂ��擾����
	D3DXVECTOR3 move = pPlayer->GetMove();

	if (pPlayer->IsJump() == false)
	{ // �n��ɗ������u��

		// �a���̔g��𐶐�����
		CSlashRipple::Create(pPlayer->GetPos(), pPlayer->GetRot());
	}

	// �ړ��ʂ�ݒ肷��
	move.x = 0.0f;
	move.y = -20.0f + (m_nActionCount);
	move.z = 0.0f;

	// �ړ��ʂ�K�p����
	pPlayer->SetMove(move);

	// �s���J�E���g�����Z����
	m_nActionCount++;

	if (m_nActionCount >= DAGGER_ATTACK_START &&
		m_nActionCount <= DAGGER_ATTACK_END)
	{ // �s���J�E���g�����ȏ�̏ꍇ

		// �O�Ղ̕`��󋵂� true �ɂ���
		pPlayer->GetDagger()->SetEnableDispOrbit(true);

		// �؂ւ̍U�����菈��
		collision::TreeAttack(*pPlayer, DAGGER_HEIGHT);

		// ���V�̎��Ƃ̓����蔻��
		collision::PalmFruitHit(pPlayer, DAGGER_HEIGHT);

		// �G�ƃ_�K�[�̓����蔻��
		collision::EnemyHitToDagger(pPlayer->GetPos(), ATTACK_DAGGER_HEIGHT, ATTACK_DAGGER_RADIUS);
	}

	if (m_nActionCount % SWOOP_COUNT == 0)
	{ // �s���J�E���g����萔�ɒB�����ꍇ

		// �s����ݒ肷��
		SetAction(ACTION_NONE);

		// �_�K�[��\�����Ȃ�
		pPlayer->GetDagger()->SetEnableDisp(false);
		pPlayer->GetDagger()->SetEnableDispOrbit(false);

		// ���e��`�悷��
		pPlayer->GetHandGun(0)->SetEnableDisp(true);
		pPlayer->GetHandGun(1)->SetEnableDisp(true);
	}
}

//=========================
// �����[�h����
//=========================
void CPlayerAction::ReloadProcess(CPlayer* pPlayer)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = pPlayer->GetPos();

	if (pPlayer->IsJump() == true)
	{ // �󒆂ɂ���ꍇ

		// �ʒu���ړ�����
		pos += m_moveRecoil;
	}
	else
	{ // ��L�ȊO

		// �����󋵂� false �ɂ���
		m_bRecoil = false;
	}

	// �ʒu��K�p����
	pPlayer->SetPos(pos);

	// �r��������ɂ�����
	pPlayer->GetHierarchy(5)->SetRot(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.25f));
	pPlayer->GetHierarchy(6)->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.25f));
	pPlayer->GetHierarchy(7)->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, -D3DX_PI * 0.25f));
	pPlayer->GetHierarchy(8)->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, D3DX_PI * 0.25f));
	pPlayer->GetHierarchy(9)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	pPlayer->GetHierarchy(10)->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	if (pPlayer->GetHandGun(0)->GetState() == CHandgun::STATE_NONE &&
		pPlayer->GetHandGun(1)->GetState() == CHandgun::STATE_NONE)
	{ // �ǂ����̃n���h�K�����ʏ�ɖ߂����ꍇ

		// �����[�h�����ɂ���
		pPlayer->GetBulletUI()->SetNumBullet(MAX_REMAINING_BULLET);

		// �ʏ��Ԃɂ���
		SetAction(ACTION_NONE);
	}
}