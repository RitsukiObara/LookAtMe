//===================================
//
// �S�[���w�b�_�[[Goal.h]
// Author�F��������
//
//===================================
#ifndef _GOAL_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _GOAL_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object3D.h"
#include "scene.h"

//-----------------------------------
// �N���X��`(�S�[��)
//-----------------------------------
class CGoal : public CObject3D
{
public:		// �N�ł��A�N�Z�X�ł���

	CGoal();					// �R���X�g���N�^
	CGoal(CObject::TYPE type, CObject::PRIORITY priority = PRIORITY_PLAYER);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CGoal();			// �f�X�g���N�^
	void Box(void);				// �R���X�g���N�^�̔�

	// �����o�֐�
	virtual HRESULT Init(void);		// ����������
	virtual void Uninit(void);		// �I������
	virtual void Update(void) = 0;	// �X�V����
	virtual void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);		// ���̐ݒ菈��

	void AlphaChange(void);			// �����x�̕ω�

	// �ÓI�����o�֐�
	static CGoal* Create(const D3DXVECTOR3& pos, const CScene::MODE mode);		// ��������

private:	// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXCOLOR m_col;		// �F
	float m_fSubCol;		// �����x�̌��Z��
};

#endif