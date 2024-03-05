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
class CLifeUI;				// ����UI
class CPlayerController;	// �v���C���[�R���g���[���[
class CAirplane;			// ��s�@
class CShadowCircle;		// �ۉe

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
		MOTIONTYPE_SWOOP,			// �}�~��
		MOTIONTYPE_DAMAGE,			// �_���[�W
		MOTIONTYPE_AIRDIVE,			// ��s�@�~��
		MOTIONTYPE_DEATH,			// ���S
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
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void Hit(const int nDamage, const float fRotSmash);		// �q�b�g����
	void Healing(const int nHealing);			// �񕜏���
	void SetData(const D3DXVECTOR3& pos);		// ���̐ݒ菈��
	void ArrivalAirplane(void);					// ��s�@�̓�������

	// �Z�b�g�E�Q�b�g�֌W
	CMotion* GetMotion(void) const;					// ���[�V�����̏��̎擾����
	CPlayerAction* GetAction(void) const;			// �s���̏��̎擾����
	CHandgun* GetHandGun(const int nCount) const;	// ���e�̏��̎擾����
	CDagger* GetDagger(void) const;					// �_�K�[�̏��̎擾����
	CAim* GetAim(void) const;						// �G�C���̎擾����
	CBulletUI* GetBulletUI(void) const;				// �c�eUI�̏��̎擾����
	CGoldBoneUI* GetGoldBoneUI(void) const;			// ���̍�UI�̏��̎擾����
	CLifeUI* GetLifeUI(void) const;					// ����UI�̏��̎擾����
	CPlayerController* GetController(void) const;	// �R���g���[���[�̏��̎擾����
	CAirplane* GetAirplane(void) const;				// ��s�@�̏��̎擾����
	void DeleteAirplane();							// ��s�@��NULL������

	void SetRotDest(const D3DXVECTOR3& rot);	// �ړI�̌����̐ݒ菈��
	D3DXVECTOR3 GetRotDest(void) const;			// �ړI�̌����̎擾����
	void SetMove(const D3DXVECTOR3& move);		// �ړ��ʂ̐ݒ菈��
	D3DXVECTOR3 GetMove(void) const;			// �ړ��ʂ̎擾����
	void SetLife(const int nLife);				// �̗͂̐ݒ菈��
	int GetLife(void) const;					// �̗͂̎擾����
	void SetState(const SState state);			// ��Ԃ̐ݒ菈��
	SState GetState(void) const;				// ��Ԃ̎擾����
	void SetEnableJump(const bool bJump);		// �W�����v�󋵂̐ݒ菈��
	bool IsJump(void) const;					// �W�����v�󋵂̎擾����
	void SetAreaIdx(const int nIdx);			// �敪�̔ԍ��̐ݒ菈��
	int GetAreaIdx(void) const;					// �敪�̔ԍ��̎擾����

	// �ÓI�����o�֐�
	static CPlayer* Create(const D3DXVECTOR3& pos);	// ��������

protected:		// �����Ɣh���N���X�����A�N�Z�X�ł���

	// �����o�֐�
	void StateManager(void);		// ��ԊǗ�����
	void Move(void);				// �ړ�����
	void EmergentReload(void);		// �ً}�̃����[�h����
	void ShadowPosSet(void);		// �e�̈ʒu�ݒ菈��

	void ElevationCollision(void);	// �N���n�ʂ̓����蔻�菈��
	void Collision(void);			// �����蔻�菈��
	void BlockCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx);		// �u���b�N�Ƃ̓����蔻��
	void AlterCollision(void);		// �Ւd�Ƃ̓����蔻��
	void StageCollision(void);		// �X�e�[�W�Ƃ̓����蔻��

private:		// ���������A�N�Z�X�ł���

	// ��Ԃ��Ƃ̏���
	void DamageState();				// �_���[�W��ԏ���
	void InvisibleState();			// ���G��ԏ���

	// �����o�ϐ�
	CMotion* m_pMotion;					// ���[�V�����̏��
	CPlayerAction* m_pAction;			// �v���C���[�̍s���̏��
	CHandgun* m_apHandGun[NUM_HANDGUN];	// ���e�̏��
	CDagger* m_pDagger;					// �_�K�[�̏��
	CAim* m_pAim;						// �G�C���̏��
	CBulletUI* m_pBulletUI;				// �e�ۂ̏��
	CGoldBoneUI* m_pGoldBoneUI;			// ���̍�UI�̏��
	CLifeUI* m_pLifeUI;					// ����UI�̏��
	CPlayerController* m_pController;	// �v���C���[�̃R���g���[���[�̏��
	CAirplane* m_pAirplane;				// ��s�@�̏��
	CShadowCircle* m_pShadow;			// �ۉe�̏��

	D3DXVECTOR3 m_rotDest;		// �ڕW�̌���
	D3DXVECTOR3 m_move;			// �ړ���
	SState m_stateInfo;			// ��Ԋ֘A�̍\����
	int m_nLife;				// �̗�
	int m_nAreaIdx;				// �敪�̔ԍ�
	bool m_bMove;				// �ړ���
	bool m_bJump;				// �W�����v��

};

#endif