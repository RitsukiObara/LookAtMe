//============================================
//
// �`���[�g���A����ʃw�b�_�[[tutorial.h]
// Author�F��������
//
//============================================
#ifndef _TUTORIAL_H_			//���̃}�N����`������Ă��Ȃ�������
#define _TUTORIAL_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "scene.h"

//--------------------------------------------
// �N���X(�`���[�g���A���N���X)
//--------------------------------------------
class CTutorial : public CScene
{
public:				// �N�ł��A�N�Z�X�ł���

	CTutorial();		// �R���X�g���N�^
	~CTutorial();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V����
	void Draw(void) override;			// �`�揈��

private:			// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	int m_nEndCount;					// �I���܂ł̃J�E���g
};

#endif