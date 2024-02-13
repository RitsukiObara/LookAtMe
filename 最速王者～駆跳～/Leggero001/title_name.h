//============================================
//
// �^�C�g���w�b�_�[[title_name.h]
// Author�F��������
//
//============================================
#ifndef _TITLE_NAME_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _TITLE_NAME_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object2D.h"

//--------------------------------------------
// �N���X��`(�^�C�g��)
//--------------------------------------------
class CTitleName : public CObject2D
{
public:				// �N�ł��A�N�Z�X�ł���

	// ���
	typedef enum
	{
		STATE_MOVE = 0,		// �ړ����
		STATE_BRAKE,		// �u���[�L���
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	CTitleName();			// �R���X�g���N�^
	~CTitleName();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �ÓI�����o�֐�
	static CTitleName* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void MoveState(void);	// �ړ���ԏ���
	void BrakeState(void);	// �u���[�L��ԏ���

	// �����o�ϐ�
	STATE m_state;			// ���
	int m_nStateCount;		// ��ԃJ�E���g
};

#endif