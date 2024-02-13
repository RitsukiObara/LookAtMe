//===================================
//
// �ؔ��w�b�_�[[woodbox.h]
// Author ��������
//
//===================================
#ifndef _WOODBOX_H_
#define _WOODBOX_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "obstacle.h"

//-----------------------------------
// �N���X��`(��Q��)
//-----------------------------------
class CWoodBox : public CObstacle
{
public:			// �N�ł��A�N�Z�X�ł���

	CWoodBox();					// �R���X�g���N�^
	~CWoodBox();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const TYPE type);		// ���̐ݒ菈��

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	float m_fGravity;			// �d��
};

#endif