//===================================
//
// �X��(�x��)�w�b�_�[[streetpole.h]
// Author ��������
//
//===================================
#ifndef _STREETPOLE_H_
#define _STREETPOLE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "obstacle.h"

//-----------------------------------
// �N���X��`(��Q��)
//-----------------------------------
class CStreetPole : public CObstacle
{
public:			// �N�ł��A�N�Z�X�ł���

	CStreetPole();				// �R���X�g���N�^
	~CStreetPole();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	bool Hit(void);				// �q�b�g����

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// ���̐ݒ菈��

private:		// ���������A�N�Z�X�ł���

};

#endif