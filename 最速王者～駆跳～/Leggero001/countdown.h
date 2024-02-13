//============================================
//
// �J�E���g�_�E���w�b�_�[[countdown.h]
// Author�F��������
//
//============================================
#ifndef _COUNTDOWN_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _COUNTDOWN_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object2D.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CLocus2D;			// �c��

//--------------------------------------------
// �N���X��`(�J�E���g�_�E��)
//--------------------------------------------
class CCountdown : public CObject2D
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(�e�N�X�`��)
	typedef enum
	{
		TEX_NUMBER = 0,			// ����
		TEX_START,				// �X�^�[�g
		TEX_MAX					// ���̗񋓌^�̑���
	}TEX;

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_COUNT = 0,		// �J�E���g���
		STATE_START,			// �X�^�[�g���
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

	CCountdown();				// �R���X�g���N�^
	~CCountdown();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);		// ���̐ݒ菈��
	void Count(void);		// �J�E���g�_�E������
	bool SearchLocus(void);	// �c���{������
	void LocusSize(void);	// �c���̃T�C�Y�ݒ菈��

	// �ÓI�����o�֐�
	static CCountdown* Create(void);	// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CLocus2D* m_pLocus;			// �c���̃|�C���^
	STATE m_state;				// ���
	int m_nCountDown;			// �X�^�[�g�܂ł̃^�C��
	int m_nIntervalCount;		// �Ԋu�̃J�E���g
	float m_fAlpha;				// �����x

	// �ÓI�����o�ϐ�
	static const char* m_apTexturename[TEX_MAX];	// �e�N�X�`���̃p�X��
};

#endif