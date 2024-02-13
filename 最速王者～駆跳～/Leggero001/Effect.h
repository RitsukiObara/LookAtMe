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
	typedef enum
	{
		TYPE_NONE = 0,			// ����
		TYPE_FIRE,				// ���n
		TYPE_ITEMGET,			// �A�C�e���l��
		TYPE_BIGJUMP,			// ���W�����v
		TYPE_SPEEDSTAR,			// ���x�̐�
		TYPE_CATCH,				// �R�L���b�`
		TYPE_RUNDUST,			// ��������̚�
		TYPE_BIGJUMPCHARGE,		// ���W�����v�̃`���[�W
		TYPE_LANDDUST,			// ���n��
		TYPE_MAX				// ���̗񋓌^�̑���
	}TYPE;

	CEffect();			// �R���X�g���N�^
	~CEffect();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type);				// ���̐ݒ菈��

	// ��ނ��Ƃ̏���
	void UpdateFire(void);			// ���n�̏���
	void UpdateBJCharge(void);		// ���W�����v�̃`���[�W�̏���

	// �ÓI�����o�֐�
	static CEffect* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type);		// ��������

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXCOLOR m_col;		// �F
	TYPE m_type;			// ���
	int m_nLife;			// ����
	float m_fSub;			// �����ɂȂ�Ԋu
	float m_fContra;		// ���a�̏k�ފԊu
	bool m_bAdd;			// ���Z������
};

#endif