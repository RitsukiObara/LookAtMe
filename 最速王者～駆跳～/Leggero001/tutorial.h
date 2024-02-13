//============================================
//
// �`���[�g���A����ʃw�b�_�[[tutorial.h]
// Author�F��������
//
//============================================
#ifndef _TUTORIAL_H_			//���̃}�N����`������Ă��Ȃ�������
#define _TUTORIAL_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "scene.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CBalloonUI;		// �ӂ�������UI

//--------------------------------------------
// �N���X(�`���[�g���A���N���X)
//--------------------------------------------
class CTutorial : public CScene
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_SPEED,		// ���xUI�\��
		STATE_BOOST,		// �u�[�X�g�������
		STATE_POLE,			// �X���W�����v
		STATE_STAMINA,		// �X�^�~�i
		STATE_END,			// �I�����
		STATE_SKIP,			// �X�L�b�v���
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	CTutorial();		// �R���X�g���N�^
	~CTutorial();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);					// ����������
	void Uninit(void);					// �I������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��

	void SetData(const MODE mode);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֐�
	static void SetState(const STATE state);	// ��Ԃ̐ݒ菈��
	static STATE GetState(void);				// ��Ԃ̎擾����

	static CBalloonUI* GetBalloonUI(void);		// �ӂ�����2D�̎擾����

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void SkipCreate(void);				// �X�L�b�v�~�̏o������

	// �����o�ϐ�
	int m_nEndCount;					// �I���܂ł̃J�E���g

	// �ÓI�����o�ϐ�
	static STATE m_state;				// ���
	static CBalloonUI* m_pBalloonUI;	// �ӂ�����2D�̏��
};

#endif