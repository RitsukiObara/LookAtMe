//===================================
//
// ���[�V�����u���[(�F)�w�b�_�[[blur_col.h]
// Author ��������
//
//===================================
#ifndef _BLUR_COL_H_
#define _BLUR_COL_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "blur.h"

//-----------------------------------
// �N���X��`(CBlurCol)
//-----------------------------------
class CBlurCol : public CBlur
{
public:					// �N�ł��A�N�Z�X�ł���

	CBlurCol(PRIORITY priority = PRIORITY_EFFECT);		// �I�[�o�[���[�h�R���X�g���N�^
	~CBlurCol();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const CXFile::SXFile& filedata, const int nLife);		// ���̐ݒ菈��(�F�L��)

	// �ÓI�����o�֐�
	static CBlurCol* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const CXFile::SXFile& filedata, const int nLife, const PRIORITY priority);	// ��������(�F�L��)

private:				// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXCOLOR m_col;		// �F
};

#endif