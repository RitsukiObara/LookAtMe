//============================================
//
// �I�u�W�F�N�g�O�p�`2D�w�b�_�[[objectTri2D.h]
// Author�F��������
//
//============================================
#ifndef _OBJECTTRI2D_H_		//���̃}�N����`������Ă��Ȃ�������
#define _OBJECTTRI2D_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �N���X(�I�u�W�F�N�g�O�p�`2D�N���X)
//--------------------------------------------
class CObjectTri2D : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	CObjectTri2D(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CObjectTri2D();		// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void BindTexture(int nIdx);		// �e�N�X�`���̊��蓖�ď���

	// ���_����n
	void SetVertex(void);						// ���_���̐ݒ�
	void SetVertexRot(void);					// ���_���̐ݒ�(�����L��)
	void SetVtxHardCoding(const D3DXVECTOR3& Left, const D3DXVECTOR3& Up, const D3DXVECTOR3& Right);	// ���_���̐ݒ�(�x�^�ł�)
	void SetVtxColor(const D3DXCOLOR& col);		// ���_�̐F�ݒ菈��
	void SetVtxTexture(void);					// �e�N�X�`���̐ݒ�
	void SetVertexAll(void);					// �S�Ă̒��_���̐ݒ�

	// �Z�b�g�E�Q�b�g�֌W
	void SetPos(const D3DXVECTOR3& pos);		// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) const;				// �ʒu�擾����

	void SetPosOld(const D3DXVECTOR3& posOld);	// �O��̈ʒu�ݒ菈��
	D3DXVECTOR3 GetPosOld(void) const;			// �O��̈ʒu�擾����

	void SetRot(const D3DXVECTOR3& rot);		// �����ݒ菈��
	D3DXVECTOR3 GetRot(void) const;				// �����擾����

	void SetSize(const D3DXVECTOR3& size);		// �T�C�Y�ݒ菈��
	D3DXVECTOR3 GetSize(void) const;			// �T�C�Y�擾����

	void SetLength(void);						// �����ݒ菈��
	float GetLength(void) const;				// �����擾����

	int GetTexIdx(void) const;					// �e�N�X�`���̔ԍ��̎擾����

	// �ÓI�����o�֐�
	static CObjectTri2D* Create(const CObject::TYPE type, const PRIORITY priority);		// ��������

protected:		// �����Ɣh���N���X�����A�N�Z�X�ł���

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posOld;						// �O��̈ʒu
	D3DXVECTOR3 m_rot;							// ����
	D3DXVECTOR3 m_size;							// �T�C�Y
	float m_fLength;							// ����
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�̃|�C���^
	int m_nTexIdx;								// �e�N�X�`���̃C���f�b�N�X
};

#endif