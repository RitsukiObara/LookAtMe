//===================================
//
// ���̍��w�b�_�[[gold_bone.h]
// Author ��������
//
//===================================
#ifndef _GOLD_BONE_H_
#define _GOLD_BONE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CGoldBoneLight;		// ���̍��̌�

//-----------------------------------
// �N���X��`(���̍�)
//-----------------------------------
class CGoldBone : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_GET,			// �擾���
		STATE_UP,			// �㏸���
		STATE_ALTERSET,		// �Ւd�������
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CGoldBone();			// �R���X�g���N�^
	~CGoldBone();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);				// ���̐ݒ菈��
	void Hit(void);					// �q�b�g����

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);		// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;				// ��Ԃ̎擾����
	void SetLight(void);					// ���̐ݒ菈��
	CGoldBoneLight* GetLight(void);			// ���̎擾����

	// �ÓI�����o�֐�
	static CGoldBone* Create(const D3DXVECTOR3& pos);	// ��������

	static CListManager<CGoldBone*> GetList(void);		// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Cycle(void);		// ��]����
	void Get(void);			// �擾����
	void Up(void);			// �㏸����
	void AlterSet(void);	// �Ւd�ݒ菈��

	// �����o�֐�
	CGoldBoneLight* m_pLight;	// ���̏��

	STATE m_state;			// ���
	int m_nPoleIdx;			// �Β��̃C���f�b�N�X
	float m_fDestHeight;	// �ړI�̍���
	float m_fRotMove;		// �����̈ړ���

	// �ÓI�����o�ϐ�
	static CListManager<CGoldBone*> m_list;		// ���X�g
};

#endif