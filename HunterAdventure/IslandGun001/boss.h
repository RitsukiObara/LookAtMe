//===================================
//
// �{�X�w�b�_�[[boss.h]
// Author ��������
//
//===================================
#ifndef _BOSS_H_
#define _BOSS_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "character.h"
#include "list_manager.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CMotion;			// ���[�V����
class CBossState;		// �{�X�̏��
class CBossLifeUI;		// �{�X�̗̑�UI
class CBossCollision;	// �{�X�̓����蔻��

//-----------------------------------
// �N���X��`(CBoss)
//-----------------------------------
class CBoss : public CCharacter
{
public:					// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���[�V�����̎��)
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		// �ʏ탂�[�V����
		MOTIONTYPE_FLY,				// ��s���[�V����
		MOTIONTYPE_HOVERING,		// �z�o�����O���[�V����
		MOTIONTYPE_LANDING,			// ���n���[�V����
		MOTIONTYPE_HOWLING,			// �Y�����у��[�V����
		MOTIONTYPE_FLYING,			// ��s���[�V����
		MOTIONTYPE_CHARGE,			// �`���[�W���[�V����
		MOTIONTYPE_DOWN,			// �_�E�����[�V����
		MOTIONTYPE_STOMP,			// �����݃��[�V����
		MOTIONTYPE_DEATH,			// ���S���[�V����
		MOTIONTYPE_MAX				// ���̗񋓌^�̑���
	};

	// ��_�̃p�[�c
	enum WEAKPART
	{
		WEAK_BODY = 0,				// ��
		WEAK_RFSHIN,				// �E�O��
		WEAK_LFSHIN,				// ���O��
		WEAK_RBSHIN,				// �E�����
		WEAK_LBSHIN,				// �������
		WEAK_MAX					// ���̗񋓌^�̑���
	};

	// �_���[�W�󋵊֌W
	struct SDamage
	{
		int nCount;		// �J�E���g
		bool bDamage;	// �_���[�W��
	};

	CBoss();		// �R���X�g���N�^
	~CBoss();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void Hit(const int nDamage);	// �q�b�g����
	void BarrierBreak(const D3DXVECTOR3& pos, const int nPart, const int nDamage);		// �o���A�j�󏈗�
	void BarrierHit(const D3DXVECTOR3& pos, const int nPart, const int nCntPart);		// �o���A�̃q�b�g����
	void StunRecovery(void);		// �C��񕜏���

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// ���̐ݒ菈��

	bool ElevationCollision(void);					// �N���n�ʂƂ̓����蔻��
	void ChangeState(CBossState* pNext);			// ��Ԃ̑J�ڏ���

	// �Z�b�g�E�Q�b�g�֌W
	CMotion* GetMotion(void);		// ���[�V�����̎擾����
	CBossCollision* GetColl(const int nIdx);		// �����蔻��̎擾����

	void SetEnableHit(const bool bHit);		// �q�b�g�󋵂̐ݒ菈��
	bool IsHit(void) const;					// �q�b�g�󋵂̎擾����

	// �ÓI�����o�֐�
	static CBoss* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// ��������

	static CListManager<CBoss*> GetList(void);			// ���X�g�̎擾����

private:				// ���������A�N�Z�X�ł���

	// �����o�֐�
	void EnemyAllClear(void);	// �G�̑S��������
	void Damage(void);			// �_���[�W����

	// �����o�ϐ�
	CMotion* m_pMotion;		// ���[�V�����̏��
	CBossState* m_pState;	// ��Ԃ̏��
	CBossLifeUI* m_pLifeUI;	// �̗�UI�̏��
	CBossCollision* m_apColl[MAX_PARTS];	// �����蔻��̋�
	D3DXMATERIAL* m_apMatCopy[MAX_PARTS];	// �}�e���A���̃R�s�[

	SDamage m_damage;		// �_���[�W���
	int m_aWeakPointLife[WEAK_MAX];			// ��_�̃��C�t
	int m_nLife;			// �̗�
	bool m_bDown;			// �_�E����
	bool m_bHit;			// �q�b�g��

	// �ÓI�����o�ϐ�
	static CListManager<CBoss*> m_list;		// ���X�g
};

#endif