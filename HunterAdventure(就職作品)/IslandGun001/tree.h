//===================================
//
// �؃w�b�_�[[tree.h]
// Author ��������
//
//===================================
#ifndef _TREE_H_
#define _TREE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(��)
//-----------------------------------
class CTree : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_PALM = 0,		// ���V�̖�
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	CTree(CObject::TYPE type = TYPE_TREE, PRIORITY priority = PRIORITY_BLOCK);				// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CTree();		// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	virtual void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);	// ���̐ݒ菈��
	virtual void Hit(void);					// �q�b�g����

	// �Z�b�g�E�Q�b�g�֌W
	TYPE GetType(void) const;				// ��ނ̎擾����

	// �ÓI�����o�֐�
	static CTree* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);	// ��������

	static CListManager<CTree*> GetList(void);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	TYPE m_type;		// ���
	
	// �ÓI�����o�ϐ�
	static CListManager<CTree*> m_list;		// ���X�g
};

#endif