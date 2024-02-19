//============================================
//
// ����UI�w�b�_�[[ranking_rank.h]
// Author�F��������
//
//============================================
#ifndef _RANKING_RANK_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _RANKING_RANK_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;		// 2D�|���S��

//--------------------------------------------
// �N���X��`(����)
//--------------------------------------------
class CRankUI : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	CRankUI();				// �R���X�g���N�^
	~CRankUI();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V����
	void Draw(void) override;			// �`�揈��

	void SetData(void);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CRankUI* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �萔
	static const int MAX_RANK = 5;		// ���ʂ̍ő吔

	// �����o�ϐ�
	CObject2D* m_apRankUI[MAX_RANK];	// ����UI�̏��
};

#endif