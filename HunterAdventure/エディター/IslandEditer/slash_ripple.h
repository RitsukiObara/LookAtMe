//===================================
//
// �a���̔g��w�b�_�[[slash_ripple.h]
// Author�F��������
//
//===================================
#ifndef _SLASH_RIPPLE_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _SLASH_RIPPLE_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "ripple.h"

//-----------------------------------
// �N���X��`(�a���̔g��)
//-----------------------------------
class CSlashRipple : public CRipple
{
public:		// �N�ł��A�N�Z�X�ł���

	CSlashRipple();			// �R���X�g���N�^
	~CSlashRipple();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot) override;				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CSlashRipple* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// ��������

private:	// ���������A�N�Z�X�ł���

};

#endif