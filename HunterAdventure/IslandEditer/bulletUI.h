//============================================
//
// �c�eUI�w�b�_�[[bulletUI.h]
// Author�F��������
//
//============================================
#ifndef _BULLET_UI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _BULLET_UI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define MAX_REMAINING_BULLET		(15)		// �c�e�̍ő�l

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;		// 2D�|���S��

//--------------------------------------------
// �N���X��`(�c�eUI)
//--------------------------------------------
class CBulletUI : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	// �\���̒�`(�c�e)
	struct SBullet
	{
		CObject2D* pBullet;		// �c�e
		bool bDisp;				// �`���
	};

	CBulletUI();				// �R���X�g���N�^
	~CBulletUI();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V����
	void Draw(void) override;			// �`�揈��

	void SetData(void);					// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetNumBullet(const int nNum);	// �c�e���̐ݒ菈��
	int GetNumBullet(void) const;		// �c�e���̎擾����

	// �ÓI�����o�֐�
	static CBulletUI* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	SBullet m_aBullet[MAX_REMAINING_BULLET];	// �e�ۂ̏��
	CObject2D* m_pReload;		// �����[�h�̕���
	int m_nNumBullet;		// �c�e��
};

#endif