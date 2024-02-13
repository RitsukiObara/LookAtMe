//============================================
//
// ���nUI�w�b�_�[[landUI.h]
// Author�F��������
//
//============================================
#ifndef _LANDUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _LANDUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object2D.h"

//--------------------------------------------
// �N���X��`(���l)
//--------------------------------------------
class CLandUI : public CObject2D
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		TYPE_BAD = 0,		// BAD
		TYPE_GOOD,			// GOOD
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_APPEAR = 0,	// �o��
		STATE_DISP,			// �\��
		STATE_DELETE,		// ����
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	CLandUI();				// �R���X�g���N�^
	~CLandUI();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(TYPE type);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CLandUI* Create(TYPE type);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Move(void);			// �ړ�����
	void DispDecision(void);	// �\����Ԕ��菈��

	// �����o�ϐ�
	TYPE m_type;				// ���
	STATE m_state;				// ���
	int m_nStateCount;			// ��ԃJ�E���g

	// �ÓI�����o�ϐ�
	static const char* m_apTexturename[TYPE_MAX];	// �e�N�X�`���̃p�X��
};

#endif