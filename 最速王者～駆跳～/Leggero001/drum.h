//===================================
//
// �h�����ʃw�b�_�[[drum.h]
// Author ��������
//
//===================================
#ifndef _DRUM_H_
#define _DRUM_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "obstacle.h"

//-----------------------------------
// �N���X��`(��Q��)
//-----------------------------------
class CDrum : public CObstacle
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`
	typedef enum
	{
		STATE_NONE = 0,			// �ʏ���
		STATE_ROLL,				// �]������
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

	CDrum();					// �R���X�g���N�^
	~CDrum();					// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	bool Hit(void);				// �q�b�g����
	void HitLeft(void);			// �q�b�g(���ɔ�΂�)����

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// ���̐ݒ菈��

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXVECTOR3 m_move;				// �ړ���
	float m_fRotMove;				// �����̈ړ���
	STATE m_state;					// ���
	int m_nCollIdx;					// �����蔻��|���S���̃C���f�b�N�X
	int m_nSoundCount;				// �T�E���h�̃J�E���g
};

#endif