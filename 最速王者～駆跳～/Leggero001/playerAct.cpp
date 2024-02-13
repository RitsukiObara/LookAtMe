//============================================
//
// �v���C���[�s���Ǘ��̃��C������[playerAct.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "scene.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "playerAct.h"
#include "player.h"
#include "motion.h"
#include "landUI.h"

#include "actionUI.h"
#include "aura.h"
#include "debugproc.h"
#include "Effect.h"
#include "fraction.h"
#include "orbit.h"
#include "ripple.h"
#include "rubsmoke.h"
#include "score.h"
#include "speedUI.h"
#include "sound.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define PLAYER_WALLKICK_SPEED_R			(D3DXVECTOR3(8.0f, 17.0f, 0.0f))		// �ǃL�b�N���̉E�ɔ�ԑ��x
#define PLAYER_WALLKICK_SPEED_L			(D3DXVECTOR3(-8.0f, 17.0f, 0.0f))		// �ǃL�b�N���̍��ɔ�ԑ��x
#define WALLKICK_FRACTION_COL			(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f))		// �ǃL�b�N���̔j�Ђ̐F
#define PLAYER_WALLFRIC_GRAVITY			(-0.5f)		// �v���C���[�̕ǎC���Ԃ̏d��
#define PLAYER_BACKPACK_NUM				(1)			// �o�b�N�p�b�N�̂��郂�f���̔ԍ�
#define PLAYER_JET_SHIFT_DIST			(D3DXVECTOR3(-7.0f,29.0f,0.0f))		// �W�F�b�g���˂̂Ƃ��̂̒��S���炸�炷����
#define PLAYER_JET_ADD_MOVE				(-9.5f)		// �W�F�b�g���˂̒ǉ��ړ���
#define PLAYER_BIGJUMP_ADD_POS_Y		(30.0f)		// ���W�����v���̃G�t�F�N�g�̈ʒu�̉��Z��
#define PLAYER_CLIFF_SPEED_SUCCESS		(0.3f)		// �R����Ő��������Ƃ��̑��x
#define PLAYER_CLIFF_JUMP_SUCCESS		(10.0f)		// �R����Ő��������Ƃ��̃W�����v��
#define PLAYER_CLIFF_GRAVITY_SUCCESS	(0.5f)		// �R����Ő��������Ƃ��̏d��
#define PLAYER_CLIFF_SPEED_FAILED		(0.7f)		// �R����Ŏ��s�����Ƃ��̑��x
#define PLAYER_CLIFF_JUMP_FAILED		(4.2f)		// �R����Ŏ��s�����Ƃ��̃W�����v��
#define PLAYER_CLIFF_GRAVITY_FAILED		(0.095f)	// �R����Ŏ��s�����Ƃ��̏d��
#define PLAYER_HANGING_DESTROT_SUB		(-0.7f)		// �Ԃ牺�����Ԃ̖ړI�̌����̈ړ��ʂ̌����W��
#define PLAYER_HANGING_ROT_SUB			(0.05f)		// �Ԃ牺�����Ԃ̌����̈ړ��ʂ̌����W��
#define PLAYER_POLEDOWN_POSY_MOVE		(3.5f)		// �|�[�������Ԃ�Y���̈ړ���
#define PLAYER_POLEDOWN_ROT_MOVE		(0.25f)		// �|�[�������Ԃ̌����̈ړ���
#define PLAYER_RUB_SOUND_COUNT			(60)		// �C�鉹��炷�J�E���g��
#define PLAYER_FAST_LAMPJUMP_MOVE_X		(10.0f)		// �ő��X���W�����v�̈ړ���(X��)
#define PLAYER_LAMPJUMP_MOVE_X_MAGNI	(11.0f)		// �X���W�����v�̈ړ��ʂ̔{��(X��)
#define PLAYER_LAMPJUMP_MIN_MOVE_Y		(5.0f)		// �X���W�����v�̈ړ��ʂ̍ŏ���(Y��)
#define PLAYER_LAMPJUMP_SCORE_MOVE		(12.0f)		// �X���W�����v�œ��_������ړ���

//=========================================
// �R���X�g���N�^
//=========================================
CPlayerAct::CPlayerAct()
{
	// �S�Ă̒l���N���A����
	m_posObject = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �֗^���Ă���I�u�W�F�N�g�̈ʒu
	m_Action = ACT_NONE;		// �s��
	m_nActCount = 0;			// �s���J�E���g
	m_fCliffUpSpeed = 0.0f;		// �R�o��̑��x
	m_fCliffGravity = 0.0f;		// �R�オ�莞�̏d��
	m_fRotDest = 0.0f;			// �ڕW�̌���
	m_bWallOld = false;			// �O��̕ǎC���
	m_bRightKick = false;		// �E�ǂ��R�������ǂ���
	m_bRightLeave = false;		// �Ǘ��ꎞ�̌���
	m_bCliffSuccess = false;	// �R�オ��ɐ����������ǂ���
}

//=========================================
// �f�X�g���N�^
//=========================================
CPlayerAct::~CPlayerAct()
{

}

//===========================================
// �v���C���[�̏���������
//===========================================
HRESULT CPlayerAct::Init(void)
{
	// �S�Ă̒l������������
	m_posObject = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �֗^���Ă���I�u�W�F�N�g�̈ʒu
	m_Action = ACT_NONE;		// �s��
	m_nActCount = 0;			// �s���J�E���g
	m_fCliffUpSpeed = 0.0f;		// �R�o��̑��x
	m_fCliffGravity = 0.0f;		// �R�オ�莞�̏d��
	m_fRotDest = 0.0f;			// �ڕW�̌���
	m_bWallOld = false;			// �O��̕ǎC���
	m_bRightKick = false;		// �E�ǂ��R�������ǂ���
	m_bRightLeave = false;		// �Ǘ��ꎞ�̌���
	m_bCliffSuccess = false;	// �R�オ��ɐ����������ǂ���

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �v���C���[�̏I������
//===========================================
void CPlayerAct::Uninit(void)
{

}

//===========================================
// ��������
//===========================================
CPlayerAct* CPlayerAct::Create(void)
{
	// ���[�J���|�C���^�錾
	CPlayerAct* pPlayerAct = nullptr;

	if (pPlayerAct == nullptr)
	{ // �v���C���[�̃|�C���^�� NULL �̏ꍇ

		// ���������m�ۂ���
		pPlayerAct = new CPlayerAct;
	}
	else
	{ // �v���C���[�̃|�C���^�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pPlayerAct != nullptr)
	{ // �������m�ۂɐ������Ă����ꍇ

		if (FAILED(pPlayerAct->Init()))
		{ // �������Ɏ��s�����ꍇ

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �������m�ۂɎ��s�����ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �|�C���^ ��Ԃ�
	return pPlayerAct;
}

//===========================================
// ��Ԃ̐ݒ菈��
//===========================================
void CPlayerAct::SetAct(const ACT act)
{
	// ��Ԃ�ݒ肷��
	m_Action = act;
}

//===========================================
// ��Ԃ̎擾����
//===========================================
CPlayerAct::ACT CPlayerAct::GetAct(void) const
{
	// ��Ԃ�Ԃ�
	return m_Action;
}

//===========================================
// �O��̕ǎC��󋵂̐ݒ菈��
//===========================================
void CPlayerAct::SetWallOld(const bool bWall)
{
	// �O��̕ǎC��󋵂�ݒ肷��
	m_bWallOld = bWall;
}

//===========================================
// �O��̕ǎC��󋵂̎擾����
//===========================================
bool CPlayerAct::GetWallOld(void) const
{
	// �O��̕ǎC��󋵂�Ԃ�
	return m_bWallOld;
}

//===========================================
// �Ǘ��ꎞ�̌����ݒ菈��
//===========================================
void CPlayerAct::SetRightLeave(const bool bRightLeave)
{
	// �Ǘ��ꎞ�̌�����ݒ肷��
	m_bRightLeave = bRightLeave;
}

//===========================================
// �Ǘ��ꎞ�̌����擾����
//===========================================
bool CPlayerAct::GetRightLeave(void) const
{
	// �Ǘ��ꎞ�̌�����Ԃ�
	return m_bRightLeave;
}

//===========================================
// �E�ǂ��R�������̐ݒ菈��
//===========================================
void CPlayerAct::SetRightKick(const bool bRight)
{
	// �E�ǂ��R�������ǂ�����ݒ肷��
	m_bRightKick = bRight;
}

//===========================================
// �֗^���Ă���I�u�W�F�N�g�̈ʒu�̐ݒ菈��
//===========================================
void CPlayerAct::SetPosObject(const D3DXVECTOR3& pos)
{
	// �֗^���Ă���I�u�W�F�N�g�ʒu��ݒ肷��
	m_posObject = pos;
}

//===========================================
// �֗^���Ă���I�u�W�F�N�g�̈ʒu�̎擾����
//===========================================
D3DXVECTOR3 CPlayerAct::GetPosObject(void) const
{
	// �֗^���Ă���I�u�W�F�N�g�ʒu��Ԃ�
	return m_posObject;
}

//===========================================
// �R�߂܂莞�̏d�͂̎擾����
//===========================================
float CPlayerAct::GetCliffGravity(void) const
{
	// �R���ꎞ�̏d�͂�Ԃ�
	return m_fCliffGravity;
}

//===========================================
// �ڕW�̌����̐ݒ菈��
//===========================================
void CPlayerAct::SetRotDest(const float fRot)
{
	// �����̈ړ��ʂ�ݒ肷��
	m_fRotDest = fRot;
}

//=======================================
// �s����ԏ���
//=======================================
void CPlayerAct::Action(CPlayer& player)
{
	switch (m_Action)
	{
	case ACT_NONE:					// ����

		// �ʏ�s������
		ActNone(player);

		break;

	case ACT_BIGJUMP:				// ���W�����v

		// ���W�����v�s������
		ActSuperJump(player);

		break;

	case ACT_WALLFRIC:				// �ǎC����

		// �ǎC��s������
		ActWallFriction(player);

		break;

	case ACT_WALLLEAVE:				// �Ǘ�����

		// �Ǘ���s������
		ActWallLeave(player);

		break;

	case ACT_WALLSTEPIN:			// �Ǔ��ݍ��ݏ���

		// �Ǔ��ݍ��ݏ���
		ActWallStepIn(player);

		break;

	case ACT_WALLKICK:				// �ǃL�b�N

		// �ǃL�b�N�s������
		ActWallKick(player);

		break;

	case ACT_WALLAFTERGLOW:			// �ǃL�b�N�̗]�C

		// �ǃL�b�N�̗]�C�s������
		ActWallAfterGlaw(player);

		break;

	case ACT_CLIFFCATCH:			// �R���܂���

		// �R�߂܂�s������
		ActCliffCatch(player);

		break;

	case ACT_CLIFFCLIMB:			// �R�~��

		// �R�o��s������
		ActCliffClimb(player);

		break;

	case ACT_CLIFFFALL:				// �R�~����

		// �R�~��s������
		ActCliffFall(player);

		break;

	case ACT_CLIFFLAND:				// �R�������n

		// �R�o�蒅�n�s������
		ActCliffLand(player);

		break;

	case ACT_CLIFFAFTERGLOW:		// �]�C���

		// �]�C�s������
		ActCliffAfterGlaw(player);

		break;

	case ACT_HANGING:				// �Ԃ牺������

		// �Ԃ牺����s�����
		ActHanging(player);

		break;

	case ACT_LAMPJUMP:				// �����v������ł�����

		// �����v������ł�����
		ActLampJump(player);

		break;

	case ACT_POLEDOWN:				// �|�[��������

		// �|�[�������ԏ���
		ActPoleDown(player);

		break;

	default:						// ��L�ȊO

		// �s���ω�����
		ActionChange(ACT_NONE);

		break;
	}
}

//=======================================
// �A�N�V�����ω�����
//=======================================
void CPlayerAct::ActionChange(const ACT action)
{
	// �s����Ԃ̐ݒ�
	m_Action = action;

	// �s���J�E���g������������
	m_nActCount = 0;
}

//=======================================
// �R�o�萬������̎擾����
//=======================================
bool CPlayerAct::GetCliffCatch(void) const
{
	// �R�o�萬�������Ԃ�
	return m_bCliffSuccess;
}

//=======================================
// �A�N�V�����J�E���g�̎擾����
//=======================================
int CPlayerAct::GetActCount(void) const
{
	// �U���J�E���g��Ԃ�
	return m_nActCount;
}

//=======================================
// �ʏ�s������
//=======================================
void CPlayerAct::ActNone(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = player.GetRot();		// �������擾

	// �s���J�E���g��0�ɂ���
	m_nActCount = 0;

	if (player.GetRight() == true)
	{ // �E�������Ă����ꍇ

		// �������E�����ɂ���
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // ���������Ă����ꍇ

		// �������������ɂ���
		rot.y = D3DX_PI * -0.5f;
	}

	// ������ݒ肷��
	player.SetRot(rot);
}

//=======================================
// ���W�����v�s������
//=======================================
void CPlayerAct::ActSuperJump(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = player.GetRot();		// �������擾����

	if (player.GetRight() == true)
	{ // �E�������Ă����ꍇ

		// �������E�����ɂ���
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // ���������Ă����ꍇ

		// �������������ɂ���
		rot.y = D3DX_PI * -0.5f;
	}

	// �s���J�E���g�����Z����
	m_nActCount++;

	// ���W�����v���̃G�t�F�N�g����
	SuperJumpEffect(player);

	if (player.GetMove().y >= 0.0f)
	{ // �c�̈ړ��ʂ� 0.0f �ȏ�̏ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 BodyPos = player.GetPos() + player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetPos();		// �̂̈ʒu���擾����
		D3DXVECTOR3 EffectMove = player.GetMove();														// �G�t�F�N�g�̈ړ��ʂ��擾����

		// �̂̈ʒu��ݒ肷��
		BodyPos.x += sinf(player.GetRot().y) * PLAYER_JET_SHIFT_DIST.x;
		BodyPos.y += PLAYER_JET_SHIFT_DIST.y;

		// �G�t�F�N�g�̈ړ��ʂ�ݒ肷��
		EffectMove.y += PLAYER_JET_ADD_MOVE;

		// �G�t�F�N�g�̐ݒ菈��
		CEffect::Create(BodyPos, EffectMove, 6, 10.0f, CEffect::TYPE_FIRE);
	}

	// �����̐ݒ菈��
	player.SetRot(rot);
}

//=======================================
// �ǎC��s������
//=======================================
void CPlayerAct::ActWallFriction(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player.GetPos();		// �ʒu���擾����
	D3DXVECTOR3 move = player.GetMove();	// �ړ��ʂ��擾����
	D3DXVECTOR3 rot = player.GetRot();		// �������擾����

	if (player.GetRight() == true)
	{ // �E�������Ă����ꍇ

		// �������E�����ɂ���
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // ���������Ă����ꍇ

		// �������������ɂ���
		rot.y = D3DX_PI * -0.5f;
	}

	if (move.y <= 1.0f)
	{ // �ړ��ʂ� 0.0f �ȉ��̏ꍇ

		// �d�͂𐧌�����
		move.y = PLAYER_WALLFRIC_GRAVITY;
	}

	if (m_nActCount % PLAYER_WALLFRIC_DUST_COUNT == 0)
	{ // �s���J�E���g����萔�ɒB�����ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 RubPos;			// ���̈ʒu

		// �ʒu��ݒ肷��
		RubPos.x = sinf(rot.y) * PLAYER_WIDTH;
		RubPos.y = (float)(rand() % (int)(PLAYER_HEIGHT));
		RubPos.z = 0.0f;

		// ����(��)�̐�������
		CRubSmoke::Create(pos + RubPos);
	}

	if (m_nActCount % PLAYER_RUB_SOUND_COUNT == 0)
	{ // �s���J�E���g����萔�ɒB�����ꍇ

		// �̂��C�鉹��炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RUBBING);
	}

	// �s���J�E���g�����Z����
	m_nActCount++;

	if(player.GetStamina() >= PLAYER_WALLKICK_STAMINA)
	{ // �v���C���[�ɕǃL�b�N�ł���X�^�~�i���������ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 EffectPos;		// �G�t�F�N�g�̈ʒu

		// �G�t�F�N�g�̈ʒu��ݒ肷��
		EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
		EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
		EffectPos.z = 0.0f;

		// �I�[���𐶐�����
		CAura::Create(pos + EffectPos, AURA_NORMAL_LIFE);
	}

	// �����X�V����
	player.SetRot(rot);		// ����
	player.SetMove(move);	// �ړ���
}

//=======================================
// �Ǘ����s������
//=======================================
void CPlayerAct::ActWallLeave(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 move = player.GetMove();	// �ړ��ʂ��擾����
	D3DXVECTOR3 rot = player.GetRot();		// �������擾����

	// �s���J�E���g�����Z����
	m_nActCount++;

	if (m_nActCount >= PLAYER_WALLLEAVE_COUNT)
	{ // �s���J�E���g����萔�ȏ�ɂȂ����ꍇ

		// �ʏ�s���ɕς���
		ActionChange(ACT_NONE);
	}

	// �d�͂𐧌�����
	move.y = PLAYER_WALLFRIC_GRAVITY;

	// ���[�J���ϐ��錾
	D3DXVECTOR3 EffectPos;		// �G�t�F�N�g�̈ʒu

	// �G�t�F�N�g�̈ʒu��ݒ肷��
	EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
	EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
	EffectPos.z = 0.0f;

	// �I�[���𐶐�����
	CAura::Create(player.GetPos() + EffectPos, AURA_NORMAL_LIFE);

	// �����̐ݒ菈��
	player.SetRot(rot);		// ����
	player.SetMove(move);	// �ړ���
}

//=======================================
// �Ǔ��ݍ��ݏ���
//=======================================
void CPlayerAct::ActWallStepIn(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 move = player.GetMove();	// �ړ��ʂ��擾����

	// �d�͂𐧌�����
	move.y = 0.0f;

	if (player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_WALLSTEPIN)
	{ // ���[�V�������Ǔ��ݍ��݂���Ȃ��ꍇ

		if (m_bRightKick == true)
		{ // �E�ǂ��R�����ꍇ

			// �ǃL�b�N(�E��)
			WallKickRight(player);
		}
		else
		{ // ���ǂ��R�����ꍇ

			// �ǃL�b�N(����)
			WallKickLeft(player);
		}

		// ���̐�̏������s��Ȃ�
		return;
	}

	// �����X�V����
	player.SetMove(move);	// �ړ���
}

//=======================================
// �ǃL�b�N�s������
//=======================================
void CPlayerAct::ActWallKick(CPlayer& player)
{
	// �s���J�E���g�����Z����
	m_nActCount++;

	if (m_nActCount >= PLAYER_WALLKICK_COUNT)
	{ // ��萔�ɒB�����ꍇ

		// �ʏ��Ԃɂ���
		m_Action = ACT_NONE;

		// �s���J�E���g�� 0 �ɂ���
		m_nActCount = 0;
	}

	if (player.GetMove().y >= 0.0f)
	{ // �c�̈ړ��ʂ� 0.0f �ȏ�̏ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 BodyPos = player.GetPos() + player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetPos();		// �̂̈ʒu���擾����
		D3DXVECTOR3 EffectMove = player.GetMove();														// �G�t�F�N�g�̈ړ��ʂ��擾����

		// �̂̈ʒu��ݒ肷��
		BodyPos.x += sinf(player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetRot().y) * PLAYER_JET_SHIFT_DIST.x;
		BodyPos.y += PLAYER_JET_SHIFT_DIST.y;

		// �G�t�F�N�g�̈ړ��ʂ�ݒ肷��
		EffectMove.y += PLAYER_JET_ADD_MOVE;

		// �G�t�F�N�g�̐ݒ菈��
		CEffect::Create(BodyPos, EffectMove, 6, 10.0f, CEffect::TYPE_FIRE);
	}
}

//=======================================
// �ǃL�b�N�̗]�C�s������
//=======================================
void CPlayerAct::ActWallAfterGlaw(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player.GetPos();		// �ʒu���擾����
	D3DXVECTOR3 move = player.GetMove();	// �ړ��ʂ��擾����
	D3DXVECTOR3 rot = player.GetRot();		// �������擾����

	if (m_nActCount % PLAYER_RUB_SOUND_COUNT == 0)
	{ // �s���J�E���g����萔�ɒB�����ꍇ

		// �̂��C�鉹��炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RUBBING);
	}

	// �s���J�E���g�����Z����
	m_nActCount++;

	if (m_nActCount >= PLAYER_WALLAFTERGLOW_COUNT)
	{ // ��萔�ɒB�����ꍇ

		// �ʏ��Ԃɂ���
		m_Action = ACT_NONE;

		// �s���J�E���g�� 0 �ɂ���
		m_nActCount = 0;
	}

	if (player.GetMove().y >= 0.0f)
	{ // �c�̈ړ��ʂ� 0.0f �ȏ�̏ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 BodyPos = pos + player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetPos();		// �̂̈ʒu���擾����
		D3DXVECTOR3 EffectMove = move;											// �G�t�F�N�g�̈ړ��ʂ��擾����

		// �̂̈ʒu��ݒ肷��
		BodyPos.x += sinf(player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetRot().y) * PLAYER_JET_SHIFT_DIST.x;
		BodyPos.y += PLAYER_JET_SHIFT_DIST.y;

		// �G�t�F�N�g�̈ړ��ʂ�ݒ肷��
		EffectMove.y += PLAYER_JET_ADD_MOVE;

		// �G�t�F�N�g�̐ݒ菈��
		CEffect::Create(BodyPos, EffectMove, 6, 10.0f, CEffect::TYPE_FIRE);
	}

	if (move.y <= 1.0f)
	{ // �ړ��ʂ� 0.0f �ȉ��̏ꍇ

		// �d�͂𐧌�����
		move.y = PLAYER_WALLFRIC_GRAVITY;
	}

	if (m_nActCount % PLAYER_WALLFRIC_DUST_COUNT == 0)
	{ // �s���J�E���g����萔�ɒB�����ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 RubPos;		// ���̈ʒu

		// �ʒu��ݒ肷��
		RubPos.x = sinf(rot.y) * PLAYER_WIDTH;
		RubPos.y = (float)(rand() % (int)(PLAYER_HEIGHT));
		RubPos.z = 0.0f;

		// ����(��)�̐�������
		CRubSmoke::Create(pos + RubPos);
	}

	// ���[�J���ϐ��錾
	D3DXVECTOR3 EffectPos;		// �G�t�F�N�g�̈ʒu

	// �G�t�F�N�g�̈ʒu��ݒ肷��
	EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
	EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
	EffectPos.z = 0.0f;

	// �I�[���𐶐�����
	CAura::Create(pos + EffectPos, AURA_NORMAL_LIFE);

	// �����X�V����
	player.SetMove(move);	// �ړ���
}

//=======================================
// �R�߂܂�s������
//=======================================
void CPlayerAct::ActCliffCatch(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 move = player.GetMove();		// �ړ��ʂ��擾����

	// �ړ��ʂ�0.0f�ɂ���
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �J�E���g�����Z����
	m_nActCount++;

	if (m_nActCount <= PLAYER_CLIFFCLIMB_SUCCESS_COUNT)
	{ // �J�E���g�������ȉ��̏ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 EffectPos;		// �G�t�F�N�g�̈ʒu

		// �G�t�F�N�g�̈ʒu��ݒ肷��
		EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
		EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
		EffectPos.z = 0.0f;

		// �I�[���𐶐�����
		CAura::Create(player.GetPos() + EffectPos, AURA_NORMAL_LIFE);
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true ||
		CManager::GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_A, 0) == true)
	{ // SPACE�L�[���������ꍇ

		if (m_nActCount <= PLAYER_CLIFFCLIMB_SUCCESS_COUNT)
		{ // �J�E���g�������ȉ��̏ꍇ

			// �s����ԕω�����
			ActionChange(ACT_CLIFFCLIMB);

			// �W�����v������
			move.y = PLAYER_CLIFF_JUMP_SUCCESS;

			// �W�����v����
			player.SetJump(true);

			// �R�オ��̑��x��ݒ肷��
			m_fCliffUpSpeed = PLAYER_CLIFF_SPEED_SUCCESS;

			// �R�オ�莞�̏d�͂�ݒ肷��
			m_fCliffGravity = PLAYER_CLIFF_GRAVITY_SUCCESS;

			// �R�オ�萬��
			m_bCliffSuccess = true;

			// �O�Ղ̑S���_�̈ʒu�̐ݒ菈��
			player.GetOrbit()->PosSet(player.GetPos());

			// �R�オ�胂�[�V������ݒ肷��
			player.GetMotion()->Set(CPlayer::MOTIONTYPE_CLIFFUP);
		}
		else
		{ // ��L�ȊO

			// �s����ԕω�����
			ActionChange(ACT_CLIFFCLIMB);

			// �W�����v������
			move.y = PLAYER_CLIFF_JUMP_FAILED;

			// �W�����v����
			player.SetJump(true);

			// �R�オ��̑��x��ݒ肷��
			m_fCliffUpSpeed = PLAYER_CLIFF_SPEED_FAILED;

			// �R�オ�莞�̏d�͂�ݒ肷��
			m_fCliffGravity = PLAYER_CLIFF_GRAVITY_FAILED;

			// �R�オ�莸�s
			m_bCliffSuccess = false;

			// �悶�o�胂�[�V������ݒ肷��
			player.GetMotion()->Set(CPlayer::MOTIONTYPE_ASCENT);
		}

		// �I�[����S�ď���
		CObject::AnyAllClear(CObject::PRIORITY_ENTITY, CObject::TYPE_AURA);
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true ||
		CManager::GetInputGamePad()->GetGameStickLYPress(0) <= SHRT_MIN ||
		m_nActCount >= PLAYER_CLIFFCLIMB_FALL_COUNT)
	{ // S�L�[�������܂��́A�J�E���g����萔�ȏ�ɂȂ����ꍇ

		// �s����ԕω�����
		ActionChange(ACT_CLIFFFALL);
	}

	// �ړ��ʂ��X�V����
	player.SetMove(move);
}

//=======================================
// �R�o��s������
//=======================================
void CPlayerAct::ActCliffClimb(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player.GetPos();		// �ʒu���擾����
	D3DXVECTOR3 rot = player.GetRot();		// �������擾����

	// �ړ��ʂ����Z����
	pos.x += sinf(rot.y) * m_fCliffUpSpeed;

	// �ʒu�E�����̐ݒ菈��
	player.SetPos(pos);
	player.SetRot(rot);

	// �s���J�E���g�����Z����
	m_nActCount++;

	if (m_bCliffSuccess == true)
	{ // �R�o��ɐ������Ă����ꍇ

		if (m_nActCount >= PLAYER_CLIFFCLIMB_AURA_COUNT)
		{ // �s���J�E���g����萔�𒴂����ꍇ

			// ���[�J���ϐ��錾
			D3DXVECTOR3 EffectPos;		// �G�t�F�N�g�̈ʒu

			// �G�t�F�N�g�̈ʒu��ݒ肷��
			EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
			EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
			EffectPos.z = 0.0f;

			// �I�[���𐶐�����
			CAura::Create(pos + EffectPos, AURA_CLIFF_LIFE);
		}
	}
}

//=======================================
// �R�~��s������
//=======================================
void CPlayerAct::ActCliffFall(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = player.GetRot();		// �������擾

	// �s���J�E���g�����Z����
	m_nActCount++;

	if (player.GetRight() == true)
	{ // �E�������Ă����ꍇ

		// �������E�����ɂ���
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // ���������Ă����ꍇ

		// �������������ɂ���
		rot.y = D3DX_PI * -0.5f;
	}

	// ������ݒ肷��
	player.SetRot(rot);

	if (m_nActCount >= PLAYER_CLIFFFALL_COUNT)
	{ // �s���J�E���g�����ȏ�ɂȂ����ꍇ

		// �s����ԕω�����
		ActionChange(ACT_NONE);
	}
}

//=======================================
// �R�o�蒅�n�s������
//=======================================
void CPlayerAct::ActCliffLand(CPlayer& player)
{
	// ���[�J���ϐ��錾
	float fSpeed = player.GetSpeed();

	if (m_bCliffSuccess == true)
	{ // �R�o��ɐ������Ă����ꍇ

		if (m_nActCount <= PLAYER_CLIFFLAND_AURA_COUNT)
		{ // �s���J�E���g����萔�𒴂����ꍇ

			// ���[�J���ϐ��錾
			D3DXVECTOR3 EffectPos;		// �G�t�F�N�g�̈ʒu

			// �G�t�F�N�g�̈ʒu��ݒ肷��
			EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
			EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
			EffectPos.z = 0.0f;

			// �I�[���𐶐�����
			CAura::Create(player.GetPos() + EffectPos, AURA_CLIFF_LIFE);
		}

		// �J�E���g�����Z����
		m_nActCount++;

		if (m_nActCount >= PLAYER_CLIFFLAND_COUNT)
		{ // �s���J�E���g����萔�ȏ�ɂȂ����ꍇ(���s��)

			// �s����ԕω�����
			ActionChange(ACT_CLIFFAFTERGLOW);

			if (fSpeed <= PLAYER_MIN_SPEED)
			{ // ���x���ŏ��l�ȉ��̏ꍇ

				// ���x��ݒ肷��
				player.SetSpeed(PLAYER_MIN_SPEED);
			}
			else
			{ // ��L�ȊO

				// ���x��������
				fSpeed -= PLAYER_CLIFF_SUB_SPEED;
			}

			// �X�^�~�i�̌v�Z����
			player.CalcStamina(-PLAYER_CLIFFLAND_STAMINA);

			// ���nUI�̐�������
			CLandUI::Create(CLandUI::TYPE_BAD);

			// �I�[����S�ď���
			CObject::AnyAllClear(CObject::PRIORITY_ENTITY, CObject::TYPE_AURA);

			// ���n���s����炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDINGMISS);

			// �u�[�C���O��炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFFAILED);
		}
		else
		{ // ��L�ȊO(������)

			if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true ||
				CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
			{ // �{�^���J�E���g����萔�ɂȂ����ꍇ

				if (fSpeed >= PLAYER_MAX_SPEED)
				{ // ���x���ő�l�ȏ�̏ꍇ

					// ���x��ݒ肷��
					fSpeed = PLAYER_MAX_SPEED;

					// �X�[�p�[�u�[�X�g�̉���炷
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SUPERDASH_START);

					// �u�[�X�g��Ԃɂ���
					player.SetState(CPlayer::STATE_BOOST);
				}
				else
				{ // ��L�ȊO

					// ���x�����Z����
					fSpeed += PLAYER_CLIFF_ADD_SPEED;
				}

				// �s����ԕω�����
				ActionChange(ACT_CLIFFAFTERGLOW);

				// �{�^���̏���ݒ�
				player.SetButtonCount(0);		// �{�^���J�E���g
				player.SetButtonPush(false);	// �{�^���̉�����

				// ���nUI�̐�������
				CLandUI::Create(CLandUI::TYPE_GOOD);

				// �I�[����S�ď���
				CObject::AnyAllClear(CObject::PRIORITY_ENTITY, CObject::TYPE_AURA);

				// �A�N�V����UI(���n�^�[�{)�𐶐�
				CActionUI::Create(CActionUI::TYPE_LANDTURBO, PLAYER_CLIFFLAND_SCORE);

				// �^�[�{����炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_TURBO);

				// ������炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFSUCCESS);
			}
		}
	}
	else
	{ // �R�o��Ɏ��s���Ă����ꍇ

		// �s����ԕω�����
		ActionChange(ACT_CLIFFAFTERGLOW);
	}

	// ���x��ݒ肷��
	player.SetSpeed(fSpeed);
}

//=======================================
// �]�C�s������
//=======================================
void CPlayerAct::ActCliffAfterGlaw(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = player.GetRot();		// �������擾

	if (player.GetRight() == true)
	{ // �E�������Ă����ꍇ

		// �������E�����ɂ���
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // ���������Ă����ꍇ

		// �������������ɂ���
		rot.y = D3DX_PI * -0.5f;
	}

	// ������ݒ肷��
	player.SetRot(rot);

	//�s���J�E���g�����Z����
	m_nActCount++;

	if (m_nActCount >= PLAYER_CLIFFAFTERGLOW_COUNT)
	{ // �s���J�E���g�����ȏ�ɂȂ����ꍇ

		// �s����ԕω�����
		ActionChange(ACT_NONE);
	}
}

//=======================================
// �Ԃ牺����s������
//=======================================
void CPlayerAct::ActHanging(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player.GetPos();			// �ʒu���擾����
	D3DXVECTOR3 move = player.GetMove();		// �ړ��ʂ��擾����
	D3DXVECTOR3 rot = player.GetRot();			// �������擾����

	// �s���J�E���g�����Z����
	m_nActCount++;

	if (m_fRotDest == 0.0f)
	{ // �ڕW�̌����� 0.0f �̏ꍇ

		// �����̈ړ��ʂ�ݒ肷��
		rot.x = 0.0f;
	}

	//if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
	//{ // D�L�[���������ꍇ

	//	// �ړI�̌������X�V����
	//	m_fRotDest *= 1.1f;

	//	// �����̐��K��
	//	RotNormalize(&m_fRotDest);
	//}

	//if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
	//{ // A�L�[���������ꍇ

	//	// �ړI�̌������X�V����
	//	m_fRotDest *= 1.1f;

	//	// �����̐��K��
	//	RotNormalize(&m_fRotDest);
	//}

	if (m_fRotDest <= -D3DX_PI * 0.49f)
	{ // ���������ȉ��ɂȂ����ꍇ

		// ������ݒ肷��
		m_fRotDest = -D3DX_PI * 0.49f;
	}

	if (m_fRotDest >= D3DX_PI * 0.49f)
	{ // ���������ȏ�ɂȂ����ꍇ

		// ������ݒ肷��
		m_fRotDest = D3DX_PI * 0.49f;
	}

	if (player.GetRight() == true)
	{ // �E�����̏ꍇ

		if (m_fRotDest > 0.0f)
		{ // �ڕW�̌����� 0.0f ���߂̏ꍇ

			// �����̃z�[�~���O����
			RotCorrect(m_fRotDest, &rot.x, 0.05f);

			// ���������Z����
			rot.x += 0.01f;

			if (rot.x >= m_fRotDest)
			{ // �ڕW�̌����𒴂����ꍇ

				// �ڕW�̌�����ݒ肷��
				m_fRotDest *= -0.9f;

				// �X�����a�މ���炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCREAK);
			}
		}
		else if (m_fRotDest < 0.0f)
		{ // �ڕW�̌����� 0.0f �����̏ꍇ

			// �����̃z�[�~���O����
			RotCorrect(m_fRotDest, &rot.x, 0.05f);

			// ���������Z����
			rot.x -= 0.01f;

			if (rot.x <= m_fRotDest)
			{ // �ڕW�̌����𒴂����ꍇ

				// �ڕW�̌�����ݒ肷��
				m_fRotDest *= -0.9f;

				// �X�����a�މ���炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCREAK);
			}
		}

		// �ʒu��ݒ肷��
		pos.x = m_posObject.x + sinf(rot.x) * PLAYER_HEIGHT;
		pos.y = m_posObject.y - cosf(rot.x) * PLAYER_HEIGHT;
	}
	else
	{ // �������̏ꍇ

		if (-m_fRotDest > 0.0f)
		{ // �ڕW�̌����� 0.0f ���߂̏ꍇ

			// �����̃z�[�~���O����
			RotCorrect(-m_fRotDest, &rot.x, 0.05f);

			// ���������Z����
			rot.x += 0.01f;

			if (rot.x >= -m_fRotDest)
			{ // �ڕW�̌����𒴂����ꍇ

				// �ڕW�̌�����ݒ肷��
				m_fRotDest *= -0.9f;

				// �X�����a�މ���炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCREAK);
			}
		}
		else if (-m_fRotDest < 0.0f)
		{ // �ڕW�̌����� 0.0f �����̏ꍇ

			// �����̃z�[�~���O����
			RotCorrect(-m_fRotDest, &rot.x, 0.05f);

			// ���������Z����
			rot.x -= 0.01f;

			if (rot.x <= -m_fRotDest)
			{ // �ڕW�̌����𒴂����ꍇ

				// �ڕW�̌�����ݒ肷��
				m_fRotDest *= -0.9f;

				// �X�����a�މ���炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCREAK);
			}
		}

		// �ʒu��ݒ肷��
		pos.x = m_posObject.x + sinf(-rot.x) * PLAYER_HEIGHT;
		pos.y = m_posObject.y - cosf(-rot.x) * PLAYER_HEIGHT;
	}

	// �v���C���[�̃X�C���O����
	PlayerSwing(player);

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || 
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	{ // SPACE�L�[���������ꍇ

		// �O�Ղ̑S���_�̈ʒu�̐ݒ�
		player.GetOrbit()->PosSet(pos);

		if (m_fRotDest >= 0.0f)
		{ // �ڕW�̌����� 0.0f �ȏ�̏ꍇ
			
			if (m_nActCount <= PLAYER_FAST_LAMPJUMP_COUNT)
			{ // �ő��X���W�����v�̏ꍇ

				// �ړ��ʂ�ݒ肷��
				move.x = PLAYER_FAST_LAMPJUMP_MOVE_X;
			}
			else
			{ // ��L�ȊO

				// �ړ��ʂ�ݒ肷��
				move.x = (m_fRotDest - fabsf(fabsf(m_fRotDest) - fabsf(rot.x))) * PLAYER_LAMPJUMP_MOVE_X_MAGNI;
			}

			if (move.x >= PLAYER_LAMPJUMP_SCORE_MOVE)
			{ // �ړ��ʂ����ȏ�̏ꍇ

				// �A�N�V����UI�𐶐�����
				CActionUI::Create(CActionUI::TYPE_LAMPJUMP, PLAYER_LAMPJUMP_SCORE);
			}

			// �E�����󋵂̐ݒ菈��
			player.SetRight(true);
		}
		else
		{ // ��L�ȊO

			if (m_nActCount <= PLAYER_FAST_LAMPJUMP_COUNT)
			{ // �ő��X���W�����v�̏ꍇ

				// �ړ��ʂ�ݒ肷��
				move.x = -PLAYER_FAST_LAMPJUMP_MOVE_X;
			}
			else
			{ // ��L�ȊO

				// �ړ��ʂ�ݒ肷��
				move.x = (m_fRotDest + fabsf(fabsf(m_fRotDest) - fabsf(rot.x))) * PLAYER_LAMPJUMP_MOVE_X_MAGNI;
			}

			if (move.x <= -PLAYER_LAMPJUMP_SCORE_MOVE)
			{ // �ړ��ʂ����ȉ��̏ꍇ

				// �A�N�V����UI�𐶐�����
				CActionUI::Create(CActionUI::TYPE_LAMPJUMP, PLAYER_LAMPJUMP_SCORE);
			}

			// �E�����󋵂̐ݒ菈��
			player.SetRight(false);
		}

		// �s���ύX����
		ActionChange(ACT_LAMPJUMP);

		if (m_nActCount <= PLAYER_FAST_LAMPJUMP_COUNT)
		{ // �ő��X���W�����v�̏ꍇ

			// �ړ��ʂ�ݒ肷��
			move.y = PLAYER_LAMPJUMP_MIN_MOVE_Y;
		}
		else
		{ // ��L�ȊO

			// �c�̈ړ��ʂ�ݒ肷��
			move.y = fabsf(m_fRotDest) + PLAYER_LAMPJUMP_MIN_MOVE_Y;
		}

		// ������ 0.0f �ɒ���
		rot.x = 0.0f;

		// �X���̏��ݒ菈��
		player.SetStreetLamp(nullptr);

		// �����v�W�����v���[�V������ݒ肷��
		player.GetMotion()->Set(CPlayer::MOTIONTYPE_LAMPJUMP);

		// �X���W�����v��炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPJUMP);

		// ���x���グ��
		player.SetSpeed(player.GetSpeed() + PLAYER_LAMPJUMP_ADD_SPEED);
	}

	CManager::GetDebugProc()->Print("�ړI�̌����F%f", m_fRotDest);

	// ����ݒ肷��
	player.SetPos(pos);		// �ʒu���X�V����
	player.SetMove(move);	// �ړ��ʂ��X�V����
	player.SetRot(rot);		// �������X�V����
}

//=======================================
// �X��������ł�����
//=======================================
void CPlayerAct::ActLampJump(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = player.GetRot();		// �������擾

	if (player.GetRight() == true)
	{ // �E�������Ă����ꍇ

		// �������E�����ɂ���
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // ���������Ă����ꍇ

		// �������������ɂ���
		rot.y = D3DX_PI * -0.5f;
	}

	// ������ݒ肷��
	player.SetRot(rot);

	//�s���J�E���g�����Z����
	m_nActCount++;

	//if (m_nActCount >= PLAYER_POLEJUMP_COUNT)
	//{ // �s���J�E���g�����ȏ�ɂȂ����ꍇ

	//	// �s����ԕω�����
	//	ActionChange(ACT_NONE);
	//}
}

//=======================================
// �|�[�������ԏ���
//=======================================
void CPlayerAct::ActPoleDown(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = player.GetPos();		// �ʒu���擾����
	D3DXVECTOR3 rot = player.GetRot();		// �������擾����
	float fNowRot;							// ���݂̎x���ɑ΂������

	// �������Z�o����
	fNowRot = atan2f((pos.x - m_posObject.x), (pos.z - m_posObject.z));

	// ���������Z����
	fNowRot += PLAYER_POLEDOWN_ROT_MOVE;

	// ������ݒ肷��
	rot.y = fNowRot + (D3DX_PI * 0.5f);

	// �v���C���[�������Ă���
	pos.y -= PLAYER_POLEDOWN_POSY_MOVE;

	// �����̐��K��
	RotNormalize(&rot.y);

	// �ʒu��ݒ肷��
	pos.x = m_posObject.x + sinf(fNowRot) * POLE_PLAYER_DIST;
	pos.z = m_posObject.z + cosf(fNowRot) * POLE_PLAYER_DIST;

	// �����X�V����
	player.SetPos(pos);		// �ʒu
	player.SetRot(rot);		// ����
}

//=============================================================================���ꂼ��ׂ̍�������=============================================================================

//=======================================
// ���W�����v���̃G�t�F�N�g��������
//=======================================
void CPlayerAct::SuperJumpEffect(CPlayer& player)
{
	// ���[�J���ϐ��錾
	float fAngle, fSpeed;				// �p�x�Ƒ��x
	D3DXVECTOR3 move;					// �ړ���
	D3DXVECTOR3 pos = player.GetPos();	// �ʒu

	// ������ݒ肷��
	pos.y += PLAYER_HEIGHT + PLAYER_BIGJUMP_ADD_POS_Y;

	// ������ݒ肷��
	fAngle = (float)(rand() % 629 - 314) / 100.0f;

	//�ړ��ʂ�ݒ肷��
	fSpeed = (float)(rand() % 500) / 100.0f + 10.0f;

	//�ړ��ʂ�ݒ肷��
	move.x = player.GetMove().x + sinf(fAngle) * fSpeed;
	move.y = player.GetMove().y - fSpeed;
	move.z = player.GetMove().z + cosf(fAngle) * fSpeed;

	// �G�t�F�N�g�𐶐�����
	CEffect::Create(pos, move, 15, 20.0f, CEffect::TYPE_BIGJUMP);

	//�ړ��ʂ�ݒ肷��
	move.x = player.GetMove().x + sinf(-fAngle) * fSpeed;
	move.y = player.GetMove().y - fSpeed;
	move.z = player.GetMove().z + cosf(-fAngle) * fSpeed;

	// �G�t�F�N�g�𐶐�����
	CEffect::Create(pos, move, 15, 20.0f, CEffect::TYPE_BIGJUMP);
}

//=======================================
// �v���C���[�̃X�C���O����
//=======================================
void CPlayerAct::PlayerSwing(CPlayer& player)
{
	// �������擾����
	D3DXVECTOR3 rot = player.GetHierarchy(0)->GetRot();

	// ���������Z����
	rot.x = player.GetRot().x * 0.3f;

	// �������X�V����
	player.GetHierarchy(0)->SetRot(rot);

	// �������擾����
	rot = player.GetHierarchy(1)->GetRot();

	// ���������Z����
	rot.x = player.GetRot().x * -0.3f;

	// �������X�V����
	player.GetHierarchy(1)->SetRot(rot);

	for (int nCnt = 8; nCnt < 12; nCnt++)
	{
		// �������擾����
		rot = player.GetHierarchy(nCnt)->GetRot();

		// ���������Z����
		rot.x = player.GetRot().x * 0.65f;

		if (rot.x >= 0.02f)
		{ // ���������ȏ�ɒB�����ꍇ

			// ������␳����
			rot.x = 0.02f;
		}

		// �������X�V����
		player.GetHierarchy(nCnt)->SetRot(rot);
	}
}

//=======================================
// �ǃL�b�N(�E��)
//=======================================
void CPlayerAct::WallKickRight(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = player.GetRot();		// ����
	D3DXVECTOR3 move = player.GetMove();	// �ړ���

	// �ړ��ʂ�ݒ肷��
	move.x = PLAYER_WALLKICK_SPEED_R.x + player.GetSpeed() * 0.5f;
	move.y = PLAYER_WALLKICK_SPEED_R.y + player.GetSpeed() * 0.1f;

	// ���n�̔g��𐶐�����
	CRipple::Create(player.GetPos(), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -0.25f));

	// �E������
	player.SetRight(true);

	// �������E�����ɂ���
	rot.y = D3DX_PI * 0.5f;

	// �ǃL�b�N�̋��ʏ���
	WallKickProcess(player);

	// ���̐ݒ菈��
	player.SetRot(rot);			// �����̐ݒ菈��
	player.SetMove(move);		// �ړ��ʂ̐ݒ菈��
}

//=======================================
// �ǃL�b�N(����)
//=======================================
void CPlayerAct::WallKickLeft(CPlayer& player)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = player.GetRot();		// ����
	D3DXVECTOR3 move = player.GetMove();	// �ړ���

	// �ړ��ʂ�ݒ肷��
	move.x = PLAYER_WALLKICK_SPEED_L.x - player.GetSpeed() * 0.5f;
	move.y = PLAYER_WALLKICK_SPEED_L.y + player.GetSpeed() * 0.1f;

	// ���n�̔g��𐶐�����
	CRipple::Create(player.GetPos(), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.25f));

	// ��������
	player.SetRight(false);

	// �������������ɂ���
	rot.y = D3DX_PI * -0.5f;

	// �ǃL�b�N�̋��ʏ���
	WallKickProcess(player);

	// ���̐ݒ菈��
	player.SetRot(rot);			// �����̐ݒ菈��
	player.SetMove(move);		// �ړ��ʂ̐ݒ菈��
}

//=======================================
// �ǃL�b�N���ʂ̏���
//=======================================
void CPlayerAct::WallKickProcess(CPlayer& player)
{
	// �s����ԕω�����
	ActionChange(CPlayerAct::ACT_WALLKICK);

	// �O�Ղ̑S���_�̈ʒu�̐ݒ�
	player.GetOrbit()->PosSet(player.GetPos());

	// �Ǔ��ݍ��݃��[�V�����̐ݒ菈��
	player.GetMotion()->Set(CPlayer::MOTIONTYPE_WALLSTEPIN);

	// �X�^�~�i�̌v�Z����
	player.CalcStamina(-PLAYER_WALLKICK_STAMINA);

	if (CManager::GetCamera() != nullptr &&
		CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST)
	{ // �J�����̏�񂪂��邩�A�u�[�X�g��ԈȊO�̏ꍇ

		// �ǃL�b�N���̃J�����̐ݒ菈��
		CManager::GetCamera()->SetType(CCamera::TYPE_WALLKICK);
	}

	// �A�N�V����UI(�ǃL�b�N)�𐶐�
	CActionUI::Create(CActionUI::TYPE_WALLKICK, PLAYER_WALLKICK_SCORE);

	// �j�Ђ̐ݒ菈��
	CFraction::SetWallKick(player.GetPos(), -1, WALLKICK_FRACTION_COL);

	// �ǎC��J�E���g������������
	player.SetWallFricCount(0);

	// �ǃL�b�N����炷
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_WALLKICK);

	// ���x���グ��
	player.SetSpeed(player.GetSpeed() + PLAYER_WALLKICK_ADD_SPEED);
}