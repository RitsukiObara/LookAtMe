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

//-----------------------------------
// �N���X��`(�u���b�N)
//-----------------------------------
class CBlock : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	//-----------------------------------
	// ���
	//-----------------------------------
	typedef enum
	{
		TYPE_DIRT = 0,		// �n�ʃu���b�N
		TYPE_ROCK,			// ��u���b�N
		TYPE_ICE,			// �X�u���b�N
		TYPE_MAGMA,			// �}�O�}�u���b�N
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	CBlock();															// �R���X�g���N�^
	CBlock(CObject::TYPE type, PRIORITY priority = PRIORITY_BLOCK);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CBlock();													// �f�X�g���N�^
	void Box(void);														// �R���X�g���N�^�̔�

	// �����o�֐�
	virtual HRESULT Init(void);		// ����������
	virtual void Uninit(void);		// �I������
	virtual void Update(void);		// �X�V����
	virtual void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const TYPE type, const bool bCliffR, const bool bCliffL);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetType(const TYPE type);			// ��ނ̐ݒ菈��
	TYPE GetType(void) const;				// ��ނ̎擾����

	void SetCliffRight(const bool bCliff);	// �E�R�̐ݒ菈��
	bool GetCliffRight(void) const;			// �E�R�̎擾����
	void SetCliffLeft(const bool bCliff);	// ���R�̐ݒ菈��
	bool GetCliffLeft(void) const;			// ���R�̎擾����

	D3DXMATERIAL GetMaterial(int nIdx)const;		// �}�e���A���̎擾����

// �f�o�b�O����
#ifdef _DEBUG

	bool GetDelete(void) const;			// �����󋵂̎擾����
	void SetDelete(const bool bDelete);	// �����󋵂̐ݒ菈��

#endif 

	// �ÓI�����o�֐�
	static CBlock* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const TYPE type, const bool bCliffR, const bool bCliffL);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	TYPE m_type;						// ���
	D3DXMATERIAL m_aMat[MAX_MATERIAL];	// �}�g���b�N�X�ւ̃|�C���^
	bool m_bCliffRight;					// �E�����R���܂�o���邩
	bool m_bCliffLeft;					// �������R���܂�o���邩

// �f�o�b�O����
#ifdef _DEBUG

	bool m_bDelete;			// ������
#endif 

};

#endif