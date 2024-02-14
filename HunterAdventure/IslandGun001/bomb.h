//===================================
//
// ���e�w�b�_�[[bomb.h]
// Author ��������
//
//===================================
#ifndef _BOMB_H_
#define _BOMB_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CBombFuse;		// ���ΐ�

//-----------------------------------
// �N���X��`(���e)
//-----------------------------------
class CBomb : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_GROWTH = 0,		// �������
		STATE_RIPEN,			// ������
		STATE_BOUND,			// �o�E���h���
		STATE_DETONATION,		// �N�����
		STATE_EXPLOSION,		// �������
		STATE_MAX				// ���̗񋓌^�̑���
	};

	CBomb();			// �R���X�g���N�^
	~CBomb();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void Hit(const float fRot, const float fSpeed);		// �q�b�g����
	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);		// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;				// ��Ԃ̎擾����

	// �ÓI�����o�֐�
	static CBomb* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// ��������

	static CListManager<CBomb*> GetList(void);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void RotMove(void);				// �����̈ړ�����
	void Move(void);				// �ړ�����
	void Gravity(void);				// �d�͏���
	bool ElevationCollision(void);	// �N���n�ʂ̓����蔻�菈��
	void Collision(void);			// �����蔻�菈��

	// �e��ԏ���
	void Growth(void);				// ������ԏ���
	void Bound(void);				// �o�E���h��ԏ���
	void Detonation(void);			// �N����ԏ���
	bool Explosion(void);			// ������ԏ���

	// �����o�ϐ�
	CBombFuse* m_pFuse;			// ���ΐ��̏��
	D3DXCOLOR m_col;			// �F
	D3DXVECTOR3 m_move;			// �ړ���
	STATE m_state;				// ���
	int m_nBoundCount;			// �o�E���h�J�E���g
	int m_nExplosionCount;		// �����J�E���g
	bool m_bAdd;				// ���Z��

	// �ÓI�����o�ϐ�
	static CListManager<CBomb*> m_list;		// ���X�g
};

#endif