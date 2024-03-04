//===================================
//
// ���̍��̌��w�b�_�[[gold_bone_light.h]
// Author ��������
//
//===================================
#ifndef _GOLD_BONE_LIGHT_H_
#define _GOLD_BONE_LIGHT_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "mesh_cylinder.h"

//-----------------------------------
// �N���X��`(���̍��̌�)
//-----------------------------------
class CGoldBoneLight : public CMeshCylinder
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_EXTEND = 0,		// �g����
		STATE_SHRINK,			// �k�����
		STATE_MAX				// ���̗񋓌^�̑���
	};

	CGoldBoneLight();			// �R���X�g���N�^
	~CGoldBoneLight();			// �f�X�g���N�^

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
	static CGoldBoneLight* Create(const D3DXVECTOR3& pos);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Extend(void);		// �L�я�ԏ���
	void Shrink(void);		// �k����ԏ���
	void SetTex(void);		// �e�N�X�`�����W�̐ݒ菈��

	// �����o�ϐ�
	STATE m_state;			// ���
};

#endif