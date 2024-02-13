//============================================
//
// �G�w�b�_�[[enemy.h]
// Author�F��������
//
//============================================
#ifndef _ENEMY_H_			//���̃}�N����`������Ă��Ȃ�������
#define _ENEMY_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "character.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CMotion;		// ���[�V����

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define ENEMY_GRAVITY		(0.5f)		// �G�̏d��
#define ENEMY_WIDTH			(50.0f)		// �G�̕�
#define ENEMY_HEIGHT		(120.0f)	// �G�̍���

//--------------------------------------------
// �N���X(�G�N���X)
//--------------------------------------------
class CEnemy : public CCharacter
{
public:			// �N�ł��A�N�Z�X�ł���

	// ���
	typedef enum
	{
		TYPE_SHOT = 0,		// �e�ł��^
		TYPE_KICK,			// �L�b�N�^
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// ���
	typedef enum
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_DEATH,		// ���S���
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	CEnemy();				// �R���X�g���N�^
	CEnemy(CObject::TYPE type, PRIORITY priority = PRIORITY_BLOCK);		// �R���X�g���N�^(�I�[�o�[���[�h)
	virtual ~CEnemy();		// �f�X�g���N�^
	void Box(void);			// �R���X�g���N�^�̔�

	// �����o�֐�
	virtual HRESULT Init(void) = 0;	// ����������
	virtual void Uninit(void);		// �I������
	virtual void Update(void);		// �X�V����
	virtual void Draw(void);		// �`�揈��

	void Hit(void);										// �q�b�g����
	virtual void SetData(const D3DXVECTOR3& pos) = 0;	// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);	// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;			// ��Ԃ̎擾����

	void SetMotion(void);				// ���[�V�����̐ݒ�(����)����
	CMotion* GetMotion(void);			// ���[�V�����̎擾����

	// �ÓI�����o�֐�
	static CEnemy* Create(const D3DXVECTOR3& pos, const TYPE type);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CMotion* m_pMotion;		// ���[�V�����̏��
	TYPE m_type;			// ���
	STATE m_state;			// ���
};

#endif