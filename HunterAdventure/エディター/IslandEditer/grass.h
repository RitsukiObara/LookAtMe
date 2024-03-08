//===================================
//
// ���w�b�_�[[grass.h]
// Author ��������
//
//===================================
#ifndef _GRASS_H_
#define _GRASS_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object3D.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(CGrass)
//-----------------------------------
class CGrass : public CObject3D
{
public:

	CGrass();			// �R���X�g���N�^
	~CGrass();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CGrass* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size);		// ��������

	static CListManager<CGrass*> GetList(void);			// ���X�g�̎擾����

private:

	// �����o�֐�
	//void SetVertexPos(void);		// ���_���W�̐ݒ菈��

	// �����o�ϐ�
	D3DXVECTOR3 m_LeftUp;			// �ړI�̍���̍��W
	D3DXVECTOR3 m_RightUp;			// �ړI�̉E��̍��W
	D3DXVECTOR3 m_LeftUpDest;		// �ړI�̍���̍��W
	D3DXVECTOR3 m_RightUpDest;		// �ړI�̉E��̍��W

	// �ÓI�����o�ϐ�
	static CListManager<CGrass*> m_list;		// ���X�g
};

#endif