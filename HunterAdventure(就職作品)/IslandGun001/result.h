//============================================
//
// ���U���g�w�b�_�[[result.h]
// Author�F��������
//
//============================================
#ifndef _RESULT_H_			//���̃}�N����`������Ă��Ȃ�������
#define _RESULT_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "scene.h"

//--------------------------------------------
// �N���X(���S��ʃN���X)
//--------------------------------------------
class CResult : public CScene
{
public:				// �N�ł��A�N�Z�X�ł���

	CResult();		// �R���X�g���N�^
	~CResult();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

private:					// ���������A�N�Z�X�ł���
};

#endif