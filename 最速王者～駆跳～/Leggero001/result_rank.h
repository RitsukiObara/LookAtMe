//============================================
//
// ���U���g�����N�w�b�_�[[result_rank.h]
// Author�F��������
//
//============================================
#ifndef _RESULT_RANK_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _RESULT_RANK_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "model.h"
#include "result.h"

//--------------------------------------------
// �N���X��`(�^�C��)
//--------------------------------------------
class CResultRank : public CModel
{
public:				// �N�ł��A�N�Z�X�ł���

	CResultRank();		// �R���X�g���N�^
	~CResultRank();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3 pos, const CResult::RANK rank);			// ���̐ݒ菈��

	// �ÓI�����o�ϐ�
	static CResultRank* Create(const D3DXVECTOR3 pos, const CResult::RANK rank);		// ��������

private:			// ���������A�N�Z�X�ł���

};

#endif