//===================================
//
// �l�p�e�w�b�_�[[shadowSquare.h]
// Author ��������
//
//===================================
#ifndef _SHADOWSQUARE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _SHADOWSQUARE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"
#include "object3D.h"

//-----------------------------------
// �N���X��`(�l�p�e)
//-----------------------------------
class CShadowSquare : public CObject3D
{
public:			// �N�ł��A�N�Z�X�ł���

	CShadowSquare();			// �R���X�g���N�^
	~CShadowSquare();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size);		// ���̐ݒ菈��

	void AlphaSet(void);		// �����x�̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	int GetNumID(void) const;		// ID�̎擾����

	// �ÓI�����o�֐�
	static CShadowSquare* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size);	// ��������
	static CShadowSquare* GetShadow(int nIdx);																// �l�p�e�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXCOLOR m_col;			// �F
	int m_nID;					// �e�̃C���f�b�N�X

	// �ÓI�����o�ϐ�
	static int m_nNumAll;		// �e�̑���
};


#endif