//===================================
//
// ���V�̎��w�b�_�[[palm_fruit.h]
// Author ��������
//
//===================================
#ifndef _PALM_FRUIT_H_
#define _PALM_FRUIT_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(���V�̎�)
//-----------------------------------
class CPalmFruit : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,	// �ʏ���
		STATE_FALL,		// �������
		STATE_BOUND,	// �o�E���h���
		STATE_STOP,		// ��~���
		STATE_GET,		// �������
		STATE_MAX		// ���̗񋓌^�̑���
	};

	CPalmFruit();					// �R���X�g���N�^
	~CPalmFruit();					// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// ���̐ݒ菈��
	void Hit(void);					// �q�b�g����

	// �Z�b�g�E�Q�b�g�֌W
	void SetMove(const D3DXVECTOR3& move);	// �ړ��ʂ̐ݒ菈��
	D3DXVECTOR3 GetMove(void) const;		// �ړ��ʂ̎擾����
	void SetState(const STATE state);		// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;				// ��Ԃ̎擾����

	// �ÓI�����o�֐�
	static CPalmFruit* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);	// ��������

	static CListManager<CPalmFruit*> GetList(void);		// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���
	
	// �����o�֐�
	void Move(void);				// �ړ�����
	void Gravity(void);				// �d�͏���
	bool ElevationCollision(void);	// �N���n�ʂ̓����蔻�菈��
	void StageCollision(void);		// �X�e�[�W�̓����蔻��

	// �����o�ϐ�
	D3DXVECTOR3 m_move;			// �ړ���
	STATE m_state;				// ���

	// �ÓI�����o�ϐ�
	static CListManager<CPalmFruit*> m_list;		// ���X�g
};

#endif