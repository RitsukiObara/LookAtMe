//============================================
//
// �v���C���[�w�b�_�[[player.h]
// Author�F��������
//
//============================================
#ifndef _PLAYER_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "character.h"
#include "scene.h"
#include "playerAct.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CPlayerAct;			// �v���C���[�̏��
class CMotion;				// ���[�V����
class COrbit;				// �O��
class CStreetLamp;			// �X��(����)
class CSonic;				// �������o
class CSpeedUI;				// ���xUI
class CStaminaUI;			// �X�^�~�iUI

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define PLAYER_CLIFF_ADD_SPEED		(1.0f)								// �R�o�莞�̑��x�̉��Z��
#define PLAYER_CLIFF_SUB_SPEED		(0.4f)								// �R�o�莞�̑��x�̌��Z��
#define PLAYER_WALLKICK_ADD_SPEED	(0.05f)								// �ǃL�b�N���̑��x�̉��Z��
#define PLAYER_BIGJUMP_ADD_SPEED	(0.1f)								// ��W�����v���̑��x�̉��Z��
#define PLAYER_ACTION_ADD_SPEED		(0.01f)								// �A�N�V�������̑��x�̉��Z��
#define PLAYER_LAMPJUMP_ADD_SPEED	(0.05f)								// �X���W�����v���̑��x�̉��Z��
#define PLAYER_WIDTH				(20.0f)								// �v���C���[�̉���
#define PLAYER_HEIGHT				(85.0f)								// �v���C���[�̏c��
#define PLAYER_COLLISION_WIDTH		(PLAYER_WIDTH + 0.01f)				// �v���C���[�̉���
#define PLAYER_COLLISION_HEIGHT		(PLAYER_HEIGHT + 0.01f)				// �v���C���[�̏c��
#define PLAYER_RADIUS				(30.0f)								// �v���C���[�̔��a
#define PLAYER_MAX_STAMINA			(100.0f)							// �v���C���[�̃X�^�~�i�̏����l
#define PLAYER_INIT_SPEED			(7.0f)								// �v���C���[�̏����ړ���
#define PLAYER_MAX_SPEED			(13.0f)								// �v���C���[�̈ړ��ʂ̍ő�l
#define PLAYER_MIN_SPEED			(PLAYER_INIT_SPEED)					// �v���C���[�̈ړ��ʂ̍ŏ��l
#define PLAYER_GRAVITY				(0.55f)								// �v���C���[�̏d��
#define PLAYER_WALLKICK_GRAVITY		(0.6f)								// �v���C���[�̕ǃL�b�N���̏d��
#define PLAYER_LAND_STAGING_GRAVITY	(-19.0f)							// �v���C���[�̒��n���o�̏o��d��

#define PLAYER_JUMP					(13.5f)								// �v���C���[�̃W�����v��
#define PLAYER_BIGJUMP				(20.0f)								// �v���C���[�̒��W�����v��

// �{�^���J�E���g�֌W
#define PLAYER_BIGJUMP_BTNCOUNT		(15)		// ���W�����v�s���ɂȂ�܂ł̃J�E���g��
#define PLAYER_WALLKICK_BTNCOUNT	(1)			// �ǃL�b�N�s���ɂȂ�܂ł̃J�E���g��
#define PLAYER_ATTACK_BTNCOUNT		(1)			// �U���s���ɂȂ�܂ł̃J�E���g��
#define PLAYER_CLIFFLAND_BTNCOUNT	(1)			// �R���n�s���ɂȂ�܂ł̃J�E���g��
#define PLAYER_LAMPJUMP_BTNCOUNT	(1)			// �X���W�����v�ɂȂ�܂ł̃J�E���g��

// �f�o�b�O�p
#ifdef _DEBUG

// �X�^�~�i�֌W
#define PLAYER_BIGJUMP_STAMINA			(20.0f)		// 2�i�W�����v���Ɍ��炷�X�^�~�i
#define PLAYER_WALLKICK_STAMINA			(3.0f)		// �ǃL�b�N���Ɍ��炷�X�^�~�i
#define PLAYER_CLIFFLAND_STAMINA		(5.0f)		// �R���n���Ɍ��炷�X�^�~�i
#define PLAYER_AUTO_HEAL_STAMINA_STOP	(0.10f)		// �~�܂��Ă��鎞�̎����񕜃X�^�~�i��
#define PLAYER_AUTO_HEAL_STAMINA_MOVE	(0.03f)		// �����Ă��鎞�̎����񕜃X�^�~�i��

#else

// �X�^�~�i�֌W
#define PLAYER_BIGJUMP_STAMINA			(20.0f)		// 2�i�W�����v���Ɍ��炷�X�^�~�i
#define PLAYER_WALLKICK_STAMINA			(2.0f)		// �ǃL�b�N���Ɍ��炷�X�^�~�i
#define PLAYER_CLIFFLAND_STAMINA		(5.0f)		// �R���n���Ɍ��炷�X�^�~�i
#define PLAYER_AUTO_HEAL_STAMINA_STOP	(0.15f)		// �~�܂��Ă��鎞�̎����񕜃X�^�~�i��
#define PLAYER_AUTO_HEAL_STAMINA_MOVE	(0.05f)		// �����Ă��鎞�̎����񕜃X�^�~�i��

// �R�s�[
//#define PLAYER_BIGJUMP_STAMINA		(20.0f)		// 2�i�W�����v���Ɍ��炷�X�^�~�i
//#define PLAYER_WALLKICK_STAMINA		(2.0f)		// �ǃL�b�N���Ɍ��炷�X�^�~�i
//#define PLAYER_CLIFFLAND_STAMINA		(5.0f)		// �R���n���Ɍ��炷�X�^�~�i
//#define PLAYER_AUTO_HEAL_STAMINA_STOP	(0.15f)		// �~�܂��Ă��鎞�̎����񕜃X�^�~�i��
//#define PLAYER_AUTO_HEAL_STAMINA_MOVE	(0.05f)		// �����Ă��鎞�̎����񕜃X�^�~�i��

#endif

//--------------------------------------------
// �N���X(�v���C���[�N���X)
//--------------------------------------------
class CPlayer : public CCharacter
{
public:			// �N�ł��A�N�Z�X�ł���

	//************************************************************
	//	�񋓌^��`(���[�V�����̎��)
	//************************************************************
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,		// �ҋ@
		MOTIONTYPE_MOVE,			// �ړ�
		MOTIONTYPE_JUMP,			// �W�����v
		MOTIONTYPE_WALLFRIC,		// �ǎC����
		MOTIONTYPE_WALLSTEPIN,		// ���ݍ���
		MOTIONTYPE_WALLKICK,		// �ǃL�b�N
		MOTIONTYPE_BIGJUMP,			// ���W�����v
		MOTIONTYPE_CLIFFCATCH,		// �R�߂܂�
		MOTIONTYPE_CLIFFUP,			// �R�オ��
		MOTIONTYPE_HANGING,			// �Ԃ牺����
		MOTIONTYPE_LANDING,			// ���n���[�V����
		MOTIONTYPE_LAMPJUMP,		// �X���W�����v
		MOTIONTYPE_POLEDOWN,		// �|�[������
		MOTIONTYPE_FALL,			// ����
		MOTIONTYPE_START,			// �X�^�[�g�O
		MOTIONTYPE_ASCENT,			// �悶�o��
		MOTIONTYPE_ATTACK,			// �U��
		MOTIONTYPE_MAX				// ���̗񋓂̑���
	}MOTIONTYPE;

	//--------------------------------------------
	// ���
	//--------------------------------------------
	typedef enum
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_BOOST,		// �u�[�X�g���
		STATE_DAMAGE,		// �_���[�W���
		STATE_INVINCIBLE,	// ���G���
		STATE_CRUSH,		// ���E���
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	//--------------------------------------------
	// �O�Ղ̃e�N�X�`��
	//--------------------------------------------
	typedef enum
	{
		ORBIT_TEX_NORMAL = 0,		// �ʏ�̋O��
		ORBIT_TEX_POLE,				// �|�[���̋O��
		ORBIT_TEX_MAX				// ���̗񋓌^�̑���
	}ORBIT_TEX;

	//--------------------------------------------
	// �\���̒�`(�����̏�)
	//--------------------------------------------
	typedef struct
	{
		D3DXVECTOR3 scaleDest;		// �ڕW�̊g�嗦
		bool bTop;					// �ォ��̔���
		bool bBottom;				// ������̔���
		bool bRight;				// �E����̔���
		bool bLeft;					// ������̔���
	}SCrush;

	//--------------------------------------------
	// �\���̒�`(�A�N�V�����{�^���֌W)
	//--------------------------------------------
	typedef struct
	{
		int nButtonCount;			// �{�^���������Ă��鎞��
		bool bSpacePush;			// �W�����v�L�[�̉�����
	}SButton;

	CPlayer();				// �R���X�g���N�^
	CPlayer(CObject::TYPE type, PRIORITY priority = PRIORITY_PLAYER);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CPlayer();		// �f�X�g���N�^
	void Box(void);			// �R���X�g���N�^�̔�

	// �����o�֐�
	virtual HRESULT Init(void);		// ����������
	virtual void Uninit(void);		// �I������
	virtual void Update(void) = 0;	// �X�V����
	virtual void Draw(void);		// �`�揈��
	void Hit(void);					// �q�b�g����

	virtual void SetData(const D3DXVECTOR3& pos, const CScene::MODE mode);		// ���̐ݒ菈��

	void StartUpdate(void);		// �X�^�[�g�X�V����
	void CountDownUpdate(void);	// �J�E���g�_�E���X�V����
	void MainUpdate(void);		// �ʏ�X�V����
	void GoalUpdate(void);		// �S�[���X�V����
	void FinishUpdate(void);	// �I���X�V����

	// �Z�b�g�E�Q�b�g�֌W
	void SetColor(const D3DXCOLOR& col);		// �F�̐ݒ菈��
	D3DXCOLOR GetColor(void) const;				// �F�̎擾����

	void SetMove(const D3DXVECTOR3& move);		// �ړ��ʂ̐ݒ菈��
	D3DXVECTOR3 GetMove(void) const;			// �ړ��ʂ̎擾����

	void SetState(const STATE state);			// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;					// ��Ԃ̎擾����

	void SetJump(const bool bJump);				// �W�����v�󋵂̐ݒ菈��
	bool GetJump(void) const;					// �W�����v�󋵂̎擾����

	void SetSpeed(float fSpeed);				// ���x�̐ݒ菈��
	float GetSpeed(void) const;					// ���x�̎擾����

	void SetButtonPush(const bool bPush);		// �{�^�������󋵂̐ݒ菈��
	void SetButtonCount(const int nCount);		// �{�^���J�E���g�̐ݒ菈��
	SButton GetButton(void);					// �{�^���̎擾����

	void SetRight(const bool bRight);			// �E�����󋵂̐ݒ菈��
	bool GetRight(void) const;					// �E�����󋵂̎擾����

	void SetWallFricCount(const int nCount);	// �ǎC��J�E���g�̐ݒ菈��

	void SetCrushTop(const bool bTop);			// ���E����(��)�̐ݒ菈��
	void SetCrushBottom(const bool bBottom);	// ���E����(��)�̐ݒ菈��
	void SetCrushRight(const bool bRight);		// ���E����(�E)�̐ݒ菈��
	void SetCrushLeft(const bool bLeft);		// ���E����(��)�̐ݒ菈��

	CMotion* GetMotion(void);					// ���[�V�����̏��̐ݒ菈��

	CPlayerAct* GetAction(void);				// �s���̏��̎擾����

	COrbit* GetOrbit(void);						// �O�Ղ̏��̎擾����

	float GetStamina(void);						// �X�^�~�i�̎擾����
	void CalcStamina(float fAdd);				// �X�^�~�i�̌v�Z����

	void SetStreetLamp(CStreetLamp* pTarget);	// �X���̐ݒ菈��
	CStreetLamp* GetStreetLamp(void) const;		// �X���̎擾����

	CSpeedUI* GetSpeedUI(void);					// ���xUI�̎擾����

	CStaminaUI* GetStaminaUI(void);				// �X�^�~�iUI�̎擾����

	// �ÓI�����o�֐�
	static CPlayer* Create(const D3DXVECTOR3& pos, const CScene::MODE mode);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Control(void);			// ���쏈��
	void PlayerGravity(void);	// �d�͏���
	void ActionControl(void);	// �A�N�V�����{�^���̑��쏈��
	void Move(void);			// �ړ�����
	void MoveProcess(void);		// �ړ��̋��ʂ̏���
	void SpeedStarEffect(void);	// ���x�̐��̔�������
	void RunDustEffect(void);	// ��������̚��̔�������
	void Jump(void);			// �W�����v����
	void State(void);			// ��ԏ���
	void WallKick(bool bRight, bool bLeft);	// �ǃL�b�N����
	void StageCollision(bool* pRight, bool* pLeft);		// �X�e�[�W�̓����蔻��
	void StagePenetration(void);			// �߂荞�ݔ��菈��
	bool SignPoleCollision(void);			// �W���̓����蔻�菈��
	void GroundLanding(void);				// �n�ʒ��n����
	void WallFriction(bool &bRight, bool &bLeft);		// �ǎC�菈��
	void PlayerOrbit(void);					// �O�Ղ̏���
	void PlayerSonic(void);					// �������o�̏���

	bool BlockCollision(bool* pRight, bool* pLeft);		// �u���b�N�̓����蔻��
	bool ObstacleCollision(void);						// ��Q���̓����蔻��
	bool CollPolygonCollision(void);					// �����蔻��|���S���̓����蔻��

	bool WallKickJump(bool bRight, bool bLeft);	// �ǃL�b�N(�W�����v�̂ݔ���)
	void WallKickDirect(void);					// �ǃL�b�N(�����L�[�t��)

	// ��ԏ����֌W
	void StateNone(void);					// �ʏ��ԏ���
	void StateBoost(void);					// �u�[�X�g���
	void StateDamage(void);					// �_���[�W��ԏ���
	void StateInvincible(void);				// ���G��ԏ���
	void StateCrush(void);					// ���E��ԏ���

	// �����o�ϐ�
	CMotion* m_pMotion;				// ���[�V�����̏��
	CPlayerAct* m_pAction;			// �s���̏��
	COrbit* m_pOrbit;				// �O�Ղ̏��
	CStreetLamp* m_pStreetLamp;		// �X��(����)�̏��
	CSonic* m_pSonic;				// �������o�̏��
	CSpeedUI* m_pSpeedUI;			// ���xUI�̏��
	CStaminaUI* m_pStaminaUI;		// �X�^�~�i�̏��

	SCrush m_crush;					// ���E����
	D3DXCOLOR m_col;				// �F
	D3DXVECTOR3 m_move;				// �ړ���
	STATE m_state;					// ���
	int m_nStateCount;				// ��ԃJ�E���g
	int m_nShadowIdx;				// �e�̃C���f�b�N�X
	int m_nWallFricCount;			// �ǎC��J�E���g
	int m_nJumpCount;				// �W�����v���Ă���J�E���g
	int m_nOrbitIdx[ORBIT_TEX_MAX];	// �O�Ղ̃e�N�X�`���̃C���f�b�N�X
	int m_nMoveCount;				// �_�b�V���J�E���g
	float m_fSpeed;					// ���鑬�x
	float m_fStamina;				// �X�^�~�i
	float m_fMoveDest;				// �ړI�̈ړ���
	SButton m_button;				// �{�^���̏��
	bool m_bJump;					// �W�����v��
	bool m_bRight;					// �E�������ǂ���
	bool m_bMove;					// �ړ���
};

#endif