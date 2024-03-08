//===================================
//
// �Ւd�w�b�_�[[alter.h]
// Author ��������
//
//===================================
#ifndef _ALTER_H_
#define _ALTER_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "gold_bone.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CAlterPole;		// �Β�
class CAlterMessage;	// �Ւd���b�Z�[�W

//-----------------------------------
// �N���X��`(�Ւd)
//-----------------------------------
class CAlter : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_CHARGE,		// �`���[�W���
		STATE_WAIT,			// �ҋ@���
		STATE_BREAK,		// �j����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CAlter();			// �R���X�g���N�^
	~CAlter();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(void);				// ���̐ݒ菈��
	void Break(void);				// �j�󏈗�

	// �Z�b�g�E�Q�b�g�֌W
	CAlterPole* GetPole(const int nIdx) const;		// �Β��̎擾����
	void SetAlterMessage(CAlterMessage* pMessage);	// �Ւd�̃��b�Z�[�W�̐ݒ菈��
	void SetState(const STATE state);				// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;						// ��Ԃ̎擾����
	void SetEnableLightUp(const bool bLight);		// ���C�g�_���󋵂̐ݒ菈��
	bool IsLightUp(void) const;						// ���C�g�_���󋵂̎擾����

	// �ÓI�����o�֐�
	static CAlter* Create(void);	// ��������

	// �萔��`
	static const int NUM_POLE = 3;	// �Β��̐�

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void ChargeTrans(void);			// �`���[�W��Ԃւ̑J�ڏ���
	void GoldBoneLight(void);		// ���̍��̌�����

	// �����o�ϐ�
	CAlterPole* m_apPole[NUM_POLE];		// �Β��̏��
	CAlterMessage* m_pMessage;			// ���b�Z�[�W�̏��
	STATE m_state;						// ���
	bool m_bLightUp;					// ���C�g�_����
};

#endif