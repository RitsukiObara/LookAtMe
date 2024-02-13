//===================================
//
// �I�u�W�F�N�g�O�p3D�w�b�_�[[objectTri3D.h]
// Author ��������
//
//===================================
#ifndef _OBJECTTRI3D_H_
#define _OBJECTTRI3D_H_

#include "object.h"

//--------------------------------------------
// �N���X(�I�u�W�F�N�g�O�p3D�N���X)
//--------------------------------------------
class CObjectTri3D : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	CObjectTri3D(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CObjectTri3D();		// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void DrawLightOff(void);		// �`�揈��(���C�e�B���O����)

	void BindTexture(int nIdx);		// �e�N�X�`���̊��蓖�ď���

	// ���_����n
	void SetVertex(void);									// ���_���̐ݒ�
	void SetVertexHardCoding(const D3DXVECTOR3& Left, const D3DXVECTOR3& Up, const D3DXVECTOR3& Right);			// ���_���̐ݒ�(���ߑł�)
	void SetVtxColor(const D3DXCOLOR& col);					// ���_�J���[�̐ݒ菈��
	void SetVtxTexture(void);								// �e�N�X�`�����W�̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetPos(const D3DXVECTOR3& pos);	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) const;			// �ʒu�擾����

	void SetPosOld(const D3DXVECTOR3& posOld);	// �O��̈ʒu�ݒ菈��
	D3DXVECTOR3 GetPosOld(void) const;			// �O��̈ʒu�擾����

	void SetRot(const D3DXVECTOR3& rot);	// �����ݒ菈��
	D3DXVECTOR3 GetRot(void) const;			// �����擾����

	void SetSize(const D3DXVECTOR3& size);	// �T�C�Y�ݒ菈��
	D3DXVECTOR3 GetSize(void) const;		// �T�C�Y�擾����

	D3DXMATRIX GetMatrix(void) const;		// �}�g���b�N�X�̎擾����

	int GetTexIdx(void) const;				// �e�N�X�`���̃C���f�b�N�X�̎擾����

	// �ÓI�����o�֐�
	static CObjectTri3D* Create(const TYPE type, const PRIORITY priority);		// ��������

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posOld;						// �O��̈ʒu
	D3DXVECTOR3 m_rot;							// ����
	D3DXVECTOR3 m_size;							// �T�C�Y
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�̃|�C���^
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	int m_nTexIdx;								// �e�N�X�`���̃C���f�b�N�X
};

#endif