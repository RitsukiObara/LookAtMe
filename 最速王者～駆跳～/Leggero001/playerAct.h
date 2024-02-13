//============================================
//
// �v���C���[�̍s���Ǘ��w�b�_�[[playerAct.h]
// Author�F��������
//
//============================================
#ifndef _PLAYERACT_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _PLAYERACT_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//--------------------------------------------
// �}�N����`
//--------------------------------------------
// �J�E���g�֌W
#define PLAYER_CLIFFCLIMB_SUCCESS_COUNT		(60)		// �R�͂܂��Ԃł̐����̃J�E���g��
#define PLAYER_CLIFFCLIMB_FALL_COUNT		(120)		// �R�͂܂��Ԃł̎��������̃J�E���g��
#define PLAYER_INVINCIBLE_COUNT				(100)		// ���G��Ԃ̃J�E���g��
#define PLAYER_DAMAGE_COUNT					(10)		// �_���[�W��Ԃ̃J�E���g��
#define PLAYER_WALLKICK_COUNT				(45)		// �ǃL�b�N��Ԃ̃J�E���g��
#define PLAYER_WALLAFTERGLOW_COUNT			(20)		// �ǃL�b�N�̗]�C��Ԃ̃J�E���g��
#define PLAYER_WALLLEAVE_COUNT				(6)			// �Ǘ�����Ԃ̃J�E���g��
#define PLAYER_CLIFFLAND_COUNT				(12)		// �R�������n�̃J�E���g��
#define PLAYER_CLIFFAFTERGLOW_COUNT			(4)			// �]�C��Ԃ̃J�E���g��
#define PLAYER_WALLFRIC_COUNT				(80)		// �ǎC���Ԃ��������������J�E���g��
#define PLAYER_CLIFFFALL_COUNT				(30)		// �R�~���Ԃ̃J�E���g��
#define PLAYER_POLEJUMP_COUNT				(3)			// �|�[������W�����v���Ă����Ԃ̃J�E���g��
#define PLAYER_WALLFRIC_DUST_COUNT			(4)			// �ǎC�莞�̚����o��J�E���g��
#define PLAYER_CLIFFCLIMB_AURA_COUNT		(25)		// �R�オ�莞�̃I�[�����o�Ă���J�E���g
#define PLAYER_CLIFFLAND_AURA_COUNT			(8)			// �R���n���̃I�[�����o�Ă���J�E���g
#define PLAYER_FAST_LAMPJUMP_COUNT			(13)		// �ő��X���W�����v���o��J�E���g��

// �X�R�A�֌W
#define PLAYER_SUPERJUMP_SCORE			(300)		// �v���C���[�̒��W�����v�̃X�R�A
#define PLAYER_CLIFFLAND_SCORE			(500)		// �v���C���[�̊R�������n�̃X�R�A
#define PLAYER_WALLKICK_SCORE			(100)		// �v���C���[�̕ǃL�b�N�̃X�R�A
#define PLAYER_LAMPJUMP_SCORE			(200)		// �v���C���[�̊X���W�����v�̃X�R�A
#define PLAYER_OBSTACLE_SCORE			(50)		// �v���C���[�̏�Q���ւ̃A�N�V�����̃X�R�A

#define POLE_PLAYER_DIST				(35.0f)		// �Ԃ牺�����Ă��鎞�̃|�[���ƃv���C���[�Ƃ̋���
#define AURA_RAND_SIZE					(75.0f)		// �I�[�����o�����̃����_���̃T�C�Y
#define AURA_NORMAL_LIFE				(20)		// �I�[���̒ʏ�̎���
#define AURA_CLIFF_LIFE					(60)		// �I�[���̊R�͂܂莞�̎���

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CPlayer;			// �v���C���[

//--------------------------------------------
// �N���X(�v���C���[�̍s���N���X)
//--------------------------------------------
class CPlayerAct
{
public:			// �N�ł��A�N�Z�X�ł���

	//--------------------------------------------
	// �s��
	//--------------------------------------------
	typedef enum
	{
		ACT_NONE = 0,			// ����
		ACT_BIGJUMP,			// ���W�����v
		ACT_WALLFRIC,			// �ǎC����
		ACT_WALLLEAVE,			// �Ǘ�����
		ACT_WALLSTEPIN,			// �Ǔ��ݍ��ݏ���
		ACT_WALLKICK,			// �ǃL�b�N
		ACT_WALLAFTERGLOW,		// �ǃL�b�N�̗]�C
		ACT_CLIFFCATCH,			// �R���܂�
		ACT_CLIFFCLIMB,			// �R�o��
		ACT_CLIFFFALL,			// �R�~��
		ACT_CLIFFLAND,			// �R�������n
		ACT_CLIFFAFTERGLOW,		// �R���n�̗]�C���
		ACT_HANGING,			// �Ԃ牺������
		ACT_LAMPJUMP,			// �����v������ł�����
		ACT_POLEDOWN,			// �|�[��������
		ACT_MAX					// ���̗񋓌^�̑���
	}ACT;

	CPlayerAct();				// �R���X�g���N�^
	~CPlayerAct();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������

	void Action(CPlayer& player);			// �s����ԏ���

	// �ÓI�����o�֐�
	static CPlayerAct* Create(void);		// ��������

	// �Z�b�g�Q�b�g�֌W
	void SetAct(const ACT act);				// �s����Ԃ̐ݒ菈��
	ACT GetAct(void) const;					// �s����Ԃ̎擾����

	void SetWallOld(const bool bWall);		// �O��̕ǎC��󋵂̐ݒ菈��
	bool GetWallOld(void) const;			// �O��̕ǎC��󋵂̎擾����

	void SetRightLeave(const bool bRightLeave);		// �Ǘ��ꎞ�̌����ݒ菈��
	bool GetRightLeave(void) const;					// �Ǘ��ꎞ�̌����擾����

	void SetRightKick(const bool bRight);			// �E�ǂ��R�������̐ݒ菈��

	void SetPosObject(const D3DXVECTOR3& pos);		// �֗^���Ă���I�u�W�F�N�g�̈ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPosObject(void) const;			// �֗^���Ă���I�u�W�F�N�g�̈ʒu�̎擾����

	float GetCliffGravity(void) const;		// �R�߂܂莞�̏d�͂̎擾����

	void SetRotDest(const float fRot);		// �����̈ړ��ʂ̐ݒ菈��

	void ActionChange(const ACT action);	// �A�N�V�����ω�����

	bool GetCliffCatch(void) const;			// �R�o�萬������̎擾����

	int GetActCount(void) const;			// �A�N�V�����J�E���g�̎擾����

private:

	// �����o�֐�
	void ActNone(CPlayer& player);				// �ʏ�s������
	void ActSuperJump(CPlayer& player);			// ���W�����v�s������
	void ActWallFriction(CPlayer& player);		// �ǎC��s������
	void ActWallLeave(CPlayer& player);			// �Ǘ���s������
	void ActWallStepIn(CPlayer& player);		// �Ǔ��ݍ��ݏ���
	void ActWallKick(CPlayer& player);			// �ǃL�b�N�s������
	void ActWallAfterGlaw(CPlayer& player);		// �ǃL�b�N�̗]�C�s������
	void ActCliffCatch(CPlayer& player);		// �R�߂܂�s������
	void ActCliffClimb(CPlayer& player);		// �R�~��s������
	void ActCliffFall(CPlayer& player);			// �R�o��s������
	void ActCliffLand(CPlayer& player);			// �R�o�蒅�n�s������
	void ActCliffAfterGlaw(CPlayer& player);	// �R���n�̗]�C�s������
	void ActHanging(CPlayer& player);			// �Ԃ牺����s������
	void ActLampJump(CPlayer& player);			// �����v������ł����ԏ���
	void ActPoleDown(CPlayer& player);			// �|�[�������ԏ���

	void SuperJumpEffect(CPlayer& player);	// ���W�����v���̃G�t�F�N�g��������
	void PlayerSwing(CPlayer& player);		// �v���C���[�̃X�C���O����
	void WallKickRight(CPlayer& player);	// �ǃL�b�N(�E��)
	void WallKickLeft(CPlayer& player);		// �ǃL�b�N(����)
	void WallKickProcess(CPlayer& player);	// �ǃL�b�N���ʂ̏���

	// �����o�ϐ�
	D3DXVECTOR3 m_posObject;	// �֗^���Ă���I�u�W�F�N�g�̈ʒu
	ACT m_Action;				// �s��
	int m_nActCount;			// �s���J�E���g
	float m_fCliffUpSpeed;		// �R�o��̑��x
	float m_fCliffGravity;		// �R�オ�莞�̏d��
	float m_fRotDest;			// �ڕW�̌���
	bool m_bRightLeave;			// �Ǘ��ꂵ���Ƃ��ɉE�������Ă������ǂ���
	bool m_bRightKick;			// �E�ǂ��R�������ǂ���
	bool m_bWallOld;			// �O��ǎC�肵�Ă������ǂ���
	bool m_bCliffSuccess;		// �R�o��Ɏ��s�������ǂ���
};

#endif