//===================================
//
// �G�t�F�N�g���f���w�b�_�[[effect_model.h]
// Author ��������
//
//===================================
#ifndef _EFFECT_MODEL_H_
#define _EFFECT_MODEL_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �N���X��`(CEffectModel)
//-----------------------------------
class CEffectModel : public CModel
{
public:					// �N�ł��A�N�Z�X�ł���

	CEffectModel();			// �R���X�g���N�^
	~CEffectModel();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXVECTOR3& scale, const int nLife, const D3DXVECTOR3& rotMove);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CEffectModel* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXVECTOR3& scale, const int nLife, const D3DXVECTOR3& rotMove);	// ��������

private:				// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Move(void);		// �ړ�����
	void RotMove(void);		// �����̈ړ�����
	void SubAlpha(void);	// �����x�̌��Z����
	void Reduce(void);		// �g�嗦�̌��Z����

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rotMove;	// �����̈ړ���
	D3DXVECTOR3 m_reduce;	// �g�嗦�̏k�ފԊu
	int m_nLife;			// ����
	float m_fAlpha;			// �����x
	float m_fSub;			// �����ɂȂ�Ԋu
};

#endif