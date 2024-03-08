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

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define NUM_HANDGUN		(2)		// ���e�̐�

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CMotion;				// ���[�V����
class CPlayerAction;		// �v���C���[�̍s��
class CHandgun;				// ���e
class CDagger;				// �_�K�[
class CAim;					// �G�C��
class CBulletUI;			// �e��UI
class CGoldBoneUI;			// ���̍�UI
class CPlayerController;	// �v���C���[�R���g���[���[

//--------------------------------------------
// �N���X(�v���C���[�N���X)
//--------------------------------------------
class CPlayer : public CCharacter
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���[�V�����̎��)
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		// �ҋ@
		MOTIONTYPE_MOVE,			// �ړ�
		MOTIONTYPE_DODGE,			// ���
		MOTIONTYPE_DAGGER,			// �_�K�[
		MOTIONTYPE_MAX				// ���̗񋓌^�̑���
	};

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,				// �ʏ���
		STATE_DAMAGE,				// �_���[�W���
		STATE_INVINSIBLE,			// ���G���
		STATE_DEATH,				// ���S���
		STATE_MAX					// ���̗񋓌^�̑���
	};

	// �\���̒�`(��Ԋ֘A)
	struct SState
	{
		D3DXCOLOR col;		// �̂̐F(��Ԃɂ���ĕς��)
		STATE state;		// ���
		int nCount;			// ��ԃJ�E���g
	};

	CPlayer();				// �R���X�g���N�^
	~CPlayer();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void Hit(const int nDamage);	// �q�b�g����
	void SetData(const D3DXVECTOR3& pos);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	CMotion* GetMotion(void) const;					// ���[�V�����̏��̎擾����
	CPlayerAction* GetAction(void) const;			// �s���̏��̎擾����
	CHandgun* GetHandGun(const int nCount) const;	// ���e�̏��̎擾����
	CDagger* GetDagger(void) const;					// �_�K�[�̏��̎擾����
	CAim* GetAim(void) const;						// �G�C���̎擾����
	CBulletUI* GetBulletUI(void) const;				// �c�eUI�̏��̎擾����
	CGoldBoneUI* GetGoldBoneUI(void) const;			// ���̍�UI�̏��̎擾����

	void SetRotDest(const D3DXVECTOR3& rot);	// �ړI�̌����̐ݒ菈��
	D3DXVECTOR3 GetRotDest(void) const;			// �ړI�̌����̎擾����
	void SetMove(const D3DXVECTOR3& move);		// �ړ��ʂ̐ݒ菈��
	D3DXVECTOR3 GetMove(void) const;			// �ړ��ʂ̎擾����
	SState GetState(void) const;				// ��Ԃ̎擾����
	void SetEnableJump(const bool bJump);		// �W�����v�󋵂̐ݒ菈��
	bool IsJump(void) const;					// �W�����v�󋵂̎擾����

	// �ÓI�����o�֐�
	static CPlayer* Create(const D3DXVECTOR3& pos);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void StateManager(void);		// ��ԊǗ�����
	void Move(void);				// �ړ�����
	void ElevationCollision(void);	// �N���n�ʂ̓����蔻�菈��
	void TreeCollision(void);		// �؂Ƃ̓����蔻��
	void BlockCollision(void);		// �u���b�N�Ƃ̓����蔻��
	void RockCollision(void);		// ��Ƃ̓����蔻��

	// �����o�ϐ�
	CMotion* m_pMotion;					// ���[�V�����̏��
	CPlayerAction* m_pAction;			// �v���C���[�̍s���̏��
	CHandgun* m_apHandGun[NUM_HANDGUN];	// ���e�̏��
	CDagger* m_pDagger;					// �_�K�[�̏��
	CAim* m_pAim;						// �G�C���̏��
	CBulletUI* m_pBulletUI;				// �e�ۂ̏��
	CGoldBoneUI* m_pGoldBoneUI;			// ���̍�UI�̏��
	CPlayerController* m_pController;	// �v���C���[�̃R���g���[���[�̏��

	D3DXVECTOR3 m_rotDest;		// �ڕW�̌���
	D3DXVECTOR3 m_move;			// �ړ���
	SState m_stateInfo;			// ��Ԋ֘A�̍\����
	int m_nLife;				// �̗�
	bool m_bMove;				// �ړ���
	bool m_bJump;				// �W�����v��

};

#endif