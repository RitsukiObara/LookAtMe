//============================================
//
// �^�C�g����ʃw�b�_�[[title.h]
// Author�F��������
//
//============================================
#ifndef _TITLE_H_			//���̃}�N����`������Ă��Ȃ�������
#define _TITLE_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "scene.h"

//--------------------------------------------
// �N���X(�^�C�g���N���X)
//--------------------------------------------
class CTitle : public CScene
{
public:				// �N�ł��A�N�Z�X�ł���

	CTitle();		// �R���X�g���N�^
	~CTitle();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V����
	void Draw(void) override;			// �`�揈��

private:					// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	int m_nTransCount;		// �J�ڃJ�E���g
};

#endif