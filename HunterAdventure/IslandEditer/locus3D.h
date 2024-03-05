//===================================
//
// 3D�c���w�b�_�[[locus3D.h]
// Author ��������
//
//===================================
#ifndef _LOCUS3D_H_
#define _LOCUS3D_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// �N���X��`(CLocus3D)
//-----------------------------------
class CLocus3D : public CBillboard
{
public:			// �N�ł��A�N�Z�X�ł���

	CLocus3D();				// �R���X�g���N�^
	~CLocus3D();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx, const bool bLookY, const bool bLighting);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CLocus3D* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx, const bool bLookY, const bool bLighting);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	int m_nLife;			// ����
	float m_fSub;			// �����x�̌�����
	float m_fAlpha;			// �����x
	bool m_bLightOff;		// ���C�e�B���O��
};

#endif