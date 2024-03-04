//===================================
//
// �`���[�g���A����s�@�w�b�_�[[tutorial_airplane.h]
// Author ��������
//
//===================================
#ifndef _TUTORIAL_AIRPLANE_H_
#define _TUTORIAL_AIRPLANE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CPropeller;			// �v���y��

//-----------------------------------
// �N���X��`(�`���[�g���A����s�@)
//-----------------------------------
class CTutorialAirplane : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_STOP = 0,		// ��~���
		STATE_FLY,			// ��s���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CTutorialAirplane();			// �R���X�g���N�^
	~CTutorialAirplane();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(void);				// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);		// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;				// ��Ԃ̎擾����

	// �ÓI�����o�֐�
	static CTutorialAirplane* Create(void);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Fly(void);			// ��s����

	// �����o�ϐ�
	CPropeller* m_pPropeller;		// �v���y���̏��
	STATE m_state;					// ���
};

#endif