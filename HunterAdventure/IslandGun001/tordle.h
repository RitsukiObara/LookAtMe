//===================================
//
// �^�[�h���w�b�_�[[tordle.h]
// Author ��������
//
//===================================
#ifndef _TORDLE_H_
#define _TORDLE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "enemy.h"

//-----------------------------------
// �N���X��`(CTordle)
//-----------------------------------
class CTordle : public CEnemy
{
public:					// �N�ł��A�N�Z�X�ł���

	CTordle();			// �R���X�g���N�^
	~CTordle();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type) override;		// ���̐ݒ菈��
	void Hit(const int nDamage, const float fKnockback) override;		// �q�b�g����

private:				// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Chase(void);			// �ǐՏ���
	void Move(void);			// �ړ�����
	void RotMove(void);			// �����̈ړ�����

	// �����o�ϐ�
	float m_fMoveX;				// �ړ���(X��)
	float m_fMoveZ;				// �ړ���(Z��)
	D3DXVECTOR3 m_rotDest;		// �ڕW�̌���
};

#endif