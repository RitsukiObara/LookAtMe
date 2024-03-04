//===================================
//
// �{�X�̃o���A�w�b�_�[[boss_barrier.h]
// Author ��������
//
//===================================
#ifndef _BOSS_BARRIER_H_
#define _BOSS_BARRIER_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// �N���X��`(CBossBarrier)
//-----------------------------------
class CBossBarrier : public CBillboard
{
public:

	CBossBarrier();				// �R���X�g���N�^
	~CBossBarrier();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBossBarrier* Create(const D3DXVECTOR3& pos);		// ��������

private:

	// �����o�ϐ�
	int m_nLife;		// ����
};

#endif