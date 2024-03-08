//===================================
//
// �G�f�B�b�g�w�b�_�[[edit.h]
// Author ��������
//
//===================================
#ifndef _EDIT_H_
#define _EDIT_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object.h"
#include "motion.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CBoss;			// �{�X
class CBossCollision;	// �{�X�̓����蔻��

//-----------------------------------
// �N���X��`(�G�f�B�b�g)
//-----------------------------------
class CEdit : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	CEdit();					// �R���X�g���N�^
	~CEdit();					// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(void);				// ���̐ݒ菈��
	void CollMatrix(void);			// �����蔻��̃}�g���b�N�X

	// �Z�b�g�E�Q�b�g�֌W
	CBoss* GetBoss(void);			// �{�X�̎擾����
	CBossCollision* GetColl(const int nIdx);		// �����蔻��̏��̎擾����

	// �ÓI�����o�֐�
	static CEdit* Create(void);		// ��������

private:		// ���������A�N�Z�X�ł���
	
	// �����o�֐�
	void Positioning(void);			// �ʒu�ݒ菈��
	void SetIdx(void);				// �ԍ��̐ݒ菈��
	void CorrectIdx(void);			// �ԍ��̕␳����
	void AddCollision(void);		// �����蔻��̒ǉ�����
	void NumCollDisp(void);			// �����蔻��̑����\������
	void SetRadius(void);			// �����蔻��̔��a�̐ݒ菈��
	void SetWeakness(void);			// �����蔻��̎�_�󋵂̐ݒ菈��

	// �����o�ϐ�
	CBoss* m_pBoss;		// �{�X
	CBossCollision* m_apColl[MAX_PARTS];	// �����蔻��̋�
	int m_nSetPartNumber;		// ���ݐݒ蒆�̃p�[�c�̔ԍ�
	int m_nSetCollNumber;		// ���ݐݒ蒆�̓����蔻��̔ԍ�
};

#endif