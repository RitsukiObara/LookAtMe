//===================================
//
// ���e�w�b�_�[[handgun.h]
// Author ��������
//
//===================================
#ifndef _HANDGUN_H_
#define _HANDGUN_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �N���X��`(���e)
//-----------------------------------
class CHandgun : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,	// �ʏ���
		STATE_RELOAD,	// �����[�h���
		STATE_RETURN,	// �߂���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	CHandgun();			// �R���X�g���N�^
	~CHandgun();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot, D3DXMATRIX* mtx);				// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);			// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;					// ��Ԃ̎擾����
	void SetEnableDisp(const bool bDisp);		// �`��󋵂̐ݒ菈��
	bool IsDisp(void) const;					// �`��󋵂̎擾����

	// �ÓI�����o�֐�
	static CHandgun* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot, D3DXMATRIX* mtx);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void PosMove(void);				// �ʒu�̈ړ�����
	void RotMove(void);				// �����̈ړ�����
	void ReturnMove(void);			// �߂��Ԃ̈ړ�����

	// �����o�ϐ�
	D3DXVECTOR3 m_posDest;			// �ړI�̈ʒu
	D3DXVECTOR3 m_posInit;			// �����ʒu
	D3DXVECTOR3 m_rotInit;			// ��������
	D3DXMATRIX* m_pMtxParent;		// �e�̃}�g���b�N�X
	STATE m_state;					// ���
	int m_nReloadCount;				// �����[�h�J�E���g
	float m_fRotMove;				// �����̈ړ���
	bool m_bDisp;					// �`���
};

#endif