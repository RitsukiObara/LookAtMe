//===================================
//
//�����L���O��ʃw�b�_�[[ranking.h]
//Author ��������
//
//===================================
#ifndef _RANKING_H_			//���̃}�N����`������Ă��Ȃ�������
#define _RANKING_H_			//2�d�C���N���[�h�h�~�̃}�N����ݒ肷��

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "scene.h"

//-----------------------------------
// �N���X��`(�����L���O�V�[��)
//-----------------------------------
class CRanking : public CScene
{
public:			// �N�ł��A�N�Z�X�ł���

	CRanking();					// �R���X�g���N�^
	~CRanking();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V
	void Draw(void) override;			// �`�揈��

	// �萔
	static const int MAX_RANKING = 5;	// �����L���O�̐�

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	int m_nTransCount;				// �����J�ڃJ�E���g
};

#endif