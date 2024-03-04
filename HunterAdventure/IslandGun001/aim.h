//===================================
//
// �G�C���w�b�_�[[aim.h]
// Author ��������
//
//===================================
#ifndef _AIM_H_
#define _AIM_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// �N���X��`(CAim)
//-----------------------------------
class CAim : public CBillboard
{
public:

	CAim();			// �R���X�g���N�^
	~CAim();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);			// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetPosPlayer(const D3DXVECTOR3& pos);		// �v���C���[�̈ʒu�̐ݒ菈��

	// �ÓI�����o�֐�
	static CAim* Create(const D3DXVECTOR3& pos);	// ��������

private:

	// �����o�֐�
	void Collision(void);			// �����蔻�菈��
	bool EnemyCollision(void);		// �G�Ƃ̓����蔻��
	bool BalloonCollision(void);	// ���D�Ƃ̓����蔻��

	// �����o�ϐ�
	D3DXVECTOR3 m_posPlayer;		// �v���C���[�̈ʒu
	float m_fLength;				// ����
	bool m_bColl;					// �����蔻���
};

#endif