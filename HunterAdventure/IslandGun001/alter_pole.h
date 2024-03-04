//===================================
//
// �Β��w�b�_�[[alter_pole.h]
// Author ��������
//
//===================================
#ifndef _ALTER_POLE_H_
#define _ALTER_POLE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CGoldBone;		// ���̍�

//-----------------------------------
// �N���X��`(�Β�)
//-----------------------------------
class CAlterPole : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CAlterPole();			// �R���X�g���N�^
	~CAlterPole();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);				// ���̐ݒ菈��

	// �����o�֐�
	void Charge(void);		// �`���[�W����
	void GoldBonePosSet(const D3DXVECTOR3& pos);		// ���̍��̈ʒu�ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetGoldBone(CGoldBone* pBone);			// ���̍��̐ݒ菈��
	CGoldBone* GetGoldBone(void) const;			// ���̍��̎擾����
	void SetBoneRot(const float fRot);			// ���̍��ւ̌����̐ݒ菈��
	float GetBoneRot(void) const;				// ���̍��ւ̌����̎擾����
	void SetBoneDist(const float fDist);		// ���̍��ւ̋����̐ݒ菈��
	float GetBoneDist(void) const;				// ���̍��ւ̋����̎擾����
	void SetEnableEmpty(const bool bEmpty);		// �󔒏󋵂̐ݒ菈��
	bool IsEmpty(void) const;					// �󔒏󋵂̎擾����
	void SetEnableArrival(const bool bArv);		// �����󋵂̐ݒ菈��
	bool IsArrival(void) const;					// �����󋵂̎擾����

	// �ÓI�����o�֐�
	static CAlterPole* Create(const D3DXVECTOR3& pos);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CGoldBone* m_pGoldBone;	// ���̍��̏��
	float m_fBoneRot;		// ���ւ̌���
	float m_fBoneDist;		// ���ւ̋���
	bool m_bEmpty;			// �󔒏�
	bool m_bArrival;		// ������
};

#endif