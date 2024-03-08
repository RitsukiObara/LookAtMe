//===================================
//
// ���[�V�����u���[(���̐F)�w�b�_�[[blur_org.h]
// Author ��������
//
//===================================
#ifndef _BLUR_ORG_H_
#define _BLUR_ORG_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "blur.h"

//-----------------------------------
// �N���X��`(CBlurOrg)
//-----------------------------------
class CBlurOrg : public CBlur
{
public:					// �N�ł��A�N�Z�X�ł���

	CBlurOrg(PRIORITY priority = PRIORITY_EFFECT);		// �I�[�o�[���[�h�R���X�g���N�^
	~CBlurOrg();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const float fAlpha, const CXFile::SXFile& filedata, const int nLife);		// ���̐ݒ菈��(�F�L��)

	// �ÓI�����o�֐�
	static CBlurOrg* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const float fAlpha, const CXFile::SXFile& filedata, const int nLife, const PRIORITY priority);	// ��������(�F�L��)

private:				// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	float m_fAlpha;		// �����x
};

#endif