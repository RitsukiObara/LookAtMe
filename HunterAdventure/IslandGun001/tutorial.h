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
class CTutorialPlayer;			// �`���[�g���A���v���C���[
class CSignboard;				// �Ŕ�
class CDoor;					// �h�A
class CTutorialAirplane;		// �`���[�g���A���̔�s�@

//--------------------------------------------
// �N���X(�`���[�g���A���N���X)
//--------------------------------------------
class CTutorial : public CScene
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_EXPL,			// �������
		STATE_TRANS,		// �J�ڏ��
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CTutorial();		// �R���X�g���N�^
	~CTutorial();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V����
	void Draw(void) override;			// �`�揈��

	// �ÓI�����o�֐�
	static CTutorialPlayer* GetPlayer(void);			// �v���C���[�̎擾����
	static CDoor* GetDoor(void);						// �h�A�̎擾����
	static void SetState(const STATE state);			// ��Ԃ̐ݒ菈��
	static STATE GetState(void);						// ��Ԃ̎擾����
	static void SetLookSign(CSignboard* pSign);			// �Ŕ̐ݒ菈��
	static CSignboard* GetLookSign(void);				// �Ŕ̎擾����
	static CTutorialAirplane* GetAirplane(void);		// ��s�@�̎擾����

	static void DeletePlayer(void);				// �v���C���[��NULL������
	static void DeleteDoor(void);				// �h�A��NULL������
	static void DeleteAirplane(void);			// ��s�@��NULL������

private:			// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	int m_nTransCount;					// �J�ڃJ�E���g

	// �ÓI�����o�ϐ�
	static CTutorialPlayer* m_pPlayer;	// �v���C���[�̏��
	static CSignboard* m_pLook;			// ���݌��Ă�Ŕ�
	static CDoor* m_pDoor;				// �h�A�̏��
	static CTutorialAirplane* m_pAirplane;	// ��s�@�̏��
	static STATE m_state;				// ���
};

#endif