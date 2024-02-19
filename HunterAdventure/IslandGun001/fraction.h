//===================================
//
// �j�Ѓw�b�_�[[fraction.h]
// Author ��������
//
//===================================
#ifndef _FRACTION_H_
#define _FRACTION_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �N���X��`(�j��)
//-----------------------------------
class CFraction : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_DELETE,		// �������
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_SCREW = 0,		// �l�W
		TYPE_GEAR,			// ����
		TYPE_RING,			// �ւ���
		TYPE_WOOD,			// ��
		TYPE_ROCK,			// ��
		TYPE_BOSSROCK,		// �{�X�̐Ԃ��Ƃ�
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	CFraction();			// �R���X�g���N�^
	~CFraction();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const TYPE type, const int nLife, const int nMoveWidth, const int nMoveHeight);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CFraction* Create(const D3DXVECTOR3& pos, const TYPE type, const int nLife, const int nMoveWidth, const int nMoveHeight);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void MoveSet(const int nMoveWidth, const int nMoveHeight);		// �ړ��ʂ̐ݒ菈��
	void Move(void);		// �ړ�����
	void RotMove(void);		// �����̈ړ�����
	void Elevation(void);	// �N���n�ʂƂ̓����蔻��

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rotMove;	// �����̈ړ���
	STATE m_state;			// ���
	int m_nLife;			// ����
	float m_fAlpha;			// �����x
};

#endif