//============================================
//
// ���e�̔����w�b�_�[[bomb_explosion.h]
// Author�F��������
//
//============================================
#ifndef _BOMB_EXPLOSION_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _BOMB_EXPLOSION_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "mesh_sphere.h"
#include "list_manager.h"

//--------------------------------------------
// �N���X��`(����)
//--------------------------------------------
class CBombExplosion : public CMeshSphere
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_EXTEND = 0,	// �g����
		STATE_CLEAR,		// ���������
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CBombExplosion();				// �R���X�g���N�^
	~CBombExplosion();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;		// ����������
	void Uninit(void) override;			// �I������
	void Update(void) override;			// �X�V����
	void Draw(void) override;			// �`�揈��

	void SetData(const D3DXVECTOR3& pos);	// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBombExplosion* Create(const D3DXVECTOR3& pos);	// ��������
	static CListManager<CBombExplosion*> GetList(void);		// ���X�g�̎擾����

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void AddScale(const float fAdd);		// �g�又��

	// �����o�ϐ�
	STATE m_state;				// ���
	int m_nStateCount;			// ��ԃJ�E���g
	float m_fAlpha;				// �����x

	// �ÓI�����o�ϐ�
	static CListManager<CBombExplosion*> m_list;	// ���X�g
};

#endif