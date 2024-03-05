//===================================
//
// �G�t�F�N�g�w�b�_�[[Effect.h]
// Author ��������
//
//===================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// �N���X��`(CEffect)
//-----------------------------------
class CEffect : public CBillboard
{
public:

	// ���
	enum TYPE
	{
		TYPE_NONE = 0,			// �ʏ�
		TYPE_FIRE,				// ���n
		TYPE_DUST,				// ��
		TYPE_RUPTURE,			// �j��
		TYPE_SMOKE,				// ��
		TYPE_POPSMOKE,			// ��(�A�j����)
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	CEffect();			// �R���X�g���N�^
	~CEffect();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type, const D3DXCOLOR& col, const bool bAdd, const bool bZTest);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CEffect* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type, const D3DXCOLOR& col, const bool bAdd, const bool bZTest);	// ��������

private:

	// �����o�֐�
	void Move(void);				// �ړ�����
	void SubAlpha(void);			// �����x���Z����
	void Sizing(void);				// �T�C�W���O����

	// ��ނ��Ƃ̏���
	void FireProcess(void);			// ���n�̏���
	void RuptureProcess(void);		// �j��̏���

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXCOLOR m_col;		// �F
	TYPE m_type;			// ���
	int m_nLife;			// ����
	int m_nLifeOrigin;		// ���X�̎���
	float m_fSub;			// �����ɂȂ�Ԋu
	float m_fContra;		// ���a�̏k�ފԊu
	bool m_bAdd;			// ���Z������
	bool m_bZTest;			// Z�e�X�g�̗L��
};

#endif