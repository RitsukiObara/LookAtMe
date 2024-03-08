//===================================
//
// �Ւd�̃��b�Z�[�W�w�b�_�[[alter_message.h]
// Author ��������
//
//===================================
#ifndef _ALTER_MESSAGE_H_
#define _ALTER_MESSAGE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// �N���X��`(�Ւd���b�Z�[�W)
//-----------------------------------
class CAlterMessage : public CBillboard
{
public:

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_EXTEND = 0,	// �L�я��
		STATE_SHRINK,		// �k�ݏ��
		STATE_MAX			// ���̐�̏������s��Ȃ�
	};

	CAlterMessage();		// �R���X�g���N�^
	~CAlterMessage();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CAlterMessage* Create(const D3DXVECTOR3& pos);	// ��������

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_sizeDest;	// �ړI�̃T�C�Y
	STATE m_state;			// ���
	int m_nStateCount;		// ��ԃJ�E���g
};

#endif