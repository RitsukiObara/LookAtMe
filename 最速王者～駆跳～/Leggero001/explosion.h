//============================================
//
// �����w�b�_�[[bullet.h]
// Author�F��������
//
//============================================
#ifndef _EXPLOSION_H_			//���̃}�N����`������Ă��Ȃ�������
#define _EXPLOSION_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define EXPLOSION_SIZE		(D3DXVECTOR3(60.0f, 60.0f, 0.0f))			// �����̃T�C�Y

//--------------------------------------------
// �N���X(�����N���X)
//--------------------------------------------
class CExplosion : public CBillboardAnim
{
public:			// �N�ł��A�N�Z�X�ł���

	CExplosion();			// �R���X�g���N�^
	~CExplosion();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);					// �ݒ菈��

	// �ÓI�����o�֐�
	static CExplosion* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	int m_nLife;			// ����
};

#endif