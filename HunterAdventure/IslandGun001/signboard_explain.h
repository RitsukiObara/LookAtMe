//===================================
//
// �Ŕ̐����w�b�_�[[signboard_explain.h]
// Author ��������
//
//===================================
#ifndef _SIGNBOARD_EXPLAIN_H_
#define _SIGNBOARD_EXPLAIN_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object.h"
#include "signboard.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CObject2D;			// �I�u�W�F�N�g2D

//-----------------------------------
// �N���X��`(CSignboardExpl)
//-----------------------------------
class CSignboardExpl : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_APPEAR = 0,		// �o�����
		STATE_STOP,				// ��~���
		STATE_LEAVE,			// �ޏ���
		STATE_MAX				// ���̗񋓌^�̑���
	};

	// �\���̒�`(�w�i)
	struct SScreen
	{
		CObject2D* pScreen;		// �w�i�̏��
		float fAlpha;			// �����x
	};

	// �\���̒�`(����)
	struct SExplain
	{
		CObject2D* pExplain;	// �����̏��
		float fMoveY;			// Y���̈ړ���
	};

	CSignboardExpl();			// �R���X�g���N�^
	~CSignboardExpl();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const CSignboard::TYPE type);						// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CSignboardExpl* Create(const CSignboard::TYPE type);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Appear(void);		// �o����ԏ���
	bool Leave(void);		// �ޏ��ԏ���

	// �����o�ϐ�
	SScreen m_screen;		// ��ʂ̏��
	SExplain m_explain;		// �����̏��
	STATE m_state;			// ���
};

#endif