//============================================
//
// �e�w�b�_�[[bullet.h]
// Author�F��������
//
//============================================
#ifndef _BULLET_H_			//���̃}�N����`������Ă��Ȃ�������
#define _BULLET_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define BULLET_SIZE		(D3DXVECTOR3(40.0f, 20.0f, 0.0f))			// �e�̃T�C�Y

//--------------------------------------------
// �N���X(�e�N���X)
//--------------------------------------------
class CBullet : public CBillboard
{
public:			// �N�ł��A�N�Z�X�ł���

	// �e�̎��
	typedef enum
	{
		TYPE_PLAYER = 0,	// �v���C���[�̒e
		TYPE_ENEMY,			// �G�̒e
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	CBullet();				// �R���X�g���N�^
	~CBullet();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fMoveRot, TYPE type);			// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetMoveRot(const float fMoveRot);			// �ړ������̐ݒ菈��
	float GetMoveRot(void) const;					// �ړ������̎擾����

	void SetMove(const D3DXVECTOR3& move);			// �ړ��ʂ̐ݒ菈��
	D3DXVECTOR3 GetMove(void) const;				// �ړ��ʂ̎擾����

	// �ÓI�����o�֐�
	static CBullet* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fMoveRot, TYPE type);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Move(void);		// �ړ�����
	void Impact(const D3DXVECTOR3& pos);			// ���e����

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	TYPE m_type;			// ���
	int m_nLife;			// ����
	float m_fMoveRot;		// �ړ�����
};

#endif