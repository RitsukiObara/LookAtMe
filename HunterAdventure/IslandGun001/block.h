//===================================
//
// �u���b�N�w�b�_�[[block.h]
// Author ��������
//
//===================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"
#include "area.h"

//-----------------------------------
// �N���X��`(�u���b�N)
//-----------------------------------
class CBlock : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CBlock();			// �R���X�g���N�^
	~CBlock();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBlock* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale);	// ��������

	// �Z�b�g�E�Q�b�g�֌W
	D3DXVECTOR3 GetVtxMax(void) const;		// ���_�̍ő�l�̎擾����
	D3DXVECTOR3 GetVtxMin(void) const;		// ���_�̍ŏ��l�̎擾����

	static CListManager<CBlock*> GetList(const int nIdx);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���
	
	// �����o�ϐ�
	D3DXVECTOR3 m_vtxMax;		// ���_�̍ő�l
	D3DXVECTOR3 m_vtxMin;		// ���_�̍ŏ��l
	int m_nFieldIdx;			// �敪�̔ԍ�

	// �ÓI�����o�ϐ�
	static CListManager<CBlock*> m_aList[area::NUM_AREA];		// ���X�g
};

#endif