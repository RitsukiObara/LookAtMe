//===================================
//
// ���U���w�b�_�[[wind_shot.h]
// Author ��������
//
//===================================
#ifndef _WIND_SHOT_H_
#define _WIND_SHOT_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "mesh_tornado.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(���U��)
//-----------------------------------
class CWindShot : public CMeshTornado
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_ATTACK = 0,		// �U�����
		STATE_DELETE,			// ��������
		STATE_MAX				// ���̗񋓌^�̑���
	};

	CWindShot();				// �R���X�g���N�^
	~CWindShot();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);					// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);		// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;				// ��Ԃ̎擾����

	// �ÓI�����o�֐�
	static CWindShot* Create(const D3DXVECTOR3& pos);		// ��������

	static CListManager<CWindShot*> GetList(void);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void ElevationCollision(void);		// �N���n�ʂƂ̓����蔻��
	void RotMove(void);					// �����ړ�����
	void Attack(void);					// �U����ԏ���

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	STATE m_state;			// ���
	float m_fRotMove;		// �ړ�����
	float m_fAlpha;			// �����x

	// �ÓI�����o�ϐ�
	static CListManager<CWindShot*> m_list;		// ���X�g
};

#endif