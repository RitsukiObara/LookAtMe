//===================================
//
// ��w�b�_�[[rock.h]
// Author ��������
//
//===================================
#ifndef _ROCK_H_
#define _ROCK_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(��)
//-----------------------------------
class CRock : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_BROWN = 0,	// ���F��
		TYPE_GRAY,		// �D�F��
		TYPE_BREAK,		// �����
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	CRock();			// �R���X�g���N�^
	~CRock();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type);			// ���̐ݒ菈��
	void Break(void);				// �j�󏈗�

	// �Z�b�g�E�Q�b�g�֌W
	TYPE GetType(void) const;			// ��ނ̎擾����
	float GetRadius(void) const;		// ���a�̎擾����
	float GetTopHeight(void) const;		// ��̍����̎擾����
	float GetBottomHeight(void) const;	// ���̍����̎擾����

	// �ÓI�����o�֐�
	static CRock* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type);	// ��������

	static CListManager<CRock*> GetList(void);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���
	
	// �����o�ϐ�
	TYPE m_type;		// ���
	float m_fRadius;	// ���a
	float m_fTop;		// ��̍���
	float m_fBottom;	// ���̍���

	// �ÓI�����o�ϐ�
	static CListManager<CRock*> m_list;		// ���X�g
};

#endif