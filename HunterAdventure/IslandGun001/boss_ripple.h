//=====================================================
//
// �{�X�̎��S�g��w�b�_�[ [boss_ripple.h]
// Author�F��������
//
//=====================================================
#ifndef _BOSS_RIPPLE_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _BOSS_RIPPLE_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************************
// �C���N���[�h�t�@�C��
//*****************************************************
#include "mesh_donut.h"

//-----------------------------------------------------
// �N���X��`(�{�X�̎��S�g��)
//-----------------------------------------------------
class CBossRipple : public CMeshDonut
{
public:		// �N�ł��A�N�Z�X�ł���

	CBossRipple();		// �I�[�o�[���[�h�R���X�g���N�^
	~CBossRipple();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBossRipple* Create(const D3DXVECTOR3& pos);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Spread(void);		// �L������

	// �����o�ϐ�
	float m_fAddCircum;		// �~���̒ǉ���
	float m_fAddWidth;		// ���̒ǉ���
	float m_fAlpha;			// �����x
};


#endif