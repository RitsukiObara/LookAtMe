//============================================
//
// �R���e�B�j���[UI�\���w�b�_�[[continueUI.h]
// Author�F��������
//
//============================================
#ifndef _CONTINUE_UI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _CONTINUE_UI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;		// 2D�|���S��
class CNumber;			// �ԍ�

//--------------------------------------------
// �N���X��`(�R���e�B�j���[UI)
//--------------------------------------------
class CContinueUI : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_SCREEN = 0,	// �w�i
		TYPE_GROUND,		// ���n
		TYPE_QUERY,			// ����
		TYPE_COST,			// �R�X�g
		TYPE_YES,			// YES�̑I����
		TYPE_NO,			// NO�̑I����
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �񋓌^��`(�I����)
	enum SELECT
	{
		SELECT_YES = 0,		// YES�̑I����
		SELECT_NO,			// NO�̑I����
		SELECT_MAX			// ���̗񋓌^�̑���
	};

	CContinueUI();				// �R���X�g���N�^
	~CContinueUI();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V����
	void Draw(void) override;			// �`�揈��

	void SetData(void);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CContinueUI* Create(void);	// ��������

	// �ÓI�����o�֐�
	static void SubScoreReset(void);	// ���_�̌��Z�ʂ̃��Z�b�g����
	static int GetSubScore(void);		// ���_�̌��Z�ʂ̎擾����

	// �萔��`
	static const int NUM_COSTDIGIT = 4;		// ����X�R�A�̌���

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Select(void);		// �I������
	bool Decide(void);		// ���菈��
	void Continue(void);	// �R���e�B�j���[����

	// �����o�ϐ�
	CObject2D* m_apObject[TYPE_MAX];		// �|���S���̏��
	CNumber* m_apNumber[NUM_COSTDIGIT];		// �ԍ�
	SELECT m_select;		// �I����

	// �ÓI�����o�ϐ�
	static int m_nSubScore;		// ���_�̌��Z�� 
};

#endif