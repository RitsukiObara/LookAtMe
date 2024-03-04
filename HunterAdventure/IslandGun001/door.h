//===================================
//
// �h�A�w�b�_�[[door.h]
// Author ��������
//
//===================================
#ifndef _DOOR_H_
#define _DOOR_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CPushTiming;		// �{�^���\��

//-----------------------------------
// �N���X��`(�h�A)
//-----------------------------------
class CDoor : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_OPEN,			// �J�����
		STATE_CLOSE,		// �����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CDoor();		// �R���X�g���N�^
	~CDoor();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);				// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);		// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;				// ��Ԃ̎擾����
	void SetEnableDisp(const bool bDisp);	// �`��󋵂̐ݒ菈��
	bool IsDisp(void) const;				// �`��󋵂̎擾����

	// �ÓI�����o�֐�
	static CDoor* Create(const D3DXVECTOR3& pos);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Open(void);		// �J������
	void Close(void);		// ������

	// �����o�ϐ�
	CModel* m_pDoor;		// �h�A�{�̂̏��
	CPushTiming* m_pButton;	// �{�^���̏��
	STATE m_state;			// ���
	bool m_bDisp;			// �{�^���̕`���
};

#endif