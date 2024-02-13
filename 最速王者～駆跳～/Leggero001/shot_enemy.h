//============================================
//
// �e�����G�w�b�_�[[shotenemy.h]
// Author�F��������
//
//============================================
#ifndef _SHOTENEMY_H_			//���̃}�N����`������Ă��Ȃ�������
#define _SHOTENEMY_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "enemy.h"

//--------------------------------------------
// �N���X(�e�����G�N���X)
//--------------------------------------------
class CShotEnemy : public CEnemy
{
public:			// �N�ł��A�N�Z�X�ł���

	// ���[�V�����̎��
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,		// �ʏ�
		MOTIONTYPE_MAX				// ���̗񋓌^�̑���
	}MOTIONTYPE;

	// ���
	typedef enum
	{
		ACT_SHOT = 0,	// �ˌ����
		ACT_TURN,		// �U��������
		ACT_MAX			// ���̗񋓌^�̑���
	}ACT;

	CShotEnemy();		// �R���X�g���N�^
	~CShotEnemy();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init();		// ����������
	void Uninit(void);	// �I������
	void Update(void);	// �X�V����
	void Draw(void);	// �`�揈��

	void SetData(const D3DXVECTOR3& pos);		// ���̐ݒ菈��

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void InitRot(void);		// �����̌����̐ݒ菈��
	void Shot(void);		// �ˌ�����
	void Turn(void);		// �U���������

	void ActionShot(void);	// �e������Ԃł̏���
	void ActionTurn(void);	// �U�������Ԃł̏���

	// �����o�ϐ�
	ACT m_action;			// �s�����
	int m_nShotCount;		// �e�����J�E���g
	float m_fGravity;		// �d��
	float m_fRotDest;		// �ړI�̌���
	float m_fRotDiff;		// �����̍���
};

#endif