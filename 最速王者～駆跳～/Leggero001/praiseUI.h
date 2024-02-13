//============================================
//
// �J�ߌ��t�w�b�_�[[praiseUI.h]
// Author�F��������
//
//============================================
#ifndef _PRAISEUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _PRAISEUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object2D.h"

//--------------------------------------------
// �N���X��`(���l)
//--------------------------------------------
class CPraiseUI : public CObject2D
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		TYPE_AMAZING = 0,		// �A���C�W���O
		TYPE_GREAT,				// �O���[�g
		TYPE_EXCITING,			// �G�L�T�C�e�B���O
		TYPE_MAX				// ���̗񋓌^�̑���
	}TYPE;

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_APPEAR = 0,		// �o�����
		STATE_MOVE,				// �ړ����
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

	CPraiseUI();				// �R���X�g���N�^
	~CPraiseUI();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const int nScore);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CPraiseUI* Create(const int nScore);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Appear(void);		// �o������
	void Move(void);		// �ړ�����
	void Cycle(void);		// ��]����
	void ScaleDown(void);	// �k������

	// �����o�ϐ�
	D3DXVECTOR3 m_posDest;	// �ړI�̈ʒu
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_sizeDest;	// �ړI�̃T�C�Y
	D3DXVECTOR3 m_scaling;	// �T�C�Y�̊g���
	STATE m_state;			// ���
	int m_nStateCount;		// ��ԃJ�E���g
	int m_nScore;			// ���_

	// �ÓI�����o�ϐ�
	static const char* m_apTexturename[TYPE_MAX];	// �e�N�X�`���̃p�X��
};

#endif