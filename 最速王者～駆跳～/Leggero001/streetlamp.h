//===================================
//
// �X��(����)�w�b�_�[[streetlamp.h]
// Author ��������
//
//===================================
#ifndef _STREETLAMP_H_
#define _STREETLAMP_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "obstacle.h"

//-----------------------------------
// �N���X��`(��Q��)
//-----------------------------------
class CStreetLamp : public CObstacle
{
public:			// �N�ł��A�N�Z�X�ł���

	CStreetLamp();				// �R���X�g���N�^
	~CStreetLamp();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	void PlayerCatch(D3DXVECTOR3* pos);		// �v���C���[�̃L���b�`����

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// ���̐ݒ菈��

private:		// ���������A�N�Z�X�ł���


};

#endif