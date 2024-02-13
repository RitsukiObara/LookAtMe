//===================================
//
// ���ΐ��w�b�_�[[hydrant.h]
// Author ��������
//
//===================================
#ifndef _HYDRANT_H_
#define _HYDRANT_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "obstacle.h"

//-----------------------------------
// �N���X��`(��Q��)
//-----------------------------------
class CHydrant : public CObstacle
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`
	typedef enum
	{
		STATE_NONE = 0,			// �ʏ���
		STATE_SMASH,			// ������΂����
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

	CHydrant();					// �R���X�g���N�^
	~CHydrant();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	bool Hit(void);				// �q�b�g����

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// ���̐ݒ菈��

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	STATE m_state;				// ���
	float m_fGravity;			// �d�͏���
	float m_fRotMove;			// �����̈ړ���
};

#endif