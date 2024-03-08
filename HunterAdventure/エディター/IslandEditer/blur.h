//===================================
//
// ���[�V�����u���[�w�b�_�[[blur.h]
// Author ��������
//
//===================================
#ifndef _BLUR_H_
#define _BLUR_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �N���X��`(CBlur)
//-----------------------------------
class CBlur : public CModel
{
public:					// �N�ł��A�N�Z�X�ł���

	CBlur(PRIORITY priority = PRIORITY_EFFECT);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CBlur();			// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile& filedata, const int nLife, const float fAlpha);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֐�
	float GetSub(void) const;			// ���Z�ʂ̎擾����

private:				// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	int m_nLife;			// ����
	float m_fSub;			// ���Z��
};

#endif