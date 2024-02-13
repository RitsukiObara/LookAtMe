//===================================
//
// �I�[���w�b�_�[[aura.h]
// Author ��������
//
//===================================
#ifndef _AURA_H_
#define _AURA_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// �N���X��`(CAura)
//-----------------------------------
class CAura : public CBillboard
{
public:			// �N�ł��A�N�Z�X�ł���

	CAura();			// �R���X�g���N�^
	~CAura();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const int nLife);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CAura* Create(const D3DXVECTOR3& pos, const int nLife);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	int m_nLife;			// ����
	float m_fContra;		// ���a�̏k�ފԊu
};

#endif