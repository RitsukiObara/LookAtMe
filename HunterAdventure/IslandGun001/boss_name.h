//============================================
//
// �{�X�̖��O�\���w�b�_�[[boss_name.h]
// Author�F��������
//
//============================================
#ifndef _BOSS_NAME_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _BOSS_NAME_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;		// 2D�|���S��

//--------------------------------------------
// �N���X��`(�{�X�̖��O�\��)
//--------------------------------------------
class CBossName : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_NAMESHADOW = 0,	// ���O�̉e
		TYPE_NAME,				// ���O
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_APPEAR = 0,		// �o�����
		STATE_STOP,				// ��~���
		STATE_SHADOW,			// �e���
		STATE_LEAVE,			// �ޏ���
		STATE_MAX				// ���̗񋓌^�̑���
	};

	CBossName();				// �R���X�g���N�^
	~CBossName();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V����
	void Draw(void) override;			// �`�揈��

	void SetData(void);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBossName* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	bool Appear(CObject2D* pObject);		// �o����ԏ���
	void AppearArrival(void);				// �o����ԑJ�ڏ���
	void Shadow(void);						// �e��ԏ���
	bool Leave(CObject2D* pObject);			// �ޏ��ԏ���

	// �����o�ϐ�
	CObject2D* m_apObject[TYPE_MAX];	// ���O�\���̏��
	STATE m_state;			// ���
	int m_nStateCount;		// ��ԃJ�E���g
};

#endif