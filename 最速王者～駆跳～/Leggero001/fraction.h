//============================================
//
// �j�Ѓw�b�_�[[fraction.h]
// Author�F��������
//
//============================================
#ifndef _FRACTION_H_			//���̃}�N����`������Ă��Ȃ�������
#define _FRACTION_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "model.h"

//--------------------------------------------
// �N���X(�j�ЃN���X)
//--------------------------------------------
class CFraction : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CFraction();			// �R���X�g���N�^
	~CFraction();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init();			// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3 scale, const D3DXVECTOR3& move, const int nTexIdx, const D3DXCOLOR col);			// ���̐ݒ菈��

	static void SetObstacle(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col);			// ��Q���̔j�Ђ̐ݒ菈��
	static void SetWallKick(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col);			// �ǃL�b�N���̔j�Ђ̐ݒ菈��
	static void SetLCliffCatch(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col);		// ���̊R�͂ݎ��̔j�Ђ̐ݒ菈��
	static void SetRCliffCatch(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col);		// �E�̊R�͂ݎ��̔j�Ђ̐ݒ菈��

	// �ÓI�����o�֐�
	static CFraction* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3 scale, const D3DXVECTOR3& move, const int nTexIdx, const D3DXCOLOR col);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXCOLOR m_col;		// �F
};

#endif