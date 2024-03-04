//===================================
//
// ���D�X�|�i�[�w�b�_�[[balloon_spawner.h]
// Author ��������
//
//===================================
#ifndef _BALLOON_SPAWNER_H_
#define _BALLOON_SPAWNER_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object.h"
#include "list_manager.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CBalloon;			// ���D

//-----------------------------------
// �N���X��`(���D�X�|�i�[)
//-----------------------------------
class CBalloonSpawner : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	CBalloonSpawner();		// �R���X�g���N�^
	~CBalloonSpawner();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);				// ���̐ݒ菈��
	void Hit(void);					// �q�b�g����

	// �Z�b�g�E�Q�b�g�֌W
	CBalloon* GetBalloon(void) const;	// ���D�̎擾����

	// �ÓI�����o�֐�
	static CBalloonSpawner* Create(const D3DXVECTOR3& pos);		// ��������

	static CListManager<CBalloonSpawner*> GetList(void);		// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CBalloon* m_pBalloon;	// ���D�̏��

	D3DXVECTOR3 m_pos;		// �ʒu
	int m_nIntervalCount;	// ���D�̊Ԋu�J�E���g

	// �ÓI�����o�ϐ�
	static CListManager<CBalloonSpawner*> m_list;		// ���X�g���
};

#endif