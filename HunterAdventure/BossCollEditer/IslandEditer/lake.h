//===================================
//
// ����w�b�_�[[lake.h]
// Author ��������
//
//===================================
#ifndef _LAKE_H_
#define _LAKE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object3D_multitex.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(����)
//-----------------------------------
class CLake : public CObject3DMultiTex
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(�e�N�X�`���̎��)
	enum NUM
	{
		NUM_RIPPLE = 0,	// �g��̃e�N�X�`��
		NUM_OCEAN,		// �C�̃e�N�X�`��
		NUM_MAX			// ���̗񋓌^�̑���
	};

	CLake();			// �R���X�g���N�^
	~CLake();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CLake* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);		// ��������

	static CListManager<CLake*> GetList(void);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���
	
	// �����o�ϐ�
	D3DXVECTOR2 m_aTexU[NUM_MAX];		// �e�N�X�`�����W�̊J�n�ʒu(U�l)

	// �ÓI�����o�ϐ�
	static CListManager<CLake*> m_list;		// ���X�g
};

#endif