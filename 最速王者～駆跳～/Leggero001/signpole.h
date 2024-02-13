//===================================
//
// �W���w�b�_�[[signpole.h]
// Author ��������
//
//===================================
#ifndef _SIGNPOLE_H_
#define _SIGNPOLE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "obstacle.h"

//-----------------------------------
// �N���X��`(��Q��)
//-----------------------------------
class CSignPole : public CObstacle
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_NONE = 0,			// �ʏ���
		STATE_TOPPLE,			// �|����
		STATE_STOP,				// ��~���
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

	CSignPole();				// �R���X�g���N�^
	~CSignPole();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	bool Hit(void);				// �q�b�g����

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	STATE GetState(void);		// ��Ԃ̐ݒ菈��

	D3DXVECTOR3 GetVtx(const int nIdx);					// ���_�̎擾����

	float ElevationCollision(const D3DXVECTOR3 pos);	// �΂߂̏�Q���̓����蔻��
	float ObliqueCollision(const D3DXVECTOR3* Vtx, const D3DXVECTOR3& pos);									// �΂߂̓����蔻��

	bool ToppleCollision(const D3DXVECTOR3 pos);		// �|�ꒆ�̏�Q���̓����蔻��
	bool ToppleSystem(void);							// �|�ꒆ�̓����蔻��̑�{

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	STATE m_state;				// ���
	float m_fGravity;			// �d��
	D3DXVECTOR3 m_vtx[4];		// ���_���W
};

#endif