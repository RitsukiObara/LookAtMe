//============================================
//
// �����o���w�b�_�[[balloon.h]
// Author�F��������
//
//============================================
#ifndef _BALLOON_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _BALLOON_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// �N���X��`(�r���{�[�h)
//--------------------------------------------
class CBalloon : public CBillboard
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ푀��
		TYPE_SUPERJUMP,		// ���W�����v
		TYPE_WALLKICK,		// �ǃL�b�N
		TYPE_CLIFFCLIMB,	// �R�オ��
		TYPE_CLIFFLAND,		// �R���n
		TYPE_ACTION,		// �A�N�V����
		TYPE_POLECATCH,		// �|�[���L���b�`
		TYPE_POLEJUMP,		// �|�[���W�����v
		TYPE_BOOST,			// �u�[�X�g
		TYPE_STAMINA,		// �X�^�~�i
		TYPE_SPEED,			// ���x�֌W
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_SMALL = 0,	// ���k���
		STATE_DISP,			// �\�����
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	CBalloon();				// �R���X�g���N�^
	~CBalloon();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);							// ��Ԃ̐ݒ菈��

	TYPE GetType(void) const;									// ��ނ̎擾����

	// �ÓI�����o�֐�
	static CBalloon* Create(const D3DXVECTOR3& pos, const TYPE type);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Disp(void);		// �\����Ԃ̏���
	void Small(void);		// ���k��Ԃ̏���
	void Expansion(void);	// �g�又��
	void Reduce(void);		// �k������
	void Bound(void);		// �o�E���h����

	// �����o�ϐ�
	D3DXVECTOR3 m_sizeDisp;		// �`�掞�̃T�C�Y
	D3DXVECTOR3 m_sizeMove;		// �T�C�Y�̈ړ���
	STATE m_state;				// ���
	TYPE m_type;				// ���
	bool m_bChange;				// �ω���
};

#endif