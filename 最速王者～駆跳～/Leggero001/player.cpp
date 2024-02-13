//============================================
//
// �v���C���[�̃��C������[player.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "player.h"
#include "game_player.h"
#include "tutorial_player.h"
#include "playerAct.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "sound.h"
#include "useful.h"
#include "texture.h"

#include "actionUI.h"
#include "bgmulti.h"
#include "bullet.h"
#include "block.h"
#include "camera.h"
#include "collision.h"
#include "collpolygon.h"
#include "enemy.h"
#include "Effect.h"
#include "effect2D.h"
#include "Particle.h"
#include "particle2D.h"
#include "file.h"
#include "fraction.h"
#include "motion.h"
#include "orbit.h"
#include "ripple.h"
#include "score.h"
#include "shadowCircle.h"
#include "signpole.h"
#include "staminaUI.h"
#include "streetlamp.h"
#include "speedUI.h"
#include "sonic.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define PLAYER_SIGNPOLE_WALK			(10.0f)		// �W���n��̔������p�}�N����`
#define PLAYER_MOVE_DAMP_MAGNI			(0.25f)		// �ړ��ʂ̌����W��
#define PLAYER_WALLKICK_MOVE_DAMP_MAGNI	(0.00f)		// �ǃL�b�N���̈ړ��ʂ̌����W��
#define PLAYER_JUMP_MOVE_CORRECT		(0.3f)		// �v���C���[�̃W�����v���̈ړ��ʂ̕␳�{��
#define DAMAGE_HITSTOP					(4)			// �_���[�W���̃q�b�g�X�g�b�v
#define PLAYER_JUMP_MOVE_MAGNI			(0.8f)		// �W�����v���̈ړ��ʂ̔{��
#define PLAYER_CRUSH_FLY				(20.0f)		// �v���C���[�̈��E�������̔�ђ��˂鍂��
#define PLAYER_CRUSH_WIDTH				(0.2f)		// �v���C���[�����������Ƃ��̕�
#define PLAYER_CRUSH_WIDTH_SUB			(0.05f)		// �v���C���[�����������Ƃ��̕��̌��Z��
#define PLAYER_BOOST_COUNT				(120)		// �u�[�X�g��Ԃ������J�E���g��
#define PLAYER_JUMP_ATTEN_COUNT			(40)		// �W�����v���̈ړ��ʂ��������n�߂�J�E���g��
#define PLAYER_BOOST_ADD_SPEED			(10.0f)		// �u�[�X�g���̒ǉ��X�s�[�h
#define PLAYER_DASH_SOUND_COUNT			(15)		// �ړ�����炷�J�E���g��
#define PLAYER_CRUSH_COUNT				(180)		// �v���C���[�̈�����Ԃ̃J�E���g
#define PLAYER_CRUSHVOICE_COUNT			(20)		// �ׂꂽ���̐����o���J�E���g
#define PLAYER_BOOSTSOUND_COUNT			(44)		// �X�[�p�[�u�[�X�g�̉��̏o���J�E���g
#define PLAYER_FALLSOUND_COUNT			(28)		// �������������J�E���g
#define PLAYER_FALLSOUND_GRAVITY		(-5.0f)		// �������������d��
#define PLAYER_JUMPCHARGE_SOUND_COUNT	(50)		// ���W�����v�`���[�W�����o���J�E���g��
#define PLAYER_SONIC_JUMP_COUNT			(3)			// �ő��W�����v�̃J�E���g��

//--------------------------------------------
// �}�N����`(�e�N�X�`���n)
//--------------------------------------------
#define ORBIT_TEXTURE			"data/TEXTURE/Orbit.png"					// �ʏ�̋O�Ղ̃e�N�X�`��
#define ORBIT_POLE_TEXTURE		"data/TEXTURE/PoleOrbit.png"				// �Ԃ牺�����Ԃ̋O�Ղ̃e�N�X�`��

//=========================================
// �R���X�g���N�^
//=========================================
CPlayer::CPlayer() : CCharacter(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// �R���X�g���N�^�̔�
	Box();
}

//=========================================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================================
CPlayer::CPlayer(CObject::TYPE type, PRIORITY priority) : CCharacter(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//=========================================
// �f�X�g���N�^
//=========================================
CPlayer::~CPlayer()
{

}

//=========================================
// �R���X�g���N�^�̔�
//=========================================
void CPlayer::Box(void)
{
	// �S�Ă̒l���N���A����
	m_pMotion = nullptr;							// ���[�V�����̏��
	m_pAction = nullptr;							// �s���֌W
	m_pOrbit = nullptr;								// �O�Տ��
	m_pStreetLamp = nullptr;						// �X���̏��
	m_pSonic = nullptr;								// �������o�̏��
	m_pSpeedUI = nullptr;							// ���xUI�̏��
	m_crush.scaleDest = NONE_SCALE;					// �ڕW�̊g�嗦
	m_crush.bTop = false;							// �ォ��̔���
	m_crush.bBottom = false;						// ������̔���
	m_crush.bRight = false;							// �E����̔���
	m_crush.bLeft = false;							// ������̔���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_state = STATE_NONE;							// ���
	m_nStateCount = 0;								// ��ԃJ�E���g
	m_nShadowIdx = INIT_SHADOW;						// �e�̃C���f�b�N�X
	m_nWallFricCount = 0;							// �ǎC��J�E���g
	m_nJumpCount = 0;								// �W�����v���Ă���J�E���g
	m_nOrbitIdx[ORBIT_TEX_NORMAL] = NONE_TEXIDX;	// �O�Ղ̃e�N�X�`���̃C���f�b�N�X
	m_nOrbitIdx[ORBIT_TEX_POLE] = NONE_TEXIDX;		// �O�Ղ̃e�N�X�`���̃C���f�b�N�X
	m_nMoveCount = 0;								// �_�b�V���J�E���g
	m_fSpeed = 0.0f;								// ���x
	m_fStamina = PLAYER_MAX_STAMINA;				// �X�^�~�i
	m_fMoveDest = 0.0f;								// �ړI�̈ړ���
	m_button.nButtonCount = 0;						// �{�^���������Ă��鎞��
	m_button.bSpacePush = false;					// �W�����v�L�[�̉�����
	m_bJump = false;								// �W�����v��
	m_bRight = false;								// �E�������Ă��邩
	m_bMove = false;								// �ړ���
}

//===========================================
// �v���C���[�̏���������
//===========================================
HRESULT CPlayer::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �x�^�ł�
	SetNumModel(14);

	// �f�[�^�̐ݒ菈��
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // ���[�V������ NULL �������ꍇ

		// ���[�V�����̐�������
		m_pMotion = CMotion::Create();
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �x����
		MessageBox(NULL, "�v���C���[�̃��[�V�����̃|�C���^���g���Ă��܂��I", "�x���I", MB_ICONWARNING);
	}

	if (m_pMotion != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ���[�V�����̏����擾����
		m_pMotion->SetModel(GetHierarchy(), GetNumModel());

		// ���[�h����
		m_pMotion->Load(CMotion::TYPE_PLAYER);
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �x����
		MessageBox(NULL, "�v���C���[�̃��[�V�����̃��������m�ۂł��܂���ł����I", "�x���I", MB_ICONWARNING);
	}

	m_pStreetLamp = nullptr;					// �X���̏��
	m_pSonic = nullptr;							// �������o�̏��
	m_pSpeedUI = nullptr;						// ���xUI�̏��
	m_pStaminaUI = nullptr;						// �X�^�~�iUI�̏��
	m_crush.scaleDest = NONE_SCALE;				// �ڕW�̊g�嗦
	m_crush.bTop = false;						// �ォ��̔���
	m_crush.bBottom = false;					// ������̔���
	m_crush.bRight = false;						// �E����̔���
	m_crush.bLeft = false;						// ������̔���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_state = STATE_NONE;						// ���
	m_nStateCount = 0;							// ��ԃJ�E���g
	m_nWallFricCount = 0;						// �ǎC��J�E���g
	m_nJumpCount = 0;							// �W�����v���Ă���J�E���g
	m_nMoveCount = 0;							// �_�b�V���J�E���g
	m_fSpeed = PLAYER_INIT_SPEED;				// ���x
	m_fStamina = PLAYER_MAX_STAMINA;			// �X�^�~�i
	m_fMoveDest = 0.0f;							// �ړI�̈ړ���
	m_button.nButtonCount = 0;					// �{�^���������Ă��鎞��
	m_button.bSpacePush = false;				// �W�����v�L�[�̉�����
	m_bJump = false;							// �W�����v��
	m_bRight = true;							// �E�������Ă��邩
	m_bMove = false;							// �ړ���

	// ���[�V�����̐ݒ菈��
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	// �s���̏��𐶐�����
	m_pAction = CPlayerAct::Create();

	// �e�N�X�`���̓ǂݍ���
	m_nOrbitIdx[ORBIT_TEX_NORMAL] = CManager::GetTexture()->Regist(ORBIT_TEXTURE);
	m_nOrbitIdx[ORBIT_TEX_POLE] = CManager::GetTexture()->Regist(ORBIT_POLE_TEXTURE);

	// �O�Ղ𐶐�����
	m_pOrbit = COrbit::Create(GetPos(), GetRot(), m_nOrbitIdx[ORBIT_TEX_NORMAL]);

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �v���C���[�̏I������
//===========================================
void CPlayer::Uninit(void)
{
	// ���[�V�����̃��������J������
	delete m_pMotion;
	m_pMotion = nullptr;

	// �s���̏��̃��������J������
	delete m_pAction;
	m_pAction = nullptr;

	// �|�C���^�� NULL �ɂ���
	m_pStreetLamp = nullptr;		// �X��
	m_pSonic = nullptr;				// �_�b�V�����o

	if (m_pSpeedUI != nullptr)
	{ // ���xUI�� NULL ����Ȃ������ꍇ

		// �I������
		m_pSpeedUI->Uninit();
		m_pSpeedUI = nullptr;
	}

	if (m_pStaminaUI != nullptr)
	{ // �X�^�~�iUI�� NULL ����Ȃ������ꍇ

		// �I������
		m_pStaminaUI->Uninit();
		m_pStaminaUI = nullptr;
	}

	// �I������
	CCharacter::Uninit();

	// �v���C���[�|�C���^��NULL������
	CGame::DeletePlayer();
}

//===========================================
// �X�^�[�g�X�V����
//===========================================
void CPlayer::StartUpdate(void)
{
	if (CManager::GetCamera() != nullptr)
	{ // �J�����̏�񂪂���ꍇ

		switch (CManager::GetCamera()->GetType())
		{
		case CCamera::TYPE_STARTSTAGE:		// �X�e�[�W�f��

			// ���ɖ���

			break;

		case CCamera::TYPE_STARTRUN:		// ����p

			if (m_nMoveCount % PLAYER_DASH_SOUND_COUNT == 0 &&
				m_bJump == false)
			{ // �ړ��J�E���g����萔�ɒB�������A���ɗv��ꍇ

				// �ړ�����炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DASH);
			}

			// �ړ��J�E���g�����Z����
			m_nMoveCount++;

			{ // �ʒu�̍X�V

				// ���[�J���ϐ��錾
				D3DXVECTOR3 pos = GetPos();

				// ���点��
				pos.x += PLAYER_INIT_SPEED;

				// �ʒu�̐ݒ菈��
				SetPos(pos);
			}

			if (m_pMotion->GetType() != MOTIONTYPE_MOVE)
			{ // �ړ����[�V�����ȊO�̏ꍇ

				// �ړ����[�V������ݒ肷��
				m_pMotion->Set(MOTIONTYPE_MOVE);
			}

			break;

		case CCamera::TYPE_STARTBACK:		// �߂�

			if (m_pMotion->GetType() != MOTIONTYPE_START)
			{ // �X�^�[�g�O���[�V�����ȊO�̏ꍇ

				// �X�^�[�g�O���[�V������ݒ肷��
				m_pMotion->Set(MOTIONTYPE_START);
			}

			break;
		}

		// ���[�V�����̍X�V����
		m_pMotion->Update();
	}

	if (m_bJump == true)
	{ // �W�����v�󋵂� true �̏ꍇ

		// �ʒu�̐ݒ菈��(Y������)
		CShadowCircle::SetPosRotXZ(m_nShadowIdx, GetPos(), GetRot());
	}
	else
	{ // ��L�ȊO

		// �e�̈ʒu�����ݒ菈��
		CShadowCircle::SetPosRot(m_nShadowIdx, GetPos(), GetRot());
	}
}

//===========================================
// �J�E���g�_�E���X�V����
//===========================================
void CPlayer::CountDownUpdate(void)
{
	if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL)
	{ // �������[�V�����ȊO�̏ꍇ

		// �������[�V������ݒ肷��
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);
	}

	// ���[�V�����̍X�V����
	m_pMotion->Update();
}

//===========================================
// �ʏ�X�V����
//===========================================
void CPlayer::MainUpdate(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾
	bool bRight = false;			// �E
	bool bLeft = false;				// ��

	// �O��̈ʒu��ۑ�����
	SetPosOld(pos);

	// ��ԏ���
	State();

	if (m_state != STATE_CRUSH)
	{ // ���E��ԈȊO�̏ꍇ

		if (m_bMove == true)
		{ // �ړ����Ă���ꍇ

			// �ړ��J�E���g�����Z����
			m_nMoveCount++;

			// �X�^�~�i����ʎ����񕜂�����
			CalcStamina(PLAYER_AUTO_HEAL_STAMINA_MOVE);

			if (m_nMoveCount % PLAYER_DASH_SOUND_COUNT == 0 &&
				m_bJump == false)
			{ // �ړ��J�E���g����萔�ɒB�������A���ɗv��ꍇ

				// �ړ�����炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DASH);
			}
		}
		else
		{ // �ړ����Ă��Ȃ��ꍇ

			// �X�^�~�i�������񕜂�����
			CalcStamina(PLAYER_AUTO_HEAL_STAMINA_STOP);

			// �J�E���g�� 0 �ɂ���
			m_nMoveCount = 0;
		}

		// ���쏈��
		Control();

		// �ʒu���擾����
		pos = GetPos();

		if (m_pAction->GetAct() != CPlayerAct::ACT_POLEDOWN)
		{ // �|�[�������ԈȊO�̏ꍇ

			// �ʒu��ݒ肷��
			pos.z = 0.0f;
		}

		// �ړ��ʂ����Z����
		pos.x += m_move.x;

		// ��������̚��̐ݒ菈��
		RunDustEffect();

		// �ʒu�̐ݒ菈��
		SetPos(pos);

		// �d�͏���
		PlayerGravity();
	}

	// �u���b�N�̓����蔻�菈��
	StageCollision(&bRight, &bLeft);

	if (m_state != STATE_CRUSH)
	{ // ���E��ԈȊO�̏ꍇ

		// �ǎC�菈��
		WallFriction(bRight, bLeft);

		// �ǃL�b�N����
		WallKick(bRight, bLeft);

		// �s����ԏ���
		m_pAction->Action(*this);

		// �A�C�e���Ƃ̓����蔻��
		collision::PlayerItemCollision(*this);
	}

	// ���x�̐���������
	SpeedStarEffect();

	// �n�ʒ��n����
	GroundLanding();

	// �ʒu�̎擾����
	pos = GetPos();

	// �ʒu�̐ݒ菈��
	SetPos(pos);

	if (m_state != STATE_CRUSH)
	{ // ���E��ԈȊO�̏ꍇ

		// ���[�V�����̍X�V����
		m_pMotion->Update();

		// �v���C���[�ł̋O�Տ���
		PlayerOrbit();
	}

	if (m_pAction->GetAct() == CPlayerAct::ACT_NONE &&
		m_pMotion->GetType() != MOTIONTYPE_FALL &&
		m_move.y < -10.0f)
	{ // �����Ă���ꍇ

		// �������[�V������ݒ肷��
		m_pMotion->Set(MOTIONTYPE_FALL);
	}

	//// �v���C���[�ł̍������o����
	//PlayerSonic();

	// �v���C���[�ƃJ�����͈̔͂̓����蔻�菈��
	collision::PlayerCameraRangeCollision(*this, PLAYER_WIDTH, PLAYER_HEIGHT);

	if (CManager::GetDebugProc() != nullptr)
	{ // �f�o�b�O�\���� NULL ����Ȃ��ꍇ

		// �f�o�b�O�\��
		CManager::GetDebugProc()->Print("�ړ��ʁF%f\n", m_move.x);
	}
}

//===========================================
// �S�[���X�V����
//===========================================
void CPlayer::GoalUpdate(void)
{
	if (m_pMotion->GetType() != MOTIONTYPE_MOVE)
	{ // �ړ����[�V�����ȊO�̏ꍇ

		// ���胂�[�V������ǉ�����
		m_pMotion->Set(MOTIONTYPE_MOVE);
	}

	// �s���󋵂�ʏ��Ԃɂ���
	m_pAction->ActionChange(CPlayerAct::ACT_NONE);

	// �E����������
	m_bRight = true;

	// ���[�V�����̍X�V����
	m_pMotion->Update();
}

//===========================================
// �I���X�V����
//===========================================
void CPlayer::FinishUpdate(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �v���C���[�̈ʒu
	bool bRight = false;			// �E
	bool bLeft = false;				// ��

	if (m_nMoveCount % PLAYER_DASH_SOUND_COUNT == 0 &&
		m_bJump == false)
	{ // �ړ��J�E���g����萔�ɒB�������A���ɗv��ꍇ

		// �ړ�����炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DASH);
	}

	// �ړ��J�E���g�����Z����
	m_nMoveCount++;

	// ���ɕ�������
	pos.x += PLAYER_MIN_SPEED;

	// �ʒu�̐ݒ菈��
	SetPos(pos);

	// ��������̚��̐ݒ菈��
	RunDustEffect();

	// �d�͏���
	PlayerGravity();

	// �u���b�N�̓����蔻�菈��
	StageCollision(&bRight, &bLeft);

	// �n�ʒ��n����
	GroundLanding();

	// �E����������
	m_bRight = true;

	// �ړ�����
	m_bMove = true;

	// ���[�V�����̍X�V����
	m_pMotion->Update();
}

//===========================================
// �v���C���[�̕`�揈��
//===========================================
void CPlayer::Draw(void)
{
	switch (m_state)
	{
	case STATE_NONE:		// �ʏ���

		// �`�揈��
		CCharacter::Draw();

		break;

	case STATE_BOOST:		// �u�[�X�g���

		// �`�揈��
		CCharacter::Draw();

		break;

	case STATE_DAMAGE:		// �_���[�W���

		// �`�揈��
		CCharacter::Draw(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		break;

	case STATE_INVINCIBLE:	// ���G���

		// �`�揈��
		CCharacter::Draw(m_col.a);

		break;

	case STATE_CRUSH:

		// �`�揈��
		CCharacter::Draw();

		break;
	}
}

//===========================================
// �F�̐ݒ菈��
//===========================================
void CPlayer::SetColor(const D3DXCOLOR& col)
{
	// �F��ݒ肷��
	m_col = col;
}

//===========================================
// �F�̎擾����
//===========================================
D3DXCOLOR CPlayer::GetColor(void) const
{
	// �F��Ԃ�
	return m_col;
}

//===========================================
// �ړ��ʂ̐ݒ菈��
//===========================================
void CPlayer::SetMove(const D3DXVECTOR3& move)
{
	// �ړ��ʂ�ݒ肷��
	m_move = move;
}

//===========================================
// �ړ��ʂ̎擾����
//===========================================
D3DXVECTOR3 CPlayer::GetMove(void) const
{
	// �ړ��ʂ�Ԃ�
	return m_move;
}

//===========================================
// ��Ԃ̐ݒ菈��
//===========================================
void CPlayer::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;

	// ��ԃJ�E���g��ݒ肷��
	m_nStateCount = 0;
}

//===========================================
// ��Ԃ̎擾����
//===========================================
CPlayer::STATE CPlayer::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//===========================================
// �W�����v�󋵂̐ݒ菈��
//===========================================
void CPlayer::SetJump(const bool bJump)
{
	// �W�����v�󋵂�ݒ肷��
	m_bJump = bJump;
}

//===========================================
// �W�����v�󋵂̎擾����
//===========================================
bool CPlayer::GetJump(void) const
{
	// �W�����v�󋵂�Ԃ�
	return m_bJump;
}

//===========================================
// ���x�̐ݒ菈��
//===========================================
void CPlayer::SetSpeed(float fSpeed)
{
	if (fSpeed >= PLAYER_MAX_SPEED)
	{ // ���x���ő�l�ȏ�̏ꍇ

		// ���x��ݒ肷��
		fSpeed = PLAYER_MAX_SPEED;
	}

	// ���x��ݒ肷��
	m_fSpeed = fSpeed;
}

//===========================================
// ���x�̎擾����
//===========================================
float CPlayer::GetSpeed(void) const
{
	// ���x��Ԃ�
	return m_fSpeed;
}

//===========================================
// �{�^�������󋵂̐ݒ菈��
//===========================================
void CPlayer::SetButtonPush(const bool bPush)
{
	// �{�^���̉����󋵂�ݒ肷��
	m_button.bSpacePush = bPush;
}

//===========================================
// �{�^���J�E���g�̐ݒ菈��
//===========================================
void CPlayer::SetButtonCount(const int nCount)
{
	// �{�^���̃J�E���g��ݒ肷��
	m_button.nButtonCount = nCount;
}

//===========================================
// �{�^���J�E���g�̎擾����
//===========================================
CPlayer::SButton CPlayer::GetButton(void)
{
	// �{�^���̏���Ԃ�
	return m_button;
}

//===========================================
// �E�����󋵂̐ݒ菈��
//===========================================
void CPlayer::SetRight(const bool bRight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// �E�����󋵂�ݒ肷��
	m_bRight = bRight;

	if (m_bRight == true)
	{ // �E�������Ă����ꍇ

		// �������E�����ɂ���
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // ���������Ă����ꍇ

		// �������������ɂ���
		rot.y = D3DX_PI * -0.5f;
	}

	// �������X�V����
	SetRot(rot);
}

//===========================================
// �E�����󋵂̎擾����
//===========================================
bool CPlayer::GetRight(void) const
{
	// �E�����󋵂�Ԃ�
	return m_bRight;
}

//===========================================
// �ǎC��J�E���g�̐ݒ菈��
//===========================================
void CPlayer::SetWallFricCount(const int nCount)
{
	// �ǎC��J�E���g��ݒ肷��
	m_nWallFricCount = nCount;
}

//===========================================
// ���E����(��)�̐ݒ菈��
//===========================================
void CPlayer::SetCrushTop(const bool bTop)
{
	// �ォ��̔����ݒ肷��
	m_crush.bTop = bTop;
}

//===========================================
// ���E����(��)�̐ݒ菈��
//===========================================
void CPlayer::SetCrushBottom(const bool bBottom)
{
	// ������̔����ݒ肷��
	m_crush.bBottom = bBottom;
}

//===========================================
// ���E����(�E)�̐ݒ菈��
//===========================================
void CPlayer::SetCrushRight(const bool bRight)
{
	// �E����̔����ݒ肷��
	m_crush.bRight = bRight;
}

//===========================================
// ���E����(��)�̐ݒ菈��
//===========================================
void CPlayer::SetCrushLeft(const bool bLeft)
{
	// ������̔����ݒ肷��
	m_crush.bLeft = bLeft;
}

//===========================================
// ���[�V�����̏��̐ݒ菈��
//===========================================
CMotion* CPlayer::GetMotion(void)
{
	// ���[�V�����̏���Ԃ�
	return m_pMotion;
}

//===========================================
// �s���̏��̎擾����
//===========================================
CPlayerAct* CPlayer::GetAction(void)
{
	// �s������Ԃ�
	return m_pAction;
}

//===========================================
// �O�Ղ̏��̎擾����
//===========================================
COrbit* CPlayer::GetOrbit(void)
{
	// �O�Ղ̏���Ԃ�
	return m_pOrbit;
}

//===========================================
// ��������
//===========================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos, const CScene::MODE mode)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CPlayer* pPlayer = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pPlayer == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		switch (mode)
		{
		case CScene::MODE_TUTORIAL:		// �`���[�g���A��

			// ���������m�ۂ���
			pPlayer = new CTutorialPlayer;

			break;

		case CScene::MODE_GAME:			// �Q�[��

			// ���������m�ۂ���
			pPlayer = new CGamePlayer;

			break;

		default:						// ��L�ȊO

			// NULL ��Ԃ�
			return nullptr;

			break;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pPlayer != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�v���C���[�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pPlayer->SetData(pos, mode);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �v���C���[�̃|�C���^��Ԃ�
	return pPlayer;
}

//===========================================
// ���쏈��
//===========================================
void CPlayer::Control(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu�̎擾

	// �A�N�V�������쏈��
	ActionControl();

	// ��Q���̓����蔻��
	collision::PlayerObstcleAttack(this);

	if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCATCH)
	{ // �R���܂��ԈȊO

		if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFLAND && 
			m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCLIMB && 
			m_pAction->GetAct() != CPlayerAct::ACT_HANGING)
		{ // �R�������n��Ԃ��A�R������ԈȊO

			// �v���C���[�̈ړ�����
			Move();

			// �W�����v����
			Jump();
		}
	}

	// �ʒu�̐ݒ菈��
	SetPos(pos);

	if (m_bJump == true)
	{ // �W�����v���̏ꍇ

		if (m_nJumpCount % PLAYER_FALLSOUND_COUNT == 0 &&
			m_move.y <= PLAYER_FALLSOUND_GRAVITY)
		{ // ���J�E���g���o�߂������A�d�͂����ȉ��̏ꍇ

			// ��������炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FALLING);
		}

		// �W�����v�J�E���g�����Z����
		m_nJumpCount++;
	}
	else
	{ // ��L�ȊO

		// �W�����v�J�E���g�����Z�b�g����
		m_nJumpCount = 0;
	}
}

//===========================================
// �d�͏���
//===========================================
void CPlayer::PlayerGravity(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();			// �ʒu���擾����

	if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCATCH)
	{ // �R���܂��ԈȊO

		if (m_pAction->GetAct() == CPlayerAct::ACT_WALLKICK)
		{ // �ǃL�b�N��Ԃ̏ꍇ

			// �d�͏���
			Gravity(&m_move.y, pos, PLAYER_WALLKICK_GRAVITY);
		}
		else if (m_pAction->GetAct() == CPlayerAct::ACT_WALLSTEPIN)
		{ // �Ǔ��ݍ��ݏ�Ԃ̏ꍇ

			// �ړ��ʂ� 0 �ɂ���
			m_move.y = 0.0f;
		}
		else if (m_pAction->GetAct() == CPlayerAct::ACT::ACT_CLIFFCLIMB)
		{ // �R������Ԃ̏ꍇ

			// �d�͏���
			Gravity(&m_move.y, pos, m_pAction->GetCliffGravity());
		}
		else if(m_pAction->GetAct() != CPlayerAct::ACT::ACT_HANGING &&
			m_pAction->GetAct() != CPlayerAct::ACT_POLEDOWN)
		{ // �Ԃ牺�����ԈȊO���A�|�[���_�E����ԈȊO�̏ꍇ

			// �d�͏���
			Gravity(&m_move.y, pos, PLAYER_GRAVITY);
		}
	}

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}

//===========================================
// �A�N�V�����{�^���̑��쏈��
//===========================================
void CPlayer::ActionControl(void)
{
	if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW &&
		(CManager::GetInputKeyboard()->GetPress(DIK_SPACE) == true || CManager::GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_A, 0) == true))
	{ // �]�C��ԈȊO���ASPACE�L�[�������Ă����ꍇ

		// �{�^���J�E���g�����Z����
		m_button.nButtonCount++;

		if (m_fStamina >= PLAYER_BIGJUMP_STAMINA &&
			m_button.nButtonCount >= PLAYER_BIGJUMP_BTNCOUNT &&
			m_bJump == false)
		{ // ���W�����v���X�^�~�i�����邩�A�{�^���J�E���g�����ȏ�ɂȂ������A�W�����v���Ă��Ȃ��ꍇ

			if (m_button.nButtonCount % PLAYER_JUMPCHARGE_SOUND_COUNT == 0)
			{ // ���W�����v����{�^���J�E���g����萔�ɒB�����ꍇ

				// ���W�����v�`���[�W����炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMPCHARGE);
			}

			// ���W�����v�̃`���[�W�̃p�[�e�B�N����ݒ�
			CParticle::Create(D3DXVECTOR3(GetPos().x, GetPos().y + (PLAYER_HEIGHT * 0.5f), GetPos().z), CEffect::TYPE_BIGJUMPCHARGE);
		}
	}

	if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW &&
		(CManager::GetInputKeyboard()->GetRelease(DIK_SPACE) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::JOYKEY_A, 0) == true))
	{ // SPACE�L�[�𗣂����ꍇ

		// �����󋵂� true �ɂ���
		m_button.bSpacePush = true;
	}
	else
	{ // ��L�ȊO

		if (m_button.bSpacePush == true)
		{ // �O��̉����󋵂� true �̏ꍇ

			// �{�^���J�E���g��0�ɂ���
			m_button.nButtonCount = 0;
		}

		// �����󋵂� false �ɂ���
		m_button.bSpacePush = false;
	}
}

//===========================================
// �v���C���[�̈ړ�����
//===========================================
void CPlayer::Move(void)
{
	if (m_pAction->GetAct() != CPlayerAct::ACT_WALLKICK &&
		m_pAction->GetAct() != CPlayerAct::ACT_LAMPJUMP && 
		m_pAction->GetAct() != CPlayerAct::ACT_POLEDOWN &&
		m_pAction->GetAct() != CPlayerAct::ACT_WALLSTEPIN)
	{ // �A�N�V�������ȊO�̏ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true ||
			CManager::GetInputGamePad()->GetGameStickLXPress(0) < 0)
		{ // A�L�[�������Ă����ꍇ

			// �ړ��̋��ʂ̏���
			MoveProcess();

			if (m_bRight == true)
			{ // �E�����󋵂� true �̏ꍇ

				// ��������
				m_bRight = false;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true ||
			CManager::GetInputGamePad()->GetGameStickLXPress(0) > 0)
		{ // D�L�[�������Ă���ꍇ

			// �ړ��̋��ʂ̏���
			MoveProcess();

			if (m_bRight == false)
			{ // �E�����󋵂� false �̏ꍇ

				// �E������
				m_bRight = true;
			}
		}
		else
		{ // ���������Ă��Ȃ��ꍇ

			if(m_pAction->GetAct() != CPlayerAct::ACT_WALLKICK)
			{ // �ǃL�b�N������Ȃ��ꍇ

				if (m_bJump == true)
				{ // �W�����v���̏ꍇ

					// �ڕW�̈ړ��ʂ�ݒ肷��
					m_fMoveDest = 0.0f;
				}
				else
				{ // �n�ʂɗ����Ă���ꍇ

					// �ړ��ʂ��X�V(��������)
					m_move.x += (0.0f - m_move.x) * PLAYER_MOVE_DAMP_MAGNI;
				}
			}

			if (m_bMove == true)
			{ // �ړ��󋵂� true �̏ꍇ

				// �ړ��󋵂� false �ɂ���
				m_bMove = false;

				if (m_pMotion->GetType() != CPlayer::MOTIONTYPE_BIGJUMP &&
					m_pMotion->GetType() != MOTIONTYPE_LANDING)
				{ // ���W�����v���[�V�����E���n���[�V�����ȊO�̏ꍇ

					// �ʏ탂�[�V�����ɐݒ肷��
					m_pMotion->Set(MOTIONTYPE_NEUTRAL);
				}
			}
		}
	}

	if (m_pAction->GetAct() == CPlayerAct::ACT_WALLKICK)
	{ // �ǃL�b�N���̏ꍇ

		// �ړ��ʂ��X�V(��������)
		m_move.x += (0.0f - m_move.x) * PLAYER_WALLKICK_MOVE_DAMP_MAGNI;
	}
	else if (m_pAction->GetAct() == CPlayerAct::ACT_NONE)
	{ // �ʏ��Ԃ̏ꍇ

		if (m_bJump == true &&
			m_nJumpCount >= PLAYER_JUMP_ATTEN_COUNT)
		{ // �󒆂ɂ���ꍇ

			// �ړ��ʂ�ݒ肷��
			m_move.x *= PLAYER_JUMP_MOVE_MAGNI;
		}
	}

	if (m_bJump == true &&
		(m_pAction->GetAct() == CPlayerAct::ACT_NONE ||
			m_pAction->GetAct() == CPlayerAct::ACT_BIGJUMP ||
			m_pAction->GetAct() == CPlayerAct::ACT_WALLFRIC ||
			m_pAction->GetAct() == CPlayerAct::ACT_WALLAFTERGLOW))
	{ // �W�����v���̏ꍇ

		// �ړ��ʂ̕␳����
		Correct(m_fMoveDest, &m_move.x, PLAYER_JUMP_MOVE_CORRECT);
	}
}

//=======================================
// �ړ��̋��ʂ̏���
//=======================================
void CPlayer::MoveProcess(void)
{
	if (m_bJump == true)
	{ // �󒆂ɂ���ꍇ

		// �ړ��ʂ�ݒ肷��
		m_fMoveDest = sinf(GetRot().y) * m_fSpeed * PLAYER_JUMP_MOVE_MAGNI;
	}
	else
	{ // �n��ɂ���ꍇ

		// �ړ��ʂ�ݒ肷��
		m_move.x = sinf(GetRot().y) * m_fSpeed;
	}

	if (m_bMove == false)
	{ // �ړ��󋵂� false�@�̏ꍇ

		// �ړ��󋵂� true �ɂ���
		m_bMove = true;

		if (m_bJump == false)
		{ // ���ɂ����ꍇ

			// ����n�߉���炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RUNSTART);
		}
		
		if (m_pMotion->GetType() != CPlayer::MOTIONTYPE_BIGJUMP &&
			m_pMotion->GetType() != MOTIONTYPE_LANDING)
		{ // ���W�����v�E���n���[�V�����ȊO�̏ꍇ

			// �ړ����[�V�����ɐݒ肷��
			m_pMotion->Set(MOTIONTYPE_MOVE);
		}
	}
}

//=======================================
// ���x�̐��̔�������
//=======================================
void CPlayer::SpeedStarEffect(void)
{
	if (m_bMove == true &&
		m_fSpeed >= PLAYER_MAX_SPEED)
	{ // �ʏ��Ԃ��A�ړ����Ă���ꍇ

		// [�����p�ϐ�]
		D3DXVECTOR3 posEffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �G�t�F�N�g�̈ʒu
		int nLife = 0;											// �̗�
		float fRadius = 0.0f;									// ���a

		if (m_bRight == true)
		{ // �E�����̏ꍇ

			// �G�t�F�N�g�̈ʒu��ݒ肷��
			posEffect.x = GetPos().x + sinf(-GetRot().x) * (rand() % (int)(PLAYER_HEIGHT));
			posEffect.y = GetPos().y + cosf(-GetRot().x) * (rand() % (int)(PLAYER_HEIGHT));
			posEffect.z = GetPos().z;
		}
		else
		{ // �������̏ꍇ

			// �G�t�F�N�g�̈ʒu��ݒ肷��
			posEffect.x = GetPos().x + sinf(GetRot().x) * (rand() % (int)(PLAYER_HEIGHT));
			posEffect.y = GetPos().y + cosf(GetRot().x) * (rand() % (int)(PLAYER_HEIGHT));
			posEffect.z = GetPos().z;
		}

		// ���a��ݒ肷��
		fRadius = (float)(rand() % 1000) / 100.0f + 15.0f;

		// �����̐ݒ�
		nLife = rand() % 15 + 45;

		// �G�t�F�N�g�̐ݒ菈��
		CEffect::Create
		(
			posEffect,		// �ʒu
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �ړ���
			nLife,			// ����
			fRadius,		// ���a
			CEffect::TYPE_SPEEDSTAR			// ���
		);
	}
}

//=======================================
// ��������̚��̔�������
//=======================================
void CPlayer::RunDustEffect(void)
{
	if (m_bMove == true &&
		m_bJump == false)
	{ // �n�ʂ��ړ����Ă���ꍇ

		// [�����p�ϐ�]
		D3DXVECTOR3 posEffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �G�t�F�N�g�̈ʒu
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		float fRadius = 0.0f;									// ���a

		// �G�t�F�N�g�̈ʒu��ݒ肷��
		posEffect.x = GetPos().x + sinf(-GetRot().y) * (rand() % 10);
		posEffect.y = GetPos().y;
		posEffect.z = GetPos().z;

		// �ړ��ʂ�ݒ肷��
		move.x = sinf(-GetRot().y) * (float)(rand() % 5 + (m_fSpeed * 0.5f));
		move.y = (float)(rand() % 3 + 2);
		move.z = 0.0f;

		// ���a��ݒ肷��
		fRadius = (float)((rand() % 8) + (m_fSpeed * 0.5f));

		// �G�t�F�N�g�̐ݒ菈��
		CEffect::Create
		(
			posEffect,		// �ʒu
			move,			// �ړ���
			30,				// ����
			fRadius,		// ���a
			CEffect::TYPE_RUNDUST			// ���
		);
	}
}

//=======================================
// �W�����v����
//=======================================
void CPlayer::Jump(void)
{
	if (m_bJump == false &&
		m_pAction->GetAct() != CPlayerAct::ACT_CLIFFLAND &&
		m_pAction->GetAct() != CPlayerAct::ACT_CLIFFAFTERGLOW &&
		m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCATCH &&
		m_button.bSpacePush == true)
	{ // �W�����v�󋵂� false ���A����ԈȊO�̏ꍇ

		// �W�����v����
		m_bJump = true;

		if (m_button.nButtonCount <= PLAYER_SONIC_JUMP_COUNT)
		{ // �ő��W�����v

			// �W�����v������
			m_move.y = PLAYER_JUMP;

			// �W�����v���𗬂�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMP);

			// �W�����v���[�V�����ɐݒ肷��
			m_pMotion->Set(MOTIONTYPE_JUMP);
		}
		else
		{ // ��L�ȊO

			if (m_fStamina >= PLAYER_BIGJUMP_STAMINA &&
				m_button.nButtonCount >= PLAYER_BIGJUMP_BTNCOUNT)
			{ // �X�^�~�i���]���Ă��邩�A�{�^���J�E���g����萔�̏ꍇ

				// �W�����v������
				m_move.y = PLAYER_BIGJUMP;

				// �s����ԕω�����
				m_pAction->ActionChange(CPlayerAct::ACT_BIGJUMP);

				// �O�Ղ̑S���_�̈ʒu�̐ݒ�
				m_pOrbit->PosSet(GetPos());

				// �A�N�V����UI(���W�����v)�𐶐�
				CActionUI::Create(CActionUI::TYPE_SUPERJUMP, PLAYER_SUPERJUMP_SCORE);

				// �W�����v���[�V�����ɐݒ肷��
				m_pMotion->Set(MOTIONTYPE_BIGJUMP);

				// �X�^�~�i�̌v�Z����
				CalcStamina(-PLAYER_BIGJUMP_STAMINA);

				// �W�����v���𗬂�
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BIGJUMP);

				// ���x���グ��
				SetSpeed(m_fSpeed + PLAYER_BIGJUMP_ADD_SPEED);
			}
			else
			{ // ��L�ȊO

				// �W�����v������
				m_move.y = PLAYER_JUMP;

				// �W�����v���[�V�����ɐݒ肷��
				m_pMotion->Set(MOTIONTYPE_JUMP);

				// �W�����v���𗬂�
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMP);
			}
		}
	}
}

//=======================================
// �q�b�g����
//=======================================
void CPlayer::Hit(void)
{
	if (m_state == STATE_NONE)
	{ // �ʏ��Ԃ̏ꍇ

		// �_���[�W��Ԃɂ���
		SetState(STATE_DAMAGE);

		// �q�b�g�X�g�b�v�̐ݒ菈��
		CGame::SetHitStop(DAMAGE_HITSTOP);
	}

	// �����p�[�e�B�N���𐶐�
	CParticle::Create(GetPos(), CEffect::TYPE::TYPE_FIRE);
}

//=======================================
// ���̐ݒ菈��
//=======================================
void CPlayer::SetData(const D3DXVECTOR3& pos, const CScene::MODE mode)
{
	// �S�Ă̒l������������
	SetPos(pos);			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));			// ����
	SetScale(NONE_SCALE);	// �g�嗦

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// ����������
		GetHierarchy(nCntData)->SetPos(pos);										// �ʒu
		GetHierarchy(nCntData)->SetPosOld(pos);										// �O��̈ʒu
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// ����
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);								// �g�嗦
		GetHierarchy(nCntData)->SetFileData(CXFile::TYPE(INIT_PLAYER + nCntData));	// �f�[�^�̐ݒ菈��
	}

	// ���[�J���ϐ��錾
	CShadowCircle* pShadow = nullptr;		// �e�ւ̃|�C���^

	// �e�̐ݒ菈��
	pShadow = CShadowCircle::Create(pos, GetRot(), 40.0f);

	if (pShadow != nullptr)
	{ // �e�̃|�C���^�� NULL�ȊO�̏ꍇ

		// �e�̃C���f�b�N�X�ݒ�
		m_nShadowIdx = pShadow->GetNumID();
	}

	// ���xUI�̐�������
	m_pSpeedUI = CSpeedUI::Create(mode);

	// �X�^�~�i�̃��������m�ۂ���
	m_pStaminaUI = CStaminaUI::Create(mode);
}

//=======================================
// ��ԏ���
//=======================================
void CPlayer::State(void)
{
	switch (m_state)
	{
	case STATE_NONE:		// �ʏ���

		// �ʏ��ԏ���
		StateNone();

		break;

	case STATE_BOOST:		// �u�[�X�g���

		// �u�[�X�g��ԏ���
		StateBoost();

		break;

	case STATE_DAMAGE:		// �_���[�W���

		// �_���[�W��ԏ���
		StateDamage();

		break;

	case STATE_INVINCIBLE:	// ���G���

		// ���G��ԏ���
		StateInvincible();

		break;

	case STATE_CRUSH:		// ���E���

		// ���E��ԏ���
		StateCrush();

		break;
	}
}

//=======================================
// �ʏ��ԏ���
//=======================================
void CPlayer::StateNone(void)
{
	// �F��ݒ肷��
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=======================================
// �u�[�X�g���
//=======================================
void CPlayer::StateBoost(void)
{
	if (CManager::GetCamera() != nullptr &&
		CManager::GetCamera()->GetType() == CCamera::TYPE_NONE)
	{ // �J�������ʏ��Ԃ̏ꍇ

		// �u�[�X�g�J������ݒ肷��
		CManager::GetCamera()->SetType(CCamera::TYPE_BOOST);
	}

	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	// ���x�𑬂�����
	m_fSpeed = PLAYER_MAX_SPEED + PLAYER_BOOST_ADD_SPEED;

	if (m_nStateCount >= PLAYER_BOOST_COUNT)
	{ // ��ԃJ�E���g����萔�ɒB�����ꍇ

		// �ʏ��Ԃɂ���
		SetState(STATE_NONE);

		// �X�s�[�h��߂�
		m_fSpeed = PLAYER_MAX_SPEED;
	}

	if (m_nStateCount % PLAYER_BOOSTSOUND_COUNT == 0)
	{ // ��ԃJ�E���g������炷�J�E���g�ɒB�����ꍇ

		// �X�[�p�[�u�[�X�g�̉���炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SUPERDASH);
	}

	if (m_pSpeedUI != nullptr)
	{ // �X�^�~�iUI�̏�� NULL ����Ȃ��ꍇ

		// ���[�J���ϐ��錾
		D3DXVECTOR3 Effectpos;		// �G�t�F�N�g�̈ʒu
		D3DXVECTOR3 size;			// �X�s�[�hUI�̃��[�^�[�̈ʒu

		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			// �ʒu������������
			Effectpos = m_pSpeedUI->GetObject2D(CSpeedUI::TYPE_METER)->GetPos();
			size = m_pSpeedUI->GetObject2D(CSpeedUI::TYPE_METER)->GetSize();

			// �ʒu��ݒ肷��
			Effectpos.x = Effectpos.x + rand() % (int)(size.x);
			Effectpos.y = Effectpos.y + size.y;
			Effectpos.z = 0.0f;

			// ���E�˔j�����o��
			CParticle2D::Create(Effectpos, CEffect2D::TYPE_OVERTOP);
		}
	}
}

//=======================================
// �_���[�W��ԏ���
//=======================================
void CPlayer::StateDamage(void)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	// �F��Ԃɐݒ肷��
	m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	if (m_nStateCount >= PLAYER_DAMAGE_COUNT)
	{ // ��ԃJ�E���g����萔�ɒB�����ꍇ

		// ���G��Ԃɂ���
		SetState(STATE_INVINCIBLE);

		// �F��ʏ�ɐݒ肷��
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

//=======================================
// ���G��ԏ���
//=======================================
void CPlayer::StateInvincible(void)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount % 15 == 0)
	{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

		if (m_col.a >= 1.0f)
		{ // �f�[�^�� 0.0f �ȉ��̏ꍇ

			// �f�[�^�� 1.0f �ɂ���
			m_col.a = 0.0f;
		}
		else if (m_col.a <= 0.0f)
		{ // �f�[�^�� 1.0f �ȏ�̏ꍇ

			// �f�[�^�� 0.0f �ɂ���
			m_col.a = 1.0f;
		}
	}

	if (m_nStateCount >= PLAYER_INVINCIBLE_COUNT)
	{ // ��ԃJ�E���g����萔�ɒB�����ꍇ

		// ��Ԃ�ݒ肷��
		SetState(STATE_NONE);
	}
}

//=======================================
// ���E��ԏ���
//=======================================
void CPlayer::StateCrush(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();			// �ʒu���擾����
	D3DXVECTOR3 scale = GetScale();		// �g�嗦���擾����

	// �g�嗦��������
	scale.y -= PLAYER_CRUSH_WIDTH_SUB;
	scale.z -= PLAYER_CRUSH_WIDTH_SUB;

	if (scale.y <= m_crush.scaleDest.y)
	{ // �g�嗦���ڕW�ȉ��ɂȂ����ꍇ

		// �g�嗦��ݒ肷��
		scale.y = m_crush.scaleDest.y;
	}

	if (scale.z <= m_crush.scaleDest.z)
	{ // �g�嗦���ڕW�ȉ��ɂȂ����ꍇ

		// �g�嗦��ݒ肷��
		scale.z = m_crush.scaleDest.z;
	}

	// �e���f���̊g�嗦�̐ݒ�
	for (int nCnt = 0; nCnt < GetNumModel(); nCnt++)
	{
		if (GetHierarchy(nCnt) != nullptr)
		{ // ���f���� NULL ����Ȃ��ꍇ

			// �g�嗦��ݒ肷��
			GetHierarchy(nCnt)->SetScale(scale);
		}
	}

	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount % PLAYER_CRUSH_COUNT == 0)
	{ // ��ԃJ�E���g����萔�ɒB�����ꍇ

		// �S���f���̊g�嗦�𒼂�
		for (int nCnt = 0; nCnt < GetNumModel(); nCnt++)
		{
			if (GetHierarchy(nCnt) != nullptr)
			{
				GetHierarchy(nCnt)->SetScale(NONE_SCALE);
			}
		}

		// �g�嗦��ݒ肷��
		scale = NONE_SCALE;

		// ���G��Ԃɂ���
		m_state = STATE_INVINCIBLE;

		// ��ԃJ�E���g������������
		m_nStateCount = 0;

		// ��ɏ�����ђ��˂�����
		pos.y = pos.y + PLAYER_CRUSH_FLY;
	}

	if (m_nStateCount == PLAYER_CRUSHVOICE_COUNT)
	{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

		// �ׂꂽ���̐����o��
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_PRESSVOICE);
	}

	// ����ݒ肷��
	SetPos(pos);		// �ʒu��ݒ肷��
	SetScale(scale);	// �g�嗦��ݒ肷��
}

//=======================================
// �ǎC�菈��
//=======================================
void CPlayer::WallFriction(bool &bRight, bool &bLeft)
{
	if (bRight == true || bLeft == true)
	{ // �E�̕ǂ܂��́A���̕ǂɎC���Ă���ꍇ

		// �ǎC��J�E���g�����Z����
		m_nWallFricCount++;
	}
	else if(m_bJump == false)
	{ // �ǂɎC��Ă��Ȃ��ꍇ

		// �ǎC��J�E���g������������
		m_nWallFricCount = 0;
	}

	if (m_nWallFricCount >= PLAYER_WALLFRIC_COUNT)
	{ // �ǎC��J�E���g����萔�𒴂����ꍇ

		// �ǎC�蔻��� false �ɂ���
		bRight = false;
		bLeft = false;

		// �A�N�V������ݒ肷��
		m_pAction->ActionChange(CPlayerAct::ACT_NONE);
	}

	if (bRight == true || bLeft == true)
	{ // �E�̕ǂ܂��́A���̕ǂɎC���Ă���ꍇ

		if (m_pAction->GetAct() == CPlayerAct::ACT_WALLKICK)
		{ // �s���󋵂� �ǃL�b�N �̏ꍇ

			// �s���̐؂�ւ�����
			m_pAction->ActionChange(CPlayerAct::ACT_WALLAFTERGLOW);

			if (m_pMotion->GetType() != CPlayer::MOTIONTYPE_WALLFRIC)
			{ // �ǎC�胂�[�V�����ȊO�̏ꍇ

				// �ǎC�胂�[�V�����̐ݒ菈��
				m_pMotion->Set(CPlayer::MOTIONTYPE_WALLFRIC);
			}
		}
		else if (m_pAction->GetAct() != CPlayerAct::ACT_WALLFRIC && 
			m_pAction->GetAct() != CPlayerAct::ACT_CLIFFCATCH && 
			m_pAction->GetAct() != CPlayerAct::ACT_CLIFFFALL &&
			m_pAction->GetAct() != CPlayerAct::ACT_WALLAFTERGLOW)
		{ // ���̍s���󋵂���Ȃ��ꍇ

			// �s���̐؂�ւ�����
			m_pAction->ActionChange(CPlayerAct::ACT_WALLFRIC);

			if (m_pMotion->GetType() != CPlayer::MOTIONTYPE_WALLFRIC)
			{ // �ǎC�胂�[�V�����ȊO�̏ꍇ

				// �ǎC�胂�[�V�����̐ݒ菈��
				m_pMotion->Set(CPlayer::MOTIONTYPE_WALLFRIC);
			}
		}

		if (bRight == true)
		{ // �E�ǂɎC��Ă����ꍇ

			// �E�C���ݒ肷��
			m_pAction->SetRightLeave(true);
		}

		if (bLeft == true)
		{ // ���ǂɎC��Ă����ꍇ

			// ���C���ݒ肷��
			m_pAction->SetRightLeave(false);
		}
	}

	if (m_pAction->GetAct() == CPlayerAct::ACT_WALLFRIC &&
		bRight == false && 
		bLeft == false)
	{ // �ǂ𗣂����ꍇ

		// �s���̐؂�ւ�����
		m_pAction->ActionChange(CPlayerAct::ACT_WALLLEAVE);
	}
}

//=======================================
// �O�Ղ̔j������
//=======================================
void CPlayer::PlayerOrbit(void)
{
	if (m_pAction->GetAct() == CPlayerAct::ACT_BIGJUMP ||
		m_pAction->GetAct() == CPlayerAct::ACT_WALLKICK ||
		m_pAction->GetAct() == CPlayerAct::ACT_LAMPJUMP ||
		m_pAction->GetAct() == CPlayerAct::ACT_POLEDOWN)
	{ // ����A�N�V�������̏ꍇ

		// �O�Ղ̐e�̈ʒu�̐ݒ菈��
		m_pOrbit->SetParentData(GetPos(), GetRot());

		// �e�N�X�`���̊��蓖�ď���
		m_pOrbit->BindTexture(m_nOrbitIdx[ORBIT_TEX_NORMAL]);
	}
	else if (m_pAction->GetAct() == CPlayerAct::ACT_CLIFFCLIMB &&
		m_pAction->GetCliffCatch() == true)
	{ // �R�߂܂蒆���A�R�o��ɐ������Ă����ꍇ

		// �O�Ղ̐e�̈ʒu�̐ݒ菈��
		m_pOrbit->SetParentData(GetPos(), GetRot());

		// �e�N�X�`���̊��蓖�ď���
		m_pOrbit->BindTexture(m_nOrbitIdx[ORBIT_TEX_NORMAL]);
	}
	else if (m_pAction->GetAct() == CPlayerAct::ACT_HANGING)
	{ // �Ԃ牺�����Ԃ̏ꍇ

		if (m_bRight == true)
		{ // �E�������Ă���ꍇ

			// �O�Ղ̐e�̈ʒu�̐ݒ菈��
			m_pOrbit->SetParentData(GetPos(), -GetRot());
		}
		else
		{ // ���������Ă���ꍇ

			// �O�Ղ̐e�̈ʒu�̐ݒ菈��
			m_pOrbit->SetParentData(GetPos(), GetRot());
		}

		// �e�N�X�`���̊��蓖�ď���
		m_pOrbit->BindTexture(m_nOrbitIdx[ORBIT_TEX_POLE]);
	}
	else
	{ // ��L�ȊO

		//// �O�Ղ̈ʒu��S�ݒ肷��
		//m_pOrbit->PosSet(GetPos());
	}
}

//=======================================
// �������o�̏���
//=======================================
void CPlayer::PlayerSonic(void)
{
		if (m_fSpeed >= PLAYER_MAX_SPEED)
	{ // �ō����̏ꍇ

		if (m_pSonic == nullptr)
		{ // �O�Ղ��Ȃ��ꍇ

			// �O�Ղ𐶐�����
			m_pSonic = CSonic::Create();
		}
	}
	else
	{ // ��L�ȊO

		if (m_pSonic != nullptr)
		{ // �O�Ղ����݂���ꍇ

			// �O�Ղ̏I������
			m_pSonic->Uninit();

			// �O�Ղ� NULL �ɂ���
			m_pSonic = nullptr;
		}
	}
}

//=======================================
// �ǃL�b�N����
//=======================================
void CPlayer::WallKick(bool bRight, bool bLeft)
{
	// �W�����v�̂݃o�[�W����
	if (WallKickJump(bRight, bLeft) == false)
	{ // �ǃL�b�N���肪�ʂ�Ȃ������ꍇ

		// �����L�[�o�[�W����
		WallKickDirect();
	}
}

//=======================================
// �ǃL�b�N(�W�����v�̂ݔ���)
//=======================================
bool CPlayer::WallKickJump(bool bRight, bool bLeft)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// ����

	if (m_fStamina >= PLAYER_WALLKICK_STAMINA && 
		(CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true))
	{ // SPACE�L�[�����������A�X�^�~�i���]���Ă����ꍇ

		if (bRight == true)
		{ // �E�ǂ��痣�ꂽ�ꍇ

			// �E�ǂ��R����
			m_pAction->SetRightKick(true);

			// �Ǔ��ݍ��ݏ�Ԃɂ���
			m_pAction->ActionChange(CPlayerAct::ACT_WALLSTEPIN);

			// �Ǔ��ݍ��݃��[�V������ݒ肷��
			m_pMotion->Set(MOTIONTYPE_WALLSTEPIN);

			// �ǃL�b�N����
			return true;
		}

		if (bLeft == true)
		{ // ���ǂ��痣�ꂽ�ꍇ

			// ���ǂ��R����
			m_pAction->SetRightKick(false);

			// �Ǔ��ݍ��ݏ�Ԃɂ���
			m_pAction->ActionChange(CPlayerAct::ACT_WALLSTEPIN);

			// �Ǔ��ݍ��݃��[�V������ݒ肷��
			m_pMotion->Set(MOTIONTYPE_WALLSTEPIN);

			// �ǃL�b�N����
			return true;
		}
	}

	// �����̐ݒ菈��
	SetRot(rot);

	// �ǃL�b�N���s
	return false;
}

//=======================================
// �ǃL�b�N(�����L�[�t��)
//=======================================
void CPlayer::WallKickDirect(void)
{
	if (m_fStamina >= PLAYER_WALLKICK_STAMINA && 
		(CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true) &&
		m_pAction->GetAct() == CPlayerAct::ACT_WALLLEAVE)
	{ // SPACE�L�[�����������A�X�^�~�i���]���Ă����ꍇ

		if (m_bRight == true && 
			m_pAction->GetRightLeave() == true &&
			CManager::GetInputKeyboard()->GetPress(DIK_D) == true || 
			CManager::GetInputGamePad()->GetGameStickLXPress(0) > 0)
		{ // �E�ǂ��痣�ꂽ�ꍇ

			// �E�ǂ��R����
			m_pAction->SetRightKick(true);

			// �Ǔ��ݍ��ݏ�Ԃɂ���
			m_pAction->ActionChange(CPlayerAct::ACT_WALLSTEPIN);

			// �Ǔ��ݍ��݃��[�V������ݒ肷��
			m_pMotion->Set(MOTIONTYPE_WALLSTEPIN);
		}
		else if(m_bRight == false && 
			m_pAction->GetRightLeave() == false &&
			CManager::GetInputKeyboard()->GetPress(DIK_A) == true ||
			CManager::GetInputGamePad()->GetGameStickLXPress(0) < 0)
		{ // ���ǂ��痣�ꂽ�ꍇ

			// ���ǂ��R����
			m_pAction->SetRightKick(false);

			// �Ǔ��ݍ��ݏ�Ԃɂ���
			m_pAction->ActionChange(CPlayerAct::ACT_WALLSTEPIN);

			// �Ǔ��ݍ��݃��[�V������ݒ肷��
			m_pMotion->Set(MOTIONTYPE_WALLSTEPIN);
		}
	}
}

//=======================================
// �X�e�[�W�̓����蔻��
//=======================================
void CPlayer::StageCollision(bool* pRight, bool* pLeft)
{
	// ���[�J���ϐ��錾
	bool bJump = true;			// �ꎞ�I�ȃW�����v��

	// �W���Ƃ̓����蔻��
	if (SignPoleCollision() == true)
	{ // �W���Ƃ̓����蔻�肪 true �������ꍇ

		if (bJump == true)
		{ // �W�����v�󋵂� true �̏ꍇ

			// �W�����v�󋵂� false �ɂ���
			bJump = false;

			// �W�����v�󋵂� false �ɂ���
			m_bJump = false;
		}
	}

	// ��Q���̓����蔻��
	if (ObstacleCollision() == true)
	{ // ��Q���Ƃ̓����蔻�肪 true �������ꍇ

		if (bJump == true)
		{ // �W�����v�󋵂� true �̏ꍇ

			// �W�����v�󋵂� false �ɂ���
			bJump = false;

			// �W�����v�󋵂� false �ɂ���
			m_bJump = false;
		}
	}

	// �����蔻��|���S���̓����蔻��
	if (CollPolygonCollision() == true)
	{ // �����蔻��|���S���Ƃ̓����蔻�肪 true �������ꍇ

		if (bJump == true)
		{ // �W�����v�󋵂� true �̏ꍇ

			// �W�����v�󋵂� false �ɂ���
			bJump = false;

			// �W�����v�󋵂� false �ɂ���
			m_bJump = false;
		}
	}

	// �u���b�N�̓����蔻��
	if (BlockCollision(pRight, pLeft) == true)
	{ // �u���b�N�̓����蔻�肪 true �������ꍇ

		if (bJump == true)
		{ // �W�����v�󋵂� true �̏ꍇ

			// �W�����v�󋵂� false �ɂ���
			bJump = false;

			// �W�����v�󋵂� false �ɂ���
			m_bJump = false;
		}
	}

	// �X�e�[�W�̂߂荞�ݏ���
	StagePenetration();

	// �W�����v�󋵂�ݒ肷��
	m_bJump = bJump;

	if (m_bJump == true)
	{ // �W�����v�󋵂� true �̏ꍇ

		// �ʒu�̐ݒ菈��(Y������)
		CShadowCircle::SetPosRotXZ(m_nShadowIdx, GetPos(), GetRot());
	}
	else
	{ // ��L�ȊO

		// �e�̈ʒu�����ݒ菈��
		CShadowCircle::SetPosRot(m_nShadowIdx, GetPos(), GetRot());
	}

	if (m_state != STATE_INVINCIBLE &&
		m_state != STATE_CRUSH)
	{ // ���G��ԈȊO���A������ԈȊO�̏ꍇ

		if (m_crush.bTop == true &&
			m_crush.bBottom == true)
		{ // �㉺���狲�܂ꂽ�ꍇ

			// ���E��Ԃɐݒ肷��
			SetState(STATE_CRUSH);

			// �ڕW�̊g�嗦��ݒ肷��
			m_crush.scaleDest = D3DXVECTOR3(1.0f, PLAYER_CRUSH_WIDTH, 1.0f);

			// ���x�𔼕��ɂ���
			SetSpeed(PLAYER_MIN_SPEED + ((GetSpeed() - PLAYER_MIN_SPEED) * 0.5f));
		}
		else if (m_crush.bLeft == true && 
			m_crush.bRight == true)
		{ // ���E���狲�܂ꂽ�ꍇ

			// ���E��Ԃɐݒ肷��
			SetState(STATE_CRUSH);

			// �ڕW�̊g�嗦��ݒ肷��
			m_crush.scaleDest = D3DXVECTOR3(1.0f, 1.0f, PLAYER_CRUSH_WIDTH);

			// ���x�𔼕��ɂ���
			SetSpeed(PLAYER_MIN_SPEED + ((GetSpeed() - PLAYER_MIN_SPEED) * 0.5f));
		}
	}

	// ���E�����S�� false �ɂ���
	m_crush.bTop = false;			// ��
	m_crush.bBottom = false;		// ��
	m_crush.bRight = false;			// �E
	m_crush.bLeft = false;			// ��
}

//=======================================
// ��Q���̓����蔻�菈��
//=======================================
bool CPlayer::SignPoleCollision(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;			// �I�u�W�F�N�g�̃|�C���^
	CObstacle* pObstacle = nullptr;		// ��Q���̃|�C���^
	CSignPole* pSignPole = nullptr;		// �W���̃|�C���^
	float fHeight = 0.0f;				// ����
	D3DXVECTOR3 pos = GetPos();			// �ʒu�̎擾����
	bool bLand = false;					// ���n��

	// �I�u�W�F�N�g�̏����擾����
	pObj = CObject::GetTop(PRIORITY_BLOCK);

	while(pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == TYPE_OBSTACLE)
		{ // ��Q���������ꍇ

			// ��Q���̏����擾����
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // ��Q���̏�� NULL ����Ȃ��ꍇ

				if (pObstacle->GetType() == CObstacle::TYPE_SIGNPOLE)
				{ // ��ނ��W���������ꍇ

					// �W���̏����擾����
					pSignPole = dynamic_cast<CSignPole*>(pObstacle);

					if (pSignPole != nullptr)
					{ // �W���̏��� NULL ����Ȃ������ꍇ

						// �����蔻��
						fHeight = pSignPole->ElevationCollision(pos);

						if (fHeight + PLAYER_SIGNPOLE_WALK >= pos.y &&
							pSignPole->GetPos().y - PLAYER_SIGNPOLE_WALK <= pos.y &&
							pSignPole->GetVtx(0).x >= pos.x &&
							pSignPole->GetVtx(2).x <= pos.x &&
							pSignPole->GetState() != CSignPole::STATE_NONE)
						{ // ���݂̍��������Ⴂ���A�W���͈͓̔����A�ʏ��ԈȊO�̏ꍇ

							// �ʒu��ݒ肷��
							pos.y = fHeight + PLAYER_SIGNPOLE_WALK;

							// �ړ��ʂ�����������
							m_move.y = 0.0f;

							// �s����ԕω�����
							m_pAction->ActionChange(CPlayerAct::ACT_NONE);

							if(m_bMove == false &&
								m_pMotion->GetType() != MOTIONTYPE_NEUTRAL)
							{ // �ʏ탂�[�V�������ݒ肳��Ă��Ȃ������ꍇ

								// �ʏ탂�[�V������ݒ肷��
								m_pMotion->Set(MOTIONTYPE_NEUTRAL);
							}
							else if (m_bMove == true &&
								m_pMotion->GetType() != MOTIONTYPE_MOVE)
							{ // �ړ����[�V�������ݒ肳��Ă��Ȃ������ꍇ

								// �ړ����[�V������ݒ肷��
								m_pMotion->Set(MOTIONTYPE_MOVE);
							}

							// ���n�󋵂� true �ɂ���
							bLand = true;
						}
					}
				}
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}

	// �ʒu�̐ݒ菈��
	SetPos(pos);

	// ���n�󋵂�Ԃ�
	return bLand;
}

//=======================================
// �n�ʒ��n����
//=======================================
void CPlayer::GroundLanding(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();			// �ʒu���擾����

	if (pos.y <= 0.0f)
	{ // �n�ʂɒB�����ꍇ

		if (m_bJump == true)
		{ // �W�����v�� true �̏ꍇ

			// �W�����v�󋵂� false �ɂ���
			m_bJump = false;

			// ���n����炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
		}

		// �d�͂����Z�b�g����
		m_move.y = 0.0f;

		// �ʒu��ݒ肷��
		pos.y = 0.0f;

		if (m_pAction->GetAct() == CPlayerAct::ACT_POLEDOWN)
		{ // �|�[�����蒆�̏ꍇ

			// �ʒu��ݒ肷��
			pos.x = m_pAction->GetPosObject().x;
			pos.z = 0.0f;
		}

		// �s����ԕω�����
		m_pAction->ActionChange(CPlayerAct::ACT_NONE);

		if (m_bMove == true)
		{ // �ړ����Ă���ꍇ

			if (m_pMotion->GetType() != MOTIONTYPE_MOVE)
			{ // �ړ����[�V�����ȊO�̏ꍇ

				// �ړ����[�V�����ɐݒ肷��
				m_pMotion->Set(MOTIONTYPE_MOVE);
			}
		}
		else
		{ // �ړ����Ă��Ȃ��ꍇ

			if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL)
			{ // �ʏ탂�[�V�����ȊO�̏ꍇ

				// �ʏ탂�[�V�����ɐݒ肷��
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}
	}

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}

//=======================================
// �X�^�~�i�̉��Z����
//=======================================
void CPlayer::CalcStamina(float fAdd)
{
	// �X�^�~�i�����Z����
	m_fStamina += fAdd;

	if (m_fStamina <= 0.0f)
	{ // �X�^�~�i�̍ŏ����𒴂����ꍇ

		// �X�^�~�i�� 0.0f �ɂ���
		m_fStamina = 0.0f;
	}

	if (m_fStamina >= PLAYER_MAX_STAMINA)
	{ // �X�^�~�i�̍ő吔�𒴂����ꍇ

		// �X�^�~�i�� MAX �ɂ���
		m_fStamina = PLAYER_MAX_STAMINA;
	}
}

//=======================================
// �X���̐ݒ菈��
//=======================================
void CPlayer::SetStreetLamp(CStreetLamp* pTarget)
{
	// �X���̏���ݒ肷��
	m_pStreetLamp = pTarget;
}

//=======================================
// �X���̎擾����
//=======================================
CStreetLamp* CPlayer::GetStreetLamp(void) const
{
	// �X���̏���Ԃ�
	return m_pStreetLamp;
}

//=======================================
// ���xUI�̎擾����
//=======================================
CSpeedUI* CPlayer::GetSpeedUI(void)
{
	// ���xUI��Ԃ�
	return m_pSpeedUI;
}

//======================================
// �X�^�~�iUI�̎擾����
//======================================
CStaminaUI* CPlayer::GetStaminaUI(void)
{
	// �X�^�~�iUI�̏���Ԃ�
	return m_pStaminaUI;
}

//=======================================
// �X�^�~�i�̎擾����
//=======================================
float CPlayer::GetStamina(void)
{
	// �X�^�~�i��Ԃ�
	return m_fStamina;
}

//=======================================
// �u���b�N�̓����蔻��
//=======================================
bool CPlayer::BlockCollision(bool* pRight, bool* pLeft)
{
	if (collision::PlayerBlockCollision(pRight, pLeft, this, PLAYER_WIDTH, PLAYER_HEIGHT) == true)
	{ // �����蔻�肪 true �̏ꍇ

		// �d�͂�0.0f�ɂ���
		m_move.y = 0.0f;

		if (m_bMove == true)
		{ // �ړ����Ă���ꍇ

			if (m_pAction->GetAct() != CPlayerAct::ACT_CLIFFLAND)
			{ // �R�o���ԈȊO�̏ꍇ

				if (m_pMotion->GetType() != MOTIONTYPE_MOVE &&
					m_pMotion->GetType() != MOTIONTYPE_LANDING &&
					m_pMotion->GetType() != MOTIONTYPE_ATTACK)
				{ // �ړ����[�V�����A���n���[�V�����A�U�����[�V�����ȊO�̏ꍇ

					// �ړ����[�V�����ɐݒ肷��
					m_pMotion->Set(MOTIONTYPE_MOVE);
				}
			}
		}
		else
		{ // �ړ����Ă��Ȃ��ꍇ

			if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // �ʏ탂�[�V�����A���n���[�V�����A�U�����[�V�����ȊO�̏ꍇ

				// �ʏ탂�[�V�����ɐݒ肷��
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}

		// true ��Ԃ�
		return true;
	}
	else
	{ // ��L�ȊO

		// false ��Ԃ�
		return false;
	}
}

//=======================================
// ��Q���̓����蔻��
//=======================================
bool CPlayer::ObstacleCollision(void)
{
	if (collision::PlayerObstacleCollision(this, PLAYER_WIDTH, PLAYER_HEIGHT) == true)
	{ // �����蔻�肪 true �̏ꍇ

		// �d�͂�0.0f�ɂ���
		m_move.y = 0.0f;

		if (m_bMove == true)
		{ // �ړ����Ă���ꍇ

			if (m_pMotion->GetType() != MOTIONTYPE_MOVE &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // �ړ����[�V�����A���n���[�V�����A�U�����[�V�����ȊO�̏ꍇ

				// �ړ����[�V�����ɐݒ肷��
				m_pMotion->Set(MOTIONTYPE_MOVE);
			}
		}
		else
		{ // �ړ����Ă��Ȃ��ꍇ

			if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // �ʏ탂�[�V�����A���n���[�V�����A�U�����[�V�����ȊO�̏ꍇ

				// �ʏ탂�[�V�����ɐݒ肷��
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}

		// true ��Ԃ�
		return true;
	}
	else
	{ // ��L�ȊO

		// false ��Ԃ�
		return false;
	}
}

//=======================================
// �����蔻��|���S���̓����蔻��
//=======================================
bool CPlayer::CollPolygonCollision(void)
{
	if (collision::PlayerCollPolygonCollision(this, PLAYER_WIDTH, PLAYER_HEIGHT) == true)
	{ // �����蔻�肪 true �̏ꍇ

		// �d�͂�0.0f�ɂ���
		m_move.y = 0.0f;

		if (m_bMove == true)
		{ // �ړ����Ă���ꍇ

			if (m_pMotion->GetType() != MOTIONTYPE_MOVE &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // �ړ����[�V�����A���n���[�V�����A�U�����[�V�����ȊO�̏ꍇ

				// �ړ����[�V�����ɐݒ肷��
				m_pMotion->Set(MOTIONTYPE_MOVE);
			}
		}
		else
		{ // �ړ����Ă��Ȃ��ꍇ

			if (m_pMotion->GetType() != MOTIONTYPE_NEUTRAL &&
				m_pMotion->GetType() != MOTIONTYPE_LANDING &&
				m_pMotion->GetType() != MOTIONTYPE_ATTACK)
			{ // �ʏ탂�[�V�����A���n���[�V�����A�U�����[�V�����ȊO�̏ꍇ

				// �ʏ탂�[�V�����ɐݒ肷��
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}

		// true ��Ԃ�
		return true;
	}
	else
	{ // ��L�ȊO

		// false ��Ԃ�
		return false;
	}
}

//=======================================
// �߂荞�ݔ��菈��
//=======================================
void CPlayer::StagePenetration(void)
{
	// �u���b�N�Ƃ̂߂荞�ݏ���
	collision::PlayerBlockPenetration(this, PLAYER_WIDTH, PLAYER_HEIGHT);

	// ��Q���Ƃ̂߂荞�ݏ���
	collision::PlayerObstaclePenetration(this, PLAYER_WIDTH, PLAYER_HEIGHT);

	// �����蔻��|���S���Ƃ̂߂荞�ݏ���
	collision::PlayerCollPolygonPenetration(this, PLAYER_WIDTH, PLAYER_HEIGHT);
}