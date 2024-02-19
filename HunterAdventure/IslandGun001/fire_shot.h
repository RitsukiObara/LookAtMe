//===================================
//
// ���U���w�b�_�[[fire_shot.h]
// Author ��������
//
//===================================
#ifndef _FIRE_SHOT_H_
#define _FIRE_SHOT_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "mesh_cylinder.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(���U��)
//-----------------------------------
class CFireShot : public CMeshCylinder
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_EXTEND = 0,		// �L�я��
		STATE_SHRINK,			// �k�ݏ���
		STATE_MAX				// ���̗񋓌^�̑���
	};

	CFireShot();				// �R���X�g���N�^
	~CFireShot();				// �f�X�g���N�^

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
	static CFireShot* Create(const D3DXVECTOR3& pos);		// ��������

	static CListManager<CFireShot*> GetList(void);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Extend(void);		// �L�я�ԏ���
	bool Shrink(void);		// �k�ݏ�ԏ���
	void SetTex(void);		// �e�N�X�`�����W�̐ݒ菈��

	// �����o�ϐ�
	STATE m_state;			// ���

	// �ÓI�����o�ϐ�
	static CListManager<CFireShot*> m_list;		// ���X�g
};

#endif