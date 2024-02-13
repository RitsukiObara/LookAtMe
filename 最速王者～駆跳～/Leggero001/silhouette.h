//===================================
//
// �^�C�g���e�G�w�b�_�[[silhouette.h]
// Author ��������
//
//===================================
#ifndef _SILHOUETTE_H_
#define _SILHOUETTE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object2D.h"

//-----------------------------------
// �N���X��`(CSilhouette)
//-----------------------------------
class CSilhouette : public CObject2D
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		TYPE_RUN = 0,		// ����l�^
		TYPE_CLIMBOVER,		// ���z����l�^
		TYPE_GRIP,			// �͂ސl�^
		TYPE_JUMP,			// ��щz����l�^
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_APPEAR = 0,	// �o�����
		STATE_STOP,			// ��~���
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	CSilhouette();			// �R���X�g���N�^
	~CSilhouette();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const TYPE type, const bool bRight);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CSilhouette* Create(const TYPE type, const bool bRight);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	STATE m_state;		// ���
	TYPE m_type;		// ���
	int m_nStateCount;	// ��ԃJ�E���g
	float m_fCircumRot;	// �~����̌���
	float m_fRotMove;	// �����̈ړ���
	bool m_bRight;		// �E�����ǂ���

	// �ÓI�����o�ϐ�
	static const char* m_apTextureFile[TYPE_MAX];			// �e�N�X�`���̖��O
};

#endif