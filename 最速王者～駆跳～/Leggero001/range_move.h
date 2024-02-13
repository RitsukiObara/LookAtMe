//===================================
//
// �͈͈ړ��w�b�_�[[range_move.h]
// Author�F��������
//
//===================================
#ifndef _RANGE_MOVE_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _RANGE_MOVE_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object3D.h"

//-----------------------------------
// �N���X��`(�S�[��)
//-----------------------------------
class CRangeMove : public CObject3D
{
public:		// �N�ł��A�N�Z�X�ł���

	// ���
	typedef enum
	{
		STATE_ADJUST = 0,	// �T�C�Y�������
		STATE_MOVE,			// �ړ����
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	CRangeMove();			// �R���X�g���N�^
	~CRangeMove();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CRangeMove* Create(const D3DXVECTOR3& pos);		// ��������

private:	// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Move(void);			// �ړ�����
	void Pos(void);				// �ʒu�ړ�����
	void StateChange(void);		// ��Ԃ̐؂�ւ�����
	void SizeX(void);			// �T�C�Y(X��)�̒�������
	void SizeY(void);			// �T�C�Y(Y��)�̒�������
	void Size(void);			// �T�C�Y(�S��)�̒�������
	void RangeMove(void);		// �͈͂̈ړ�����
	D3DXVECTOR3 Hit(D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin);		// �����蔻�菈��

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	STATE m_state;			// ���
};

#endif