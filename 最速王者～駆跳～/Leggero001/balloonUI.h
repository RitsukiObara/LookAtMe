//============================================
//
// �����o��UI�w�b�_�[[balloonUI.h]
// Author�F��������
//
//============================================
#ifndef _BALLOONUI_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _BALLOONUI_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object2D.h"
#include "balloon.h"

//--------------------------------------------
// �N���X��`(2D�����o��)
//--------------------------------------------
class CBalloonUI : public CObject2D
{
public:				// �N�ł��A�N�Z�X�ł���

	CBalloonUI();			// �R���X�g���N�^
	~CBalloonUI();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetType(const CBalloon::TYPE nType);		// ��ނ̐ݒ菈��
	CBalloon::TYPE GetType(void) const;				// ��ނ̎擾����

	// �ÓI�����o�֐�
	static CBalloonUI* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CBalloon::TYPE m_type;	// ���
};

#endif