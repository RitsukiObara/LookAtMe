//============================================
//
// �L�����N�^�[�u���[�w�b�_�[[chara_blur.h]
// Author�F��������
//
//============================================
#ifndef _CHARA_BLUR_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _CHARA_BLUR_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "character.h"

//--------------------------------------------
// �N���X(�L�����N�^�[�u���[�N���X)
//--------------------------------------------
class CCharaBlur : public CCharacter
{
public:			// �N�ł��A�N�Z�X�ł���

	CCharaBlur(PRIORITY priority = PRIORITY_EFFECT);		// �I�[�o�[���[�h�R���X�g���N�^
	~CCharaBlur();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const int nNumModel, const D3DXCOLOR& col, const int nLife);		// ���̐ݒ菈��
	void SetModelData(const int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile& filedata, const int nParentIdx);		// ���f���̏��ݒ菈��

	// �ÓI�����o�֐�
	static CCharaBlur* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const int nNumModel, const D3DXCOLOR& col, const int nLife, const PRIORITY priority);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXCOLOR m_col;		// �F
	int m_nLife;			// ����
	float m_fSub;			// ���Z��
};

#endif