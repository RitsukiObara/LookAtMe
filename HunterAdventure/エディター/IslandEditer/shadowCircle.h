//===================================
//
// �ۉe�w�b�_�[[shadowCircle.h]
// Author ��������
//
//===================================
#ifndef _SHADOWCIRCLE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _SHADOWCIRCLE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"
#include "object3D.h"

//-----------------------------------
// �N���X��`(�e)
//-----------------------------------
class CShadowCircle : public CObject3D
{
public:			// �N�ł��A�N�Z�X�ł���

	CShadowCircle();			// �R���X�g���N�^
	~CShadowCircle();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V����
	void Draw(void) override;			// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const float fRadius, D3DXVECTOR3* posParent);		// ���̐ݒ菈��
	void Distance(void);		// �����x�̐ݒ菈��

	// �ÓI�����o�֐�
	static CShadowCircle* Create(const D3DXVECTOR3& pos, const float fRadius, D3DXVECTOR3* posParent);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXVECTOR3* m_posParent;	// �e�̈ʒu
	D3DXCOLOR m_col;			// �F
	float m_fRadiusInit;		// �����̔��a
};


#endif