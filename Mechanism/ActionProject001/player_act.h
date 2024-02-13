//============================================
//
// �v���C���[�̍s���w�b�_�[[player_act.h]
// Author�F��������
//
//============================================
#ifndef _PLAYER_ACT_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_ACT_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//--------------------------------------------
// �N���X(�v���C���[�̍s���N���X)
//--------------------------------------------
class CPlayerAct
{
public:			// �N�ł��A�N�Z�X�ł���

	//************************************************************
	//	�񋓌^��`(�s�����)
	//************************************************************
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
		STATE_INVINCIBLE,	// ���G���
		STATE_BOUND,		// �o�E���h���
		STATE_CANNON,		// ��C���
		STATE_FLY,			// ���ł�����
		STATE_DEATH,		// ���S���
		STATE_FALL,			// �������
		STATE_MAX			// ���̗񋓂̑���
	};

	CPlayerAct();			// �R���X�g���N�^
	~CPlayerAct();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(CPlayer& player);		// �X�V����

	void SetData(void);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֐�
	void SetState(STATE act);			// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;			// ��Ԃ̎擾����
	void SetFront(bool bFront);		// �O��󋵂̐ݒ菈��
	bool GetFront(void) const;		// �O��󋵂̎擾����

	// �ÓI�����o�֐�
	static CPlayerAct* Create(void);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void NoneControl(CPlayer& player);	// �ʏ펞�̑��쏈��
	void GroundQuake(CPlayer& player);	// �O���E���h�N�G�C�N
	void RotDecide(CPlayer& player);	// �����̌��菈��
	void Control(CPlayer& player);		// ���쏈��
	void MoveProcess(CPlayer& player);	// �ړ��̊�{����
	void ModeSpeed(CPlayer& player);	// ���[�h���Ƃ̃X�s�[�h����
	void Move(CPlayer& player);			// �ړ�����
	void Gravity(CPlayer& player);		// �d�͏���
	void RotMove(CPlayer& player);		// �����̈ړ�����
	void CannonRot(CPlayer& player);	// ��C�Ō�����ς��鏈��
	void GravityReset(CPlayer& player);	// �d�͂̃��Z�b�g����
	void FlyMove(CPlayer& player);		// ��C�Ŕ�ԏ���

	// �����o�ϐ�
	STATE m_state;			// �s�����
	int m_nStateCount;		// ��ԃJ�E���g
	bool m_bFront;			// �O���
};

#endif