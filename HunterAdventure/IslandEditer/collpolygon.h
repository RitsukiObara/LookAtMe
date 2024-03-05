//============================================
//
// �����蔻��|���S���w�b�_�[[collpolygon.h]
// Author�F��������
//
//============================================
#ifndef _COLLPOLYGON_H_		//���̃}�N����`������Ă��Ȃ�������
#define _COLLPOLYGON_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object3D.h"

//--------------------------------------------
// �N���X(�����蔻��N���X)
//--------------------------------------------
class CCollPolygon : public CObject3D
{
public:			// �N�ł��A�N�Z�X�ł���

	CCollPolygon();				// �R���X�g���N�^
	~CCollPolygon();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֐�
	D3DXVECTOR3 GetLeftUp(void) const;		// ������W�̎擾����
	D3DXVECTOR3 GetRightUp(void) const;		// �E����W�̎擾����
	D3DXVECTOR3 GetLeftDown(void) const;	// �������W�̎擾����
	D3DXVECTOR3 GetRightDown(void) const;	// �E�����W�̎擾����

	int GetNumID(void) const;				// �|���S���̔ԍ��̎擾����

	// �ÓI�����o�֐�
	static int Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin);	// ��������
	static void SetPosRot(const int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// �ʒu�ƌ����̐ݒ菈��
	static int Delete(const int nIdx);																	// �j������
	static CCollPolygon* GetCollPolygon(const int nIdx);												// �����蔻��|���S���̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXVECTOR3 m_vtxLeftUp;	// ����̍��W
	D3DXVECTOR3 m_vtxRightUp;	// �E��̍��W
	D3DXVECTOR3 m_vtxLeftDown;	// �����̍��W
	D3DXVECTOR3 m_vtxRightDown;	// �E���̍��W
	int m_nNumID;				// �|���S���̔ԍ�

	// �ÓI�����o�ϐ�
	static int m_nNumAll;		// �|���S���̑���
};

#endif