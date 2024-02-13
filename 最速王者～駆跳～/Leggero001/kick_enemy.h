//============================================
//
// ��Q���R��G�w�b�_�[[kick_enemy.h]
// Author�F��������
//
//============================================
#ifndef _KICK_ENEMY_H_			//���̃}�N����`������Ă��Ȃ�������
#define _KICK_ENEMY_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "enemy.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CDrum;			// ��Q��

//--------------------------------------------
// �N���X(��Q���R��G�N���X)
//--------------------------------------------
class CKickEnemy : public CEnemy
{
public:			// �N�ł��A�N�Z�X�ł���

	//--------------------------------------------
	// ���[�V�����̎��
	//--------------------------------------------
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,		// �ʏ�
		MOTIONTYPE_MAX				// ���̗񋓌^�̑���
	}MOTIONTYPE;

	//--------------------------------------------
	// �s�����
	//--------------------------------------------
	typedef enum
	{
		ACT_WAITING = 0,			// �ҋ@���
		ACT_BRING,					// �\�����
		ACT_SHOOT,					// �V���[�g����
		ACT_MAX						// ���̗񋓌^�̑���
	}ACT;

	CKickEnemy();		// �R���X�g���N�^
	~CKickEnemy();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init();		// ����������
	void Uninit(void);	// �I������
	void Update(void);	// �X�V����
	void Draw(void);	// �`�揈��

	void SetData(const D3DXVECTOR3& pos);	// ���̐ݒ菈��

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	bool SearchObstacle(void);	// ��Q���̑{������
	void Turn(void);			// �U���������
	void Collision(void);		// �����蔻�菈��

	// �����o�ϐ�
	ACT m_action;				// �s�����
	CDrum* m_pDrum;				// �h�����ʂ̃|�C���^
	D3DXVECTOR3 m_posDest;		// �h�����ʂ̖ړI�̈ʒu
	D3DXVECTOR3 m_drumMove;		// �h�����ʂ̈ړ���
	int m_nStateCount;			// ��ԃJ�E���g
	float m_fGravity;			// �d��
	bool m_bRight;				// �E������
};

#endif