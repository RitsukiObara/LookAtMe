//============================================
//
// �C�艌�w�b�_�[[rubsmoke.h]
// Author�F��������
//
//============================================
#ifndef _RUBSMOKE_H_			//���̃}�N����`������Ă��Ȃ�������
#define _RUBSMOKE_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// �N���X(�C�艌�N���X)
//--------------------------------------------
class CRubSmoke : public CBillboardAnim
{
public:			// �N�ł��A�N�Z�X�ł���

	CRubSmoke();			// �R���X�g���N�^
	~CRubSmoke();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);					// �ݒ菈��

	// �ÓI�����o�֐�
	static CRubSmoke* Create(const D3DXVECTOR3& pos);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	int m_nLife;			// ����
};

#endif